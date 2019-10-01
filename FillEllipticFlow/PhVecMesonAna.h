#ifndef PhVecMesonAna_h
#define PhVecMesonAna_h

#include "TObject.h"

// class Fun4AllServer;
// class CentralityReco;
// class recoConsts;

class TFile;
class TChain;
class TTree;
class TProfile;
class TH1F;
class TH2F;

class PhDiMuonHistoManager;
// class PhDiMuonProManager;
class PhDiMuonUtility;
// class PhDiMuonCut;
class PhDiMuonEvent;
class PhDiMuonTrack;

class PhVecMesonAna: public TObject
{
  public:
    PhVecMesonAna(int pid, int list, long start_event, long stop_event, int mode);
    virtual ~PhVecMesonAna();

    void setInPutDir(const std::string inputdir);
    void setOutPutFile(const std::string outputfile);
    void setInPutList(const std::string iInPutList);
    void setStopEvent(const long StopEvent);
    void setStartEvent(const long StartEvent);

    int Init();
    int Make();
    int Finish();

  private:
    std::string mInPutList;
    std::string mInPutDir;
    std::string mOutPutFile;
    long mStopEvent; // used event number
    long mStartEvent;

    long mStart_Event; // transfer event number
    long mStop_Event;

    int mPid; // 0 for J/Psi | 1 for phi
    int mMode; // 0 for UnLike-Sign | 1 for Like-Sign | 2 for Mixed-Event
    int mList;
    double mBbczCut_val;

    int mRunId;
    int mRunIndex;
    int mNumOfEvents;

    TFile* File_mOutPut;

    // utilities
    PhDiMuonHistoManager *mPhDiMuonHistoManager;
    // PhDiMuonProManager *mPhDiMuonProManager;
    PhDiMuonUtility *mPhDiMuonUtility;
    // PhDiMuonCut *mPhDiMuonCut;

    // di-muon TChain
    TChain *mChain_DiMuon;
    PhDiMuonEvent *mDiMuonEvent;
    PhDiMuonTrack *mDiMuonTrack;

    ClassDef(PhVecMesonAna,1)
};

#endif

