#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>

void plotQA_DiMuonSpec()
{
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_DiMuonSpec.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH1F *h_mDiMuonInteSpec_North_pt_0 = (TH1F*)File_InPut->Get("h_mDiMuonInteSpec_North_pt_0");
  TH1F *h_mDiMuonInteSpec_North_pt_1 = (TH1F*)File_InPut->Get("h_mDiMuonInteSpec_North_pt_1");
  TH1F *h_mDiMuonInteSpec_South_pt_0 = (TH1F*)File_InPut->Get("h_mDiMuonInteSpec_South_pt_0");
  TH1F *h_mDiMuonInteSpec_South_pt_1 = (TH1F*)File_InPut->Get("h_mDiMuonInteSpec_South_pt_1");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1000,500);
  c_play->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_play->cd(i_pad+1);
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1);
  h_mDiMuonInteSpec_North_pt_0->SetTitle("0.0 < p_{T} < 2.0 GeV/c");
  h_mDiMuonInteSpec_North_pt_0->SetStats(0);
  h_mDiMuonInteSpec_North_pt_0->GetXaxis()->SetTitle("InvMass(#mu^{+},#mu^{-}) (GeV/c^{2})");
  h_mDiMuonInteSpec_North_pt_0->GetXaxis()->SetTitleSize(0.06);
  h_mDiMuonInteSpec_North_pt_0->GetXaxis()->CenterTitle();
  h_mDiMuonInteSpec_North_pt_0->GetYaxis()->SetTitle();
  h_mDiMuonInteSpec_North_pt_0->GetYaxis()->SetTitleSize(0.06);
  h_mDiMuonInteSpec_North_pt_0->GetYaxis()->CenterTitle();
  h_mDiMuonInteSpec_North_pt_0->SetLineColor(2);
  h_mDiMuonInteSpec_North_pt_0->Draw("h");
  h_mDiMuonInteSpec_South_pt_0->Draw("h same");
  TLegend *leg_low = new TLegend(0.6,0.6,0.8,0.8);
  leg_low->SetBorderSize(0);
  leg_low->SetFillColor(10);
  leg_low->AddEntry(h_mDiMuonInteSpec_North_pt_0,"North","l");
  leg_low->AddEntry(h_mDiMuonInteSpec_South_pt_0,"South","l");
  leg_low->Draw("same");

  c_play->cd(2);
  h_mDiMuonInteSpec_North_pt_1->SetTitle("2.0 < p_{T} < 6.0 GeV/c");
  h_mDiMuonInteSpec_North_pt_1->SetStats(0);
  h_mDiMuonInteSpec_North_pt_1->GetXaxis()->SetTitle("InvMass(#mu^{+},#mu^{-}) (GeV/c^{2})");
  h_mDiMuonInteSpec_North_pt_1->GetXaxis()->SetTitleSize(0.06);
  h_mDiMuonInteSpec_North_pt_1->GetXaxis()->CenterTitle();
  h_mDiMuonInteSpec_North_pt_1->GetYaxis()->SetTitle();
  h_mDiMuonInteSpec_North_pt_1->GetYaxis()->SetTitleSize(0.06);
  h_mDiMuonInteSpec_North_pt_1->GetYaxis()->CenterTitle();
  h_mDiMuonInteSpec_North_pt_1->SetLineColor(2);
  h_mDiMuonInteSpec_North_pt_1->Draw("h");
  h_mDiMuonInteSpec_South_pt_1->Draw("h same");
  TLegend *leg_high = new TLegend(0.6,0.6,0.8,0.8);
  leg_high->SetBorderSize(0);
  leg_high->SetFillColor(10);
  leg_high->AddEntry(h_mDiMuonInteSpec_North_pt_1,"North","l");
  leg_high->AddEntry(h_mDiMuonInteSpec_South_pt_1,"South","l");
  leg_high->Draw("same");

  c_play->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_DiMuon/c_DiMuonSpec.eps");
}
