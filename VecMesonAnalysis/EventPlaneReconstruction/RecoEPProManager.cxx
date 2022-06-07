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

//===============BBC Event Plane ReCenter=========================
void RecoEPProManager::initPro_BbcReCenter()
{
  std::cout << "initialize BBC EP ReCenter Correction Profiles!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      std::string ProName;

      ProName = Form("p_mQx_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcSouth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

      ProName = Form("p_mQy_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcSouth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

      ProName = Form("p_mQx_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcNorth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

      ProName = Form("p_mQy_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcNorth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);
    }
  }
}

void RecoEPProManager::fillPro_BbcSouthReCenter(TVector2 QVecRaw_BbcSouth, float QWeight_BbcSouth, int order, int vtx4, int runIndex, int cent20)
{
  float Qx_South = QVecRaw_BbcSouth.X()/QWeight_BbcSouth;
  float Qy_South = QVecRaw_BbcSouth.Y()/QWeight_BbcSouth;
  p_mQx_BbcSouth[order][vtx4]->Fill((double)runIndex,(double)cent20,(double)Qx_South);
  p_mQy_BbcSouth[order][vtx4]->Fill((double)runIndex,(double)cent20,(double)Qy_South);
}

void RecoEPProManager::fillPro_BbcNorthReCenter(TVector2 QVecRaw_BbcNorth, float QWeight_BbcNorth, int order, int vtx4, int runIndex, int cent20)
{
  float Qx_North = QVecRaw_BbcNorth.X()/QWeight_BbcNorth;
  float Qy_North = QVecRaw_BbcNorth.Y()/QWeight_BbcNorth;
  p_mQx_BbcNorth[order][vtx4]->Fill((double)runIndex,(double)cent20,(double)Qx_North);
  p_mQy_BbcNorth[order][vtx4]->Fill((double)runIndex,(double)cent20,(double)Qy_North);
}

void RecoEPProManager::writePro_BbcReCenter()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      p_mQx_BbcSouth[i_order][i_vtx]->Write();
      p_mQy_BbcSouth[i_order][i_vtx]->Write();
      p_mQx_BbcNorth[i_order][i_vtx]->Write();
      p_mQy_BbcNorth[i_order][i_vtx]->Write();
    }
  }
}
//===============BBC Event Plane ReCenter=========================

//===============BBC Event Plane Shift=========================
void RecoEPProManager::initPro_BbcShift()
{
  std::cout << "initialize BBC EP Shift Correction Profiles!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      for(int i_shift = 0; i_shift < vecMesonFlow::mNumOfShiftOrder; ++i_shift)
      {
	std::string ProName;

	ProName = Form("p_mCos_BbcSouth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mCos_BbcSouth[i_order][i_vtx][i_shift] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

	ProName = Form("p_mSin_BbcSouth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mSin_BbcSouth[i_order][i_vtx][i_shift] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

	ProName = Form("p_mCos_BbcNorth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mCos_BbcNorth[i_order][i_vtx][i_shift] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

	ProName = Form("p_mSin_BbcNorth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mSin_BbcNorth[i_order][i_vtx][i_shift] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);
      }
    }
  }
}

void RecoEPProManager::fillPro_BbcSouthShift(float PsiReCenter_BbcSouth, int order, int vtx4, int runIndex, int cent20)
{
  for(int i_shift = 0; i_shift < vecMesonFlow::mNumOfShiftOrder; ++i_shift)
  {
    float shiftorder = vecMesonFlow::mShiftOrder[i_shift];
    float harmonic = vecMesonFlow::mHarmonic[order];
    float Psi_Cos = TMath::Cos(shiftorder*harmonic*PsiReCenter_BbcSouth);
    float Psi_Sin = TMath::Sin(shiftorder*harmonic*PsiReCenter_BbcSouth);
    // std::cout << "Psi_Cos = " << Psi_Cos << ", Psi_Sin = " << Psi_Sin << std::endl;
    p_mCos_BbcSouth[order][vtx4][i_shift]->Fill((double)runIndex,(double)cent20,(double)Psi_Cos);
    p_mSin_BbcSouth[order][vtx4][i_shift]->Fill((double)runIndex,(double)cent20,(double)Psi_Sin);
  }
}

void RecoEPProManager::fillPro_BbcNorthShift(float PsiReCenter_BbcNorth, int order, int vtx4, int runIndex, int cent20)
{
  for(int i_shift = 0; i_shift < vecMesonFlow::mNumOfShiftOrder; ++i_shift)
  {
    float shiftorder = vecMesonFlow::mShiftOrder[i_shift];
    float harmonic = vecMesonFlow::mHarmonic[order];
    float Psi_Cos = TMath::Cos(shiftorder*harmonic*PsiReCenter_BbcNorth);
    float Psi_Sin = TMath::Sin(shiftorder*harmonic*PsiReCenter_BbcNorth);
    // std::cout << "Psi_Cos = " << Psi_Cos << ", Psi_Sin = " << Psi_Sin << std::endl;
    p_mCos_BbcNorth[order][vtx4][i_shift]->Fill((double)runIndex,(double)cent20,(double)Psi_Cos);
    p_mSin_BbcNorth[order][vtx4][i_shift]->Fill((double)runIndex,(double)cent20,(double)Psi_Sin);
  }
}

void RecoEPProManager::writePro_BbcShift()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      for(int i_shift = 0; i_shift < vecMesonFlow::mNumOfShiftOrder; ++i_shift)
      {
	p_mCos_BbcSouth[i_order][i_vtx][i_shift]->Write();
	p_mSin_BbcSouth[i_order][i_vtx][i_shift]->Write();
	p_mCos_BbcNorth[i_order][i_vtx][i_shift]->Write();
	p_mSin_BbcNorth[i_order][i_vtx][i_shift]->Write();
      }
    }
  }
}

//===============BBC Event Plane Shift=========================

//===============BBC Event Plane Resolution=========================
void RecoEPProManager::initPro_BbcResolution()
{
  std::cout << "initialize BBC sub Event Plane Resolution !" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    std::string ProName = Form("p_mResolution_BbcSub_%s",Order[i_order].c_str());
    p_mResolution_BbcSub[i_order] = new TProfile(ProName.c_str(),ProName.c_str(),vecMesonFlow::mNumOfCentrality20,-0.5,19.5);

    ProName = Form("p_mResQA_BbcSub_%s",Order[i_order].c_str());
    p_mResQA_BbcSub[i_order] = new TProfile2D(ProName.c_str(),ProName.c_str(),1500,-0.5,1499.5,vecMesonFlow::mNumOfCentrality20,-0.5,19.5);
  }
}

void RecoEPProManager::fillPro_BbcResolution(float PsiReCenter_BbcSouth, float PsiReCenter_BbcNorth, int order, int runIndex, int cent20)
{ // PsiShift_BbcNorth - PsiShift_BbcSouth
  float harmonic = vecMesonFlow::mHarmonic[order];
  float resolution = TMath::Cos(harmonic*(PsiReCenter_BbcNorth-PsiReCenter_BbcSouth));
  p_mResolution_BbcSub[order]->Fill((double)cent20,(double)resolution);
  p_mResQA_BbcSub[order]->Fill((double)runIndex,(double)cent20,(double)resolution);
}

void RecoEPProManager::writePro_BbcResolution()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    p_mResolution_BbcSub[i_order]->Write();
    p_mResQA_BbcSub[i_order]->Write();
  }
}

//===============BBC Event Plane Resolution=========================

