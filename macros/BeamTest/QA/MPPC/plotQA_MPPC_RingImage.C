#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

void plotQA_MPPC_RingImage(const int runID = 649)
{
  string mode = "Calibration";
  if(runID >= 665 && runID <= 673) mode = "PositionScan";

  float tdc_Start = 490;
  float tdc_Stop  = 590;

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/MPPC/%s/sipmTDC_run%d.root",mode.c_str(),runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mRingImage_on = (TH2D*)File_InPut->Get("h_mRingImage_on")->Clone();
  TH2D *h_mRingImage_before = (TH2D*)File_InPut->Get("h_mRingImage_before")->Clone();
  TH2D *h_mRingImage_after = (TH2D*)File_InPut->Get("h_mRingImage_after")->Clone();

  TCanvas *c_RingImage = new TCanvas("c_RingImage","c_RingImage",10,10,1500,500);
  c_RingImage->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_RingImage->cd(i_pad+1)->SetLeftMargin(0.15);
    c_RingImage->cd(i_pad+1)->SetBottomMargin(0.15);
    c_RingImage->cd(i_pad+1)->SetRightMargin(0.15);
    c_RingImage->cd(i_pad+1)->SetTicks(1,1);
    c_RingImage->cd(i_pad+1)->SetGrid(0,0);
    // c_RingImage->cd(i_pad+1)->SetLogz();
  }
  // string title = Form("120 GeV proton & run%d",runID);
  // if(runID > 343 && runID < 381) title = Form("meson & run%d",runID);

  string title;

  c_RingImage->cd(1);
  title = Form("%s & %1.1f < tdc < %1.1f",mode.c_str(),tdc_Start,tdc_Stop);
  h_mRingImage_on->SetTitle(title.c_str());
  h_mRingImage_on->SetStats(0);
  h_mRingImage_on->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_on->GetXaxis()->CenterTitle();
  h_mRingImage_on->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_on->GetYaxis()->CenterTitle();
  h_mRingImage_on->Draw("colz");

  c_RingImage->cd(2);
  title = Form("Run%d & tdc < %1.1f",runID,tdc_Start);
  h_mRingImage_before->SetTitle(title.c_str());
  h_mRingImage_before->SetStats(0);
  h_mRingImage_before->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_before->GetXaxis()->CenterTitle();
  h_mRingImage_before->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_before->GetYaxis()->CenterTitle();
  h_mRingImage_before->Draw("colz");

  c_RingImage->cd(3);
  title = Form("tdc > %1.1f",tdc_Stop);
  h_mRingImage_after->SetTitle(title.c_str());
  h_mRingImage_after->SetStats(0);
  h_mRingImage_after->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_after->GetXaxis()->CenterTitle();
  h_mRingImage_after->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_after->GetYaxis()->CenterTitle();
  h_mRingImage_after->Draw("colz");

  string c_ringimage = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/MPPC/%s/c_RingImage_MPPC_%d.eps",mode.c_str(),runID);
  c_RingImage->SaveAs(c_ringimage.c_str());
}
