#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>

void plotQA_CentCom()
{
  string input_old = "/Users/xusun/WorkSpace/PHENIX/Data/EventPlane/withPhiWeight/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_Old = TFile::Open(input_old.c_str());
  TH2F *h_mCentOld_NarrowVtxBbc = (TH2F*)File_Old->Get("h_mCentrality_NarrowVtxBbc");
  TH1F *h_mCentOld_NarrowVtxBbc_Proj = (TH1F*)h_mCentOld_NarrowVtxBbc->ProjectionY("h_mCentrality_MiniBias_Proj");

  string input_new = "/Users/xusun/WorkSpace/PHENIX/Data/EventPlane/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_New = TFile::Open(input_new.c_str());
  TH2F *h_mCentNew_NarrowVtxBbc = (TH2F*)File_New->Get("h_mCentrality_NarrowVtxBbc");
  TH1F *h_mCentNew_NarrowVtxBbc_Proj = (TH1F*)h_mCentNew_NarrowVtxBbc->ProjectionY("h_mCentrality_MiniBias_Proj");

  TCanvas *c_centrality = new TCanvas("c_centrality","c_centrality",10,10,1600,800);
  c_centrality->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_centrality->cd(i_pad+1);
    c_centrality->cd(i_pad+1)->SetLeftMargin(0.15);
    c_centrality->cd(i_pad+1)->SetBottomMargin(0.15);
    c_centrality->cd(i_pad+1)->SetTicks(1,1);
    c_centrality->cd(i_pad+1)->SetGrid(0,0);
  }

  c_centrality->cd(1);
  h_mCentOld_NarrowVtxBbc_Proj->SetStats(0);
  h_mCentOld_NarrowVtxBbc_Proj->SetTitle("BBCLL1 narrowvtx & BBC VtxZ (< 10 cm)");
  h_mCentOld_NarrowVtxBbc_Proj->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentOld_NarrowVtxBbc_Proj->GetYaxis()->SetRangeUser(0.0,1.2*h_mCentOld_NarrowVtxBbc_Proj->GetMaximum());
  h_mCentOld_NarrowVtxBbc_Proj->DrawCopy("hE");

  TLegend *leg_old = new TLegend(0.2,0.25,0.8,0.40);
  leg_old->SetBorderSize(0);
  leg_old->SetFillColor(10);
  leg_old->AddEntry(h_mCentOld_NarrowVtxBbc_Proj,"old run lists","l");
  leg_old->Draw("same");

  c_centrality->cd(2);
  h_mCentNew_NarrowVtxBbc_Proj->SetStats(0);
  h_mCentNew_NarrowVtxBbc_Proj->SetTitle("BBCLL1 narrowvtx & BBC VtxZ (< 10 cm)");
  h_mCentNew_NarrowVtxBbc_Proj->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentNew_NarrowVtxBbc_Proj->GetYaxis()->SetRangeUser(0.0,1.2*h_mCentNew_NarrowVtxBbc_Proj->GetMaximum());
  h_mCentNew_NarrowVtxBbc_Proj->DrawCopy("hE");

  TLegend *leg_new = new TLegend(0.2,0.25,0.8,0.40);
  leg_new->SetBorderSize(0);
  leg_new->SetFillColor(10);
  leg_new->AddEntry(h_mCentNew_NarrowVtxBbc_Proj,"new run lists","l");
  leg_new->Draw("same");
  
  c_centrality->SaveAs("/Users/xusun/WorkSpace/PHENIX/figures/QA_Centrality/c_centrality_comparison.eps");
}

