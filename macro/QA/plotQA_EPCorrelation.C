#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

void plotQA_EPCorrelation(int order = 1, int cent20 = 1)
{
  const string mOrder[3] = {"1st","2nd","3rd"};
  const string mCentrality = Form("%1.1f-%1.1f%%",vecMesonFlow::mCentStart20[cent20],vecMesonFlow::mCentStop20[cent20]);

  // string input_rawEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/noPhiWeight/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  string input_rawEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_Raw = TFile::Open(input_rawEP.c_str());
  string HistName_Raw = Form("h_mEPRaw_Correlation_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH2F *h_mEPRaw_Correlation = (TH2F*)File_InPut_Raw->Get(HistName_Raw.c_str());

  string input_recenterEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ReCenterEP_ShiftPar.root";
  // string input_recenterEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/noPhiWeight/taxi_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut_ReCenter = TFile::Open(input_recenterEP.c_str());
  string HistName_ReCenter = Form("h_mEPReCenter_Correlation_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH2F *h_mEPReCenter_Correlation = (TH2F*)File_InPut_ReCenter->Get(HistName_ReCenter.c_str());

  // string input_shiftEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/noPhiWeight/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  string input_shiftEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_ShiftEP_Resoluiton.root";
  TFile *File_InPut_Shift = TFile::Open(input_shiftEP.c_str());
  string HistName_Shift = Form("h_mEPShift_Correlation_%s_Centrality_%d",mOrder[order].c_str(),cent20);
  TH2F *h_mEPShift_Correlation = (TH2F*)File_InPut_Shift->Get(HistName_Shift.c_str());

  string title;
  string titleX = Form("#Psi_{%s}^{Soutth}",mOrder[order].c_str());
  string titleY = Form("#Psi_{%s}^{North}",mOrder[order].c_str());
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
  title = Form("%s Raw EP Correlation @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEPRaw_Correlation->SetTitle(title.c_str());
  h_mEPRaw_Correlation->SetStats(0);
  h_mEPRaw_Correlation->GetXaxis()->SetTitle(titleX.c_str());
  h_mEPRaw_Correlation->GetXaxis()->SetTitleSize(0.06);
  h_mEPRaw_Correlation->GetXaxis()->CenterTitle();

  h_mEPRaw_Correlation->GetYaxis()->SetTitle(titleY.c_str());
  h_mEPRaw_Correlation->GetYaxis()->SetTitleSize(0.06);
  h_mEPRaw_Correlation->GetYaxis()->CenterTitle();
  h_mEPRaw_Correlation->Draw("colz");
  
  c_correlation->cd(2);
  title = Form("%s ReCentered EP Correlation @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEPReCenter_Correlation->SetTitle(title.c_str());
  h_mEPReCenter_Correlation->SetStats(0);
  h_mEPReCenter_Correlation->GetXaxis()->SetTitle(titleX.c_str());
  h_mEPReCenter_Correlation->GetXaxis()->SetTitleSize(0.06);
  h_mEPReCenter_Correlation->GetXaxis()->CenterTitle();

  h_mEPReCenter_Correlation->GetYaxis()->SetTitle(titleY.c_str());
  h_mEPReCenter_Correlation->GetYaxis()->SetTitleSize(0.06);
  h_mEPReCenter_Correlation->GetYaxis()->CenterTitle();
  h_mEPReCenter_Correlation->Draw("colz");
  
  c_correlation->cd(3);
  title = Form("%s Shifted EP Correlation @ %s",mOrder[order].c_str(),mCentrality.c_str());
  h_mEPShift_Correlation->SetTitle(title.c_str());
  h_mEPShift_Correlation->SetStats(0);
  h_mEPShift_Correlation->GetXaxis()->SetTitle(titleX.c_str());
  h_mEPShift_Correlation->GetXaxis()->SetTitleSize(0.06);
  h_mEPShift_Correlation->GetXaxis()->CenterTitle();

  h_mEPShift_Correlation->GetYaxis()->SetTitle(titleY.c_str());
  h_mEPShift_Correlation->GetYaxis()->SetTitleSize(0.06);
  h_mEPShift_Correlation->GetYaxis()->CenterTitle();
  h_mEPShift_Correlation->Draw("colz");

  string FigName = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_EPCorrelation_%s_%d_good.eps",mOrder[order].c_str(),cent20);
  // string FigName = Form("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/QA_BBC/EventPlane/c_EPCorrelation_%s_%d_woPhiWeight.eps",mOrder[order].c_str(),cent20);
  c_correlation->SaveAs(FigName.c_str());
}

