// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"

using PandaResampler::Resampler2;

int main (int argc, char **argv)
{
  for (uint M = 0; M < 100; M++)
    {
      for (auto o : {1, 2, 4, 8, 16 })
        {
          for (auto p : {1, 8, 12, 16, 20, 24})
            {
              Resampler2 ups (Resampler2::UP, o, Resampler2::find_precision_for_bits (p), true);

              /* do this to check we don't access areas of the stack after the end of the input
               * need to use --enable-asan to trigger it
               */
              float in[M], out[M * o];
              ups.process_block (&in[0], M, &out[0]);
            }
        }
    }

  return 0;
}
