#pragma once

#include "IControl.h"
#include "IGraphicsStructs.h"
#include "SpectrumAnalyzer.h"
#include <iostream>

namespace des {

using namespace iplug;
using namespace iplug::igraphics;

class IVSpectrumControl : public IControl, public IVectorBase
{
public:
  IVSpectrumControl(const IRECT& bounds, const char* label, const IVStyle& style)
  : IControl(bounds)
  , IVectorBase(style)
  {
    // TODO
  }
  
  void Draw(IGraphics &graphics) override
  {
    // TODO
  }
  
  void OnMsgFromDelegate(int msgTag, int dataSize, const void* pData) override
  {
    if(! IsDisabled() && msgTag == ISender<>::kUpdateMessage)
    {
      IByteStream stream(pData, dataSize);
      
      int pos = 0;
      ISenderData<1> data;
      pos = stream.Get(&data, pos);
      
      SpectrumData spectrum = static_cast<SpectrumData>(data.vals[0]);
      
      std::cout << spectrum[12] << std::endl;
      
    }
  }
};

} // end namespace des
