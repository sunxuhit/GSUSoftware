#include <vector>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TVector2.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TStyle.h>
#include <TGraph.h>

bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov);
bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
bool isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform);

int findRing_HoughTransform()
{
  gStyle->SetPalette(kDeepSea);

  string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root"; // data
  // string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root"; // sim
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mRingFinder_SingleEvent"); // data
  // TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mPhotonDist_SingleEvent"); // sim

  TCanvas *c_ringFinder = new TCanvas("c_ringFinder","c_ringFinder",10,10,800,800);
  c_ringFinder->SetLeftMargin(0.15);
  c_ringFinder->SetBottomMargin(0.15);
  c_ringFinder->SetTicks(1,1);
  c_ringFinder->SetGrid(1,1);
  h_mPhotonDist->SetStats(0);
  h_mPhotonDist->SetTitle("Hough Transform");
  h_mPhotonDist->Draw("colz");

  // ring finder QA
  int NumOfBinX = h_mPhotonDist->GetXaxis()->GetNbins();
  int NumOfBinY = h_mPhotonDist->GetYaxis()->GetNbins();
  int NumOfPhotons = 0;
  std::vector<int> xPixel; // corresponding binX number for each photon hit
  std::vector<int> yPixel; // corresponding binY number for each photon hit

  for(int i_bin_x = 0; i_bin_x < NumOfBinX; ++i_bin_x)
  {
    for(int i_bin_y = 0; i_bin_y < NumOfBinY; ++i_bin_y)
    {
      double is_photon = h_mPhotonDist->GetBinContent(i_bin_x+1,i_bin_y+1);
      if(is_photon > 0) 
      {
	cout << "i_bin_x = " << i_bin_x+1 << ", i_bin_y = " << i_bin_y+1 << ", is_photon = " << is_photon << endl;
	NumOfPhotons++;
	xPixel.push_back(i_bin_x+1);
	yPixel.push_back(i_bin_y+1);
      }
    }
  }
  cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfEntries = " << h_mPhotonDist->GetEntries() << endl;

  if(NumOfPhotons < 3) return 0;
  // TH3D *h_HoughTransform = new TH3D("h_HoughTransform","h_HoughTransform",105,-52.5,52.5,105,-52.5,52.5,105,0,52.5);
  TH3D *h_HoughTransform = new TH3D("h_HoughTransform","h_HoughTransform",108,-54.0,54.0,108,-54.0,54.0,108,0,54.0);
  float NumOfCombinations = TMath::Factorial(NumOfPhotons)/(TMath::Factorial(3)*TMath::Factorial(NumOfPhotons-3));
  cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfCombinations = " << NumOfCombinations << endl;
  TEllipse *circle[(int)NumOfCombinations];
  int counter = 0;
  for(int i_hit_1st = 0; i_hit_1st < NumOfPhotons-2; ++i_hit_1st)
  {
    TVector2 firstHit(-999.9,-999.9);
    double x_firstHit = h_mPhotonDist->GetXaxis()->GetBinCenter(xPixel[i_hit_1st]);
    double y_firstHit = h_mPhotonDist->GetYaxis()->GetBinCenter(yPixel[i_hit_1st]);
    firstHit.Set(x_firstHit,y_firstHit);
    for(int i_hit_2nd = i_hit_1st+1; i_hit_2nd < NumOfPhotons-1; ++i_hit_2nd)
    {
      TVector2 secondHit(-999.9,-999.9);
      double x_secondHit = h_mPhotonDist->GetXaxis()->GetBinCenter(xPixel[i_hit_2nd]);
      double y_secondHit = h_mPhotonDist->GetYaxis()->GetBinCenter(yPixel[i_hit_2nd]);
      secondHit.Set(x_secondHit,y_secondHit);
      for(int i_hit_3rd = i_hit_2nd+1; i_hit_3rd < NumOfPhotons; ++i_hit_3rd)
      {
	TVector2 thirdHit(-999.9,-999.9);
	double x_thirdHit = h_mPhotonDist->GetXaxis()->GetBinCenter(xPixel[i_hit_3rd]);
	double y_thirdHit = h_mPhotonDist->GetYaxis()->GetBinCenter(yPixel[i_hit_3rd]);
	thirdHit.Set(x_thirdHit,y_thirdHit);

	double x_Cherenkov = -999.9;
	double y_Cherenkov = -999.9;
	double r_Cherenkov = -999.9;

	bool ringStatus = findRing(firstHit,secondHit,thirdHit, x_Cherenkov, y_Cherenkov, r_Cherenkov);
	if(ringStatus) 
	{
	  h_HoughTransform->Fill(x_Cherenkov,y_Cherenkov,r_Cherenkov);

	  circle[counter] = new TEllipse(x_Cherenkov,y_Cherenkov,r_Cherenkov,r_Cherenkov);
	  circle[counter]->SetFillColor(0);
	  circle[counter]->SetFillStyle(0);
	  circle[counter]->SetLineWidth(1);
	  circle[counter]->SetLineColor(4);
	  if( counter % 10 == 1) circle[counter]->Draw("same");
	  counter++;

	  /* cout << "firstHit.x = " << firstHit.X() << ", firstHit.y = " << firstHit.Y() << endl; */
	  /* cout << "secondHit.x = " << secondHit.X() << ", secondHit.y = " << secondHit.Y() << endl; */
	  /* cout << "thirdHit.x = " << thirdHit.X() << ", thirdHit.y = " << thirdHit.Y() << endl; */
	  /* cout << "x_Cherenkov = " << x_Cherenkov << ", y_Cherenkov = " << y_Cherenkov << ", r_Cherenkov = " << r_Cherenkov << endl; */
	  /* cout << endl; */
	}
      }
    }
  }


  int hBin_x = -1;
  int hBin_y = -1;
  int hBin_r = -1;
  int NumOfPhotonsOnRing = 0;

  int globalBin = h_HoughTransform->GetMaximumBin(hBin_x,hBin_y,hBin_r);
  int maxVote = h_HoughTransform->GetBinContent(globalBin);
  double x_HoughTransform = -999.9;
  double y_HoughTransform = -999.9;
  double r_HoughTransform = 0.0;
  if(globalBin > 0)
  {
    x_HoughTransform = h_HoughTransform->GetXaxis()->GetBinCenter(hBin_x);
    y_HoughTransform = h_HoughTransform->GetYaxis()->GetBinCenter(hBin_y);
    r_HoughTransform = h_HoughTransform->GetZaxis()->GetBinCenter(hBin_r);
    cout << "hBin_x = " << hBin_x << ", hBin_y = " << hBin_y << ", hBin_r = " << hBin_r << ", globalBin = " << globalBin << ", with maxVote = " << maxVote << endl;
    cout << "x_HoughTransform = " << x_HoughTransform << ", y_HoughTransform = " << y_HoughTransform << ", r_HoughTransform = " << r_HoughTransform << endl;

    for(int i_hit = 0; i_hit < NumOfPhotons; ++i_hit)
    {
      TVector2 photonHit;
      double x_photonHit = h_mPhotonDist->GetXaxis()->GetBinCenter(xPixel[i_hit]);
      double y_photonHit = h_mPhotonDist->GetYaxis()->GetBinCenter(yPixel[i_hit]);
      photonHit.Set(x_photonHit,y_photonHit);
      if( isOnRing(photonHit,x_HoughTransform,y_HoughTransform,r_HoughTransform) )
      {
	NumOfPhotonsOnRing++;
      }
    }
    cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfPhotonsOnRing = " << NumOfPhotonsOnRing << endl;
  }

  TEllipse *circle_HoughTransform = new TEllipse(x_HoughTransform,y_HoughTransform,r_HoughTransform,r_HoughTransform);
  circle_HoughTransform->SetFillColor(0);
  circle_HoughTransform->SetFillStyle(0);
  circle_HoughTransform->SetLineWidth(4);
  circle_HoughTransform->SetLineColor(2);
  circle_HoughTransform->Draw("same");
  h_mPhotonDist->Draw("col same");

  TGraph *g_center = new TGraph();
  g_center->SetPoint(0,x_HoughTransform,y_HoughTransform);
  g_center->SetMarkerStyle(29);
  g_center->SetMarkerColor(kRed);
  g_center->SetMarkerSize(1.8);
  g_center->Draw("p same");

  string ringFinder = "/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_ringFinder_HT.eps";
  c_ringFinder->SaveAs(ringFinder.c_str());

  return 1;
}


bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov)
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

bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5) return true;
  if(TMath::Abs(firstHit.X()-thirdHit.X())  < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y())  < 1e-5) return true;
  if(TMath::Abs(secondHit.X()-thirdHit.X()) < 1e-5 && TMath::Abs(secondHit.Y()-thirdHit.Y()) < 1e-5) return true;
  if(firstHit.Mod() > 1000.0 || secondHit.Mod() > 1000.0 || thirdHit.Mod() > 1000.0) return true; // not proper initialized

  return false;
}

bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.X()-thirdHit.X()) < 1e-5) return true;
  if(TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y()) < 1e-5) return true;

  double slope12 = (firstHit.Y()-secondHit.Y())/(firstHit.X()-secondHit.X());
  double slope13 = (firstHit.Y()-thirdHit.Y())/(firstHit.X()-thirdHit.X());

  if(TMath::Abs(slope12-slope13) < 1e-5) return true;

  return false;
}

bool isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform)
{
  double x_diff = photonHit.X() - x_HoughTransform;
  double y_diff = photonHit.Y() - y_HoughTransform;
  double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r_HoughTransform;

  double sigma_x = 1.5;
  double sigma_y = 1.5;
  double sigma_r = TMath::Sqrt(sigma_x*sigma_x+sigma_y*sigma_y);

  if( TMath::Abs(r_diff) < sigma_r) return true;

  return false;
}
