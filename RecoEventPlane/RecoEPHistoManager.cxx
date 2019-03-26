#include "RecoEPHistoManager.h"
#include <TH2F.h>
#include <TH1F.h>
#include <TMath.h>
#include <TString.h>

#include <iostream>
#include <string>

ClassImp(RecoEPHistoManager)

//------------------------------------------------------------

RecoEPHistoManager::RecoEPHistoManager()
{
  mDebug = 0;
}

RecoEPHistoManager::~RecoEPHistoManager()
{
}

//------------------------------------------------------------

void RecoEPHistoManager::initQA_Global()
{
  std::cout << "initialize Global QA!" << std::endl;
  h_mVtZ_Zdc = new TH1F("h_mVtZ_Zdc","h_mVtZ_Zdc",201,-100.5,100.5);
  h_mVtZ_Bbc = new TH1F("h_mVtZ_Bbc","h_mVtZ_Bbc",201,-100.5,100.5);
  h_mCentrality = new TH1F("h_mCentrality","h_mCentrality",101,-0.5,100.5);
}

void RecoEPHistoManager::fillQA_Global(float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtZ_Zdc->Fill(vtx_zdc);
  h_mVtZ_Bbc->Fill(vtx_bbc);
  h_mCentrality->Fill(centrality);
}

void RecoEPHistoManager::writeQA_Global()
{
  h_mVtZ_Bbc->Write();
  h_mVtZ_Zdc->Write();
  h_mCentrality->Write();
}

//------------------------------------------------------------

void RecoEPHistoManager::initQA_BbcAdc()
{
  std::cout << "initialize BBC ADC QA for each pmt!" << std::endl;
  for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    std::string HistName = Form("h_mAdc_Bbc_%d",i_pmt);
    h_mAdc_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),4096,-0.5,4095.5);
  }
}

void RecoEPHistoManager::fillQA_BbcAdc(int i_pmt, float adc)
{
  h_mAdc_Bbc[i_pmt]->Fill(adc);
}

void RecoEPHistoManager::writeQA_BbcAdc()
{
  for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    h_mAdc_Bbc[i_pmt]->Write();
  }
}

//------------------------------------------------------------

void RecoEPHistoManager::initQA_BbcCharge()
{
  std::cout << "initialize BBC Charge QA!" << std::endl;
  if(mDebug == 1)
  {
    std::cout << "initialize BBC Charge QA for each pmt!" << std::endl;
    for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
    {
      std::string HistName = Form("h_mCharge_Bbc_%d",i_pmt);
      h_mCharge_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),100,-0.5,99.5);
    }
  }

  h_mChargeXY_BbcSouth = new TH2F("h_mChargeXY_BbcSouth","h_mChargeXY_BbcSouth",40,-200,200,40,-200,200);
  h_mGeoZ_BbcSouth = new TH1F("h_mGeoZ_BbcSouth","h_mGeoZ_BbcSouth",500,-1500.5,-1000.5);

  h_mChargeXY_BbcNorth = new TH2F("h_mChargeXY_BbcNorth","h_mChargeXY_BbcNorth",40,-200,200,40,-200,200); 
  h_mGeoZ_BbcNorth = new TH1F("h_mGeoZ_BbcNorth","h_mGeoZ_BbcNorth",500,1000.5,1500.5);
}

void RecoEPHistoManager::fillQA_BbcCharge(int i_pmt, float bbcx, float bbcy, float bbcz, float charge)
{
  if(mDebug == 1)
  {
    h_mCharge_Bbc[i_pmt]->Fill(charge);
  }
  if(bbcz < 0) // south
  {
    h_mChargeXY_BbcSouth->Fill(bbcx,bbcy,charge);
    h_mGeoZ_BbcSouth->Fill(bbcz);
  }
  if(bbcz > 0) // north
  {
    h_mChargeXY_BbcNorth->Fill(bbcx,bbcy,charge);
    h_mGeoZ_BbcNorth->Fill(bbcz);
  }
}

void RecoEPHistoManager::writeQA_BbcCharge()
{
  if(mDebug == 1)
  {
    for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
    {
      h_mCharge_Bbc[i_pmt]->Write();
    }
  }

  h_mChargeXY_BbcSouth->Write();
  h_mGeoZ_BbcSouth->Write();

  h_mChargeXY_BbcNorth->Write();
  h_mGeoZ_BbcNorth->Write();
}

//------------------------------------------------------------

void RecoEPHistoManager::initQA_BbcChargeReCalib()
{
  std::cout << "initialize BBC Recalibrated Charge QA!" << std::endl;
  if(mDebug == 1)
  {
    std::cout << "initialize BBC Recalibrated Charge QA for each pmt!" << std::endl;
    for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
    {
      std::string HistName = Form("h_mChargeReCalib_Bbc_%d",i_pmt);
      h_mChargeReCalib_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),100,-0.5,99.5);
    }
  }
  h_mChargeReCalibXY_BbcSouth = new TH2F("h_mChargeReCalibXY_BbcSouth","h_mChargeReCalibXY_BbcSouth",40,-200,200,40,-200,200);
  h_mGeoReCalibZ_BbcSouth = new TH1F("h_mGeoReCalibZ_BbcSouth","h_mGeoReCalibZ_BbcSouth",500,-1500.5,-1000.5);
  h_mChargeReCalibXY_BbcNorth = new TH2F("h_mChargeReCalibXY_BbcNorth","h_mChargeReCalibXY_BbcNorth",40,-200,200,40,-200,200); 
  h_mGeoReCalibZ_BbcNorth = new TH1F("h_mGeoReCalibZ_BbcNorth","h_mGeoReCalibZ_BbcNorth",500,1000.5,1500.5);
}

void RecoEPHistoManager::fillQA_BbcChargeReCalib(int i_pmt, float bbcx, float bbcy, float bbcz, float charge)
{
  if(mDebug == 1)
  {
    h_mChargeReCalib_Bbc[i_pmt]->Fill(charge);
  }
  if(bbcz < 0) // south
  {
    h_mChargeReCalibXY_BbcSouth->Fill(bbcx,bbcy,charge);
    h_mGeoReCalibZ_BbcSouth->Fill(bbcz);
  }
  if(bbcz > 0) // north
  {
    h_mChargeReCalibXY_BbcNorth->Fill(bbcx,bbcy,charge);
    h_mGeoReCalibZ_BbcNorth->Fill(bbcz);
  }
}

void RecoEPHistoManager::writeQA_BbcChargeReCalib()
{
  if(mDebug == 1)
  {
    for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
    {
      h_mChargeReCalib_Bbc[i_pmt]->Write();
    }
  }
  h_mChargeReCalibXY_BbcSouth->Write();
  h_mGeoReCalibZ_BbcSouth->Write();
  h_mChargeReCalibXY_BbcNorth->Write();
  h_mGeoReCalibZ_BbcNorth->Write();
}

//------------------------------------------------------------

void RecoEPHistoManager::initHist_BbcRawEP()
{
  h_mEP1st_BbcSouth = new TH1F("h_mEP1st_BbcSouth","h_mEP1st_BbcSouth",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_BbcNorth = new TH1F("h_mEP1st_BbcNorth","h_mEP1st_BbcNorth",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_Correlation = new TH2F("h_mEP1st_Correlation","h_mEP1st_Correlation",100,-TMath::Pi(),TMath::Pi(),100,-TMath::Pi(),TMath::Pi());

  h_mEP2nd_BbcSouth = new TH1F("h_mEP2nd_BbcSouth","h_mEP2nd_BbcSouth",360,-TMath::Pi(),TMath::Pi());;
  h_mEP2nd_BbcNorth = new TH1F("h_mEP2nd_BbcNorth","h_mEP2nd_BbcNorth",360,-TMath::Pi(),TMath::Pi());;
  h_mEP2nd_Correlation = new TH2F("h_mEP2nd_Correlation","h_mEP2nd_Correlation",60,-TMath::Pi(),TMath::Pi(),60,-TMath::Pi(),TMath::Pi());

  h_mEP3rd_BbcSouth = new TH1F("h_mEP3rd_BbcSouth","h_mEP3rd_BbcSouth",360,-TMath::Pi(),TMath::Pi());;
  h_mEP3rd_BbcNorth = new TH1F("h_mEP3rd_BbcNorth","h_mEP3rd_BbcNorth",360,-TMath::Pi(),TMath::Pi());;
  h_mEP3rd_Correlation = new TH2F("h_mEP3rd_Correlation","h_mEP3rd_Correlation",360,-TMath::Pi(),TMath::Pi(),360,-TMath::Pi(),TMath::Pi());
}

void RecoEPHistoManager::fillHist_BbcRawEP(float Psi1st_BbcSouth, float Psi1st_BbcNorth, float Psi2nd_BbcSouth, float Psi2nd_BbcNorth, float Psi3rd_BbcSouth, float Psi3rd_BbcNorth)
{
  h_mEP1st_BbcSouth->Fill(Psi1st_BbcSouth);
  h_mEP1st_BbcNorth->Fill(Psi1st_BbcNorth);
  h_mEP1st_Correlation->Fill(Psi1st_BbcSouth,Psi1st_BbcNorth);

  h_mEP2nd_BbcSouth->Fill(Psi2nd_BbcSouth);
  h_mEP2nd_BbcNorth->Fill(Psi2nd_BbcNorth);
  h_mEP2nd_Correlation->Fill(Psi2nd_BbcSouth,Psi2nd_BbcNorth);

  h_mEP3rd_BbcSouth->Fill(Psi3rd_BbcSouth);
  h_mEP3rd_BbcNorth->Fill(Psi3rd_BbcNorth);
  h_mEP3rd_Correlation->Fill(Psi3rd_BbcSouth,Psi3rd_BbcNorth);
}

void RecoEPHistoManager::writeHist_BbcRawEP()
{
  h_mEP1st_BbcSouth->Write();
  h_mEP1st_BbcNorth->Write();
  h_mEP1st_Correlation->Write();

  h_mEP2nd_BbcSouth->Write();
  h_mEP2nd_BbcNorth->Write();
  h_mEP2nd_Correlation->Write();

  h_mEP3rd_BbcSouth->Write();
  h_mEP3rd_BbcNorth->Write();
  h_mEP3rd_Correlation->Write();
}

//------------------------------------------------------------
