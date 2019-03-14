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
}

int RecoEventPlane::Init(PHCompositeNode *topNode) 
{
  cout << "RecoEventPlane::Init => " << endl;

  cout << "Setting Module Flags: " << endl;
  mRC              = recoConsts::instance();
  mRunSelection    = mRC->get_IntFlag("RD_RUN_SELECTION", 0);
  mSystemSelection = mRC->get_IntFlag("RD_SYSTEM_SELECTION", 0);
  mBbczCut_val     = mRC->get_DoubleFlag("RD_BBCZCUT_VAL", 10);
  mEPCalib         = mRC->get_IntFlag("EP_CALIB", 0);
  mDebug           = mRC->get_IntFlag("EP_DEBUG", 0);
  mBbcPmt_flag     = mRC->get_IntFlag("EP_BBC", 0);

  cout << "run " << mRunSelection << " @ system ";
  if(mRunSelection == 14 && mSystemSelection == 0) cout << "Au+Au" << endl;
  cout << "BBCZ cut value is " << mBbczCut_val << endl;

  cout << "BBC phi bin correction status is: "<<std::endl;
  if(mBbcPmt_flag == 0) cout << "do nothing" << endl;
  if(mBbcPmt_flag > 0)  cout << "hot/cold (20\% off average) phi bin removed" << endl;

  // init output structure
  File_mOutPut = TFile::Open(mOutPutFile.data(), "recreate");
  if(!File_mOutPut) cout<<"Cann't create " << mOutPutFile.data() <<endl;

  initHistoRunQA();

  mNumOfEvents = 0;

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
  h_mVtZ_Zdc->Fill(zdcz);
  bool isZDCOK = (zdc1>0 && zdc2>0 && zdcz > -9000);
  if (!isZDCOK) return DISCARDEVENT;

  float bbcz = mPHGlobal->getBbcZVertex();
  h_mVtZ_Bbc->Fill(bbcz);

  float cent = mPHGlobal->getCentrality();
  h_mCentrality->Fill(cent);

  // if ( bbcz >= bbczcut_val || bbcz <= -(bbczcut_val)) return DISCARDEVENT;

  // cout << "centrality = " << cent << endl;
  // cout << "BBC_z = " << bbcz << endl;
  // cout << "ZDC_z = " << zdcz << endl;
  // cout << endl;

  /*
  float Q1x_South = 0.0;
  float Q1y_South = 0.0;

  float Q1x_North = 0.0;
  float Q1y_North = 0.0;

  float Q2x_South = 0.0;
  float Q2y_South = 0.0;
  float Qw_South = 0.0;

  float Q2x_North = 0.0;
  float Q2y_North = 0.0;
  float Qw_North = 0.0;

  for (int i_pmt=0; i_pmt<128; i_pmt++) 
  {
    short adc = mBbcRaw->get_Adc(i_pmt);
    short tdc = mBbcRaw->get_Tdc0(i_pmt);
    float time0 = bbccalib->getHitTime0(i_pmt, tdc, adc);
    float charge = bbccalib->getCharge(i_pmt, adc);
    float bbcx= bbcgeo->getX(i_pmt);
    float bbcy= bbcgeo->getY(i_pmt);
    float bbcz= bbcgeo->getZ(i_pmt);

    if (time0 > 0 && charge > 0 && bbcz < 0) // south
    {
      h_mAdc_Bbc[i_pmt]->Fill(adc);
      h_mCharge_Bbc[i_pmt]->Fill(charge);
      h_mGeoXY_BbcSouth->Fill(bbcx,bbcy,charge);
      h_mGeoZ_BbcSouth->Fill(bbcz);

      float phi=atan2(bbcy,bbcx);
      float weight = charge;

      Q1x_South += weight*cos(1.0*phi);
      Q1y_South += weight*sin(1.0*phi);

      Q2x_South += weight*cos(2.0*phi);
      Q2y_South += weight*sin(2.0*phi);

      Qw_South += weight;
      // cout << "i_pmt = " << i_pmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
    }
    if (time0 > 0 && charge > 0 && bbcz > 0)  // north
    {
      h_mAdc_Bbc[i_pmt]->Fill(adc);
      h_mCharge_Bbc[i_pmt]->Fill(charge);
      h_mGeoXY_BbcNorth->Fill(bbcx,bbcy,charge);
      h_mGeoZ_BbcNorth->Fill(bbcz);

      float phi=atan2(bbcy,bbcx);
      float weight = charge;


      Q1x_North += weight*cos(1.0*phi);
      Q1y_North += weight*sin(1.0*phi);

      Q2x_North += weight*cos(2.0*phi);
      Q2y_North += weight*sin(2.0*phi);

      Qw_North += weight;
      // cout << "i_pmt = " << i_pmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
    }
  }

  h_mQx1st_BbcSouth->Fill(Q1x_South/Qw_South);
  h_mQy1st_BbcSouth->Fill(Q1y_South/Qw_South);
  h_mQx1st_BbcNorth->Fill(Q1x_North/Qw_South);
  h_mQy1st_BbcNorth->Fill(Q1y_North/Qw_South);

  h_mQx2nd_BbcSouth->Fill(Q2x_South/Qw_South);
  h_mQy2nd_BbcSouth->Fill(Q2y_South/Qw_South);
  h_mQx2nd_BbcNorth->Fill(Q2x_North/Qw_South);
  h_mQy2nd_BbcNorth->Fill(Q2y_North/Qw_South);


  float Psi_1st_South = atan2(Q1y_South/Qw_South,Q1x_South/Qw_South);
  h_mEP1st_BbcSouth->Fill(Psi_1st_South);

  float Psi_1st_North = atan2(Q1y_North/Qw_North,Q1x_North/Qw_North);
  h_mEP1st_BbcNorth->Fill(Psi_1st_North);
  h_mEP1st_Correlation->Fill(Psi_1st_South,Psi_1st_North);

  float Psi_2nd_South = atan2(Q2y_South/Qw_South,Q2x_South/Qw_South)/2.0;
  h_mEP2nd_BbcSouth->Fill(Psi_2nd_South);

  float Psi_2nd_North = atan2(Q2y_North/Qw_North,Q2x_North/Qw_North)/2.0;
  h_mEP2nd_BbcNorth->Fill(Psi_2nd_North);
  h_mEP2nd_Correlation->Fill(Psi_2nd_South,Psi_2nd_North);

  // fill Re-Center correction
  int vz_sign = 0;
  if(BBC_vtx_Z > 0.0) vz_sign = 0;
  if(BBC_vtx_Z <= 0.0) vz_sign = 1;

  if(mMode == 0) // fill Re-Center Correction factors
  {
    p_mQx1st_BbcSouth[vz_sign]->Fill(123.0,5.0,Q1x_South/Qw_South);
    p_mQy1st_BbcSouth[vz_sign]->Fill(123.0,5.0,Q1y_South/Qw_South);
    p_mQx1st_BbcNorth[vz_sign]->Fill(123.0,5.0,Q1x_North/Qw_South);
    p_mQy1st_BbcNorth[vz_sign]->Fill(123.0,5.0,Q1y_North/Qw_South);

    p_mQx2nd_BbcSouth[vz_sign]->Fill(123.0,5.0,Q2x_South/Qw_South);
    p_mQy2nd_BbcSouth[vz_sign]->Fill(123.0,5.0,Q2y_South/Qw_South);
    p_mQx2nd_BbcNorth[vz_sign]->Fill(123.0,5.0,Q2x_North/Qw_South);
    p_mQy2nd_BbcNorth[vz_sign]->Fill(123.0,5.0,Q2y_North/Qw_South);
  }
  if(mMode == 1) // apply Re-Center Correction and fill Shift Correction
  {
    int bin_Q1x_South = p_mQx1st_BbcSouth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q1x_South = p_mQx1st_BbcSouth[vz_sign]->GetBinContent(bin_Q1x_South);
    float Q1x_South_ReCenter = Q1x_South/Qw_South - mean_Q1x_South;
    h_mQx1st_BbcSouth_ReCenter->Fill(Q1x_South_ReCenter);

    int bin_Q1y_South = p_mQy1st_BbcSouth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q1y_South = p_mQy1st_BbcSouth[vz_sign]->GetBinContent(bin_Q1y_South);
    float Q1y_South_ReCenter = Q1y_South/Qw_South - mean_Q1y_South;
    h_mQy1st_BbcSouth_ReCenter->Fill(Q1y_South_ReCenter);

    float Psi_1st_South_ReCenter = atan2(Q1y_South_ReCenter,Q1x_South_ReCenter);
    h_mEP1st_BbcSouth_ReCenter->Fill(Psi_1st_South_ReCenter);

    int bin_Q1x_North = p_mQx1st_BbcNorth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q1x_North = p_mQx1st_BbcNorth[vz_sign]->GetBinContent(bin_Q1x_North);
    float Q1x_North_ReCenter = Q1x_North/Qw_North - mean_Q1x_North;
    h_mQx1st_BbcNorth_ReCenter->Fill(Q1x_North_ReCenter);

    int bin_Q1y_North = p_mQy1st_BbcNorth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q1y_North = p_mQy1st_BbcNorth[vz_sign]->GetBinContent(bin_Q1y_North);
    float Q1y_North_ReCenter = Q1y_North/Qw_North - mean_Q1y_North;
    h_mQy1st_BbcNorth_ReCenter->Fill(Q1y_North_ReCenter);

    float Psi_1st_North_ReCenter = atan2(Q1y_North_ReCenter,Q1x_North_ReCenter);
    h_mEP1st_BbcNorth_ReCenter->Fill(Psi_1st_North_ReCenter);
    h_mEP1st_Correlation_ReCenter->Fill(Psi_1st_South_ReCenter,Psi_1st_North_ReCenter);

    int bin_Q2x_South = p_mQx2nd_BbcSouth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q2x_South = p_mQx2nd_BbcSouth[vz_sign]->GetBinContent(bin_Q2x_South);
    float Q2x_South_ReCenter = Q2x_South/Qw_South - mean_Q2x_South;
    h_mQx2nd_BbcSouth_ReCenter->Fill(Q2x_South_ReCenter);

    int bin_Q2y_South = p_mQy2nd_BbcSouth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q2y_South = p_mQy2nd_BbcSouth[vz_sign]->GetBinContent(bin_Q2y_South);
    float Q2y_South_ReCenter = Q2y_South/Qw_South - mean_Q2y_South;
    h_mQy2nd_BbcSouth_ReCenter->Fill(Q2y_South_ReCenter);

    float Psi_2nd_South_ReCenter = atan2(Q2y_South_ReCenter,Q2x_South_ReCenter)/2.0;
    h_mEP2nd_BbcSouth_ReCenter->Fill(Psi_2nd_South_ReCenter);

    int bin_Q2x_North = p_mQx2nd_BbcNorth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q2x_North = p_mQx2nd_BbcNorth[vz_sign]->GetBinContent(bin_Q2x_North);
    float Q2x_North_ReCenter = Q2x_North/Qw_North - mean_Q2x_North;
    h_mQx2nd_BbcNorth_ReCenter->Fill(Q2x_North_ReCenter);

    int bin_Q2y_North = p_mQy2nd_BbcNorth[vz_sign]->FindBin(123.0,5.0);
    float mean_Q2y_North = p_mQy2nd_BbcNorth[vz_sign]->GetBinContent(bin_Q2y_North);
    float Q2y_North_ReCenter = Q2y_North/Qw_North - mean_Q2y_North;
    h_mQy2nd_BbcNorth_ReCenter->Fill(Q2y_North_ReCenter);

    float Psi_2nd_North_ReCenter = atan2(Q2y_North_ReCenter,Q2x_North_ReCenter)/2.0;
    h_mEP2nd_BbcNorth_ReCenter->Fill(Psi_2nd_North_ReCenter);
    h_mEP2nd_Correlation_ReCenter->Fill(Psi_2nd_South_ReCenter,Psi_2nd_North_ReCenter);
  }
  */

  return EVENT_OK;
}

int RecoEventPlane::End(PHCompositeNode *topNode) 
{
  File_mOutPut->cd();
  writeHistoRunQA();
  /*
  if(mMode == 0)
  {
    for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
    {
      h_mAdc_Bbc[i_pmt]->Write();
      h_mCharge_Bbc[i_pmt]->Write();
    }

    h_mGeoXY_BbcSouth->Write();
    h_mGeoZ_BbcSouth->Write();

    h_mGeoXY_BbcNorth->Write();
    h_mGeoZ_BbcNorth->Write();

    h_mQx1st_BbcSouth->Write();
    h_mQy1st_BbcSouth->Write();
    h_mQx1st_BbcNorth->Write();
    h_mQy1st_BbcNorth->Write();

    h_mQx2nd_BbcSouth->Write();
    h_mQy2nd_BbcSouth->Write();
    h_mQx2nd_BbcNorth->Write();
    h_mQy2nd_BbcNorth->Write();

    h_mEP1st_BbcSouth->Write();
    h_mEP1st_BbcNorth->Write();
    h_mEP1st_Correlation->Write();

    h_mEP2nd_BbcSouth->Write();
    h_mEP2nd_BbcNorth->Write();
    h_mEP2nd_Correlation->Write();

    for(int i_vertex = 0; i_vertex < 2; ++i_vertex)
    {
      p_mQx1st_BbcSouth[i_vertex]->Write();
      p_mQy1st_BbcSouth[i_vertex]->Write();
      p_mQx1st_BbcNorth[i_vertex]->Write();
      p_mQy1st_BbcNorth[i_vertex]->Write();

      p_mQx2nd_BbcSouth[i_vertex]->Write();
      p_mQy2nd_BbcSouth[i_vertex]->Write();
      p_mQx2nd_BbcNorth[i_vertex]->Write();
      p_mQy2nd_BbcNorth[i_vertex]->Write();
    }
  }

  if(mMode == 1)
  {
    h_mQx1st_BbcSouth_ReCenter->Write();
    h_mQy1st_BbcSouth_ReCenter->Write();
    h_mQx1st_BbcNorth_ReCenter->Write();
    h_mQy1st_BbcNorth_ReCenter->Write();

    h_mQx2nd_BbcSouth_ReCenter->Write();
    h_mQy2nd_BbcSouth_ReCenter->Write();
    h_mQx2nd_BbcNorth_ReCenter->Write();
    h_mQy2nd_BbcNorth_ReCenter->Write();

    h_mEP1st_BbcSouth_ReCenter->Write();
    h_mEP1st_BbcNorth_ReCenter->Write();
    h_mEP1st_Correlation_ReCenter->Write();

    h_mEP2nd_BbcSouth_ReCenter->Write();
    h_mEP2nd_BbcNorth_ReCenter->Write();
    h_mEP2nd_Correlation_ReCenter->Write();
  }
  */

  File_mOutPut->Close();

  return EVENT_OK;
}

int RecoEventPlane::getNodes(PHCompositeNode *topNode)
{
  mPHGlobal = findNode::getClass<PHGlobal>(topNode,"PHGlobal");
  if( !mPHGlobal )
  {
    std::cout<<"can't find PHGlobal "<<std::endl;
    return DISCARDEVENT;//exit(1);
  }

  mBbcRaw = findNode::getClass<BbcRaw>(topNode,"BbcRaw");
  if (!mBbcRaw)
  {
    std::cout << PHWHERE << "Could not find BbcRaw !" << std::endl;
    return EVENT_OK;
  }

  mVtxOut = findNode::getClass<VtxOut>(topNode,"VtxOut");
  if( !mVtxOut )
  {
    std::cout<<"can't find VtxOut "<<std::endl;
    return DISCARDEVENT;//exit(1);
  }

  return EVENT_OK;
}

int RecoEventPlane::initHistoRunQA()
{
  h_mVtZ_Bbc = new TH1F("h_mVtZ_Bbc","h_mVtZ_Bbc",201,-100.5,100.5);
  h_mVtZ_Zdc = new TH1F("h_mVtZ_Zdc","h_mVtZ_Zdc",201,-100.5,100.5);
  h_mCentrality = new TH1F("h_mCentrality","h_mCentrality",101,0.5,100.5);

  return 0;

  /*
  //-------------------------------------------------------------------------------
  // BBC QA
  for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    string HistName;

    HistName = Form("h_mAdc_Bbc_%d",i_pmt);
    h_mAdc_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),5000,0,10000);

    HistName = Form("h_mCharge_Bbc_%d",i_pmt);
    h_mCharge_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),100,0,50);
  }

  h_mGeoXY_BbcSouth = new TH2F("h_mGeoXY_BbcSouth","h_mGeoXY_BbcSouth",40,-200.0,200.0,40,-200.0,200.0);
  h_mGeoZ_BbcSouth  = new TH1F("h_mGeoZ_BbcSouth","h_mGeoZ_BbcSouth",1000,-2000,2000);

  h_mGeoXY_BbcNorth = new TH2F("h_mGeoXY_BbcNorth","h_mGeoXY_BbcNorth",40,-200.0,200.0,40,-200.0,200.0);
  h_mGeoZ_BbcNorth  = new TH1F("h_mGeoZ_BbcNorth","h_mGeoZ_BbcNorth",1000,-2000,2000);
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  h_mQx1st_BbcSouth = new TH1F("h_mQx1st_BbcSouth","h_mQx1st_BbcSouth",100,-2.0,2.0); // Q-Vector distribution
  h_mQy1st_BbcSouth = new TH1F("h_mQy1st_BbcSouth","h_mQy1st_BbcSouth",100,-2.0,2.0);
  h_mQx1st_BbcNorth = new TH1F("h_mQx1st_BbcNorth","h_mQx1st_BbcNorth",100,-2.0,2.0);
  h_mQy1st_BbcNorth = new TH1F("h_mQy1st_BbcNorth","h_mQy1st_BbcNorth",100,-2.0,2.0);

  h_mQx2nd_BbcSouth = new TH1F("h_mQx2nd_BbcSouth","h_mQx2nd_BbcSouth",100,-2.0,2.0);
  h_mQy2nd_BbcSouth = new TH1F("h_mQy2nd_BbcSouth","h_mQy2nd_BbcSouth",100,-2.0,2.0);
  h_mQx2nd_BbcNorth = new TH1F("h_mQx2nd_BbcNorth","h_mQx2nd_BbcNorth",100,-2.0,2.0);
  h_mQy2nd_BbcNorth = new TH1F("h_mQy2nd_BbcNorth","h_mQy2nd_BbcNorth",100,-2.0,2.0);

  h_mEP1st_BbcSouth = new TH1F("h_mEP1st_BbcSouth","h_mEP1st_BbcSouth",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_BbcNorth = new TH1F("h_mEP1st_BbcNorth","h_mEP1st_BbcNorth",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_Correlation = new TH2F("h_mEP1st_Correlation","h_mEP1st_Correlation",360,-TMath::Pi(),TMath::Pi(),360,-TMath::Pi(),TMath::Pi());

  h_mEP2nd_BbcSouth = new TH1F("h_mEP2nd_BbcSouth","h_mEP2nd_BbcSouth",360,-TMath::Pi(),TMath::Pi());
  h_mEP2nd_BbcNorth = new TH1F("h_mEP2nd_BbcNorth","h_mEP2nd_BbcNorth",360,-TMath::Pi(),TMath::Pi());
  h_mEP2nd_Correlation = new TH2F("h_mEP2nd_Correlation","h_mEP2nd_Correlation",360,-TMath::Pi(),TMath::Pi(),360,-TMath::Pi(),TMath::Pi());
  //-------------------------------------------------------------------------------

  // initialize Re-Center TProfile2D
  const string mVertex[2] = {"pos","neg"};
  if(mMode == 0)
  {
    for(int i_vertex = 0; i_vertex < 2; ++i_vertex)
    {
      string ProName;

      //-------------------------------------------------------------------------------
      // Re-Center Correction factor for 1st EP: x-axis is runId | y-axis is centrality
      ProName = Form("p_mQx1st_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQx1st_BbcSouth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQy1st_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQy1st_BbcSouth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQx1st_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQx1st_BbcNorth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQy1st_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQy1st_BbcNorth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);
      //-------------------------------------------------------------------------------

      //-------------------------------------------------------------------------------
      // Re-Center Correction factor for 2nd EP: x-axis is runId | y-axis is centrality
      ProName = Form("p_mQx2nd_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQx2nd_BbcSouth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQy2nd_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQy2nd_BbcSouth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQx2nd_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQx2nd_BbcNorth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);

      ProName = Form("p_mQy2nd_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQy2nd_BbcNorth[i_vertex] = new TProfile2D(ProName.c_str(),ProName.c_str(),1000,-0.5,999.5,9,-0.5,8.5);
      //-------------------------------------------------------------------------------
    }
  }

  // BBC Re-Centered EP
  if(mMode == 1)
  {
    File_mInput = TFile::Open("/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/macro/RawEP.root");

    for(int i_vertex = 0; i_vertex < 2; ++i_vertex)
    {
      string ProName;

      //-------------------------------------------------------------------------------
      // Re-Center Correction factor for 1st EP: x-axis is runId | y-axis is centrality
      ProName = Form("p_mQx1st_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQx1st_BbcSouth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy1st_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQy1st_BbcSouth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQx1st_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQx1st_BbcNorth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy1st_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQy1st_BbcNorth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();
      //-------------------------------------------------------------------------------

      //-------------------------------------------------------------------------------
      // Re-Center Correction factor for 2nd EP: x-axis is runId | y-axis is centrality
      ProName = Form("p_mQx2nd_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQx2nd_BbcSouth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy2nd_vertex_%s_BbcSouth",mVertex[i_vertex].c_str());
      p_mQy2nd_BbcSouth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQx2nd_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQx2nd_BbcNorth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy2nd_vertex_%s_BbcNorth",mVertex[i_vertex].c_str());
      p_mQy2nd_BbcNorth[i_vertex] = (TProfile2D*)File_mInput->Get(ProName.c_str())->Clone();
      //-------------------------------------------------------------------------------
    }
    h_mQx1st_BbcSouth_ReCenter = new TH1F("h_mQx1st_BbcSouth_ReCenter","h_mQx1st_BbcSouth_ReCenter",100,-2.0,2.0);
    h_mQy1st_BbcSouth_ReCenter = new TH1F("h_mQy1st_BbcSouth_ReCenter","h_mQy1st_BbcSouth_ReCenter",100,-2.0,2.0);
    h_mQx1st_BbcNorth_ReCenter = new TH1F("h_mQx1st_BbcNorth_ReCenter","h_mQx1st_BbcNorth_ReCenter",100,-2.0,2.0);
    h_mQy1st_BbcNorth_ReCenter = new TH1F("h_mQy1st_BbcNorth_ReCenter","h_mQy1st_BbcNorth_ReCenter",100,-2.0,2.0);

    h_mQx2nd_BbcSouth_ReCenter = new TH1F("h_mQx2nd_BbcSouth_ReCenter","h_mQx2nd_BbcSouth_ReCenter",100,-2.0,2.0);
    h_mQy2nd_BbcSouth_ReCenter = new TH1F("h_mQy2nd_BbcSouth_ReCenter","h_mQy2nd_BbcSouth_ReCenter",100,-2.0,2.0);
    h_mQx2nd_BbcNorth_ReCenter = new TH1F("h_mQx2nd_BbcNorth_ReCenter","h_mQx2nd_BbcNorth_ReCenter",100,-2.0,2.0);
    h_mQy2nd_BbcNorth_ReCenter = new TH1F("h_mQy2nd_BbcNorth_ReCenter","h_mQy2nd_BbcNorth_ReCenter",100,-2.0,2.0);

    h_mEP1st_BbcSouth_ReCenter = new TH1F("h_mEP1st_BbcSouth_ReCenter","h_mEP1st_BbcSouth_ReCenter",360,-TMath::Pi(),TMath::Pi());
    h_mEP1st_BbcNorth_ReCenter = new TH1F("h_mEP1st_BbcNorth_ReCenter","h_mEP1st_BbcNorth_ReCenter",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_Correlation_ReCenter = new TH2F("h_mEP1st_Correlation_ReCenter","h_mEP1st_Correlation_ReCenter",360,-TMath::Pi(),TMath::Pi(),360,-TMath::Pi(),TMath::Pi());

    h_mEP2nd_BbcSouth_ReCenter = new TH1F("h_mEP2nd_BbcSouth_ReCenter","h_mEP2nd_BbcSouth_ReCenter",360,-TMath::Pi(),TMath::Pi());
    h_mEP2nd_BbcNorth_ReCenter = new TH1F("h_mEP2nd_BbcNorth_ReCenter","h_mEP2nd_BbcNorth_ReCenter",360,-TMath::Pi(),TMath::Pi());
  h_mEP2nd_Correlation_ReCenter = new TH2F("h_mEP2nd_Correlation_ReCenter","h_mEP2nd_Correlation_ReCenter",360,-TMath::Pi(),TMath::Pi(),360,-TMath::Pi(),TMath::Pi());
  }
  */
}

int RecoEventPlane::writeHistoRunQA()
{
  h_mVtZ_Bbc->Write();
  h_mVtZ_Zdc->Write();
  h_mCentrality->Write();

  return 0;
}
