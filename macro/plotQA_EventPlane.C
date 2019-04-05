#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>

void plotQA_EventPlane()
{
  string input_rawEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_Raw = TFile::Open(input_rawEP.c_str());
  TH1F *h_mEPRaw_BbcNorth_2nd_Centrality_2 = (TH1F*)File_InPut_Raw->Get("h_mEPRaw_BbcNorth_2nd_Centrality_2");
  TH2F *h_mEPRaw_Correlation_2nd_Centrality_2 = (TH2F*)File_InPut_Raw->Get("h_mEPRaw_Correlation_2nd_Centrality_2");

  string input_recenterEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut_ReCenter = TFile::Open(input_recenterEP.c_str());
  TH1F *h_mEPReCenter_BbcNorth_2nd_Centrality_2 = (TH1F*)File_InPut_ReCenter->Get("h_mEPReCenter_BbcNorth_2nd_Centrality_2");
  TH2F *h_mEPReCenter_Correlation_2nd_Centrality_2 = (TH2F*)File_InPut_ReCenter->Get("h_mEPReCenter_Correlation_2nd_Centrality_2");

  string input_shiftEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ShiftEP_Resoluiton.root";
  TFile *File_InPut_Shift = TFile::Open(input_shiftEP.c_str());
  TH1F *h_mEPShift_BbcNorth_2nd_Centrality_2 = (TH1F*)File_InPut_Shift->Get("h_mEPShift_BbcNorth_2nd_Centrality_2");
  TH2F *h_mEPShift_Correlation_2nd_Centrality_2 = (TH2F*)File_InPut_Shift->Get("h_mEPShift_Correlation_2nd_Centrality_2");

  TCanvas *c_EventPlane = new TCanvas("c_EventPlane","c_EventPlane",10,10,800,800);
  c_EventPlane->cd();
  c_EventPlane->cd()->SetLeftMargin(0.15);
  c_EventPlane->cd()->SetBottomMargin(0.15);
  c_EventPlane->cd()->SetTicks(1,1);
  c_EventPlane->cd()->SetGrid(0,0);

  h_mEPRaw_BbcNorth_2nd_Centrality_2->SetStats(0);
  h_mEPRaw_BbcNorth_2nd_Centrality_2->SetLineColor(1);
  h_mEPRaw_BbcNorth_2nd_Centrality_2->GetXaxis()->SetTitle("#Psi_{2}^{North}");
  h_mEPRaw_BbcNorth_2nd_Centrality_2->Draw("hE");

  h_mEPReCenter_BbcNorth_2nd_Centrality_2->SetLineColor(2);
  h_mEPReCenter_BbcNorth_2nd_Centrality_2->Draw("hE same");

  h_mEPShift_BbcNorth_2nd_Centrality_2->SetLineColor(4);
  h_mEPShift_BbcNorth_2nd_Centrality_2->Draw("hE same");

  TF1 *f_poly = new TF1("f_poly","pol0",-TMath::Pi()/2.0,TMath::Pi()/2.0);
  h_mEPShift_BbcNorth_2nd_Centrality_2->Fit(f_poly,"N");
  f_poly->SetLineColor(4);
  f_poly->Draw("l same");
  float chi2 = f_poly->GetChisquare();
  float ndf = f_poly->GetNDF();
  string Chi2_NDF = Form("#chi^{2}/ndf = %3.1f/%3.1f",chi2,ndf);

  TLegend *leg = new TLegend(0.4,0.6,0.8,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(h_mEPRaw_BbcNorth_2nd_Centrality_2,"Raw Event Plane","l");
  leg->AddEntry(h_mEPReCenter_BbcNorth_2nd_Centrality_2,"ReCentered Event Plane","l");
  leg->AddEntry(h_mEPShift_BbcNorth_2nd_Centrality_2,"Shifted Event Plane","l");
  leg->AddEntry(f_poly,Chi2_NDF.c_str(),"l");
  leg->Draw();
  c_EventPlane->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/c_EventPlane.eps");


  TCanvas *c_correlation = new TCanvas("c_correlation","c_correlation",10,10,1500,500);
  c_correlation->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_correlation->cd(i_pad+1);
    c_correlation->cd(i_pad+1)->SetLeftMargin(0.15);
    c_correlation->cd(i_pad+1)->SetBottomMargin(0.15);
    c_correlation->cd(i_pad+1)->SetTicks(1,1);
    c_correlation->cd(i_pad+1)->SetGrid(0,0);
  }
  
  c_correlation->cd(1);
  h_mEPRaw_Correlation_2nd_Centrality_2->SetStats(0);
  h_mEPRaw_Correlation_2nd_Centrality_2->GetXaxis()->SetTitle("#Psi_{2}^{South}");
  h_mEPRaw_Correlation_2nd_Centrality_2->GetXaxis()->SetTitleSize(0.06);
  h_mEPRaw_Correlation_2nd_Centrality_2->GetXaxis()->CenterTitle();

  h_mEPRaw_Correlation_2nd_Centrality_2->GetYaxis()->SetTitle("#Psi_{2}^{North}");
  h_mEPRaw_Correlation_2nd_Centrality_2->GetYaxis()->SetTitleSize(0.06);
  h_mEPRaw_Correlation_2nd_Centrality_2->GetYaxis()->CenterTitle();
  h_mEPRaw_Correlation_2nd_Centrality_2->Draw("colz");
  
  c_correlation->cd(2);
  h_mEPReCenter_Correlation_2nd_Centrality_2->SetStats(0);
  h_mEPReCenter_Correlation_2nd_Centrality_2->GetXaxis()->SetTitle("#Psi_{2}^{South}");
  h_mEPReCenter_Correlation_2nd_Centrality_2->GetXaxis()->SetTitleSize(0.06);
  h_mEPReCenter_Correlation_2nd_Centrality_2->GetXaxis()->CenterTitle();

  h_mEPReCenter_Correlation_2nd_Centrality_2->GetYaxis()->SetTitle("#Psi_{2}^{North}");
  h_mEPReCenter_Correlation_2nd_Centrality_2->GetYaxis()->SetTitleSize(0.06);
  h_mEPReCenter_Correlation_2nd_Centrality_2->GetYaxis()->CenterTitle();
  h_mEPReCenter_Correlation_2nd_Centrality_2->Draw("colz");
  
  c_correlation->cd(3);
  h_mEPShift_Correlation_2nd_Centrality_2->SetStats(0);
  h_mEPShift_Correlation_2nd_Centrality_2->GetXaxis()->SetTitle("#Psi_{2}^{South}");
  h_mEPShift_Correlation_2nd_Centrality_2->GetXaxis()->SetTitleSize(0.06);
  h_mEPShift_Correlation_2nd_Centrality_2->GetXaxis()->CenterTitle();

  h_mEPShift_Correlation_2nd_Centrality_2->GetYaxis()->SetTitle("#Psi_{2}^{North}");
  h_mEPShift_Correlation_2nd_Centrality_2->GetYaxis()->SetTitleSize(0.06);
  h_mEPShift_Correlation_2nd_Centrality_2->GetYaxis()->CenterTitle();
  h_mEPShift_Correlation_2nd_Centrality_2->Draw("colz");

  c_correlation->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/c_correlation.eps");
}

