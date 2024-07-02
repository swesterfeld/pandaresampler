// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#include "pandaresampler.hh"

#include <vector>

using PandaResampler::Resampler2;
using std::vector;

int
main()
{
  for (uint M = 0; M < 100; M++)
    {
      for (auto over : { 1, 2, 4, 8 })
        {
          vector<float> in (M), out (M * over);

          for (auto p : { 1, 8, 12, 16, 20, 24 })
            {
              Resampler2 ups (Resampler2::UP, over, Resampler2::find_precision_for_bits (p), true);
              Resampler2 downs (Resampler2::DOWN, over, Resampler2::find_precision_for_bits (p), true);

              /* do this to check we don't access areas of the stack after the end of the input
               * need to use --enable-asan to trigger it
               */
              ups.process_block (in.data(), M, out.data());

              /* this doesn't help for post-end validation due to internal buffers, but do it anyway
               * to maybe catch other problems
               */
              downs.process_block (out.data(), M * over, in.data());
            }
        }
    }

  return 0;
}
