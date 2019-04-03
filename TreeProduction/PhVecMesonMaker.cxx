//local
#include "PhVecMesonMaker.h"

//PHENIX
#include <Fun4AllServer.h>
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>
#include <PHGlobal.h>
#include <PHCompositeNode.h>

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
#include "RecoEventPlane.h"

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
  mRunHeader = NULL;
  mRunId     = -999;
  mVtxOut    = NULL; // vertex
  mBbcRaw    = NULL; // BBC
  mBbcCalib  = NULL;
  mBbcGeo    = NULL;


  mOutPutFile  = outputfile;
  File_mOutPut = NULL; // output file

  mRecoEPHistoManager = NULL;
  mRecoEPProManager   = NULL;
  mRecoEPUtility      = NULL;
  mRecoEventPlane     = NULL;
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
  mRecoEventPlane     = new RecoEventPlane(); // initialize utilities
  mRecoEPHistoManager = new RecoEPHistoManager(); // initialize histograms
  mRecoEPHistoManager->initQA_Global();
  mRecoEPProManager   = new RecoEPProManager(); // initialize histograms

  if(mMode == 0) // fill re-center parameters & raw event plane
  {
    cout << "fill re-center parameters & raw event plane!" << endl;
    mRecoEPUtility->initBBC();
    mRecoEventPlane->initRawBbcEventPlane();
    mRecoEPHistoManager->initHist_BbcRawEP();
    mRecoEPHistoManager->initHist_BbcRawQVector();
    mRecoEPProManager->initPro_BbcReCenter();
    if(mQA_Bbc == 1)
    {
      cout << "fill QA for BBC!" << endl;
      mRecoEPHistoManager->set_debug(mDebug_Bbc);
      mRecoEPHistoManager->initQA_BbcCharge();
      mRecoEPHistoManager->initQA_BbcChargeReCalib();
      if(mDebug_Bbc == 1) 
      {
	cout << "Start debugging for BBC!" << endl;
	mRecoEPHistoManager->initQA_BbcAdc();
      }
    }
  }
  if(mMode == 1) // fill shift parameters & re-centered event plane
  {
    cout << "fill shift parameters & re-centered event plane!" << endl;
    mRecoEPUtility->initBBC();
    mRecoEventPlane->initRawBbcEventPlane();
    mRecoEventPlane->initReCenterBbcEventPlane();
    bool isReCenter = mRecoEventPlane->readPro_ReCenter();
    if(!isReCenter) return ABORTRUN;
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
    cout << "could not find in good run list! & drop the run!" << endl;
    return ABORTRUN;
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
  if(mNumOfEvents%1000 == 0) cout << "processing events:  " << mNumOfEvents << "/" << 10000 << endl;
  // if(mNumOfEvents > 100) return ABORTRUN;

  float zdc1 = mPHGlobal->getZdcEnergyN();
  float zdc2 = mPHGlobal->getZdcEnergyS();
  float zdcz = mPHGlobal->getZdcZVertex();
  bool isZDCOK = (zdc1>0 && zdc2>0 && zdcz > -9000);
  if (!isZDCOK) return DISCARDEVENT;

  float vtx_bbcz = mPHGlobal->getBbcZVertex();
  float centrality = mPHGlobal->getCentrality();
  mRecoEPHistoManager->fillQA_Global_nocuts(zdcz,vtx_bbcz,centrality);

  if ( vtx_bbcz >= mBbczCut_val || vtx_bbcz <= -(mBbczCut_val)) return DISCARDEVENT; // apply bbc cuts
  mRecoEPHistoManager->fillQA_Global(zdcz,vtx_bbcz,centrality);

  if(mMode == 0)  // fill re-center & raw event plane
  {
    for (int i_pmt=0; i_pmt<128; i_pmt++) 
    {
      short adc = mBbcRaw->get_Adc(i_pmt);
      short tdc = mBbcRaw->get_Tdc0(i_pmt);
      float time0 = mBbcCalib->getHitTime0(i_pmt, tdc, adc);
      float charge = mBbcCalib->getCharge(i_pmt, adc);
      float bbcx = mBbcGeo->getX(i_pmt);
      float bbcy = mBbcGeo->getY(i_pmt);
      float bbcz = mBbcGeo->getZ(i_pmt);

      if(time0 > 0 && charge > 0)
      {
	const float charge_recalib = mRecoEPUtility->get_recal_charge(i_pmt,mRunId,adc);

	for(int i_order = 0; i_order < 3; ++i_order) // 0 for 1st, 1 for 2nd, 2 for 3rd
	{
	  if(i_pmt < 64)
	  { // south
	    mRecoEventPlane->addQVectorRaw_BbcSouth(bbcx,bbcy,charge_recalib,i_order);
	  }
	  else
	  { // north
	    mRecoEventPlane->addQVectorRaw_BbcNorth(bbcx,bbcy,charge_recalib,i_order);
	  }
	}
	if(mQA_Bbc == 1)
	{
	  if(mDebug_Bbc == 1) mRecoEPHistoManager->fillQA_BbcAdc(i_pmt,adc); // fill Adc QA
	  mRecoEPHistoManager->fillQA_BbcCharge(i_pmt,bbcx,bbcy,bbcz,charge);
	  mRecoEPHistoManager->fillQA_BbcChargeReCalib(i_pmt,bbcx,bbcy,bbcz,charge_recalib);
	}
      }
    }

    const int cent20 = mRecoEPUtility->getCentralityBin20(centrality);
    const int cent10 = mRecoEPUtility->getCentralityBin10(centrality);
    // const int cent4  = mRecoEPUtility->getCentralityBin4(centrality);
    const int vtx4   = mRecoEPUtility->getVertexBin(vtx_bbcz);
    // cout << "vertex = " << vtx_bbcz << ", vtx4 = " << vtx4 << ", centrality = " << centrality << ", cent20 = " << cent20 << ", cent10 = " << cent10 << ", cent4 = " << cent4 << endl;
    for(int i_order = 0; i_order < 3; ++i_order)
    {
      float PsiRaw_BbcSouth = mRecoEventPlane->calPsiRaw_BbcSouth(i_order);
      float PsiRaw_BbcNorth = mRecoEventPlane->calPsiRaw_BbcNorth(i_order);
      mRecoEPHistoManager->fillHist_BbcRawEP(PsiRaw_BbcSouth,PsiRaw_BbcNorth,i_order,cent20);

      TVector2 QVec_BbcSouth = mRecoEventPlane->getQVectorRaw_BbcSouth(i_order);
      float QWeight_BbcSouth = mRecoEventPlane->getQWeight_BbcSouth(i_order);
      mRecoEPProManager->fillPro_BbcSouthReCenter(QVec_BbcSouth,QWeight_BbcSouth,i_order,vtx4,mRunId,cent20); // fill re-center parameters

      TVector2 QVec_BbcNorth = mRecoEventPlane->getQVectorRaw_BbcNorth(i_order);
      float QWeight_BbcNorth = mRecoEventPlane->getQWeight_BbcNorth(i_order);
      mRecoEPProManager->fillPro_BbcNorthReCenter(QVec_BbcNorth,QWeight_BbcNorth,i_order,vtx4,mRunId,cent20); // fill re-center parameters

      // mRecoEPHistoManager->fillHist_BbcRawQVector(QVec_BbcSouth,QWeight_BbcSouth,QVec_BbcNorth,QWeight_BbcNorth,i_order,cent20);
      mRecoEPHistoManager->fillHist_BbcRawQVector(QVec_BbcSouth,QWeight_BbcSouth,QVec_BbcNorth,QWeight_BbcNorth,i_order,cent10);
    }
    // mRecoEventPlane->printRawBbcEventPlane(0);
    // mRecoEventPlane->printRawBbcEventPlane(1);
    // mRecoEventPlane->printRawBbcEventPlane(2);
    mRecoEventPlane->clearRawBbcEventPlane();
  }
  if(mMode == 1)  // fill shift parameters & re-centered event plane
  {
    for (int i_pmt=0; i_pmt<128; i_pmt++) 
    {
      short adc = mBbcRaw->get_Adc(i_pmt);
      short tdc = mBbcRaw->get_Tdc0(i_pmt);
      float time0 = mBbcCalib->getHitTime0(i_pmt, tdc, adc);
      float charge = mBbcCalib->getCharge(i_pmt, adc);
      float bbcx = mBbcGeo->getX(i_pmt);
      float bbcy = mBbcGeo->getY(i_pmt);

      if(time0 > 0 && charge > 0)
      {
	const float charge_recalib = mRecoEPUtility->get_recal_charge(i_pmt,mRunId,adc);

	for(int i_order = 0; i_order < 3; ++i_order) // 0 for 1st, 1 for 2nd, 2 for 3rd
	{
	  if(i_pmt < 64)
	  { // south
	    mRecoEventPlane->addQVectorRaw_BbcSouth(bbcx,bbcy,charge_recalib,i_order);
	  }
	  else
	  { // north
	    mRecoEventPlane->addQVectorRaw_BbcNorth(bbcx,bbcy,charge_recalib,i_order);
	  }
	}
      }
    }

    const int cent20 = mRecoEPUtility->getCentralityBin20(centrality);
    // const int cent10 = mRecoEPUtility->getCentralityBin10(centrality);
    // const int cent4  = mRecoEPUtility->getCentralityBin4(centrality);
    const int vtx4   = mRecoEPUtility->getVertexBin(vtx_bbcz);
    // cout << "vertex = " << vtx_bbcz << ", vtx4 = " << vtx4 << ", centrality = " << centrality << ", cent20 = " << cent20 << endl;
    for(int i_order = 0; i_order < 3; ++i_order)
    {
      TVector2 QVecReCenter_BbcSouth = mRecoEventPlane->getQVectorReCenter_BbcSouth(i_order,vtx4,mRunId,cent20);
      cout << "QVecReCenter_BbcSouth.X = " << QVecReCenter_BbcSouth.X() << ", QVecReCenter_BbcSouth.Y() = " << QVecReCenter_BbcSouth.Y() << endl;
      TVector2 QVecReCenter_BbcNorth = mRecoEventPlane->getQVectorReCenter_BbcNorth(i_order,vtx4,mRunId,cent20);
      cout << "QVecReCenter_BbcNorth.X = " << QVecReCenter_BbcNorth.X() << ", QVecReCenter_BbcNorth.Y() = " << QVecReCenter_BbcNorth.Y() << endl;
    }

    /*
    mRecoEventPlane->printRawBbcEventPlane(0);
    mRecoEventPlane->printReCenterBbcEventPlane(0,vtx4,mRunId,cent20);

    mRecoEventPlane->printRawBbcEventPlane(1);
    mRecoEventPlane->printReCenterBbcEventPlane(1,vtx4,mRunId,cent20);

    mRecoEventPlane->printReCenterBbcEventPlane(2,vtx4,mRunId,cent20);
    mRecoEventPlane->printRawBbcEventPlane(2);
    */

    mRecoEventPlane->clearRawBbcEventPlane();
    mRecoEventPlane->clearReCenterBbcEventPlane();
  }

  return EVENT_OK;
}

int PhVecMesonMaker::End(PHCompositeNode *topNode) 
{
  File_mOutPut->cd();
  mRecoEPHistoManager->writeQA_Global();

  if(mMode == 0)
  {
    mRecoEPHistoManager->writeHist_BbcRawEP();
    mRecoEPHistoManager->writeHist_BbcRawQVector();
    mRecoEPProManager->writePro_BbcReCenter();

    if(mQA_Bbc == 1)
    {
      if(mDebug_Bbc == 1) mRecoEPHistoManager->writeQA_BbcAdc();
      mRecoEPHistoManager->writeQA_BbcCharge();
      mRecoEPHistoManager->writeQA_BbcChargeReCalib();
    }
  }
  if(mMode == 1)
  {
    mRecoEventPlane->closePro_ReCenter();
  }

  File_mOutPut->Close();

  return EVENT_OK;
}

int PhVecMesonMaker::getNodes(PHCompositeNode *topNode)
{
  mPHGlobal = findNode::getClass<PHGlobal>(topNode,"PHGlobal");
  if( !mPHGlobal )
  {
    std::cout<<"can't find PHGlobal "<<std::endl;
    return DISCARDEVENT;
  }

  mBbcRaw = findNode::getClass<BbcRaw>(topNode,"BbcRaw");
  if (!mBbcRaw)
  {
    std::cout << PHWHERE << "Could not find BbcRaw !" << std::endl;
    return DISCARDEVENT;
  }

  mVtxOut = findNode::getClass<VtxOut>(topNode,"VtxOut");
  if( !mVtxOut )
  {
    std::cout<<"can't find VtxOut "<<std::endl;
    return DISCARDEVENT;
  }

  return EVENT_OK;
}
