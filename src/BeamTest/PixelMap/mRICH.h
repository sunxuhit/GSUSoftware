#ifndef mRICH_h
#define mRICH_h

namespace mRICH
{
  const double mHalfWidth = 52.5; // (mm) half width (x) and height (y) of the glass window

  const int mNumOfPixels = 33; // 16*2 3mm-pixels + 1 2*2mm-glasswindow+1mm-gap
  const double mPixels[mNumOfPixels+1] = {-50.5,-47.5,-44.5,-41.5,-38.5,-35.5,-32.5,-29.5,-26.5,-23.5,-20.5,-17.5,-14.5,-11.5,-8.5,-5.5,-2.5,2.5,5.5,8.5,11.5,14.5,17.5,20.5,23.5,26.5,29.5,32.5,35.5,38.5,41.5,44.5,47.5,50.5};
}

#endif
