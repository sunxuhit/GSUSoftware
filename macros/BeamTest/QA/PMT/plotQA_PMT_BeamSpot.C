#include "string"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "../../../draw.h"

void plotQA_PMT_BeamSpot(const int runID = 124)
{
  gStyle->SetPalette(kRainBow);
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


  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/%s/BeamSpot_run%d.root",mode.c_str(),runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mRingImage = (TH2D*)File_InPut->Get("h_mRingImage")->Clone();
  TH2D *h_mCluster = (TH2D*)File_InPut->Get("h_mCluster")->Clone();
  TH2D *h_mCluster_1st = (TH2D*)File_InPut->Get("h_mCluster_1st")->Clone();
  TH2D *h_mCluster_2nd = (TH2D*)File_InPut->Get("h_mCluster_2nd")->Clone();
  TH2D *h_mCluster_3rd = (TH2D*)File_InPut->Get("h_mCluster_3rd")->Clone();

  TCanvas *c_BeamSpot = new TCanvas("c_BeamSpot","c_BeamSpot",10,10,800,800);
  c_BeamSpot->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_BeamSpot->cd(i_pad+1)->SetLeftMargin(0.15);
    c_BeamSpot->cd(i_pad+1)->SetBottomMargin(0.15);
    c_BeamSpot->cd(i_pad+1)->SetRightMargin(0.15);
    c_BeamSpot->cd(i_pad+1)->SetTicks(1,1);
    c_BeamSpot->cd(i_pad+1)->SetGrid(0,0);
    // c_BeamSpot->cd(i_pad+1)->SetLogz();
  }
  c_BeamSpot->cd(1);
  string title = Form("Ring Image @ Run %d", runID);
  h_mRingImage->SetTitle(title.c_str());
  h_mRingImage->SetStats(0);
  h_mRingImage->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage->GetXaxis()->CenterTitle();
  h_mRingImage->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage->GetYaxis()->CenterTitle();
  h_mRingImage->Draw("colz");

  if(mode != "MesonRun")
  {
    // plot beam spot
    PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
    PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
    PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
    PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);
  }

  c_BeamSpot->cd(2);
  title = Form("Cluster Finder @ Run %d", runID);
  h_mCluster->SetTitle(title.c_str());
  h_mCluster->SetStats(0);
  h_mCluster->GetXaxis()->SetTitle("pixel ID");
  h_mCluster->GetXaxis()->CenterTitle();
  h_mCluster->GetYaxis()->SetTitle("pixel ID");
  h_mCluster->GetYaxis()->CenterTitle();
  h_mCluster->Draw("colz");

  c_BeamSpot->cd(3);
  h_mCluster_1st->SetTitle("1^{st} Cluster");
  h_mCluster_1st->SetStats(0);
  h_mCluster_1st->GetXaxis()->SetTitle("pixel ID");
  h_mCluster_1st->GetXaxis()->CenterTitle();
  h_mCluster_1st->GetYaxis()->SetTitle("pixel ID");
  h_mCluster_1st->GetYaxis()->CenterTitle();
  h_mCluster_1st->Draw("colz");

  c_BeamSpot->cd(4);
  h_mCluster_2nd->SetTitle("2^{nd} Cluster");
  h_mCluster_2nd->SetStats(0);
  h_mCluster_2nd->GetXaxis()->SetTitle("pixel ID");
  h_mCluster_2nd->GetXaxis()->CenterTitle();
  h_mCluster_2nd->GetYaxis()->SetTitle("pixel ID");
  h_mCluster_2nd->GetYaxis()->CenterTitle();
  h_mCluster_2nd->Draw("colz");

  string FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/%s/c_BeamSpot_%d.eps",mode.c_str(),runID);
  c_BeamSpot->SaveAs(FigName.c_str());

  TCanvas *c_EventDisplay = new TCanvas("c_EventDisplay","c_EventDisplay",10,10,800,800);
  c_EventDisplay->cd()->SetLeftMargin(0.15);
  c_EventDisplay->cd()->SetBottomMargin(0.15);
  c_EventDisplay->cd()->SetRightMargin(0.15);
  c_EventDisplay->cd()->SetTicks(1,1);
  c_EventDisplay->cd()->SetGrid(0,0);
  c_EventDisplay->cd();
  // h_mRingImage->SetTitle(title.c_str());
  h_mRingImage->SetStats(0);
  h_mRingImage->GetXaxis()->SetTitle("pixel ID");
  h_mRingImage->GetXaxis()->CenterTitle();
  h_mRingImage->GetYaxis()->SetTitle("pixel ID");
  h_mRingImage->GetYaxis()->CenterTitle();
  h_mRingImage->Draw("colz");

  if(mode != "MesonRun")
  {
    // plot beam spot
    PlotLine(beam_x_start-0.5,beam_x_start-0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
    PlotLine(beam_x_stop+0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_stop+0.5,2,5,2);
    PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_start-0.5,beam_y_start-0.5,2,5,2);
    PlotLine(beam_x_start-0.5,beam_x_stop+0.5,beam_y_stop+0.5,beam_y_stop+0.5,2,5,2);
  }

  FigName = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/%s/c_EventDisplay_%d.eps",mode.c_str(),runID);
  c_EventDisplay->SaveAs(FigName.c_str());
}
