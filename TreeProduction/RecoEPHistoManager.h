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
    void fillQA_Global_nocuts(float vtx_zdc, float vtx_bbc, float centrality);
    void fillQA_Global(float vtx_zdc, float vtx_bbc, float centrality);
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
    //-----------------BBC QA---------------------

    //===============Raw BBC Event Plane=========================
    void initHist_BbcRawEP();
    void fillHist_BbcRawEP(float Psi_BbcSouth, float Psi_BbcNorth, int order, int cent);
    void writeHist_BbcRawEP();

    void initHist_BbcRawQVector();
    void fillHist_BbcRawQVector(TVector2 QVec_BbcSouth, float QWeight_BbcSouth, TVector2 QVec_BbcNorth, float QWeight_BbcNorth, int order, int cent);
    void writeHist_BbcRawQVector();
    //===============Raw BBC Event Plane=========================

  private:

    // vertex QA
    TH1F *h_mVtZ_Bbc_nocuts;
    TH2F *h_mVtR_Bbc_nocuts;
    TH1F *h_mVtZ_Zdc_nocuts;
    TH1F *h_mCentrality_nocuts;

    TH1F *h_mVtZ_Bbc;
    TH2F *h_mVtR_Bbc;
    TH1F *h_mVtZ_Zdc;
    TH1F *h_mCentrality;

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



    // BBC Raw EP
    TH1F *h_mEPRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mEPRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    // TH1F *h_mEPRaw_BbcSN[3][vecMesonFlow::mNumOfCentrality20];
    TH2F *h_mEPRaw_Correlation[3][vecMesonFlow::mNumOfCentrality20];

    TH1F *h_mQxRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyRaw_BbcSouth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQxRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];
    TH1F *h_mQyRaw_BbcNorth[3][vecMesonFlow::mNumOfCentrality20];

    ClassDef(RecoEPHistoManager,1)
};
#endif
