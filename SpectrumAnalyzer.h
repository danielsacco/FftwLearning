#pragma once

#include <fftw3.h>
#include <vector>
#include <complex>
#include <queue>
#include <type_traits>



namespace des {

using std::vector;
using std::complex;
using std::queue;
using SpectrumData = vector<complex<double>>;

class SpectrumAnalyzer {
  
  public:
    SpectrumAnalyzer(int fftSize);
    
    ~SpectrumAnalyzer();
    
    void push_sample(double sample);
    
    void push_sample(vector<double> samplesBlock);
    
    void processPendingData(std::function<void(SpectrumData)> spectrumDataReceiver);
    
  private:

    void calculateSpectrum();
    void emitResult();
    
    SpectrumData resultAsSpectrumData();
    
    vector<double> inputBuffer;
    queue<vector<double>> pendingBlocks;
    
    const int fftInputSize;
    const int fftOutputSize = fftInputSize / 2 + 1;
    double *fftInput;
    fftw_complex *fftResult;
    fftw_plan fftPlan;
  };
}
