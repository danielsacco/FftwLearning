#pragma once

#include <fftw3.h>
#include <vector>
#include <complex>
#include <queue>
#include <mutex>
#include <type_traits>
#include "IPlugConstants.h"
#include <boost/circular_buffer.hpp>

namespace des {

using std::vector;
using std::complex;
using SpectrumData = vector<complex<double>>;

class WindowFunction {
public:
  WindowFunction(int size) {
    // Blackman wB (n) = 0.42 − 0.5 cos(2π n/N ) + 0.08 cos(4π n/N )
    // Hamming wH (n) = 0.54 − 0.46 cos(2π n/N )
    
    values.reserve(size);
    
    // Using Blackman
    int adjustedDenominator = size - 1;
    values.push_back(0.0);
    for (int i = 1; i < adjustedDenominator ; i++) {
      values.push_back(0.42659 - 0.49656 * std::cos(DOUBLE_PI * i/adjustedDenominator) + 0.076849 * std::cos(QUAD_PI * i/adjustedDenominator)
      );
    }
    values.push_back(0.0);
  }
  
  const double operator[](std::size_t idx) const { return values[idx]; }
  
private:
  const double DOUBLE_PI = 2.0 * iplug::PI;
  const double QUAD_PI = 4.0 * iplug::PI;
  vector<double> values;
};

class SpectrumAnalyzer {
  
public:
  SpectrumAnalyzer(int fftSize, int hopSize);
  
  ~SpectrumAnalyzer();
  
  void push_sample(double sample);
  
  void push_sample(vector<double> samplesBlock);
  
  void processPendingData(std::function<void(SpectrumData)> spectrumDataReceiver);
  
private:

  void calculateSpectrum();
  void emitResult();
  void safelyPushBlock(vector<double>& dataBlock) {
    std::lock_guard<std::mutex> montoto(queueMutex);
    pendingBlocks.push(dataBlock);
  }
  void safelyPopBlock() {
    std::lock_guard<std::mutex> montoto(queueMutex);
    pendingBlocks.pop();
  }

  SpectrumData resultAsSpectrumData();
  
  boost::circular_buffer<double> inputBuffer;
  std::queue<vector<double>> pendingBlocks;
  std::mutex queueMutex;
  
  const int fftInputSize;
  const int fftOutputSize = fftInputSize / 2 + 1;
  const int hopSize;
  int hopCounter;
  double *fftInput;
  fftw_complex *fftResult;
  fftw_plan fftPlan;

  WindowFunction window;
};


} // End namespace des


