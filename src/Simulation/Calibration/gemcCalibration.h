#ifndef gemcCalibration_h
#define gemcCalibration_h

#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <TObject.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TF1.h>
#include <TVector2.h>

class Material;
class RingFinder;

class gemcCalibration : public TObject
{
  public:
    gemcCalibration();
    ~gemcCalibration();

    void is_PMT(bool b)
    {
      is_pmt = b;
    }

    void set_det(std::string detector)
    {
      mDet = detector;
    }

    void set_airgap(std::string airgap)
    {
      mAirGap = airgap;
    }

    int Init();
    int initChain();
    int initSimpleTree();
    int resetSimpleTree();

    int Make();

    int Finish();
    int writeSimpleTree();

    // beam spot cut
    bool isHodoXY(float vx, float vy);
    void set_HodoXY(float vx, float vy)
    {
      mVx_Hodo = vx;
      mVy_Hodo = vy;
    }

    bool isHodoR(float vx, float vy);
    void set_HodoR(float vr)
    {
      mVr_Hodo = vr;
    }

    bool isVetoXY(float vx, float vy);
    void set_VetoXY(float vx, float vy)
    {
      mVx_Veto = vx;
      mVy_Veto = vy;
    }

    bool isVetoR(float vx, float vy);
    void set_VetoR(float vr)
    {
      mVr_Veto = vr;
    }

    // pid cut
    bool isPhoton(int pid, int i_track);
    bool isReflection(double pz_out, int i_track);
    bool isOnAerogel(int detector_id, int i);
    bool isOnPhotonSensor(int detector_id, int i);
    bool isInSensorPlane(double out_x, double out_y);

    // ring finder
    int initRingImage();
    int clearRingImage(); 
    int writeRingImage();

  private:
    Material *mMat;
    RingFinder *mRingFinder; // ring finder

    std::string mDet, mAirGap, mHome;
    bool is_pmt;

    std::string mOutPutFile;
    TFile *File_mOutPut;
    float mVx_Hodo, mVy_Hodo, mVr_Hodo;
    float mVx_Veto, mVy_Veto, mVr_Veto;

    TH1D *h_mNumOfEvents;

    TH2D *h_mBeamSpot; // beam configuration
    TH2D *h_mBeamSpotX;
    TH2D *h_mBeamSpotY;
    TH1D *h_mPhotonSensorIn;

    TH2D *h_mRingImage; // ring image in pixel plane
    TH2D *h_mPhotonDist;
    TH2D *h_mPhotonDist_SingleEvent; // single event display
    TH2D *h_mPhotonGenerated;
    TH1D *h_mWaveLength;
    TH1D *h_mNumOfPhotons;
    TProfile *p_mNumOfPhotons;

    // ring finder
    std::vector<int> mXPixelMap; // corresponding binX number for each photon hit
    std::vector<int> mYPixelMap; // corresponding binY number for each photon hit
    TH2D *h_mRingFinder; // x: photon out_x | y: photon out_y with detector effect


    // chain for generated protons
    TChain *mChainInPut_Events;
    std::vector<int>    *gen_pid = NULL;
    std::vector<double> *gen_px = NULL;
    std::vector<double> *gen_py = NULL;
    std::vector<double> *gen_pz = NULL;
    std::vector<double> *gen_vx = NULL;
    std::vector<double> *gen_vy = NULL;
    std::vector<double> *gen_vz = NULL;

    // chain for simulated tracks
    TChain *mChainInPut_Tracks;
    std::vector<double> *trk_id      = NULL;
    std::vector<double> *trk_hitn    = NULL;
    std::vector<double> *trk_pid     = NULL;
    std::vector<double> *trk_mpid    = NULL;
    std::vector<double> *trk_tid     = NULL;
    std::vector<double> *trk_mtid    = NULL;
    std::vector<double> *trk_otid    = NULL;
    std::vector<double> *trk_trackE  = NULL;
    std::vector<double> *trk_totEdep = NULL;
    std::vector<double> *trk_avg_x   = NULL;
    std::vector<double> *trk_avg_y   = NULL;
    std::vector<double> *trk_avg_z   = NULL;
    std::vector<double> *trk_avg_lx  = NULL;
    std::vector<double> *trk_avg_ly  = NULL;
    std::vector<double> *trk_avg_lz  = NULL;
    std::vector<double> *trk_px      = NULL;
    std::vector<double> *trk_py      = NULL;
    std::vector<double> *trk_pz      = NULL;
    std::vector<double> *trk_vx      = NULL;
    std::vector<double> *trk_vy      = NULL;
    std::vector<double> *trk_vz      = NULL;
    std::vector<double> *trk_mvx     = NULL;
    std::vector<double> *trk_mvy     = NULL;
    std::vector<double> *trk_mvz     = NULL;
    std::vector<double> *trk_avg_t   = NULL;
    std::vector<double> *trk_in_x    = NULL;
    std::vector<double> *trk_in_y    = NULL;
    std::vector<double> *trk_in_z    = NULL;
    std::vector<double> *trk_in_px   = NULL;
    std::vector<double> *trk_in_py   = NULL;
    std::vector<double> *trk_in_pz   = NULL;
    std::vector<double> *trk_in_t    = NULL;
    std::vector<double> *trk_out_x   = NULL;
    std::vector<double> *trk_out_y   = NULL;
    std::vector<double> *trk_out_z   = NULL;
    std::vector<double> *trk_out_px  = NULL;
    std::vector<double> *trk_out_py  = NULL;
    std::vector<double> *trk_out_pz  = NULL;
    std::vector<double> *trk_out_t   = NULL;

    // simple TTree for Marco
    TTree *mSimpleTree;
    int mNumOfPhotons;
    int mPid[10000];
    float mWaveLength[10000];
    float mX_Sensor[10000];
    float mY_Sensor[10000];

    ClassDef(gemcCalibration,1)
};
#endif
