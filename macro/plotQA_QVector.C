#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>

void plotQA_QVector()
{
  string input_raw= "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_Raw = TFile::Open(input_raw.c_str());
  TH1F *h_mQx1st_BbcNorth = (TH1F*)File_Raw->Get("h_mQx1st_BbcNorth")->Clone();
  TH1F *h_mQy1st_BbcNorth = (TH1F*)File_Raw->Get("h_mQy1st_BbcNorth")->Clone();

  string input_recenter = "/Users/xusun/WorkSpace/PHENIX/Data/ReCenterEP.root";
  TFile *File_ReCenter = TFile::Open(input_recenter.c_str());
  TH1F *h_mQx1st_BbcNorth_ReCenter = (TH1F*)File_ReCenter->Get("h_mQx1st_BbcNorth_ReCenter")->Clone();
  TH1F *h_mQy1st_BbcNorth_ReCenter = (TH1F*)File_ReCenter->Get("h_mQy1st_BbcNorth_ReCenter")->Clone();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,800);
  c_play->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetGrid(0,0);
    c_play->cd(i_pad+1)->SetTicks(1,1);
  }

  c_play->cd(1);
  h_mQx1st_BbcNorth->Draw("h");
  h_mQx1st_BbcNorth->GetXaxis()->SetTitle("Q_{x} (1st)");
  h_mQx1st_BbcNorth_ReCenter->SetLineColor(2);
  h_mQx1st_BbcNorth_ReCenter->Draw("h same");
  TLegend *leg_x = new TLegend(0.6,0.6,0.8,0.75);
  leg_x->SetFillColor(10);
  leg_x->AddEntry("h_mQx1st_BbcNorth","Raw","l");
  leg_x->AddEntry("h_mQx1st_BbcNorth_ReCenter","ReCenter","l");
  leg_x->Draw("same");

  c_play->cd(2);
  h_mQy1st_BbcNorth->Draw("h");
  h_mQy1st_BbcNorth->GetXaxis()->SetTitle("Q_{y} (1st)");
  h_mQy1st_BbcNorth_ReCenter->SetLineColor(2);
  h_mQy1st_BbcNorth_ReCenter->Draw("h same");
  TLegend *leg_y = new TLegend(0.6,0.6,0.8,0.75);
  leg_y->SetFillColor(10);
  leg_y->AddEntry("h_mQy1st_BbcNorth","Raw","l");
  leg_y->AddEntry("h_mQy1st_BbcNorth_ReCenter","ReCenter","l");
  leg_y->Draw("same");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/QVector.eps");
}
