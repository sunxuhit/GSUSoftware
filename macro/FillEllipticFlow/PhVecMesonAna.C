#include <iostream>
#include <ostream>
#include <string>
#include <TStopwatch.h>

void PhVecMesonAna(const int pid = 0, const int list = 1, const long start_event = 0, const long stop_event = 1024, const int mode = 0)
{
  // pid: 0 for J/Psi | 1 for phi
  // list: list number of input TTree list
  // mode: 0 for US | 1 for LS | 2 for ME (TODO)

  TStopwatch *stopWatch = new TStopwatch();
  stopWatch->Start();

  gSystem->Load("$HOME/WorkSpace/PHENIX/install/lib/libPhVecMesonAna.so");
  cout << "Load in libPhVecMesonAna.so!" << endl;
  cout << "Start to Analyze PicoDst TTree!" << endl;

  PhVecMesonAna *mVecMesonAna = new PhVecMesonAna(pid,list,start_event,stop_event,mode);
  mVecMesonAna->Init();
  mVecMesonAna->Make();
  mVecMesonAna->Finish();

  cout << "Completed Analyzing Di-Muon Yields in PhVecMesonAna.C." << endl;
}
