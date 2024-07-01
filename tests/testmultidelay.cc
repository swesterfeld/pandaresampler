// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"

#include <cstring>
#include <cassert>
#include <cmath>

using PandaResampler::Resampler2;

using std::max;

enum Verbose {
  VERBOSE,
  SILENT
};

struct Test
{
  bool up = false;
  bool down = false;
  bool over = false;
  int ratio = 2;
  Resampler2::Precision prec = Resampler2::PREC_96DB;
  bool fir = false;
  bool iir = false;
  bool iir_sse = false;
  double run (Verbose verbose);
};

double
Test::run (Verbose verbose)
{
  assert (up || down || over);
  assert (fir || iir || iir_sse);

  bool sse = fir || iir_sse;

  Resampler2 ups (Resampler2::UP, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);
  Resampler2 downs (Resampler2::DOWN, ratio, prec, sse, fir ? Resampler2::FILTER_FIR : Resampler2::FILTER_IIR);

  const double rate = 44100;
  const double freq = 1000;
  constexpr int SAMPLES = 1024;
  constexpr int MAX_RATIO = 8;
  assert (ratio <= MAX_RATIO);

  alignas(16) float in[SAMPLES] = { 0, };
  alignas(16) float out[SAMPLES * MAX_RATIO] = { 0, };

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

      alignas(16) float tmp[SAMPLES * MAX_RATIO] = { 0, };
      ups.process_block (in, SAMPLES, tmp);
      downs.process_block (tmp, SAMPLES * ratio, out);
    }

  printf ("# new_rate = %f delay = %f\n", new_rate, delay);

  double error = 0;
  for (int i = 0; i < out_samples; i++)
    {
      double pos = i - delay;
      double s = 0;

      if (pos >= 0)
        s = sin (pos * freq / new_rate * 2 * M_PI);
      if (pos > delay)
        error = max (error, s - out[i]);

      if (verbose == VERBOSE)
        printf ("%d %.10f %.10f\n", i, out[i], s);
    }
  const double max_error = 0.00005;
  printf ("# error=%f (bound %f)\n", error, max_error);
  return error;
}

int
main (int argc, char **argv)
{
  if (argc == 1)
    {
      for (int udo : { 1, 2, 3 })
        {
          Test test;
          test.up   = udo == 1;
          test.down = udo == 2;
          test.over = udo == 3;
          test.fir = true;

          for (int ratio : { 2, 4, 8 })
            {
              for (auto prec : { Resampler2::PREC_96DB, Resampler2::PREC_120DB, Resampler2::PREC_144DB })
                {
                  test.ratio = ratio;
                  test.prec = prec;
                  test.run (SILENT);
                }
            }
        }
    }
  else
    {
      Test test;

      test.up = strcmp (argv[1], "up") == 0;
      test.down = strcmp (argv[1], "down") == 0;
      test.over = strcmp (argv[1], "over") == 0;
      test.ratio = atoi (argv[2]);
      test.prec = Resampler2::find_precision_for_bits (atoi (argv[3]));

      test.fir = strcmp (argv[4], "fir") == 0;
      test.iir = strcmp (argv[4], "iir") == 0;
      test.iir_sse = strcmp (argv[4], "iir-sse") == 0;

      test.run (VERBOSE);
    }
}
