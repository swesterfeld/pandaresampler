#include "pandaresampler.hh"

#include <cstring>
#include <cassert>
#include <cmath>

using PandaResampler::Resampler2;

int
main (int argc, char **argv)
{
  bool up = strcmp (argv[1], "up") == 0;
  bool down = strcmp (argv[1], "down") == 0;
  assert (up || down);

  const int ratio = atoi (argv[2]);

  Resampler2::Precision prec = Resampler2::find_precision_for_bits (atoi (argv[3]));

  bool fir = strcmp (argv[4], "fir") == 0;
  bool iir = strcmp (argv[4], "iir") == 0;
  bool iir_sse = strcmp (argv[4], "iir-sse") == 0;
  bool sse = fir || iir_sse;

  assert (fir || iir || iir_sse);

  Resampler2 res (up ? Resampler2::UP : Resampler2::DOWN, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);

  const double rate = 44100;
  const double new_rate = rate * (up ? ratio : 1. / ratio);

  constexpr int SAMPLES = 1024;

  alignas(16) float in[SAMPLES] = { 0, };
  alignas(16) float out[SAMPLES * ratio] = { 0, };

  for (int i = 0; i < SAMPLES; i++)
    in[i] = sin (i * 1000. / rate * 2 * M_PI);
  res.process_block (in, SAMPLES, out);

  double delay = res.delay();

  printf ("# new_rate = %f delay = %f\n", new_rate, delay);

  for (int i = 0; i < (up ? (SAMPLES * ratio) : (SAMPLES / ratio)); i++)
    {
      double pos = i - delay;
      double s = 0;

      if (pos >= 0)
        s = sin (pos * 1000. / new_rate * 2 * M_PI);

      printf ("%d %f %f\n", i, out[i], s);
    }
}
