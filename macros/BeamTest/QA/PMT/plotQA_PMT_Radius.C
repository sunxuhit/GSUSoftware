#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

void plotQA_PMT_Radius()
{
  TFile *File_InPut = TFile::Open("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH3D *h_mNumOfCherenkovPhotons = (TH3D*)File_InPut->Get("h_mNumOfCherenkovPhotons");
  TH2D *h_mPhotonsRadius = (TH2D*)h_mNumOfCherenkovPhotons->Project3D("zy");
  TH1D *h_mRadius = (TH1D*)h_mNumOfCherenkovPhotons->Project3D("z");

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
  h_mPhotonsRadius->Draw("colz");

  c_Radius->cd(2);
  h_mRadius->Draw();
  TF1 *f_gaus = new TF1("f_gaus","gaus",0,100);
  f_gaus->SetParameter(0,100);
  f_gaus->SetParameter(1,40.0);
  f_gaus->SetParameter(2,2.0);
  f_gaus->SetRange(35.0,45.0);
  h_mRadius->Fit(f_gaus,"NR");
  f_gaus->SetLineColor(2);
  f_gaus->SetLineWidth(2);
  f_gaus->SetLineStyle(2);
  f_gaus->Draw("l same");

  c_Radius->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_Radius.eps");
}

