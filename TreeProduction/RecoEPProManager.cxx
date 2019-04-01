#include "RecoEPProManager.h"
#include <TMath.h>
#include <TString.h>
#include <TProfile2D.h>

#include <iostream>
#include <string>

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
      p_mQx_BbcSouth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),10000,405000.5,415000.5,vecMesonFlow::mNumOfVertex,-0.5,19.5);

      ProName = Form("p_mQy_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcSouth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),10000,405000.5,415000.5,vecMesonFlow::mNumOfVertex,-0.5,19.5);

      ProName = Form("p_mQx_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcNorth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),10000,405000.5,415000.5,vecMesonFlow::mNumOfVertex,-0.5,19.5);

      ProName = Form("p_mQy_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcNorth[i_order][i_vtx] = new TProfile2D(ProName.c_str(),ProName.c_str(),10000,405000.5,415000.5,vecMesonFlow::mNumOfVertex,-0.5,19.5);
    }
  }
}

void RecoEPProManager::fillPro_BbcSouthReCenter(TVector2 QVec_BbcSouth, float QWeight_BbcSouth, int order, int vtx, int runId, int cent20)
{
  float Qx_South = QVec_BbcSouth.X()/QWeight_BbcSouth;
  float Qy_South = QVec_BbcSouth.Y()/QWeight_BbcSouth;
  p_mQx_BbcSouth[order][vtx]->Fill((double)runId,(double)cent20,(double)Qx_South);
  p_mQy_BbcSouth[order][vtx]->Fill((double)runId,(double)cent20,(double)Qy_South);
}

void RecoEPProManager::fillPro_BbcNorthReCenter(TVector2 QVec_BbcNorth, float QWeight_BbcNorth, int order, int vtx, int runId, int cent20)
{
  float Qx_North = QVec_BbcNorth.X()/QWeight_BbcNorth;
  float Qy_North = QVec_BbcNorth.Y()/QWeight_BbcNorth;
  p_mQx_BbcNorth[order][vtx]->Fill((double)runId,(double)cent20,(double)Qx_North);
  p_mQy_BbcNorth[order][vtx]->Fill((double)runId,(double)cent20,(double)Qy_North);
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
