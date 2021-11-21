// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"

#include <cstring>
#include <cassert>
#include <cmath>

using PandaResampler::Resampler2;

int
main (int argc, char **argv)
{
  const bool up = strcmp (argv[1], "up") == 0;
  const bool down = strcmp (argv[1], "down") == 0;
  const bool over = strcmp (argv[1], "over") == 0;
  assert (up || down || over);

  const int ratio = atoi (argv[2]);

  Resampler2::Precision prec = Resampler2::find_precision_for_bits (atoi (argv[3]));

  bool fir = strcmp (argv[4], "fir") == 0;
  bool iir = strcmp (argv[4], "iir") == 0;
  bool iir_sse = strcmp (argv[4], "iir-sse") == 0;
  bool sse = fir || iir_sse;

  assert (fir || iir || iir_sse);

  Resampler2 ups (Resampler2::UP, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);
  Resampler2 downs (Resampler2::DOWN, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);

  const double rate = 44100;
  const double freq = 1000;
  constexpr int SAMPLES = 1024;

  alignas(16) float in[SAMPLES] = { 0, };
  alignas(16) float out[SAMPLES * ratio] = { 0, };

  for (int i = 0; i < SAMPLES; i++)
    in[i] = sin (i * freq / rate * 2 * M_PI);

  double new_rate;
  int out_samples;
  double delay;

  if (up)
    {
      out_samples = SAMPLES * ratio;
      new_rate = rate * ratio;

      ups.process_block (in, SAMPLES, out);
      delay = ups.delay();
    }
  else if (down)
    {
      out_samples = SAMPLES / ratio;
      new_rate = rate / ratio;

      downs.process_block (in, SAMPLES, out);
      delay = downs.delay();
    }
  else // over
    {
      out_samples = SAMPLES;
      delay = ups.delay() / ratio + downs.delay();
      new_rate = rate;

      alignas(16) float tmp[SAMPLES * ratio] = { 0, };
      ups.process_block (in, SAMPLES, tmp);
      downs.process_block (tmp, SAMPLES * ratio, out);
    }

  printf ("# new_rate = %f delay = %f\n", new_rate, delay);

  for (int i = 0; i < out_samples; i++)
    {
      double pos = i - delay;
      double s = 0;

      if (pos >= 0)
        s = sin (pos * freq / new_rate * 2 * M_PI);

      printf ("%d %.10f %.10f\n", i, out[i], s);
    }
}
