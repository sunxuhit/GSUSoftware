#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

void plotQA_PMT_NumOfPhotons()
{
  TFile *File_Data = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/BeamTest_Calibration.root");
  TH1D *h_mNumOfPhotons_OnRing_Data = (TH1D*)File_Data->Get("h_mNumOfPhotons_OnRing");
  TH1D *h_mNumOfPhotons_OffRing_Data = (TH1D*)File_Data->Get("h_mNumOfPhotons_OffRing");
  float NumOfEvents_Data = h_mNumOfPhotons_OnRing_Data->GetEntries();

  TFile *File_Sim = TFile::Open("/Users/xusun/WorkSpace/EICPID/OutPut/Simulation/PMT/GEMC_Calibration.root");
  TH1D *h_mNumOfPhotons_OnRing_Sim = (TH1D*)File_Sim->Get("h_mNumOfPhotons_OnRing");
  TH1D *h_mNumOfPhotons_OffRing_Sim = (TH1D*)File_Sim->Get("h_mNumOfPhotons_OffRing");
  float NumOfEvents_Sim = h_mNumOfPhotons_OnRing_Sim->GetEntries();

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
  h_mNumOfPhotons_OnRing_Data->SetTitle("Num. of Photons on Ring"); // Data
  // h_mNumOfPhotons_OnRing_Data->SetStats(0);
  h_mNumOfPhotons_OnRing_Data->SetLineColor(1);
  h_mNumOfPhotons_OnRing_Data->SetLineWidth(2);
  h_mNumOfPhotons_OnRing_Data->Draw("h");
  h_mNumOfPhotons_OnRing_Sim->Scale(NumOfEvents_Data/NumOfEvents_Sim); // Sim
  h_mNumOfPhotons_OnRing_Sim->SetMarkerSize(1.4);
  h_mNumOfPhotons_OnRing_Sim->SetMarkerStyle(24);
  h_mNumOfPhotons_OnRing_Sim->SetMarkerColor(2);
  h_mNumOfPhotons_OnRing_Sim->Draw("pE same");

  c_NumOfPhotons->cd(2);
  h_mNumOfPhotons_OffRing_Data->SetTitle("Num. of Photons off Ring"); // Data
  // h_mNumOfPhotons_OffRing_Data->SetStats(0);
  h_mNumOfPhotons_OffRing_Data->SetLineColor(1);
  h_mNumOfPhotons_OffRing_Data->SetLineWidth(2);
  h_mNumOfPhotons_OffRing_Data->Draw("h");
  h_mNumOfPhotons_OffRing_Sim->Scale(NumOfEvents_Data/NumOfEvents_Sim); // Sim
  h_mNumOfPhotons_OffRing_Sim->SetMarkerSize(1.4);
  h_mNumOfPhotons_OffRing_Sim->SetMarkerStyle(24);
  h_mNumOfPhotons_OffRing_Sim->SetMarkerColor(2);
  h_mNumOfPhotons_OffRing_Sim->Draw("pE same");

  TLegend *leg = new TLegend(0.4,0.6,0.8,0.8);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_mNumOfPhotons_OffRing_Data,"Data","L");
  leg->AddEntry(h_mNumOfPhotons_OffRing_Sim,"Sim","P");
  leg->Draw("same");

  c_NumOfPhotons->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/AnaNote/c_PMT_NumOfPhotons.eps");
}

