// inspired by $ROOTSYS/tutorials/fit/fitCircle.C
#include "Fit/Fitter.h"
#include "TFile.h"
#include "TH2D.h"
#include "TArc.h"

#include <vector>

using namespace std;

void findRingMinuit()
{
  string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root"; // data
  // string inputfile = "/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root"; // sim
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mRingFinder_SingleEvent"); // data
  // TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mPhotonDist_SingleEvent"); // sim

   //generates n points around a circle and fit them
   TCanvas *c1 = new TCanvas("c1","c1",600,600);
   h_mPhotonDist->Draw("colz");
   vector<int> mXPixelMap;
   vector<int> mYPixelMap;

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
	 mXPixelMap.push_back(x_photonHit);
	 mYPixelMap.push_back(y_photonHit);
       }
     }
   }

   auto chi2Function = [&](const Double_t *par) {
      //minimisation function computing the sum of squares of residuals
      // looping at the graph points
      int np = mXPixelMap.size();
      double f = 0;
      // double *x = gr->GetX();
      // double *y = gr->GetY();
      for (int i_photon = 0; i_photon < np;i_photon++) 
      {
         double u = mXPixelMap[i_photon] - par[0];
         double v = mYPixelMap[i_photon] - par[1];
         double dr = par[2] - std::sqrt(u*u+v*v);
         f += dr*dr;
      }
      return f;
   };

   // wrap chi2 funciton in a function object for the fit
   // 3 is the number of fit parameters (size of array par)
   ROOT::Math::Functor fcn(chi2Function,3);
   ROOT::Fit::Fitter  fitter;

   double pStart[3] = {0,0,1};
   fitter.SetFCN(fcn, pStart);
   fitter.Config().ParSettings(0).SetName("x0");
   fitter.Config().ParSettings(1).SetName("y0");
   fitter.Config().ParSettings(2).SetName("R");

   // do the fit 
   bool ok = fitter.FitFCN();
   if (!ok) {
      Error("line3Dfit","Line3D Fit failed");
   }   

   const ROOT::Fit::FitResult & result = fitter.Result();
   result.Print(std::cout);

   //Draw the circle on top of the points
   TArc *arc = new TArc(result.Parameter(0),result.Parameter(1),result.Parameter(2));
   arc->SetLineColor(kRed);
   arc->SetLineWidth(4);
   arc->SetFillStyle(0);
   arc->Draw();
}
