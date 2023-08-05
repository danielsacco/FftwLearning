#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include <array>
#include "SpectrumAnalyzer.h"
#include "ISender.h"
#include "IVSpectrumControl.h"

using namespace des;

const int kNumPresets = 1;

enum EParams
{
  kGain = 0,
  kNumParams
};

enum EControlTags
{
  kCtrlTagSpectrum = 0
};

using namespace iplug;
using namespace igraphics;

class FftwLearning final : public Plugin
{
public:
  FftwLearning(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnIdle() override;
  
private:
  
  // TODO Horrible: Assuming stereo
  const int fftSize = 256;
  const int hopSize = fftSize / 4;
  std::array<SpectrumAnalyzer, 2> spectrumAnalyzers {
    SpectrumAnalyzer{fftSize, hopSize},
    SpectrumAnalyzer{fftSize, hopSize}
  };

  iplug::ISender<1, 64, SpectrumData> spectrumDataSender;
  
#endif
  
};
