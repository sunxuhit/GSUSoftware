#include <string>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>

void plotQA_BBCGeo()
{
  string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mGeoXY_BbcSouth = (TH2F*)File_InPut->Get("h_mGeoXY_BbcSouth")->Clone();
  TH2F *h_mGeoXY_BbcNorth = (TH2F*)File_InPut->Get("h_mGeoXY_BbcNorth")->Clone();
  TH1F *h_mEP1st_BbcSouth = (TH1F*)File_InPut->Get("h_mEP1st_BbcSouth")->Clone();

  float NumOfEvents = h_mEP1st_BbcSouth->GetEntries();

  h_mGeoXY_BbcSouth->Scale(1.0/NumOfEvents);
  h_mGeoXY_BbcNorth->Scale(1.0/NumOfEvents);


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
  h_mGeoXY_BbcSouth->SetTitle("BBC South");
  h_mGeoXY_BbcSouth->SetStats(0);
  h_mGeoXY_BbcSouth->Draw("colz");

  c_play->cd(2);
  h_mGeoXY_BbcNorth->SetTitle("BBC North");
  h_mGeoXY_BbcNorth->SetStats(0);
  h_mGeoXY_BbcNorth->Draw("colz");

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/BbcGeo.eps");
}
