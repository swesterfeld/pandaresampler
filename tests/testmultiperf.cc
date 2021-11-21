// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"

#include <cstring>
#include <cassert>

#include <sys/time.h>

using PandaResampler::Resampler2;

static double
gettime ()
{
  timeval tv;
  gettimeofday (&tv, 0);

  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int
main (int argc, char **argv)
{
  bool up = strcmp (argv[1], "up") == 0;
  bool down = strcmp (argv[1], "down") == 0;
  const bool over = strcmp (argv[1], "over") == 0;
  assert (up || down || over);

  if (over)
    {
      up = true;
      down = true;
    }

  const int ratio = atoi (argv[2]);

  Resampler2::Precision prec = Resampler2::find_precision_for_bits (atoi (argv[3]));

  bool fir = strcmp (argv[4], "fir") == 0;
  bool iir = strcmp (argv[4], "iir") == 0;
  bool iir_sse = strcmp (argv[4], "iir-sse") == 0;
  bool sse = fir || iir_sse;

  assert (fir || iir || iir_sse);

  Resampler2 ups (Resampler2::UP, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);
  Resampler2 downs (Resampler2::DOWN, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);

  constexpr int SAMPLES = 128;

  alignas(16) float in[SAMPLES] = { 0, };
  alignas(16) float out[SAMPLES * ratio] = { 0, };

  double t = gettime();
  double samples = 0;

  for (int i = 0; i < 1000000; i++)
    {
      if (up)
        ups.process_block (in, SAMPLES, out);

      if (down)
        downs.process_block (out, SAMPLES * ratio, in);
      samples += SAMPLES;
    }

  printf ("%f ns / sample\n", (gettime() - t) / samples * 1000000000);
}
