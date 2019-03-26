//local
#include "RecoEventPlane.h"

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
#include "RecoEPUtility.h"

using namespace std;

RecoEventPlane::RecoEventPlane(const char*outputfile)
{
  mRC = NULL;
  // RC flags
  mRunSelection    = 0;
  mSystemSelection = 0;
  mEPCalib         = 0;
  mBbczCut_val     = 10.0;
  mDebug           = 0;
  mBbcPmt_flag     = 0;

  mPHGlobal = NULL;
  mCentrality = -999;

  mRunHeader = NULL;
  mRunId = -999;

  mVtxOut = NULL; // vertex

  mBbcRaw = NULL; // BBC
  mBbcCalib = NULL;
  mBbcGeo = NULL;


  mNumOfEvents = 0; // number of events
  mMode = 0;
  File_mOutPut = NULL; // output file
  mOutPutFile = outputfile;

  mRecoEPHistoManager = NULL;
}

int RecoEventPlane::Init(PHCompositeNode *topNode) 
{
  cout << "RecoEventPlane::Init => " << endl;

  cout << "Setting Module Flags: " << endl;
  mRC              = recoConsts::instance();
  mRunSelection    = mRC->get_IntFlag("RD_RUN_SELECTION", 0);
  mSystemSelection = mRC->get_IntFlag("RD_SYSTEM_SELECTION", 0);
  mDebug           = mRC->get_IntFlag("EP_DEBUG", 0);
  mBbczCut_val     = mRC->get_DoubleFlag("RD_BBCZCUT_VAL", 10);
  mBbcPmt_flag     = mRC->get_IntFlag("EP_BBC", 0);
  mEPCalib         = mRC->get_IntFlag("EP_CALIB", 0);

  cout << "run " << mRunSelection << " @ system ";
  if(mRunSelection == 14 && mSystemSelection == 0) cout << "Au+Au" << endl;
  cout << "BBCZ cut value is " << mBbczCut_val << endl;

  cout << "BBC phi bin correction status is: " << mEPCalib <<std::endl;
  if(mBbcPmt_flag == 0) cout << "do nothing" << endl;
  if(mBbcPmt_flag > 0)  cout << "hot/cold (20\% off average) phi bin removed" << endl;

  // init output structure
  File_mOutPut = TFile::Open(mOutPutFile.data(), "recreate");
  if(!File_mOutPut) cout<<"Cann't create " << mOutPutFile.data() <<endl;

  mNumOfEvents = 0;

  // initialize histograms
  mRecoEPHistoManager = new RecoEPHistoManager();
  mRecoEPHistoManager->set_debug(mDebug);
  mRecoEPHistoManager->initQA_Global();
  if(mDebug == 1) mRecoEPHistoManager->initQA_BbcAdc();
  mRecoEPHistoManager->initQA_BbcCharge();
  mRecoEPHistoManager->initQA_BbcChargeReCalib();
  if(mEPCalib == 0) mRecoEPHistoManager->initHist_BbcRawEP();

  // initialize utilities
  mRecoEPUtility = new RecoEPUtility();
  mRecoEPUtility->read_in_recal_consts();

  return EVENT_OK;
}

int RecoEventPlane::InitRun(PHCompositeNode *topNode) 
{
  cout << "RecoEventPlane::InitRun => " << endl;
  cout << "get run header info" << endl;
  mRunHeader = findNode::getClass<RunHeader>(topNode,"RunHeader");
  if( !mRunHeader )
  {
    cout << "can't find RunHeader == > exit!!!" << endl;
    exit(1);
  }

  mRunId = mRunHeader->get_RunNumber();
  cout << "runId is "<< mRunId << endl;

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

int RecoEventPlane::process_event(PHCompositeNode *topNode) 
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

  float bbcz = mPHGlobal->getBbcZVertex();
  float cent = mPHGlobal->getCentrality();

  if ( bbcz >= mBbczCut_val || bbcz <= -(mBbczCut_val)) return DISCARDEVENT; // apply bbc cuts
  mRecoEPHistoManager->fillQA_Global(zdcz,bbcz,cent);

  float Q1x_South = 0.0;
  float Q1y_South = 0.0;
  float Q1x_North = 0.0;
  float Q1y_North = 0.0;

  const float BBCs_xoff = -0.5;
  const float BBCs_yoff = 0.0;
  const float BBCn_xoff = 7.0;
  const float BBCn_yoff = 0.0;

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
      if(mDebug == 1) mRecoEPHistoManager->fillQA_BbcAdc(i_pmt,adc); // fill Adc QA
      mRecoEPHistoManager->fillQA_BbcCharge(i_pmt,bbcx,bbcy,bbcz,charge);

      float charge_recalib = mRecoEPUtility->get_recal_charge(i_pmt,mRunId,adc);
      mRecoEPHistoManager->fillQA_BbcChargeReCalib(i_pmt,bbcx,bbcy,bbcz,charge_recalib);

      if(mEPCalib == 0)
      { // calculate Raw Qvector
	if(i_pmt < 64)
	{ // south
	  float phi = atan2(bbcy-BBCs_yoff,bbcx-BBCs_xoff);
	  float weight = charge_recalib;
	  Q1x_South += weight*cos(phi);
	  Q1y_South += weight*sin(phi);
	}
	if(i_pmt >= 64)
	{ // north
	  float phi = atan2(bbcy-BBCn_yoff,bbcx-BBCn_xoff);
	  float weight = charge_recalib;
	  Q1x_North += weight*cos(phi);
	  Q1y_North += weight*sin(phi);
	}
      }
    }
  }

  if(mEPCalib == 0 && cent > 20 && cent < 30)
  {
    float Psi1st_BbcSouth = atan2(Q1y_South,Q1x_South);
    float Psi1st_BbcNorth = atan2(Q1y_North,Q1x_North);
    mRecoEPHistoManager->fillHist_BbcRawEP(Psi1st_BbcSouth,Psi1st_BbcNorth,Psi1st_BbcSouth,Psi1st_BbcNorth,Psi1st_BbcSouth,Psi1st_BbcNorth);
  }

  return EVENT_OK;
}

int RecoEventPlane::End(PHCompositeNode *topNode) 
{
  File_mOutPut->cd();
  mRecoEPHistoManager->writeQA_Global();
  if(mDebug == 1) mRecoEPHistoManager->writeQA_BbcAdc();
  mRecoEPHistoManager->writeQA_BbcCharge();
  mRecoEPHistoManager->writeQA_BbcChargeReCalib();
  if(mEPCalib == 0) mRecoEPHistoManager->writeHist_BbcRawEP();

  File_mOutPut->Close();

  return EVENT_OK;
}

int RecoEventPlane::getNodes(PHCompositeNode *topNode)
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
