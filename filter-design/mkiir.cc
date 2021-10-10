#include "hiir/PolyphaseIir2Designer.cpp"

using hiir::PolyphaseIir2Designer;

int
main()
{
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

          printf ("  if (stage_ratio == %d && precision_ == PREC_%dDB)\n", stage, prec);
          printf ("    return create_impl_iir_with_coeffs (coeffs%d_%d);\n", stage, bits);
          printf ("\n");
        }
    }
}
