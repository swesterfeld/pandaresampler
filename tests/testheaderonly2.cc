#define PANDA_RESAMPLER_HEADER_ONLY

#include "pandaresampler.hh"
#include <math.h>

using PandaResampler::Resampler2;
using std::vector;

int fn()
{
  Resampler2 ups (Resampler2::UP, Resampler2::PREC_96DB, true);

  return 0;
}
