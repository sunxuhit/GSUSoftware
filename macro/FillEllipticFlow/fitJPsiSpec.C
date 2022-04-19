#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <string>
#include <map>
#include "./functions.h"
#include "./draw.h"
#include "../../FillEllipticFlow/PhVecMesonCons.h"

typedef std::map<std::string,TH1F*> TH1FMap;

using namespace std;

string genKey_Spec(int pid, int mode, int pt_bin, int cent4);

void fitJPsiSpec(const int pt_QA = 1, const int cent_QA = 3)
{
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(300000); 

  // string input_US = "/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/OldBin/Yields_200GeV_JPsi_US.root";
  string input_US = "/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/Yields_200GeV_JPsi_US.root";
  TFile *File_US = TFile::Open(input_US.c_str());
  TH1FMap h_mSpec_US;

  // string input_LS = "/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/OldBin/Yields_200GeV_JPsi_LS.root";
  string input_LS = "/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/Yields_200GeV_JPsi_LS.root";
  TFile *File_LS = TFile::Open(input_LS.c_str());
  TH1FMap h_mSpec_LS;

  // histograms for pt spectra
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      string KEY_Spec_US = genKey_Spec(0, 0, i_pt, i_cent);
      h_mSpec_US[KEY_Spec_US] = (TH1F*)File_US->Get(KEY_Spec_US.c_str())->Clone();
      cout << "read in: " << KEY_Spec_US.c_str() << endl;

      string KEY_Spec_LS = genKey_Spec(0, 1, i_pt, i_cent);
      h_mSpec_LS[KEY_Spec_LS] = (TH1F*)File_LS->Get(KEY_Spec_LS.c_str())->Clone();
      cout << "read in: " << KEY_Spec_LS.c_str() << endl;
    }
  }

  double fit_start = 1.5;
  double fit_stop  = 5.0; 
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->SetLogy();
  c_play->SetLeftMargin(0.15);
  c_play->SetBottomMargin(0.15);
  c_play->SetTicks(1,1);
  c_play->SetGrid(0,0);

  // fit like-sign bkg
  string KEY_Spec_LS = genKey_Spec(0, 1, pt_QA, cent_QA);
  h_mSpec_LS[KEY_Spec_LS]->SetTitle("J/#Psi-> #mu^{+}+#mu^{-}");
  h_mSpec_LS[KEY_Spec_LS]->SetStats(0);
  h_mSpec_LS[KEY_Spec_LS]->GetXaxis()->SetRangeUser(1.5,5.0);
  h_mSpec_LS[KEY_Spec_LS]->SetFillColor(kGray);
  h_mSpec_LS[KEY_Spec_LS]->SetFillStyle(3002);
  h_mSpec_LS[KEY_Spec_LS]->SetLineColor(kGray+2);
  h_mSpec_LS[KEY_Spec_LS]->Draw("hE");

  double p0_LS_init = -0.00214;
  double p1_LS_init = -0.03;
  double p2_LS_init = 1.5;
  double p3_LS_init = 4.88;
  double p4_LS_init = -7.88;

  TF1 *f_bkg_LS = new TF1("f_bkg_LS",func_bkg_LS,1.5,5.0,5);
  f_bkg_LS->SetParameter(0, p0_LS_init);
  f_bkg_LS->SetParameter(1, p1_LS_init);
  f_bkg_LS->SetParameter(2, p2_LS_init);
  f_bkg_LS->SetParameter(3, p3_LS_init);
  f_bkg_LS->SetParameter(4, p4_LS_init);
  f_bkg_LS->SetRange(fit_start,fit_stop);
  h_mSpec_LS[KEY_Spec_LS]->Fit(f_bkg_LS,"MNR");

  double p0_LS = f_bkg_LS->GetParameter(0);
  double p1_LS = f_bkg_LS->GetParameter(1);
  double p2_LS = f_bkg_LS->GetParameter(2);
  double p3_LS = f_bkg_LS->GetParameter(3);
  double p4_LS = f_bkg_LS->GetParameter(4);

  // plot like_sing bkg fit
  f_bkg_LS->SetLineColor(4);
  f_bkg_LS->SetLineWidth(4);
  f_bkg_LS->SetLineStyle(2);
  f_bkg_LS->Draw("l same");

  // fit unlike-sign spectra
  string KEY_Spec_US = genKey_Spec(0, 0, pt_QA, cent_QA);
  h_mSpec_US[KEY_Spec_US]->SetMarkerColor(2);
  h_mSpec_US[KEY_Spec_US]->SetMarkerStyle(24);
  h_mSpec_US[KEY_Spec_US]->SetMarkerSize(1.0);
  h_mSpec_US[KEY_Spec_US]->GetXaxis()->SetRangeUser(1.5,5.0);
  h_mSpec_US[KEY_Spec_US]->Draw("pE same");

  // double coeff=1.5;
  // int binl = h_mSpec_US[KEY_Spec_US]->FindBin(2.6); // 2.8 //2.6
  // int binh = h_mSpec_US[KEY_Spec_US]->FindBin(3.6); // 3.4 // 3.6
  // double binw = h_mSpec_US[KEY_Spec_US]->GetBinWidth(1);
  // double renorm = 1.0/binw;   // (1 / (bin_width of data in GeV) )
  // double N_start = coeff*h_mSpec_US[KEY_Spec_US]->Integral(binl,binh) / renorm;  // 1 // 1.5

  double p0_US_init = 1.2;  // 1.20, 1.50
  double p1_US_init = 80.0; // 70, 90
  double p2_US_init = 3.1;
  double p3_US_init = 0.1;
  // double p4_US_init = N_start;
  // double p5_US_init = N_start/5.0;  //originally set to 10.0
  double p4_US_init = 10.0;
  double p5_US_init = 2.0;  //originally set to 10.0
  double p6_US_init = -2.05;

  TF1 *f_spec_US = new TF1("f_spec_US",func_spec_US,1.5,5,12); // sig + bkg_LS + bkg_Corr
  f_spec_US->SetParameter(0, p0_US_init); f_spec_US->SetParLimits(0, 0.0, 1.3);
  f_spec_US->SetParameter(1, p1_US_init); f_spec_US->SetParLimits(1, 60, 95);
  f_spec_US->SetParameter(2, p2_US_init); 
  // f_spec_US->SetParameter(3, p3_US_init); f_spec_US->SetParLimits(3, 0.0, 0.16499);// sigma    //0.1
  f_spec_US->SetParameter(3, p3_US_init); f_spec_US->SetParLimits(3, 0.0, 0.4);// sigma    //0.1
  f_spec_US->SetParameter(4, p4_US_init);
  f_spec_US->SetParameter(5, p5_US_init);
  f_spec_US->SetParameter(6, p6_US_init);
  f_spec_US->FixParameter(7, p0_LS); // fixed bkg_LS
  f_spec_US->FixParameter(8, p1_LS);
  f_spec_US->FixParameter(9, p2_LS);
  f_spec_US->FixParameter(10,p3_LS);
  f_spec_US->FixParameter(11,p4_LS);
  f_spec_US->SetRange(fit_start,fit_stop);

  f_spec_US->SetLineColor(2);
  f_spec_US->SetLineStyle(2);
  h_mSpec_US[KEY_Spec_US]->Fit(f_spec_US,"MR");

  double p0_US = f_spec_US->GetParameter(0);
  double p1_US = f_spec_US->GetParameter(1);
  double p2_US = f_spec_US->GetParameter(2);
  double p3_US = f_spec_US->GetParameter(3);
  double p4_US = f_spec_US->GetParameter(4);
  double p5_US = f_spec_US->GetParameter(5);
  double p6_US = f_spec_US->GetParameter(6);

  TF1 *f_bkg_Corr = new TF1("f_bkg_Corr",func_bkg_Corr,1.5,5,2);
  f_bkg_Corr->SetParameter(0,p5_US);
  f_bkg_Corr->SetParameter(1,p6_US);
  f_bkg_Corr->SetLineColor(kGreen+2);
  f_bkg_Corr->SetLineWidth(3);
  f_bkg_Corr->SetLineStyle(2);
  f_bkg_Corr->Draw("l same");

  TF1 *f_sig_US = new TF1("f_sig_US",func_sig_US,1.5,5,5);
  f_sig_US->SetParameter(0, p0_US);
  f_sig_US->SetParameter(1, p1_US);
  f_sig_US->SetParameter(2, p2_US);
  f_sig_US->SetParameter(3, p3_US);
  f_sig_US->SetParameter(4, p4_US);
  f_sig_US->SetLineColor(kMagenta);
  f_sig_US->SetLineWidth(3);
  f_sig_US->Draw("l same");

  double chi2 = f_spec_US->GetChisquare();
  double ndf = f_spec_US->GetNDF();
  string fit_info = Form("#chi^{2}/ndf = %1.2f",chi2/ndf);

  TLegend *leg=new TLegend(0.6,0.6,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_mSpec_US[KEY_Spec_US],"Unlike-sign", "p");
  leg->AddEntry(h_mSpec_LS[KEY_Spec_LS],"Like-sign", "f");
  leg->AddEntry(f_spec_US,("US: "+fit_info).c_str(), "l");
  leg->AddEntry(f_sig_US,"J/#Psi ", "l");
  leg->AddEntry(f_bkg_LS,"LS BG", "l");
  leg->AddEntry(f_bkg_Corr,"Correlated BG", "l");
  leg->Draw("same");

  string leg_pt = Form("%1.1f < p_{T} < %1.1f GeV/c", vecMesonFlow::mPtStart[pt_QA],vecMesonFlow::mPtStop[pt_QA]);
  plotTopLegend((char*)leg_pt.c_str(),0.2,0.85,0.04,1,0.0,42,1,1);
  string leg_cent = "0-80\%";
  plotTopLegend((char*)leg_cent.c_str(),0.3,0.8,0.04,1,0.0,42,1,1);

  string FigName = Form("/Users/xusun/WorkSpace/PHENIX/figures/JPsiFlow/c_JPsi_Spec_Pt_%d_Cent_%d.eps",pt_QA,cent_QA);
  c_play->SaveAs(FigName.c_str());
}

string genKey_Spec(int pid, int mode, int pt_bin, int cent4)
{
  string KEY_Spec = Form("Spec_%s_Pt_%d_Centrality_%s_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),vecMesonFlow::mEventType[mode].c_str());

  return KEY_Spec;
}

