#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

void plotQA_PMT_NumOfPhotons()
{
  TFile *File_InPut = TFile::Open("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH1D *h_mNumOfPhotons_OnRing = (TH1D*)File_InPut->Get("h_mNumOfPhotons_OnRing");
  TH1D *h_mNumOfPhotons_OffRing = (TH1D*)File_InPut->Get("h_mNumOfPhotons_OffRing");

  TCanvas *c_NumOfPhotons = new TCanvas("c_NumOfPhotons","c_NumOfPhotons",10,10,1000,500);
  c_NumOfPhotons->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_NumOfPhotons->cd(i_pad+1)->SetLeftMargin(0.15);
    c_NumOfPhotons->cd(i_pad+1)->SetBottomMargin(0.15);
    c_NumOfPhotons->cd(i_pad+1)->SetRightMargin(0.15);
    c_NumOfPhotons->cd(i_pad+1)->SetTicks(1,1);
    c_NumOfPhotons->cd(i_pad+1)->SetGrid(0,0);
  }

  c_NumOfPhotons->cd(1);
  h_mNumOfPhotons_OnRing->Draw();

  c_NumOfPhotons->cd(2);
  h_mNumOfPhotons_OffRing->Draw();

  c_NumOfPhotons->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_NumOfPhotons.eps");
}

