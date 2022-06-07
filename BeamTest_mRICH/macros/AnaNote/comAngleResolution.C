#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "draw.h"

void comAngleResolution(const string airgap = "0mm")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH3D *h_mCherenkovPhotons_HT = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_HT");
  TH1D *h_mRadius_HT = (TH1D*)h_mCherenkovPhotons_HT->Project3D("z");

  TH3D *h_mCherenkovPhotons_MF = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH1D *h_mRadius_MF = (TH1D*)h_mCherenkovPhotons_MF->Project3D("z");

  // float flength = 6.0*25.4 +3.0; // mm
  float flength = 6.0*25.4; // mm

  TCanvas *c_Radius = new TCanvas("c_Radius","c_Radius",10,10,1000,500);
  c_Radius->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_Radius->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Radius->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Radius->cd(i_pad+1)->SetRightMargin(0.15);
    c_Radius->cd(i_pad+1)->SetTicks(1,1);
    c_Radius->cd(i_pad+1)->SetGrid(0,0);
  }

  c_Radius->cd(1);
  h_mRadius_HT->SetTitle("Hough Transformation"); // HT
  h_mRadius_HT->SetLineColor(1);
  h_mRadius_HT->SetLineWidth(1);
  h_mRadius_HT->GetXaxis()->SetTitle("R (mm)");
  h_mRadius_HT->GetXaxis()->CenterTitle();
  h_mRadius_HT->Draw();

  TF1 *f_gaus_HT = new TF1("f_gaus_HT","gaus",0,100);
  f_gaus_HT->SetParameter(0,100);
  f_gaus_HT->SetParameter(1,40.0);
  f_gaus_HT->SetParameter(2,2.0);
  f_gaus_HT->SetRange(35.0,45.0);
  h_mRadius_HT->Fit(f_gaus_HT,"NR");
  float norm_HT_1st = f_gaus_HT->GetParameter(0);
  float mean_HT_1st = f_gaus_HT->GetParameter(1);
  float sig_HT_1st  = f_gaus_HT->GetParameter(2);
  f_gaus_HT->SetParameter(0,norm_HT_1st);
  f_gaus_HT->SetParameter(1,mean_HT_1st);
  f_gaus_HT->SetParameter(2,sig_HT_1st);
  f_gaus_HT->SetRange(mean_HT_1st-3.0*sig_HT_1st,mean_HT_1st+3.0*sig_HT_1st);
  h_mRadius_HT->Fit(f_gaus_HT,"NR");
  f_gaus_HT->SetLineColor(2);
  f_gaus_HT->SetLineStyle(2);
  f_gaus_HT->SetLineWidth(3);
  f_gaus_HT->Draw("l same");
  float mean_HT = f_gaus_HT->GetParameter(1);
  float sig_HT = f_gaus_HT->GetParameter(2);
  string leg_gaus_HT = Form("R: %1.2f #pm %1.2f (mm)",mean_HT,sig_HT);
  plotTopLegend((char*)leg_gaus_HT.c_str(),0.48,0.6,0.04,1,0.0,42,1,1);

  float theta_HT = TMath::ATan2(mean_HT,flength); // theta = atan(r/f)
  float tan_theta_HT = mean_HT/flength; // r/f
  float sigma_theta_HT = sig_HT/(flength*(1.0+tan_theta_HT*tan_theta_HT));
  string leg_theta_HT = Form("#theta: %1.2f #pm %1.4f (Rad)",tan_theta_HT,sigma_theta_HT);
  plotTopLegend((char*)leg_theta_HT.c_str(),0.48,0.5,0.04,1,0.0,42,1,1);

  c_Radius->cd(2);
  h_mRadius_MF->SetTitle("Minuit Fit"); // MF
  h_mRadius_MF->SetLineColor(1);
  h_mRadius_MF->SetLineWidth(1);
  h_mRadius_MF->GetXaxis()->SetTitle("R (mm)");
  h_mRadius_MF->GetXaxis()->CenterTitle();
  h_mRadius_MF->Draw();

  TF1 *f_gaus_MF = new TF1("f_gaus_MF","gaus",0,100);
  f_gaus_MF->SetParameter(0,100);
  f_gaus_MF->SetParameter(1,40.0);
  f_gaus_MF->SetParameter(2,2.0);
  f_gaus_MF->SetRange(35.0,45.0);
  h_mRadius_MF->Fit(f_gaus_MF,"NR");
  float norm_MF_1st = f_gaus_MF->GetParameter(0);
  float mean_MF_1st = f_gaus_MF->GetParameter(1);
  float sig_MF_1st  = f_gaus_MF->GetParameter(2);
  f_gaus_MF->SetParameter(0,norm_MF_1st);
  f_gaus_MF->SetParameter(1,mean_MF_1st);
  f_gaus_MF->SetParameter(2,sig_MF_1st);
  f_gaus_MF->SetRange(mean_MF_1st-3.0*sig_MF_1st,mean_MF_1st+3.0*sig_MF_1st);
  h_mRadius_MF->Fit(f_gaus_MF,"NR");
  f_gaus_MF->SetLineColor(2);
  f_gaus_MF->SetLineStyle(2);
  f_gaus_MF->SetLineWidth(3);
  f_gaus_MF->Draw("l same");
  float mean_MF = f_gaus_MF->GetParameter(1);
  float sig_MF = f_gaus_MF->GetParameter(2);
  string leg_gaus_MF = Form("R: %1.2f #pm %1.2f (mm)",mean_MF,sig_MF);
  plotTopLegend((char*)leg_gaus_MF.c_str(),0.48,0.6,0.04,1,0.0,42,1,1);

  float theta_MF = TMath::ATan2(mean_MF,flength); // theta = atan(r/f)
  float tan_theta_MF = mean_MF/flength; // r/f
  float sigma_theta_MF = sig_MF/(flength*(1.0+tan_theta_MF*tan_theta_MF));
  string leg_theta_MF = Form("#theta: %1.2f #pm %1.4f (Rad)",tan_theta_MF,sigma_theta_MF);
  plotTopLegend((char*)leg_theta_MF.c_str(),0.48,0.5,0.04,1,0.0,42,1,1);
}
