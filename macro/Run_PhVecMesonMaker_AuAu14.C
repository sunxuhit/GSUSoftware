#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void Run_PhVecMesonMaker_AuAu14(const char *outFile = "picodstobject.root")
{
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libcompactCNT.so");
  gSystem->Load("libmutoo_subsysreco");
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("libMWGOO");
  gSystem->Load("libmutrg");
  //gSystem->Load("librpc_subsysreco");
  //gSystem->Load("librpc_muotrackreco");
  gSystem->Load("librecal");
  gSystem->Load("libert");
  gSystem->Load("libbbc.so");
  gSystem->Load("libzdc.so");
  gSystem->Load("../../install/lib/libPhVecMesonMaker.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc =  recoConsts::instance();
  rc->set_IntFlag("RD_RUN_SELECTION", 14);
  rc->set_IntFlag("RD_SYSTEM_SELECTION", 0);
  rc->get_IntFlag("EP_DEBUG", 0);
  rc->get_IntFlag("EP_CALIB", 0);
  rc->get_DoubleFlag("RD_BBCZCUT_VAL", 10);
  rc->get_IntFlag("EP_BBC", 0);

  PhVecMesonMaker *ana = new PhVecMesonMaker(outFile);
  se->registerSubsystem(ana);
}

void 
InputData(vector<string> &indata)
{
  //indata.push_back("CNT");
  // indata.push_back("MWG");
  // indata.push_back("MUONDST");
  indata.push_back("DST_EVE");
  // indata.push_back("dst_fvtx");
  return;
}
