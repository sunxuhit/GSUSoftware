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
  h_mVtxZ_Bbc_MiniBias = new TH2F("h_mVtxZ_Bbc_MiniBias","h_mVtxZ_Bbc_MiniBias",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mVtxZ_Zdc_MiniBias = new TH2F("h_mVtxZ_Zdc_MiniBias","h_mVtxZ_Zdc_MiniBias",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mCentrality_MiniBias = new TH2F("h_mCentrality_MiniBias","h_mCentrality_MiniBias",1500,-0.5,1499.5,101,-0.5,100.5);

  h_mVtxZ_Bbc_NarrowVtx = new TH2F("h_mVtxZ_Bbc_NarrowVtx","h_mVtxZ_Bbc_NarrowVtx",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mVtxZ_Zdc_NarrowVtx = new TH2F("h_mVtxZ_Zdc_NarrowVtx","h_mVtxZ_Zdc_NarrowVtx",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mCentrality_NarrowVtx = new TH2F("h_mCentrality_NarrowVtx","h_mCentrality_NarrowVtx",1500,-0.5,1499.5,101,-0.5,100.5);

  h_mVtxZ_Bbc_NarrowVtxBbc = new TH2F("h_mVtxZ_Bbc_NarrowVtxBbc","h_mVtxZ_Bbc_NarrowVtxBbc",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mVtxZ_Zdc_NarrowVtxBbc = new TH2F("h_mVtxZ_Zdc_NarrowVtxBbc","h_mVtxZ_Zdc_NarrowVtxBbc",1500,-0.5,1499.5,201,-100.5,100.5);
  h_mCentrality_NarrowVtxBbc = new TH2F("h_mCentrality_NarrowVtxBbc","h_mCentrality_NarrowVtxBbc",1500,-0.5,1499.5,101,-0.5,100.5);
}

void RecoEPHistoManager::fillQA_Global_MiniBias(int runIndex, float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtxZ_Bbc_MiniBias->Fill(runIndex,vtx_bbc);
  h_mVtxZ_Zdc_MiniBias->Fill(runIndex,vtx_zdc);
  h_mCentrality_MiniBias->Fill(runIndex,centrality);
}

void RecoEPHistoManager::fillQA_Global_NarrowVtx(int runIndex, float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtxZ_Bbc_NarrowVtx->Fill(runIndex,vtx_bbc);
  h_mVtxZ_Zdc_NarrowVtx->Fill(runIndex,vtx_zdc);
  h_mCentrality_NarrowVtx->Fill(runIndex,centrality);
}

void RecoEPHistoManager::fillQA_Global_NarrowVtxBbc(int runIndex, float vtx_zdc, float vtx_bbc, float centrality)
{
  h_mVtxZ_Bbc_NarrowVtxBbc->Fill(runIndex,vtx_bbc);
  h_mVtxZ_Zdc_NarrowVtxBbc->Fill(runIndex,vtx_zdc);
  h_mCentrality_NarrowVtxBbc->Fill(runIndex,centrality);
}

void RecoEPHistoManager::writeQA_Global()
{
  h_mVtxZ_Bbc_MiniBias->Write();
  h_mVtxZ_Zdc_MiniBias->Write();
  h_mCentrality_MiniBias->Write();

  h_mVtxZ_Bbc_NarrowVtx->Write();
  h_mVtxZ_Zdc_NarrowVtx->Write();
  h_mCentrality_NarrowVtx->Write();

  h_mVtxZ_Bbc_NarrowVtxBbc->Write();
  h_mVtxZ_Zdc_NarrowVtxBbc->Write();
  h_mCentrality_NarrowVtxBbc->Write();
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

void RecoEPHistoManager::initQA_BbcChargePhiWeight()
{
  std::cout << "initialize BBC phi-weight Charge QA!" << std::endl;
  h_mChargePhiWeightXY_BbcSouth = new TH2F("h_mChargePhiWeightXY_BbcSouth","h_mChargePhiWeightXY_BbcSouth",40,-200,200,40,-200,200);
  h_mGeoPhiWeightZ_BbcSouth = new TH1F("h_mGeoPhiWeightZ_BbcSouth","h_mGeoPhiWeightZ_BbcSouth",500,-1500.5,-1000.5);
  h_mChargePhiWeightXY_BbcNorth = new TH2F("h_mChargePhiWeightXY_BbcNorth","h_mChargePhiWeightXY_BbcNorth",40,-200,200,40,-200,200); 
  h_mGeoPhiWeightZ_BbcNorth = new TH1F("h_mGeoPhiWeightZ_BbcNorth","h_mGeoPhiWeightZ_BbcNorth",500,1000.5,1500.5);
}

void RecoEPHistoManager::fillQA_BbcChargePhiWeight(int i_pmt, float bbcx, float bbcy, float bbcz, float weight)
{
  if(bbcz < 0) // south
  {
    h_mChargePhiWeightXY_BbcSouth->Fill(bbcx,bbcy,weight);
    h_mGeoPhiWeightZ_BbcSouth->Fill(bbcz);
  }
  if(bbcz > 0) // north
  {
    h_mChargePhiWeightXY_BbcNorth->Fill(bbcx,bbcy,weight);
    h_mGeoPhiWeightZ_BbcNorth->Fill(bbcz);
  }
}

void RecoEPHistoManager::writeQA_BbcChargePhiWeight()
{
  h_mChargePhiWeightXY_BbcSouth->Write();
  h_mGeoPhiWeightZ_BbcSouth->Write();
  h_mChargePhiWeightXY_BbcNorth->Write();
  h_mGeoPhiWeightZ_BbcNorth->Write();
}

//------------------------------------------------------------

//===============Raw BBC Event Plane=========================
void RecoEPHistoManager::initHist_BbcRawEP()
{
  std::cout << "initialize BBC Raw EP Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  const float harmonic[3] = {1.0,2.0,3.0};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEPRaw_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPRaw_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPRaw_Correlation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPRaw_Correlation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order],90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);
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
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mEPRaw_BbcSouth[i_order][i_cent]->Write();
      h_mEPRaw_BbcNorth[i_order][i_cent]->Write();
      // h_mEPRaw_BbcSN[i_order][i_cent]->Write();
      h_mEPRaw_Correlation[i_order][i_cent]->Write();
    }
  }
}

void RecoEPHistoManager::initHist_BbcRawQVector()
{
  std::cout << "initialize BBC Raw QVector Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mQxRaw_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQxRaw_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
      HistName = Form("h_mQyRaw_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQyRaw_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);

      HistName = Form("h_mQxRaw_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQxRaw_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
      HistName = Form("h_mQyRaw_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQyRaw_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
    }
  }
}

void RecoEPHistoManager::fillHist_BbcRawQVector(TVector2 QVec_BbcSouth, float QWeight_BbcSouth, TVector2 QVec_BbcNorth, float QWeight_BbcNorth, int order, int cent)
{
  float Qx_South = QVec_BbcSouth.X()/QWeight_BbcSouth;
  float Qy_South = QVec_BbcSouth.Y()/QWeight_BbcSouth;
  h_mQxRaw_BbcSouth[order][cent]->Fill(Qx_South);
  h_mQyRaw_BbcSouth[order][cent]->Fill(Qy_South);

  float Qx_North = QVec_BbcNorth.X()/QWeight_BbcNorth;
  float Qy_North = QVec_BbcNorth.Y()/QWeight_BbcNorth;
  h_mQxRaw_BbcNorth[order][cent]->Fill(Qx_North);
  h_mQyRaw_BbcNorth[order][cent]->Fill(Qy_North);
}

void RecoEPHistoManager::writeHist_BbcRawQVector()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mQxRaw_BbcSouth[i_order][i_cent]->Write();
      h_mQyRaw_BbcSouth[i_order][i_cent]->Write();

      h_mQxRaw_BbcNorth[i_order][i_cent]->Write();
      h_mQyRaw_BbcNorth[i_order][i_cent]->Write();
    }
  }
}
//===============Raw BBC Event Plane=========================

//===============ReCenter BBC Event Plane=========================
void RecoEPHistoManager::initHist_BbcReCenterEP()
{
  std::cout << "initialize BBC ReCenter EP Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  const float harmonic[3] = {1.0,2.0,3.0};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEPReCenter_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPReCenter_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPReCenter_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPReCenter_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPReCenter_Correlation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPReCenter_Correlation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order],90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);
    }
  }
}

void RecoEPHistoManager::fillHist_BbcReCenterEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent)
{
  h_mEPReCenter_BbcSouth[order][cent]->Fill(Psi_BbcSouth);
  h_mEPReCenter_BbcNorth[order][cent]->Fill(Psi_BbcNorth);
  h_mEPReCenter_Correlation[order][cent]->Fill(Psi_BbcSouth,Psi_BbcNorth);
}

void RecoEPHistoManager::writeHist_BbcReCenterEP()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mEPReCenter_BbcSouth[i_order][i_cent]->Write();
      h_mEPReCenter_BbcNorth[i_order][i_cent]->Write();
      h_mEPReCenter_Correlation[i_order][i_cent]->Write();
    }
  }
}

void RecoEPHistoManager::initHist_BbcReCenterQVector()
{
  std::cout << "initialize BBC ReCenter QVector Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mQxReCenter_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQxReCenter_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
      HistName = Form("h_mQyReCenter_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQyReCenter_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);

      HistName = Form("h_mQxReCenter_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQxReCenter_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
      HistName = Form("h_mQyReCenter_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mQyReCenter_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),201,-1.005,1.005);
    }
  }
}

void RecoEPHistoManager::fillHist_BbcReCenterQVector(TVector2 QVec_BbcSouth, TVector2 QVec_BbcNorth, int order, int cent)
{
  float Qx_South = QVec_BbcSouth.X();
  float Qy_South = QVec_BbcSouth.Y();
  h_mQxReCenter_BbcSouth[order][cent]->Fill(Qx_South);
  h_mQyReCenter_BbcSouth[order][cent]->Fill(Qy_South);

  float Qx_North = QVec_BbcNorth.X();
  float Qy_North = QVec_BbcNorth.Y();
  h_mQxReCenter_BbcNorth[order][cent]->Fill(Qx_North);
  h_mQyReCenter_BbcNorth[order][cent]->Fill(Qy_North);
}

void RecoEPHistoManager::writeHist_BbcReCenterQVector()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mQxReCenter_BbcSouth[i_order][i_cent]->Write();
      h_mQyReCenter_BbcSouth[i_order][i_cent]->Write();

      h_mQxReCenter_BbcNorth[i_order][i_cent]->Write();
      h_mQyReCenter_BbcNorth[i_order][i_cent]->Write();
    }
  }
}
//===============ReCenter BBC Event Plane=========================

//===============Shift BBC Event Plane=========================
void RecoEPHistoManager::initHist_BbcShiftEP()
{
  std::cout << "initialize BBC Shift EP Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  const float harmonic[3] = {1.0,2.0,3.0};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEPShift_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPShift_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPShift_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPShift_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEPShift_Correlation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEPShift_Correlation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order],90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);
    }
  }
}

void RecoEPHistoManager::fillHist_BbcShiftEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent)
{
  h_mEPShift_BbcSouth[order][cent]->Fill(Psi_BbcSouth);
  h_mEPShift_BbcNorth[order][cent]->Fill(Psi_BbcNorth);
  h_mEPShift_Correlation[order][cent]->Fill(Psi_BbcSouth,Psi_BbcNorth);
}

void RecoEPHistoManager::writeHist_BbcShiftEP()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mEPShift_BbcSouth[i_order][i_cent]->Write();
      h_mEPShift_BbcNorth[i_order][i_cent]->Write();
      h_mEPShift_Correlation[i_order][i_cent]->Write();
    }
  }
}
//===============Shift BBC Event Plane=========================

//===============Shift BBC Event Plane=========================
void RecoEPHistoManager::initHist_DiMuonSpec()
{
  std::cout << "initialize Di-Muon Spectra Histograms!" << std::endl;
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
  {
    std::string HistName;
    HistName = Form("h_mDiMuonSpec_South_Centrality_%d",i_cent);
    h_mDiMuonSpec_South[i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);

    HistName = Form("h_mDiMuonSpec_North_Centrality_%d",i_cent);
    h_mDiMuonSpec_North[i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);
  }
}

void RecoEPHistoManager::fillHist_DiMuonSpec(float invmass, int cent)
{
  h_mDiMuonSpec_South[cent]->Fill(invmass);
  h_mDiMuonSpec_North[cent]->Fill(invmass);
}

void RecoEPHistoManager::writeHist_DiMuonSpec()
{
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
  {
    h_mDiMuonSpec_South[i_cent]->Write();
    h_mDiMuonSpec_North[i_cent]->Write();
  }
}
//===============Shift BBC Event Plane=========================

//------------------------------------------------------------
