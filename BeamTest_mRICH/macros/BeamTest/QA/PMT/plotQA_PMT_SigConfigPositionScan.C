#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TStyle.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TLegend.h>
#include "../../../draw.h"

void plotQA_PMT_SigConfigPositionScan(const int runID = 182)
{
  // gStyle->SetPalette(kRainBow);
  string mode = "Calibration";
  if(runID < 172) mode = "PositionScan";
  if(runID >= 196 && runID <= 215) mode = "AngleRun";
  if(runID >= 266 && runID <= 316) mode = "ThresholdScan";
  if(runID >= 344 && runID <= 380) mode = "MesonRun";

  float tdc_Start = 2000.0;
  float tdc_Stop  = 2050.0;
  if(runID >= 344 && runID <= 380) // meson run 344-380
  {
    tdc_Start = 490.0;
    tdc_Stop  = 590.0;
  }

  // beam spot
  int beam_x_start = -1;
  int beam_x_stop  = -1;
  int beam_y_start = -1;
  int beam_y_stop  = -1;

  int off_x_start = 0;
  int off_x_stop  = 4;
  int off_y_start = 0;
  int off_y_stop  = 4;

  if(runID >= 172 && runID <= 192) // Calibration
  {
    beam_x_start = 14;
    beam_x_stop  = 18;
    beam_y_start = 12;
    beam_y_stop  = 20;
  }
  if(runID == 16 || (runID >= 18 && runID <= 21)) // PositionScan 
  {
    beam_x_start = 0;
    beam_x_stop  = 5;
    beam_y_start = 27;
    beam_y_stop  = 32;
  }
  if(runID >= 22 && runID <= 35) // PositionScan 
  {
    beam_x_start = 5;
    beam_x_stop  = 11;
    beam_y_start = 20;
    beam_y_stop  = 26;
  }
  if((runID >= 42 && runID <= 51) || (runID >= 54 && runID <= 64)) // PositionScan 
  {
    beam_x_start = 13;
    beam_x_stop  = 15;
    beam_y_start = 17;
    beam_y_stop  = 20;
  }
  if((runID >= 65 && runID <= 70) || (runID >= 88 && runID <= 102)) // PositionScan 
  {
    beam_x_start = 14;
    beam_x_stop  = 18;
    beam_y_start = 12;
    beam_y_stop  = 15;
  }
  if(runID >= 103 && runID <= 122) // PositionScan 
  {
    beam_x_start = 16;
    beam_x_stop  = 19;
    beam_y_start = 12;
    beam_y_stop  = 15;
  }
  if(runID >= 123 && runID <= 142) // PositionScan 
  {
    beam_x_start = 21;
    beam_x_stop  = 26;
    beam_y_start = 4;
    beam_y_stop  = 11;

  }
  if(runID >= 144 && runID <= 163) // PositionScan 
  {
    beam_x_start = 28;
    beam_x_stop  = 32;
    beam_y_start = 0;
    beam_y_stop  = 4;
  }

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/%s/richTDC_run%d.root",mode.c_str(),runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mRingImage_on = (TH2D*)File_InPut->Get("h_mRingImage_on")->Clone();
  TH2D *h_mTdcTime_OnBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OnBeam")->Clone();
  TH2D *h_mTdcTime_OffBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OffBeam")->Clone();
  TH3D *h_mTdcTimeRadius;
  if(mode == "Calibration") h_mTdcTimeRadius = (TH3D*)File_InPut->Get("h_mTdcTimeRadius")->Clone();

  TH1D *h_mNumOfPhotons = (TH1D*)File_InPut->Get("h_mNumOfPhotons")->Clone();
  float NumOfEvents = h_mNumOfPhotons->GetEntries();
  cout << "NumOfEvents = " << NumOfEvents << endl;

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
  PlotLine(off_x_start-0.5,off_x_start-0.5,off_y_start-0.5,off_y_stop+0.5,1,5,2);
  PlotLine(off_x_stop+0.5,off_x_stop+0.5,off_y_start-0.5,off_y_stop+0.5,1,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_start-0.5,off_y_start-0.5,1,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_stop+0.5,off_y_stop+0.5,1,5,2);

  c_SigConfig->cd(2);
  int bin_OnBeam_Start = h_mTdcTime_OnBeam->GetXaxis()->FindBin(tdc_Start);
  int bin_OnBeam_Stop  = h_mTdcTime_OnBeam->GetXaxis()->FindBin(tdc_Stop);
  TH1D *h_mTime_OnBeam = (TH1D*)h_mTdcTime_OnBeam->ProjectionY("h_mTime_OnBeam",bin_OnBeam_Start,bin_OnBeam_Stop)->Clone("h_mTime_OnBeam");
  h_mTime_OnBeam->SetTitle("Time Duration On Beam");
  h_mTime_OnBeam->SetStats(0);
  h_mTime_OnBeam->GetXaxis()->SetTitle("time duration (ns)");
  h_mTime_OnBeam->GetXaxis()->CenterTitle();
  h_mTime_OnBeam->SetLineColor(2);
  h_mTime_OnBeam->SetLineWidth(2);
  h_mTime_OnBeam->Scale(1.0/NumOfEvents);
  h_mTime_OnBeam->DrawCopy("h");
  TLegend *leg_onbeam = new TLegend(0.45,0.6,0.85,0.7);
  leg_onbeam->SetBorderSize(0);
  leg_onbeam->SetFillColor(0);
  leg_onbeam->AddEntry(h_mTime_OnBeam,"photons on beam","l");
  leg_onbeam->Draw("same");

  c_SigConfig->cd(3);
  int bin_OffBeam_Start = h_mTdcTime_OffBeam->GetXaxis()->FindBin(tdc_Start);
  int bin_OffBeam_Stop  = h_mTdcTime_OffBeam->GetXaxis()->FindBin(tdc_Stop);
  TH1D *h_mTime_OffBeam = (TH1D*)h_mTdcTime_OffBeam->ProjectionY("h_mTime_OffBeam",bin_OffBeam_Start,bin_OffBeam_Stop)->Clone("h_mTime_OffBeam");
  h_mTime_OffBeam->SetTitle("Time Duration Off Beam");
  h_mTime_OffBeam->SetStats(0);
  h_mTime_OffBeam->GetXaxis()->SetTitle("time duration (ns)");
  h_mTime_OffBeam->GetXaxis()->CenterTitle();
  h_mTime_OffBeam->SetLineColor(1);
  h_mTime_OffBeam->SetLineWidth(2);
  h_mTime_OffBeam->Scale(1.0/NumOfEvents);
  h_mTime_OffBeam->DrawCopy("h");
  TLegend *leg_offbeam = new TLegend(0.45,0.6,0.85,0.7);
  leg_offbeam->SetBorderSize(0);
  leg_offbeam->SetFillColor(0);
  leg_offbeam->AddEntry(h_mTime_OffBeam,"photons off beam","l");
  leg_offbeam->Draw("same");

  // string FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigPositionScan_PMT_%d.eps",runID);
  string FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigPositionScan_PMT_%d.png",runID);
  c_SigConfig->SaveAs(FigName.c_str());

  TCanvas *c_SigConfigSum = new TCanvas("c_SigConfigSum","c_SigConfigSum",10,10,800,400);
  c_SigConfigSum->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_SigConfigSum->cd(i_pad+1)->SetLeftMargin(0.15);
    c_SigConfigSum->cd(i_pad+1)->SetBottomMargin(0.15);
    c_SigConfigSum->cd(i_pad+1)->SetRightMargin(0.15);
    c_SigConfigSum->cd(i_pad+1)->SetTicks(1,1);
    c_SigConfigSum->cd(i_pad+1)->SetGrid(0,0);
  }

  c_SigConfigSum->cd(1);
  h_mRingImage_on->Draw("colz");

  // plot beam spot
  PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);

  // plot off spot
  PlotLine(off_x_start-0.5,off_x_start-0.5,off_y_start-0.5,off_y_stop+0.5,1,5,2);
  PlotLine(off_x_stop+0.5,off_x_stop+0.5,off_y_start-0.5,off_y_stop+0.5,1,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_start-0.5,off_y_start-0.5,1,5,2);
  PlotLine(off_x_start-0.5,off_x_stop+0.5,off_y_stop+0.5,off_y_stop+0.5,1,5,2);

  c_SigConfigSum->cd(2);
  h_mTime_OnBeam->SetTitle("");
  h_mTime_OnBeam->DrawCopy("h");
  h_mTime_OffBeam->DrawCopy("h same");
  TLegend *leg = new TLegend(0.45,0.5,0.85,0.7);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(h_mTime_OnBeam,"photons on beam","l");
  leg->AddEntry(h_mTime_OffBeam,"photons off beam","l");
  leg->Draw("same");

  // string FigNameSum = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigSumPositionScan_PMT_%d.eps",runID);
  string FigNameSum = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigSumPositionScan_PMT_%d.png",runID);
  c_SigConfigSum->SaveAs(FigNameSum.c_str());
}
