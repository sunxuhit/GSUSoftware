#include <iostream>
#include <string>
#include "TSystem.h"

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libgemcCalibration.dylib)

int genCalibration(std::string detector = "PMT")
{
  std::cout << "gSystem::Load <- libgemcCalibration.dylib" << endl;

  gemcCalibration *calib = new gemcCalibration();

  calib->set_det(detector);
  if(detector == "PMT") calib->is_PMT(true);
  if(detector == "MPPC") calib->is_PMT(false);
  // calib->set_airgap("3mm_center");
  calib->set_airgap("3mm");
  // calib->set_HodoXY(3.65,3.65);
  calib->set_HodoXY(3.0,3.0);
  calib->set_VetoXY(-1.0,-1.0);
  // calib->set_HodoR(3.65);
  // calib->set_VetoR(-1.0);

  calib->Init();
  calib->Make();
  calib->Finish();

  std::cout << "All done" << endl;

  return 1;
}
