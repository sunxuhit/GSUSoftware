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

//===============BBC Event Plane=========================
void RecoEPHistoManager::initHist_BbcEP()
{
  std::cout << "initialize BBC EP Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  const float harmonic[3] = {1.0,2.0,3.0};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEP_BbcSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_BbcSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEP_BbcNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_BbcNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEP_BbcCorrelation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_BbcCorrelation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order],90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);
    }
  }
}

void RecoEPHistoManager::fillHist_BbcEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent)
{
  h_mEP_BbcSouth[order][cent]->Fill(Psi_BbcSouth);
  h_mEP_BbcNorth[order][cent]->Fill(Psi_BbcNorth);
  h_mEP_BbcCorrelation[order][cent]->Fill(Psi_BbcSouth,Psi_BbcNorth);
}

void RecoEPHistoManager::writeHist_BbcEP()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mEP_BbcSouth[i_order][i_cent]->Write();
      h_mEP_BbcNorth[i_order][i_cent]->Write();
      h_mEP_BbcCorrelation[i_order][i_cent]->Write();
    }
  }
}
//===============BBC Event Plane=========================

//===============FVTX Event Plane=========================
void RecoEPHistoManager::initHist_FvtxEP()
{
  std::cout << "initialize FVTX EP Histograms!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  const float harmonic[3] = {1.0,2.0,3.0};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      std::string HistName;
      HistName = Form("h_mEP_FvtxSouth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_FvtxSouth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEP_FvtxNorth_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_FvtxNorth[i_order][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),360/harmonic[i_order],-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);

      HistName = Form("h_mEP_FvtxCorrelation_%s_Centrality_%d",Order[i_order].c_str(),i_cent);
      h_mEP_FvtxCorrelation[i_order][i_cent] = new TH2F(HistName.c_str(),HistName.c_str(),90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order],90,-TMath::Pi()/harmonic[i_order],TMath::Pi()/harmonic[i_order]);
    }
  }
}

void RecoEPHistoManager::fillHist_FvtxEP(float Psi_FvtxSouth, float Psi_FvtxNorth, int order, int cent)
{
  h_mEP_FvtxSouth[order][cent]->Fill(Psi_FvtxSouth);
  h_mEP_FvtxNorth[order][cent]->Fill(Psi_FvtxNorth);
  h_mEP_FvtxCorrelation[order][cent]->Fill(Psi_FvtxSouth,Psi_FvtxNorth);
}

void RecoEPHistoManager::writeHist_FvtxEP()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mEP_FvtxSouth[i_order][i_cent]->Write();
      h_mEP_FvtxNorth[i_order][i_cent]->Write();
      h_mEP_FvtxCorrelation[i_order][i_cent]->Write();
    }
  }
}
//===============FVTX Event Plane=========================

//===============Di-Muon Spectra=========================
void RecoEPHistoManager::initHist_DiMuonSpec()
{
  std::cout << "initialize Di-Muon Spectra Histograms!" << std::endl;
  for(int i_pt = 0; i_pt < 3; ++i_pt)
  {
    std::string HistName;
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      HistName = Form("h_mDiMuonSpec_South_Centrality_%d_pt_%d",i_cent,i_pt);
      h_mDiMuonSpec_South[i_pt][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);

      HistName = Form("h_mDiMuonSpec_North_Centrality_%d_pt_%d",i_cent,i_pt);
      h_mDiMuonSpec_North[i_pt][i_cent] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);
    }
    HistName = Form("h_mDiMuonInteSpec_South_pt_%d",i_pt);
    h_mDiMuonInteSpec_South[i_pt] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);
    HistName = Form("h_mDiMuonInteSpec_North_pt_%d",i_pt);
    h_mDiMuonInteSpec_North[i_pt] = new TH1F(HistName.c_str(),HistName.c_str(),100,0.0,5.0);
  }
}

void RecoEPHistoManager::fillHist_DiMuonSpec(float invmass, int cent, float pt, float rapidity)
{
  int pt_bin = -1;
  if(pt > 0 && pt <= 2.0) pt_bin = 0;
  if(pt > 2.0 && pt <= 6.0) pt_bin = 1;
  if(pt > 6.0) pt_bin = 2;
  if(rapidity > 0 && pt > 0)
  { // North
    h_mDiMuonSpec_North[pt_bin][cent]->Fill(invmass);
    h_mDiMuonInteSpec_North[pt_bin]->Fill(invmass);
  }
  if(rapidity < 0 && pt > 0)
  { // South
    h_mDiMuonSpec_South[pt_bin][cent]->Fill(invmass);
    h_mDiMuonInteSpec_South[pt_bin]->Fill(invmass);
  }
}

void RecoEPHistoManager::writeHist_DiMuonSpec()
{
  for(int i_pt = 0; i_pt < 3; ++i_pt)
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
    {
      h_mDiMuonSpec_South[i_pt][i_cent]->Write();
      h_mDiMuonSpec_North[i_pt][i_cent]->Write();
    }
    h_mDiMuonInteSpec_South[i_pt]->Write();
    h_mDiMuonInteSpec_North[i_pt]->Write();
  }
}
//===============Di-Muon Spectra=========================

//------------------------------------------------------------
