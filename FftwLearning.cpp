#include "FftwLearning.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include <vector>
#include <complex>
#include "SpectrumAnalyzer.h"
#include <iostream>

FftwLearning::FftwLearning(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new ITextControl(b.GetMidVPadded(50), "Hello iPlug 2!", IText(50)));
    pGraphics->AttachControl(new IVKnobControl(b.GetCentredInside(100).GetVShifted(-100), kGain));
  };
#endif

}

#if IPLUG_DSP
void FftwLearning::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{

  const double gain = GetParam(kGain)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  std::vector<std::vector<std::complex<double>>> spectrum;
  
  for (int channel = 0; channel < nChans; channel++) {
    auto& analyzer = spectrumAnalyzers[channel];
    
    for (int sample = 0; sample < nFrames; sample++) {
      outputs[channel][sample] = inputs[channel][sample] * gain;
      
      // Fill fft input
      analyzer.push_sample(inputs[channel][sample]);
    }
  }
}

void FftwLearning::OnIdle() {
 
  for(int i = 0; i < 2; i++) {

    // Process data from spectrum analyzer
    spectrumAnalyzers[i].processPendingData([](SpectrumData spectrumData) {
      // Do something with the result
      vector<double> spectrumMagnitudes;
      for(int i = 0; i < spectrumData.size(); i++) {
        auto bin = spectrumData[i];
        
        spectrumMagnitudes.push_back(std::abs(bin));
      }
      // Print a bin to output just for debugging purposes
      //std::cout << spectrumMagnitudes[0] << "\t";
      //std::cout << spectrumMagnitudes[spectrumMagnitudes.size()-1] << std::endl;
      }
    );
    
  }
  
}

#endif
