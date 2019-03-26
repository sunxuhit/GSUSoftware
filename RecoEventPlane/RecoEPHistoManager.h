#ifndef RecoEPHistoManager_h
#define RecoEPHistoManager_h

#include <TObject.h>

class TH1F;
class TH2F;

class RecoEPHistoManager : public TObject
{
  public:
    RecoEPHistoManager();
    virtual ~RecoEPHistoManager();

    void initQA_Global();
    void fillQA_Global(float vtx_zdc, float vtx_bbc, float centrality);
    void writeQA_Global();

    void set_debug(int debug) {mDebug = debug;}

    void initQA_BbcAdc();
    void fillQA_BbcAdc(int i_pmt, float adc);
    void writeQA_BbcAdc();

    void initQA_BbcCharge();
    void fillQA_BbcCharge(int i_pmt, float bbcx, float bbcy, float bbcz, float charge);
    void writeQA_BbcCharge();

    void initQA_BbcChargeReCalib();
    void fillQA_BbcChargeReCalib(int i_pmt, float bbcx, float bbcy, float bbcz, float charge);
    void writeQA_BbcChargeReCalib();

    void initHist_BbcRawEP();
    void fillHist_BbcRawEP(float Psi1st_BbcSouth, float Psi1st_BbcNorth, float Psi2nd_BbcSouth, float Psi2nd_BbcNorth, float Psi3rd_BbcSouth, float Psi3rd_BbcNorth);
    void writeHist_BbcRawEP();

  private:

    // vertex QA
    TH1F *h_mVtZ_Bbc;
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
    TH1F *h_mEP1st_BbcSouth;
    TH1F *h_mEP1st_BbcNorth;
    TH2F *h_mEP1st_Correlation;

    TH1F *h_mEP2nd_BbcSouth;
    TH1F *h_mEP2nd_BbcNorth;
    TH2F *h_mEP2nd_Correlation;

    TH1F *h_mEP3rd_BbcSouth;
    TH1F *h_mEP3rd_BbcNorth;
    TH2F *h_mEP3rd_Correlation;

    /*
    // BBC re-center parameters
    TProfile2D *p_mQx1st_BbcSouth[2]; // 0: vertex pos/neg | 1st: runID & 2nd centrality
    TProfile2D *p_mQy1st_BbcSouth[2];
    TProfile2D *p_mQx1st_BbcNorth[2];
    TProfile2D *p_mQy1st_BbcNorth[2];

    TProfile2D *p_mQx2nd_BbcSouth[2]; // 0: vertex pos/neg | 1st: runID & 2nd centrality
    TProfile2D *p_mQy2nd_BbcSouth[2];
    TProfile2D *p_mQx2nd_BbcNorth[2];
    TProfile2D *p_mQy2nd_BbcNorth[2];

    // BBC Re-Centered EP
    TH1F *h_mQx1st_BbcSouth_ReCenter;
    TH1F *h_mQy1st_BbcSouth_ReCenter;
    TH1F *h_mQx1st_BbcNorth_ReCenter;
    TH1F *h_mQy1st_BbcNorth_ReCenter;

    TH1F *h_mQx2nd_BbcSouth_ReCenter;
    TH1F *h_mQy2nd_BbcSouth_ReCenter;
    TH1F *h_mQx2nd_BbcNorth_ReCenter;
    TH1F *h_mQy2nd_BbcNorth_ReCenter;

    TH1F *h_mEP1st_BbcSouth_ReCenter;
    TH1F *h_mEP1st_BbcNorth_ReCenter;
    TH2F *h_mEP1st_Correlation_ReCenter;

    TH1F *h_mEP2nd_BbcSouth_ReCenter;
    TH1F *h_mEP2nd_BbcNorth_ReCenter;
    TH2F *h_mEP2nd_Correlation_ReCenter;
    */
    ClassDef(RecoEPHistoManager,1)
};
#endif
