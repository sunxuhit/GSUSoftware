#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TF1.h>
#include "../../../draw.h"

// int extractTDC_PMT(const string mode = "Calibration", const int runConfig = 0)
int extractTDC_PMT(const string mode = "ThresholdScan", const int runConfig = 3)
{
  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/TDC/%s/richTDC_%d.root",mode.c_str(),runConfig);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  int const NumOfPixel = 33;
  TH2D *h_mTDC[NumOfPixel][NumOfPixel];
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      string HistName = Form("h_mTDC_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
      h_mTDC[i_pixel_x][i_pixel_y] = (TH2D*)File_InPut->Get(HistName.c_str())->Clone();
    }
  }

  // float tdc_Start = 1949.5;
  // float tdc_Stop  = 2149.5;
  float tdc_Start = 2000.5;
  float tdc_Stop  = 2050.5;

  TH1D *h_mTdcProjX[NumOfPixel][NumOfPixel];
  float mean_tdc[NumOfPixel][NumOfPixel];
  float sigma_tdc[NumOfPixel][NumOfPixel];
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      mean_tdc[i_pixel_x][i_pixel_y] = -1.0;
      sigma_tdc[i_pixel_x][i_pixel_y] = -1.0;
      if(h_mTDC[i_pixel_x][i_pixel_y]->GetEntries() > 0)
      {
	// extract tdc 3 sigma cut
	string HistName = Form("h_mTdcProjX_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
	h_mTdcProjX[i_pixel_x][i_pixel_y] = (TH1D*)h_mTDC[i_pixel_x][i_pixel_y]->ProjectionX()->Clone(HistName.c_str());

	TF1 *f_gaus = new TF1("f_gaus","gaus",0,5000);
	f_gaus->SetParameter(0,100);
	f_gaus->SetParameter(1,0.5*(tdc_Start+tdc_Stop));
	f_gaus->SetParameter(2,0.5*(tdc_Stop-tdc_Start));
	f_gaus->SetRange(tdc_Start,tdc_Stop);
	h_mTdcProjX[i_pixel_x][i_pixel_y]->Fit(f_gaus,"MNQR");

	float norm = f_gaus->GetParameter(0);
	float mean = f_gaus->GetParameter(1);
	float sigma = f_gaus->GetParameter(2);

	f_gaus->ReleaseParameter(0);
	f_gaus->ReleaseParameter(1);
	f_gaus->ReleaseParameter(2);
	f_gaus->SetParameter(0,norm);
	f_gaus->SetParameter(1,mean);
	f_gaus->SetParameter(2,sigma);
	f_gaus->SetRange(mean-3.0*sigma,mean+3.0*sigma);
	h_mTdcProjX[i_pixel_x][i_pixel_y]->Fit(f_gaus,"MNQR");

	mean_tdc[i_pixel_x][i_pixel_y] = f_gaus->GetParameter(1);
	sigma_tdc[i_pixel_x][i_pixel_y] = f_gaus->GetParameter(2);
	// cout << "i_pixel_x = " << i_pixel_x << ", i_pixel_y = " << i_pixel_y << ", mean_tdc = " << mean_tdc[i_pixel_x][i_pixel_y] << ", sigma_tdc = " << sigma_tdc[i_pixel_x][i_pixel_y] << endl;
      }
    }
  }

  TH1D *h_mTdcProjY[NumOfPixel][NumOfPixel];
  float mean_time[NumOfPixel][NumOfPixel];
  float sigma_time[NumOfPixel][NumOfPixel];
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      mean_time[i_pixel_x][i_pixel_y] = -1.0;
      sigma_time[i_pixel_x][i_pixel_y] = -1.0;
      if(h_mTDC[i_pixel_x][i_pixel_y]->GetEntries() > 0)
      {
	// extract tdc 3 sigma cut
	float cut_start = std::floor(mean_tdc[i_pixel_x][i_pixel_y] - 3.0*sigma_tdc[i_pixel_x][i_pixel_y]);
	float cut_stop  = std::ceil(mean_tdc[i_pixel_x][i_pixel_y] + 3.0*sigma_tdc[i_pixel_x][i_pixel_y]);
	int bin_start = h_mTDC[i_pixel_x][i_pixel_y]->GetXaxis()->FindBin(cut_start);
	int bin_stop  = h_mTDC[i_pixel_x][i_pixel_y]->GetXaxis()->FindBin(cut_stop);
	// cout << "cut_start = " << cut_start << ", cut_stop = " << cut_stop << endl;
	// cout << "bin_start = " << bin_start << ", bin_stop = " << bin_stop << endl;
	string HistName = Form("h_mTdcProjY_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
	h_mTdcProjY[i_pixel_x][i_pixel_y] = (TH1D*)h_mTDC[i_pixel_x][i_pixel_y]->ProjectionY(HistName.c_str(),bin_start,bin_stop);

	TF1 *f_gaus = new TF1("f_gaus","gaus",0,150);
	f_gaus->SetParameter(0,100);
	f_gaus->SetParameter(1,50.0);
	f_gaus->SetParameter(2,3.0);
	f_gaus->SetRange(35.0,70.0);
	h_mTdcProjY[i_pixel_x][i_pixel_y]->Fit(f_gaus,"MNQR");

	float norm = f_gaus->GetParameter(0);
	float mean = f_gaus->GetParameter(1);
	float sigma = f_gaus->GetParameter(2);

	f_gaus->ReleaseParameter(0);
	f_gaus->ReleaseParameter(1);
	f_gaus->ReleaseParameter(2);
	f_gaus->SetParameter(0,norm);
	f_gaus->SetParameter(1,mean);
	f_gaus->SetParameter(2,sigma);
	f_gaus->SetRange(mean-0.5*sigma,mean+3.0*sigma);
	h_mTdcProjY[i_pixel_x][i_pixel_y]->Fit(f_gaus,"MNQR");

	mean_time[i_pixel_x][i_pixel_y] = f_gaus->GetParameter(1);
	sigma_time[i_pixel_x][i_pixel_y] = f_gaus->GetParameter(2);
	// cout << "i_pixel_x = " << i_pixel_x << ", i_pixel_y = " << i_pixel_y << ", mean_time = " << mean_time[i_pixel_x][i_pixel_y] << ", sigma_time = " << sigma_time[i_pixel_x][i_pixel_y] << endl;
      }
    }
  }

  gRandom->SetSeed();
  int pixel_x = std::floor(gRandom->Uniform(0.0,33.0)); // random pixel display
  int pixel_y = std::floor(gRandom->Uniform(0.0,33.0));
  // cout << "gRandom = " << pixel_x << ", floor = " <<  std::floor(pixel_x) << endl;
  // cout << "gRandom = " << pixel_y << ", floor = " <<  std::floor(pixel_y) << endl;

  if(h_mTDC[pixel_x][pixel_y]->GetEntries() == 0) 
  {
    cout << "No Entries at Pixel_x: " << pixel_x << " & Pixel_y: " << pixel_y << endl;
    return 0;
  }

  TCanvas *c_TimeCuts = new TCanvas("c_TimeCuts","c_TimeCuts",10,10,1500,500);
  c_TimeCuts->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_TimeCuts->cd(i_pad+1)->SetLeftMargin(0.15);
    c_TimeCuts->cd(i_pad+1)->SetBottomMargin(0.15);
    c_TimeCuts->cd(i_pad+1)->SetRightMargin(0.15);
    c_TimeCuts->cd(i_pad+1)->SetTicks(1,1);
    c_TimeCuts->cd(i_pad+1)->SetGrid(0,0);
  }

  //------------------------------------------------------
  c_TimeCuts->cd(2);
  string title = Form("TDC => pixel_x %d & pixel_y %d",pixel_x,pixel_y);
  h_mTdcProjX[pixel_x][pixel_y]->SetTitle(title.c_str());
  h_mTdcProjX[pixel_x][pixel_y]->SetStats(0);
  h_mTdcProjX[pixel_x][pixel_y]->GetXaxis()->SetTitle("tdc (ns)");
  h_mTdcProjX[pixel_x][pixel_y]->GetXaxis()->CenterTitle();
  h_mTdcProjX[pixel_x][pixel_y]->GetXaxis()->SetNdivisions(505);
  h_mTdcProjX[pixel_x][pixel_y]->SetLineColor(1);
  h_mTdcProjX[pixel_x][pixel_y]->Draw();

  TF1 *f_gaus = new TF1("f_gaus","gaus",0,5000);
  f_gaus->SetParameter(0,100);
  f_gaus->SetParameter(1,0.5*(tdc_Start+tdc_Stop));
  f_gaus->SetParameter(2,0.5*(tdc_Stop-tdc_Start));
  f_gaus->SetRange(tdc_Start,tdc_Stop);
  h_mTdcProjX[pixel_x][pixel_y]->Fit(f_gaus,"MNQR");

  float norm = f_gaus->GetParameter(0);
  float mean = f_gaus->GetParameter(1);
  float sigma = f_gaus->GetParameter(2);
  f_gaus->SetParameter(0,norm);
  f_gaus->SetParameter(1,mean);
  f_gaus->SetParameter(2,sigma);
  f_gaus->SetRange(mean-3.0*sigma,mean+3.0*sigma);
  h_mTdcProjX[pixel_x][pixel_y]->Fit(f_gaus,"MNQR");

  f_gaus->SetLineColor(2);
  f_gaus->SetLineWidth(2);
  f_gaus->SetLineStyle(2);
  f_gaus->Draw("l same");

  //------------------------------------------------------
  c_TimeCuts->cd(1); // tdc vs. time duration
  h_mTDC[pixel_x][pixel_y]->SetTitle("TDC vs. Time");
  h_mTDC[pixel_x][pixel_y]->SetStats(0);
  h_mTDC[pixel_x][pixel_y]->GetXaxis()->SetTitle("tdc (ns)");
  h_mTDC[pixel_x][pixel_y]->GetXaxis()->CenterTitle();
  h_mTDC[pixel_x][pixel_y]->GetXaxis()->SetNdivisions(505);
  h_mTDC[pixel_x][pixel_y]->GetYaxis()->SetTitle("time (ns)");
  h_mTDC[pixel_x][pixel_y]->GetYaxis()->CenterTitle();
  h_mTDC[pixel_x][pixel_y]->GetYaxis()->SetNdivisions(505);
  h_mTDC[pixel_x][pixel_y]->Draw("colz");

  PlotLine(mean_tdc[pixel_x][pixel_y]-3.0*sigma_tdc[pixel_x][pixel_y],mean_tdc[pixel_x][pixel_y]-3.0*sigma_tdc[pixel_x][pixel_y],0.0,149,2,2,2);
  PlotLine(mean_tdc[pixel_x][pixel_y]+3.0*sigma_tdc[pixel_x][pixel_y],mean_tdc[pixel_x][pixel_y]+3.0*sigma_tdc[pixel_x][pixel_y],0.0,149,2,2,2);

  //------------------------------------------------------
  c_TimeCuts->cd(3);
  title = Form("Time => pixel_x %d & pixel_y %d",pixel_x,pixel_y);
  h_mTdcProjY[pixel_x][pixel_y]->SetTitle(title.c_str());
  h_mTdcProjY[pixel_x][pixel_y]->SetStats(0);
  h_mTdcProjY[pixel_x][pixel_y]->GetXaxis()->SetTitle("tdc (ns)");
  h_mTdcProjY[pixel_x][pixel_y]->GetXaxis()->CenterTitle();
  h_mTdcProjY[pixel_x][pixel_y]->GetXaxis()->SetNdivisions(505);
  h_mTdcProjY[pixel_x][pixel_y]->SetLineColor(1);
  h_mTdcProjY[pixel_x][pixel_y]->DrawCopy();

  TF1 *f_gaus_y = new TF1("f_gaus_y","gaus",0,150.0);
  f_gaus_y->SetParameter(0,100);
  f_gaus_y->SetParameter(1,50);
  f_gaus_y->SetParameter(2,3.0);
  f_gaus_y->SetRange(35.0,70.0);
  h_mTdcProjY[pixel_x][pixel_y]->Fit(f_gaus_y,"MNQR");

  float norm_y = f_gaus_y->GetParameter(0);
  float mean_y = f_gaus_y->GetParameter(1);
  float sigma_y = f_gaus_y->GetParameter(2);
  f_gaus_y->SetParameter(0,norm_y);
  f_gaus_y->SetParameter(1,mean_y);
  f_gaus_y->SetParameter(2,sigma_y);
  f_gaus_y->SetRange(mean_y-0.5*sigma_y,mean_y+3.0*sigma_y);
  h_mTdcProjY[pixel_x][pixel_y]->Fit(f_gaus_y,"MNQR");

  f_gaus_y->SetRange(mean_y-3.0*sigma_y,mean_y+3.0*sigma_y);
  f_gaus_y->SetLineColor(4);
  f_gaus_y->SetLineWidth(2);
  f_gaus_y->SetLineStyle(2);
  f_gaus_y->Draw("l same");
  // cout << "pixel_x = " << pixel_x << ", pixel_y = " << pixel_y << ", mean_time = " << mean_time[pixel_x][pixel_y] << ", sigma_time = " << sigma_time[pixel_x][pixel_y] << endl;

  TH2D *h_mean_tdc = new TH2D("h_mean_tdc","h_mean_tdc",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_sigma_tdc = new TH2D("h_sigma_tdc","h_sigma_tdc",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_mean_time = new TH2D("h_mean_time","h_mean_time",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_sigma_time = new TH2D("h_sigma_time","h_sigma_time",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      h_mean_tdc->Fill(i_pixel_x,i_pixel_y,mean_tdc[i_pixel_x][i_pixel_y]);
      h_sigma_tdc->Fill(i_pixel_x,i_pixel_y,sigma_tdc[i_pixel_x][i_pixel_y]);
      h_mean_time->Fill(i_pixel_x,i_pixel_y,mean_time[i_pixel_x][i_pixel_y]);
      h_sigma_time->Fill(i_pixel_x,i_pixel_y,sigma_time[i_pixel_x][i_pixel_y]);
    }
  }

  string outputfile = Form("/Users/xusun/WorkSpace/EICPID/OutPut/BeamTest/PMT/TDC/%s/richTimeCuts_%d.root",mode.c_str(),runConfig);
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();

  h_mean_tdc->Write();
  h_sigma_tdc->Write();
  h_mean_time->Write();
  h_sigma_time->Write();

  File_OutPut->Close();

  return 1;
}
