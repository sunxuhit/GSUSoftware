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

    int initQA_Global();
    int fillQA_Global(double vtx_zdc, double vtx_bbc, double centrality);
    int writeQA_Global();
    // int initQA_BbcAdc();
    // int initQA_BbcCharge();
    // int initQA_BbcReCalibCharge();

  private:

    // vertex QA
    TH1F *h_mVtZ_Bbc;
    TH1F *h_mVtZ_Zdc;
    TH1F *h_mCentrality;

    // BBC QA
    // TH1F *h_mAdc_Bbc[128]; // raw adc distribution
    // TH1F *h_mCharge_Bbc[128]; // (adc-ped)*gain_adc/gain_pmt

    // TH2F *h_mGeoXY_BbcSouth; // BBC XY charge map for south
    // TH1F *h_mGeoZ_BbcSouth; // BBC Z distribution map for south

    // TH2F *h_mGeoXY_BbcNorth; // BBC 2-D charge map for north 
    // TH1F *h_mGeoZ_BbcNorth; // BBC Z distribution map for south

    /*
    // BBC Raw EP
    TH1F *h_mQx1st_BbcSouth;
    TH1F *h_mQy1st_BbcSouth;
    TH1F *h_mQx1st_BbcNorth;
    TH1F *h_mQy1st_BbcNorth;

    TH1F *h_mQx2nd_BbcSouth;
    TH1F *h_mQy2nd_BbcSouth;
    TH1F *h_mQx2nd_BbcNorth;
    TH1F *h_mQy2nd_BbcNorth;

    TH1F *h_mEP1st_BbcSouth;
    TH1F *h_mEP1st_BbcNorth;
    TH2F *h_mEP1st_Correlation;

    TH1F *h_mEP2nd_BbcSouth;
    TH1F *h_mEP2nd_BbcNorth;
    TH2F *h_mEP2nd_Correlation;

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
