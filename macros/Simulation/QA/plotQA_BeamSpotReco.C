#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "../../draw.h"

void plotQA_BeamSpotReco(const string airgap = "8mm")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mBeamSpot= (TH2D*)File_InPut->Get("h_mBeamSpot");
  TH2D *h_mBeamSpotReco = (TH2D*)File_InPut->Get("h_mBeamSpotReco");

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
  h_mBeamSpot->SetTitle("input: 120 GeV/c proton");
  h_mBeamSpot->SetStats(0);
  h_mBeamSpot->SetLineColor(1);
  h_mBeamSpot->SetLineWidth(1);
  h_mBeamSpot->GetXaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpot->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpot->GetXaxis()->CenterTitle();
  h_mBeamSpot->GetYaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpot->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpot->GetYaxis()->CenterTitle();
  h_mBeamSpot->Draw("colz");

  c_Beam->cd(2);
  h_mBeamSpotReco->SetTitle("reco: 120 GeV/c proton");
  h_mBeamSpotReco->SetStats(0);
  h_mBeamSpotReco->SetLineColor(1);
  h_mBeamSpotReco->SetLineWidth(1);
  h_mBeamSpotReco->GetXaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpotReco->GetXaxis()->CenterTitle();
  h_mBeamSpotReco->GetYaxis()->SetRangeUser(-10.0,10.0);
  h_mBeamSpotReco->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpotReco->GetYaxis()->CenterTitle();
  h_mBeamSpotReco->Draw("colz");

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_BeamSpotReco_%s.eps",airgap.c_str());
  c_Beam->SaveAs(fig_name.c_str());
}
