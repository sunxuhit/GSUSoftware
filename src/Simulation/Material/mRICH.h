#ifndef mRICH_h
#define mRICH_h

namespace mRICH
{
  const double mHalfWidth = 53.8; // (mm) half width (x) and height (y) of the mRICH readout PMT arrangement

  const int mNumOfPixels = 33; // 7.3 mm gap (1.65+4.0+1.65) | edge pixel => 2*2 3.25mm-pixel | middle pixel => 2*14 3mm-pixel | optional edge pixel => 2*1 1.65 mm-pixel
  const double mPixels[mNumOfPixels+1] = {-52.15,-48.9,-45.9,-42.9,-39.9,-36.9,-33.9,-30.9,-27.9,-24.9,-21.9,-18.9,-15.9,-12.9,-9.9,-6.9,-3.65,3.65,6.9,9.9,12.9,15.9,18.9,21.9,24.9,27.9,30.9,33.9,36.9,39.9,42.9,45.9,48.9,52.15};
}

#endif
