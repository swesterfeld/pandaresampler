#!/bin/bash

function mkfir
{
  local stage="$1"
  local bits="$2"
  local n_coefficients="$3"
  local xmu="0.75" # constant: can be used for all filters
  local latt="$4"

  # design filter using ultraspherical window and sinc
  octave <(
    echo "pkg load signal;"
    echo "rate=$stage/2*44100;"
    echo "c=us_sinc($n_coefficients,$xmu,$latt,rate)';"
    echo "save mkfir_${stage}_${bits}.tmp c;"
  )
  mv us_sinc.dump mkfir_${stage}_${bits}.dump

  # generate C++ source to create filter with coefficients
  echo "  static constexpr double coeffs${stage}_${bits}[$n_coefficients] ="
  echo "  {";
  cat mkfir_${stage}_${bits}.tmp | awk '$1 != "#" && NF > 0 { if (n++ % 2 == 1) print "    "$1","; }'
  echo "  };";
  echo "  if (stage_ratio == $stage && precision_ == $bits && mode_ == UP)"
  echo "    return create_impl_with_coeffs <Upsampler2<$n_coefficients, USE_SSE> > (coeffs${stage}_${bits}, $n_coefficients, 2.0);"
  echo "  if (stage_ratio == $stage && precision_ == $bits && mode_ == DOWN)"
  echo "    return create_impl_with_coeffs <Downsampler2<$n_coefficients, USE_SSE> > (coeffs${stage}_${bits}, $n_coefficients, 1.0);"

  # create gnuplottable output
  cat mkfir_${stage}_${bits}.dump | awk '$1 != "#" && NF > 0 {
      x = $2 > 0 ? $2 : -$2;
      print $1, 20*log(x)/log(10), $1 < '$stage' / 2 * 44100 - 18000 ? 0 : -'$bits' * 6
    }' > mkfir_${stage}_${bits}.gp
  rm mkfir_${stage}_${bits}.tmp
}

{

  mkfir 2 24 52 138
  mkfir 4 24 16 136
  mkfir 8 24 12 136

  mkfir 2 20 42 113.75
  mkfir 4 20 14 113.75
  mkfir 8 20 10 113.75

  mkfir 2 16 32 88.5
  mkfir 4 16 10 86.5
  mkfir 8 16 8 88.5

  mkfir 2 12 24 67.5
  mkfir 4 12 8 67.5
  mkfir 8 12 6 67.5

  mkfir 2 8 16 48
  mkfir 4 8 6 46
  mkfir 8 8 4 42

} > mkfir.gen.cc
