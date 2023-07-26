#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include <fftw3.h>

const int kNumPresets = 1;

enum EParams
{
  kGain = 0,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class FftwLearning final : public Plugin
{
public:
  FftwLearning(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  
private:
  
  const size_t fftInputSize = 1024;
  const size_t fftOutputSize = fftInputSize / 2 + 1;
  double *fftInput;
  fftw_complex *fftResult;
  fftw_plan fftPlan;

#endif
};
