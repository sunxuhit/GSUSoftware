
void plotQA_PMT_GEMCRing(const int runID = 24)
{
  string mode = "PositionScan";
  string input_beam = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/%s/richTDC_run%d.root",mode.c_str(),runID);
  TFile *File_Beam = TFile::Open(input_beam.c_str());
  TH2D *h_mRingFinder1 = (TH2D*)File_Beam->Get("h_mRingFinder1")->Clone();
  TH2D *h_mRingFinder2 = (TH2D*)File_Beam->Get("h_mRingFinder2")->Clone();
  TH2D *h_mRingFinder3 = (TH2D*)File_Beam->Get("h_mRingFinder3")->Clone();

  string input_gemc = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/Simulation/PMT/GEMC_Calibration.root";
  TFile *File_GEMC = TFile::Open(input_gemc.c_str());
  TH2D *h_mPhotonDist = (TH2D*)File_GEMC->Get("h_mPhotonDist")->Clone();

  TCanvas *c_RingImage = new TCanvas("c_RingImage","c_RingImage",10,10,800,800);
  c_RingImage->cd()->SetLeftMargin(0.15);
  c_RingImage->cd()->SetBottomMargin(0.15);
  c_RingImage->cd()->SetRightMargin(0.15);
  c_RingImage->cd()->SetTicks(1,1);
  c_RingImage->cd()->SetGrid(0,0);

  h_mPhotonDist->SetTitle("");
  h_mPhotonDist->SetStats(0);
  h_mPhotonDist->GetXaxis()->SetTitle("x (mm)");
  h_mPhotonDist->GetXaxis()->CenterTitle();
  h_mPhotonDist->GetYaxis()->SetTitle("y (mm)");
  h_mPhotonDist->GetYaxis()->CenterTitle();
  h_mPhotonDist->Draw();

  h_mRingFinder1->Draw("col same");

  string c_ringimage = Form("/Users/xusun/WorkSpace/EICPID/figures/BeamTest_mRICH/QA/PMT/%s/c_GEMCRing_PMT_%d.eps",mode.c_str(),runID);
  c_RingImage->SaveAs(c_ringimage.c_str());
}
