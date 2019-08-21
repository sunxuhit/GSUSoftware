#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

void plotQA_PMT_AngleRun()
{
  TFile *File_Data = TFile::Open("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/AngleRun/richTDC_run198.root");
  TH2D *h_mRingImage_on = (TH2D*)File_Data->Get("h_mRingImage_on");

  TFile *File_Sim = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root");
  TH2D *h_mPhotonDist = (TH2D*)File_Sim->Get("h_mPhotonDist");

  TCanvas *c_AngleRun = new TCanvas("c_AngleRun","c_AngleRun",10,10,1000,500);
  c_AngleRun->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_AngleRun->cd(i_pad+1)->SetLeftMargin(0.15);
    c_AngleRun->cd(i_pad+1)->SetBottomMargin(0.15);
    c_AngleRun->cd(i_pad+1)->SetRightMargin(0.15);
    c_AngleRun->cd(i_pad+1)->SetTicks(1,1);
    c_AngleRun->cd(i_pad+1)->SetGrid(0,0);
  }

  c_AngleRun->cd(1);
  h_mRingImage_on->SetTitle("Data: run 198"); // data
  h_mRingImage_on->GetXaxis()->SetTitle("Pixel x");
  h_mRingImage_on->GetYaxis()->SetTitle("Pixel y");
  h_mRingImage_on->Draw("colz");

  c_AngleRun->cd(2);
  h_mPhotonDist->SetTitle("Simulation"); // sim
  h_mPhotonDist->GetXaxis()->SetTitle("out_x (mm)");
  h_mPhotonDist->GetYaxis()->SetTitle("out_y (mm)");
  h_mPhotonDist->Draw("colz");

  c_AngleRun->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_AngleRun.eps");
}
