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
	string KEY_InvMass = genKey_InvMass(pid, mode, i_pt, i_cent, i_phi);
	h_mInvMass[KEY_InvMass] = new TH1F(KEY_InvMass.c_str(),KEY_InvMass.c_str(),100,0.0,5.0);
	h_mInvMass[KEY_InvMass]->Sumw2();

	string KEY_InvMass_Raw = genKey_InvMassRaw(pid, mode, i_pt, i_cent, i_phi);
	h_mInvMass_Raw[KEY_InvMass_Raw] = new TH1F(KEY_InvMass_Raw.c_str(),KEY_InvMass_Raw.c_str(),100,0.0,5.0);
	h_mInvMass_Raw[KEY_InvMass_Raw]->Sumw2();
      }
    }
  }
}

void PhDiMuonHistoManager::fillHist_InvMass(int pid, int mode, int cent20, int cent4, float pt, float phi, float invmass, float resolution, float reweight)
{
  const int pt_bin = getPtBin(pt);
  const int phi_bin = getPhiBin(phi);
  if(pt_bin >= 0 && phi_bin >= 0 && cent4 < 3) // miniBias
  {
    // resolution corrected flow
    string KEY_InvMass = genKey_InvMass(pid, mode, pt_bin, cent4, phi_bin);
    h_mInvMass[KEY_InvMass]->Fill(invmass,reweight/resolution);

    string KEY_InvMass_miniBias = genKey_InvMass(pid, mode, pt_bin, 3, phi_bin);
    h_mInvMass[KEY_InvMass_miniBias]->Fill(invmass,reweight/resolution);

    // raw flow
    string KEY_InvMass_Raw = genKey_InvMassRaw(pid, mode, pt_bin, cent4, phi_bin);
    h_mInvMass_Raw[KEY_InvMass_Raw]->Fill(invmass,reweight);

    string KEY_InvMass_Raw_miniBias = genKey_InvMassRaw(pid, mode, pt_bin, 3, phi_bin);
    h_mInvMass_Raw[KEY_InvMass_Raw_miniBias]->Fill(invmass,reweight);
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
	string KEY_InvMass = genKey_InvMass(pid, mode, i_pt, i_cent, i_phi);
	h_mInvMass[KEY_InvMass]->Write();

	string KEY_InvMass_Raw = genKey_InvMassRaw(pid, mode, i_pt, i_cent, i_phi);
	h_mInvMass_Raw[KEY_InvMass_Raw]->Write();
      }
    }
  }
}

//------------------------------------------------------------
//===============di-muon Yields==============
void PhDiMuonHistoManager::initHist_Yields(int pid, int mode)
{
  // histograms for resolution correction
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; i_cent++) // centrality bin
  {
    string KEY_Yields = genKey_Yields(pid, mode, i_cent);
    h_mYields[KEY_Yields] = new TH1F(KEY_Yields.c_str(),KEY_Yields.c_str(),100,0.0,5.0);
    h_mYields[KEY_Yields]->Sumw2();

    string KEY_Yields_Raw = genKey_YieldsRaw(pid, mode, i_cent);
    h_mYields_Raw[KEY_Yields_Raw] = new TH1F(KEY_Yields_Raw.c_str(),KEY_Yields_Raw.c_str(),100,0.0,5.0);
    h_mYields_Raw[KEY_Yields_Raw]->Sumw2();
  }
}

void PhDiMuonHistoManager::fillHist_Yields(int pid, int mode, int cent20, float invmass, float reweight)
{
  // raw yields for resolution correction
  string KEY_Yields = genKey_Yields(pid, mode, cent20);
  h_mYields[KEY_Yields]->Fill(invmass,reweight);
}

void PhDiMuonHistoManager::fillHist_YieldsRaw(int pid, int mode, int cent20, float invmass, float reweight)
{
  // raw yields without cuts
  string KEY_Yields_Raw = genKey_YieldsRaw(pid, mode, cent20);
  h_mYields_Raw[KEY_Yields_Raw]->Fill(invmass,reweight);
}

void PhDiMuonHistoManager::writeHist_Yields(int pid, int mode)
{
  // histograms for resolution correction
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; i_cent++) // centrality bin
  {
    string KEY_Yields = genKey_Yields(pid, mode, i_cent);
    h_mYields[KEY_Yields]->Write();

    string KEY_Yields_Raw = genKey_YieldsRaw(pid, mode, i_cent);
    h_mYields_Raw[KEY_Yields_Raw]->Write();
  }
}

//------------------------------------------------------------
//===============di-muon Spectra==============
void PhDiMuonHistoManager::initHist_Spec(int pid, int mode)
{
  // histograms for pt spectra
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      string KEY_Spec = genKey_Spec(pid, mode, i_pt, i_cent);
      h_mSpec[KEY_Spec] = new TH1F(KEY_Spec.c_str(),KEY_Spec.c_str(),100,0.0,5.0);
      h_mSpec[KEY_Spec]->Sumw2();

      string KEY_Spec_Raw = genKey_SpecRaw(pid, mode, i_pt, i_cent);
      h_mSpec_Raw[KEY_Spec_Raw] = new TH1F(KEY_Spec_Raw.c_str(),KEY_Spec_Raw.c_str(),100,0.0,5.0);
      h_mSpec_Raw[KEY_Spec_Raw]->Sumw2();
    }
  }
}

void PhDiMuonHistoManager::fillHist_Spec(int pid, int mode, int cent4, float pt, float invmass, float reweight)
{
  const int pt_bin = getPtBin(pt);
  if(pt_bin >= 0 && cent4 < 3) // miniBias
  {
    // resolution corrected flow
    string KEY_Spec = genKey_Spec(pid, mode, pt_bin, cent4);
    h_mSpec[KEY_Spec]->Fill(invmass,reweight);

    string KEY_Spec_miniBias = genKey_Spec(pid, mode, pt_bin, 3);
    h_mSpec[KEY_Spec_miniBias]->Fill(invmass,reweight);
  }
}

void PhDiMuonHistoManager::fillHist_SpecRaw(int pid, int mode, int cent4, float pt, float invmass, float reweight)
{
  const int pt_bin = getPtBin(pt);
  if(pt_bin >= 0 && cent4 < 3) // miniBias
  {
    // resolution corrected flow
    string KEY_Spec_Raw = genKey_SpecRaw(pid, mode, pt_bin, cent4);
    h_mSpec_Raw[KEY_Spec_Raw]->Fill(invmass,reweight);

    string KEY_Spec_Raw_miniBias = genKey_SpecRaw(pid, mode, pt_bin, 3);
    h_mSpec_Raw[KEY_Spec_Raw_miniBias]->Fill(invmass,reweight);
  }
}

void PhDiMuonHistoManager::writeHist_Spec(int pid, int mode)
{
  // histograms for pt spectra
  for(int i_pt = 0; i_pt < vecMesonFlow::mNumOfPt; i_pt++) // pt bin
  {
    for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; i_cent++) // centrality bin
    {
      string KEY_Spec = genKey_Spec(pid, mode, i_pt, i_cent);
      h_mSpec[KEY_Spec]->Write();

      string KEY_Spec_Raw = genKey_SpecRaw(pid, mode, i_pt, i_cent);
      h_mSpec_Raw[KEY_Spec_Raw]->Write();
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

string PhDiMuonHistoManager::genKey_InvMass(int pid, int mode, int pt_bin, int cent4, int phi_bin)
{
  string KEY_InvMass = Form("InvMass_%s_Pt_%d_Centrality_%s_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),phi_bin,vecMesonFlow::mEventType[mode].c_str());

  return KEY_InvMass;
}

string PhDiMuonHistoManager::genKey_InvMassRaw(int pid, int mode, int pt_bin, int cent4, int phi_bin)
{
  string KEY_InvMass_Raw = Form("InvMassRaw_%s_Pt_%d_Centrality_%s_Phi_%d_2nd_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),phi_bin,vecMesonFlow::mEventType[mode].c_str());

  return KEY_InvMass_Raw;
}

string PhDiMuonHistoManager::genKey_Yields(int pid, int mode, int cent20)
{
  string KEY_Yields = Form("Yields_%s_Centrality_%d_%s",vecMesonFlow::mPID[pid].c_str(),cent20,vecMesonFlow::mEventType[mode].c_str());

  return KEY_Yields;
}

string PhDiMuonHistoManager::genKey_YieldsRaw(int pid, int mode, int cent20)
{
  string KEY_Yields_Raw = Form("YieldsRaw_%s_Centrality_%d_%s",vecMesonFlow::mPID[pid].c_str(),cent20,vecMesonFlow::mEventType[mode].c_str());

  return KEY_Yields_Raw;
}

string PhDiMuonHistoManager::genKey_Spec(int pid, int mode, int pt_bin, int cent4)
{
  string KEY_Spec = Form("Spec_%s_Pt_%d_Centrality_%s_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),vecMesonFlow::mEventType[mode].c_str());

  return KEY_Spec;
}

string PhDiMuonHistoManager::genKey_SpecRaw(int pid, int mode, int pt_bin, int cent4)
{
  string KEY_Spec_Raw = Form("SpecRaw_%s_Pt_%d_Centrality_%s_%s",vecMesonFlow::mPID[pid].c_str(),pt_bin,vecMesonFlow::mCentType[cent4].c_str(),vecMesonFlow::mEventType[mode].c_str());

  return KEY_Spec_Raw;
}
