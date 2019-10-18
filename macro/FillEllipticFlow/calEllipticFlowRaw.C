#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <map>
#include "../../FillEllipticFlow/PhVecMesonCons.h"
#include "./draw.h"

typedef std::map<std::string,TH1F*> TH1FMap;

using namespace std;

string genKey_InvMassRaw(int pid, int mode, int pt_bin, int cent4, int phi_bin);
string genKey_InvMass(int pid, int pt_bin, int cent4, int phi_bin);

void calEllipticFlowRaw(int pid = 0)
{
  // string input_US = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[0].c_str());
  string input_US = Form("/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[0].c_str());
  cout << "Open input file for US: " << input_US.c_str() << endl;
  TFile *File_InPutUS = TFile::Open(input_US.c_str());

  // string input_LS = Form("/gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[1].c_str());
  string input_LS = Form("/Users/xusun/WorkSpace/PHENIX/Data/DiMuonYields/Yields_200GeV_%s_%s.root",vecMesonFlow::mPID[pid].c_str(),vecMesonFlow::mEventType[1].c_str());
  cout << "Open input file for LS: " << input_LS.c_str() << endl;
  TFile *File_InPutLS = TFile::Open(input_LS.c_str());

  TH1FMap h_mInvMass_US, h_mInvMass_LS;
  TH1FMap  h_mInvMass, h_mInvMass_Inte;
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      string KEY_Inte = genKey_InvMass(pid, i_pt, i_cent, 5);
      for(int i_phi = 0; i_phi < vecMesonFlow::mNumOfPhi; i_phi++) // phi-psi bin
      {
	string KEY_InvMass_US = genKey_InvMassRaw(pid, 0, i_pt, i_cent, i_phi);
	cout << KEY_InvMass_US.c_str() << endl;
	h_mInvMass_US[KEY_InvMass_US] = (TH1F*)File_InPutUS->Get(KEY_InvMass_US.c_str());

	string KEY_InvMass_LS = genKey_InvMassRaw(pid, 1, i_pt, i_cent, i_phi);
	cout << KEY_InvMass_LS.c_str() << endl;
	h_mInvMass_LS[KEY_InvMass_LS] = (TH1F*)File_InPutLS->Get(KEY_InvMass_LS.c_str());

	string KEY_InvMass = genKey_InvMass(pid, i_pt, i_cent, i_phi);
	h_mInvMass[KEY_InvMass] = (TH1F*)h_mInvMass_US[KEY_InvMass_US]->Clone(KEY_InvMass.c_str());
	h_mInvMass[KEY_InvMass]->Add(h_mInvMass_LS[KEY_InvMass_LS],-1.0);

	if(i_phi == 0) h_mInvMass_Inte[KEY_Inte] = (TH1F*)h_mInvMass[KEY_InvMass]->Clone();
	else h_mInvMass_Inte[KEY_Inte]->Add(h_mInvMass[KEY_InvMass],1.0);
      }
    }
  }

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1200,800);
  c_play->Divide(3,2);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_play->cd(i_pad+1);
    if(i_pad < 5)
    {
      string KEY_InvMass = genKey_InvMass(pid, 1, 3, i_pad);
      h_mInvMass[KEY_InvMass]->RebinX(2);
      h_mInvMass[KEY_InvMass]->GetXaxis()->SetRangeUser(2.0,5.0);
      h_mInvMass[KEY_InvMass]->Draw("hE");

      int bin_start = h_mInvMass[KEY_InvMass]->GetXaxis()->FindBin(2.8);
      int bin_stop  = h_mInvMass[KEY_InvMass]->GetXaxis()->FindBin(3.5);
      float yields = 0.0;
      float error = 0.0;
      for(int i_bin = bin_start; i_bin < bin_stop; ++i_bin)
      {
	yields += h_mInvMass[KEY_InvMass]->GetBinContent(i_bin);
	error += h_mInvMass[KEY_InvMass]->GetBinError(i_bin)*h_mInvMass[KEY_InvMass]->GetBinError(i_bin);
      }
      string leg_yields = Form("%1.1f #pm %1.1f",yields, sqrt(error));
      plotTopLegend((char*)leg_yields.c_str(),0.5,0.6,0.06,1,0.0,42,1,1);
    }
    
    if(i_pad == 5)
    {
      string KEY_Inte = genKey_InvMass(pid, 1, 3, i_pad);
      h_mInvMass_Inte[KEY_Inte]->RebinX(2);
      h_mInvMass_Inte[KEY_Inte]->GetXaxis()->SetRangeUser(2.0,5.0);
      h_mInvMass_Inte[KEY_Inte]->Draw("hE");

      int bin_start = h_mInvMass_Inte[KEY_Inte]->GetXaxis()->FindBin(2.8);
      int bin_stop  = h_mInvMass_Inte[KEY_Inte]->GetXaxis()->FindBin(3.5);
      float yields = 0.0;
      float error = 0.0;
      for(int i_bin = bin_start; i_bin < bin_stop; ++i_bin)
      {
	yields += h_mInvMass_Inte[KEY_Inte]->GetBinContent(i_bin);
	error += h_mInvMass_Inte[KEY_Inte]->GetBinError(i_bin)*h_mInvMass_Inte[KEY_Inte]->GetBinError(i_bin);
      }
      string leg_yields = Form("%1.1f #pm %1.1f",yields, sqrt(error));
      plotTopLegend((char*)leg_yields.c_str(),0.5,0.6,0.06,1,0.0,42,1,1);
    }
  }

}

string genKey_InvMassRaw(int pid, int mode, int pt_bin, int cent4, int phi_bin)
{
  string KEY_InvMass_Raw = Form("InvMassRaw_%s_Pt_%d_Centrality_%s_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),phi_bin,vecMesonFlow::mEventType[mode].c_str());

  return KEY_InvMass_Raw;
}

string genKey_InvMass(int pid, int pt_bin, int cent4, int phi_bin)
{
  string KEY_InvMass_Raw = Form("InvMassRaw_%s_Pt_%d_Centrality_%s_Phi_%d_2nd",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),phi_bin);

  return KEY_InvMass_Raw;
}

