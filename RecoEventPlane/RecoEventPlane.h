#ifndef __RecoEventPlane_H__
#define __RecoEventPlane_H__

#include "SubsysReco.h"

// class Fun4AllServer;
// class CentralityReco;
class recoConsts;

class PHCompositeNode;
class PHGlobal;
// class TrigLvl1;
class RunHeader;
class BbcRaw;
class BbcCalib;
class BbcGeo;
class VtxOut;

class TTree;
class TFile;
class TH1F;
class TH2F;
class TProfile2D;

class RecoEventPlane: public SubsysReco {
  public:
    RecoEventPlane(const char *outputfile = "test.root");

    int Init(PHCompositeNode *topNode);
    int InitRun(PHCompositeNode *topNode);
    int process_event(PHCompositeNode *topNode);
    int ResetEvent(PHCompositeNode *topNode) {return 0;};
    int Reset(PHCompositeNode *topNode) {return 0;};
    int End(PHCompositeNode *topNode);

    void set_ana_mode(int mode)
    {
      mMode = mode;
    }

  private:
    recoConsts *mRC;
    // RC flags
    int mRunSelection;
    int mSystemSelection;
    int mEPCalib;
    double mBbczCut_val;
    int mDebug;
    int mBbcPmt_flag;

    PHGlobal* mPHGlobal;
    int mCentrality;

    RunHeader* mRunHeader;
    int mRunId;

    VtxOut* mVtxOut;

    BbcRaw* mBbcRaw;
    BbcCalib* mBbcCalib;
    BbcGeo* mBbcGeo;

    int mNumOfEvents;

    int mMode; // 0 for re-center | 1 for shift
    TFile* File_mInPut;

    //! Eval Tree Output
    TFile* File_mOutPut;
    std::string mOutPutFile;

    int getNodes(PHCompositeNode *topNode); // extract nodes needed for analysis

    // Histogram Manager
    int initHistoRunQA();
    int writeHistoRunQA();

    // vertex QA
    TH1F *h_mVtZ_Bbc;
    TH1F *h_mVtZ_Zdc;
    TH1F *h_mCentrality;

    /*
    // BBC QA
    TH1F *h_mAdc_Bbc[128]; // raw adc distribution
    TH1F *h_mCharge_Bbc[128]; // (adc-ped)*gain_adc/gain_pmt

    TH2F *h_mGeoXY_BbcSouth; // BBC XY charge map for south
    TH1F *h_mGeoZ_BbcSouth; // BBC Z distribution map for south

    TH2F *h_mGeoXY_BbcNorth; // BBC 2-D charge map for north 
    TH1F *h_mGeoZ_BbcNorth; // BBC Z distribution map for south

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
};

#endif //__H_RecoEventPlane_H__
