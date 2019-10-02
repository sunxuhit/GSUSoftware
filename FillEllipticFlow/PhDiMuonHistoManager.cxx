#include <TH2F.h>
#include <TH1F.h>
#include <TMath.h>
#include <TString.h>

#include <iostream>
#include <string>

#include "./PhDiMuonHistoManager.h"

ClassImp(PhDiMuonHistoManager)

using namespace std;

//------------------------------------------------------------

PhDiMuonHistoManager::PhDiMuonHistoManager()
{
}

PhDiMuonHistoManager::~PhDiMuonHistoManager()
{
}

//------------------------------------------------------------
//===============di-muon InvMass==============
void PhDiMuonHistoManager::initHist_InvMass(int pid, int mode)
{
  // histograms for ellpitic flow
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      for(int i_phi = 0; i_phi < vecMesonFlow::mNumOfPhi; i_phi++) // phi-psi bin
      {
	string KEY_InvMass = Form("InvMass_%s_Pt_%d_Centrality_%d_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),i_pt,i_cent,i_phi,vecMesonFlow::mEventType[mode].c_str());
	h_mInvMass[KEY_InvMass] = new TH1F(KEY_InvMass.c_str(),KEY_InvMass.c_str(),100,0.0,5.0);
	h_mInvMass[KEY_InvMass]->Sumw2();

	string KEY_InvMass_Raw = Form("InvMassRaw_%s_Pt_%d_Centrality_%d_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),i_pt,i_cent,i_phi,vecMesonFlow::mEventType[mode].c_str());
	h_mInvMass_Raw[KEY_InvMass_Raw] = new TH1F(KEY_InvMass_Raw.c_str(),KEY_InvMass_Raw.c_str(),100,0.0,5.0);
	h_mInvMass_Raw[KEY_InvMass_Raw]->Sumw2();
      }
    }
  }
}

void PhDiMuonHistoManager::writeHist_InvMass(int pid, int mode)
{
  // histograms for ellpitic flow
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      for(int i_phi = 0; i_phi < vecMesonFlow::mNumOfPhi; i_phi++) // phi-psi bin
      {
	string KEY_InvMass = Form("InvMass_%s_Pt_%d_Centrality_%d_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),i_pt,i_cent,i_phi,vecMesonFlow::mEventType[mode].c_str());
	h_mInvMass[KEY_InvMass]->Write();

	string KEY_InvMass_Raw = Form("InvMassRaw_%s_Pt_%d_Centrality_%d_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),i_pt,i_cent,i_phi,vecMesonFlow::mEventType[mode].c_str());
	h_mInvMass_Raw[KEY_InvMass_Raw]->Write();
      }
    }
  }
}

//------------------------------------------------------------
//=============utility functions==============
int PhDiMuonHistoManager::getPtBin(float pt)
{
  // 0: (0.0,2.0], 1: (2.0-4.0], 2: (4.0,6.0], 3: (6.0,10.0]
  // if(TMath::Abs(pt-vecMesonFlow::mPtStart[0]) < 1e-10) return vecMesonFlow::mPtBin[0]; // pt=0.0 == ptbin=0 
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; ++i_pt)
  {
    if(pt > vecMesonFlow::mPtStart[i_pt] && pt < vecMesonFlow::mPtStop[i_pt])
    {
      return vecMesonFlow::mPtBin[i_pt];
    }
    if(TMath::Abs(pt-vecMesonFlow::mPtStop[i_pt]) < 1e-10) return vecMesonFlow::mPtBin[i_pt]; // ptbin == mPtStop
  }

  return -1;
}
