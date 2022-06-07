#include "RecoEPProManager.h"
#include <TMath.h>
#include <TString.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include <iostream>

ClassImp(RecoEPProManager)

//------------------------------------------------------------

RecoEPProManager::RecoEPProManager()
{
}

RecoEPProManager::~RecoEPProManager()
{
}

//------------------------------------------------------------

//===============BBC Event Plane Resolution=========================
void RecoEPProManager::initPro_BbcResolution()
{
  std::cout << "initialize BBC sub Event Plane Resolution !" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    std::string ProName = Form("p_mResolution20_BbcSub_%s",Order[i_order].c_str());
    p_mResolution20_BbcSub[i_order] = new TProfile(ProName.c_str(),ProName.c_str(),vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

    ProName = Form("p_mResolution10_BbcSub_%s",Order[i_order].c_str());
    p_mResolution10_BbcSub[i_order] = new TProfile(ProName.c_str(),ProName.c_str(),vecMesonFlow::mNumOfCentrality10,-0.5,9.5);

    ProName = Form("p_mResQA_BbcSub_%s",Order[i_order].c_str());
    p_mResQA_BbcSub[i_order] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);
  }
}

void RecoEPProManager::fillPro_BbcResolution(float Psi_BbcSouth, float Psi_BbcNorth, int order, int runIndex, int cent20, int cent10)
{ // PsiShift_BbcNorth - PsiShift_BbcSouth
  float harmonic = vecMesonFlow::mHarmonic[order];
  float resolution = TMath::Cos(harmonic*(Psi_BbcNorth-Psi_BbcSouth));
  p_mResolution20_BbcSub[order]->Fill((double)cent20,(double)resolution);
  p_mResolution10_BbcSub[order]->Fill((double)cent10,(double)resolution);
  p_mResQA_BbcSub[order]->Fill((double)runIndex,(double)cent20,(double)resolution);
}

void RecoEPProManager::writePro_BbcResolution()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    p_mResolution20_BbcSub[i_order]->Write();
    p_mResolution10_BbcSub[i_order]->Write();
    p_mResQA_BbcSub[i_order]->Write();
  }
}
//===============BBC Event Plane Resolution=========================

//===============FVTX Event Plane Resolution=========================
void RecoEPProManager::initPro_FvtxResolution()
{
  std::cout << "initialize FVTX sub Event Plane Resolution !" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    std::string ProName = Form("p_mResolution20_FvtxSub_%s",Order[i_order].c_str());
    p_mResolution20_FvtxSub[i_order] = new TProfile(ProName.c_str(),ProName.c_str(),vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

    ProName = Form("p_mResolution10_FvtxSub_%s",Order[i_order].c_str());
    p_mResolution10_FvtxSub[i_order] = new TProfile(ProName.c_str(),ProName.c_str(),vecMesonFlow::mNumOfCentrality10,-0.5,9.5);

    ProName = Form("p_mResQA_FvtxSub_%s",Order[i_order].c_str());
    p_mResQA_FvtxSub[i_order] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);
  }
}

void RecoEPProManager::fillPro_FvtxResolution(float Psi_FvtxSouth, float Psi_FvtxNorth, int order, int runIndex, int cent20, int cent10)
{ // PsiShift_FvtxNorth - PsiShift_FvtxSouth
  float harmonic = vecMesonFlow::mHarmonic[order];
  float resolution = TMath::Cos(harmonic*(Psi_FvtxNorth-Psi_FvtxSouth));
  p_mResolution20_FvtxSub[order]->Fill((double)cent20,(double)resolution);
  p_mResolution10_FvtxSub[order]->Fill((double)cent10,(double)resolution);
  p_mResQA_FvtxSub[order]->Fill((double)runIndex,(double)cent20,(double)resolution);
}

void RecoEPProManager::writePro_FvtxResolution()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    p_mResolution20_FvtxSub[i_order]->Write();
    p_mResolution10_FvtxSub[i_order]->Write();
    p_mResQA_FvtxSub[i_order]->Write();
  }
}
//===============FVTX Event Plane Resolution=========================
