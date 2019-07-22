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

#include <Bbc.hh>
#include <BbcRaw.h>
#include <BbcOut.h>
#include <BbcCalib.hh>
#include <BbcGeo.hh>

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
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile2D.h>
#include <TMath.h>
#include <TString.h>

//Utility Class
#include "RecoEPHistoManager.h"
#include "RecoEPProManager.h"
#include "RecoEPUtility.h"

using namespace std;

PhVecMesonMaker::PhVecMesonMaker(const char*outputfile)
{
  mRC = NULL;
  // RC flags
  mRunSelection    = 0;
  mSystemSelection = 0;
  mBbczCut_val     = 10.0;
  mBbcPmt_flag     = 0;
  mMode            = 0;
  mDebug_Bbc       = 0;
  mQA_Bbc          = 0;

  // PH class
  mPHGlobal  = NULL;
  mTrigLvl1  = NULL;
  mRunHeader = NULL;
  mRunId     = -999;
  mRunIndex  = -999;
  mVtxOut    = NULL; // vertex
  mBbcCalib  = NULL;
  mBbcGeo    = NULL;

  mOutPutFile  = outputfile;
  File_mOutPut = NULL; // output file

  mNumOfEvents = 0; // number of events

  mRecoEPHistoManager = NULL;
  mRecoEPProManager   = NULL;
  mRecoEPUtility      = NULL;
}

PhVecMesonMaker::~PhVecMesonMaker()
{
  delete mBbcCalib;
  delete mBbcGeo;

  delete mRecoEPUtility; // delete utilities
  delete mRecoEPHistoManager; // delete histograms manager
  delete mRecoEPProManager; // delete  profiles manager
}

int PhVecMesonMaker::Init(PHCompositeNode *topNode) 
{
  cout << "PhVecMesonMaker::Init => " << endl;

  cout << "Setting Module Flags: " << endl;
  mRC              = recoConsts::instance();
  mRunSelection    = mRC->get_IntFlag("RUN_SELECTION", 0);
  mSystemSelection = mRC->get_IntFlag("SYSTEM_SELECTION", 0);
  mBbczCut_val     = mRC->get_DoubleFlag("BBCZCUT_VAL", 10);
  mBbcPmt_flag     = mRC->get_IntFlag("EP_BBC", 0);
  mMode            = mRC->get_IntFlag("ANA_MODE", 0);
  mQA_Bbc          = mRC->get_IntFlag("QA_BBC", 1);
  mDebug_Bbc       = mRC->get_IntFlag("DEBUG_BBC", 0);

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
  mRecoEPProManager   = new RecoEPProManager(); // initialize histograms

  if(mMode == 0) // generate DiMuon TTree
  {
    cout << "Produce Di-Muon TTree!" << endl;
    mRecoEPHistoManager->initQA_Global();
    mRecoEPHistoManager->initHist_BbcEP();
    mRecoEPHistoManager->initHist_FvtxEP();
    mRecoEPProManager->initPro_BbcResolution();
    mRecoEPProManager->initPro_FvtxResolution();
    mRecoEPHistoManager->initHist_DiMuonSpec();
    mRecoEPHistoManager->initHist_DiMuonQA();
    mRecoEPHistoManager->initHist_MuonTrkSpec();
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


  if(mMode == 0)  // Di-Muon TTree Production
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
    }
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
  }

  File_mOutPut->Close();

  return EVENT_OK;
}

int PhVecMesonMaker::getNodes(PHCompositeNode *topNode)
{
  mTrigLvl1 = findNode::getClass<TrigLvl1>(topNode, "TrigLvl1");
  if( !mTrigLvl1 )
  {
    std::cout << PHWHERE << "can't find TrigLvl1 Node" << std::endl;
    return DISCARDEVENT;;//exit(1);
  }

  mVtxOut = findNode::getClass<VtxOut>(topNode,"VtxOut");
  if( !mVtxOut )
  {
    std::cout<<"can't find VtxOut "<<std::endl;
    return DISCARDEVENT;
  }

  mPHGlobal = findNode::getClass<PHGlobal>(topNode,"PHGlobal");
  if( !mPHGlobal )
  {
    std::cout<<"can't find PHGlobal "<<std::endl;
    return DISCARDEVENT;
  }

  mDiMuonContainer = findNode::getClass<DiMuonContainer>(topNode,"DiMuonContainer");
  if( !mDiMuonContainer )
  {
    std::cout<<"can't find DiMuonContainer "<<std::endl;
    return DISCARDEVENT;
  }

  mReactionPlaneObject = findNode::getClass<ReactionPlaneObject>(topNode, "ReactionPlaneObject");
  if( !mReactionPlaneObject )
  {
    std::cout<<"can't find ReactionPlaneObject "<<std::endl;
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
