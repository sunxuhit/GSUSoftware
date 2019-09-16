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
  // double sigma = p0/TMath::Sqrt(x) + p1;
  double sigma = TMath::Sqrt(p0*p0/x + p1*p1);

  return sigma;
}

void extractSingleAngleResolution(const string airgap = "3mm_center")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH3D *h_mCherenkovPhotons = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH2D *h_mNumRadius = (TH2D*)h_mCherenkovPhotons->Project3D("zx"); // x-axis num of photons on ring & y-axis ring radius

  TH3D *h_mSinglePhoton = (TH3D*)File_InPut->Get("h_mSinglePhoton");
  TH1D *h_mTheta_air = (TH1D*)h_mSinglePhoton->Project3D("x")->Clone("h_mTheta_air");;
  TH1D *h_mTheta_c = (TH1D*)h_mSinglePhoton->Project3D("y")->Clone("h_mTheta_c");;
  TH1D *h_mRadius_SinglePhoton = (TH1D*)h_mSinglePhoton->Project3D("z")->Clone("h_mRadius_SinglePhoton");;

  float nref = 1.03;
  // float flength = 6.0*25.4 +8.0; // mm
  // float flength = 6.0*25.4 +3.0; // mm
  float flength = 6.0*25.4; // mm
  float nSigma = 3.0;

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
    h_mRadius[i_photon]->GetXaxis()->SetRangeUser(29,59);
    h_mRadius[i_photon]->GetXaxis()->SetTitle("R (mm)");
    h_mRadius[i_photon]->GetXaxis()->CenterTitle();
    h_mRadius[i_photon]->DrawCopy("hE");

    string FuncName = Form("f_gaus_%d",i_photon);
    f_gaus[i_photon] = new TF1(FuncName.c_str(),"gaus",0,100);
    f_gaus[i_photon]->SetParameter(0,100);
    f_gaus[i_photon]->SetParameter(1,40.0);
    f_gaus[i_photon]->SetParameter(2,2.0);
    f_gaus[i_photon]->SetRange(29.0,59.0);
    if(h_mRadius[i_photon]->GetEntries() > 50.0)
    {
      h_mRadius[i_photon]->Fit(f_gaus[i_photon],"NQR");
      float norm_1st = f_gaus[i_photon]->GetParameter(0);
      float mean_1st = f_gaus[i_photon]->GetParameter(1);
      float sig_1st  = f_gaus[i_photon]->GetParameter(2);
      f_gaus[i_photon]->SetParameter(0,norm_1st);
      f_gaus[i_photon]->SetParameter(1,mean_1st);
      f_gaus[i_photon]->SetParameter(2,sig_1st);
      f_gaus[i_photon]->SetRange(mean_1st-nSigma*sig_1st,mean_1st+nSigma*sig_1st);
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

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_radiusSlice_%s.eps",airgap.c_str());
  c_Radius->SaveAs(fig_name.c_str());

  TCanvas *c_AR_Npe= new TCanvas("c_AR_Npe","c_AR_Npe",10,10,800,800);
  c_AR_Npe->cd()->SetLeftMargin(0.15);
  c_AR_Npe->cd()->SetBottomMargin(0.15);
  c_AR_Npe->cd()->SetRightMargin(0.15);
  c_AR_Npe->cd()->SetTicks(1,1);
  c_AR_Npe->cd()->SetGrid(0,0);

  TH1D *h_frame = new TH1D("h_frame","h_frame",50,-0.5,49.5);
  for(int i_bin = 0; i_bin < 50; ++i_bin)
  {
    h_frame->SetBinContent(i_bin+1,-10.0);
    h_frame->SetBinError(i_bin+1,0.1);
  }
  h_frame->SetTitle("#sigma_{#theta_{c}} vs. N_{pe}");
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
  float sigma_glob = 1000.0*f_sigma->GetParameter(1);

  string leg = Form("#sigma_{#theta_{c},1pe} = %1.1f mRad",sigma_pe);
  plotTopLegend((char*)leg.c_str(),0.28,0.8,0.06,1,0.0,42,1,1);

  string leg_glob = Form("#sigma_{Glob} = %1.1f mRad",sigma_glob);
  plotTopLegend((char*)leg_glob.c_str(),0.28,0.7,0.06,1,0.0,42,1,1);

  fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_singleAR_Npe_%s.eps",airgap.c_str());
  c_AR_Npe->SaveAs(fig_name.c_str());

  TCanvas *c_AR_SP= new TCanvas("c_AR_SP","c_AR_SP",10,10,800,400);
  c_AR_SP->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_AR_SP->cd(i_pad+1)->SetLeftMargin(0.15);
    c_AR_SP->cd(i_pad+1)->SetBottomMargin(0.15);
    c_AR_SP->cd(i_pad+1)->SetRightMargin(0.15);
    c_AR_SP->cd(i_pad+1)->SetTicks(1,1);
    c_AR_SP->cd(i_pad+1)->SetGrid(0,0);
  }

  c_AR_SP->cd(1);
  h_mRadius_SinglePhoton->SetTitle("Single Photon Radius");
  h_mRadius_SinglePhoton->GetXaxis()->SetTitle("R (mm)");
  h_mRadius_SinglePhoton->GetXaxis()->CenterTitle();
  h_mRadius_SinglePhoton->GetXaxis()->SetTitleSize(0.06);
  h_mRadius_SinglePhoton->GetXaxis()->SetTitleOffset(0.9);
  h_mRadius_SinglePhoton->GetXaxis()->SetLabelSize(0.04);
  h_mRadius_SinglePhoton->GetXaxis()->SetRangeUser(29.0,59.0);
  h_mRadius_SinglePhoton->SetNdivisions(505,"X");
  h_mRadius_SinglePhoton->GetYaxis()->SetRangeUser(0.0,h_mRadius_SinglePhoton->GetMaximum()*1.3);
  h_mRadius_SinglePhoton->Draw("hE");

  TF1 *f_gaus_rs = new TF1("f_gaus_rs","gaus",0,100);
  f_gaus_rs->SetParameter(0,100);
  f_gaus_rs->SetParameter(1,40.0);
  f_gaus_rs->SetParameter(2,2.0);
  f_gaus_rs->SetRange(29.0,49.0);
  h_mRadius_SinglePhoton->Fit(f_gaus_rs,"NQR");

  float norm_rs_1st = f_gaus_rs->GetParameter(0);
  float mean_rs_1st = f_gaus_rs->GetParameter(1);
  float sig_rs_1st  = f_gaus_rs->GetParameter(2);
  f_gaus_rs->SetParameter(0,norm_rs_1st);
  f_gaus_rs->SetParameter(1,mean_rs_1st);
  f_gaus_rs->SetParameter(2,sig_rs_1st);
  f_gaus_rs->SetRange(mean_rs_1st-nSigma*sig_rs_1st,mean_rs_1st+nSigma*sig_rs_1st);
  h_mRadius_SinglePhoton->Fit(f_gaus_rs,"NQR");
  f_gaus_rs->SetLineColor(2);
  f_gaus_rs->SetLineStyle(2);
  f_gaus_rs->SetLineWidth(3);
  f_gaus_rs->Draw("l same");

  float mean_rs = f_gaus_rs->GetParameter(1);
  float sigma_rs = f_gaus_rs->GetParameter(2);
  float error_rs = f_gaus_rs->GetParError(2);

  float dist_rs = TMath::Sqrt(mean_rs*mean_rs+flength*flength);
  float sin_theta_rs = mean_rs/dist_rs; // sin(theta_c)*n_c = sin(theta_air)*n_air
  float theta_rs = TMath::ASin(sin_theta_rs);

  float sin_theta_c_rs = mean_rs/(dist_rs*nref); // sin(theta_c)*n_c = sin(theta_air)*n_air
  float theta_c_rs = TMath::ASin(sin_theta_c_rs);
  float sigma_theta_c_rs = TMath::Cos(theta_rs)*TMath::Cos(theta_rs)*TMath::Cos(theta_rs)*sigma_rs/(nref*flength*TMath::Cos(theta_c_rs));

  string leg_gaus_rs = Form("R: %1.2f #pm %1.2f (mm)",mean_rs,sigma_rs);
  plotTopLegend((char*)leg_gaus_rs.c_str(),0.18,0.85,0.04,1,0.0,42,1,1);

  string leg_theta_rs = Form("#theta_{c}: %1.2f #pm %1.2f (mRad)",theta_c_rs*1000.0,sigma_theta_c_rs*1000.0);
  plotTopLegend((char*)leg_theta_rs.c_str(),0.18,0.80,0.04,1,0.0,42,1,1);

  string leg_sigma_rs = Form("#sigma_{#theta_{c},1pe} = %1.2f mRad",1000.0*sigma_theta_c_rs);
  plotTopLegend((char*)leg_sigma_rs.c_str(),0.2,0.75,0.04,1,0.0,42,1,1);


  /*
  c_AngleResolution->cd(3);
  h_mTheta_air->SetTitle("Single Photon Angle in Air");
  h_mTheta_air->GetXaxis()->SetTitle("#theta_{air}");
  h_mTheta_air->GetXaxis()->CenterTitle();
  h_mTheta_air->GetXaxis()->SetTitleSize(0.06);
  h_mTheta_air->GetXaxis()->SetTitleOffset(0.9);
  h_mTheta_air->GetXaxis()->SetLabelSize(0.04);
  h_mTheta_air->GetXaxis()->SetRangeUser(0.1,0.4);
  h_mTheta_air->SetNdivisions(505,"X");
  h_mTheta_air->GetYaxis()->SetRangeUser(0.0,h_mTheta_air->GetMaximum()*1.3);
  h_mTheta_air->Draw("hE");

  TF1 *f_gaus_ta = new TF1("f_gaus_ta","gaus",0,1);
  f_gaus_ta->SetParameter(0,100);
  f_gaus_ta->SetParameter(1,0.24);
  f_gaus_ta->SetParameter(2,0.01);
  f_gaus_ta->SetRange(0.2,0.3);
  h_mTheta_air->Fit(f_gaus_ta,"NQR");

  float norm_ta_1st = f_gaus_ta->GetParameter(0);
  float mean_ta_1st = f_gaus_ta->GetParameter(1);
  float sig_ta_1st  = f_gaus_ta->GetParameter(2);
  f_gaus_ta->SetParameter(0,norm_ta_1st);
  f_gaus_ta->SetParameter(1,mean_ta_1st);
  f_gaus_ta->SetParameter(2,sig_ta_1st);
  f_gaus_ta->SetRange(mean_ta_1st-nSigma*sig_ta_1st,mean_ta_1st+nSigma*sig_ta_1st);
  h_mTheta_air->Fit(f_gaus_ta,"NQR");
  f_gaus_ta->SetLineColor(2);
  f_gaus_ta->SetLineStyle(2);
  f_gaus_ta->SetLineWidth(3);
  f_gaus_ta->Draw("l same");

  float mean_ta = f_gaus_ta->GetParameter(1);
  float sigma_ta = f_gaus_ta->GetParameter(2);
  float error_ta = f_gaus_ta->GetParError(2);

  float sin_theta_c_ta = TMath::Sin(mean_ta)/nref; // sin(theta_c)*n_c = sin(theta_air)*n_air
  float theta_c_ta = TMath::ASin(sin_theta_c_ta);
  float sigma_theta_c_ta = TMath::Cos(mean_ta)*sigma_ta/(nref*TMath::Cos(theta_c_ta));

  string leg_sigma_ta = Form("#sigma_{#theta_{c},1pe} = %1.2f mRad",1000.0*sigma_theta_c_ta);
  plotTopLegend((char*)leg_sigma_ta.c_str(),0.2,0.8,0.06,1,0.0,42,1,1);
  */

  c_AR_SP->cd(2);
  h_mTheta_c->SetTitle("Single Photon Cherenkov Angle");
  h_mTheta_c->GetXaxis()->SetTitle("#theta_{c}");
  h_mTheta_c->GetXaxis()->CenterTitle();
  h_mTheta_c->GetXaxis()->SetTitleSize(0.06);
  h_mTheta_c->GetXaxis()->SetTitleOffset(0.9);
  h_mTheta_c->GetXaxis()->SetLabelSize(0.04);
  h_mTheta_c->GetXaxis()->SetRangeUser(0.1,0.4);
  h_mTheta_c->SetNdivisions(505,"X");
  h_mTheta_c->GetYaxis()->SetRangeUser(0.0,h_mTheta_c->GetMaximum()*1.3);
  h_mTheta_c->Draw("hE");

  TF1 *f_gaus_tc = new TF1("f_gaus_tc","gaus",0,1);
  f_gaus_tc->SetParameter(0,100);
  f_gaus_tc->SetParameter(1,0.24);
  f_gaus_tc->SetParameter(2,0.01);
  f_gaus_tc->SetRange(0.2,0.3);
  h_mTheta_c->Fit(f_gaus_tc,"NQR");

  float norm_tc_1st = f_gaus_tc->GetParameter(0);
  float mean_tc_1st = f_gaus_tc->GetParameter(1);
  float sig_tc_1st  = f_gaus_tc->GetParameter(2);
  f_gaus_tc->SetParameter(0,norm_tc_1st);
  f_gaus_tc->SetParameter(1,mean_tc_1st);
  f_gaus_tc->SetParameter(2,sig_tc_1st);
  f_gaus_tc->SetRange(mean_tc_1st-nSigma*sig_tc_1st,mean_tc_1st+nSigma*sig_tc_1st);
  h_mTheta_c->Fit(f_gaus_tc,"NQR");
  f_gaus_tc->SetLineColor(2);
  f_gaus_tc->SetLineStyle(2);
  f_gaus_tc->SetLineWidth(3);
  f_gaus_tc->Draw("l same");

  float mean_tc = f_gaus_tc->GetParameter(1);
  float sigma_tc = f_gaus_tc->GetParameter(2);
  float error_tc = f_gaus_tc->GetParError(2);

  string leg_sigma_tc = Form("#sigma_{#theta_{c},1pe} = %1.2f mRad",1000.0*sigma_tc);
  plotTopLegend((char*)leg_sigma_tc.c_str(),0.2,0.80,0.06,1,0.0,42,1,1);

  fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_singleAR_SP_%s.eps",airgap.c_str());
  c_AR_SP->SaveAs(fig_name.c_str());
}

