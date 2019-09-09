#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "../../draw.h"

double singleAR(double *x_val, double *par)
{
  double x = x_val[0];
  double p0 = par[0];
  double p1 = par[1];
  double sigma = p0/TMath::Sqrt(x) + p1;
  // double sigma = TMath::Sqrt(p0*p0/x + p1*p1);

  return sigma;
}

void extractSingleAngleResolution()
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH3D *h_mCherenkovPhotons = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH2D *h_mNumRadius = (TH2D*)h_mCherenkovPhotons->Project3D("zx"); // x-axis num of photons on ring & y-axis ring radius

  float nref = 1.03;
  // float flength = 6.0*25.4 +8.0; // mm
  // float flength = 6.0*25.4 +3.0; // mm
  float flength = 6.0*25.4; // mm

  TCanvas *c_Radius = new TCanvas("c_Radius","c_Radius",2000,1500);
  c_Radius->Divide(5,4);
  TH1D *h_mRadius[20];
  TF1 *f_gaus[20];
  float sigma_theta[20];
  float error_theta[20];
  TGraphAsymmErrors *g_AngleResolution = new TGraphAsymmErrors();
  int nPoints = 0;
  for(int i_photon = 0; i_photon < 20; ++i_photon)
  {
    sigma_theta[i_photon] = 0.0;
    error_theta[i_photon] = 0.0;

    c_Radius->cd(i_photon+1);
    string HistName = Form("h_mRadius_%d",i_photon);
    h_mRadius[i_photon] = (TH1D*)h_mNumRadius->ProjectionY(HistName.c_str(),i_photon+1,i_photon+1);
    h_mRadius[i_photon]->GetXaxis()->SetRangeUser(35,49);
    h_mRadius[i_photon]->GetXaxis()->SetTitle("R (mm)");
    h_mRadius[i_photon]->GetXaxis()->CenterTitle();
    h_mRadius[i_photon]->DrawCopy("hE");

    string FuncName = Form("f_gaus_%d",i_photon);
    f_gaus[i_photon] = new TF1(FuncName.c_str(),"gaus",0,100);
    f_gaus[i_photon]->SetParameter(0,100);
    f_gaus[i_photon]->SetParameter(1,40.0);
    f_gaus[i_photon]->SetParameter(2,2.0);
    f_gaus[i_photon]->SetRange(35.0,45.0);
    if(h_mRadius[i_photon]->GetEntries() > 50.0)
    {
      h_mRadius[i_photon]->Fit(f_gaus[i_photon],"NQR");
      float norm_1st = f_gaus[i_photon]->GetParameter(0);
      float mean_1st = f_gaus[i_photon]->GetParameter(1);
      float sig_1st  = f_gaus[i_photon]->GetParameter(2);
      f_gaus[i_photon]->SetParameter(0,norm_1st);
      f_gaus[i_photon]->SetParameter(1,mean_1st);
      f_gaus[i_photon]->SetParameter(2,sig_1st);
      f_gaus[i_photon]->SetRange(mean_1st-3.0*sig_1st,mean_1st+3.0*sig_1st);
      h_mRadius[i_photon]->Fit(f_gaus[i_photon],"NQR");
      f_gaus[i_photon]->SetLineColor(2);
      f_gaus[i_photon]->SetLineStyle(2);
      f_gaus[i_photon]->SetLineWidth(3);
      f_gaus[i_photon]->Draw("l same");

      float mean_r = f_gaus[i_photon]->GetParameter(1);
      float sigma_r = f_gaus[i_photon]->GetParameter(2);
      float error_r = f_gaus[i_photon]->GetParError(2);

      /*
      float theta = TMath::ATan2(mean_r,flength); // theta = atan(r/f)
      float tan_theta = mean_r/flength; // r/f
      sigma_theta[i_photon] = sigma_r/(flength*(1.0+tan_theta*tan_theta));
      error_theta[i_photon] = error_r/(flength*(1.0+tan_theta*tan_theta));
      // cout << "i_photon = " << i_photon << ", mean_r = " << mean_r << ", sigma_r = " << sigma_r << endl;
      // cout << "i_photon = " << i_photon << ", sigma_theta = " << sigma_theta[i_photon] << " +/- " << error_theta[i_photon]<< endl;
      */

      float dist_r = TMath::Sqrt(mean_r*mean_r+flength*flength);
      float sin_theta = mean_r/dist_r; // sin(theta_c)*n_c = sin(theta_air)*n_air
      float theta = TMath::ASin(sin_theta);

      float sin_theta_c = mean_r/(dist_r*nref); // sin(theta_c)*n_c = sin(theta_air)*n_air
      float theta_c = TMath::ASin(sin_theta_c);
      float sigma_theta_c = TMath::Cos(theta)*TMath::Cos(theta)*TMath::Cos(theta)*sigma_r/(nref*flength*TMath::Cos(theta_c));
      sigma_theta[i_photon] = sigma_theta_c;
      error_theta[i_photon] = error_r*TMath::Cos(theta)*TMath::Cos(theta)*TMath::Cos(theta)/(nref*flength*TMath::Cos(theta_c));

      string leg_Npe = Form("N_{p.e.} = %d",i_photon);
      plotTopLegend((char*)leg_Npe.c_str(),0.5,0.75,0.06,1,0.0,42,1,1);

      string leg_sigma = Form("#sigma_{#theta_{c}} = %1.2f (mRad)",1000.0*sigma_theta_c);
      plotTopLegend((char*)leg_sigma.c_str(),0.5,0.65,0.06,1,0.0,42,1,1);

      g_AngleResolution->SetPoint(nPoints,i_photon,sigma_theta[i_photon]);
      g_AngleResolution->SetPointError(nPoints,0.0,0.0,error_theta[i_photon],error_theta[i_photon]);
      nPoints++;
    }
    // cout << "i_photon = " << i_photon << ", h_mNumRadius->GetBinX() = " << h_mNumRadius->GetXaxis()->GetBinCenter(i_photon+1) << endl;
  }

  TCanvas *c_AngleResolution = new TCanvas("c_AngleResolution","c_AngleResolution",10,10,500,500);
  c_AngleResolution->cd()->SetLeftMargin(0.15);
  c_AngleResolution->cd()->SetBottomMargin(0.15);
  c_AngleResolution->cd()->SetRightMargin(0.15);
  c_AngleResolution->cd()->SetTicks(1,1);
  c_AngleResolution->cd()->SetGrid(0,0);

  TH1D *h_frame = new TH1D("h_frame","h_frame",50,-0.5,49.5);
  for(int i_bin = 0; i_bin < 50; ++i_bin)
  {
    h_frame->SetBinContent(i_bin+1,-10.0);
    h_frame->SetBinError(i_bin+1,0.1);
  }
  h_frame->SetTitle("");
  h_frame->SetStats(0);
  h_frame->GetXaxis()->SetTitle("N_{pe}");
  h_frame->GetXaxis()->CenterTitle();
  h_frame->GetXaxis()->SetTitleSize(0.06);
  h_frame->GetXaxis()->SetTitleOffset(0.9);
  h_frame->GetXaxis()->SetLabelSize(0.04);
  h_frame->GetXaxis()->SetRangeUser(4,20);
  h_frame->SetNdivisions(505,"X");

  h_frame->GetYaxis()->SetTitle("#sigma_{#theta_{c}}");
  h_frame->GetYaxis()->CenterTitle();
  h_frame->GetYaxis()->SetTitleOffset(1.14);
  h_frame->GetYaxis()->SetTitleSize(0.06);
  h_frame->GetYaxis()->SetLabelSize(0.04);
  h_frame->GetYaxis()->SetRangeUser(0.0,0.02);
  h_frame->SetNdivisions(505,"Y");
  h_frame->DrawCopy("PE");

  g_AngleResolution->SetMarkerColor(2);
  g_AngleResolution->SetMarkerSize(1.4);
  g_AngleResolution->SetMarkerStyle(24);
  g_AngleResolution->Draw("pE same");

  TF1 *f_sigma = new TF1("f_sigma",singleAR,0,50,2);
  f_sigma->SetParameter(0,0.002);
  f_sigma->SetParameter(1,0.01);
  f_sigma->SetRange(4,17);
  g_AngleResolution->Fit(f_sigma,"NR");
  f_sigma->SetLineColor(2);
  f_sigma->SetLineWidth(2);
  f_sigma->SetLineStyle(2);
  f_sigma->Draw("l same");

  float sigma_pe = 1000.0*f_sigma->GetParameter(0);

  string leg = Form("#sigma_{#theta_{c},1pe} = %1.1f mRad",sigma_pe);
  plotTopLegend((char*)leg.c_str(),0.48,0.5,0.04,1,0.0,42,1,1);

  string fig_name = "/Users/xusun/WorkSpace/EICPID/figures/AnaNote/BeamTest/c_singleAR.eps";
  c_AngleResolution->SaveAs(fig_name.c_str());

  fig_name = "/Users/xusun/WorkSpace/EICPID/figures/AnaNote/BeamTest/c_RadiusSlice.eps";
  c_Radius->SaveAs(fig_name.c_str());
}

