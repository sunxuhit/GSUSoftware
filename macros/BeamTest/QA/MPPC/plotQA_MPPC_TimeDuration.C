#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "../../../draw.h"

double doubleGauss(double *x_val, double *par)
{
  double x = x_val[0];

  double norm_1st = par[0];
  double mean_1st = par[1];
  double sigma_1st = par[2];
  double sigSquare_1st = sigma_1st*sigma_1st;
  double power_1st = -0.5*(x-mean_1st)*(x-mean_1st)/sigSquare_1st;
  // double gaus_1st = norm_1st*TMath::Exp(power_1st)/TMath::Sqrt(2.0*TMath::Pi()*sigSquare_1st);
  double gaus_1st = norm_1st*TMath::Exp(power_1st);

  /*
  double norm_2nd = par[3];
  double mean_2nd = par[4];
  double sigma_2nd = par[5];
  double sigSquare_2nd = sigma_2nd*sigma_2nd;
  double power_2nd = -0.5*(x-mean_2nd)*(x-mean_2nd)/sigSquare_2nd;
  // double gaus_2nd = norm_2nd*TMath::Exp(power_2nd)/TMath::Sqrt(2.0*TMath::Pi()*sigSquare_2nd);
  double gaus_2nd = norm_2nd*TMath::Exp(power_2nd);
  */

  // double y = gaus_1st + gaus_2nd;
  double y = gaus_1st;

  return y;
}

void plotQA_MPPC_TimeDuration(int runId = 649)
{
  const string mode = "Calibration";

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/MPPC/%s/sipmTDC_run%d.root",mode.c_str(),runId);
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mTimeDuration = (TH2D*)File_InPut->Get("h_mTimeDuration")->Clone();

  int tdc_start = 490;
  int tdc_stop  = 590;
  int bin_start = h_mTimeDuration->GetXaxis()->FindBin(tdc_start);
  int bin_stop  = h_mTimeDuration->GetXaxis()->FindBin(tdc_stop);
  cout << "tdc_start = " << tdc_start << ", bin_start = " << bin_start << ", tdc_stop = " << tdc_stop << ", bin_stop = " << bin_stop << endl;
  TH1D *h_mTdcDuration = (TH1D*)h_mTimeDuration->ProjectionY("h_mTdcDuration",bin_start,bin_stop)->Clone("h_mTdcDuration");

  TCanvas *c_TimeDuration = new TCanvas("c_TimeDuration","c_TimeDuration",10,10,1600,800);
  c_TimeDuration->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_TimeDuration->cd(i_pad+1)->SetLeftMargin(0.15);
    c_TimeDuration->cd(i_pad+1)->SetBottomMargin(0.15);
    c_TimeDuration->cd(i_pad+1)->SetRightMargin(0.15);
    c_TimeDuration->cd(i_pad+1)->SetTicks(1,1);
    c_TimeDuration->cd(i_pad+1)->SetGrid(0,0);
  }

  c_TimeDuration->cd(1);
  c_TimeDuration->cd(1)->SetLogz();
  h_mTimeDuration->SetTitle("");
  h_mTimeDuration->GetXaxis()->SetTitle("tdc falling edge");
  h_mTimeDuration->GetXaxis()->CenterTitle();
  h_mTimeDuration->GetXaxis()->SetRangeUser(0,1500);
  h_mTimeDuration->GetYaxis()->SetTitle("time duration");
  h_mTimeDuration->GetYaxis()->CenterTitle();
  h_mTimeDuration->GetYaxis()->SetRangeUser(0,200);
  h_mTimeDuration->Draw("colz");
  PlotLine(tdc_start,tdc_start,-0.5,199.5,2,5,2);
  PlotLine(tdc_stop,tdc_stop,-0.5,199.5,2,5,2);

  c_TimeDuration->cd(2);
  h_mTdcDuration->SetTitle("");
  h_mTdcDuration->GetXaxis()->SetTitle("time duration");
  h_mTdcDuration->GetXaxis()->CenterTitle();
  h_mTdcDuration->GetXaxis()->SetRangeUser(0,200);
  h_mTdcDuration->Draw();

  TF1 *f_gaus = new TF1("f_gaus",doubleGauss,0,100,3);
  f_gaus->SetParameter(0,1000);
  f_gaus->SetParameter(1,50.0);
  f_gaus->SetParameter(2,5.0);
  // f_gaus->SetParameter(3,5000);
  // f_gaus->SetParameter(4,50.0);
  // f_gaus->SetParameter(5,5.0);
  f_gaus->SetRange(52,65);
  h_mTdcDuration->Fit(f_gaus,"MNR");
  f_gaus->SetLineColor(2);
  f_gaus->SetLineStyle(2);
  f_gaus->SetLineWidth(2);
  // f_gaus->SetRange(10,60);
  f_gaus->Draw("l Same");

  string c_timeduration = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/MPPC/Calibration/c_TimeDuration_%d.eps",runId); 
  c_TimeDuration->SaveAs(c_timeduration.c_str());

  float mean_timeduration = f_gaus->GetParameter(1);
  float sigma_timeduration = f_gaus->GetParameter(2);

  TH2D *h_mTimeDurationCuts = new TH2D("h_mTimeDurationCuts","h_mTimeDurationCuts",3,-0.5,2.5,800,-0.5,799.5);
  h_mTimeDurationCuts->SetBinContent(1,runId,mean_timeduration);
  h_mTimeDurationCuts->SetBinContent(2,runId,sigma_timeduration);
  h_mTimeDurationCuts->SetBinContent(3,runId,runId);
  h_mTimeDurationCuts->GetXaxis()->SetBinLabel(1,"tdc mean");
  h_mTimeDurationCuts->GetXaxis()->SetBinLabel(2,"tdc sigma");
  h_mTimeDurationCuts->GetXaxis()->SetBinLabel(3,"runId");
  h_mTimeDurationCuts->GetXaxis()->SetLabelSize(0.06);
  h_mTimeDurationCuts->GetYaxis()->SetTitle("runId");
  h_mTimeDurationCuts->GetYaxis()->CenterTitle();
  h_mTimeDurationCuts->GetYaxis()->SetTitleSize(0.06);

  string outputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/MPPC/Calibration/sipmTimeDurationCuts_run%d.root",runId);
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
  h_mTimeDurationCuts->Write();
  File_OutPut->Close();
}

