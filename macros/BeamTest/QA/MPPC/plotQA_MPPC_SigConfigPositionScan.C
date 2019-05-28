#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TStyle.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include "../../../draw.h"

void plotQA_MPPC_SigConfigPositionScan(const int runID = 649)
{
  // gStyle->SetPalette(kRainBow);
  string mode = "Calibration";
  if(runID >= 665 && runID <= 673) mode = "PositionScan";

  float tdc_Start = 490;
  float tdc_Stop  = 590;

  // beam spot
  int beam_x_start = -1;
  int beam_x_stop  = -1;
  int beam_y_start = -1;
  int beam_y_stop  = -1;

  int off_x_start = 0;
  int off_x_stop  = 4;
  int off_y_start = 0;
  int off_y_stop  = 4;

  if(runID >= 649 && runID <= 650 ) // Calibration
  {
    beam_x_start = 15;
    beam_x_stop  = 17;
    beam_y_start = 15;
    beam_y_stop  = 17;
  }
  if(runID >= 665 && runID <= 666) // PositionScan 
  {
    beam_x_start = 13;
    beam_x_stop  = 15;
    beam_y_start = 12;
    beam_y_stop  = 15;
  }
  if(runID >= 672 && runID <= 673) // PositionScan 
  {
    beam_x_start = 24;
    beam_x_stop  = 26;
    beam_y_start = 9;
    beam_y_stop  = 14;
  }

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/MPPC/%s/sipmTDC_run%d.root",mode.c_str(),runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mRingImage_on = (TH2D*)File_InPut->Get("h_mRingImage_on")->Clone();
  TH2D *h_mTdcTime_OnBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OnBeam")->Clone();
  TH2D *h_mTdcTime_OffBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OffBeam")->Clone();
  TH3D *h_mTdcTimeRadius;
  if(mode == "Calibration") h_mTdcTimeRadius = (TH3D*)File_InPut->Get("h_mTdcTimeRadius")->Clone();

  TCanvas *c_SigConfig = new TCanvas("c_SigConfig","c_SigConfig",10,10,1200,400);
  c_SigConfig->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_SigConfig->cd(i_pad+1)->SetLeftMargin(0.15);
    c_SigConfig->cd(i_pad+1)->SetBottomMargin(0.15);
    c_SigConfig->cd(i_pad+1)->SetRightMargin(0.15);
    c_SigConfig->cd(i_pad+1)->SetTicks(1,1);
    c_SigConfig->cd(i_pad+1)->SetGrid(0,0);
    // c_SigConfig->cd(i_pad+1)->SetLogz();
  }
  // string title = Form("120 GeV proton & run%d",runID);
  // if(runID > 343 && runID < 381) title = Form("meson & run%d",runID);

  c_SigConfig->cd(1);
  // string title = Form("%s & %1.1f < tdc < %1.1f",mode.c_str(),tdc_Start,tdc_Stop);
  string title = Form("runID: %d",runID);
  h_mRingImage_on->SetTitle(title.c_str());
  h_mRingImage_on->SetStats(0);
  h_mRingImage_on->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_on->GetXaxis()->CenterTitle();
  h_mRingImage_on->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_on->GetYaxis()->CenterTitle();
  h_mRingImage_on->Draw("colz");

  // plot beam spot
  PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);

  // plot off spot
  PlotLine(off_x_start-0.5,off_x_start-0.5,off_y_start-0.5,off_y_stop+0.5,2,5,2);
  PlotLine(off_x_stop+0.5,off_x_stop+0.5,off_y_start-0.5,off_y_stop+0.5,2,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_start-0.5,off_y_start-0.5,2,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_stop+0.5,off_y_stop+0.5,2,5,2);

  c_SigConfig->cd(2);
  int bin_OnBeam_Start = h_mTdcTime_OnBeam->GetXaxis()->FindBin(tdc_Start);
  int bin_OnBeam_Stop  = h_mTdcTime_OnBeam->GetXaxis()->FindBin(tdc_Stop);
  TH1D *h_mTime_OnBeam = (TH1D*)h_mTdcTime_OnBeam->ProjectionY("h_mTime_OnBeam",bin_OnBeam_Start,bin_OnBeam_Stop)->Clone("h_mTime_OnBeam");
  h_mTime_OnBeam->SetTitle("Time Duration On Beam");
  h_mTime_OnBeam->SetStats(0);
  h_mTime_OnBeam->GetXaxis()->SetTitle("time duration");
  h_mTime_OnBeam->GetXaxis()->CenterTitle();
  h_mTime_OnBeam->SetLineColor(2);
  h_mTime_OnBeam->Draw();

  c_SigConfig->cd(3);
  int bin_OffBeam_Start = h_mTdcTime_OffBeam->GetXaxis()->FindBin(tdc_Start);
  int bin_OffBeam_Stop  = h_mTdcTime_OffBeam->GetXaxis()->FindBin(tdc_Stop);
  TH1D *h_mTime_OffBeam = (TH1D*)h_mTdcTime_OffBeam->ProjectionY("h_mTime_OffBeam",bin_OffBeam_Start,bin_OffBeam_Stop)->Clone("h_mTime_OffBeam");
  h_mTime_OffBeam->SetTitle("Time Duration Off Beam");
  h_mTime_OffBeam->SetStats(0);
  h_mTime_OffBeam->GetXaxis()->SetTitle("time duration");
  h_mTime_OffBeam->GetXaxis()->CenterTitle();
  h_mTime_OffBeam->SetLineColor(1);
  h_mTime_OffBeam->Draw();

  string c_ringimage = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/MPPC/%s/c_SigConfigPositionScan_MPPC_%d.eps",mode.c_str(),runID);
  c_SigConfig->SaveAs(c_ringimage.c_str());
}
