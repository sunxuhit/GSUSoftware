#include <string>
#include <TFile.h>
#include <TProfile2D.h>
#include <TCanvas.h>

void plotQA_ReCenter()
{
  string input_raw = "/Users/xusun/WorkSpace/PHENIX/Data/RawEP.root";
  TFile *File_Raw = TFile::Open(input_raw.c_str());
  TProfile2D *p_mQx1st_vertex_neg_BbcNorth = (TProfile2D*)File_Raw->Get("p_mQx1st_vertex_neg_BbcNorth")->Clone();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->SetLeftMargin(0.15);
  c_play->SetBottomMargin(0.15);
  c_play->SetGrid(0,0);
  c_play->SetTicks(1,1);

  p_mQx1st_vertex_neg_BbcNorth->GetXaxis()->SetTitle("runID");
  p_mQx1st_vertex_neg_BbcNorth->GetYaxis()->SetTitle("Centrality");
  p_mQx1st_vertex_neg_BbcNorth->Draw();

  c_play->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_BBC/ReCenterPar.eps");
}
