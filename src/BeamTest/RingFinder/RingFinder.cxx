#include <iostream>
#include <cstdlib>
#include <string> 
#include <TMath.h>
#include <Fit/Fitter.h>
#include <Math/Functor.h>
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
// Hough Transform
int RingFinder::initRingFinder_HT()
{
  cout << "initRingFinder: initialized histograms for Hough Transform!" << endl;
  h_mHoughTransform = new TH3D("h_mHoughTransform","h_mHoughTransform",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);

  h_mCherenkovRing_HT = new TH3D("h_mCherenkovRing_HT","h_mCherenkovRing_HT",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);
  // h_mCherenkovPhotons_HT = new TH3D("h_mCherenkovPhotons_HT","h_mCherenkovPhotons_HT",50,-0.5,49.5,50,-0.5,49.5,210,0,2.0*mRICH::mHalfWidth);
  h_mCherenkovPhotons_HT = new TH3D("h_mCherenkovPhotons_HT","h_mCherenkovPhotons_HT",50,-0.5,49.5,50,-0.5,49.5,150,29.5,59.5);
  h_mNumOfCherenkovPhotons_HT = new TH3D("h_mNumOfCherenkovPhotons_HT","h_mNumOfCherenkovPhotons_HT",50,-0.5,49.5,50,-0.5,49.5,50,-0.5,49.5);

  clearRingFinder_HT();

  return 1;
}

int RingFinder::clearRingFinder_HT()
{
  h_mHoughTransform->Reset();

  mRingCenter_HT.Set(-999.9,-999.9);
  mRadius_HT = -999.9;
  mNumOfPhotonsOnRing_HT = -1;
  mNumOfPhotonsOffRing_HT = -1;

  return 1;
}

int RingFinder::writeRingFinder_HT()
{
  h_mCherenkovRing_HT->Write();
  h_mCherenkovPhotons_HT->Write();
  h_mNumOfCherenkovPhotons_HT->Write();

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
    h_mCherenkovRing_HT->Fill(x_HoughTransform,y_HoughTransform,r_HoughTransform);
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

    mRingCenter_HT.Set(x_HoughTransform,y_HoughTransform);
    mRadius_HT = r_HoughTransform;
    mNumOfPhotonsOnRing_HT = NumOfPhotonsOnRing;
    mNumOfPhotonsOffRing_HT = NumOfPhotons-NumOfPhotonsOnRing;

    if(NumOfPhotonsOnRing > 4 && TMath::Abs(x_HoughTransform) < 3.0 && TMath::Abs(y_HoughTransform) < 3.0)
    {
      h_mCherenkovPhotons_HT->Fill(NumOfPhotonsOnRing,NumOfPhotons-NumOfPhotonsOnRing,r_HoughTransform);
      h_mNumOfCherenkovPhotons_HT->Fill(NumOfPhotonsOnRing,NumOfPhotons-NumOfPhotonsOnRing,NumOfPhotons);
    }
  }

  return 0;
}

TVector2 RingFinder::getRingCenter_HT()
{
  return mRingCenter_HT;
}

float RingFinder::getRingRadius_HT()
{
  return mRadius_HT;
}

int RingFinder::getNumOfPhotonsOnRing_HT()
{
  return mNumOfPhotonsOnRing_HT;
}

int RingFinder::getNumOfPhotonsOffRing_HT()
{
  return mNumOfPhotonsOffRing_HT;
}
//------------------------------------------------------
// Minuit Fit
int RingFinder::initRingFinder_MF()
{
  cout << "initRingFinder: initialized histograms for Minuit Fit!" << endl;

  h_mCherenkovRing_MF = new TH3D("h_mCherenkovRing_MF","h_mCherenkovRing_MF",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);
  // h_mCherenkovPhotons_MF = new TH3D("h_mCherenkovPhotons_MF","h_mCherenkovPhotons_MF",50,-0.5,49.5,50,-0.5,49.5,210,0,2.0*mRICH::mHalfWidth);
  h_mCherenkovPhotons_MF = new TH3D("h_mCherenkovPhotons_MF","h_mCherenkovPhotons_MF",50,-0.5,49.5,50,-0.5,49.5,150,29.5,59.5);
  h_mNumOfCherenkovPhotons_MF = new TH3D("h_mNumOfCherenkovPhotons_MF","h_mNumOfCherenkovPhotons_MF",50,-0.5,49.5,50,-0.5,49.5,50,-0.5,49.5);
  h_mSinglePhoton = new TH3D("h_mSinglePhoton","h_mSinglePhoton",150,0.10,0.40,150,0.10,0.40,150,29.5,59.5);

  h_mRingFinder_MF = new TH2D("h_mRingFinder_MF","h_mRingFinder_MF",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels); // reset for each minuitRingRadius fit

  clearRingFinder_MF();

  return 1;
}

int RingFinder::clearRingFinder_MF()
{
  mRingCenter_MF.Set(-999.9,-999.9);
  mRadius_MF = -999.9;
  mNumOfPhotonsOnRing_MF = -1;
  mNumOfPhotonsOffRing_MF = -1;

  clearRingFinder_mRR();
  
  return 1;
}

int RingFinder::clearRingFinder_mRR()
{
  h_mRingFinder_MF->Reset();
  mXPixel_MF.clear();
  mYPixel_MF.clear();

  return 1;
}

int RingFinder::writeRingFinder_MF()
{
  h_mCherenkovRing_MF->Write();
  h_mCherenkovPhotons_MF->Write();
  h_mNumOfCherenkovPhotons_MF->Write();
  h_mSinglePhoton->Write();

  return 1;
}

int RingFinder::MinuitFit(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  std::pair<int,int> NumInfo_MF = minuitRingRadius(numOfPhotons,h_RingFinder,xPixel,yPixel);
  int Npe_MF = NumInfo_MF.first; // num of photons on ring
  int Nbkg_MF = NumInfo_MF.second; // num of photons off ring
  while(Nbkg_MF > 0)
  {
    TH2D *h_RingFinder_Temp = (TH2D*)h_mRingFinder_MF->Clone("h_RingFinder_Temp");;
    std::vector<int> xPixel_temp = mXPixel_MF;
    std::vector<int> yPixel_temp = mYPixel_MF;
    clearRingFinder_mRR();

    NumInfo_MF = minuitRingRadius(Npe_MF,h_RingFinder_Temp,xPixel_temp,yPixel_temp);
    Npe_MF = NumInfo_MF.first;
    Nbkg_MF = NumInfo_MF.second;
  }
  if(Nbkg_MF == 0)
  {
    mNumOfPhotonsOnRing_MF = NumInfo_MF.first;
    mNumOfPhotonsOffRing_MF = numOfPhotons - mNumOfPhotonsOnRing_MF;
  }
  if(Nbkg_MF < 0)
  {
    mNumOfPhotonsOnRing_MF = -1;
    mNumOfPhotonsOffRing_MF = -1;
    return -1;
  }
  // cout << "x_MinuitFit = " << mRingCenter_MF.X() << ", y_MinuitFit = " << mRingCenter_MF.Y() << ", r_MinuitFit = " << mRadius_MF << endl;
  // cout << "Npe_MF = " << mNumOfPhotonsOnRing_MF << ", Nbkg_MF = " << mNumOfPhotonsOffRing_MF << endl;

  h_mCherenkovRing_MF->Fill(mRingCenter_MF.X(),mRingCenter_MF.Y(),mRadius_MF);
  if(mNumOfPhotonsOnRing_MF > 4 && TMath::Abs(mRingCenter_MF.X()) < 3.0 && TMath::Abs(mRingCenter_MF.Y()) < 3.0)
  {
    h_mCherenkovPhotons_MF->Fill(mNumOfPhotonsOnRing_MF,mNumOfPhotonsOffRing_MF,mRadius_MF);
    h_mNumOfCherenkovPhotons_MF->Fill(mNumOfPhotonsOnRing_MF,mNumOfPhotonsOffRing_MF,numOfPhotons);

    /*
    double flength = 6.0*25.4; // mm
    double nref = 1.03;
    for(int i_photon = 0; i_photon < mNumOfPhotonsOnRing_MF; ++i_photon) // fit single photon radiusa distribution
    {
      double x_single = h_mRingFinder_MF->GetXaxis()->GetBinCenter(mXPixel_MF[i_photon]) - mRingCenter_MF.X();
      double y_single = h_mRingFinder_MF->GetYaxis()->GetBinCenter(mYPixel_MF[i_photon]) - mRingCenter_MF.Y();

      double r_singlephoton = TMath::Sqrt(x_single*x_single+y_single*y_single);

      float dist_r = TMath::Sqrt(r_singlephoton*r_singlephoton+flength*flength);
      float sin_theta_air = r_singlephoton/dist_r; // sin(theta_c)*n_c = sin(theta_air)*n_air
      float theta_air = TMath::ASin(sin_theta_air);

      float sin_theta_c = r_singlephoton/(dist_r*nref); // sin(theta_c)*n_c = sin(theta_air)*n_air
      float theta_c = TMath::ASin(sin_theta_c);

      h_mSinglePhoton->Fill(theta_air, theta_c, r_singlephoton);
    }
    */
  }

  return 1;
}

std::pair<int,int> RingFinder::minuitRingRadius(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  // return number of photons on and off ring
  // first: num of photons on ring | second: num of photons off ring
  std::pair<int,int> NumInfo = std::make_pair(-1,-1);

  int NumOfPhotons = numOfPhotons;
  if(NumOfPhotons < 3) return NumInfo;

  auto chi2Function = [&](const Double_t *par) 
  {
    //minimisation function computing the sum of squares of residuals
    double f = 0;
    for (int i_photon = 0; i_photon < NumOfPhotons; i_photon++) 
    {
      double dx = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_photon]) - par[0];
      double dy = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_photon]) - par[1];
      double dr = par[2] - std::sqrt(dx*dx+dy*dy);
      f += dr*dr;
    }
    return f;
  };

  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,3);
  ROOT::Fit::Fitter fitter;

  double pStart[3] = {0,0,1};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("x0");
  fitter.Config().ParSettings(1).SetName("y0");
  fitter.Config().ParSettings(2).SetName("R");

  // do the fit 
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("minuitRingRadius","Minuit Fit failed");
    return NumInfo;
  }   

  const ROOT::Fit::FitResult & result = fitter.Result();
  const double *fitpar = result.GetParams();
  double x_MinuitFit = fitpar[0];
  double y_MinuitFit = fitpar[1];
  double r_MinuitFit = fitpar[2];

  int Npe = 0;
  for(int i_photon = 0; i_photon < NumOfPhotons; ++i_photon)
  {
    TVector2 photonHit;
    double x_photonHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_photon]);
    double y_photonHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_photon]);
    photonHit.Set(x_photonHit,y_photonHit);
    if( isOnRing(photonHit,x_MinuitFit,y_MinuitFit,r_MinuitFit) )
    {
      Npe++;
      mXPixel_MF.push_back(xPixel[i_photon]);
      mYPixel_MF.push_back(yPixel[i_photon]);
      h_mRingFinder_MF->Fill(x_photonHit,y_photonHit);
    }
  }
  int Nbkg = NumOfPhotons-Npe;
  NumInfo = std::make_pair(Npe,Nbkg);
  // cout << "NumOfPhotons = " << NumOfPhotons << ", Npe = " << NumInfo.first << ", Nbkg = " << NumInfo.second << endl;
  if(Nbkg == 0)
  {
    mRingCenter_MF.Set(x_MinuitFit,y_MinuitFit);
    mRadius_MF = r_MinuitFit;
    // result.Print(std::cout);
    // cout << "x_MinuitFit = " << x_MinuitFit << ", y_MinuitFit = " << y_MinuitFit << ", r_MinuitFit = " << r_MinuitFit << endl;
    // cout << "NumOfPhotons = " << NumOfPhotons << ", Npe = " << NumInfo.first << ", Nbkg = " << NumInfo.second << endl;
    // cout << "--------------------------------" << endl;
  }

  return NumInfo;
}

TVector2 RingFinder::getRingCenter_MF()
{
  return mRingCenter_MF;
}

float RingFinder::getRingRadius_MF()
{
  return mRadius_MF;
}

int RingFinder::getNumOfPhotonsOnRing_MF()
{
  return mNumOfPhotonsOnRing_MF;
}

int RingFinder::getNumOfPhotonsOffRing_MF()
{
  return mNumOfPhotonsOffRing_MF;
}

//------------------------------------------------------

bool RingFinder::isOnRing(TVector2 photonHit, double x0, double y0, double r0)
{
  double x_diff = photonHit.X() - x0;
  double y_diff = photonHit.Y() - y0;
  double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r0;

  // double sigma_x = 6.0;
  // double sigma_y = 6.0;
  // double sigma_r = TMath::Sqrt(sigma_x*sigma_x+sigma_y*sigma_y);
  double sigma_r = 6.0;

  if( TMath::Abs(r_diff) < sigma_r) return true;

  return false;
}

float RingFinder::findPixelCoord(int pixel)
{
  float out_coord = -999.9;

  if(pixel < 0 || pixel > 32) return -999.9; // out of photon sensor

  float out_low = mRICH::mPixels[pixel];
  float out_high = mRICH::mPixels[pixel+1];

  out_coord = 0.5*(out_low+out_high);

  return out_coord;
}
//------------------------------------------------------
