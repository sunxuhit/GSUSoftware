#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1F.h>
#include <TLegend.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/FillEllipticFlow/PhVecMesonCons.h"
#include "./draw.h"

using namespace std;

void plotQA_Spec(int cent4 =3)
{
  string input_US = "/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_JPsi_US.root";
  TFile *File_US = TFile::Open(input_US.c_str());
  TH1F *h_mSpecUS[4];
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    string KEY_Spec = Form("Spec_%s_Pt_%d_Centrality_%s_%s",vecMesonFlow::mPID[0].c_str(),i_pt,vecMesonFlow::mCentType[cent4].c_str(),vecMesonFlow::mEventType[0].c_str());
    h_mSpecUS[i_pt] = (TH1F*)File_US->Get(KEY_Spec.c_str())->Clone();
  }

  string input_LS = "/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_JPsi_LS.root";
  TFile *File_LS = TFile::Open(input_LS.c_str());
  TH1F *h_mSpecLS[4];
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    string KEY_Spec = Form("Spec_%s_Pt_%d_Centrality_%s_%s",vecMesonFlow::mPID[0].c_str(),i_pt,vecMesonFlow::mCentType[cent4].c_str(),vecMesonFlow::mEventType[1].c_str());
    h_mSpecLS[i_pt] = (TH1F*)File_LS->Get(KEY_Spec.c_str())->Clone();
  }

  TH1F *h_mSpec[4];
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    h_mSpec[i_pt] = (TH1F*)h_mSpecUS[i_pt]->Clone();
    h_mSpec[i_pt]->Add(h_mSpecLS[i_pt],-1.0);
  }

  TCanvas *c_spec = new TCanvas("c_spec","c_spec",10,10,800,800);
  c_spec->Divide(2,2);
  for(int i_pad = 0; i_pad < vecMesonFlow::mNumOfPt; ++i_pad)
  {
    c_spec->cd(i_pad+1)->SetLeftMargin(0.15);
    c_spec->cd(i_pad+1)->SetBottomMargin(0.15);
    c_spec->cd(i_pad+1)->SetTicks(1,1);
    c_spec->cd(i_pad+1)->SetGrid(0,0);
    h_mSpecUS[i_pad]->GetXaxis()->SetRangeUser(2.0,5.0);
    h_mSpecUS[i_pad]->SetLineColor(2);
    h_mSpecUS[i_pad]->Draw("hE");
    h_mSpecLS[i_pad]->SetLineColor(4);
    h_mSpecLS[i_pad]->Draw("hE same");
  }

  TCanvas *c_yields = new TCanvas("c_yields","c_yields",10,10,800,800);
  c_yields->Divide(2,2);
  for(int i_pad = 0; i_pad < vecMesonFlow::mNumOfPt; ++i_pad)
  {
    c_yields->cd(i_pad+1)->SetLeftMargin(0.15);
    c_yields->cd(i_pad+1)->SetBottomMargin(0.15);
    c_yields->cd(i_pad+1)->SetTicks(1,1);
    c_yields->cd(i_pad+1)->SetGrid(0,0);
    h_mSpec[i_pad]->SetStats(0);
    h_mSpec[i_pad]->GetXaxis()->SetRangeUser(2.0,5.0);
    h_mSpec[i_pad]->SetLineColor(1);
    h_mSpec[i_pad]->Draw("hE");
    PlotLine(2.0,5.0,0.0,0.0,4,2,2);
    string pt_range = Form("%1.1f-%1.1f GeV/c",vecMesonFlow::mPtStart[i_pad],vecMesonFlow::mPtStop[i_pad]);
    plotTopLegend(pt_range.c_str(),0.5,0.7,0.06,1,0.0,42,1,1);
    int bin_start = h_mSpec[i_pad]->GetXaxis()->FindBin(2.8);
    int bin_stop  = h_mSpec[i_pad]->GetXaxis()->FindBin(3.5);
    // cout << "bin_start = " << bin_start << ", bin_stop = " << bin_stop << endl;
    float yields = 0.0;
    float error = 0.0;
    for(int i_bin = bin_start; i_bin < bin_stop; ++i_bin)
    {
      yields += h_mSpec[i_pad]->GetBinContent(i_bin);
      error += h_mSpec[i_pad]->GetBinError(i_bin)*h_mSpec[i_pad]->GetBinError(i_bin);
    }
    string leg_yields = Form("%1.1f #pm %1.1f",yields, sqrt(error));
    plotTopLegend(leg_yields.c_str(),0.5,0.6,0.06,1,0.0,42,1,1);
  }
}
