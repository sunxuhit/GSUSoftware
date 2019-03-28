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
  h_mVtZ_Bbc_nocuts = new TH1F("h_mVtZ_Bbc_nocuts","h_mVtZ_Bbc_nocuts",201,-100.5,100.5);
  h_mVtZ_Zdc_nocuts = new TH1F("h_mVtZ_Zdc_nocuts","h_mVtZ_Zdc_nocuts",201,-100.5,100.5);
  h_mCentrality_nocuts = new TH1F("h_mCentrality_nocuts","h_mCentrality_nocuts",101,-0.5,100.5);

  h_mVtZ_Bbc = new TH1F("h_mVtZ_Bbc","h_mVtZ_Bbc",201,-100.5,100.5);
  h_mVtZ_Zdc = new TH1F("h_mVtZ_Zdc","h_mVtZ_Zdc",201,-100.5,100.5);
  h_mCentrality = new TH1F("h_mCentrality","h_mCentrality",101,-0.5,100.5);
}

void RecoEPHistoManager::fillQA_Global_nocuts(float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtZ_Bbc_nocuts->Fill(vtx_bbc);
  h_mVtZ_Zdc_nocuts->Fill(vtx_zdc);
  h_mCentrality_nocuts->Fill(centrality);
}

void RecoEPHistoManager::fillQA_Global(float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtZ_Bbc->Fill(vtx_bbc);
  h_mVtZ_Zdc->Fill(vtx_zdc);
  h_mCentrality->Fill(centrality);
}

void RecoEPHistoManager::writeQA_Global()
{
  h_mVtZ_Bbc_nocuts->Write();
  h_mVtZ_Zdc_nocuts->Write();
  h_mCentrality_nocuts->Write();

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
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentralities; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEPRaw_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360,-TMath::Pi(),TMath::Pi());

      HistName = Form("h_mEPRaw_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360,-TMath::Pi(),TMath::Pi());

      // HistName = Form("h_mEPRaw_BbcSN_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      // h_mEPRaw_BbcSN[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360,-TMath::Pi(),TMath::Pi());

      HistName = Form("h_mEPRaw_Correlation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_Correlation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),100,-TMath::Pi(),TMath::Pi(),100,-TMath::Pi(),TMath::Pi());
    }
  }
}

void RecoEPHistoManager::fillHist_BbcRawEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent)
{
  h_mEPRaw_BbcSouth[order][cent]->Fill(Psi_BbcSouth);
  h_mEPRaw_BbcNorth[order][cent]->Fill(Psi_BbcNorth);
  // h_mEPRaw_BbcSN[order][cent]->Fill(Psi_BbcSouth-Psi_BbcNorth);
  h_mEPRaw_Correlation[order][cent]->Fill(Psi_BbcSouth,Psi_BbcNorth);
}

void RecoEPHistoManager::writeHist_BbcRawEP()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentralities; ++i_cent)
    {
      h_mEPRaw_BbcSouth[i_order][i_cent]->Write();
      h_mEPRaw_BbcNorth[i_order][i_cent]->Write();
      // h_mEPRaw_BbcSN[i_order][i_cent]->Write();
      h_mEPRaw_Correlation[i_order][i_cent]->Write();
    }
  }
}

//------------------------------------------------------------
