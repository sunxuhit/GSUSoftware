#include "TSystem.h"
#include <iostream>

int genPixelMap()
{
  gSystem->Load("/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libPixelMap.dylib");

  std::cout << "gSystem::Load <- libPixelMap.dylib" << endl;

  PixelMap pixel_map = PixelMap();
  pixel_map.Init_PixelMap_PMT();
  /* pixel_map->Init_PixelMap_MPPC(); */


  std::cout << "All done" << endl;
  gSystem->Exit(0);
}
