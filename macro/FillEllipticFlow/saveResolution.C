#include <string>
#include <TString.h>
#include <TFile.h>
#include <TProfile2D.h>
#include <TProfile.h>
#include <TH1F.h>
#include "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/TreeProduction/PhVecMesonCons.h"

void saveResolution()
{
  string inputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/condor_AuAu200GeV_Resolution.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  TProfile2D *p_mResQA_BbcSub[3];
  TProfile *p_mResolution_BbcSub[3];
  TH1F *h_mCentrality_Bbc[3];

  TProfile2D *p_mResQA_FvtxSub[3];
  TProfile *p_mResolution_FvtxSub[3];
  TH1F *h_mCentrality_Fvtx[3];

  File_InPut->cd();
  std::cout << "read in BBC & FVTX EP Resolution and Centrality!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    // BBC EP
    std::string ProName = Form("p_mResQA_BbcSub_%s",Order[i_order].c_str());
    p_mResQA_BbcSub[i_order] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();

    ProName = Form("p_mResolution_BbcSub_%s",Order[i_order].c_str());
    p_mResolution_BbcSub[i_order] = (TProfile*)File_InPut->Get(ProName.c_str())->Clone();
    
    ProName = Form("h_mCentrality_Bbc_%s",Order[i_order].c_str());
    h_mCentrality_Bbc[i_order] = (TH1F*)File_InPut->Get(ProName.c_str())->Clone();

    // FVTX EP
    ProName = Form("p_mResQA_FvtxSub_%s",Order[i_order].c_str());
    p_mResQA_FvtxSub[i_order] = (TProfile2D*)File_InPut->Get(ProName.c_str())->Clone();

    ProName = Form("p_mResolution_FvtxSub_%s",Order[i_order].c_str());
    p_mResolution_FvtxSub[i_order] = (TProfile*)File_InPut->Get(ProName.c_str())->Clone();
    
    ProName = Form("h_mCentrality_Fvtx_%s",Order[i_order].c_str());
    h_mCentrality_Fvtx[i_order] = (TH1F*)File_InPut->Get(ProName.c_str())->Clone();
  }

  string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_Resolution.root";
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    // BBC EP
    p_mResQA_BbcSub[i_order]->Write();
    p_mResolution_BbcSub[i_order]->Write();
    h_mCentrality_Bbc[i_order]->Write();

    // FVTX EP
    p_mResQA_FvtxSub[i_order]->Write();
    p_mResolution_FvtxSub[i_order]->Write();
    h_mCentrality_Fvtx[i_order]->Write();
  }
  File_OutPut->Close();

  File_InPut->Close();
}
