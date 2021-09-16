#include "pandaresampler.hh"
#include <math.h>
#include <assert.h>

using PandaResampler::Resampler2;
using std::vector;

int
main (int argc, char **argv)
{
  int rate = 48000;
  int sec = 5;
  vector<float> in (rate * sec), freq (in.size());

  double phase = 0;
  for (size_t i = 0; i < in.size(); i++)
    {
      freq[i] = double (rate) * i / in.size();
      phase += freq[i] / rate * M_PI;

      in[i] = sin (phase);
    }
  int over = atoi (argv[1]), stages;
  if (over == 0)
    {
      for (size_t i = 0; i < in.size(); i++)
        {
          printf ("%f\n", in[i]);
        }
      return 0;
    }
  switch (over)
    {
      case 1: stages = 0;
              break;
      case 2: stages = 1;
              break;
      case 4: stages = 2;
              break;
      case 8: stages = 3;
              break;
      default: assert (false);
    }
  for (int s = 0; s < stages; s++)
    {
      Resampler2 ups (Resampler2::UP, Resampler2::PREC_96DB);
      vector<float> out (in.size() * 2);
      ups.process_block (&in[0], in.size(), &out[0]);
      in = out;
    }
  for (size_t i = 0; i < in.size(); i++)
    {
      in[i] = atan (in[i] * 3) / M_PI * 2;
    }
  for (int s = 0; s < stages; s++)
    {
      Resampler2 downs (Resampler2::DOWN, Resampler2::PREC_96DB);
      vector<float> out (in.size() / 2);
      downs.process_block (&in[0], in.size(), &out[0]);
      in = out;
    }
  for (size_t i = 0; i < in.size(); i++)
    {
      printf ("%f\n", in[i]);
    }
}
