#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

void saveReCenterPar()
{
  // string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_RawEP_ReCenterPar.root";
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/taxi_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TProfile2D *p_mQx_BbcSouth[3][vecMesonFlow::mNumOfVertex]; // 0 is flow harmonic | 1 is vertex bin
  TProfile2D *p_mQy_BbcSouth[3][vecMesonFlow::mNumOfVertex];
  TProfile2D *p_mQx_BbcNorth[3][vecMesonFlow::mNumOfVertex];
  TProfile2D *p_mQy_BbcNorth[3][vecMesonFlow::mNumOfVertex];

  File_InPut->cd();
  std::cout << "read in BBC EP ReCenter Correction Profiles!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      std::string ProName;

      ProName = Form("p_mQx_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcSouth[i_order][i_vtx] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcSouth[i_order][i_vtx] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQx_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcNorth[i_order][i_vtx] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();

      ProName = Form("p_mQy_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcNorth[i_order][i_vtx] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();
    }
  }

  string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ReCenterPar.root";
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
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
  File_OutPut->Close();

  File_InPut->Close();
}
