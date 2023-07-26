#include "SpectrumAnalyzer.h"

//using namespace std;

namespace des {

SpectrumAnalyzer::SpectrumAnalyzer(size_t fftSize) : fftInputSize(fftSize) { }

void SpectrumAnalyzer::push_sample(double sample) {
  
  inputData.push_back(sample);
  
  if(inputData.size() < fftInputSize) {
    // keep accumulating samples until filling a whole block
    return;
  }
  
  // We reached the fft size.
  // TODO enqueue the data for spectrum calculation
  
  inputData.clear();
}
}
