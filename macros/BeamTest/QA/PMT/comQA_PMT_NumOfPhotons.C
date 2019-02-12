#include <string>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>

using namespace std;

void comQA_PMT_NumOfPhotons()
{
  string inputSim = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/Simulation/PMT/GEMC_Calibration.root";
  TFile *File_Sim = TFile::Open(inputSim.c_str());
  TH1D *h_NumOfPhotons_Sim = (TH1D*)File_Sim->Get("h_mNumOfPhotons")->Clone();
  TH3D *h_NumOfCherenkovPhotons_Sim = (TH3D*)File_Sim->Get("h_mNumOfCherenkovPhotons")->Clone();
  TH1D *h_NumOnRing_Sim = (TH1D*)h_NumOfCherenkovPhotons_Sim->Project3D("y")->Clone("h_NumOnRing_Sim");

  string inputData = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/BeamTest/PMT/BeamTest_Calibration.root";
  TFile *File_Data = TFile::Open(inputData.c_str());
  TH1D *h_NumOfPhotons_Data = (TH1D*)File_Data->Get("h_mNumOfPhotons")->Clone();
  TH3D *h_NumOfCherenkovPhotons_Data = (TH3D*)File_Data->Get("h_mNumOfCherenkovPhotons")->Clone();
  TH1D *h_NumOnRing_Data = (TH1D*)h_NumOfCherenkovPhotons_Data->Project3D("y")->Clone("h_NumOnRing_Data");

  TCanvas *c_NumOfPhotons = new TCanvas("c_NumOfPhotons","c_NumOfPhotons",10,10,800,800);
  c_NumOfPhotons->SetLeftMargin(0.15);
  c_NumOfPhotons->SetBottomMargin(0.15);
  c_NumOfPhotons->SetRightMargin(0.15);
  c_NumOfPhotons->SetTicks(1,1);
  c_NumOfPhotons->SetGrid(0,0);
  h_NumOfPhotons_Sim->SetTitle("");
  h_NumOfPhotons_Sim->SetStats(0);
  h_NumOfPhotons_Sim->GetXaxis()->SetTitle("Number of Photons");
  h_NumOfPhotons_Sim->GetXaxis()->CenterTitle();
  h_NumOfPhotons_Sim->GetYaxis()->SetRangeUser(0.0,1000.0);
  h_NumOfPhotons_Sim->SetLineColor(4);
  h_NumOfPhotons_Sim->Draw("h");

  h_NumOfPhotons_Data->SetLineColor(2);
  h_NumOfPhotons_Data->Draw("h same");

  TLegend *leg = new TLegend(0.5,0.5,0.8,0.7);
  /* leg->SetBorderSize(0); */
  leg->SetFillColor(0);
  leg->AddEntry(h_NumOfPhotons_Sim,"Simulation","l");
  leg->AddEntry(h_NumOfPhotons_Data,"Data","l");
  leg->Draw("same");

  c_NumOfPhotons->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_NumOfPhotons.eps");

  TCanvas *c_NumOnRing = new TCanvas("c_NumOnRing","c_NumOnRing",10,10,800,800);
  c_NumOnRing->SetLeftMargin(0.15);
  c_NumOnRing->SetBottomMargin(0.15);
  c_NumOnRing->SetRightMargin(0.15);
  c_NumOnRing->SetTicks(1,1);
  c_NumOnRing->SetGrid(0,0);
  h_NumOnRing_Sim->SetTitle("");
  h_NumOnRing_Sim->SetStats(0);
  h_NumOnRing_Sim->GetXaxis()->SetTitle("Number of Photons On Ring");
  h_NumOnRing_Sim->GetXaxis()->CenterTitle();
  h_NumOnRing_Sim->GetXaxis()->SetRangeUser(-0.5,49.5);
  h_NumOnRing_Sim->GetYaxis()->SetRangeUser(0.0,1000.0);
  h_NumOnRing_Sim->SetLineColor(4);
  h_NumOnRing_Sim->Draw("h");

  h_NumOnRing_Data->SetLineColor(2);
  h_NumOnRing_Data->Draw("h same");

  TLegend *leg_onring = new TLegend(0.5,0.5,0.8,0.7);
  /* leg->SetBorderSize(0); */
  leg_onring->SetFillColor(0);
  leg_onring->AddEntry(h_NumOnRing_Sim,"Simulation","l");
  leg_onring->AddEntry(h_NumOnRing_Data,"Data","l");
  leg_onring->Draw("same");

  c_NumOnRing->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_NumOnRing.eps");
}
