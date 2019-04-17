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

void plotQA_EventPlane(int order = 1, int cent10 = 5)
{
  const string mOrder[3] = {"1st","2nd","3rd"};
  const string mCentrality = Form("%d-%d%%",vecMesonFlow::mCentStart10[cent10],vecMesonFlow::mCentStop10[cent10]);

  string input_rawEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_Raw = TFile::Open(input_rawEP.c_str());
  string HistName_Raw;
  HistName_Raw  = Form("h_mEPRaw_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPRaw_BbcSouth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str());
  HistName_Raw  = Form("h_mEPRaw_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPRaw_BbcNorth = (TH1F*)File_InPut_Raw->Get(HistName_Raw.c_str());

  string input_recenterEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut_ReCenter = TFile::Open(input_recenterEP.c_str());
  string HistName_ReCenter;
  HistName_ReCenter = Form("h_mEPReCenter_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPReCenter_BbcSouth = (TH1F*)File_InPut_ReCenter->Get(HistName_ReCenter.c_str());
  HistName_ReCenter = Form("h_mEPReCenter_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPReCenter_BbcNorth = (TH1F*)File_InPut_ReCenter->Get(HistName_ReCenter.c_str());

  string input_shiftEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  TFile *File_InPut_Shift = TFile::Open(input_shiftEP.c_str());
  string HistName_Shift;
  HistName_Shift = Form("h_mEPShift_BbcSouth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPShift_BbcSouth = (TH1F*)File_InPut_Shift->Get(HistName_Shift.c_str());
  HistName_Shift = Form("h_mEPShift_BbcNorth_%s_Centrality_%d",mOrder[order].c_str(),cent10);
  TH1F *h_mEPShift_BbcNorth = (TH1F*)File_InPut_Shift->Get(HistName_Shift.c_str());

  string title;
  TCanvas *c_EventPlane = new TCanvas("c_EventPlane","c_EventPlane",10,10,1600,800);
  c_EventPlane->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_EventPlane->cd(i_pad+1);
    c_EventPlane->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EventPlane->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EventPlane->cd(i_pad+1)->SetTicks(1,1);
    c_EventPlane->cd(i_pad+1)->SetGrid(0,0);
  }

  c_EventPlane->cd(1);

  title = Form("%s EP BBC South @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEPRaw_BbcSouth->SetTitle(title.c_str());
  h_mEPRaw_BbcSouth->SetStats(0);
  title = Form("#Psi_{%s}^{South}",mOrder[order].c_str());
  h_mEPRaw_BbcSouth->GetXaxis()->SetTitle(title.c_str());
  h_mEPRaw_BbcSouth->GetXaxis()->CenterTitle();
  h_mEPRaw_BbcSouth->GetXaxis()->SetTitleSize(0.06);
  h_mEPRaw_BbcSouth->GetXaxis()->SetTitleFont(132);
  h_mEPRaw_BbcSouth->GetXaxis()->SetLabelSize(0.04);
  h_mEPRaw_BbcSouth->SetNdivisions(505,"X");
  h_mEPRaw_BbcSouth->GetYaxis()->CenterTitle();
  h_mEPRaw_BbcSouth->GetYaxis()->SetTitleSize(0.06);
  h_mEPRaw_BbcSouth->GetYaxis()->SetTitleFont(132);
  h_mEPRaw_BbcSouth->GetYaxis()->SetLabelSize(0.04);
  // h_mEPRaw_BbcSouth->GetYaxis()->SetRangeUser(0.0,h_mEPRaw_BbcSouth->GetMaximum()*1.1);
  h_mEPRaw_BbcSouth->SetNdivisions(505,"Y");
  h_mEPRaw_BbcSouth->SetLineColor(1);
  h_mEPRaw_BbcSouth->SetLineWidth(2);
  h_mEPRaw_BbcSouth->SetLineStyle(1);
  h_mEPRaw_BbcSouth->Draw("hE");

  h_mEPReCenter_BbcSouth->SetLineColor(2);
  h_mEPReCenter_BbcSouth->SetLineWidth(2);
  h_mEPReCenter_BbcSouth->Draw("hE same");

  h_mEPShift_BbcSouth->SetLineColor(4);
  h_mEPShift_BbcSouth->SetLineWidth(2);
  h_mEPShift_BbcSouth->Draw("hE same");

  TF1 *f_poly_South = new TF1("f_poly_South","pol0",-TMath::Pi()/(order+1.0),TMath::Pi()/(order+1.0));
  h_mEPShift_BbcSouth->Fit(f_poly_South,"N");
  f_poly_South->SetLineColor(4);
  f_poly_South->SetLineStyle(2);
  f_poly_South->Draw("l same");
  float chi2_South = f_poly_South->GetChisquare();
  float ndf_South = f_poly_South->GetNDF();
  string Chi2_NDF_South = Form("#chi^{2}/ndf = %3.1f/%3.1f",chi2_South,ndf_South);

  TLegend *leg_South = new TLegend(0.35,0.6,0.75,0.8);
  leg_South->SetBorderSize(0);
  leg_South->SetFillColor(0);
  leg_South->AddEntry(h_mEPRaw_BbcSouth,"Raw Event Plane BBC South","l");
  leg_South->AddEntry(h_mEPReCenter_BbcSouth,"ReCentered Event Plane BBC South","l");
  leg_South->AddEntry(h_mEPShift_BbcSouth,"Shifted Event Plane BBC South","l");
  leg_South->AddEntry(f_poly_South,Chi2_NDF_South.c_str(),"l");
  leg_South->Draw("same");

  c_EventPlane->cd(2);

  title = Form("%s EP BBC North @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEPRaw_BbcNorth->SetTitle(title.c_str());
  h_mEPRaw_BbcNorth->SetStats(0);
  title = Form("#Psi_{%s}^{North}",mOrder[order].c_str());
  h_mEPRaw_BbcNorth->GetXaxis()->SetTitle(title.c_str());
  h_mEPRaw_BbcNorth->GetXaxis()->CenterTitle();
  h_mEPRaw_BbcNorth->GetXaxis()->SetTitleSize(0.06);
  h_mEPRaw_BbcNorth->GetXaxis()->SetTitleFont(132);
  h_mEPRaw_BbcNorth->GetXaxis()->SetLabelSize(0.04);
  h_mEPRaw_BbcNorth->SetNdivisions(505,"X");
  h_mEPRaw_BbcNorth->GetYaxis()->CenterTitle();
  h_mEPRaw_BbcNorth->GetYaxis()->SetTitleSize(0.06);
  h_mEPRaw_BbcNorth->GetYaxis()->SetTitleFont(132);
  h_mEPRaw_BbcNorth->GetYaxis()->SetLabelSize(0.04);
  // h_mEPRaw_BbcNorth->GetYaxis()->SetRangeUser(0.0,h_mEPRaw_BbcNorth->GetMaximum()*1.1);
  h_mEPRaw_BbcNorth->SetNdivisions(505,"Y");
  h_mEPRaw_BbcNorth->SetLineColor(1);
  h_mEPRaw_BbcNorth->SetLineWidth(2);
  h_mEPRaw_BbcNorth->SetLineStyle(1);
  h_mEPRaw_BbcNorth->Draw("hE");

  h_mEPReCenter_BbcNorth->SetLineColor(2);
  h_mEPReCenter_BbcNorth->SetLineWidth(2);
  h_mEPReCenter_BbcNorth->Draw("hE same");

  h_mEPShift_BbcNorth->SetLineColor(4);
  h_mEPShift_BbcNorth->SetLineWidth(2);
  h_mEPShift_BbcNorth->Draw("hE same");

  TF1 *f_poly_North = new TF1("f_poly_North","pol0",-TMath::Pi()/(order+1.0),TMath::Pi()/(order+1.0));
  h_mEPShift_BbcNorth->Fit(f_poly_North,"N");
  f_poly_North->SetLineColor(4);
  f_poly_North->Draw("l same");
  float chi2_North = f_poly_North->GetChisquare();
  float ndf_North = f_poly_North->GetNDF();
  string Chi2_NDF_North = Form("#chi^{2}/ndf = %3.1f/%3.1f",chi2_North,ndf_North);

  TLegend *leg_North = new TLegend(0.35,0.6,0.75,0.8);
  leg_North->SetBorderSize(0);
  leg_North->SetFillColor(0);
  leg_North->AddEntry(h_mEPRaw_BbcNorth,"Raw Event Plane BBC North","l");
  leg_North->AddEntry(h_mEPReCenter_BbcNorth,"ReCentered Event Plane BBC North","l");
  leg_North->AddEntry(h_mEPShift_BbcNorth,"Shifted Event Plane BBC North","l");
  leg_North->AddEntry(f_poly_North,Chi2_NDF_North.c_str(),"l");
  leg_North->Draw("same");

  string FigName = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_EventPlane_%s.eps",mOrder[order].c_str());
  c_EventPlane->SaveAs(FigName.c_str());
}
