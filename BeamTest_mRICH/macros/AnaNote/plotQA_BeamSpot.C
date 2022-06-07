#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "draw.h"

void plotQA_BeamSpot(const string airgap = "8mm")
{
  string input_sim = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_Sim = TFile::Open(input_sim.c_str());
  TH2D *h_mBeamSpotReco_Sim = (TH2D*)File_Sim->Get("h_mBeamSpotReco");

  string input_data = "/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root";
  TFile *File_Data = TFile::Open(input_data.c_str());
  TH2D *h_mBeamSpotReco_Data = (TH2D*)File_Data->Get("h_mBeamSpotReco");

  TCanvas *c_Beam = new TCanvas("c_Beam","c_Beam",10,10,1000,500);
  c_Beam->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_Beam->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Beam->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Beam->cd(i_pad+1)->SetRightMargin(0.15);
    c_Beam->cd(i_pad+1)->SetTicks(1,1);
    c_Beam->cd(i_pad+1)->SetGrid(0,0);
  }

  c_Beam->cd(1);
  h_mBeamSpotReco_Data->SetTitle("120 GeV/c proton data");
  h_mBeamSpotReco_Data->SetLineColor(1);
  h_mBeamSpotReco_Data->SetLineWidth(1);
  h_mBeamSpotReco_Data->GetXaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco_Data->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpotReco_Data->GetXaxis()->CenterTitle();
  h_mBeamSpotReco_Data->GetYaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco_Data->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpotReco_Data->GetYaxis()->CenterTitle();
  h_mBeamSpotReco_Data->Draw("colz");


  c_Beam->cd(2);
  h_mBeamSpotReco_Sim->SetTitle("120 GeV/c proton sim");
  h_mBeamSpotReco_Sim->SetLineColor(1);
  h_mBeamSpotReco_Sim->SetLineWidth(1);
  h_mBeamSpotReco_Sim->GetXaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco_Sim->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpotReco_Sim->GetXaxis()->CenterTitle();
  h_mBeamSpotReco_Sim->GetYaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco_Sim->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpotReco_Sim->GetYaxis()->CenterTitle();
  h_mBeamSpotReco_Sim->Draw("colz");

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_BeamSpotReco_%s.eps",airgap.c_str());
  c_Beam->SaveAs(fig_name.c_str());
}
