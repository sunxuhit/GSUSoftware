#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>

void plotQA_Vtx()
{
  string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mVtZ = (TH1F*)File_InPut->Get("h_mVtZ")->Clone();
  TH1F *h_mVtZ_Bbc = (TH1F*)File_InPut->Get("h_mVtZ_Bbc")->Clone();
  TH1F *h_mVtZ_Zdc = (TH1F*)File_InPut->Get("h_mVtZ_Zdc")->Clone();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->SetLeftMargin(0.15);
  c_play->SetBottomMargin(0.15);
  c_play->SetGrid(0,0);
  c_play->SetTicks(1,1);

  h_mVtZ->SetTitle("Vtx Z");
  h_mVtZ->SetStats(0);
  h_mVtZ->Draw("h");

  h_mVtZ_Bbc->SetLineColor(2);
  h_mVtZ_Bbc->Draw("h same");

  h_mVtZ_Zdc->SetLineColor(4);
  h_mVtZ_Zdc->Draw("h same");

  TLegend *leg = new TLegend(0.6,0.6,0.8,0.75);
  leg->SetFillColor(10);
  leg->AddEntry("h_mVtZ","VtxOut","l");
  leg->AddEntry("h_mVtZ_Bbc","BBC Vtx","l");
  leg->AddEntry("h_mVtZ_Zdc","ZDC Vtx","l");
  leg->Draw("same");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/Vertex.eps");
}
