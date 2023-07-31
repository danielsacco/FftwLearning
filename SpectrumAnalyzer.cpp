#include "SpectrumAnalyzer.h"

namespace des {

SpectrumAnalyzer::SpectrumAnalyzer(int fftSize, int hopSize) :
fftInputSize(fftSize),
hopSize(hopSize),
hopCounter(hopSize),
window(WindowFunction(fftSize)),
inputBuffer(fftSize) {
  
  fftInput = (double*) fftw_malloc(sizeof(double) * fftInputSize);
  fftResult = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftOutputSize);
  fftPlan = fftw_plan_dft_r2c_1d(fftInputSize, fftInput, fftResult, FFTW_ESTIMATE);
}

SpectrumAnalyzer::~SpectrumAnalyzer() {
  fftw_destroy_plan(fftPlan);
  fftw_free(fftInput);
  fftw_free(fftResult);
}

void SpectrumAnalyzer::push_sample(double sample) {
  
  inputBuffer.push_back(sample);
  
  hopCounter--;
  if (hopCounter == 0) {
    // We reached the hop size. Enqueue the data snapshot for spectrum calculation
    vector<double> dataBlock(inputBuffer.begin(), inputBuffer.end());
    safelyPushBlock(dataBlock);
    hopCounter = hopSize;

    // Discard eldest elements from queue if they where not consumed
    // TODO Make pending queue limit configurable
    while (pendingBlocks.size() > 50) {
      safelyPopBlock();
    }
    
  }
}

void SpectrumAnalyzer::processPendingData(std::function<void(SpectrumData)> spectrumDataReceiver) {

  while (! pendingBlocks.empty()) {
    
    auto& block = pendingBlocks.front();
    
    // TODO Any elegant way of doing this ?
    for(int i = 0; i < fftInputSize; i++) {
      fftInput[i] = block[i] * window[i];
    }

    safelyPopBlock();
    
    fftw_execute(fftPlan);
    
    SpectrumData result = resultAsSpectrumData();
    
    // Apply the result to a function received by parameter
    spectrumDataReceiver(result);

  }
}

SpectrumData SpectrumAnalyzer::resultAsSpectrumData() {
  SpectrumData data;
  
  for(int i = 0; i < fftOutputSize; i++) {
    data.push_back(complex{fftResult[i][0], fftResult[i][1]});
  }
  
  return data;
}
} // End namespace des
