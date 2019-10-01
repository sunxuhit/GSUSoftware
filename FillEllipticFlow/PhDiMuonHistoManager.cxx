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
      }
    }
  }
}
