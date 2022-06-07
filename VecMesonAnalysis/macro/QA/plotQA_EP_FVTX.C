#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

void plotQA_EP_FVTX(int order = 1, int cent20 = 2)
{
  const string mOrder[3] = {"1st","2nd","3rd"};
  const string mCentrality = Form("%1.1f-%1.1f%%",vecMesonFlow::mCentStart20[cent20],vecMesonFlow::mCentStop20[cent20]);

  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_DiMuonSpec.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  string HistName_Bbc = Form("h_mEP_BbcCorrelation_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH2F *h_mEP_BbcCorrelation = (TH2F*)File_InPut->Get(HistName_Bbc.c_str());

  string HistName_Fvtx = Form("h_mEP_FvtxCorrelation_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH2F *h_mEP_FvtxCorrelation = (TH2F*)File_InPut->Get(HistName_Fvtx.c_str());

  string title;
  string titleX = Form("#Psi_{%s}^{Soutth}",mOrder[order].c_str());
  string titleY = Form("#Psi_{%s}^{North}",mOrder[order].c_str());
  TCanvas *c_correlation = new TCanvas("c_correlation","c_correlation",10,10,800,400);
  c_correlation->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_correlation->cd(i_pad+1);
    c_correlation->cd(i_pad+1)->SetLeftMargin(0.15);
    c_correlation->cd(i_pad+1)->SetBottomMargin(0.15);
    c_correlation->cd(i_pad+1)->SetTicks(1,1);
    c_correlation->cd(i_pad+1)->SetGrid(0,0);
  }
  
  c_correlation->cd(1);
  title = Form("%s BBC EP Correlation @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEP_BbcCorrelation->SetTitle(title.c_str());
  h_mEP_BbcCorrelation->SetStats(0);
  h_mEP_BbcCorrelation->GetXaxis()->SetTitle(titleX.c_str());
  h_mEP_BbcCorrelation->GetXaxis()->SetTitleSize(0.06);
  h_mEP_BbcCorrelation->GetXaxis()->CenterTitle();

  h_mEP_BbcCorrelation->GetYaxis()->SetTitle(titleY.c_str());
  h_mEP_BbcCorrelation->GetYaxis()->SetTitleSize(0.06);
  h_mEP_BbcCorrelation->GetYaxis()->CenterTitle();
  h_mEP_BbcCorrelation->Draw("colz");
  
  c_correlation->cd(2);
  title = Form("%s FVTX EP Correlation @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEP_FvtxCorrelation->SetTitle(title.c_str());
  h_mEP_FvtxCorrelation->SetStats(0);
  h_mEP_FvtxCorrelation->GetXaxis()->SetTitle(titleX.c_str());
  h_mEP_FvtxCorrelation->GetXaxis()->SetTitleSize(0.06);
  h_mEP_FvtxCorrelation->GetXaxis()->CenterTitle();

  h_mEP_FvtxCorrelation->GetYaxis()->SetTitle(titleY.c_str());
  h_mEP_FvtxCorrelation->GetYaxis()->SetTitleSize(0.06);
  h_mEP_FvtxCorrelation->GetYaxis()->CenterTitle();
  h_mEP_FvtxCorrelation->Draw("colz");

  string FigName = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_DiMuon/c_EPCorrelation_%s_%d_fvtx.eps",mOrder[order].c_str(),cent20);
  c_correlation->SaveAs(FigName.c_str());
}

