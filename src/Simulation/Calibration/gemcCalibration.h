#ifndef gemcCalibration_h
#define gemcCalibration_h

#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "TObject.h"
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

    int Init();
    int initChain();
    int initHistograms();
    int initGausSmearing();

    int Make();

    int Finish();
    int writeHistograms();
    int writeGausSmearing();

    bool isPhoton(int pid, int i_track);
    bool isReflection(double pz_out, int i_track);
    bool isOnAerogel(int detector_id, int i);
    bool isOnPhotonSensor(int detector_id, int i);

    double GausSmearing(TF1 *f_gaus);
    bool isInSensorPlane(double out_x, double out_y);

    // ring finder
    int initRingFinder();
    int writeRingFinder();
    int clearRingFinder(); 
    int HoughTransform(TH1D *h_NumOfPhotons, TH2D *h_PhotonDist, std::vector<int> xPixel, std::vector<int> yPixel);
    bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov);
    bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform);


  private:
    std::string mDet, mHome;
    bool is_pmt;

    Material *mMat;
    std::string mOutPutFile;
    TFile *File_mOutPut;

    TH1D *h_mNumOfEvents;
    TH2D *h_mPhotonDist;
    TH2D *h_mPhotonGenerated;
    TH1D *h_mWaveLength;
    TH1D *h_mNumOfPhotonsDist;
    TProfile *p_mNumOfPhotons;

    TF1 *f_mGaus;
    TH2D *h_mXGausSmearing;
    TH2D *h_mYGausSmearing;

    // ring finder
    std::vector<int> mXPixelMap; // corresponding binX number for each photon hit
    std::vector<int> mYPixelMap; // corresponding binY number for each photon hit

    TH1D *h_mNumOfPhotons; // number of total photons in each event 
    TH2D *h_mRingFinder; // x: photon out_x | y: photon out_y with detector effect
    TH3D *h_mHoughTransform; // x | y | R
    TH3D *h_mQA_HT; // QA for hough transform

    TH3D *h_mCherenkovRing; // x | y | R
    TH2D *h_mNumOfCherenkovPhotons; // 

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

    ClassDef(gemcCalibration,1)
};
#endif
