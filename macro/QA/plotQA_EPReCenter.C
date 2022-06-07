#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>

void plotQA_EPReCenter()
{
  string input_raw= "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_Raw = TFile::Open(input_raw.c_str());
  TH1F *h_mEP1st_BbcSouth = (TH1F*)File_Raw->Get("h_mEP1st_BbcSouth")->Clone();
  TH1F *h_mEP1st_BbcNorth = (TH1F*)File_Raw->Get("h_mEP1st_BbcNorth")->Clone();
  TH1F *h_mEP2nd_BbcSouth = (TH1F*)File_Raw->Get("h_mEP2nd_BbcSouth")->Clone();
  TH1F *h_mEP2nd_BbcNorth = (TH1F*)File_Raw->Get("h_mEP2nd_BbcNorth")->Clone();

  string input_recenter = "/Users/xusun/WorkSpace/PHENIX/Data/ReCenterEP.root";
  TFile *File_ReCenter = TFile::Open(input_recenter.c_str());
  TH1F *h_mEP1st_BbcSouth_ReCenter = (TH1F*)File_ReCenter->Get("h_mEP1st_BbcSouth_ReCenter")->Clone();
  TH1F *h_mEP1st_BbcNorth_ReCenter = (TH1F*)File_ReCenter->Get("h_mEP1st_BbcNorth_ReCenter")->Clone();
  TH1F *h_mEP2nd_BbcSouth_ReCenter = (TH1F*)File_ReCenter->Get("h_mEP2nd_BbcSouth_ReCenter")->Clone();
  TH1F *h_mEP2nd_BbcNorth_ReCenter = (TH1F*)File_ReCenter->Get("h_mEP2nd_BbcNorth_ReCenter")->Clone();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,1600);
  c_play->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetGrid(0,0);
    c_play->cd(i_pad+1)->SetTicks(1,1);
  }

  c_play->cd(1);
  h_mEP1st_BbcSouth->Draw("h");
  h_mEP1st_BbcSouth_ReCenter->SetLineColor(2);
  h_mEP1st_BbcSouth_ReCenter->Draw("h same");

  c_play->cd(2);
  h_mEP1st_BbcNorth->Draw("h");
  h_mEP1st_BbcNorth_ReCenter->SetLineColor(2);
  h_mEP1st_BbcNorth_ReCenter->Draw("h same");

  c_play->cd(3);
  h_mEP2nd_BbcSouth->Draw("h");
  h_mEP2nd_BbcSouth_ReCenter->SetLineColor(2);
  h_mEP2nd_BbcSouth_ReCenter->Draw("h same");

  c_play->cd(4);
  h_mEP2nd_BbcNorth->Draw("h");
  h_mEP2nd_BbcNorth_ReCenter->SetLineColor(2);
  h_mEP2nd_BbcNorth_ReCenter->Draw("h same");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/EPReCenter.eps");
}
