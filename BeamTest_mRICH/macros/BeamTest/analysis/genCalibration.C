#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libCalibration.dylib)

int genCalibration(std::string detector = "PMT")
{
  std::cout << "gSystem::Load <- libCalibration.dylib" << endl;

  Calibration *calib = new Calibration();

  calib->set_det(detector);
  if(detector == "PMT") calib->is_PMT(true);
  if(detector == "MPPC") calib->is_PMT(false);

  calib->Init();
  calib->Make();
  calib->Finish();


  std::cout << "All done" << endl;

  return 1;
}
