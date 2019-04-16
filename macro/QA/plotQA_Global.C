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
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mVtZ_Bbc_nocuts = (TH1F*)File_InPut->Get("h_mVtZ_Bbc_nocuts");
  TH1F *h_mVtZ_Bbc = (TH1F*)File_InPut->Get("h_mVtZ_Bbc");
  TH1F *h_mVtZ_Zdc_nocuts = (TH1F*)File_InPut->Get("h_mVtZ_Zdc_nocuts");
  TH1F *h_mVtZ_Zdc = (TH1F*)File_InPut->Get("h_mVtZ_Zdc");
  TH1F *h_mCentrality_nocuts = (TH1F*)File_InPut->Get("h_mCentrality_nocuts");
  TH1F *h_mCentrality = (TH1F*)File_InPut->Get("h_mCentrality");

  TCanvas *c_global = new TCanvas("c_global","c_global",10,10,1500,500);
  c_global->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_global->cd(i_pad+1);
    c_global->cd(i_pad+1)->SetLeftMargin(0.15);
    c_global->cd(i_pad+1)->SetBottomMargin(0.15);
    c_global->cd(i_pad+1)->SetTicks(1,1);
    c_global->cd(i_pad+1)->SetGrid(0,0);
  }

  c_global->cd(1);
  h_mVtZ_Bbc_nocuts->SetTitle("BBC Vertex");
  h_mVtZ_Bbc_nocuts->SetStats(0);
  h_mVtZ_Bbc_nocuts->SetLineColor(2);
  h_mVtZ_Bbc_nocuts->SetLineWidth(4);
  h_mVtZ_Bbc_nocuts->SetLineStyle(1);
  h_mVtZ_Bbc_nocuts->GetXaxis()->SetTitle("BBC Vertex Z");
  h_mVtZ_Bbc_nocuts->Draw("hE");
  h_mVtZ_Bbc->SetLineColor(4);
  h_mVtZ_Bbc->SetLineWidth(4);
  h_mVtZ_Bbc->SetLineStyle(2);
  h_mVtZ_Bbc->Draw("hE same");
  
  c_global->cd(2);
  h_mVtZ_Zdc_nocuts->SetTitle("ZDC Vertex");
  h_mVtZ_Zdc_nocuts->SetStats(0);
  h_mVtZ_Zdc_nocuts->SetLineColor(2);
  h_mVtZ_Zdc_nocuts->SetLineWidth(4);
  h_mVtZ_Zdc_nocuts->SetLineStyle(1);
  h_mVtZ_Zdc_nocuts->GetXaxis()->SetTitle("ZDC Vertex Z");
  h_mVtZ_Zdc_nocuts->Draw("hE");
  h_mVtZ_Zdc->SetLineColor(4);
  h_mVtZ_Zdc->SetLineWidth(4);
  h_mVtZ_Zdc->SetLineStyle(2);
  h_mVtZ_Zdc->Draw("hE same");

  c_global->cd(3);
  h_mCentrality_nocuts->SetTitle("Centrality");
  h_mCentrality_nocuts->SetStats(0);
  h_mCentrality_nocuts->SetLineColor(2);
  h_mCentrality_nocuts->SetLineWidth(4);
  h_mCentrality_nocuts->SetLineStyle(1);
  h_mCentrality_nocuts->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentrality_nocuts->Draw("hE");
  h_mCentrality->SetLineColor(4);
  h_mCentrality->SetLineWidth(4);
  h_mCentrality->SetLineStyle(2);
  h_mCentrality->Draw("hE same");

  TLegend *leg = new TLegend(0.2,0.3,0.7,0.5);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(h_mCentrality_nocuts,"w/o BBC cuts","l");
  leg->AddEntry(h_mCentrality,"with BBC cuts (< 10cm)","l");
  leg->Draw("same");
  
  c_global->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_global.eps");
}

