#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>

void plotQA_EPCorr()
{
  string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mEP1st_Correlation = (TH2F*)File_InPut->Get("h_mEP1st_Correlation")->Clone();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->SetLeftMargin(0.15);
  c_play->SetBottomMargin(0.15);
  c_play->SetGrid(0,0);
  c_play->SetTicks(1,1);

  c_play->cd(1);
  h_mEP1st_Correlation->GetXaxis()->SetTitle("#Psi_{1st,South}");
  h_mEP1st_Correlation->GetYaxis()->SetTitle("#Psi_{1st,North}");
  h_mEP1st_Correlation->Draw("colz");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/EPRawCorr.eps");
}
