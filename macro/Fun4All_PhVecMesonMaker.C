#include <iostream>
#include <ostream>
#include <string>

// void Fun4All_PhVecMesonMaker(const int nEvents = 10024, const int mode = 1, const int jobId = 10)
void Fun4All_PhVecMesonMaker(const int nEvents = -1, const int mode = 1, const int jobId = 24)
{
  gSystem->Load("../../install/lib/libPhVecMesonMaker.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc =  recoConsts::instance();
  rc->set_IntFlag("RUN_SELECTION", 14);
  rc->set_IntFlag("SYSTEM_SELECTION", 0);
  rc->set_DoubleFlag("BBCZCUT_VAL", 10);
  rc->set_IntFlag("ANA_MODE",mode); // fill event plane resolution

  if(mode == 0) std::cout << "mode = 0 & fill event plane resolution" << std::endl;
  if(mode == 1) std::cout << "mode = 1 & fill di-muon TTree" << std::endl;

  std::string outputfile;
  if(mode == 0) outputfile = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/EventPlaneResolution/file_AuAu200GeV_Resoluiton_%d.root",jobId);
  if(mode == 1) outputfile = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonTree/file_AuAu200GeV_DiMuonTree_%d.root",jobId);
  std::cout << "outputfile = " << outputfile.c_str() << std::endl;

  PhVecMesonMaker *ana = new PhVecMesonMaker(outputfile.c_str());
  se->registerSubsystem(ana);

  std::string input_file = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/list/run_list/Split_picoDst_Run14AuAu200GeV_%d.list",jobId);
  cout << "input_file: " << input_file.c_str() << endl;
  Fun4AllDstInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->AddListFile(input_file);
  se->registerInputManager(in);

  se->run(nEvents);
  se->End();

  delete se;

  cout << "Completed reconstruction in Fun4All_PhVecMesonMaker.C." << endl;
}
