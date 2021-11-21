// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "hiir/PolyphaseIir2Designer.cpp"

using hiir::PolyphaseIir2Designer;

double
mk_group_delay (int stage, const double *coeffs, uint n_coeffs)
{
  const double rate = 44100. * stage;

  return hiir::PolyphaseIir2Designer::compute_group_delay (coeffs, n_coeffs, 1000 / rate, false);
}

void
test_group_delay()
{
  /* test group delay computation against well known values from hiir/oversampling.txt */
  const int n_coeffs = 8;
  const double coeffs[n_coeffs] = { // 2x stage
    0.029771566661791642,
    0.11293802507590323,
    0.23389457050522444,
    0.37412196640361606,
    0.51845353417401152,
    0.65849242953158127,
    0.79323734846738669,
    0.92851085864352823
  };
  printf ("%f expect 4\n", mk_group_delay (2, coeffs, n_coeffs));

  const int n_coeffs4 = 4;
  const double coeffs4[n_coeffs4] = { // 4x stage
    0.039003493679774473,
    0.15935669751127579,
    0.3760341917496039,
    0.73417946269919343
  };
  printf ("%f expect 2.758\n", mk_group_delay (4, coeffs4, n_coeffs4));

  const int n_coeffs8 = 2;
  const double coeffs8[n_coeffs8] = { // 8x stage
    0.11116740519894572,
    0.53837812120804285
  };
  printf ("%f expect 1.6\n", mk_group_delay (8, coeffs8, n_coeffs8));
}

int
main()
{
  if (0)
    {
      test_group_delay();
      return 0;
    }

  for (auto bits : { 8, 12, 16, 20, 24 })
    {
      for (auto stage : { 2, 4, 8 })
        {
          int prec = bits * 6;

          const double passband_end = 18000;
          const double rate = 44100 * stage / 2;
          const double tbw = (rate / 2 - passband_end) / rate;

          int n_coeffs = PolyphaseIir2Designer::compute_nbr_coefs_from_proto (bits * 6, tbw);

          printf ("  constexpr std::array<double,%d> coeffs%d_%d = {\n", n_coeffs, stage, bits);
          double coeffs[n_coeffs];
          hiir::PolyphaseIir2Designer::compute_coefs_spec_order_tbw (coeffs, n_coeffs, tbw);
          for (int i = 0; i < n_coeffs; i++)
            printf ("    %.17g,\n", coeffs[i]);
          printf ("  };\n");

          double gdelay = mk_group_delay (stage, coeffs, n_coeffs);

          printf ("  if (stage_ratio == %d && precision_ == PREC_%dDB)\n", stage, prec);
          printf ("    return create_impl_iir_with_coeffs (coeffs%d_%d, %f);\n", stage, bits, gdelay);
          printf ("\n");
        }
    }
}
