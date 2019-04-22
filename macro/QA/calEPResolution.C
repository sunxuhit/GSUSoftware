#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1F.h>
#include <TLegend.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

using namespace std;

void calEPResolution()
{
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  const std::string mOrder[3] = {"1st","2nd","3rd"};
  TProfile2D *p_mResolution_BbcSub[3];
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    string ProName = Form("p_mResolution_BbcSub_%s",mOrder[i_order].c_str());
    cout << "read in: " << ProName.c_str() << endl;
    p_mResolution_BbcSub[i_order] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();
  }

  TGraphAsymmErrors *g_mResolution_BbcSub[3];
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    g_mResolution_BbcSub[i_order] = new TGraphAsymmErrors();
    for(int i_cent = 0; i_cent < 20; ++i_cent)
    {
      int bin_cos = p_mResolution_BbcSub[i_order]->FindBin(i_cent);
      float val_cos = p_mResolution_BbcSub[i_order]->GetBinContent(bin_cos);
      float err_cos = p_mResolution_BbcSub[i_order]->GetBinError(bin_cos);
      int mean_cent20 = 0.5*(vecMesonFlow::mCentStart20[i_cent]+vecMesonFlow::mCentStop20[i_cent]);
      // cout << "i_order = " << i_order << ", i_cent = " << i_cent << ", bin_cos = " << bin_cos << ", val_cos = " << val_cos << endl;
      if(val_cos > 0.0)
      {
	float val_resolution = TMath::Sqrt(val_cos);
	float err_resolution = err_cos/(2.0*val_resolution);
	g_mResolution_BbcSub[i_order]->SetPoint(i_cent,mean_cent20,val_resolution*100.0);
	g_mResolution_BbcSub[i_order]->SetPointError(i_cent,0.0,0.0,err_resolution*100.0,err_resolution*100.0);
      }
    }
  }

  int mColor[3] = {kGray+3,kRed-4,kAzure+4};
  int mStyle[3] = {33,20,22};

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
  h_play->SetTitle("BBC Event Plane Resolution");
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
  h_play->GetYaxis()->SetRangeUser(-1.0,40.0);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetLabelSize(0.04);
  h_play->SetNdivisions(505,"X");
  h_play->SetNdivisions(505,"Y");
  h_play->Draw("pE");

  TLegend *leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    g_mResolution_BbcSub[i_order]->SetMarkerColor(mColor[i_order]);
    g_mResolution_BbcSub[i_order]->SetMarkerStyle(mStyle[i_order]);
    g_mResolution_BbcSub[i_order]->SetMarkerSize(1.4);
    g_mResolution_BbcSub[i_order]->Draw("pE same");
    leg->AddEntry(g_mResolution_BbcSub[i_order],mOrder[i_order].c_str(),"P");
  }
  leg->Draw("same");
  c_play->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_BbcEPResolution.eps");

  string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_Resolution.root";
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    p_mResolution_BbcSub[i_order]->Write();
    // string GraphName = Form("g_mResolution_BbcSub_%s",mOrder[i_order].c_str());
    // g_mResolution_BbcSub[i_order]->SetName(GraphName.c_str());
    // g_mResolution_BbcSub[i_order]->Write();
  }
  File_OutPut->Close();

  File_InPut->Close();
}

