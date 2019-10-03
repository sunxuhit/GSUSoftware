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
  // 0: (0.0,2.0], 1: (2.0,4.0], 2: (4.0,6.0], 3: (6.0,10.0]
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; ++i_pt)
  {
    if(pt > vecMesonFlow::mPtStart[i_pt] && pt < vecMesonFlow::mPtStop[i_pt])
    {
      return vecMesonFlow::mPtBin[i_pt];
    }
    if(TMath::Abs(pt-vecMesonFlow::mPtStop[i_pt]) < FLT_EPSILON) 
    {
      return vecMesonFlow::mPtBin[i_pt]; // ptbin == mPtStop
    }
  }

  return -1;
}

int PhDiMuonHistoManager::getPhiBin(float phi)
{
  // 0: [0.0,1.0], 1: (1.0,2.0], 2: (2.0,3.0], 3: (3.0,4.0], 4: (4.0,5.0], 5: (5.0,6.0], 6: (6.0,7.0]
  // * 0.5*TMath::Pi()

  float phi_shift = -999.9;
  for(int i_Psi = 0; i_Psi < vecMesonFlow::mNumOfPsi; ++i_Psi)
  { // shift phi-Psi2 to [-pi/2,pi/2]
    if(phi > vecMesonFlow::mPsiStart[i_Psi]*TMath::PiOver2() && phi < vecMesonFlow::mPsiStop[i_Psi]*TMath::PiOver2())
    {
      phi_shift = phi - (i_Psi-1)*TMath::Pi();
    }
    if( (i_Psi != 1) && (TMath::Abs(phi-vecMesonFlow::mPsiStart[i_Psi]*TMath::PiOver2()) < FLT_EPSILON) ) 
    { // shift lower edage to -pi/2
      phi_shift = phi - (i_Psi-1)*TMath::Pi();
    }
    if( (i_Psi != 1) && (TMath::Abs(phi-vecMesonFlow::mPsiStop[i_Psi]*TMath::PiOver2()) < FLT_EPSILON) ) 
    { // shift upper edage to pi/2
      phi_shift = phi - (i_Psi-1)*TMath::Pi();
    }
  }
  // cout << "phi = " << phi << ", phi_shift = " << phi_shift << endl;

  for(int i_phi = 0; i_phi < vecMesonFlow::mNumOfPhi; ++i_phi) // phi-Psi2 bin
  {
    if(TMath::Abs(phi_shift) > vecMesonFlow::mPhiStart[i_phi]*TMath::PiOver2()/7.0 && TMath::Abs(phi_shift) < vecMesonFlow::mPhiStop[i_phi]*TMath::PiOver2()/7.0)
    {
      return vecMesonFlow::mPhiBin[i_phi];
    }
    if( (i_phi == 0) && (TMath::Abs(TMath::Abs(phi_shift)-vecMesonFlow::mPhiStart[i_phi]*TMath::PiOver2()/7.0) < FLT_EPSILON) )
    { // special case for the lower edge in first bin
      return vecMesonFlow::mPhiBin[i_phi]; // phibin == mPhiStart
    }
    if(TMath::Abs(TMath::Abs(phi_shift)-vecMesonFlow::mPhiStop[i_phi]*TMath::PiOver2()/7.0) < FLT_EPSILON) 
    {
      return vecMesonFlow::mPhiBin[i_phi]; // phibin == mPhiStop
    }
  }

  return -1;
}
