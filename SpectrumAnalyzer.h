#pragma once

#include <fftw3.h>
#include <vector>
#include <queue>
#include <type_traits>

using namespace std;

namespace des {
  class SpectrumAnalyzer {
  
  public:
    SpectrumAnalyzer(size_t fftSize);
    
    void push_sample(double sample);
    
  private:

    void calculateSpectrum();
    void emitResult();
    
    vector<double> inputData;
    queue<vector<double>> pendingBlocks;
    
    const size_t fftInputSize;
    const size_t fftOutputSize = fftInputSize / 2 + 1;
    double *fftInput;
    fftw_complex *fftResult;
    fftw_plan fftPlan;
  };
}
