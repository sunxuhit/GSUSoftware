//local
#include "PhVecMesonMaker.h"

//PHENIX
#include <Fun4AllServer.h>
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>
#include <PHGlobal.h>
#include <PHCompositeNode.h>
#include "TrigLvl1.h"
#include "TriggerHelper.h"

#include <RunHeader.h>
#include <recoConsts.h>
#include <RunToTime.hh>
#include <EventHeader.h>

// #include <Bbc.hh>
// #include <BbcRaw.h>
// #include <BbcOut.h>
// #include <BbcCalib.hh>
// #include <BbcGeo.hh>

#include <VtxOut.h>
#include <PHPoint.h>

#include <DiMuonContainer.h>
#include <DiMuon.h>

#include "ReactionPlaneObject.h"
#include "ReactionPlaneSngl.h"
#include "RpConst.h"

//stl
#include <iostream>
#include <ostream>
#include <string>

//ROOT
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile2D.h>
#include <TMath.h>
#include <TString.h>
#include <TVector3.h>

//Utility Class
#include "RecoEPHistoManager.h"
#include "RecoEPProManager.h"
#include "RecoEPUtility.h"
#include "PhDiMuonEvent.h"

using namespace std;

PhVecMesonMaker::PhVecMesonMaker(const char*outputfile)
{
  mRC = NULL;
  // RC flags
  mRunSelection    = 0;
  mSystemSelection = 0;
  mMode            = 0;
  mBbczCut_val     = 10.0;

  // PH class
  mPHGlobal  = NULL;
  mTrigLvl1  = NULL;
  mRunHeader = NULL;
  mRunId     = -999;
  mRunIndex  = -999;
  mVtxOut    = NULL; // vertex
  // mBbcCalib  = NULL;
  // mBbcGeo    = NULL;

  mOutPutFile  = outputfile;
  File_mOutPut = NULL; // output file
  mTree_DiMuon = NULL;
  mDiMuonEvent = NULL;

  mNumOfEvents = 0; // number of events

  mRecoEPHistoManager = NULL;
  mRecoEPProManager   = NULL;
  mRecoEPUtility      = NULL;
}

PhVecMesonMaker::~PhVecMesonMaker()
{
  // delete mBbcCalib;
  // delete mBbcGeo;

  delete mRecoEPUtility; // delete utilities
  delete mRecoEPHistoManager; // delete histograms manager
  delete mRecoEPProManager; // delete  profiles manager

  /*
  if(mMode == 1)
  {
    delete mTree_DiMuon;
    delete mDiMuonEvent;
  }
  */
}

int PhVecMesonMaker::Init(PHCompositeNode *topNode) 
{
  cout << "PhVecMesonMaker::Init => " << endl;

  cout << "Setting Module Flags: " << endl;
  mRC              = recoConsts::instance();
  mRunSelection    = mRC->get_IntFlag("RUN_SELECTION", 0);
  mSystemSelection = mRC->get_IntFlag("SYSTEM_SELECTION", 0);
  mMode            = mRC->get_IntFlag("ANA_MODE", 0);
  mBbczCut_val     = mRC->get_DoubleFlag("BBCZCUT_VAL", 10);

  cout << "run " << mRunSelection << " @ system ";
  if(mRunSelection == 14 && mSystemSelection == 0) cout << "Au+Au" << endl;
  cout << "BBCZ cut value is " << mBbczCut_val << endl;

  // init output structure
  File_mOutPut = new TFile(mOutPutFile.data(), "recreate");
  if(!File_mOutPut) cout<<"Cann't create " << mOutPutFile.data() <<endl;

  mNumOfEvents = 0; // number of events

  mRecoEPUtility      = new RecoEPUtility(); // initialize utilities
  mRecoEPUtility->initRunIndex();
  mRecoEPHistoManager = new RecoEPHistoManager(); // initialize histograms
  mRecoEPHistoManager->initQA_Global();
  mRecoEPProManager   = new RecoEPProManager(); // initialize histograms

  if(mMode == 0) // Produce Event Plane Resoluiton & di-muon spectra QA
  {
    cout << "Calculate Event Plane Resolution!" << endl;
    mRecoEPHistoManager->initHist_BbcEP();
    mRecoEPHistoManager->initHist_FvtxEP();
    mRecoEPProManager->initPro_BbcResolution();
    mRecoEPProManager->initPro_FvtxResolution();

    mRecoEPHistoManager->initHist_DiMuonSpec();
    mRecoEPHistoManager->initHist_DiMuonQA();
    mRecoEPHistoManager->initHist_MuonTrkSpec();
    mRecoEPHistoManager->initHist_MuonTrkQA();
  }

  if(mMode == 1) // Produce di-muon TTree
  {
    cout << "Produce DiMuon TTree!" << endl;
    mDiMuonEvent = new PhDiMuonEvent();
    mTree_DiMuon = new TTree("DiMuonEvent","DiMuonEvent");
    mTree_DiMuon->Branch("DiMuonBranch","PhDiMuonEvent",&mDiMuonEvent);
    mTree_DiMuon->SetAutoSave(5000000);
  }

  return EVENT_OK;
}

int PhVecMesonMaker::InitRun(PHCompositeNode *topNode) 
{
  cout << "PhVecMesonMaker::InitRun => " << endl;
  cout << "get run header info" << endl;
  mRunHeader = findNode::getClass<RunHeader>(topNode,"RunHeader");
  if( !mRunHeader )
  {
    cout << "can't find RunHeader == > exit!!!" << endl;
    exit(1);
  }

  mRunId = mRunHeader->get_RunNumber();
  cout << "runId is "<< mRunId << endl;
  if(!mRecoEPUtility->isGoodRun(mRunId)) 
  {
    cout << "isGoodRun => could not find in good run list! & drop the run in PhVecMesonMaker!" << endl;
    return ABORTRUN;
  }

  mRunIndex = mRecoEPUtility->find_runIndex(mRunId);
  if(mRunIndex < 0)
  {
    cout << "find_runIndex => could not find in good run list! & drop the run in PhVecMesonMaker!" << endl;
    return ABORTRUN;
  }
  else
  {
    cout << "PhVecMesonMaker -> runId: " << mRunId << " => runIndex: " << mRunIndex << endl;
  }

  /*
  // set BbcCalib
  mBbcCalib = new BbcCalib();
  mBbcGeo   = new BbcGeo();

  int bbccalib_version = 4002; // after Run5 and Field_ON
  cout << "Set BbcCalib: run number= " << mRunId << ", version of BbcCalib = " << bbccalib_version << endl;
  RunToTime* runTime = RunToTime::instance();
  PHTimeStamp* ts( runTime->getBeginTime(mRunId) );
  PHTimeStamp tstart = *ts;
  
  mBbcCalib->restore(tstart, bbccalib_version);
  delete ts;
  */

  return EVENT_OK;
}

int PhVecMesonMaker::process_event(PHCompositeNode *topNode) 
{
  if( getNodes(topNode) == DISCARDEVENT ) return DISCARDEVENT;

  mNumOfEvents++;
  // if(mNumOfEvents%1000 == 0) cout << "processing events:  " << mNumOfEvents << "/" << 10000 << endl;
  if(mNumOfEvents%10000 == 0) cout << "processing events:  " << mNumOfEvents << endl;
  // if(mNumOfEvents > 100) return ABORTRUN;

  float centrality = mPHGlobal->getCentrality();
  float zdcz = mPHGlobal->getZdcZVertex();
  float vtx_bbcz = mPHGlobal->getBbcZVertex();
  int eventId = mEventHeader->get_EvtSequence();
  

  if(isMinimumBias()) // fill QA for minium bias trigger
  {
    mRecoEPHistoManager->fillQA_Global_MiniBias(mRunIndex,zdcz,vtx_bbcz,centrality);
  }

  if( !isMinimumBiasNarrowVtx() ) return DISCARDEVENT; // select events wth minimum bias trigger & narrow vertex cut
  if(centrality < 0) return DISCARDEVENT; // remove no ZDC coincidence events
  mRecoEPHistoManager->fillQA_Global_NarrowVtx(mRunIndex,zdcz,vtx_bbcz,centrality);

  if ( vtx_bbcz >= mBbczCut_val || vtx_bbcz <= -1.0*(mBbczCut_val)) return DISCARDEVENT; // apply bbc cuts
  mRecoEPHistoManager->fillQA_Global_NarrowVtxBbc(mRunIndex,zdcz,vtx_bbcz,centrality);

  const int cent20 = mRecoEPUtility->getCentralityBin20(centrality);
  // const int cent10 = mRecoEPUtility->getCentralityBin10(centrality);
  // const int cent4  = mRecoEPUtility->getCentralityBin4(centrality);
  // const int vtx4   = mRecoEPUtility->getVertexBin(vtx_bbcz);
  // cout << "centrality = " << centrality << ", cent20 = " << cent20 << ", cent10 = " << cent10 << ", vtx4 = " << vtx4 << endl;


  if(mMode == 0)  // produce event plane resolution & di-muon QA histograms
  {
    // Event Plane QA
    for(int i_order = 0; i_order < 3; ++i_order) 
    {
      // BBC Event Plane
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_BBC, 0, i_order));
      float Psi_BbcSouth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_BBC, 1, i_order));
      float Psi_BbcNorth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      if(Psi_BbcSouth > -900.0 && Psi_BbcNorth > -900.0)
      {
	mRecoEPHistoManager->fillHist_BbcEP(Psi_BbcSouth,Psi_BbcNorth,i_order,cent20);
	mRecoEPProManager->fillPro_BbcResolution(Psi_BbcSouth,Psi_BbcNorth,i_order,mRunIndex,cent20);
      }

      // FVTX Event Plane => all sectors w/ eta > 1.0
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_FVT, 40, i_order));
      float Psi_FvtxSouth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_FVT, 41, i_order));
      float Psi_FvtxNorth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      if(Psi_FvtxSouth > -900.0 && Psi_FvtxNorth > -900.0)
      {
	mRecoEPHistoManager->fillHist_FvtxEP(Psi_FvtxSouth,Psi_FvtxNorth,i_order,cent20);
	mRecoEPProManager->fillPro_FvtxResolution(Psi_FvtxSouth,Psi_FvtxNorth,i_order,mRunIndex,cent20);
      }
    }

    // di-muon spectra
    for(unsigned int i_dimuon = 0; i_dimuon < mDiMuonContainer->get_nDiMuons(); ++i_dimuon) 
    {
      // di-muon spectra
      DiMuon *dimuon = mDiMuonContainer->get_DiMuon(i_dimuon);
      float InvMass = dimuon->get_mass();
      // float pt = dimuon->get_pT();
      float px = dimuon->get_px();
      float py = dimuon->get_py();
      float pz = dimuon->get_pz();
      float rapidity = dimuon->get_rapidity(); // > 0 => north
      mRecoEPHistoManager->fillHist_DiMuonSpec(InvMass,px,py,pz,rapidity);

      short charge = dimuon->get_charge();
      float dca_r = dimuon->get_dca_r();
      float dca_z = dimuon->get_dca_z();
      float vtx_chi2 = dimuon->get_Evt_vtxchi2();
      mRecoEPHistoManager->fillHist_DiMuonQA(charge,dca_r,dca_z,vtx_chi2,rapidity);

      // single muon spectra
      float px_tr0 = dimuon->get_Tr0_px();
      float py_tr0 = dimuon->get_Tr0_py();
      float pz_tr0 = dimuon->get_Tr0_pz();
      float rapidity_tr0 = dimuon->get_Tr0_rapidity(); // > 0 => north
      mRecoEPHistoManager->fillHist_MuonTr0Spec(px_tr0,py_tr0,pz_tr0,rapidity_tr0);

      float px_tr1 = dimuon->get_Tr1_px();
      float py_tr1 = dimuon->get_Tr1_py();
      float pz_tr1 = dimuon->get_Tr1_pz();
      float rapidity_tr1 = dimuon->get_Tr1_rapidity(); // > 0 => north
      mRecoEPHistoManager->fillHist_MuonTr1Spec(px_tr1,py_tr1,pz_tr1,rapidity_tr1);

      // single muon QA
      short trhits_tr0 = dimuon->get_Tr0_trhits();
      short idhits_tr0 = dimuon->get_Tr0_idhits();
      float DG0_tr0 = dimuon->get_Tr0_DG0();
      float DDG0_tr0 = dimuon->get_Tr0_DDG0();
      float trchi2_tr0 = dimuon->get_Tr0_trchi2();
      float idchi2_tr0 = dimuon->get_Tr0_idchi2();
      short ntrhits_tr0 = dimuon->get_Tr0_ntrhits();
      short nidhits_tr0 = dimuon->get_Tr0_nidhits();
      short lastgap_tr0 = dimuon->get_Tr0_lastgap();
      float idx_tr0 = dimuon->get_Tr0_idx();
      float idy_tr0 = dimuon->get_Tr0_idy();
      float dca_r_tr0 = dimuon->get_Tr0_dca_r();
      float dca_z_tr0 = dimuon->get_Tr0_dca_z();
      mRecoEPHistoManager->fillHist_MuonTr0_hits(trhits_tr0,trchi2_tr0,ntrhits_tr0,rapidity_tr0);
      mRecoEPHistoManager->fillHist_MuonTr0_id(idhits_tr0,idchi2_tr0,nidhits_tr0,idx_tr0,idy_tr0,rapidity_tr0);
      mRecoEPHistoManager->fillHist_MuonTr0_QA(DG0_tr0,DDG0_tr0,lastgap_tr0,dca_r_tr0,dca_z_tr0,rapidity_tr0);

      short trhits_tr1 = dimuon->get_Tr1_trhits();
      short idhits_tr1 = dimuon->get_Tr1_idhits();
      float DG0_tr1 = dimuon->get_Tr1_DG0();
      float DDG0_tr1 = dimuon->get_Tr1_DDG0();
      float trchi2_tr1 = dimuon->get_Tr1_trchi2();
      float idchi2_tr1 = dimuon->get_Tr1_idchi2();
      short ntrhits_tr1 = dimuon->get_Tr1_ntrhits();
      short nidhits_tr1 = dimuon->get_Tr1_nidhits();
      short lastgap_tr1 = dimuon->get_Tr1_lastgap();
      float idx_tr1 = dimuon->get_Tr1_idx();
      float idy_tr1 = dimuon->get_Tr1_idy();
      float dca_r_tr1 = dimuon->get_Tr1_dca_r();
      float dca_z_tr1 = dimuon->get_Tr1_dca_z();
      mRecoEPHistoManager->fillHist_MuonTr1_hits(trhits_tr1,trchi2_tr1,ntrhits_tr1,rapidity_tr1);
      mRecoEPHistoManager->fillHist_MuonTr1_id(idhits_tr1,idchi2_tr1,nidhits_tr1,idx_tr1,idy_tr1,rapidity_tr1);
      mRecoEPHistoManager->fillHist_MuonTr1_QA(DG0_tr1,DDG0_tr1,lastgap_tr1,dca_r_tr1,dca_z_tr1,rapidity_tr1);
    }
  }

  if(mMode == 1)  // produce di-muon TTree
  {
    // Event Level
    mDiMuonEvent->clearTrackList();

    // Vertex
    TVector3 vtx_BBC;
    float vtx_BbcX = mDiMuonContainer->get_Evt_vtxX();
    float vtx_BbcY = mDiMuonContainer->get_Evt_vtxY();
    float vtx_BbcZ = mDiMuonContainer->get_Evt_vtxZ();
    vtx_BBC.SetXYZ(vtx_BbcX,vtx_BbcY,vtx_BbcZ);
    mDiMuonEvent->setVtxBbc(vtx_BBC);

    TVector3 vtx_FVTX;
    float vtx_FvtxX = mDiMuonContainer->get_Evt_fvtxX();
    float vtx_FvtxY = mDiMuonContainer->get_Evt_fvtxY();
    float vtx_FvtxZ = mDiMuonContainer->get_Evt_fvtxZ();
    vtx_FVTX.SetXYZ(vtx_FvtxX,vtx_FvtxY,vtx_FvtxZ);
    mDiMuonEvent->setVtxFvtx(vtx_FVTX);

    // Global Info
    mDiMuonEvent->setRunId(mRunId);
    mDiMuonEvent->setRunIndex(mRunIndex);
    mDiMuonEvent->setEventId(eventId);
    mDiMuonEvent->setCentrality(centrality);

    // Event Plane
    for(int i_order = 0; i_order < 3; ++i_order) 
    {
      // BBC Event Plane
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_BBC, 0, i_order));
      float Psi_BbcSouth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_BBC, 1, i_order));
      float Psi_BbcNorth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;

      // FVTX Event Plane => all sectors w/ eta > 1.0
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_FVT, 40, i_order));
      float Psi_FvtxSouth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;
      mReactionPlaneSngl = mReactionPlaneObject->getReactionPlane(RP::calcIdCode(RP::ID_FVT, 41, i_order));
      float Psi_FvtxNorth = (mReactionPlaneSngl) ? mReactionPlaneSngl->GetPsi() : -999.9;

      if(i_order == 0)
      {
	mDiMuonEvent->setPsi_BbcSouth_1st(Psi_BbcSouth);
	mDiMuonEvent->setPsi_BbcNorth_1st(Psi_BbcNorth);
	mDiMuonEvent->setPsi_FvtxSouth_1st(Psi_FvtxSouth);
	mDiMuonEvent->setPsi_FvtxNorth_1st(Psi_FvtxNorth);
      }
      if(i_order == 1)
      {
	mDiMuonEvent->setPsi_BbcSouth_2nd(Psi_BbcSouth);
	mDiMuonEvent->setPsi_BbcNorth_2nd(Psi_BbcNorth);
	mDiMuonEvent->setPsi_FvtxSouth_2nd(Psi_FvtxSouth);
	mDiMuonEvent->setPsi_FvtxNorth_2nd(Psi_FvtxNorth);
      }
      if(i_order == 2)
      {
	mDiMuonEvent->setPsi_BbcSouth_3rd(Psi_BbcSouth);
	mDiMuonEvent->setPsi_BbcNorth_3rd(Psi_BbcNorth);
	mDiMuonEvent->setPsi_FvtxSouth_3rd(Psi_FvtxSouth);
	mDiMuonEvent->setPsi_FvtxNorth_3rd(Psi_FvtxNorth);
      }
    }

    int NumOfDiMuons = 0;
    int NumOfDiMuonsSouth = 0;
    int NumOfDiMuonsNorth = 0;

    // Track Level => di-muon pairs
    for(unsigned int i_dimuon = 0; i_dimuon < mDiMuonContainer->get_nDiMuons(); ++i_dimuon) 
    {
      // create di-muon track
      mDiMuonTrack = mDiMuonEvent->createTrack();

      // di-muon spectra
      DiMuon *dimuon = mDiMuonContainer->get_DiMuon(i_dimuon);
      float InvMass = dimuon->get_mass(); // get di-muon info from DiMuonContainer
      float px = dimuon->get_px();
      float py = dimuon->get_py();
      float pz = dimuon->get_pz();
      float rapidity = dimuon->get_rapidity(); // > 0 => north
      short charge = dimuon->get_charge();
      float dca_r = dimuon->get_dca_r();
      float dca_z = dimuon->get_dca_z();
      float vtx_chi2 = dimuon->get_Evt_vtxchi2();

      mDiMuonTrack->setInvMass(InvMass); // set di-muon info to mTree_DiMuon
      mDiMuonTrack->setPx(px);
      mDiMuonTrack->setPy(py);
      mDiMuonTrack->setPz(pz);
      mDiMuonTrack->setRapidity(rapidity);
      mDiMuonTrack->setCharge(charge);
      mDiMuonTrack->setDcaR(dca_r);
      mDiMuonTrack->setDcaZ(dca_z);
      mDiMuonTrack->setVtxChi2(vtx_chi2);

      NumOfDiMuons++;
      if(rapidity < 0) NumOfDiMuonsSouth++;
      if(rapidity > 0) NumOfDiMuonsNorth++;

      // single muon spectra
      // Tr0
      float px_tr0 = dimuon->get_Tr0_px(); // get Tr0 info from DiMuonContainer
      float py_tr0 = dimuon->get_Tr0_py();
      float pz_tr0 = dimuon->get_Tr0_pz();
      float rapidity_tr0 = dimuon->get_Tr0_rapidity(); // > 0 => north
      float DG0_tr0 = dimuon->get_Tr0_DG0();
      float DDG0_tr0 = dimuon->get_Tr0_DDG0();
      float dca_r_tr0 = dimuon->get_Tr0_dca_r();
      float dca_z_tr0 = dimuon->get_Tr0_dca_z();
      short trhits_tr0 = dimuon->get_Tr0_trhits();
      float trchi2_tr0 = dimuon->get_Tr0_trchi2();
      short ntrhits_tr0 = dimuon->get_Tr0_ntrhits();
      short idhits_tr0 = dimuon->get_Tr0_idhits();
      float idchi2_tr0 = dimuon->get_Tr0_idchi2();
      short nidhits_tr0 = dimuon->get_Tr0_nidhits();
      float idx_tr0 = dimuon->get_Tr0_idx();
      float idy_tr0 = dimuon->get_Tr0_idy();
      short lastgap_tr0 = dimuon->get_Tr0_lastgap();
      
      mDiMuonTrack->setPx_tr0(px_tr0); // set Tr0 info to mTree_DiMuon 
      mDiMuonTrack->setPy_tr0(py_tr0); 
      mDiMuonTrack->setPz_tr0(pz_tr0); 
      mDiMuonTrack->setRapidity_tr0(rapidity_tr0); 
      mDiMuonTrack->setDG0_tr0(DG0_tr0); 
      mDiMuonTrack->setDDG0_tr0(DDG0_tr0); 
      mDiMuonTrack->setDcaR_tr0(dca_r_tr0); 
      mDiMuonTrack->setDcaZ_tr0(dca_z_tr0); 
      mDiMuonTrack->setTrhits_tr0(trhits_tr0); 
      mDiMuonTrack->setTrchi2_tr0(trchi2_tr0); 
      mDiMuonTrack->setNTrhits_tr0(ntrhits_tr0); 
      mDiMuonTrack->setIdhits_tr0(idhits_tr0); 
      mDiMuonTrack->setIdchi2_tr0(idchi2_tr0); 
      mDiMuonTrack->setNIdhits_tr0(nidhits_tr0); 
      mDiMuonTrack->setIdx_tr0(idx_tr0); 
      mDiMuonTrack->setIdy_tr0(idy_tr0); 
      mDiMuonTrack->setLastgap_tr0(lastgap_tr0); 

      // Tr1
      float px_tr1 = dimuon->get_Tr1_px(); // get Tr1 info from DiMuonContainer
      float py_tr1 = dimuon->get_Tr1_py();
      float pz_tr1 = dimuon->get_Tr1_pz();
      float rapidity_tr1 = dimuon->get_Tr1_rapidity(); // > 0 => north
      float DG0_tr1 = dimuon->get_Tr1_DG0();
      float DDG0_tr1 = dimuon->get_Tr1_DDG0();
      float dca_r_tr1 = dimuon->get_Tr1_dca_r();
      float dca_z_tr1 = dimuon->get_Tr1_dca_z();
      short trhits_tr1 = dimuon->get_Tr1_trhits();
      float trchi2_tr1 = dimuon->get_Tr1_trchi2();
      short ntrhits_tr1 = dimuon->get_Tr1_ntrhits();
      short idhits_tr1 = dimuon->get_Tr1_idhits();
      float idchi2_tr1 = dimuon->get_Tr1_idchi2();
      short nidhits_tr1 = dimuon->get_Tr1_nidhits();
      float idx_tr1 = dimuon->get_Tr1_idx();
      float idy_tr1 = dimuon->get_Tr1_idy();
      short lastgap_tr1 = dimuon->get_Tr1_lastgap();
      
      mDiMuonTrack->setPx_tr1(px_tr1); // set Tr1 info to mTree_DiMuon 
      mDiMuonTrack->setPy_tr1(py_tr1); 
      mDiMuonTrack->setPz_tr1(pz_tr1); 
      mDiMuonTrack->setRapidity_tr1(rapidity_tr1); 
      mDiMuonTrack->setDG0_tr1(DG0_tr1); 
      mDiMuonTrack->setDDG0_tr1(DDG0_tr1); 
      mDiMuonTrack->setDcaR_tr1(dca_r_tr1); 
      mDiMuonTrack->setDcaZ_tr1(dca_z_tr1); 
      mDiMuonTrack->setTrhits_tr1(trhits_tr1); 
      mDiMuonTrack->setTrchi2_tr1(trchi2_tr1); 
      mDiMuonTrack->setNTrhits_tr1(ntrhits_tr1); 
      mDiMuonTrack->setIdhits_tr1(idhits_tr1); 
      mDiMuonTrack->setIdchi2_tr1(idchi2_tr1); 
      mDiMuonTrack->setNIdhits_tr1(nidhits_tr1); 
      mDiMuonTrack->setIdx_tr1(idx_tr1); 
      mDiMuonTrack->setIdy_tr1(idy_tr1); 
      mDiMuonTrack->setLastgap_tr1(lastgap_tr1); 
    }
    mDiMuonEvent->setNumOfDiMuons(NumOfDiMuons);
    mDiMuonEvent->setNumOfDiMuonsSouth(NumOfDiMuonsSouth);
    mDiMuonEvent->setNumOfDiMuonsNorth(NumOfDiMuonsNorth);

    mTree_DiMuon->Fill();
  }

  return EVENT_OK;
}

int PhVecMesonMaker::End(PHCompositeNode *topNode) 
{
  File_mOutPut->cd();
  mRecoEPHistoManager->writeQA_Global();

  if(mMode == 0)
  {
    mRecoEPHistoManager->writeHist_BbcEP();
    mRecoEPHistoManager->writeHist_FvtxEP();
    mRecoEPProManager->writePro_BbcResolution();
    mRecoEPProManager->writePro_FvtxResolution();

    mRecoEPHistoManager->writeHist_DiMuonSpec();
    mRecoEPHistoManager->writeHist_DiMuonQA();
    mRecoEPHistoManager->writeHist_MuonTrkSpec();
    mRecoEPHistoManager->writeHist_MuonTrkQA();
  }

  if(mMode == 1) // Produce di-muon TTree
  {
    cout << "Write Di-Muon TTree!" << endl;
    mTree_DiMuon->Write("",TObject::kOverwrite);
  }

  File_mOutPut->Close();

  return EVENT_OK;
}

int PhVecMesonMaker::getNodes(PHCompositeNode *topNode)
{
  mTrigLvl1 = findNode::getClass<TrigLvl1>(topNode, "TrigLvl1");
  if( !mTrigLvl1 )
  {
    std::cout << PHWHERE << "can't find TrigLvl1!" << std::endl;
    return DISCARDEVENT;;//exit(1);
  }

  mVtxOut = findNode::getClass<VtxOut>(topNode,"VtxOut");
  if( !mVtxOut )
  {
    std::cout<<"can't find VtxOut!"<<std::endl;
    return DISCARDEVENT;
  }

  mPHGlobal = findNode::getClass<PHGlobal>(topNode,"PHGlobal");
  if( !mPHGlobal )
  {
    std::cout<<"can't find PHGlobal!"<<std::endl;
    return DISCARDEVENT;
  }

  mEventHeader = findNode::getClass<EventHeader>(topNode,"EventHeader");
  if( !mEventHeader )
  {
    std::cout<<"can't find EventHeader!"<<std::endl;
    return DISCARDEVENT;
  }

  mDiMuonContainer = findNode::getClass<DiMuonContainer>(topNode,"DiMuonContainer");
  if( !mDiMuonContainer )
  {
    std::cout<<"can't find DiMuonContainer!"<<std::endl;
    return DISCARDEVENT;
  }

  mReactionPlaneObject = findNode::getClass<ReactionPlaneObject>(topNode, "ReactionPlaneObject");
  if( !mReactionPlaneObject )
  {
    std::cout<<"can't find ReactionPlaneObject!"<<std::endl;
    return DISCARDEVENT;//exit(1);
  }

  return EVENT_OK;
}

bool PhVecMesonMaker::isMinimumBias()
{
  // Au+Au @ Run14 trigger selection
  // const unsigned int BBCLL1_narrowvtx = 0x00000010; // 4 - BBCLL1(>1 tubes) narrowvtx
  // const unsigned int BBCLL1           = 0x00000020; // 5 - BBCLL1(>1 tubes)
  // const unsigned int BBCLL1_novertex  = 0x00000040; // 6 - BBCLL1(>1 tubes) novertex
  const int TRIGGERBIT = 5;
  bool trigscaled_on = false;
  trigscaled_on = mTrigLvl1->get_lvl1_trigscaled_bit(TRIGGERBIT);
  // cout << "TRIGGERBIT = " << TRIGGERBIT << ", trigscaled_on = " << trigscaled_on << endl;

  // ZDC coincidence
  float zdc1 = mPHGlobal->getZdcEnergyN();
  float zdc2 = mPHGlobal->getZdcEnergyS();
  float zdcz = mPHGlobal->getZdcZVertex();
  bool isZDCOK = (zdc1>0 && zdc2>0 && zdcz > -9000);

  // BBC vertex cut
  float vtx_bbcz = mPHGlobal->getBbcZVertex();
  bool isBbcOK = fabs(vtx_bbcz) < 30;

  return trigscaled_on && isZDCOK && isBbcOK;
}

bool PhVecMesonMaker::isMinimumBiasNarrowVtx()
{
  // Au+Au @ Run14 trigger selection
  // const unsigned int BBCLL1_narrowvtx = 0x00000010; // 4 - BBCLL1(>1 tubes) narrowvtx
  // const unsigned int BBCLL1           = 0x00000020; // 5 - BBCLL1(>1 tubes)
  // const unsigned int BBCLL1_novertex  = 0x00000040; // 6 - BBCLL1(>1 tubes) novertex
  // const int TRIGGERBIT = mRC->get_IntFlag("TRIGGER_BIT",4);
  const int TRIGGERBIT = 4;
  bool trigscaled_on = false;
  trigscaled_on = mTrigLvl1->get_lvl1_trigscaled_bit(TRIGGERBIT);
  // cout << "TRIGGERBIT = " << TRIGGERBIT << ", trigscaled_on = " << trigscaled_on << endl;

  // ZDC coincidence
  float zdc1 = mPHGlobal->getZdcEnergyN();
  float zdc2 = mPHGlobal->getZdcEnergyS();
  float zdcz = mPHGlobal->getZdcZVertex();
  bool isZDCOK = (zdc1>0 && zdc2>0 && zdcz > -9000);

  // BBC vertex cut
  float vtx_bbcz = mPHGlobal->getBbcZVertex();
  bool isBbcOK = fabs(vtx_bbcz) < 30;

  return trigscaled_on && isZDCOK && isBbcOK;
}
