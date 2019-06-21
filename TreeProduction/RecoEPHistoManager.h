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

    void set_debug(int debug) {mDebug = debug;}

    //-----------------BBC QA---------------------
    void initQA_BbcAdc();
    void fillQA_BbcAdc(int i_pmt, float adc);
    void writeQA_BbcAdc();

    void initQA_BbcCharge();
    void fillQA_BbcCharge(int i_pmt, float bbcx, float bbcy, float bbcz, float charge);
    void writeQA_BbcCharge();

    void initQA_BbcChargeReCalib();
    void fillQA_BbcChargeReCalib(int i_pmt, float bbcx, float bbcy, float bbcz, float charge);
    void writeQA_BbcChargeReCalib();

    void initQA_BbcChargePhiWeight();
    void fillQA_BbcChargePhiWeight(int i_pmt, float bbcx, float bbcy, float bbcz, float weight);
    void writeQA_BbcChargePhiWeight();
    //-----------------BBC QA---------------------

    //===============Raw BBC Event Plane==================
    void initHist_BbcRawEP();
    void fillHist_BbcRawEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_BbcRawEP();

    void initHist_BbcRawQVector();
    void fillHist_BbcRawQVector(TVector2 QVec_BbcSouth, float QWeight_BbcSouth, TVector2 QVec_BbcNorth, float QWeight_BbcNorth, int order, int cent);
    void writeHist_BbcRawQVector();
    //===============Raw BBC Event Plane==================

    //===============ReCenter BBC Event Plane================
    void initHist_BbcReCenterEP();
    void fillHist_BbcReCenterEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_BbcReCenterEP();

    void initHist_BbcReCenterQVector();
    void fillHist_BbcReCenterQVector(TVector2 QVec_BbcSouth, TVector2 QVec_BbcNorth, int order, int cent);
    void writeHist_BbcReCenterQVector();
    //===============ReCenter BBC Event Plane================

    //===============Shift BBC Event Plane==============
    void initHist_BbcShiftEP();
    void fillHist_BbcShiftEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_BbcShiftEP();
    //===============Shift BBC Event Plane==============

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

    // BBC QA
    int mDebug;
    TH1F *h_mAdc_Bbc[128]; // raw adc distribution

    TH1F *h_mCharge_Bbc[128]; // (adc-ped)*gain_adc/gain_pmt
    TH2F *h_mChargeXY_BbcSouth; // BBC XY charge map for south
    TH1F *h_mGeoZ_BbcSouth; // BBC Z distribution map for south
    TH2F *h_mChargeXY_BbcNorth; 
    TH1F *h_mGeoZ_BbcNorth; 

    TH1F *h_mChargeReCalib_Bbc[128]; // recalibrated charge
    TH2F *h_mChargeReCalibXY_BbcSouth; // BBC XY recalib charge map for south
    TH1F *h_mGeoReCalibZ_BbcSouth; // BBC Z distribution map for south
    TH2F *h_mChargeReCalibXY_BbcNorth;
    TH1F *h_mGeoReCalibZ_BbcNorth; 

    TH2F *h_mChargePhiWeightXY_BbcSouth; // BBC XY recalib charge map for south
    TH1F *h_mGeoPhiWeightZ_BbcSouth; // BBC Z distribution map for south
    TH2F *h_mChargePhiWeightXY_BbcNorth;
    TH1F *h_mGeoPhiWeightZ_BbcNorth; 

    // BBC Raw EP
    TH1F *h_mEPRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEPRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEPRaw_Correlation[3][vecMesonFlow::mNumOfCentrality20];

    TH1F *h_mQxRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQxRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];

    // BBC ReCenter EP
    TH1F *h_mEPReCenter_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEPReCenter_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEPReCenter_Correlation[3][vecMesonFlow::mNumOfCentrality20];

    TH1F *h_mQxReCenter_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyReCenter_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQxReCenter_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyReCenter_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];

    // BBC Shift EP
    TH1F *h_mEPShift_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEPShift_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEPShift_Correlation[3][vecMesonFlow::mNumOfCentrality20];

    // DiMuon Spectra
    TH1F *h_mDiMuonSpec_South[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mDiMuonSpec_North[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mDiMuonInteSpec_South[3];
    TH1F *h_mDiMuonInteSpec_North[3];

    ClassDef(RecoEPHistoManager,1)
};
#endif
