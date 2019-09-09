#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "../../draw.h"

void extractAngleResolution(const string airgap = "8mm")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH3D *h_mCherenkovPhotons = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH1D *h_mRadius = (TH1D*)h_mCherenkovPhotons->Project3D("z");
  TH2D *h_mNumRadius = (TH2D*)h_mCherenkovPhotons->Project3D("zx"); // x-axis num of photons on ring & y-axis ring radius

  float nref = 1.03;
  // float flength = 6.0*25.4 +8.0; // mm
  // float flength = 6.0*25.4 +3.0; // mm
  float flength = 6.0*25.4; // mm

  TCanvas *c_Radius = new TCanvas("c_Radius","c_Radius",10,10,500,500);
  c_Radius->cd()->SetLeftMargin(0.15);
  c_Radius->cd()->SetBottomMargin(0.15);
  c_Radius->cd()->SetRightMargin(0.15);
  c_Radius->cd()->SetTicks(1,1);
  c_Radius->cd()->SetGrid(0,0);

  h_mRadius->SetTitle("Minuit Fit"); // MF
  h_mRadius->SetLineColor(1);
  h_mRadius->SetLineWidth(1);
  h_mRadius->GetXaxis()->SetTitle("R (mm)");
  h_mRadius->GetXaxis()->CenterTitle();
  h_mRadius->GetXaxis()->SetRangeUser(32.0,52.0);
  h_mRadius->Draw();

  TF1 *f_gaus = new TF1("f_gaus","gaus",0,100);
  f_gaus->SetParameter(0,100);
  f_gaus->SetParameter(1,40.0);
  f_gaus->SetParameter(2,2.0);
  f_gaus->SetRange(35.0,45.0);
  h_mRadius->Fit(f_gaus,"NR");
  float norm_1st = f_gaus->GetParameter(0);
  float mean_1st = f_gaus->GetParameter(1);
  float sig_1st  = f_gaus->GetParameter(2);
  f_gaus->SetParameter(0,norm_1st);
  f_gaus->SetParameter(1,mean_1st);
  f_gaus->SetParameter(2,sig_1st);
  f_gaus->SetRange(mean_1st-3.0*sig_1st,mean_1st+3.0*sig_1st);
  h_mRadius->Fit(f_gaus,"NR");
  f_gaus->SetLineColor(2);
  f_gaus->SetLineStyle(2);
  f_gaus->SetLineWidth(3);
  f_gaus->Draw("l same");
  float mean_r = f_gaus->GetParameter(1);
  float sigma_r = f_gaus->GetParameter(2);
  string leg_gaus = Form("R: %1.2f #pm %1.2f (mm)",mean_r,sigma_r);
  plotTopLegend((char*)leg_gaus.c_str(),0.46,0.6,0.04,1,0.0,42,1,1);

  /*
  float tan_theta = mean_r/flength; // r/f
  float theta_tan = TMath::ATan2(mean_r,flength); // theta = atan(r/f)
  float sigma_theta = sigma_r/(flength*(1.0+tan_theta*tan_theta));
  cout << "theta (atan) = " << theta_tan << endl;
  */

  float dist_r = TMath::Sqrt(mean_r*mean_r+flength*flength);
  float sin_theta = mean_r/dist_r; // sin(theta_c)*n_c = sin(theta_air)*n_air
  float theta = TMath::ASin(sin_theta);
  float sigma_theta = sigma_r/(flength*(1.0+theta*theta));

  float sin_theta_c = mean_r/(dist_r*nref); // sin(theta_c)*n_c = sin(theta_air)*n_air
  float theta_c = TMath::ASin(sin_theta_c);
  float sigma_theta_c = TMath::Cos(theta)*TMath::Cos(theta)*TMath::Cos(theta)*sigma_r/(nref*flength*TMath::Cos(theta_c));
  // float sigma_theta_c = TMath::Cos(theta)*sigma_theta/(nref*TMath::Cos(theta_c));
  // cout << "theta (asin) = " << theta << ", theta_c = " << theta_c << endl;
  string leg_theta_c = Form("#theta_{c}: %1.2f #pm %1.2f (mRad)",theta_c*1000.0,sigma_theta_c*1000.0);
  plotTopLegend((char*)leg_theta_c.c_str(),0.46,0.5,0.04,1,0.0,42,1,1);

  string leg_sigma = Form("#sigma_{#theta_{c}} = %1.2f (mRad)",1000.0*sigma_theta_c);
  plotTopLegend((char*)leg_sigma.c_str(),0.46,0.4,0.04,1,0.0,42,1,1);

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_radius_%s.eps",airgap.c_str());
  c_Radius->SaveAs(fig_name.c_str());
}
