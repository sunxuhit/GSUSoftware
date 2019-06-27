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
    void fillHist_DiMuonSpec(float invmass, int cent, float pt, float rapidity);
    void writeHist_DiMuonSpec();
    //===============di-muon spectra==============

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
    TH1F *h_mDiMuonSpec_South[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mDiMuonSpec_North[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mDiMuonInteSpec_South[3];
    TH1F *h_mDiMuonInteSpec_North[3];

    ClassDef(RecoEPHistoManager,1)
};
#endif
