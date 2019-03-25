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

class RecoEPHistoManager;
class RecoEPUtility;

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

    // utilities
    int getNodes(PHCompositeNode *topNode); // extract nodes needed for analysis

    RecoEPHistoManager *mRecoEPHistoManager;
    RecoEPUtility *mRecoEPUtility;
};

#endif //__H_RecoEventPlane_H__
