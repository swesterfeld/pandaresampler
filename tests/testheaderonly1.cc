// This Source Code Form is licensed MPL-2.0: http://mozilla.org/MPL/2.0
#define PANDA_RESAMPLER_HEADER_ONLY

#include "pandaresampler.hh"
#include <math.h>

using PandaResampler::Resampler2;
using std::vector;

int main()
{
  Resampler2 ups (Resampler2::UP, 2, Resampler2::PREC_96DB, true);

  return 0;
}
