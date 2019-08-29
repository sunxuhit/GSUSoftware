#ifndef RingFinder_h
#define RingFinder_h

#include <utility>
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

    //-----------------------------
    // Hough Transform
    int initRingFinder_HT();
    int writeRingFinder_HT();
    int clearRingFinder_HT(); 

    int HoughTransform(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel);
    bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov);
    bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);

    TVector2 getRingCenter_HT();
    float getRingRadius_HT();
    int getNumOfPhotonsOnRing_HT();
    int getNumOfPhotonsOffRing_HT();
    //-----------------------------

    //-----------------------------
    // Minuit Fit
    int initRingFinder_MF();
    int writeRingFinder_MF();
    int clearRingFinder_MF(); 
    int clearRingFinder_mRR(); 

    int MinuitFit(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel);
    std::pair<int,int> minuitRingRadius(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel);

    TVector2 getRingCenter_MF();
    float getRingRadius_MF();
    int getNumOfPhotonsOnRing_MF();
    int getNumOfPhotonsOffRing_MF();
    //-----------------------------

    float findPixelCoord(int pixel); // return correspoding pixel coordinate
    bool isOnRing(TVector2 photonHit, double x0, double y0, double r0);

  private:
    //-----------------------------
    // Hough Transform
    TVector2 mRingCenter_HT;
    float mRadius_HT;
    int mNumOfPhotonsOnRing_HT;
    int mNumOfPhotonsOffRing_HT;

    TH3D *h_mHoughTransform; // x | y | R for Hough Transform => reset after each event

    TH3D *h_mCherenkovRing_HT; // x | y | R for all rings
    TH3D *h_mCherenkovPhotons_HT; // number of photons on ring | number of photons off ring | ring radius
    TH3D *h_mNumOfCherenkovPhotons_HT; // num of photons on ring | num of photons off ring | total num of photons
    //-----------------------------

    //-----------------------------
    // Minuit Fit 
    TVector2 mRingCenter_MF;
    float mRadius_MF;
    int mNumOfPhotonsOnRing_MF;
    int mNumOfPhotonsOffRing_MF;

    TH3D *h_mCherenkovRing_MF; // x | y | R for all rings
    TH3D *h_mCherenkovPhotons_MF; // number of photons on ring | number of photons off ring | ring radius
    TH3D *h_mNumOfCherenkovPhotons_MF; // num of photons on ring | num of photons off ring | total num of photons

    std::vector<int> mXPixel_MF;
    std::vector<int> mYPixel_MF;
    TH2D *h_mRingFinder_MF; // x: photon out_x | y: photon out_y with detector effect

    //-----------------------------

    ClassDef(RingFinder,1)
};

#endif
