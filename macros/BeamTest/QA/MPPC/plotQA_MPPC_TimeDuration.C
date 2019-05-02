#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "../../../draw.h"

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

  c_TimeDuration->SaveAs("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/MPPC/Calibration/c_TimeDuration.eps");
}

