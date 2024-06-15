// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"
#include <zita-resampler/resampler.h>

#include <fftw3.h>

#include <cmath>
#include <cassert>
#include <string>

using PandaResampler::Resampler2;
using std::vector;
using std::string;

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
zita_resample (float *in, size_t n_in_samples, int from, int to, float *out, size_t n_out_samples)
{
  Resampler resampler;
  const int hlen = 32;
  assert (resampler.setup (from, to, 1, hlen) == 0);

  resampler.out_count = n_out_samples / resampler.nchan();
  resampler.out_data = out;

  /* avoid timeshift: zita needs k/2 - 1 samples before the actual input */
  resampler.inp_count = resampler.inpsize () / 2 - 1;
  resampler.inp_data  = nullptr;
  resampler.process();

  resampler.inp_count = n_in_samples / resampler.nchan();
  resampler.inp_data = (float *) &in[0];
  resampler.process();

  /* zita needs k/2 samples after the actual input */
  resampler.inp_count = resampler.inpsize() / 2;
  resampler.inp_data  = nullptr;
  resampler.process();
}

class Saturation
{
  static constexpr int OVERSAMPLE = 8;
  static constexpr auto PREC      = Resampler2::PREC_96DB;
  static constexpr float DRIVE    = 3;

  Resampler2 ups { Resampler2::UP, OVERSAMPLE, PREC };
  Resampler2 downs { Resampler2::DOWN, OVERSAMPLE, PREC };
public:
  void
  reset()
  {
    ups.reset();
    downs.reset();
  }
  void
  zprocess (const float *in, size_t n_samples, float *out)
  {
    float tmp[n_samples * OVERSAMPLE];

    zita_resample ((float *) in, n_samples, 48000, 48000 * OVERSAMPLE, tmp, n_samples * OVERSAMPLE);

    for (size_t i = 0; i < n_samples * OVERSAMPLE; i++)
      tmp[i] = std::tanh (tmp[i] * DRIVE);

    zita_resample (tmp, n_samples * OVERSAMPLE, 48000 * OVERSAMPLE, 48000, out, n_samples);
  }
  void
  process (const float *in, size_t n_samples, float *out)
  {
    float tmp[n_samples * OVERSAMPLE];

    ups.process_block (in, n_samples, tmp);
    for (size_t i = 0; i < n_samples * OVERSAMPLE; i++)
      tmp[i] = std::tanh (tmp[i] * DRIVE);
    downs.process_block (tmp, n_samples * OVERSAMPLE, out);
  }
};

int
main (int argc, char **argv)
{
  string mode;
  if (argc == 3)
    mode = argv[2];
  if (mode != "zita" && mode != "panda")
    {
      printf ("usage: testsawquality <freq> <mode>\n");
      return 1;
    }
  const int FFT_SIZE = 8192;

  vector<float> in (FFT_SIZE);
  vector<float> out (FFT_SIZE);

  const float freq = atof (argv[1]);
  for (size_t i = 0; i < in.size(); i++)
    for (int p = 1; p * freq < 24000; p++)
      in[i] += sin (freq * p * i / 48000 * 2 * M_PI) / p;

  Saturation saturation;
  if (mode == "zita")
    saturation.zprocess (in.data(), in.size(), out.data());
  else
    saturation.process (in.data(), in.size(), out.data());

  double normalize = 0;
  for (size_t i = 0; i < in.size(); i++)
    {
      double w = window_blackman ((2.0 * i - FFT_SIZE) / FFT_SIZE);
      normalize += w;
      out[i] *= w;
    }
  normalize /= 2;

  vector<float> fft_out (FFT_SIZE + 2 /* fftw extra complex output value */);
  fft (FFT_SIZE, &out[0], &fft_out[0]);
  for (size_t i = 0; i < fft_out.size(); i += 2)
    {
      float freq = (i / 2) / double (FFT_SIZE) * 48000;
      float amp = sqrt (fft_out[i] * fft_out[i] + fft_out[i+1] * fft_out[i+1]) / normalize;
      printf ("%f %.17g\n", freq, db (amp));
    }
}
