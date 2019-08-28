#ifndef RingFinder_h
#define RingFinder_h

#include <vector>

#include "TObject.h"
#include "TVector2.h"
#include "TH2D.h"
#include "TH3D.h"

#include "../Material/mRICH.h"

class RingFinder : public TObject
{
  public:
    RingFinder();
    ~RingFinder();

    // ring finder
    int initRingFinder();
    int writeRingFinder();
    int clearRingFinder(); 

    int HoughTransform(int numOfPhotons, TH2D *h_PhotonDist, std::vector<int> xPixel, std::vector<int> yPixel);
    bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov);
    bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform);
    float findPixelCoord(int pixel); // return correspoding pixel coordinate

    TVector2 getRingCenter();
    float getRingRadius();
    int getNumOfPhotonsOnRing();
    int getNumOfPhotonsOffRing();

  private:
    // ring parameters
    TVector2 mRingCenter;
    float mRadius;
    int mNumOfPhotonsOnRing;
    int mNumOfPhotonsOffRing;

    TH3D *h_mHoughTransform; // x | y | R

    TH3D *h_mQA_HT; // QA for hough transform

    TH3D *h_mCherenkovRing; // x | y | R
    TH3D *h_mNumOfCherenkovPhotons; // number of photons | number of photons on ring | ring radius
    TH2D *h_mNumOfPhotons_OnOffRing;

    ClassDef(RingFinder,1)
};

#endif
