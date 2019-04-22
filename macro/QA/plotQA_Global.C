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
  // string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mVtxZ_Bbc_MiniBias = (TH2F*)File_InPut->Get("h_mVtxZ_Bbc_MiniBias");
  TH2F *h_mVtxZ_Zdc_MiniBias = (TH2F*)File_InPut->Get("h_mVtxZ_Zdc_MiniBias");
  TH2F *h_mCentrality_MiniBias = (TH2F*)File_InPut->Get("h_mCentrality_MiniBias");

  TH2F *h_mVtxZ_Bbc_NarrowVtx = (TH2F*)File_InPut->Get("h_mVtxZ_Bbc_NarrowVtx");
  TH2F *h_mVtxZ_Zdc_NarrowVtx = (TH2F*)File_InPut->Get("h_mVtxZ_Zdc_NarrowVtx");
  TH2F *h_mCentrality_NarrowVtx = (TH2F*)File_InPut->Get("h_mCentrality_NarrowVtx");

  TH2F *h_mVtxZ_Bbc_NarrowVtxBbc = (TH2F*)File_InPut->Get("h_mVtxZ_Bbc_NarrowVtxBbc");
  TH2F *h_mVtxZ_Zdc_NarrowVtxBbc = (TH2F*)File_InPut->Get("h_mVtxZ_Zdc_NarrowVtxBbc");
  TH2F *h_mCentrality_NarrowVtxBbc = (TH2F*)File_InPut->Get("h_mCentrality_NarrowVtxBbc");

  TH1F *h_mVtxZ_Bbc_MiniBias_ProjY = (TH1F*)h_mVtxZ_Bbc_MiniBias->ProjectionY();
  TH1F *h_mVtxZ_Zdc_MiniBias_ProjY = (TH1F*)h_mVtxZ_Zdc_MiniBias->ProjectionY();
  TH1F *h_mCentrality_MiniBias_ProjY = (TH1F*)h_mCentrality_MiniBias->ProjectionY();

  TH1F *h_mVtxZ_Bbc_NarrowVtx_ProjY = (TH1F*)h_mVtxZ_Bbc_NarrowVtx->ProjectionY();
  TH1F *h_mVtxZ_Zdc_NarrowVtx_ProjY = (TH1F*)h_mVtxZ_Zdc_NarrowVtx->ProjectionY();
  TH1F *h_mCentrality_NarrowVtx_ProjY = (TH1F*)h_mCentrality_NarrowVtx->ProjectionY();

  TH1F *h_mVtxZ_Bbc_NarrowVtxBbc_ProjY = (TH1F*)h_mVtxZ_Bbc_NarrowVtxBbc->ProjectionY();
  TH1F *h_mVtxZ_Zdc_NarrowVtxBbc_ProjY = (TH1F*)h_mVtxZ_Zdc_NarrowVtxBbc->ProjectionY();
  TH1F *h_mCentrality_NarrowVtxBbc_ProjY = (TH1F*)h_mCentrality_NarrowVtxBbc->ProjectionY();

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
  c_global->cd(1)->SetLogy();
  h_mVtxZ_Bbc_NarrowVtx_ProjY->SetTitle("BBC Vertex");
  h_mVtxZ_Bbc_NarrowVtx_ProjY->SetStats(0);
  h_mVtxZ_Bbc_NarrowVtx_ProjY->SetLineColor(2);
  h_mVtxZ_Bbc_NarrowVtx_ProjY->SetLineWidth(2);
  h_mVtxZ_Bbc_NarrowVtx_ProjY->SetLineStyle(1);
  h_mVtxZ_Bbc_NarrowVtx_ProjY->GetXaxis()->SetTitle("BBC Vertex Z");
  h_mVtxZ_Bbc_NarrowVtx_ProjY->Draw("hE");
  h_mVtxZ_Bbc_NarrowVtxBbc_ProjY->SetLineColor(4);
  h_mVtxZ_Bbc_NarrowVtxBbc_ProjY->SetLineWidth(2);
  h_mVtxZ_Bbc_NarrowVtxBbc_ProjY->SetLineStyle(1);
  h_mVtxZ_Bbc_NarrowVtxBbc_ProjY->Draw("hE same");
  h_mVtxZ_Bbc_MiniBias_ProjY->SetLineColor(1);
  h_mVtxZ_Bbc_MiniBias_ProjY->SetLineWidth(2);
  h_mVtxZ_Bbc_MiniBias_ProjY->SetLineStyle(1);
  h_mVtxZ_Bbc_MiniBias_ProjY->Draw("hE same");
  
  c_global->cd(2);
  c_global->cd(2)->SetLogy();
  h_mVtxZ_Zdc_NarrowVtx_ProjY->SetTitle("ZDC Vertex");
  h_mVtxZ_Zdc_NarrowVtx_ProjY->SetStats(0);
  h_mVtxZ_Zdc_NarrowVtx_ProjY->SetLineColor(2);
  h_mVtxZ_Zdc_NarrowVtx_ProjY->SetLineWidth(2);
  h_mVtxZ_Zdc_NarrowVtx_ProjY->SetLineStyle(1);
  h_mVtxZ_Zdc_NarrowVtx_ProjY->GetXaxis()->SetTitle("ZDC Vertex Z");
  h_mVtxZ_Zdc_NarrowVtx_ProjY->Draw("hE");
  h_mVtxZ_Zdc_NarrowVtxBbc_ProjY->SetLineColor(4);
  h_mVtxZ_Zdc_NarrowVtxBbc_ProjY->SetLineWidth(2);
  h_mVtxZ_Zdc_NarrowVtxBbc_ProjY->SetLineStyle(1);
  h_mVtxZ_Zdc_NarrowVtxBbc_ProjY->Draw("hE same");
  h_mVtxZ_Zdc_MiniBias_ProjY->SetLineColor(1);
  h_mVtxZ_Zdc_MiniBias_ProjY->SetLineWidth(2);
  h_mVtxZ_Zdc_MiniBias_ProjY->SetLineStyle(1);
  h_mVtxZ_Zdc_MiniBias_ProjY->Draw("hE same");

  c_global->cd(3);
  c_global->cd(3)->SetLogy();
  h_mCentrality_NarrowVtx_ProjY->SetTitle("Centrality");
  h_mCentrality_NarrowVtx_ProjY->SetStats(0);
  h_mCentrality_NarrowVtx_ProjY->SetLineColor(2);
  h_mCentrality_NarrowVtx_ProjY->SetLineWidth(2);
  h_mCentrality_NarrowVtx_ProjY->SetLineStyle(1);
  h_mCentrality_NarrowVtx_ProjY->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentrality_NarrowVtx_ProjY->GetYaxis()->SetRangeUser(0.1,1.5*h_mCentrality_NarrowVtx_ProjY->GetMaximum());
  h_mCentrality_NarrowVtx_ProjY->Draw("hE");
  h_mCentrality_NarrowVtxBbc_ProjY->SetLineColor(4);
  h_mCentrality_NarrowVtxBbc_ProjY->SetLineWidth(2);
  h_mCentrality_NarrowVtxBbc_ProjY->SetLineStyle(1);
  h_mCentrality_NarrowVtxBbc_ProjY->Draw("hE same");
  h_mCentrality_MiniBias_ProjY->SetLineColor(1);
  h_mCentrality_MiniBias_ProjY->SetLineWidth(2);
  h_mCentrality_MiniBias_ProjY->SetLineStyle(1);
  h_mCentrality_MiniBias_ProjY->Draw("hE same");

  TLegend *leg = new TLegend(0.2,0.25,0.8,0.55);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(h_mCentrality_MiniBias_ProjY,"BBCLL1 & BBC VtxZ (< 30 cm)","l");
  leg->AddEntry(h_mCentrality_NarrowVtx_ProjY,"BBCLL1 narrowvtx & BBC VtxZ (< 30 cm)","l");
  leg->AddEntry(h_mCentrality_NarrowVtxBbc_ProjY,"BBCLL1 narrowvtx & BBC VtxZ (< 10 cm)","l");
  leg->Draw("same");
  
  c_global->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_global.eps");
}

