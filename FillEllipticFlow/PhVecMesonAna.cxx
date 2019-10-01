//local
#include "PhVecMesonAna.h"

//stl
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>

//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TMath.h>
#include <TString.h>
#include <TVector3.h>

//Utility Class
#include "PhDiMuonHistoManager.h"
// #include "PhDiMuonProManager.h"
#include "PhDiMuonUtility.h"
// #include "PhDiMuonCut.h"
#include "PhDiMuonEvent.h"
#include "PhVecMesonCons.h"

ClassImp(PhVecMesonAna)

using namespace std;

PhVecMesonAna::PhVecMesonAna(int pid, int list, long start_event, long stop_event, int mode)
{
  mPid = pid;
  mMode= mode;
  mList = list;

  mStart_Event = start_event;
  mStop_Event = stop_event;

  mBbczCut_val     = 10.0;
  File_mOutPut = NULL; // output file
  mChain_DiMuon = NULL;
  mDiMuonEvent = NULL;

  // mPhDiMuonHistoManager = NULL;
  // mPhDiMuonProManager   = NULL;
  mPhDiMuonUtility      = NULL;
  // mPhDiMuonCut = NULL;
}

PhVecMesonAna::~PhVecMesonAna()
{
  delete mPhDiMuonUtility; // delete utilities
  // delete mPhDiMuonHistoManager; // delete histograms manager
  // delete mPhDiMuonProManager; // delete  profiles manager
  // delete mPhDiMuonCut; // delete cut manager
}

int PhVecMesonAna::Init() 
{
  mPhDiMuonUtility = new PhDiMuonUtility(); // initialize utilities
  mPhDiMuonUtility->initRunIndex();
  mPhDiMuonHistoManager = new PhDiMuonHistoManager(); // initialize histograms
  mPhDiMuonHistoManager->initHist_InvMass(mPid,mMode);
  // mPhDiMuonProManager   = new PhDiMuonProManager(); // initialize histograms

  // initialize input/output
  string inputdir = "/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonTree/";
  setInPutDir(inputdir);

  const int list_start = vecMesonFlow::mList_Delta*mList + 1; // start list
  const int list_stop  = vecMesonFlow::mList_Delta*(mList+1); // stop list
  string inputlist = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/list/DiMuonList/Split_DiMuon_Run14AuAu200GeV_%d_%d.list",list_start,list_stop);
  setInPutList(inputlist);

  string outputfile = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_%s_%s_%d.root",vecMesonFlow::mPID[mPid].c_str(),vecMesonFlow::mEventType[mMode].c_str(),mList);
  setOutPutFile(outputfile);
  File_mOutPut = new TFile(mOutPutFile.data(), "recreate");
  if(!File_mOutPut) cout<<"Cann't create " << mOutPutFile.data() <<endl;

  setStartEvent(mStart_Event);
  setStopEvent(mStop_Event);

  string Notification = Form("Initializing parameters and input/output for %s %s",vecMesonFlow::mPID[mPid].c_str(),vecMesonFlow::mEventType[mMode].c_str());
  cout << Notification.c_str() << endl;

  bool isBranch = true;
  if (!mInPutList.empty())   // if input file is ok
  {
    cout << "Open input probability file list" << endl;
    ifstream in(mInPutList.c_str());  // input stream
    if(in)
    {
      cout << "input file probability list is ok" << endl;
      mChain_DiMuon = new TChain("DiMuonEvent","DiMuonEvent");
      char str[255];       // char array for each file name
      long entries_save = 0;
      while(in)
      {
	in.getline(str,255);  // take the lines of the file list
	if(str[0] != 0)
	{
	  string addfile;
	  addfile = str;
	  addfile = inputdir + addfile;
	  mChain_DiMuon->AddFile(addfile.c_str(),-1,"DiMuonEvent");
	  long file_entries = mChain_DiMuon->GetEntries();
	  cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	  entries_save = file_entries;
	}
      }
    }
    else
    {
      string InFo_Warning = Form("WARNING: %s %s file input is problemtic",vecMesonFlow::mPID[mPid].c_str(),vecMesonFlow::mEventType[mMode].c_str());
      cout << "WARNING: input probability file input is problemtic" << endl;
      isBranch = false;
    }
  }

  // Set the input tree
  if (isBranch && !mChain_DiMuon->GetBranch( "DiMuonBranch" ))
  {
    cerr << "ERROR: Could not find branch 'DiMuonBranch' in tree!" << endl;
    return -1;
  }

  if(isBranch)
  {
    cout << "Initializing InPut DiMuon TTree!" << endl;
    mDiMuonEvent = new PhDiMuonEvent();
    mChain_DiMuon->SetBranchAddress( "DiMuonBranch", &mDiMuonEvent);

    int num_events = mChain_DiMuon->GetEntriesFast();
    cout << "Number of events in file(s) = " << num_events << endl;
    if(mStartEvent > num_events) mStartEvent = num_events;
    if(mStopEvent > num_events) mStopEvent   = num_events;

    cout << "New nStartEvent = " << mStartEvent << ", new nStopEvent = " << mStopEvent << endl;
  }

  return 0;
}

int PhVecMesonAna::Make() 
{
  mChain_DiMuon->SetBranchAddress( "DiMuonBranch", &mDiMuonEvent);
  mChain_DiMuon->GetEntry(0); // For unknown reasons root doesn't like it if someone starts to read a file not from the 0 entry

  for(int i_event = mStartEvent; i_event < mStopEvent; ++i_event)
  {
    if (!mChain_DiMuon->GetEntry( i_event )) // take the event -> information is stored in event
      break;  // end of data chunk

    /*
    // get Vertex
    TVector3 vtx_BBC = mDiMuonEvent->getVtxBbc();
    TVector3 vtx_FVTX = mDiMuonEvent->getVtxFvtx();

    // get Global Info
    float centrality = mDiMuonEvent->getCentrality();
    int runId = mDiMuonEvent->getRunId();
    int runIndex = mDiMuonEvent->getRunIndex();
    int eventId = mDiMuonEvent->getEventId();

    const int cent20 = mPhDiMuonUtility->getCentralityBin20(centrality);
    // const int cent10 = mPhDiMuonUtility->getCentralityBin10(centrality);
    // const int cent4  = mPhDiMuonUtility->getCentralityBin4(centrality);
    // const int vtx4   = mPhDiMuonUtility->getVertexBin(vtx_bbcz);
    // cout << "centrality = " << centrality << ", cent20 = " << cent20 << ", cent10 = " << cent10 << ", vtx4 = " << vtx4 << endl;

    // get Event Plane
    // float Psi_BbcSouth_1st = mDiMuonEvent->getPsi_BbcSouth_1st();
    // float Psi_BbcNorth_1st = mDiMuonEvent->getPsi_BbcNorth_1st();
    float Psi_BbcSouth_2nd = mDiMuonEvent->getPsi_BbcSouth_2nd();
    float Psi_BbcNorth_2nd = mDiMuonEvent->getPsi_BbcNorth_2nd();
    // float Psi_BbcSouth_3rd = mDiMuonEvent->getPsi_BbcSouth_3rd();
    // float Psi_BbcNorth_3rd = mDiMuonEvent->getPsi_BbcNorth_3rd();

    // float Psi_FvtxSouth_1st = mDiMuonEvent->getPsi_FvtxSouth_1st();
    // float Psi_FvtxNorth_1st = mDiMuonEvent->getPsi_FvtxNorth_1st();
    float Psi_FvtxSouth_2nd = mDiMuonEvent->getPsi_FvtxSouth_2nd();
    float Psi_FvtxNorth_2nd = mDiMuonEvent->getPsi_FvtxNorth_2nd();
    // float Psi_FvtxSouth_3rd = mDiMuonEvent->getPsi_FvtxSouth_3rd();
    // float Psi_FvtxNorth_3rd = mDiMuonEvent->getPsi_FvtxNorth_3rd();

    // int NumOfTracks = mDiMuonEvent->getNumTracks();
    int NumOfDiMuons = mDiMuonEvent->getNumOfDiMuons();

    // Track Level => di-muon pairs
    for(int i_dimuon = 0; i_dimuon < NumOfDiMuons; ++i_dimuon) 
    {
      // get di-muon track
      mDiMuonTrack = mDiMuonEvent->getTrack(i_dimuon);

      // get di-muon info
      float invmass           = mDiMuonTrack->getInvMass(); // set di-muon info to mTree_DiMuon
      float px                = mDiMuonTrack->getPx();
      float py                = mDiMuonTrack->getPy();
      float pz                = mDiMuonTrack->getPz();
      float rapidity          = mDiMuonTrack->getRapidity();
      short charge            = mDiMuonTrack->getCharge();
      float dca_r             = mDiMuonTrack->getDcaR();
      float dca_z             = mDiMuonTrack->getDcaZ();
      float vtxchi2           = mDiMuonTrack->getVtxChi2();
      float invmass_fvtx      = mDiMuonTrack->getInvMass_fvtx(); // fvtx
      float invmass_fvtxmutr  = mDiMuonTrack->getInvMass_fvtxmutr(); // fvtxmutr
      float px_fvtxmutr       = mDiMuonTrack->getPx_fvtxmutr();
      float py_fvtxmutr       = mDiMuonTrack->getPy_fvtxmutr();
      float pz_fvtxmutr       = mDiMuonTrack->getPz_fvtxmutr();
      float rapidity_fvtxmutr = mDiMuonTrack->getRapidity_fvtxmutr();
      float vtxchi2_fvtxmutr  = mDiMuonTrack->getVtxChi2_fvtxmutr();

      // single muon spectra
      // Tr0
      float px_tr0            = mDiMuonTrack->setPx_tr0(px_tr0); // set Tr0 info to mTree_DiMuon 
      float py_tr0            = mDiMuonTrack->setPy_tr0(py_tr0); 
      float pz_tr0            = mDiMuonTrack->setPz_tr0(pz_tr0); 
      float rapidity_tr0      = mDiMuonTrack->setRapidity_tr0(rapidity_tr0); 
      float DG0_tr0           = mDiMuonTrack->setDG0_tr0(DG0_tr0); 
      float DDG0_tr0          = mDiMuonTrack->setDDG0_tr0(DDG0_tr0); 
      float dca_r_tr0         = mDiMuonTrack->setDcaR_tr0(dca_r_tr0); 
      float dca_z_tr0         = mDiMuonTrack->setDcaZ_tr0(dca_z_tr0); 
      short trhits_tr0        = mDiMuonTrack->setTrhits_tr0(trhits_tr0); 
      float trchi2_tr0        = mDiMuonTrack->setTrchi2_tr0(trchi2_tr0); 
      short ntrhits_tr0       = mDiMuonTrack->setNTrhits_tr0(ntrhits_tr0); 
      short idhits_tr0        = mDiMuonTrack->setIdhits_tr0(idhits_tr0); 
      float idchi2_tr0        = mDiMuonTrack->setIdchi2_tr0(idchi2_tr0); 
      short nidhits_tr0       = mDiMuonTrack->setNIdhits_tr0(nidhits_tr0); 
      float idx_tr0           = mDiMuonTrack->setIdx_tr0(idx_tr0); 
      float idy_tr0           = mDiMuonTrack->setIdy_tr0(idy_tr0); 
      short lastgap_tr0       = mDiMuonTrack->setLastgap_tr0(lastgap_tr0); 
      float px_fvtx_tr0       = mDiMuonTrack->setPxfvtx_tr0(px_fvtx_tr0); // fvtx
      float py_fvtx_tr0       = mDiMuonTrack->setPyfvtx_tr0(py_fvtx_tr0);
      float pz_fvtx_tr0       = mDiMuonTrack->setPzfvtx_tr0(pz_fvtx_tr0);
      float dphi_fvtx_tr0     = mDiMuonTrack->setDphifvtx_tr0(dphi_fvtx_tr0);
      float dtheta_fvtx_tr0   = mDiMuonTrack->setDthetafvtx_tr0(dtheta_fvtx_tr0);
      float dr_fvtx_tr0       = mDiMuonTrack->setDrfvtx_tr0(dr_fvtx_tr0);
      float chi2_fvtx_tr0     = mDiMuonTrack->setChi2fvtx_tr0(chi2_fvtx_tr0);
      float px_fvtxmutr_tr0   = mDiMuonTrack->setPxfvtxmutr_tr0(px_fvtxmutr_tr0); // fvtxmutr
      float py_fvtxmutr_tr0   = mDiMuonTrack->setPyfvtxmutr_tr0(py_fvtxmutr_tr0);
      float pz_fvtxmutr_tr0   = mDiMuonTrack->setPzfvtxmutr_tr0(pz_fvtxmutr_tr0);
      float chi2_fvtxmutr_tr0 = mDiMuonTrack->setChi2fvtxmutr_tr0(chi2_fvtxmutr_tr0);

      // Tr1
      float px_tr1            = mDiMuonTrack->setPx_tr1(px_tr1); // set Tr1 info to mTree_DiMuon 
      float py_tr1            = mDiMuonTrack->setPy_tr1(py_tr1); 
      float pz_tr1            = mDiMuonTrack->setPz_tr1(pz_tr1); 
      float rapidity_tr1      = mDiMuonTrack->setRapidity_tr1(rapidity_tr1); 
      float DG0_tr1           = mDiMuonTrack->setDG0_tr1(DG0_tr1); 
      float DDG0_tr1          = mDiMuonTrack->setDDG0_tr1(DDG0_tr1); 
      float dca_r_tr1         = mDiMuonTrack->setDcaR_tr1(dca_r_tr1); 
      float dca_z_tr1         = mDiMuonTrack->setDcaZ_tr1(dca_z_tr1); 
      short trhits_tr1        = mDiMuonTrack->setTrhits_tr1(trhits_tr1); 
      float trchi2_tr1        = mDiMuonTrack->setTrchi2_tr1(trchi2_tr1); 
      short ntrhits_tr1       = mDiMuonTrack->setNTrhits_tr1(ntrhits_tr1); 
      short idhits_tr1        = mDiMuonTrack->setIdhits_tr1(idhits_tr1); 
      float idchi2_tr1        = mDiMuonTrack->setIdchi2_tr1(idchi2_tr1); 
      short nidhits_tr1       = mDiMuonTrack->setNIdhits_tr1(nidhits_tr1); 
      float idx_tr1           = mDiMuonTrack->setIdx_tr1(idx_tr1); 
      float idy_tr1           = mDiMuonTrack->setIdy_tr1(idy_tr1); 
      short lastgap_tr1       = mDiMuonTrack->setLastgap_tr1(lastgap_tr1); 
      float px_fvtx_tr1       = mDiMuonTrack->setPxfvtx_tr1(px_fvtx_tr1); // fvtx
      float py_fvtx_tr1       = mDiMuonTrack->setPyfvtx_tr1(py_fvtx_tr1);
      float pz_fvtx_tr1       = mDiMuonTrack->setPzfvtx_tr1(pz_fvtx_tr1);
      float dphi_fvtx_tr1     = mDiMuonTrack->setDphifvtx_tr1(dphi_fvtx_tr1);
      float dtheta_fvtx_tr1   = mDiMuonTrack->setDthetafvtx_tr1(dtheta_fvtx_tr1);
      float dr_fvtx_tr1       = mDiMuonTrack->setDrfvtx_tr1(dr_fvtx_tr1);
      float chi2_fvtx_tr1     = mDiMuonTrack->setChi2fvtx_tr1(chi2_fvtx_tr1);
      float px_fvtxmutr_tr1   = mDiMuonTrack->setPxfvtxmutr_tr1(px_fvtxmutr_tr1); // fvtxmutr
      float py_fvtxmutr_tr1   = mDiMuonTrack->setPyfvtxmutr_tr1(py_fvtxmutr_tr1);
      float pz_fvtxmutr_tr1   = mDiMuonTrack->setPzfvtxmutr_tr1(pz_fvtxmutr_tr1);
      float chi2_fvtxmutr_tr1 = mDiMuonTrack->setChi2fvtxmutr_tr1(chi2_fvtxmutr_tr1);
    }
  */
  }

  return 1;
}

int PhVecMesonAna::Finish() 
{
  File_mOutPut->cd();
  mPhDiMuonHistoManager->writeHist_InvMass(mPid,mMode);

  File_mOutPut->Close();

  return 1;
}

//----------------------------------------------------
// set Input/Output
void PhVecMesonAna::setInPutDir(const string inputdir)
{
  mInPutDir = inputdir;
  cout << "Input directory was set to: " << mInPutDir.c_str() << endl;
}
void PhVecMesonAna::setOutPutFile(const string outputfile)
{
  mOutPutFile = outputfile;
  cout << "Output file was set to: " << mOutPutFile.c_str() << endl;
}
void PhVecMesonAna::setInPutList(const string inputlist)
{
  mInPutList = inputlist;
  string InFo_InPutList = Form("InPut %s list was set to: %s",vecMesonFlow::mEventType[mMode].c_str(),mInPutList.c_str());
  cout << InFo_InPutList.c_str() << endl;
}
void PhVecMesonAna::setStopEvent(const long StopEvent)
{
  mStopEvent = StopEvent;
  cout << "nStopEvent = " << mStopEvent << endl;
}
void PhVecMesonAna::setStartEvent(const long StartEvent)
{
  mStartEvent = StartEvent;
  cout << "nStartEvent = " << mStartEvent << endl;
}
//----------------------------------------------------
