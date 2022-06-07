#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include <cmath>
#include <cassert>
#include <string>
#include <iostream>
#include <utility>
#include <functional>
#include <stdexcept>
using namespace std;

void plotQA_RingImage()
{
  string inputfile = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/Simulation/PMT/GEMC_Calibration.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2D *h_mPhotonDist= (TH2D*)File_InPut->Get("h_mPhotonDist")->Clone();

  TCanvas *c_RingImage = new TCanvas("c_RingImage","c_RingImage",10,10,500,500);
  c_RingImage->cd()->SetLeftMargin(0.15);
  c_RingImage->cd()->SetBottomMargin(0.15);
  c_RingImage->cd()->SetRightMargin(0.15);
  c_RingImage->cd()->SetTicks(1,1);
  c_RingImage->cd()->SetGrid(0,0);
  c_RingImage->cd()->SetLogz();

  h_mPhotonDist->SetTitle("");
  h_mPhotonDist->SetStats(0);
  h_mPhotonDist->GetXaxis()->SetTitle("x (mm)");
  h_mPhotonDist->GetXaxis()->CenterTitle();
  h_mPhotonDist->GetYaxis()->SetTitle("y (mm)");
  h_mPhotonDist->GetYaxis()->CenterTitle();
  h_mPhotonDist->Draw("colz");

  c_RingImage->SaveAs("/Users/xusun/WorkSpace/Library/Presentations/GSU/Conference/EICPID_July_2019/figures/c_RingImage_GEMC.png");
}
