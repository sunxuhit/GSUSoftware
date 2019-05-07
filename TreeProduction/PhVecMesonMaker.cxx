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
  mTrigLvl1  = NULL;
  mRunHeader = NULL;
  mRunId     = -999;
  mRunIndex  = -999;
  mVtxOut    = NULL; // vertex
  mBbcRaw    = NULL; // BBC
  mBbcCalib  = NULL;
  mBbcGeo    = NULL;

  mOutPutFile  = outputfile;
  File_mOutPut = NULL; // output file

  mNumOfEvents = 0; // number of events

  mRecoEPHistoManager = NULL;
  mRecoEPProManager   = NULL;
  mRecoEPUtility      = NULL;
  mRecoEventPlane     = NULL;
}

PhVecMesonMaker::~PhVecMesonMaker()
{
  delete mBbcCalib;
  delete mBbcGeo;

  delete mRecoEPUtility; // delete utilities
  delete mRecoEventPlane; // delete event plane
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
  mRecoEventPlane     = new RecoEventPlane(); // initialize utilities
  mRecoEPUtility->initRunIndex();
  mRecoEPUtility->initBBC();
  mRecoEPHistoManager = new RecoEPHistoManager(); // initialize histograms
  mRecoEPHistoManager->initQA_Global();
  mRecoEPProManager   = new RecoEPProManager(); // initialize histograms

  if(mMode == 0) // fill re-center parameters & raw event plane
  {
    cout << "fill re-center parameters & raw event plane!" << endl;
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
      mRecoEPHistoManager->initQA_BbcChargePhiWeight();
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
    mRecoEventPlane->initRawBbcEventPlane();
    mRecoEventPlane->initReCenterBbcEventPlane();
    bool isReCenter = mRecoEventPlane->readPro_ReCenter();
    if(!isReCenter) 
    {
      cout << "Could NOT find the re-center parameters! & drop the run!" << endl;
      return ABORTRUN;
    }
    mRecoEPHistoManager->initHist_BbcReCenterEP();
    mRecoEPHistoManager->initHist_BbcReCenterQVector();
    mRecoEPProManager->initPro_BbcShift();
  }
  if(mMode == 2) // calculate resolution & fill shifted event plane
  {
    cout << "calculate resolution & fill shifted event plane!" << endl;
    mRecoEventPlane->initRawBbcEventPlane();
    mRecoEventPlane->initReCenterBbcEventPlane();
    bool isReCenter = mRecoEventPlane->readPro_ReCenter();
    if(!isReCenter) 
    {
      cout << "Could NOT find the re-center parameters! & drop the run!" << endl;
      return ABORTRUN;
    }
    bool isShift = mRecoEventPlane->readPro_Shift();
    if(!isShift) 
    {
      cout << "Could NOT find the shift parameters! & drop the run!" << endl;
      return ABORTRUN;
    }
    mRecoEPHistoManager->initHist_BbcShiftEP();
    mRecoEPProManager->initPro_BbcResolution();
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

  // float zdc1 = mPHGlobal->getZdcEnergyN();
  // float zdc2 = mPHGlobal->getZdcEnergyS();
  // float zdcz = mPHGlobal->getZdcZVertex();
  // bool isZDCOK = (zdc1>0 && zdc2>0 && zdcz > -9000);
  // if (!isZDCOK) return DISCARDEVENT;

  // float vtx_bbcz = mPHGlobal->getBbcZVertex();

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

  // calculate raw Q-Vector for BBC
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
      const float phi_weight = mRecoEPUtility->get_phiweight_correction(i_pmt,mRunId,centrality);
      const float weight_BbcEP = charge_recalib*phi_weight; // charge recalibration & phi-weight correction

      for(int i_order = 0; i_order < 3; ++i_order) // 0 for 1st, 1 for 2nd, 2 for 3rd
      {
	if(i_pmt < 64)
	{ // south
	  mRecoEventPlane->addQVectorRaw_BbcSouth(bbcx,bbcy,weight_BbcEP,i_order);
	  // mRecoEventPlane->addQVectorRaw_BbcSouth(bbcx,bbcy,charge,i_order); // try to re-produce Takashi's results
	}
	else
	{ // north
	  mRecoEventPlane->addQVectorRaw_BbcNorth(bbcx,bbcy,weight_BbcEP,i_order);
	  // mRecoEventPlane->addQVectorRaw_BbcNorth(bbcx,bbcy,charge,i_order); // try to re-produce Takashi's results
	}
      }
    }
  }

  const int cent20 = mRecoEPUtility->getCentralityBin20(centrality);
  // const int cent10 = mRecoEPUtility->getCentralityBin10(centrality);
  // const int cent4  = mRecoEPUtility->getCentralityBin4(centrality);
  const int vtx4   = mRecoEPUtility->getVertexBin(vtx_bbcz);
  // cout << "centrality = " << centrality << ", cent20 = " << cent20 << ", cent10 = " << cent10 << ", vtx4 = " << vtx4 << endl;

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

      if(time0 > 0 && charge > 0 && mQA_Bbc == 1)
      {
	const float charge_recalib = mRecoEPUtility->get_recal_charge(i_pmt,mRunId,adc);
	const float phi_weight = mRecoEPUtility->get_phiweight_correction(i_pmt,mRunId,centrality);
	const float weight_BbcEP = charge_recalib*phi_weight;

	if(mDebug_Bbc == 1) mRecoEPHistoManager->fillQA_BbcAdc(i_pmt,adc); // fill Adc QA
	mRecoEPHistoManager->fillQA_BbcCharge(i_pmt,bbcx,bbcy,bbcz,charge);
	mRecoEPHistoManager->fillQA_BbcChargeReCalib(i_pmt,bbcx,bbcy,bbcz,charge_recalib);
	mRecoEPHistoManager->fillQA_BbcChargePhiWeight(i_pmt,bbcx,bbcy,bbcz,weight_BbcEP);
      }
    }

    for(int i_order = 0; i_order < 3; ++i_order)
    {
      float PsiRaw_BbcSouth = mRecoEventPlane->calPsiRaw_BbcSouth(i_order);
      float PsiRaw_BbcNorth = mRecoEventPlane->calPsiRaw_BbcNorth(i_order);
      mRecoEPHistoManager->fillHist_BbcRawEP(PsiRaw_BbcSouth,PsiRaw_BbcNorth,i_order,cent20);

      TVector2 QVecRaw_BbcSouth = mRecoEventPlane->getQVectorRaw_BbcSouth(i_order);
      float QWeight_BbcSouth = mRecoEventPlane->getQWeight_BbcSouth(i_order);
      mRecoEPProManager->fillPro_BbcSouthReCenter(QVecRaw_BbcSouth,QWeight_BbcSouth,i_order,vtx4,mRunIndex,cent20); // fill re-center parameters

      TVector2 QVecRaw_BbcNorth = mRecoEventPlane->getQVectorRaw_BbcNorth(i_order);
      float QWeight_BbcNorth = mRecoEventPlane->getQWeight_BbcNorth(i_order);
      mRecoEPProManager->fillPro_BbcNorthReCenter(QVecRaw_BbcNorth,QWeight_BbcNorth,i_order,vtx4,mRunIndex,cent20); // fill re-center parameters

      mRecoEPHistoManager->fillHist_BbcRawQVector(QVecRaw_BbcSouth,QWeight_BbcSouth,QVecRaw_BbcNorth,QWeight_BbcNorth,i_order,cent20);
    }

    // mRecoEventPlane->printRawBbcEventPlane(0);
    // mRecoEventPlane->printRawBbcEventPlane(1);
    // mRecoEventPlane->printRawBbcEventPlane(2);

    mRecoEventPlane->clearRawBbcEventPlane();
  }
  if(mMode == 1)  // fill shift parameters & re-centered event plane
  {
    for(int i_order = 0; i_order < 3; ++i_order)
    {
      float PsiReCenter_BbcSouth = mRecoEventPlane->calPsiReCenter_BbcSouth(i_order,vtx4,mRunIndex,cent20);
      mRecoEPProManager->fillPro_BbcSouthShift(PsiReCenter_BbcSouth, i_order, vtx4, mRunIndex, cent20); // fill shift parater

      float PsiReCenter_BbcNorth = mRecoEventPlane->calPsiReCenter_BbcNorth(i_order,vtx4,mRunIndex,cent20);
      mRecoEPProManager->fillPro_BbcNorthShift(PsiReCenter_BbcNorth, i_order, vtx4, mRunIndex, cent20); // fill shift parater

      mRecoEPHistoManager->fillHist_BbcReCenterEP(PsiReCenter_BbcSouth,PsiReCenter_BbcNorth,i_order,cent20);

      TVector2 QVecReCenter_BbcSouth = mRecoEventPlane->getQVectorReCenter_BbcSouth(i_order,vtx4,mRunIndex,cent20);
      TVector2 QVecReCenter_BbcNorth = mRecoEventPlane->getQVectorReCenter_BbcNorth(i_order,vtx4,mRunIndex,cent20);
      mRecoEPHistoManager->fillHist_BbcReCenterQVector(QVecReCenter_BbcSouth,QVecReCenter_BbcNorth,i_order,cent20);
    }

    /*
    mRecoEventPlane->printRawBbcEventPlane(0);
    mRecoEventPlane->printReCenterBbcEventPlane(0,vtx4,mRunIndex,cent20);

    mRecoEventPlane->printRawBbcEventPlane(1);
    mRecoEventPlane->printReCenterBbcEventPlane(1,vtx4,mRunIndex,cent20);

    mRecoEventPlane->printReCenterBbcEventPlane(2,vtx4,mRunIndex,cent20);
    mRecoEventPlane->printRawBbcEventPlane(2);
    */

    mRecoEventPlane->clearRawBbcEventPlane();
    mRecoEventPlane->clearReCenterBbcEventPlane();
  }
  if(mMode == 2)  // calculate resolution & fill shifted event plane
  {
    for(int i_order = 0; i_order < 3; ++i_order)
    {
      float PsiReCenter_BbcSouth = mRecoEventPlane->calPsiReCenter_BbcSouth(i_order,vtx4,mRunIndex,cent20);
      float PsiShift_BbcSouth = mRecoEventPlane->calPsiShift_BbcSouth(PsiReCenter_BbcSouth,i_order,vtx4,mRunIndex,cent20); // calcualte shift EP
      // cout << "i_order = " << i_order << ", PsiReCenter_BbcSouth = " << PsiReCenter_BbcSouth << ", PsiShift_BbcSouth = " << PsiShift_BbcSouth << endl;

      float PsiReCenter_BbcNorth = mRecoEventPlane->calPsiReCenter_BbcNorth(i_order,vtx4,mRunIndex,cent20);
      float PsiShift_BbcNorth = mRecoEventPlane->calPsiShift_BbcNorth(PsiReCenter_BbcNorth,i_order,vtx4,mRunIndex,cent20); // calcualte shift EP
      // cout << "i_order = " << i_order << ", PsiReCenter_BbcNorth = " << PsiReCenter_BbcNorth << ", PsiShift_BbcNorth = " << PsiShift_BbcNorth << endl;

      mRecoEPHistoManager->fillHist_BbcShiftEP(PsiShift_BbcSouth,PsiShift_BbcNorth,i_order,cent20);
      mRecoEPProManager->fillPro_BbcResolution(PsiShift_BbcSouth,PsiShift_BbcNorth,i_order,mRunIndex,cent20);
    }

    // mRecoEventPlane->printRawBbcEventPlane(1);

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
      mRecoEPHistoManager->writeQA_BbcChargePhiWeight();
    }
  }
  if(mMode == 1)
  {
    mRecoEPHistoManager->writeHist_BbcReCenterEP();
    mRecoEPHistoManager->writeHist_BbcReCenterQVector();
    mRecoEPProManager->writePro_BbcShift();
  }
  if(mMode == 2)
  {
    mRecoEPHistoManager->writeHist_BbcShiftEP();
    mRecoEPProManager->writePro_BbcResolution();
  }

  File_mOutPut->Close();

  if(mMode == 1) mRecoEventPlane->closePro_ReCenter();
  if(mMode == 2) 
  {
    mRecoEventPlane->closePro_ReCenter();
    mRecoEventPlane->closePro_Shift();
  }

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

  mTrigLvl1 = findNode::getClass<TrigLvl1>(topNode, "TrigLvl1");
  if( !mTrigLvl1 )
  {
    std::cout << PHWHERE << "can't find TrigLvl1 Node" << std::endl;
    return DISCARDEVENT;;//exit(1);
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
