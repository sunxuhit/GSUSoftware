// inspired by $ROOTSYS/tutorials/fit/fitCircle.C
#include "Fit/Fitter.h"
#include "TFile.h"
#include "TH2D.h"
#include "TArc.h"
#include "TGraph.h"
#include <TStyle.h>

#include <vector>
#include <utility>
#include "/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/src/Simulation/Material/mRICH.h"

using namespace std;

TVector2 mRingCenter_MF;
float mRadius_MF;
int mNumOfPhotonsOnRing_MF;
int mNumOfPhotonsOffRing_MF;

std::vector<int> mXPixel_MF;
std::vector<int> mYPixel_MF;
TH2D *h_mRingFinder_MF; // x: photon out_x | y: photon out_y with detector effect

TVector2 mRingCenter_plot[10];
float mRadius_plot[10];
TH2D *h_mRingFinder_plot[10];
TArc *arc[10];
TGraph *g_center[10];

int MinuitFit(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel);
std::pair<int,int> minuitRingRadius(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel);
int clearRingFinder_mRR();
int clearRingFinder_MF();
bool isOnRing(TVector2 photonHit, double x0, double y0, double r0);

void findRing_MinuitFit()
{
  gStyle->SetPalette(kDeepSea);

  string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root"; // data
  // string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root"; // sim
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mRingFinder_SingleEvent"); // data
  // TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mPhotonDist_SingleEvent"); // sim

  vector<int> mXPixelMap;
  vector<int> mYPixelMap;
  int numOfPhotons = 0;
  for(int i_binx = 0; i_binx < h_mPhotonDist->GetNbinsX(); ++i_binx)
  {
    for(int i_biny = 0; i_biny < h_mPhotonDist->GetNbinsY(); ++i_biny)
    {
      int global_bin = h_mPhotonDist->GetBin(i_binx+1,i_biny+1);
      if(h_mPhotonDist->GetBinContent(global_bin) > 0)
      {
	// cout << "i_binx = " << i_binx << ", i_biny = " << i_biny << ", global_bin = " << global_bin << endl;
	double x_photonHit = h_mPhotonDist->GetXaxis()->GetBinCenter(i_binx+1);
	double y_photonHit = h_mPhotonDist->GetYaxis()->GetBinCenter(i_biny+1);
	// cout << "x_photonHit = " << x_photonHit << ", y_photonHit = " << y_photonHit << endl;
	mXPixelMap.push_back(i_binx+1);
	mYPixelMap.push_back(i_biny+1);
	numOfPhotons++;
      }
    }
  }

  h_mRingFinder_MF = new TH2D("h_mRingFinder_MF","h_mRingFinder_MF",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  clearRingFinder_MF();
  int numOfFit = MinuitFit(numOfPhotons,h_mPhotonDist,mXPixelMap,mYPixelMap);
  cout << "numOfFit = " << numOfFit << endl;

  //generates n points around a circle and fit them
  TCanvas *c_ringFinder = new TCanvas("c_ringFinder","c_ringFinder",10,10,numOfFit*800,800);
  c_ringFinder->Divide(numOfFit,1);
  for(int i_pad = 0; i_pad < numOfFit; ++i_pad)
  {
    c_ringFinder->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ringFinder->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ringFinder->cd(i_pad+1)->SetTicks(1,1);
    c_ringFinder->cd(i_pad+1)->SetGrid(1,1);
    c_ringFinder->cd(i_pad+1);

    string title = std::to_string(i_pad+1) + " MinuitFit";
    h_mRingFinder_plot[i_pad]->SetStats(0);
    h_mRingFinder_plot[i_pad]->SetTitle(title.c_str());
    h_mRingFinder_plot[i_pad]->GetXaxis()->SetTitle("x (mm)");
    h_mRingFinder_plot[i_pad]->GetXaxis()->CenterTitle();
    h_mRingFinder_plot[i_pad]->GetYaxis()->SetTitle("y (mm)");
    h_mRingFinder_plot[i_pad]->GetYaxis()->CenterTitle();
    h_mRingFinder_plot[i_pad]->Draw("colz");

    //Draw the circle on top of the points
    arc[i_pad] = new TArc(mRingCenter_plot[i_pad].X(),mRingCenter_plot[i_pad].Y(),mRadius_plot[i_pad]);
    arc[i_pad]->SetLineColor(kRed);
    arc[i_pad]->SetLineWidth(4);
    arc[i_pad]->SetFillStyle(0);
    arc[i_pad]->Draw();

    g_center[i_pad] = new TGraph();
    g_center[i_pad]->SetPoint(0,mRingCenter_plot[i_pad].X(),mRingCenter_plot[i_pad].Y());
    g_center[i_pad]->SetMarkerStyle(29);
    g_center[i_pad]->SetMarkerColor(kRed);
    g_center[i_pad]->SetMarkerSize(1.8);
    g_center[i_pad]->Draw("p same");
  }

  string ringFinder = "/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_ringFinder_MF.eps";
  c_ringFinder->SaveAs(ringFinder.c_str());
}

int MinuitFit(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfFit = 1;
  std::pair<int,int> NumInfo_MF = minuitRingRadius(numOfPhotons,h_RingFinder,xPixel,yPixel);
  int Npe_MF = NumInfo_MF.first; // num of photons on ring
  int Nbkg_MF = NumInfo_MF.second; // num of photons off ring

  // save ploting histograms
  h_mRingFinder_plot[0] = (TH2D*)h_RingFinder->Clone("h_mRingFinder_plot_0");
  mRingCenter_plot[0] = mRingCenter_MF;
  mRadius_plot[0] = mRadius_MF;
  cout << "Plot: x_MinuitFit = " << mRingCenter_plot[0].X() << ", y_MinuitFit = " << mRingCenter_plot[0].Y() << ", r_MinuitFit = " << mRadius_plot[0] << endl;

  while(Nbkg_MF > 0)
  {
    TH2D *h_RingFinder_Temp = (TH2D*)h_mRingFinder_MF->Clone("h_RingFinder_Temp");;
    std::vector<int> xPixel_temp = mXPixel_MF;
    std::vector<int> yPixel_temp = mYPixel_MF;
    clearRingFinder_mRR();

    NumInfo_MF = minuitRingRadius(Npe_MF,h_RingFinder_Temp,xPixel_temp,yPixel_temp);
    Npe_MF = NumInfo_MF.first;
    Nbkg_MF = NumInfo_MF.second;

    // save ploting histograms
    std::string HistName = "h_mRingFinder_plot_" + std::to_string(NumOfFit);
    cout << HistName.c_str() << endl;;
    h_mRingFinder_plot[NumOfFit] = (TH2D*)h_RingFinder_Temp->Clone(HistName.c_str());
    mRingCenter_plot[NumOfFit] = mRingCenter_MF;
    mRadius_plot[NumOfFit] = mRadius_MF;
    cout << "Plot: x_MinuitFit = " << mRingCenter_plot[NumOfFit].X() << ", y_MinuitFit = " << mRingCenter_plot[NumOfFit].Y() << ", r_MinuitFit = " << mRadius_plot[NumOfFit] << endl;

    NumOfFit++;
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
  // cout << "Npe_MF = " << mNumOfPhotonsOnRing_MF << ", Nbkg_MF = " << mNumOfPhotonsOffRing_MF << endl;

  return NumOfFit;
}

std::pair<int,int> minuitRingRadius(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
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

  // fitter.Config().ParSettings(0).Fix();
  // fitter.Config().ParSettings(1).Fix();

  // do the fit 
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("line3Dfit","Line3D Fit failed");
    return NumInfo;
  }   

  const ROOT::Fit::FitResult & result = fitter.Result();
  result.Print(std::cout);
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
  mRingCenter_MF.Set(x_MinuitFit,y_MinuitFit);
  mRadius_MF = r_MinuitFit;
  cout << "x_MinuitFit = " << x_MinuitFit << ", y_MinuitFit = " << y_MinuitFit << ", r_MinuitFit = " << r_MinuitFit << endl;
  cout << "NumOfPhotons = " << NumOfPhotons << ", Npe = " << NumInfo.first << ", Nbkg = " << NumInfo.second << endl;
  cout << "--------------------------------" << endl;

  return NumInfo;
}

int clearRingFinder_mRR()
{
  h_mRingFinder_MF->Reset();
  mXPixel_MF.clear();
  mYPixel_MF.clear();

  return 1;
}

int clearRingFinder_MF()
{
  mRingCenter_MF.Set(-999.9,-999.9);
  mRadius_MF = -999.9;
  mNumOfPhotonsOnRing_MF = -1;
  mNumOfPhotonsOffRing_MF = -1;

  clearRingFinder_mRR();
  
  return 1;
}

bool isOnRing(TVector2 photonHit, double x0, double y0, double r0)
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
