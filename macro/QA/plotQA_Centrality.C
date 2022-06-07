#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>

void plotQA_Centrality(int run_number = 0)
{
  // int runId[6] = {413748,414154,414702,413267,413080,413092};
  int runIndex[14] = {0,68,114,162,201,264,410,504,617,682,790,893,924,1037};

  // string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/EventPlane/withPhiWeight/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  string inputfile = "/Users/xusun/WorkSpace/PHENIX/Data/EventPlane/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mCentrality_MiniBias = (TH2F*)File_InPut->Get("h_mCentrality_MiniBias");
  // TH2F *h_mCentrality_MiniBias = (TH2F*)File_InPut->Get("h_mCentrality_NarrowVtxBbc");
  int bin = h_mCentrality_MiniBias->GetXaxis()->FindBin(runIndex[run_number]);
  cout << "runIndex = " << runIndex[run_number] << ", bin = " << bin << endl;

  TH1F *h_mCentrality_MiniBias_Proj = (TH1F*)h_mCentrality_MiniBias->ProjectionY("h_mCentrality_MiniBias_Proj",bin,bin);

  TCanvas *c_centrality = new TCanvas("c_centrality","c_centrality",10,10,800,800);
  c_centrality->cd();
  c_centrality->cd()->SetLeftMargin(0.15);
  c_centrality->cd()->SetBottomMargin(0.15);
  c_centrality->cd()->SetTicks(1,1);
  c_centrality->cd()->SetGrid(0,0);
  h_mCentrality_MiniBias_Proj->SetStats(0);
  h_mCentrality_MiniBias_Proj->SetTitle("BBCLL1 & BBC VtxZ (< 30 cm)");
  h_mCentrality_MiniBias_Proj->GetXaxis()->SetTitle("Centrality (%)");
  h_mCentrality_MiniBias_Proj->GetYaxis()->SetRangeUser(0.0,1.2*h_mCentrality_MiniBias_Proj->GetMaximum());
  h_mCentrality_MiniBias_Proj->DrawCopy("hE");
  h_mCentrality_MiniBias_Proj->Draw("hE");

  string leg = Form("runIndex: %d",runIndex[run_number]);
  TLegend *leg_cent = new TLegend(0.2,0.25,0.8,0.40);
  leg_cent->SetBorderSize(0);
  leg_cent->SetFillColor(10);
  leg_cent->AddEntry(h_mCentrality_MiniBias_Proj,leg.c_str(),"l");
  leg_cent->Draw("same");
  
  string FigName = Form("/Users/xusun/WorkSpace/PHENIX/figures/QA_Centrality/c_centrality_%d.eps",runIndex[run_number]);
  c_centrality->SaveAs(FigName.c_str());
}

