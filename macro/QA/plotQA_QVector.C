#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

using namespace std;

void plotQA_QVector(int order = 1, int cent20 = 5)
{
  const string mOrder[3] = {"1st","2nd","3rd"};
  const string mCentrality = Form("%1.1f-%1.1f%%",vecMesonFlow::mCentStart20[cent20],vecMesonFlow::mCentStop20[cent20]);

  string inputfile_raw = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_Raw = TFile::Open(inputfile_raw.c_str());
  string HistName_Raw;
  HistName_Raw = Form("h_mQxRaw_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQxRaw_BbcSouth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str())->Clone();
  HistName_Raw = Form("h_mQyRaw_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQyRaw_BbcSouth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str())->Clone();
  HistName_Raw = Form("h_mQxRaw_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQxRaw_BbcNorth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str())->Clone();
  HistName_Raw = Form("h_mQyRaw_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQyRaw_BbcNorth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str())->Clone();

  string inputfile_rec = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut_Rec = TFile::Open(inputfile_rec.c_str());
  string HistName_Rec;
  HistName_Rec = Form("h_mQxReCenter_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQxReCenter_BbcSouth = (TH1F*)File_InPut_Rec->Get(HistName_Rec.c_str())->Clone();
  HistName_Rec = Form("h_mQyReCenter_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQyReCenter_BbcSouth = (TH1F*)File_InPut_Rec->Get(HistName_Rec.c_str())->Clone();
  HistName_Rec = Form("h_mQxReCenter_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQxReCenter_BbcNorth = (TH1F*)File_InPut_Rec->Get(HistName_Rec.c_str())->Clone();
  HistName_Rec = Form("h_mQyReCenter_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH1F *h_mQyReCenter_BbcNorth = (TH1F*)File_InPut_Rec->Get(HistName_Rec.c_str())->Clone();

  string title;
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,1600);
  c_play->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_play->cd(i_pad+1);
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1); // Qx South
  title = Form("%s Q_{x} BBC South @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mQxRaw_BbcSouth->SetTitle(title.c_str());
  h_mQxRaw_BbcSouth->SetStats(0);
  h_mQxRaw_BbcSouth->GetXaxis()->SetTitle("Q_{x}");
  h_mQxRaw_BbcSouth->GetXaxis()->CenterTitle();
  h_mQxRaw_BbcSouth->GetXaxis()->SetTitleSize(0.06);
  h_mQxRaw_BbcSouth->GetXaxis()->SetTitleFont(132);
  h_mQxRaw_BbcSouth->GetXaxis()->SetLabelSize(0.04);
  h_mQxRaw_BbcSouth->SetNdivisions(505,"X");
  h_mQxRaw_BbcSouth->GetYaxis()->CenterTitle();
  h_mQxRaw_BbcSouth->GetYaxis()->SetTitleSize(0.06);
  h_mQxRaw_BbcSouth->GetYaxis()->SetTitleFont(132);
  h_mQxRaw_BbcSouth->GetYaxis()->SetLabelSize(0.04);
  h_mQxRaw_BbcSouth->SetNdivisions(505,"Y");
  h_mQxRaw_BbcSouth->SetLineColor(1);
  h_mQxRaw_BbcSouth->SetLineWidth(2);
  h_mQxRaw_BbcSouth->SetLineStyle(1);
  h_mQxRaw_BbcSouth->Draw("hE");
  h_mQxReCenter_BbcSouth->SetLineColor(2);
  h_mQxReCenter_BbcSouth->SetLineWidth(2);
  h_mQxReCenter_BbcSouth->SetLineStyle(2);
  h_mQxReCenter_BbcSouth->Draw("hE same");

  float mean_QxRawSouth = h_mQxRaw_BbcSouth->GetMean();
  string leg_QxRawSouth = Form("Raw Q_{x}: %1.3f",mean_QxRawSouth);
  float mean_QxReCenterSouth = h_mQxReCenter_BbcSouth->GetMean();
  string leg_QxRecSouth = Form("Re-Centered Q_{x}: %1.3f",mean_QxReCenterSouth);
  TLegend *leg_QxSouth = new TLegend(0.60,0.6,0.85,0.8);
  leg_QxSouth->SetBorderSize(0);
  leg_QxSouth->SetFillColor(10);
  leg_QxSouth->AddEntry(h_mQxRaw_BbcSouth,leg_QxRawSouth.c_str(),"l");
  leg_QxSouth->AddEntry(h_mQxReCenter_BbcSouth,leg_QxRecSouth.c_str(),"l");
  leg_QxSouth->Draw("same");

  c_play->cd(2); // Qy South
  title = Form("%s Q_{y} BBC South @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mQyRaw_BbcSouth->SetTitle(title.c_str());
  h_mQyRaw_BbcSouth->SetStats(0);
  h_mQyRaw_BbcSouth->GetXaxis()->SetTitle("Q_{y}");
  h_mQyRaw_BbcSouth->GetXaxis()->CenterTitle();
  h_mQyRaw_BbcSouth->GetXaxis()->SetTitleSize(0.06);
  h_mQyRaw_BbcSouth->GetXaxis()->SetTitleFont(132);
  h_mQyRaw_BbcSouth->GetXaxis()->SetLabelSize(0.04);
  h_mQyRaw_BbcSouth->SetNdivisions(505,"X");
  h_mQyRaw_BbcSouth->GetYaxis()->CenterTitle();
  h_mQyRaw_BbcSouth->GetYaxis()->SetTitleSize(0.06);
  h_mQyRaw_BbcSouth->GetYaxis()->SetTitleFont(132);
  h_mQyRaw_BbcSouth->GetYaxis()->SetLabelSize(0.04);
  h_mQyRaw_BbcSouth->SetNdivisions(505,"Y");
  h_mQyRaw_BbcSouth->SetLineColor(1);
  h_mQyRaw_BbcSouth->SetLineWidth(2);
  h_mQyRaw_BbcSouth->SetLineStyle(1);
  h_mQyRaw_BbcSouth->Draw("hE");
  h_mQyReCenter_BbcSouth->SetLineColor(2);
  h_mQyReCenter_BbcSouth->SetLineWidth(2);
  h_mQyReCenter_BbcSouth->SetLineStyle(2);
  h_mQyReCenter_BbcSouth->Draw("hE same");

  float mean_QyRawSouth = h_mQyRaw_BbcSouth->GetMean();
  string leg_QyRawSouth = Form("Raw Q_{y}: %1.3f",mean_QyRawSouth);
  float mean_QyReCenterSouth = h_mQyReCenter_BbcSouth->GetMean();
  string leg_QyRecSouth = Form("Re-Centered Q_{y}: %1.3f",mean_QyReCenterSouth);
  TLegend *leg_QySouth = new TLegend(0.60,0.6,0.85,0.8);
  leg_QySouth->SetBorderSize(0);
  leg_QySouth->SetFillColor(10);
  leg_QySouth->AddEntry(h_mQyRaw_BbcSouth,leg_QyRawSouth.c_str(),"l");
  leg_QySouth->AddEntry(h_mQyReCenter_BbcSouth,leg_QyRecSouth.c_str(),"l");
  leg_QySouth->Draw("same");

  c_play->cd(3); // Qx North
  title = Form("%s Q_{x} BBC North @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mQxRaw_BbcNorth->SetTitle(title.c_str());
  h_mQxRaw_BbcNorth->SetStats(0);
  h_mQxRaw_BbcNorth->GetXaxis()->SetTitle("Q_{x}");
  h_mQxRaw_BbcNorth->GetXaxis()->CenterTitle();
  h_mQxRaw_BbcNorth->GetXaxis()->SetTitleSize(0.06);
  h_mQxRaw_BbcNorth->GetXaxis()->SetTitleFont(132);
  h_mQxRaw_BbcNorth->GetXaxis()->SetLabelSize(0.04);
  h_mQxRaw_BbcNorth->SetNdivisions(505,"X");
  h_mQxRaw_BbcNorth->GetYaxis()->CenterTitle();
  h_mQxRaw_BbcNorth->GetYaxis()->SetTitleSize(0.06);
  h_mQxRaw_BbcNorth->GetYaxis()->SetTitleFont(132);
  h_mQxRaw_BbcNorth->GetYaxis()->SetLabelSize(0.04);
  h_mQxRaw_BbcNorth->SetNdivisions(505,"Y");
  h_mQxRaw_BbcNorth->SetLineColor(1);
  h_mQxRaw_BbcNorth->SetLineWidth(2);
  h_mQxRaw_BbcNorth->SetLineStyle(1);
  h_mQxRaw_BbcNorth->Draw("hE");
  h_mQxReCenter_BbcNorth->SetLineColor(2);
  h_mQxReCenter_BbcNorth->SetLineWidth(2);
  h_mQxReCenter_BbcNorth->SetLineStyle(2);
  h_mQxReCenter_BbcNorth->Draw("hE same");

  float mean_QxRawNorth = h_mQxRaw_BbcNorth->GetMean();
  string leg_QxRawNorth = Form("Raw Q_{x}: %1.3f",mean_QxRawNorth);
  float mean_QxReCenterNorth = h_mQxReCenter_BbcNorth->GetMean();
  string leg_QxRecNorth = Form("Re-Centered Q_{x}: %1.3f",mean_QxReCenterNorth);
  TLegend *leg_QxNorth = new TLegend(0.60,0.6,0.85,0.8);
  leg_QxNorth->SetBorderSize(0);
  leg_QxNorth->SetFillColor(10);
  leg_QxNorth->AddEntry(h_mQxRaw_BbcNorth,leg_QxRawNorth.c_str(),"l");
  leg_QxNorth->AddEntry(h_mQxReCenter_BbcNorth,leg_QxRecNorth.c_str(),"l");
  leg_QxNorth->Draw("same");

  c_play->cd(4); // Qy North
  title = Form("%s Q_{y} BBC North @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mQyRaw_BbcNorth->SetTitle(title.c_str());
  h_mQyRaw_BbcNorth->SetStats(0);
  h_mQyRaw_BbcNorth->GetXaxis()->SetTitle("Q_{y}");
  h_mQyRaw_BbcNorth->GetXaxis()->CenterTitle();
  h_mQyRaw_BbcNorth->GetXaxis()->SetTitleSize(0.06);
  h_mQyRaw_BbcNorth->GetXaxis()->SetTitleFont(132);
  h_mQyRaw_BbcNorth->GetXaxis()->SetLabelSize(0.04);
  h_mQyRaw_BbcNorth->SetNdivisions(505,"X");
  h_mQyRaw_BbcNorth->GetYaxis()->CenterTitle();
  h_mQyRaw_BbcNorth->GetYaxis()->SetTitleSize(0.06);
  h_mQyRaw_BbcNorth->GetYaxis()->SetTitleFont(132);
  h_mQyRaw_BbcNorth->GetYaxis()->SetLabelSize(0.04);
  h_mQyRaw_BbcNorth->SetNdivisions(505,"Y");
  h_mQyRaw_BbcNorth->SetLineColor(1);
  h_mQyRaw_BbcNorth->SetLineWidth(2);
  h_mQyRaw_BbcNorth->SetLineStyle(1);
  h_mQyRaw_BbcNorth->Draw("hE");
  h_mQyReCenter_BbcNorth->SetLineColor(2);
  h_mQyReCenter_BbcNorth->SetLineWidth(2);
  h_mQyReCenter_BbcNorth->SetLineStyle(2);
  h_mQyReCenter_BbcNorth->Draw("hE same");

  float mean_QyRawNorth = h_mQyRaw_BbcNorth->GetMean();
  string leg_QyRawNorth = Form("Raw Q_{y}: %1.3f",mean_QyRawNorth);
  float mean_QyReCenterNorth = h_mQyReCenter_BbcNorth->GetMean();
  string leg_QyRecNorth = Form("Re-Centered Q_{y}: %1.3f",mean_QyReCenterNorth);
  TLegend *leg_QyNorth = new TLegend(0.60,0.6,0.85,0.8);
  leg_QyNorth->SetBorderSize(0);
  leg_QyNorth->SetFillColor(10);
  leg_QyNorth->AddEntry(h_mQyRaw_BbcNorth,leg_QyRawNorth.c_str(),"l");
  leg_QyNorth->AddEntry(h_mQyReCenter_BbcNorth,leg_QyRecNorth.c_str(),"l");
  leg_QyNorth->Draw("same");

  string FigName = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_BbcQVector_%s_%d.eps",mOrder[order].c_str(),cent20);
  c_play->SaveAs(FigName.c_str());
}
