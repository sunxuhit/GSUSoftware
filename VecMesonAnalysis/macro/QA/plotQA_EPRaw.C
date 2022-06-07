#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>

void plotQA_EPRaw()
{
  string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mEP1st_BbcSouth = (TH1F*)File_InPut->Get("h_mEP1st_BbcSouth")->Clone();
  TH1F *h_mEP1st_BbcNorth = (TH1F*)File_InPut->Get("h_mEP1st_BbcNorth")->Clone();
  TH1F *h_mEP2nd_BbcSouth = (TH1F*)File_InPut->Get("h_mEP2nd_BbcSouth")->Clone();
  TH1F *h_mEP2nd_BbcNorth = (TH1F*)File_InPut->Get("h_mEP2nd_BbcNorth")->Clone();

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

  c_play->cd(2);
  h_mEP1st_BbcNorth->Draw("h");

  c_play->cd(3);
  h_mEP2nd_BbcSouth->Draw("h");

  c_play->cd(4);
  h_mEP2nd_BbcNorth->Draw("h");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/EPRaw.eps");
}
