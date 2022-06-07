#ifndef BeamFinder_h
#define BeamFinder_h

#include <vector>

#include "TObject.h"
#include <TH2D.h>

#include "../PixelMap/mRICH.h"

class BeamFinder : public TObject
{
  public:
    BeamFinder();
    ~BeamFinder();

    int clearClusterFinder_5by5(); 
    int clearClusterFinder_3by3(); 
    int findCluster_5by5(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel);
    int findCluster_3by3(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel);

    int initBeamFinder();
    int fillBeamFinder_5by5();
    int fillBeamFinder_3by3();
    int fillBeamOn(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel);
    int fillBeamOff(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel);
    int writeBeamFinder();

  private:
    bool mVerbosity;
    bool mVerbosity_QA;
    // 5*5 cluster finder
    std::vector<int> mXClusterMap_5by5; // corresponding binX number for each cluster
    std::vector<int> mYClusterMap_5by5; // corresponding binY number for each cluster
    std::vector<int> mRankMap_5by5; // number of surrounding photons 

    std::vector<int> mXCluster_5by5_1st;
    std::vector<int> mYCluster_5by5_1st;
    std::vector<int> mRank_5by5_1st;
    int mMeanX_5by5_1st;
    int mMeanY_5by5_1st;

    std::vector<int> mXCluster_5by5_2nd;
    std::vector<int> mYCluster_5by5_2nd;
    std::vector<int> mRank_5by5_2nd;
    int mMeanX_5by5_2nd;
    int mMeanY_5by5_2nd;

    std::vector<int> mXCluster_5by5_3rd;
    std::vector<int> mYCluster_5by5_3rd;
    std::vector<int> mRank_5by5_3rd;
    int mMeanX_5by5_3rd;
    int mMeanY_5by5_3rd;

    TH2D *h_mBeamFinder_5by5_Display;
    TH2D *h_mBeamFinder_5by5_1st;
    TH2D *h_mBeamFinder_5by5_2nd;
    TH2D *h_mBeamFinder_5by5_3rd;

    // 3*3 cluster finder
    std::vector<int> mXClusterMap_3by3; // corresponding binX number for each cluster
    std::vector<int> mYClusterMap_3by3; // corresponding binY number for each cluster
    std::vector<int> mRankMap_3by3; // number of surrounding photons 

    std::vector<int> mXCluster_3by3_1st;
    std::vector<int> mYCluster_3by3_1st;
    std::vector<int> mRank_3by3_1st;
    int mMeanX_3by3_1st;
    int mMeanY_3by3_1st;

    std::vector<int> mXCluster_3by3_2nd;
    std::vector<int> mYCluster_3by3_2nd;
    std::vector<int> mRank_3by3_2nd;
    int mMeanX_3by3_2nd;
    int mMeanY_3by3_2nd;

    std::vector<int> mXCluster_3by3_3rd;
    std::vector<int> mYCluster_3by3_3rd;
    std::vector<int> mRank_3by3_3rd;
    int mMeanX_3by3_3rd;
    int mMeanY_3by3_3rd;

    TH2D *h_mBeamFinder_3by3_Display;
    TH2D *h_mBeamFinder_3by3_1st;
    TH2D *h_mBeamFinder_3by3_2nd;
    TH2D *h_mBeamFinder_3by3_3rd;

    TH2D *h_mRingImage_on; // Ring Image with Beam hit
    TH2D *h_mRingImage_off; // Ring Image without Beam hit

    ClassDef(BeamFinder,1)
};

#endif
