// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"
#include <math.h>

using PandaResampler::Resampler2;
using std::vector;

int main()
{
  vector<float> in (256);
  vector<float> out (512);

  for (size_t i = 0; i < in.size(); i++)
    in[i] = sin (440.0 * i / 48000 * 2 * M_PI);

  Resampler2 ups (Resampler2::UP, 2, Resampler2::PREC_96DB, true);

  ups.process_block (&in[0], in.size(), &out[0]);
  for (size_t i = 0; i < in.size(); i++)
    printf ("%zd %f #IN\n", i, in[i]);

  for (size_t i = 0; i < out.size(); i++)
    printf ("%zd %f #OUT\n", i, out[i]);

  return 0;
}
