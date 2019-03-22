#include "RecoEPHistoManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TString.h"

ClassImp(RecoEPHistoManager)

//------------------------------------------------------------
RecoEPHistoManager::RecoEPHistoManager()
{
}

RecoEPHistoManager::~RecoEPHistoManager()
{
}
//------------------------------------------------------------

int RecoEPHistoManager::initQA_Global()
{
  h_mVtZ_Zdc = new TH1F("h_mVtZ_Zdc","h_mVtZ_Zdc",201,-100.5,100.5);
  h_mVtZ_Bbc = new TH1F("h_mVtZ_Bbc","h_mVtZ_Bbc",201,-100.5,100.5);
  h_mCentrality = new TH1F("h_mCentrality","h_mCentrality",101,0.5,100.5);

  return 1;
}

int RecoEPHistoManager::fillQA_Global(double vtx_zdc, double vtx_bbc, double centrality)
{
  h_mVtZ_Zdc->Fill(vtx_zdc);
  h_mVtZ_Bbc->Fill(vtx_bbc);
  h_mCentrality->Fill(centrality);

  return 1;
}

int RecoEPHistoManager::writeQA_Global()
{
  h_mVtZ_Bbc->Write();
  h_mVtZ_Zdc->Write();
  h_mCentrality->Write();

  return 1;
}
