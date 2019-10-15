#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include "../../FillEllipticFlow/PhVecMesonCons.h"

using namespace std;

void calEllipticFlow(int pid = 0)
{
  string input_US = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[0].c_str());
  TFile *File_InPutUS = TFile::Open();
  cout << input_US.c_str() << endl;

  string input_LS = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[1].c_str());
  cout << input_LS.c_str() << endl;


}
