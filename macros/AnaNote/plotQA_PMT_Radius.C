#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "draw.h"

void plotQA_PMT_Radius()
{
  TFile *File_Data = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH3D *h_mNumOfCherenkovPhotons_Data = (TH3D*)File_Data->Get("h_mNumOfCherenkovPhotons");
  TH1D *h_mRadius_Data = (TH1D*)h_mNumOfCherenkovPhotons_Data->Project3D("z");
  TH1D *h_mNumOfPhotons_OnRing_Data = (TH1D*)File_Data->Get("h_mNumOfPhotons");
  float NumOfEvents_Data = h_mNumOfPhotons_OnRing_Data->GetEntries();

  TFile *File_Sim = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root");
  TH3D *h_mNumOfCherenkovPhotons_Sim = (TH3D*)File_Sim->Get("h_mNumOfCherenkovPhotons");
  TH1D *h_mRadius_Sim = (TH1D*)h_mNumOfCherenkovPhotons_Sim->Project3D("z");
  TH1D *h_mNumOfPhotons_OnRing_Sim = (TH1D*)File_Sim->Get("h_mNumOfPhotons");
  float NumOfEvents_Sim = h_mNumOfPhotons_OnRing_Sim->GetEntries();

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
  h_mRadius_Data->SetTitle("Data"); // Data
  h_mRadius_Data->Scale(1.0/NumOfEvents_Data);
  h_mRadius_Data->SetLineColor(1);
  h_mRadius_Data->SetLineWidth(1);
  h_mRadius_Data->GetXaxis()->SetTitle("R (mm)");
  h_mRadius_Data->GetXaxis()->CenterTitle();
  h_mRadius_Data->Draw("h same");
  TF1 *f_gaus_data = new TF1("f_gaus_data","gaus",0,100);
  f_gaus_data->SetParameter(0,100);
  f_gaus_data->SetParameter(1,40.0);
  f_gaus_data->SetParameter(2,2.0);
  f_gaus_data->SetRange(35.0,45.0);
  h_mRadius_Data->Fit(f_gaus_data,"NR");
  float norm_data_1st = f_gaus_data->GetParameter(0);
  float mean_data_1st = f_gaus_data->GetParameter(1);
  float sig_data_1st  = f_gaus_data->GetParameter(2);
  f_gaus_data->SetParameter(0,norm_data_1st);
  f_gaus_data->SetParameter(1,mean_data_1st);
  f_gaus_data->SetParameter(2,sig_data_1st);
  f_gaus_data->SetRange(mean_data_1st-sig_data_1st,mean_data_1st+sig_data_1st);
  h_mRadius_Data->Fit(f_gaus_data,"NR");
  f_gaus_data->SetLineColor(2);
  f_gaus_data->SetLineStyle(2);
  f_gaus_data->SetLineWidth(3);
  f_gaus_data->Draw("l same");
  float mean_data = f_gaus_data->GetParameter(1);
  float sig_data = f_gaus_data->GetParameter(2);
  string leg_gaus_data = Form("R: %1.2f #pm %1.2f (mm)",mean_data,sig_data);
  plotTopLegend((char*)leg_gaus_data.c_str(),0.48,0.6,0.04,1,0.0,42,1,1);

  float theta_data = TMath::ATan2(mean_data,flength); // theta = atan(r/f)
  float tan_theta_data = mean_data/flength; // r/f
  float sigma_theta_data = sig_data/(flength*(1.0+tan_theta_data*tan_theta_data));
  string leg_theta_data = Form("#theta: %1.2f #pm %1.4f (Rad)",tan_theta_data,sigma_theta_data);
  plotTopLegend((char*)leg_theta_data.c_str(),0.48,0.5,0.04,1,0.0,42,1,1);

  c_Radius->cd(2);
  h_mRadius_Sim->SetTitle("Simulation"); // Simulation
  h_mRadius_Sim->Scale(1.0/NumOfEvents_Sim);
  h_mRadius_Sim->SetLineColor(1);
  h_mRadius_Sim->SetLineWidth(1);
  h_mRadius_Sim->GetXaxis()->SetTitle("R (mm)");
  h_mRadius_Sim->GetXaxis()->CenterTitle();
  h_mRadius_Sim->Draw("h same");
  TF1 *f_gaus_sim = new TF1("f_gaus_sim","gaus",0,100);
  f_gaus_sim->SetParameter(0,100);
  f_gaus_sim->SetParameter(1,40.0);
  f_gaus_sim->SetParameter(2,2.0);
  f_gaus_sim->SetRange(35.0,45.0);
  h_mRadius_Sim->Fit(f_gaus_sim,"NR");
  float norm_sim_1st = f_gaus_sim->GetParameter(0);
  float mean_sim_1st = f_gaus_sim->GetParameter(1);
  float sig_sim_1st  = f_gaus_sim->GetParameter(2);
  f_gaus_sim->SetParameter(0,norm_sim_1st);
  f_gaus_sim->SetParameter(1,mean_sim_1st);
  f_gaus_sim->SetParameter(2,sig_sim_1st);
  f_gaus_sim->SetRange(mean_sim_1st-2.0*sig_sim_1st,mean_sim_1st+2.0*sig_sim_1st);
  h_mRadius_Sim->Fit(f_gaus_sim,"NR");
  f_gaus_sim->SetLineColor(2);
  f_gaus_sim->SetLineStyle(2);
  f_gaus_sim->SetLineWidth(3);
  f_gaus_sim->Draw("l same");
  float mean_sim = f_gaus_sim->GetParameter(1);
  float sig_sim = f_gaus_sim->GetParameter(2);
  string leg_gaus_sim = Form("R: %1.2f #pm %1.2f (mm)",mean_sim,sig_sim);
  plotTopLegend((char*)leg_gaus_sim.c_str(),0.48,0.6,0.04,1,0.0,42,1,1);

  float theta_sim = TMath::ATan2(mean_sim,flength); // theta = atan(r/f)
  float tan_theta_sim = mean_sim/flength; // r/f
  float sigma_theta_sim = sig_sim/(flength*(1.0+tan_theta_sim*tan_theta_sim));
  string leg_theta_sim = Form("#theta: %1.2f #pm %1.4f (Rad)",tan_theta_sim,sigma_theta_sim);
  plotTopLegend((char*)leg_theta_sim.c_str(),0.48,0.5,0.04,1,0.0,42,1,1);

  c_Radius->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_Radius.eps");
}

