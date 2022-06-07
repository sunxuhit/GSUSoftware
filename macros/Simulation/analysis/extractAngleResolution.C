#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "../../draw.h"

double doubleGauss(double *x_val, double *par)
{
  double x = x_val[0];

  double mean = par[0];

  double sigma_1st = par[1];
  double norm_1st = par[2];
  double sigSquare_1st = sigma_1st*sigma_1st;
  double power_1st = -0.5*(x-mean)*(x-mean)/sigSquare_1st;
  double gaus_1st = norm_1st*TMath::Exp(power_1st)/TMath::Sqrt(2.0*TMath::Pi()*sigSquare_1st);

  double sigma_2nd = par[3];
  double norm_2nd = par[4];
  double sigSquare_2nd = sigma_2nd*sigma_2nd;
  double power_2nd = -0.5*(x-mean)*(x-mean)/sigSquare_2nd;
  double gaus_2nd = norm_2nd*TMath::Exp(power_2nd)/TMath::Sqrt(2.0*TMath::Pi()*sigSquare_2nd);

  double y = gaus_1st + gaus_2nd;

  return y;
}

double Gauss(double *x_val, double *par)
{
  double x = x_val[0];

  double mean = par[0];
  double sigma = par[1];
  double norm = par[2];
  double sigSquare = sigma*sigma;
  double power = -0.5*(x-mean)*(x-mean)/sigSquare;
  double gaus = norm*TMath::Exp(power)/TMath::Sqrt(2.0*TMath::Pi()*sigSquare);

  return gaus;
}

void extractAngleResolution(const string airgap = "3mm")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH3D *h_mCherenkovPhotons = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH1D *h_mRadius = (TH1D*)h_mCherenkovPhotons->Project3D("z");
  TH2D *h_mNumRadius = (TH2D*)h_mCherenkovPhotons->Project3D("zx"); // x-axis num of photons on ring & y-axis ring radius

  float nref = 1.03;
  // float flength = 6.0*25.4 +8.0; // mm
  // float flength = 6.0*25.4 +3.7; // mm
  float flength = 6.0*25.4; // mm
  float nSigma = 1.0;

  TCanvas *c_Radius = new TCanvas("c_Radius","c_Radius",10,10,500,500);
  c_Radius->cd()->SetLeftMargin(0.15);
  c_Radius->cd()->SetBottomMargin(0.15);
  c_Radius->cd()->SetRightMargin(0.15);
  c_Radius->cd()->SetTicks(1,1);
  c_Radius->cd()->SetGrid(0,0);
  // c_Radius->cd()->SetLogy(1);

  h_mRadius->SetTitle(airgap.c_str()); // MF
  h_mRadius->SetLineColor(1);
  h_mRadius->SetLineWidth(1);
  h_mRadius->GetXaxis()->SetTitle("R (mm)");
  h_mRadius->GetXaxis()->CenterTitle();
  h_mRadius->GetXaxis()->SetRangeUser(30.0,48.0);
  h_mRadius->GetYaxis()->SetRangeUser(0.1,h_mRadius->GetMaximum()*1.2);
  h_mRadius->Draw("hE");

  /*
  TF1 *f_gaus = new TF1("f_gaus",doubleGauss,0,100,5);
  f_gaus->SetParameter(0,40.0);
  f_gaus->SetParameter(1,2.0);
  f_gaus->SetParLimits(1,0.0,10.0);
  f_gaus->SetParameter(2,100.0);
  f_gaus->SetParameter(3,10.0);
  f_gaus->SetParLimits(3,0.0,10.0);
  f_gaus->SetParameter(4,100.0);
  f_gaus->SetRange(34.0,50.0);
  h_mRadius->Fit(f_gaus,"NR");
  f_gaus->SetLineColor(2);
  f_gaus->SetLineStyle(1);
  f_gaus->SetLineWidth(3);
  f_gaus->Draw("l same");

  double mean = f_gaus->GetParameter(0);
  double sigma_1st = f_gaus->GetParameter(1);
  double norm_1st = f_gaus->GetParameter(2);
  double sigma_2nd = f_gaus->GetParameter(3);
  double norm_2nd = f_gaus->GetParameter(4);

  TF1 *f_gaus_1st = new TF1("f_gaus_1st",Gauss,0,100,3);
  f_gaus_1st->FixParameter(0,mean);
  f_gaus_1st->FixParameter(1,sigma_1st);
  f_gaus_1st->FixParameter(2,norm_1st);
  f_gaus_1st->SetRange(34.0,50.0);
  f_gaus_1st->SetLineColor(1);
  f_gaus_1st->SetLineStyle(2);
  f_gaus_1st->SetLineWidth(2);
  f_gaus_1st->Draw("l same");

  TF1 *f_gaus_2nd = new TF1("f_gaus_2nd",Gauss,0,100,3);
  f_gaus_2nd->FixParameter(0,mean);
  f_gaus_2nd->FixParameter(1,sigma_2nd);
  f_gaus_2nd->FixParameter(2,norm_2nd);
  f_gaus_2nd->SetRange(34.0,50.0);
  f_gaus_2nd->SetLineColor(4);
  f_gaus_2nd->SetLineStyle(2);
  f_gaus_2nd->SetLineWidth(2);
  f_gaus_2nd->Draw("l same");

  double chi2 = f_gaus->GetChisquare();
  double ndf = f_gaus->GetNDF();
  cout << "chi2/ndf = " << chi2/ndf << endl;

  float mean_r = f_gaus->GetParameter(0);
  float sigma_r = f_gaus->GetParameter(1);
  string leg_gaus = Form("R: %1.2f #pm %1.2f (mm)",mean_r,sigma_r);
  plotTopLegend((char*)leg_gaus.c_str(),0.18,0.85,0.04,1,0.0,42,1,1);
  */

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
  f_gaus->SetRange(mean_1st-nSigma*sig_1st,mean_1st+nSigma*sig_1st);
  h_mRadius->Fit(f_gaus,"NR");
  f_gaus->SetLineColor(2);
  f_gaus->SetLineStyle(2);
  f_gaus->SetLineWidth(3);
  f_gaus->Draw("l same");
  float mean_r = f_gaus->GetParameter(1);
  float sigma_r = f_gaus->GetParameter(2);
  string leg_gaus = Form("R: %1.2f #pm %1.2f (mm)",mean_r,sigma_r);
  plotTopLegend((char*)leg_gaus.c_str(),0.18,0.85,0.04,1,0.0,42,1,1);

  double chi2 = f_gaus->GetChisquare();
  double ndf = f_gaus->GetNDF();
  cout << "chi2/ndf = " << chi2/ndf << endl;

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
  plotTopLegend((char*)leg_theta_c.c_str(),0.18,0.80,0.04,1,0.0,42,1,1);

  string leg_sigma = Form("#sigma_{#theta_{c}} = %1.2f (mRad)",1000.0*sigma_theta_c);
  plotTopLegend((char*)leg_sigma.c_str(),0.18,0.75,0.04,1,0.0,42,1,1);

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_meanAR_%s.eps",airgap.c_str());
  c_Radius->SaveAs(fig_name.c_str());
}
