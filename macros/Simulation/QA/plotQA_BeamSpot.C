#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "../../draw.h"

void plotQA_BeamSpot(const string airgap = "3mm")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mBeamSpot= (TH2D*)File_InPut->Get("h_mBeamSpot");
  TH2D *h_mPhotonDist = (TH2D*)File_InPut->Get("h_mPhotonDist");

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
  h_mBeamSpot->SetTitle("120 GeV/c proton");
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
  c_Beam->cd(2)->SetLogz();
  h_mPhotonDist->SetTitle("120 GeV/c proton");
  h_mPhotonDist->SetStats(0);
  h_mPhotonDist->SetLineColor(1);
  h_mPhotonDist->SetLineWidth(1);
  h_mPhotonDist->GetXaxis()->SetTitle("x (mm)");
  h_mPhotonDist->GetXaxis()->CenterTitle();
  h_mPhotonDist->GetYaxis()->SetTitle("y (mm)");
  h_mPhotonDist->GetYaxis()->CenterTitle();
  h_mPhotonDist->Draw("colz");

  h_mBeamSpot->SetTitle("120 GeV/c proton");
  h_mBeamSpot->SetStats(0);
  h_mBeamSpot->SetLineColor(1);
  h_mBeamSpot->SetLineWidth(1);
  h_mBeamSpot->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpot->GetXaxis()->CenterTitle();
  h_mBeamSpot->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpot->GetYaxis()->CenterTitle();
  h_mBeamSpot->Draw("col same");

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_BeamSpot_%s.eps",airgap.c_str());
  c_Beam->SaveAs(fig_name.c_str());
}
