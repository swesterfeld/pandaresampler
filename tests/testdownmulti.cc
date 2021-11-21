// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"
#include <math.h>
#include <fftw3.h>
#include <assert.h>
#include <string.h>

using PandaResampler::Resampler2;
using std::vector;

inline double
window_blackman (double x)
{
  if (fabs (x) > 1)
    return 0;
  return 0.42 + 0.5 * cos (M_PI * x) + 0.08 * cos (2.0 * M_PI * x);
}

void
fft (const uint n_values, float *r_values_in, float *ri_values_out)
{
  auto plan_fft = fftwf_plan_dft_r2c_1d (n_values, r_values_in, (fftwf_complex *) ri_values_out, FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);

  fftwf_execute_dft_r2c (plan_fft, r_values_in, (fftwf_complex *) ri_values_out);

  // usually we should keep the plan, but for this simple test program, the fft
  // is only computed once, so we can destroy the plan here
  fftwf_destroy_plan (plan_fft);
}

double
db (double x)
{
  return 20 * log10 (std::max (x, 0.00000001));
}

void
check_amps (int ds, double freq, Resampler2::Precision prec, bool fir)
{
  const int FFT_SIZE = 8192;
  vector<float> in (FFT_SIZE * ds);
  vector<float> out (FFT_SIZE);

  double normalize = 0;
  for (size_t i = 0; i < in.size(); i++)
    {
      double w = window_blackman ((2.0 * i - FFT_SIZE) / FFT_SIZE);
      normalize += w;
      in[i] = sin (i * freq / (44100 * ds) * 2 * M_PI) * w;
    }
  normalize /= 2 * ds;

  Resampler2 downs (Resampler2::DOWN, ds, prec, false, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);
  downs.process_block (&in[0], in.size(), &out[0]);

  vector<float> fft_out (FFT_SIZE + 2 /* fftw extra complex output value */);
  fft (FFT_SIZE, &out[0], &fft_out[0]);

  /*
  for (size_t i = 0; i < in.size(); i++)
    printf ("%zd %.17g #IN\n", i, in[i]);

  for (size_t i = 0; i < out.size(); i++)
    printf ("%zd %.17g #OUT\n", i, out[i]);
  */

  float passband_amp = 0;
  float transband_amp = 0;
  for (size_t i = 0; i < fft_out.size(); i += 2)
    {
      float freq = (i / 2) / double (FFT_SIZE) * 44100;
      float amp = sqrt (fft_out[i] * fft_out[i] + fft_out[i+1] * fft_out[i+1]) / normalize;

      if (freq < 18000)
        passband_amp = std::max (passband_amp, amp);
      else
        transband_amp = std::max (transband_amp, amp);
      //printf ("%f %.17g #FFT\n", freq, amp);
    }
  printf ("%f %.17g %.17g # PASS/TRANS\n", freq, db (passband_amp), db (transband_amp));
}

int
main (int argc, char **argv)
{
  const int ds = atoi (argv[1]);

  Resampler2::Precision prec = Resampler2::find_precision_for_bits (atoi (argv[2]));

  bool fir = strcmp (argv[3], "fir") == 0;
  bool iir = strcmp (argv[3], "iir") == 0;
  assert (fir || iir);

  for (float freq = 100; freq < 22050 * ds; freq += 100)
    check_amps (ds, freq, prec, fir);

  return 0;
}
