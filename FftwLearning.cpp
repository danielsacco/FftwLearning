#include "FftwLearning.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include <fftw3.h>
#include <vector>
#include <complex>
#include "SpectrumAnalyzer.h"

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

std::vector<double> vectorView(double* data, size_t length) {
  std::vector<double> view(length);
  for(int i = 0; i < length; i++) {
    view[i] = data[i];
  }
  return view;
}

std::vector<std::complex<double>> vectorView(fftw_complex* data, size_t length) {
  std::vector<std::complex<double>> view(length);
  for(int i = 0; i < length; i++) {
    view[i] = std::complex{data[i][0], data[i][1]};
  }
  return view;
}

#if IPLUG_DSP
void FftwLearning::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{

  //double *fftInput;
  //fftw_complex *fftResult;
  //fftw_plan fftPlan;
  //size_t fftOutputSize = nFrames / 2 + 1;
  
  if(fftInput == nullptr || fftResult == nullptr || fftPlan == nullptr) {
    fftInput = (double*) fftw_malloc(sizeof(double) * nFrames);
    fftResult = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftOutputSize);
    fftPlan = fftw_plan_dft_r2c_1d(nFrames, fftInput, fftResult, FFTW_ESTIMATE);
  }
  
  const double gain = GetParam(kGain)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  std::vector<std::vector<std::complex<double>>> spectrum;
  
  for (int channel = 0; channel < nChans; channel++) {
    for (int sample = 0; sample < nFrames; sample++) {
      outputs[channel][sample] = inputs[channel][sample] * gain;
      
      // Fill fft input
      fftInput[sample] = inputs[channel][sample];
      
      
    }
    fftw_execute(fftPlan);
    
    // TODO Use output for something useful :)
    spectrum.push_back(vectorView(fftResult, fftOutputSize));
  }
  
  
  //fftw_destroy_plan(fftPlan);
  //fftw_free(fftInput);
  //fftw_free(fftResult);
}
#endif
