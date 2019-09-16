#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

void plotQA_PMT_Radius()
{
  TFile *File_InPut = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH3D *h_mCherenkovPhotons_MF = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH1D *h_mPhotons = (TH1D*)h_mCherenkovPhotons_MF->Project3D("x");
  TH1D *h_mRadius = (TH1D*)h_mCherenkovPhotons_MF->Project3D("z");

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
  h_mRadius->SetTitle("Ring Radius");
  // h_mRadius->SetStats(0);
  h_mRadius->GetXaxis()->SetTitle("R (mm)");
  h_mRadius->GetXaxis()->CenterTitle();
  h_mRadius->GetXaxis()->SetRangeUser(30.0,50.0);
  h_mRadius->Draw();

  c_Radius->cd(2);
  h_mPhotons->SetTitle("Num. Of Photons On Ring");
  // h_mPhotons->SetStats(0);
  h_mPhotons->GetXaxis()->SetTitle("N_{pe}");
  h_mPhotons->GetXaxis()->CenterTitle();
  h_mPhotons->GetXaxis()->SetRangeUser(0.0,40.0);
  h_mPhotons->Draw();

  c_Radius->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_Radius_TB.eps");
}

