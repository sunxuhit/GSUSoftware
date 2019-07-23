#ifndef RecoEPHistoManager_h
#define RecoEPHistoManager_h

#include <TObject.h>
#include "PhVecMesonCons.h"
#include <TVector2.h>

class TH1F;
class TH2F;

class RecoEPHistoManager : public TObject
{
  public:
    RecoEPHistoManager();
    virtual ~RecoEPHistoManager();

    void initQA_Global();
    void fillQA_Global_MiniBias(int runIndex, float vtx_zdc, float vtx_bbc, float centrality);
    void fillQA_Global_NarrowVtx(int runIndex, float vtx_zdc, float vtx_bbc, float centrality);
    void fillQA_Global_NarrowVtxBbc(int runIndex, float vtx_zdc, float vtx_bbc, float centrality);
    void writeQA_Global();

    //===============BBC Event Plane==============
    void initHist_BbcEP();
    void fillHist_BbcEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_BbcEP();
    //===============BBC Event Plane==============

    //===============FVTX Event Plane==============
    void initHist_FvtxEP();
    void fillHist_FvtxEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_FvtxEP();
    //===============FVTX Event Plane==============

    //===============di-muon spectra==============
    void initHist_DiMuonSpec();
    void fillHist_DiMuonSpec(float invmass, float px, float py, float pz, float rapidity);
    void writeHist_DiMuonSpec();

    void initHist_DiMuonQA();
    void fillHist_DiMuonQA(short charge, float dca_r, float dca_z, float vtx_chi2, float rapidity);
    void writeHist_DiMuonQA();
    //===============di-muon spectra==============

    //===============single muon spectra==============
    void initHist_MuonTrkSpec();
    void fillHist_MuonTr0Spec(float px, float py, float pz, float rapidity);
    void fillHist_MuonTr1Spec(float px, float py, float pz, float rapidity);
    void writeHist_MuonTrkSpec();

    void initHist_MuonTrkQA();

    void fillHist_MuonTr0_hits(short trhits, float trchi2, short ntrhits, float rapidity);
    void fillHist_MuonTr0_id(short idhits, float idchi2, short nidhits, float idx, float idy, float rapidity);
    void fillHist_MuonTr0_QA(float DG0, float DDG0, short lastgap, float dca_r, float dca_z, float rapidity);
    void fillHist_MuonTr1_hits(short trhits, float trchi2, short ntrhits, float rapidity);
    void fillHist_MuonTr1_id(short idhits, float idchi2, short nidhits, float idx, float idy, float rapidity);
    void fillHist_MuonTr1_QA(float DG0, float DDG0, short lastgap, float dca_r, float dca_z, float rapidity);

    void writeHist_MuonTrkQA();
    //===============single muon spectra==============

  private:

    // vertex QA
    TH2F *h_mVtxZ_Bbc_MiniBias; // runIndex vs. VtxZ
    TH2F *h_mVtxZ_Zdc_MiniBias; // runIndex vs. VtxZ
    TH2F *h_mCentrality_MiniBias; // runIndex vs. centrality

    TH2F *h_mVtxZ_Bbc_NarrowVtx;
    TH2F *h_mVtxZ_Zdc_NarrowVtx;
    TH2F *h_mCentrality_NarrowVtx;

    TH2F *h_mVtxZ_Bbc_NarrowVtxBbc;
    TH2F *h_mVtxZ_Zdc_NarrowVtxBbc;
    TH2F *h_mCentrality_NarrowVtxBbc;

    // BBC EP QA
    TH1F *h_mEP_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEP_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEP_BbcCorrelation[3][vecMesonFlow::mNumOfCentrality20];

    // FVTX EP QA
    TH1F *h_mEP_FvtxSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEP_FvtxNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEP_FvtxCorrelation[3][vecMesonFlow::mNumOfCentrality20];

    // DiMuon Spectra
    TH2F *h_mInvMass_DiMuon; // rapidity vs. inv mass
    TH2F *h_mPt_DiMuon;
    TH2F *h_mPz_DiMuon;
    TH2F *h_mPhi_DiMuon;

    // DiMuon QA 
    TH2F *h_mCharge_DiMuon;
    TH2F *h_mDcaR_DiMuon;
    TH2F *h_mDcaZ_DiMuon;
    TH2F *h_mEvtVtxChi2_DiMuon;

    // Single Muon Spectra
    TH2F *h_mPt_Tr0; // rapidity vs. pT
    TH2F *h_mPhi_Tr0;
    TH2F *h_mPz_Tr0;
    TH2F *h_mTrhits_Tr0;
    TH2F *h_mIdhits_Tr0;
    TH2F *h_mDG0_Tr0;
    TH2F *h_mDDG0_Tr0;
    TH2F *h_mTrchi2_Tr0;
    TH2F *h_mIdchi2_Tr0;
    TH2F *h_mNtrhits_Tr0;
    TH2F *h_mNidhits_Tr0;
    TH2F *h_mLastgap_Tr0;
    TH2F *h_mIdx_Tr0;
    TH2F *h_mIdy_Tr0;
    TH2F *h_mDcaR_Tr0;
    TH2F *h_mDcaZ_Tr0;

    TH2F *h_mPt_Tr1;
    TH2F *h_mPhi_Tr1;
    TH2F *h_mPz_Tr1;
    TH2F *h_mTrhits_Tr1;
    TH2F *h_mIdhits_Tr1;
    TH2F *h_mDG0_Tr1;
    TH2F *h_mDDG0_Tr1;
    TH2F *h_mTrchi2_Tr1;
    TH2F *h_mIdchi2_Tr1;
    TH2F *h_mNtrhits_Tr1;
    TH2F *h_mNidhits_Tr1;
    TH2F *h_mLastgap_Tr1;
    TH2F *h_mIdx_Tr1;
    TH2F *h_mIdy_Tr1;
    TH2F *h_mDcaR_Tr1;
    TH2F *h_mDcaZ_Tr1;

    ClassDef(RecoEPHistoManager,1)
};
#endif
