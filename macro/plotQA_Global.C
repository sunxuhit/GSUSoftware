#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>

void plotQA_Global()
{
  string input_rawEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_Raw = TFile::Open(input_rawEP.c_str());
  TH1F *h_mVtZ_Bbc_nocuts = (TH1F*)File_InPut_Raw->Get("h_mVtZ_Bbc_nocuts");
  TH1F *h_mVtZ_Bbc = (TH1F*)File_InPut_Raw->Get("h_mVtZ_Bbc");
  TH1F *h_mCentrality_nocuts = (TH1F*)File_InPut_Raw->Get("h_mCentrality_nocuts");
  TH1F *h_mCentrality = (TH1F*)File_InPut_Raw->Get("h_mCentrality");

  TCanvas *c_global = new TCanvas("c_global","c_global",10,10,1600,800);
  c_global->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_global->cd(i_pad+1);
    c_global->cd(i_pad+1)->SetLeftMargin(0.15);
    c_global->cd(i_pad+1)->SetBottomMargin(0.15);
    c_global->cd(i_pad+1)->SetTicks(1,1);
    c_global->cd(i_pad+1)->SetGrid(0,0);
  }

  c_global->cd(1);
  h_mVtZ_Bbc_nocuts->SetLineColor(2);
  h_mVtZ_Bbc_nocuts->SetMarkerColor(2);
  h_mVtZ_Bbc_nocuts->SetMarkerSize(1.5);
  h_mVtZ_Bbc_nocuts->SetMarkerStyle(24);
  h_mVtZ_Bbc_nocuts->GetXaxis()->SetTitle("BBC Vertex Z");
  h_mVtZ_Bbc_nocuts->Draw("pE");

  h_mVtZ_Bbc->SetLineColor(4);
  h_mVtZ_Bbc->Draw("hE same");

  c_global->cd(2);
  h_mCentrality_nocuts->SetLineColor(2);
  h_mCentrality_nocuts->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentrality_nocuts->Draw("hE");
  
  h_mCentrality->SetLineColor(4);
  h_mCentrality->Draw("hE same");

  c_global->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/c_global.eps");
}

