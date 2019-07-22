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
  h_mInvMass_DiMuon = new TH2F("h_mInvMass_DiMuon","h_mInvMass_DiMuon",100,-5.0,5.0,100,0.0,5.0);
  h_mPt_DiMuon = new TH2F("h_mPt_DiMuon","h_mPt_DiMuon",100,-5.0,5.0,100,0.0,10.0);
  h_mPhi_DiMuon = new TH2F("h_mPhi_DiMuon","h_mPhi_DiMuon",100,-5.0,5.0,100,-TMath::Pi(),TMath::Pi());
  h_mPz_DiMuon = new TH2F("h_mPz_DiMuon","h_mPz_DiMuon",100,-5.0,5.0,100,-20.0,0.0);
}

void RecoEPHistoManager::fillHist_DiMuonSpec(float invmass, float px, float py, float pz, float rapidity)
{
  float pt = TMath::Sqrt(px*px+py*py);
  float phi = TMath::ATan2(py,px);
  h_mInvMass_DiMuon->Fill(rapidity,invmass);
  h_mPt_DiMuon->Fill(rapidity,pt);
  h_mPhi_DiMuon->Fill(rapidity,phi);
  h_mPz_DiMuon->Fill(rapidity,pz);
}

void RecoEPHistoManager::writeHist_DiMuonSpec()
{
  std::cout << "write Di-Muon Spectra Histograms!" << std::endl;
  h_mInvMass_DiMuon->Write();
  h_mPt_DiMuon->Write();
  h_mPhi_DiMuon->Write();
  h_mPz_DiMuon->Write();
}
//===============Di-Muon Spectra=========================

//===============Di-Muon QA=========================
void RecoEPHistoManager::initHist_DiMuonQA()
{
  std::cout << "initialize Di-Muon QA Histograms!" << std::endl;
  h_mCharge_DiMuon = new TH2F("h_mCharge_DiMuon","h_mCharge_DiMuon",100,-5.0,5.0,7,-3.5,3.5);
  h_mDcaR_DiMuon = new TH2F("h_mDcaR_DiMuon","h_mDcaR_DiMuon",100,0.0,5.0,100,-5.0,5.0);
  h_mDcaZ_DiMuon = new TH2F("h_mDcaZ_DiMuon","h_mDcaZ_DiMuon",100,-5.0,5.0,100,-5.0,5.0);
  h_mEvtVtxChi2_DiMuon = new TH2F("h_mEvtVtxChi2_DiMuon","h_mEvtVtxChi2_DiMuon",100,-5.0,5.0,100,0.0,20.0);
}

void RecoEPHistoManager::fillHist_DiMuonQA(short charge, float dca_r, float dca_z, float vtx_chi2, float rapidity)
{
  h_mCharge_DiMuon->Fill(rapidity,charge);
  h_mDcaR_DiMuon->Fill(rapidity,dca_r);
  h_mDcaZ_DiMuon->Fill(rapidity,dca_z);
  h_mEvtVtxChi2_DiMuon->Fill(rapidity,vtx_chi2);
}

void RecoEPHistoManager::writeHist_DiMuonQA()
{
  std::cout << "write Di-Muon QA Histograms!" << std::endl;
  h_mCharge_DiMuon->Write();
  h_mDcaR_DiMuon->Write();
  h_mDcaZ_DiMuon->Write();
  h_mEvtVtxChi2_DiMuon->Write();
}
//===============Di-Muon QA=========================

//===============Single Muon Spectra=========================
void RecoEPHistoManager::initHist_MuonTrkSpec()
{
  std::cout << "initialize Single Muon Spectra Histograms!" << std::endl;
  h_mPt_Tr0 = new TH2F("h_mPt_Tr0","h_mPt_Tr0",100,-5.0,5.0,100,0.0,10.0); 
  h_mPhi_Tr0 = new TH2F("h_mPhi_Tr0","h_mPhi_Tr0",100,-5.0,5.0,100,-TMath::Pi(),TMath::Pi());
  h_mPz_Tr0 = new TH2F("h_mPz_Tr0","h_mPz_Tr0",100,-5.0,5.0,200,-20.0,20.0);

  h_mPt_Tr1 = new TH2F("h_mPt_Tr1","h_mPt_Tr1",100,-5.0,5.0,100,0.0,10.0); 
  h_mPhi_Tr1 = new TH2F("h_mPhi_Tr1","h_mPhi_Tr1",100,-5.0,5.0,100,-TMath::Pi(),TMath::Pi());
  h_mPz_Tr1 = new TH2F("h_mPz_Tr1","h_mPz_Tr1",100,-5.0,5.0,200,-20.0,20.0);
}

void RecoEPHistoManager::fillHist_MuonTr0Spec(float px, float py, float pz, float rapidity)
{
  float pt = TMath::Sqrt(px*px+py*py);
  float phi = TMath::ATan2(py,px);
  h_mPt_Tr0->Fill(rapidity,pt);
  h_mPhi_Tr0->Fill(rapidity,phi);
  h_mPz_Tr0->Fill(rapidity,pz);
}

void RecoEPHistoManager::fillHist_MuonTr1Spec(float px, float py, float pz, float rapidity)
{
  float pt = TMath::Sqrt(px*px+py*py);
  float phi = TMath::ATan2(py,px);
  h_mPt_Tr1->Fill(rapidity,pt);
  h_mPhi_Tr1->Fill(rapidity,phi);
  h_mPz_Tr1->Fill(rapidity,pz);
}

void RecoEPHistoManager::writeHist_MuonTrkSpec()
{
  std::cout << "write Single Muon Spectra Histograms!" << std::endl;
  h_mPt_Tr0->Write();
  h_mPhi_Tr0->Write();
  h_mPz_Tr0->Write();

  h_mPt_Tr1->Write();
  h_mPhi_Tr1->Write();
  h_mPz_Tr1->Write();
}

//===============Single Muon Spectra=========================

//------------------------------------------------------------
