// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"
#include <math.h>
#include <assert.h>
#include <cstring>
#include <zita-resampler/resampler.h>

using PandaResampler::Resampler2;
using std::vector;

void
zita_resample (vector<float>& in, int from, int to, vector<float>& out)
{
  Resampler resampler;
  const int hlen = 32;
  assert (resampler.setup (from, to, 1, hlen) == 0);

  resampler.out_count = out.size() / resampler.nchan();
  resampler.out_data = &out[0];

  /* avoid timeshift: zita needs k/2 - 1 samples before the actual input */
  resampler.inp_count = resampler.inpsize () / 2 - 1;
  resampler.inp_data  = nullptr;
  resampler.process();

  resampler.inp_count = in.size() / resampler.nchan();
  resampler.inp_data = (float *) &in[0];
  resampler.process();

  /* zita needs k/2 samples after the actual input */
  resampler.inp_count = resampler.inpsize() / 2;
  resampler.inp_data  = nullptr;
  resampler.process();
}

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
  int over = atoi (argv[1]);
  bool zita = strcmp (argv[2], "zita") == 0;
  bool panda_fir = strcmp (argv[2], "fir") == 0;
  bool panda_iir = strcmp (argv[2], "iir") == 0;
  assert (zita || panda_fir || panda_iir);
  bool use_sse_if_available;
  Resampler2::Filter filter;
  if (panda_iir)
    {
      filter  = Resampler2::FILTER_IIR;
      use_sse_if_available = false;
    }
  if (panda_fir)
    {
      filter  = Resampler2::FILTER_FIR;
      use_sse_if_available = true;
    }
  if (over == 0)
    {
      for (size_t i = 0; i < in.size(); i++)
        {
          printf ("%f\n", in[i]);
        }
      return 0;
    }
  if (panda_fir || panda_iir)
    {
      Resampler2 ups (Resampler2::UP, over, Resampler2::PREC_96DB, use_sse_if_available, filter);
      vector<float> out (in.size() * over);
      ups.process_block (&in[0], in.size(), &out[0]);
      in = out;
    }
  else
    {
      vector<float> out;
      out.resize (in.size() * over);
      zita_resample (in, 48000, 48000 * over, out);
      in = out;
    }
  for (size_t i = 0; i < in.size(); i++)
    {
      in[i] = atan (in[i] * 3) / M_PI * 2;
    }
  if (panda_fir || panda_iir)
    {
      Resampler2 downs (Resampler2::DOWN, over, Resampler2::PREC_96DB, use_sse_if_available, filter);
      vector<float> out (in.size() / over);
      downs.process_block (&in[0], in.size(), &out[0]);
      in = out;
    }
  else
    {
      vector<float> out;
      out.resize (in.size() / over);
      zita_resample (in, 48000 * over, 48000, out);
      in = out;
    }
  float in_max = 0;
  for (size_t i = 0; i < in.size(); i++)
    {
      in_max = std::max (fabs (in[i]), in_max);
    }
  for (size_t i = 0; i < in.size(); i++)
    {
      printf ("%f\n", in[i] / in_max);
    }
}
