#include "TSystem.h"
#include <iostream>

/* R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libPixelMap.dylib) */
R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/build/src/BeamTest/PixelMap/libPixelMap.dylib)

int genPixelMap()
{
  gSystem->Load("/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libPixelMap.dylib");

  std::cout << "gSystem::Load <- libPixelMap.dylib" << endl;

  PixelMap *pixel_map = new PixelMap();
  pixel_map->Init_PixelMap_PMT();
  /* pixel_map->Init_PixelMap_MPPC(); */


  std::cout << "All done" << endl;

  return 1;
}
