#include <string>
#include <TFile.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TLegend.h>
#include <TCanvas.h>

void plotQA_CorrPar()
{
  string input_recenterEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_RawEP_ReCenterPar.root";
  TFile *File_InPut_ReCenter = TFile::Open(input_recenterEP.c_str());
  TProfile2D *p_mQx_BbcNorth_2nd_Vtx_2 = (TProfile2D*)File_InPut_ReCenter->Get("p_mQx_BbcNorth_2nd_Vtx_2");
  TProfile *p_mQx_BbcNorth = (TProfile*)p_mQx_BbcNorth_2nd_Vtx_2->ProjectionY()->Clone();

  string input_shiftEP = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/file_AuAu200GeV_ReCenterEP_ShiftPar.root";
  TFile *File_InPut_Shift = TFile::Open(input_shiftEP.c_str());
  TProfile2D *p_mCos_BbcNorth_2nd_Vtx_2_Shift_7 = (TProfile2D*)File_InPut_Shift->Get("p_mCos_BbcNorth_2nd_Vtx_2_Shift_7");
  TProfile *p_mCos_BbcNorth = (TProfile*)p_mCos_BbcNorth_2nd_Vtx_2_Shift_7->ProjectionY()->Clone();

  TCanvas *c_correction = new TCanvas("c_correction","c_correction",10,10,1600,800);
  c_correction->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_correction->cd(i_pad+1);
    c_correction->cd(i_pad+1)->SetLeftMargin(0.15);
    c_correction->cd(i_pad+1)->SetBottomMargin(0.15);
    c_correction->cd(i_pad+1)->SetTicks(1,1);
    c_correction->cd(i_pad+1)->SetGrid(0,0);
  }

  c_correction->cd(1);
  p_mQx_BbcNorth->GetXaxis()->SetTitle("Centrality (5%)");
  p_mQx_BbcNorth->GetYaxis()->SetTitle("<Qx>");
  p_mQx_BbcNorth->Draw("pE");

  c_correction->cd(2);
  p_mCos_BbcNorth->GetXaxis()->SetTitle("Centrality (5%)");
  p_mCos_BbcNorth->GetYaxis()->SetTitle("<cos(7*2*#Psi)>");
  p_mCos_BbcNorth->Draw("pE");

  c_correction->SaveAs("/direct/phenix+u/xusun/WorkSpace/PHENIX/figures/c_correction.eps");
}
