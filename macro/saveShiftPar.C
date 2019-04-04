#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

void saveShiftPar()
{
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TProfile2D *p_mCos_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
  TProfile2D *p_mSin_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
  TProfile2D *p_mCos_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
  TProfile2D *p_mSin_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];

  File_InPut->cd();
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
	p_mCos_BbcSouth[i_order][i_vtx][i_shift] = (TProfile2D*)File_InPut->Get(ProName.c_str());

	ProName = Form("p_mSin_BbcSouth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mSin_BbcSouth[i_order][i_vtx][i_shift] = (TProfile2D*)File_InPut->Get(ProName.c_str());

	ProName = Form("p_mCos_BbcNorth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mCos_BbcNorth[i_order][i_vtx][i_shift] = (TProfile2D*)File_InPut->Get(ProName.c_str());

	ProName = Form("p_mSin_BbcNorth_%s_Vtx_%d_Shift_%d",Order[i_order].c_str(),i_vtx,i_shift);
	p_mSin_BbcNorth[i_order][i_vtx][i_shift] = (TProfile2D*)File_InPut->Get(ProName.c_str());

	std::cout << "saving " << ProName.c_str() << "!" << std::endl;
      }
    }
  }

  string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ShiftPar.root";
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
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
  File_OutPut->Close();

  File_InPut->Close();
}
