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

void plotQA_PMT_SigConfigCalibration(const int runID = 182)
{
  // gStyle->SetPalette(kRainBow);
  string mode = "Calibration";

  float tdc_Start = 2000.0;
  float tdc_Stop  = 2050.0;

  // beam spot
  int beam_x_start = 14;
  int beam_x_stop  = 18;
  int beam_y_start = 12;
  int beam_y_stop  = 20;

  int off_x_start = 0;
  int off_x_stop  = 4;
  int off_y_start = 0;
  int off_y_stop  = 4;

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/%s/richTDC_run%d.root",mode.c_str(),runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TH2D *h_mRingImage_on = (TH2D*)File_InPut->Get("h_mRingImage_on")->Clone();
  TH2D *h_mTdcTime_OnBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OnBeam")->Clone();
  TH2D *h_mTdcTime_OffBeam = (TH2D*)File_InPut->Get("h_mTdcTime_OffBeam")->Clone();
  TH3D *h_mTdcTimeRadius = (TH3D*)File_InPut->Get("h_mTdcTimeRadius")->Clone();
  TH3D *h_mTdcTimeRadius_clone = (TH3D*)File_InPut->Get("h_mTdcTimeRadius")->Clone();

  TH1D *h_mNumOfPhotons = (TH1D*)File_InPut->Get("h_mNumOfPhotons")->Clone();
  float NumOfEvents = h_mNumOfPhotons->GetEntries();
  cout << "NumOfEvents = " << NumOfEvents << endl;

  TCanvas *c_SigConfig = new TCanvas("c_SigConfig","c_SigConfig",10,10,800,800);
  c_SigConfig->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
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

  // plot ring radius
  TEllipse *circle_inner = new TEllipse(16,16,11,11);
  circle_inner->SetFillColor(0);
  circle_inner->SetFillStyle(0);
  circle_inner->SetLineWidth(4);
  circle_inner->SetLineColor(4);
  circle_inner->SetLineStyle(2);
  circle_inner->Draw("same");
  TEllipse *circle_outer = new TEllipse(16,16,14,14);
  circle_outer->SetFillColor(0);
  circle_outer->SetFillStyle(0);
  circle_outer->SetLineWidth(4);
  circle_outer->SetLineColor(4);
  circle_outer->SetLineStyle(2);
  circle_outer->Draw("same");

  c_SigConfig->cd(2);
  int binx_OnRing_Start = h_mTdcTimeRadius->GetXaxis()->FindBin(tdc_Start);
  int binx_OnRing_Stop  = h_mTdcTimeRadius->GetXaxis()->FindBin(tdc_Stop);
  int binz_OnRing_Start = h_mTdcTimeRadius->GetZaxis()->FindBin(37.5);
  int binz_OnRing_Stop  = h_mTdcTimeRadius->GetZaxis()->FindBin(42.5);
  TH1D *h_mTime_OnRing = (TH1D*)h_mTdcTimeRadius->ProjectionY("h_mTimeRadius",binx_OnRing_Start,binx_OnRing_Stop,binz_OnRing_Start,binz_OnRing_Stop)->Clone("h_mTime_OnRing");
  h_mTime_OnRing->SetTitle("Time Duration On Ring");
  h_mTime_OnRing->SetStats(0);
  h_mTime_OnRing->GetXaxis()->SetTitle("time duration (ns)");
  h_mTime_OnRing->GetXaxis()->CenterTitle();
  h_mTime_OnRing->SetLineColor(4);
  h_mTime_OnRing->SetLineWidth(2);
  h_mTime_OnRing->Scale(1.0/NumOfEvents);
  h_mTime_OnRing->DrawCopy("h");
  TLegend *leg_onring = new TLegend(0.45,0.6,0.85,0.7);
  leg_onring->SetBorderSize(0);
  leg_onring->SetFillColor(0);
  leg_onring->AddEntry(h_mTime_OnRing,"photons on ring","l");
  leg_onring->Draw("same");

  c_SigConfig->cd(3);
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

  c_SigConfig->cd(4);
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

  // string FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigCalibration_PMT_%d.eps",runID);
  string FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigCalibration_PMT_%d.png",runID);
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

  // plot ring radius
  circle_inner->Draw("same");
  circle_outer->Draw("same");

  c_SigConfigSum->cd(2);
  h_mTime_OnRing->SetTitle("");
  h_mTime_OnRing->DrawCopy("h");
  h_mTime_OnBeam->DrawCopy("h same");
  h_mTime_OffBeam->DrawCopy("h same");
  TLegend *leg = new TLegend(0.45,0.5,0.85,0.7);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(h_mTime_OnRing,"photons on ring","l");
  leg->AddEntry(h_mTime_OnBeam,"photons on beam","l");
  leg->AddEntry(h_mTime_OffBeam,"photons off beam","l");
  leg->Draw("same");

  // string FigNameSum = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigSumCalibration_PMT_%d.eps",runID);
  string FigNameSum = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigSumCalibration_PMT_%d.png",runID);
  c_SigConfigSum->SaveAs(FigNameSum.c_str());

  TCanvas *c_tdc = new TCanvas("c_tdc","c_tdc",1600,400);
  c_tdc->Divide(4,1);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_tdc->cd(i_pad+1)->SetLeftMargin(0.15);
    c_tdc->cd(i_pad+1)->SetBottomMargin(0.15);
    c_tdc->cd(i_pad+1)->SetRightMargin(0.15);
    c_tdc->cd(i_pad+1)->SetTicks(1,1);
    c_tdc->cd(i_pad+1)->SetGrid(0,0);
  }

  c_tdc->cd(1);
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

  // plot ring radius
  circle_inner->Draw("same");
  circle_outer->Draw("same");

  c_tdc->cd(2);
  h_mTdcTimeRadius->GetZaxis()->SetRange(binz_OnRing_Start,binz_OnRing_Stop);
  TH2D *h_mTdcTime = (TH2D*)h_mTdcTimeRadius->Project3D("yx");
  h_mTdcTime->SetTitle("Photons On Ring");
  h_mTdcTime->SetStats(0);
  h_mTdcTime->GetXaxis()->SetTitle("tdc (ns)");
  h_mTdcTime->GetXaxis()->CenterTitle();
  h_mTdcTime->GetXaxis()->SetRangeUser(tdc_Start,tdc_Stop);
  h_mTdcTime->GetYaxis()->SetTitle("time duration (ns)");
  h_mTdcTime->GetYaxis()->CenterTitle();
  h_mTdcTime->Draw("colz");

  c_tdc->cd(3);
  h_mTdcTime_OnBeam->SetTitle("Photons On Beam");
  h_mTdcTime_OnBeam->SetStats(0);
  h_mTdcTime_OnBeam->GetXaxis()->SetTitle("tdc (ns)");
  h_mTdcTime_OnBeam->GetXaxis()->CenterTitle();
  h_mTdcTime_OnBeam->GetXaxis()->SetRangeUser(tdc_Start,tdc_Stop);
  h_mTdcTime_OnBeam->GetYaxis()->SetTitle("time duration (ns)");
  h_mTdcTime_OnBeam->GetYaxis()->CenterTitle();
  h_mTdcTime_OnBeam->Draw("colz");

  c_tdc->cd(4);
  h_mTdcTime_OffBeam->SetTitle("Photons On Beam");
  h_mTdcTime_OffBeam->SetStats(0);
  h_mTdcTime_OffBeam->GetXaxis()->SetTitle("tdc (ns)");
  h_mTdcTime_OffBeam->GetXaxis()->CenterTitle();
  h_mTdcTime_OffBeam->GetXaxis()->SetRangeUser(tdc_Start,tdc_Stop);
  h_mTdcTime_OffBeam->GetYaxis()->SetTitle("time duration (ns)");
  h_mTdcTime_OffBeam->GetYaxis()->CenterTitle();
  h_mTdcTime_OffBeam->Draw("colz");

  string FigName2d = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_SigConfigSumCalibration2D_PMT_%d.png",runID);
  c_tdc->SaveAs(FigName2d.c_str());

  TCanvas *c_radius = new TCanvas("c_radius","c_radius",10,10,800,800);
  c_radius->cd()->SetLeftMargin(0.15);
  c_radius->cd()->SetBottomMargin(0.15);
  c_radius->cd()->SetRightMargin(0.15);
  c_radius->cd()->SetTicks(1,1);
  c_radius->cd()->SetGrid(0,0);
  int binx_Start = h_mTdcTimeRadius_clone->GetXaxis()->FindBin(tdc_Start);
  int binx_Stop  = h_mTdcTimeRadius_clone->GetXaxis()->FindBin(tdc_Stop);
  int biny_Start = h_mTdcTimeRadius_clone->GetYaxis()->FindBin(40.0);
  int biny_Stop  = h_mTdcTimeRadius_clone->GetYaxis()->FindBin(60.0);
  cout << "biny_Start = " << biny_Start << ", biny_Stop = " << biny_Stop << endl;
  TH1D *h_mRadius = (TH1D*)h_mTdcTimeRadius_clone->ProjectionZ("h_mRadius",binx_Start,binx_Stop,biny_Start,biny_Stop)->Clone("h_mRadius");
  h_mRadius->SetTitle("Radius per pixel");
  h_mRadius->SetStats(0);
  h_mRadius->GetXaxis()->SetTitle("R (mm)");
  h_mRadius->GetXaxis()->CenterTitle();
  h_mRadius->SetLineColor(4);
  h_mRadius->SetLineWidth(2);
  h_mRadius->Scale(1.0/NumOfEvents);
  h_mRadius->DrawCopy("h");

  FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/PMT/c_Radius_PMT_%d.png",runID);
  c_radius->SaveAs(FigName.c_str());
}
