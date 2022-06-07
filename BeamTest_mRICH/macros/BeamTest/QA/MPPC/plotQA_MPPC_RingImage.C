#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "../../../draw.h"

void plotQA_MPPC_RingImage(const int runID = 649)
{
  gStyle->SetPalette(kRainBow);
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
  TH2D *h_mRingImage_before = (TH2D*)File_InPut->Get("h_mRingImage_before")->Clone();
  TH2D *h_mRingImage_after = (TH2D*)File_InPut->Get("h_mRingImage_after")->Clone();

  TH2D *h_mRingImage_Display1 = (TH2D*)File_InPut->Get("h_mRingImage_Display1")->Clone();
  TH2D *h_mRingImage_Display2 = (TH2D*)File_InPut->Get("h_mRingImage_Display2")->Clone();
  TH2D *h_mRingImage_Display3 = (TH2D*)File_InPut->Get("h_mRingImage_Display3")->Clone();

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

  // string c_ringimage = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/SiPM/c_RingImage_MPPC_%d.eps",runID);
  string c_ringimage = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/SiPM/c_RingImage_MPPC_%d.png",runID);
  c_RingImage->SaveAs(c_ringimage.c_str());

  TCanvas *c_RingImage_Display = new TCanvas("c_RingImage_Display","c_RingImage_Display",10,10,800,800);
  c_RingImage_Display->Divide(2,2);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_RingImage_Display->cd(i_pad+1)->SetLeftMargin(0.15);
    c_RingImage_Display->cd(i_pad+1)->SetBottomMargin(0.15);
    c_RingImage_Display->cd(i_pad+1)->SetRightMargin(0.15);
    c_RingImage_Display->cd(i_pad+1)->SetTicks(1,1);
    c_RingImage_Display->cd(i_pad+1)->SetGrid(0,0);
    // c_RingImage_Display->cd(i_pad+1)->SetLogz();
  }

  c_RingImage_Display->cd(1);
  title = Form("%s & %1.1f < tdc < %1.1f",mode.c_str(),tdc_Start,tdc_Stop);
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

  c_RingImage_Display->cd(2);
  h_mRingImage_Display1->SetTitle("Single Event: 1024");
  h_mRingImage_Display1->SetStats(0);
  h_mRingImage_Display1->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_Display1->GetXaxis()->CenterTitle();
  h_mRingImage_Display1->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_Display1->GetYaxis()->CenterTitle();
  h_mRingImage_Display1->Draw("colz");

  // plot beam spot
  PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);

  c_RingImage_Display->cd(3);
  h_mRingImage_Display2->SetTitle("Single Event: 2048");
  h_mRingImage_Display2->SetStats(0);
  h_mRingImage_Display2->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_Display2->GetXaxis()->CenterTitle();
  h_mRingImage_Display2->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_Display2->GetYaxis()->CenterTitle();
  h_mRingImage_Display2->Draw("colz");

  // plot beam spot
  PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);

  c_RingImage_Display->cd(4);
  h_mRingImage_Display3->SetTitle("Single Event: 4096");
  h_mRingImage_Display3->SetStats(0);
  h_mRingImage_Display3->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage_Display3->GetXaxis()->CenterTitle();
  h_mRingImage_Display3->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage_Display3->GetYaxis()->CenterTitle();
  h_mRingImage_Display3->Draw("colz");

  // plot beam spot
  PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
  PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);

  // string c_ringimage_display = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/SiPM/c_RingImageDisplay_MPPC_%d.eps",runID);
  string c_ringimage_display = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/Proposal_2019/SiPM/c_RingImageDisplay_MPPC_%d.png",runID);
  c_RingImage_Display->SaveAs(c_ringimage_display.c_str());
}
