#include "TSystem.h"
#include <iostream>

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/lib/libMaterial.dylib)

int genQuantumEff(double lambda = 550.0)
{
  std::cout << "gSystem::Load <- libMaterial.dylib" << endl;

  Material *mat = new Material();
  double qe_PMT = mat->extrapQE_PMT(lambda);
  std::cout << "quantum efficiency for PMT with lambda = " << lambda << " is: " << qe_PMT << endl;
  double qe_MPPC = mat->extrapQE_MPPC(lambda);
  std::cout << "quantum efficiency for MPPC with lambda = " << lambda << " is: " << qe_MPPC << endl;

  std::cout << "All done" << endl;

  return 1;
}
