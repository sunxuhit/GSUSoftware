#include "TSystem.h"
#include <iostream>

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libCalibration.dylib)

int genCalibration()
{
  std::cout << "gSystem::Load <- libCalibration.dylib" << endl;

  Calibration *calib = new Calibration("PMT","Calibration");
  calib->Init();
  calib->Make();
  calib->Finish();


  std::cout << "All done" << endl;

  return 1;
}
