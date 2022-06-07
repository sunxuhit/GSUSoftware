#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

void plotQA_PMT_NumOfPhotons()
{
  TFile *File_InPut = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH3D *h_mCherenkovPhotons_MF = (TH3D*)File_InPut->Get("h_mCherenkovPhotons_MF");
  TH1D *h_mPhotons = (TH1D*)h_mCherenkovPhotons_MF->Project3D("x");

  TCanvas *c_NumOfPhotons = new TCanvas("c_NumOfPhotons","c_NumOfPhotons",10,10,500,500);
  c_NumOfPhotons->cd()->SetLeftMargin(0.15);
  c_NumOfPhotons->cd()->SetBottomMargin(0.15);
  c_NumOfPhotons->cd()->SetRightMargin(0.15);
  c_NumOfPhotons->cd()->SetTicks(1,1);
  c_NumOfPhotons->cd()->SetGrid(0,0);

  h_mPhotons->SetTitle("Num. Of Photons On Ring");
  // h_mPhotons->SetStats(0);
  h_mPhotons->GetXaxis()->SetTitle("N_{pe}");
  h_mPhotons->GetXaxis()->CenterTitle();
  h_mPhotons->GetXaxis()->SetRangeUser(0.0,40.0);
  h_mPhotons->Draw();

  c_NumOfPhotons->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/BeamTest/c_NumOfPhotons_TB.eps");
}

