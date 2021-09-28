#!/bin/bash

function mkfir
{
  local stage="$1"
  local bits="$2"
  local n_coefficients="$3"
  local xmu="$4"
  local latt="$5"

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
  mkfir 2 16 32 0.75 88.5
  mkfir 4 16 16 0.75 88.5
  mkfir 8 16 8 0.75 88.5
} > mkfir.gen.cc
