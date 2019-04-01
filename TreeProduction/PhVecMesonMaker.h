#ifndef PhVecMesonMaker_h
#define PhVecMesonMaker_h

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

class RecoEPHistoManager;
class RecoEPProManager;
class RecoEPUtility;
class RecoEventPlane;

class PhVecMesonMaker: public SubsysReco {
  public:
    PhVecMesonMaker(const char *outputfile = "test.root");

    int Init(PHCompositeNode *topNode);
    int InitRun(PHCompositeNode *topNode);
    int process_event(PHCompositeNode *topNode);
    int ResetEvent(PHCompositeNode *topNode) {return 0;};
    int Reset(PHCompositeNode *topNode) {return 0;};
    int End(PHCompositeNode *topNode);

  private:
    recoConsts *mRC;
    // RC flags
    int mRunSelection;
    int mSystemSelection;
    double mBbczCut_val;
    int mBbcPmt_flag;
    int mMode;
    int mQA_Bbc;
    int mDebug_Bbc;

    PHGlobal* mPHGlobal;

    RunHeader* mRunHeader;
    int mRunId;

    VtxOut* mVtxOut;

    BbcRaw* mBbcRaw;
    BbcCalib* mBbcCalib;
    BbcGeo* mBbcGeo;

    int mNumOfEvents;

    TFile* File_mInPut;

    //! Eval Tree Output
    TFile* File_mOutPut;
    std::string mOutPutFile;

    // utilities
    int getNodes(PHCompositeNode *topNode); // extract nodes needed for analysis

    RecoEPHistoManager *mRecoEPHistoManager;
    RecoEPProManager *mRecoEPProManager;
    RecoEPUtility *mRecoEPUtility;
    RecoEventPlane *mRecoEventPlane;
};

#endif
