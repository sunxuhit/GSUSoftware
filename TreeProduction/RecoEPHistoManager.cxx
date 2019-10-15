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

  for(int i_order = 0; i_order < 3; ++i_order)
  {
    std::string HistName = Form("h_mCentrality20_Bbc_%s",Order[i_order].c_str());
    h_mCentrality20_Bbc[i_order] = new TH1F(HistName.c_str(),HistName.c_str(),24,-1.5,22.5);
    HistName = Form("h_mCentrality10_Bbc_%s",Order[i_order].c_str());
    h_mCentrality10_Bbc[i_order] = new TH1F(HistName.c_str(),HistName.c_str(),14,-1.5,12.5);
  }
}

void RecoEPHistoManager::fillHist_BbcEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent20, int cent10)
{
  h_mEP_BbcSouth[order][cent20]->Fill(Psi_BbcSouth);
  h_mEP_BbcNorth[order][cent20]->Fill(Psi_BbcNorth);
  h_mEP_BbcCorrelation[order][cent20]->Fill(Psi_BbcSouth,Psi_BbcNorth);
  h_mCentrality20_Bbc[order]->Fill(cent20);
  h_mCentrality10_Bbc[order]->Fill(cent10);
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
    h_mCentrality20_Bbc[i_order]->Write();
    h_mCentrality10_Bbc[i_order]->Write();
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

  for(int i_order = 0; i_order < 3; ++i_order)
  {
    std::string HistName = Form("h_mCentrality20_Fvtx_%s",Order[i_order].c_str());
    h_mCentrality20_Fvtx[i_order] = new TH1F(HistName.c_str(),HistName.c_str(),24,-1.5,22.5);
    HistName = Form("h_mCentrality10_Fvtx_%s",Order[i_order].c_str());
    h_mCentrality10_Fvtx[i_order] = new TH1F(HistName.c_str(),HistName.c_str(),14,-1.5,12.5);
  }
}

void RecoEPHistoManager::fillHist_FvtxEP(float Psi_FvtxSouth, float Psi_FvtxNorth, int order, int cent20, int cent10)
{
  h_mEP_FvtxSouth[order][cent20]->Fill(Psi_FvtxSouth);
  h_mEP_FvtxNorth[order][cent20]->Fill(Psi_FvtxNorth);
  h_mEP_FvtxCorrelation[order][cent20]->Fill(Psi_FvtxSouth,Psi_FvtxNorth);
  h_mCentrality20_Fvtx[order]->Fill(cent20);
  h_mCentrality10_Fvtx[order]->Fill(cent10);
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
    h_mCentrality20_Fvtx[i_order]->Write();
    h_mCentrality10_Fvtx[i_order]->Write();
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
  h_mDcaR_DiMuon = new TH2F("h_mDcaR_DiMuon","h_mDcaR_DiMuon",100,-5.0,5.0,100,0.0,5.0);
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

//===============Single Muon QA=========================
void RecoEPHistoManager::initHist_MuonTrkQA()
{
  std::cout << "initialize Single Muon QA Histograms!" << std::endl;
  h_mTrhits_Tr0 = new TH2F("h_mTrhits_Tr0","h_mTrhits_Tr0",100,-5.0,5.0,2000,-1999.5,0.5);
  h_mIdhits_Tr0 = new TH2F("h_mIdhits_Tr0","h_mIdhits_Tr0",100,-5.0,5.0,2000,-0.5,1999.5);
  h_mDG0_Tr0 = new TH2F("h_mDG0_Tr0","h_mDG0_Tr0",100,-5.0,5.0,100,-0.5,29.5);
  h_mDDG0_Tr0 = new TH2F("h_mDDG0_Tr0","h_mDDG0_Tr0",100,-5.0,5.0,100,-0.5,19.5);
  h_mTrchi2_Tr0 = new TH2F("h_mTrchi2_Tr0","h_mTrchi2_Tr0",100,-5.0,5.0,100,-1.0,20.0);
  h_mIdchi2_Tr0 = new TH2F("h_mIdchi2_Tr0","h_mIdchi2_Tr0",100,-5.0,5.0,100,-1.0,20.0);
  h_mNtrhits_Tr0 = new TH2F("h_mNtrhits_Tr0","h_mNtrhits_Tr0",100,-5.0,5.0,20,-0.5,19.5);
  h_mNidhits_Tr0 = new TH2F("h_mNidhits_Tr0","h_mNidhits_Tr0",100,-5.0,5.0,20,-0.5,19.5);
  h_mLastgap_Tr0 = new TH2F("h_mLastgap_Tr0","h_mLastgap_Tr0",100,-5.0,5.0,10,-0.5,9.5);
  h_mIdx_Tr0 = new TH2F("h_mIdx_Tr0","h_mIdx_Tr0",100,-5.0,5.0,100,-50.0,50.0);
  h_mIdy_Tr0 = new TH2F("h_mIdy_Tr0","h_mIdy_Tr0",100,-5.0,5.0,100,-50.0,50.0);
  h_mDcaR_Tr0 = new TH2F("h_mDcaR_Tr0","h_mDcaR_Tr0",100,-5.0,5.0,100,0.0,5.0);
  h_mDcaZ_Tr0 = new TH2F("h_mDcaZ_Tr0","h_mDcaZ_Tr0",100,-5.0,5.0,100,-5.0,5.0);

  h_mTrhits_Tr1 = new TH2F("h_mTrhits_Tr1","h_mTrhits_Tr1",100,-5.0,5.0,2000,-1999.5,0.5);
  h_mIdhits_Tr1 = new TH2F("h_mIdhits_Tr1","h_mIdhits_Tr1",100,-5.0,5.0,2000,-0.5,1999.5);
  h_mDG0_Tr1 = new TH2F("h_mDG0_Tr1","h_mDG0_Tr1",100,-5.0,5.0,100,-0.5,29.5);
  h_mDDG0_Tr1 = new TH2F("h_mDDG0_Tr1","h_mDDG0_Tr1",100,-5.0,5.0,100,-0.5,19.5);
  h_mTrchi2_Tr1 = new TH2F("h_mTrchi2_Tr1","h_mTrchi2_Tr1",100,-5.0,5.0,100,-1.0,20.0);
  h_mIdchi2_Tr1 = new TH2F("h_mIdchi2_Tr1","h_mIdchi2_Tr1",100,-5.0,5.0,100,-1.0,20.0);
  h_mNtrhits_Tr1 = new TH2F("h_mNtrhits_Tr1","h_mNtrhits_Tr1",100,-5.0,5.0,20,-0.5,19.5);
  h_mNidhits_Tr1 = new TH2F("h_mNidhits_Tr1","h_mNidhits_Tr1",100,-5.0,5.0,20,-0.5,19.5);
  h_mLastgap_Tr1 = new TH2F("h_mLastgap_Tr1","h_mLastgap_Tr1",100,-5.0,5.0,10,-0.5,9.5);
  h_mIdx_Tr1 = new TH2F("h_mIdx_Tr1","h_mIdx_Tr1",100,-5.0,5.0,100,-50.0,50.0);
  h_mIdy_Tr1 = new TH2F("h_mIdy_Tr1","h_mIdy_Tr1",100,-5.0,5.0,100,-50.0,50.0);
  h_mDcaR_Tr1 = new TH2F("h_mDcaR_Tr1","h_mDcaR_Tr1",100,-5.0,5.0,100,0.0,5.0);
  h_mDcaZ_Tr1 = new TH2F("h_mDcaZ_Tr1","h_mDcaZ_Tr1",100,-5.0,5.0,100,-5.0,5.0);
}

void RecoEPHistoManager::fillHist_MuonTr0_hits(short trhits, float trchi2, short ntrhits, float rapidity)
{
  h_mTrhits_Tr0->Fill(rapidity,trhits);
  h_mTrchi2_Tr0->Fill(rapidity,trchi2);
  h_mNtrhits_Tr0->Fill(rapidity,ntrhits);
}

void RecoEPHistoManager::fillHist_MuonTr0_id(short idhits, float idchi2, short nidhits, float idx, float idy, float rapidity)
{
  h_mIdhits_Tr0->Fill(rapidity,idhits);
  h_mIdchi2_Tr0->Fill(rapidity,idchi2);
  h_mNidhits_Tr0->Fill(rapidity,nidhits);
  h_mIdx_Tr0->Fill(rapidity,idx);
  h_mIdy_Tr0->Fill(rapidity,idy);
}

void RecoEPHistoManager::fillHist_MuonTr0_QA(float DG0, float DDG0, short lastgap, float dca_r, float dca_z, float rapidity)
{
  h_mDG0_Tr0->Fill(rapidity,DG0);
  h_mDDG0_Tr0->Fill(rapidity,DDG0);
  h_mLastgap_Tr0->Fill(rapidity,lastgap);
  h_mDcaR_Tr0->Fill(rapidity,dca_r);
  h_mDcaZ_Tr0->Fill(rapidity,dca_z);
}

void RecoEPHistoManager::fillHist_MuonTr1_hits(short trhits, float trchi2, short ntrhits, float rapidity)
{
  h_mTrhits_Tr1->Fill(rapidity,trhits);
  h_mTrchi2_Tr1->Fill(rapidity,trchi2);
  h_mNtrhits_Tr1->Fill(rapidity,ntrhits);
}

void RecoEPHistoManager::fillHist_MuonTr1_id(short idhits, float idchi2, short nidhits, float idx, float idy, float rapidity)
{
  h_mIdhits_Tr1->Fill(rapidity,idhits);
  h_mIdchi2_Tr1->Fill(rapidity,idchi2);
  h_mNidhits_Tr1->Fill(rapidity,nidhits);
  h_mIdx_Tr1->Fill(rapidity,idx);
  h_mIdy_Tr1->Fill(rapidity,idy);
}

void RecoEPHistoManager::fillHist_MuonTr1_QA(float DG0, float DDG0, short lastgap, float dca_r, float dca_z, float rapidity)
{
  h_mDG0_Tr1->Fill(rapidity,DG0);
  h_mDDG0_Tr1->Fill(rapidity,DDG0);
  h_mLastgap_Tr1->Fill(rapidity,lastgap);
  h_mDcaR_Tr1->Fill(rapidity,dca_r);
  h_mDcaZ_Tr1->Fill(rapidity,dca_z);
}

void RecoEPHistoManager::writeHist_MuonTrkQA()
{
  std::cout << "write Single Muon QA Histograms!" << std::endl;
  h_mTrhits_Tr0->Write();
  h_mIdhits_Tr0->Write();
  h_mDG0_Tr0->Write();
  h_mDDG0_Tr0->Write();
  h_mTrchi2_Tr0->Write();
  h_mIdchi2_Tr0->Write();
  h_mNtrhits_Tr0->Write();
  h_mNidhits_Tr0->Write();
  h_mLastgap_Tr0->Write();
  h_mIdx_Tr0->Write();
  h_mIdy_Tr0->Write();
  h_mDcaR_Tr0->Write();
  h_mDcaZ_Tr0->Write();

  h_mTrhits_Tr1->Write();
  h_mIdhits_Tr1->Write();
  h_mDG0_Tr1->Write();
  h_mDDG0_Tr1->Write();
  h_mTrchi2_Tr1->Write();
  h_mIdchi2_Tr1->Write();
  h_mNtrhits_Tr1->Write();
  h_mNidhits_Tr1->Write();
  h_mLastgap_Tr1->Write();
  h_mIdx_Tr1->Write();
  h_mIdy_Tr1->Write();
  h_mDcaR_Tr1->Write();
  h_mDcaZ_Tr1->Write();
}
//===============Single Muon QA=========================

//------------------------------------------------------------
