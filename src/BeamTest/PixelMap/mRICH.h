#ifndef mRICH_h
#define mRICH_h

namespace mRICH
{
  const double mHalfWidth = 53.15; // (mm) half width (x) and height (y) of the mRICH readout PMT arrangement

  const int mNumOfPixels = 33; // 6.0 mm gap (1.65+2.7+1.65) | edge pixel => 2*2 3.25mm-pixel | middle pixel => 2*14 3mm-pixel | optional edge pixel => 2*1 1.65 mm-pixel
  // const double mPixels[mNumOfPixels+1] = {-52.15,-48.9,-45.9,-42.9,-39.9,-36.9,-33.9,-30.9,-27.9,-24.9,-21.9,-18.9,-15.9,-12.9,-9.9,-6.9,-3.65,3.65,6.9,9.9,12.9,15.9,18.9,21.9,24.9,27.9,30.9,33.9,36.9,39.9,42.9,45.9,48.9,52.15};
  const double mPixels[mNumOfPixels+1] = {-51.5,-48.25,-45.25,-42.25,-39.25,-36.25,-33.25,-30.25,-27.25,-24.25,-21.25,-18.25,-15.25,-12.25,-9.25,-6.25,-3.0,3.0,6.25,9.25,12.25,15.25,18.25,21.25,24.25,27.25,30.25,33.25,36.25,39.25,42.25,45.25,48.25,51.5};
}

#endif
