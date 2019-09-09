#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "../../draw.h"

void plotQA_BeamSpot(const string airgap = "8mm_center")
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration_%s.root",airgap.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mBeamSpot= (TH2D*)File_InPut->Get("h_mBeamSpot");

  TCanvas *c_Beam = new TCanvas("c_Beam","c_Beam",10,10,500,500);
  c_Beam->cd()->SetLeftMargin(0.15);
  c_Beam->cd()->SetBottomMargin(0.15);
  c_Beam->cd()->SetRightMargin(0.15);
  c_Beam->cd()->SetTicks(1,1);
  c_Beam->cd()->SetGrid(0,0);

  h_mBeamSpot->SetTitle("120 GeV/c proton");
  h_mBeamSpot->SetLineColor(1);
  h_mBeamSpot->SetLineWidth(1);
  h_mBeamSpot->GetXaxis()->SetRangeUser(-1.0,1.0);
  h_mBeamSpot->GetXaxis()->SetTitle("x (mm)");
  h_mBeamSpot->GetXaxis()->CenterTitle();
  h_mBeamSpot->GetYaxis()->SetRangeUser(-1.0,1.0);
  h_mBeamSpot->GetYaxis()->SetTitle("y (mm)");
  h_mBeamSpot->GetYaxis()->CenterTitle();
  h_mBeamSpot->Draw("colz");

  string fig_name = Form("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/Simulation/c_BeamSpot_%s.eps",airgap.c_str());
  c_Beam->SaveAs(fig_name.c_str());
}
