#include <iostream>
#include <cstdlib>
#include <string> 
#include <TMath.h>
#include "./RingFinder.h"

using namespace std;

ClassImp(RingFinder)

RingFinder::RingFinder()
{
  cout << "RingFinder::RingFinder() ----- Constructor ! ------" << endl;
}

RingFinder::~RingFinder()
{
  cout << "RingFinder::~RingFinder() ----- Release memory ! ------" << endl;
}

//--------------------------------------------------------------------

int RingFinder::initRingFinder()
{
  cout << "initRingFinder: initialized histograms for Ring Finder!" << endl;
  h_mHoughTransform = new TH3D("h_mHoughTransform","h_mHoughTransform",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);

  h_mQA_HT = new TH3D("h_mQA_HT","h_mQA_HT",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);

  h_mCherenkovRing = new TH3D("h_mCherenkovRing","h_mCherenkovRing",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);
  h_mNumOfCherenkovPhotons = new TH3D("h_mNumOfCherenkovPhotons","h_mNumOfCherenkovPhotons",50,-0.5,49.5,50,-0.5,49.5,210,0,2.0*mRICH::mHalfWidth);
  h_mNumOfPhotons_OnOffRing = new TH2D("h_mNumOfPhotons_OnOffRing","h_mNumOfPhotons_OnOffRing",50,-0.5,49.5,50,-0.5,49.5);

  clearRingFinder();

  return 1;
}

int RingFinder::clearRingFinder()
{
  h_mHoughTransform->Reset();

  mRingCenter.Set(-999.9,-999.9);
  mRadius = -999.9;
  mNumOfPhotonsOnRing = -1;
  mNumOfPhotonsOffRing = -1;

  return 1;
}

int RingFinder::writeRingFinder()
{
  h_mQA_HT->Write();

  h_mCherenkovRing->Write();
  h_mNumOfCherenkovPhotons->Write();
  h_mNumOfPhotons_OnOffRing->Write();

  return 1;
}

bool RingFinder::findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov)
{
  // check if 3 hit points are at same position or collinear
  if(isSamePosition(firstHit,secondHit,thirdHit) || isCollinear(firstHit,secondHit,thirdHit) ) return false;

  double a = firstHit.X() - secondHit.X(); // a = x1 - x2
  double b = firstHit.Y() - secondHit.Y(); // b = y1 - y2
  double c = firstHit.X() - thirdHit.X();  // c = x1 - x3
  double d = firstHit.Y() - thirdHit.Y();  // d = y1 - y3
  double e = ((firstHit.X()*firstHit.X() - secondHit.X()*secondHit.X()) + (firstHit.Y()*firstHit.Y() - secondHit.Y()*secondHit.Y()))/2.0;  //e = ((x1*x1 - x2*x2) + (y1*y1 - y2*y2))/2.0;
  double f = ((firstHit.X()*firstHit.X() - thirdHit.X()*thirdHit.X()) + (firstHit.Y()*firstHit.Y() - thirdHit.Y()*thirdHit.Y()))/2.0;  //f = ((x1*x1 - x3*x3) + (y1*y1 - y3*y3))/2.0;
  double det = b*c - a*d;

  x_Cherenkov = -(d*e - b*f)/det;
  y_Cherenkov = -(a*f - c*e)/det;
  r_Cherenkov = TMath::Sqrt((x_Cherenkov-firstHit.X())*(x_Cherenkov-firstHit.X())+(y_Cherenkov-firstHit.Y())*(y_Cherenkov-firstHit.Y()));

  return true;
}

bool RingFinder::isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5) return true;
  if(TMath::Abs(firstHit.X()-thirdHit.X())  < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y())  < 1e-5) return true;
  if(TMath::Abs(secondHit.X()-thirdHit.X()) < 1e-5 && TMath::Abs(secondHit.Y()-thirdHit.Y()) < 1e-5) return true;
  if(firstHit.Mod() > 1000.0 || secondHit.Mod() > 1000.0 || thirdHit.Mod() > 1000.0) return true; // not proper initialized

  return false;
}

bool RingFinder::isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.X()-thirdHit.X()) < 1e-5) return true;
  if(TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y()) < 1e-5) return true;

  double slope12 = (firstHit.Y()-secondHit.Y())/(firstHit.X()-secondHit.X());
  double slope13 = (firstHit.Y()-thirdHit.Y())/(firstHit.X()-thirdHit.X());

  if(TMath::Abs(slope12-slope13) < 1e-5) return true;

  return false;
}

bool RingFinder::isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform)
{
  double x_diff = photonHit.X() - x_HoughTransform;
  double y_diff = photonHit.Y() - y_HoughTransform;
  double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r_HoughTransform;

  // double sigma_x = 6.0;
  // double sigma_y = 6.0;
  // double sigma_r = TMath::Sqrt(sigma_x*sigma_x+sigma_y*sigma_y);
  double sigma_r = 6.0;

  if( TMath::Abs(r_diff) < sigma_r) return true;

  return false;
}

int RingFinder::HoughTransform(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfPhotons = numOfPhotons;
  if(NumOfPhotons < 3) return 0;
  float NumOfCombinations = TMath::Factorial(NumOfPhotons)/(TMath::Factorial(3)*TMath::Factorial(NumOfPhotons-3));
  // cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfCombinations = " << NumOfCombinations << endl;
  for(int i_hit_1st = 0; i_hit_1st < NumOfPhotons-2; ++i_hit_1st)
  {
    TVector2 firstHit(-999.9,-999.9);
    double x_firstHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_1st]);
    double y_firstHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_1st]);
    firstHit.Set(x_firstHit,y_firstHit);
    for(int i_hit_2nd = i_hit_1st+1; i_hit_2nd < NumOfPhotons-1; ++i_hit_2nd)
    {
      TVector2 secondHit(-999.9,-999.9);
      double x_secondHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_2nd]);
      double y_secondHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_2nd]);
      secondHit.Set(x_secondHit,y_secondHit);
      for(int i_hit_3rd = i_hit_2nd+1; i_hit_3rd < NumOfPhotons; ++i_hit_3rd)
      {
	TVector2 thirdHit(-999.9,-999.9);
	double x_thirdHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_3rd]);
	double y_thirdHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_3rd]);
	thirdHit.Set(x_thirdHit,y_thirdHit);

	double x_Cherenkov = -999.9;
	double y_Cherenkov = -999.9;
	double r_Cherenkov = -999.9;

	bool ringStatus = findRing(firstHit,secondHit,thirdHit, x_Cherenkov, y_Cherenkov, r_Cherenkov);
	if(ringStatus) 
	{
	  h_mQA_HT->Fill(x_Cherenkov,y_Cherenkov,r_Cherenkov);
	  h_mHoughTransform->Fill(x_Cherenkov,y_Cherenkov,r_Cherenkov);
	  // cout << "firstHit.x = " << firstHit.X() << ", firstHit.y = " << firstHit.Y() << endl;
	  // cout << "secondHit.x = " << secondHit.X() << ", secondHit.y = " << secondHit.Y() << endl;
	  // cout << "thirdHit.x = " << thirdHit.X() << ", thirdHit.y = " << thirdHit.Y() << endl;
	  // cout << "x_Cherenkov = " << x_Cherenkov << ", y_Cherenkov = " << y_Cherenkov << ", r_Cherenkov = " << r_Cherenkov << endl;
	  // cout << endl;
	}
      }
    }
  }

  int hBin_x = -1;
  int hBin_y = -1;
  int hBin_r = -1;
  int NumOfPhotonsOnRing = 0;

  int globalBin = h_mHoughTransform->GetMaximumBin(hBin_x,hBin_y,hBin_r);
  int maxVote = h_mHoughTransform->GetBinContent(globalBin);
  if(globalBin > 0)
  {
    double x_HoughTransform = h_mHoughTransform->GetXaxis()->GetBinCenter(hBin_x);
    double y_HoughTransform = h_mHoughTransform->GetYaxis()->GetBinCenter(hBin_y);
    double r_HoughTransform = h_mHoughTransform->GetZaxis()->GetBinCenter(hBin_r);
    h_mCherenkovRing->Fill(x_HoughTransform,y_HoughTransform,r_HoughTransform);
    // cout << "hBin_x = " << hBin_x << ", hBin_y = " << hBin_y << ", hBin_r = " << hBin_r << ", globalBin = " << globalBin << ", with maxVote = " << maxVote << endl;
    // cout << "x_HoughTransform = " << x_HoughTransform << ", y_HoughTransform = " << y_HoughTransform << ", r_HoughTransform = " << r_HoughTransform << endl;

    for(int i_hit = 0; i_hit < NumOfPhotons; ++i_hit)
    {
      TVector2 photonHit;
      double x_photonHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit]);
      double y_photonHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit]);
      photonHit.Set(x_photonHit,y_photonHit);
      if( isOnRing(photonHit,x_HoughTransform,y_HoughTransform,r_HoughTransform) )
      {
	NumOfPhotonsOnRing++;
      }
    }
    // cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfPhotonsOnRing = " << NumOfPhotonsOnRing << endl;

    mRingCenter.Set(x_HoughTransform,y_HoughTransform);
    mRadius = r_HoughTransform;
    mNumOfPhotonsOnRing = NumOfPhotonsOnRing;
    mNumOfPhotonsOffRing = NumOfPhotons-NumOfPhotonsOnRing;

    if(NumOfPhotonsOnRing > 4 && TMath::Abs(x_HoughTransform) < 5.5 && TMath::Abs(y_HoughTransform) < 5.5)
    // if(TMath::Abs(x_HoughTransform) < 5.5 && TMath::Abs(y_HoughTransform) < 5.5)
    {
      h_mNumOfCherenkovPhotons->Fill(NumOfPhotons,NumOfPhotonsOnRing,r_HoughTransform);
      h_mNumOfPhotons_OnOffRing->Fill(NumOfPhotonsOnRing,NumOfPhotons-NumOfPhotonsOnRing);
    }
  }

  return 0;
}

//------------------------------------------------------

float RingFinder::findPixelCoord(int pixel)
{
  float out_coord = -999.9;

  if(pixel < 0 || pixel > 32) return -999.9; // out of photon sensor

  float out_low = mRICH::mPixels[pixel];
  float out_high = mRICH::mPixels[pixel+1];

  out_coord = 0.5*(out_low+out_high);

  return out_coord;
}

TVector2 RingFinder::getRingCenter()
{
  return mRingCenter;
}

float RingFinder::getRingRadius()
{
  return mRadius;
}

int RingFinder::getNumOfPhotonsOnRing()
{
  return mNumOfPhotonsOnRing;
}

int RingFinder::getNumOfPhotonsOffRing()
{
  return mNumOfPhotonsOffRing;
}

//------------------------------------------------------
