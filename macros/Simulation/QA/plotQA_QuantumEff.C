#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"

void plotQA_QuantumEff()
{
  const double qe_PMT[45] = { 
    11.50,16.83,22.45,28.18,32.87,33.37,33.63,33.73,33.46,33.12,
    32.74,32.42,32.45,31.54,30.76,29.76,28.77,27.59,26.16,24.31,
    22.48,20.93,19.67,18.49,16.50,13.05,10.11,8.36 ,7.15 ,6.12 ,
    5.19 ,4.29 ,3.46 ,2.69 ,2.00 ,1.41 ,0.95 ,0.60 ,0.35 ,0.19 ,
    0.10 ,0.05 ,0.02 ,0.01 ,0.00};

  const double lambda_PMT[45] = { 
    260,270,280,290,300,310,320,330,340,350,
    360,370,380,390,400,410,420,430,440,450,
    460,470,480,490,500,510,520,530,540,550,
    560,570,580,590,600,610,620,630,640,650,
    660,670,680,690,700};

  TGraph *g_quanEff = new TGraph(45,lambda_PMT,qe_PMT);
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->cd();
  c_play->cd()->SetLeftMargin(0.15);
  c_play->cd()->SetBottomMargin(0.15);
  c_play->cd()->SetGrid(1,1);
  c_play->cd()->SetTicks(1,1);
  c_play->cd()->SetLogy();

  TH1F *h_play = new TH1F("h_play","h_play",600,199.5,799.5);
  for(int i_bin = 0; i_bin < 1000; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-10);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetTitle("");
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("wave length (nm)");
  h_play->GetXaxis()->CenterTitle();
  h_play->GetXaxis()->SetNdivisions(510);
  h_play->GetYaxis()->SetTitle("Quantum Efficiency (%)");
  h_play->GetYaxis()->CenterTitle();
  h_play->GetYaxis()->SetRangeUser(0.1,100.0);
  h_play->Draw("pE");

  g_quanEff->SetMarkerSize(1.2);
  g_quanEff->SetMarkerColor(2);
  g_quanEff->SetMarkerStyle(24);
  g_quanEff->Draw("p Same");
}
