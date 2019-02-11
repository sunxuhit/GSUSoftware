#include <string>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>

using namespace std;

void comQA_PMT_Radius()
{
  string inputSim = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/Simulation/PMT/GEMC_Calibration.root";
  TFile *File_Sim = TFile::Open(inputSim.c_str());
  TH3D *h_CherenkovRing_Sim = (TH3D*)File_Sim->Get("h_mCherenkovRing")->Clone();
  TH1D *h_RingRadius_Sim = (TH1D*)h_CherenkovRing_Sim->Project3D("z")->Clone("h_RingRadius_Sim");
  double Inte_Sim = h_RingRadius_Sim->Integral();
  h_RingRadius_Sim->Scale(1.0/Inte_Sim);

  string inputData = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/BeamTest/PMT/BeamTest_Calibration.root";
  TFile *File_Data = TFile::Open(inputData.c_str());
  TH3D *h_CherenkovRing_Data = (TH3D*)File_Data->Get("h_mCherenkovRing")->Clone();
  TH1D *h_RingRadius_Data = (TH1D*)h_CherenkovRing_Data->Project3D("z")->Clone("h_RingRadius_Data");
  double Inte_Data = h_RingRadius_Data->Integral();
  h_RingRadius_Data->Scale(1.0/Inte_Data);

  TCanvas *c_RingRadius = new TCanvas("c_RingRadius","c_RingRadius",10,10,800,800);
  c_RingRadius->SetLeftMargin(0.15);
  c_RingRadius->SetBottomMargin(0.15);
  c_RingRadius->SetRightMargin(0.15);
  c_RingRadius->SetTicks(1,1);
  c_RingRadius->SetGrid(0,0);
  h_RingRadius_Sim->SetTitle("ring radiuas");
  h_RingRadius_Sim->SetStats(0);
  h_RingRadius_Sim->GetXaxis()->SetTitle("R (mm)");
  h_RingRadius_Sim->GetXaxis()->CenterTitle();
  h_RingRadius_Sim->SetLineColor(4);
  h_RingRadius_Sim->Draw("h");

  h_RingRadius_Data->SetLineColor(2);
  h_RingRadius_Data->Draw("h same");

  TLegend *leg = new TLegend(0.5,0.5,0.8,0.7);
  /* leg->SetBorderSize(0); */
  leg->SetFillColor(0);
  leg->AddEntry(h_RingRadius_Sim,"Simulation","l");
  leg->AddEntry(h_RingRadius_Data,"Data","l");
  leg->Draw("same");

  c_RingRadius->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/Calibration/c_RingRadius.eps");
}
