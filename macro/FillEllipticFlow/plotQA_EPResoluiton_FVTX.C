#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1F.h>
#include <TLegend.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

using namespace std;

void plotQA_EPResoluiton_FVTX(int order = 1)
{
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/FillEllipticFlow/file_AuAu200GeV_Resolution.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  const std::string mOrder[3] = {"1st","2nd","3rd"};
  TProfile *p_mResolution_BbcSub[3];
  TProfile *p_mResolution_FvtxSub[3];
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    string ProName_Bbc = Form("p_mResolution_BbcSub_%s",mOrder[i_order].c_str());
    cout << "read in: " << ProName_Bbc.c_str() << endl;
    p_mResolution_BbcSub[i_order] = (TProfile*)File_InPut->Get(ProName_Bbc.c_str())->Clone();
    string ProName_Fvtx = Form("p_mResolution_FvtxSub_%s",mOrder[i_order].c_str());
    cout << "read in: " << ProName_Fvtx.c_str() << endl;
    p_mResolution_FvtxSub[i_order] = (TProfile*)File_InPut->Get(ProName_Fvtx.c_str())->Clone();
  }

  TGraphAsymmErrors *g_mResolution_BbcSub = new TGraphAsymmErrors();
  for(int i_cent = 0; i_cent < 20; ++i_cent)
  {
    int bin_cos = p_mResolution_BbcSub[order]->FindBin(i_cent);
    float val_cos = p_mResolution_BbcSub[order]->GetBinContent(bin_cos);
    float err_cos = p_mResolution_BbcSub[order]->GetBinError(bin_cos);
    int mean_cent20 = 0.5*(vecMesonFlow::mCentStart20[i_cent]+vecMesonFlow::mCentStop20[i_cent]);
    // cout << "order = " << order << ", i_cent = " << i_cent << ", bin_cos = " << bin_cos << ", val_cos = " << val_cos << endl;
    if(val_cos > 0.0)
    {
      float val_resolution = TMath::Sqrt(val_cos);
      float err_resolution = err_cos/(2.0*val_resolution);
      g_mResolution_BbcSub->SetPoint(i_cent,mean_cent20,val_resolution*100.0);
      g_mResolution_BbcSub->SetPointError(i_cent,0.0,0.0,err_resolution*100.0,err_resolution*100.0);
    }
  }

  TGraphAsymmErrors *g_mResolution_FvtxSub = new TGraphAsymmErrors();
  for(int i_cent = 0; i_cent < 20; ++i_cent)
  {
    int bin_cos = p_mResolution_FvtxSub[order]->FindBin(i_cent);
    float val_cos = p_mResolution_FvtxSub[order]->GetBinContent(bin_cos);
    float err_cos = p_mResolution_FvtxSub[order]->GetBinError(bin_cos);
    int mean_cent20 = 0.5*(vecMesonFlow::mCentStart20[i_cent]+vecMesonFlow::mCentStop20[i_cent]);
    // cout << "order = " << order << ", i_cent = " << i_cent << ", bin_cos = " << bin_cos << ", val_cos = " << val_cos << endl;
    if(val_cos > 0.0)
    {
      float val_resolution = TMath::Sqrt(val_cos);
      float err_resolution = err_cos/(2.0*val_resolution);
      g_mResolution_FvtxSub->SetPoint(i_cent,mean_cent20,val_resolution*100.0);
      g_mResolution_FvtxSub->SetPointError(i_cent,0.0,0.0,err_resolution*100.0,err_resolution*100.0);
    }
  }

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->cd();
  c_play->cd()->SetLeftMargin(0.15);
  c_play->cd()->SetBottomMargin(0.15);
  c_play->cd()->SetTicks(1,1);
  c_play->cd()->SetGrid(0,0);

  TH1F *h_play = new TH1F("h_play","h_play",1000,0.0,100.0);
  for(int i_bin = 0; i_bin < 1000; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-10.0);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetTitle("Event Plane Resolution");
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("centrality (%)");
  h_play->GetYaxis()->SetTitle("Event Plane Resolution (%)");
  h_play->GetXaxis()->CenterTitle();
  h_play->GetYaxis()->CenterTitle();
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetTitleFont(132);
  h_play->GetYaxis()->SetTitleFont(132);
  h_play->GetXaxis()->SetRangeUser(0,100);
  h_play->GetYaxis()->SetRangeUser(-1.0,80.0);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetLabelSize(0.04);
  h_play->SetNdivisions(505,"X");
  h_play->SetNdivisions(505,"Y");
  h_play->Draw("pE");

  g_mResolution_BbcSub->SetMarkerColor(kGray+2);
  g_mResolution_BbcSub->SetMarkerStyle(20);
  g_mResolution_BbcSub->SetMarkerSize(1.4);
  g_mResolution_BbcSub->Draw("pE same");

  g_mResolution_FvtxSub->SetMarkerColor(kAzure);
  g_mResolution_FvtxSub->SetMarkerStyle(24);
  g_mResolution_FvtxSub->SetMarkerSize(1.4);
  g_mResolution_FvtxSub->Draw("pE same");

  TLegend *leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(g_mResolution_BbcSub,"BBC 2^{nd} EP","P");
  leg->AddEntry(g_mResolution_FvtxSub,"FVTX 2^{nd} EP","P");
  leg->Draw("same");
  // c_play->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_DiMuon/c_EPResolution_fvtx.eps");
}

