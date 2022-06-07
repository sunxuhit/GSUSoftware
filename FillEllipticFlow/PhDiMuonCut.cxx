#include <TMath.h>
#include <TString.h>

#include <iostream>
#include <string>

#include "./PhDiMuonCut.h"

ClassImp(PhDiMuonCut)

using namespace std;

//------------------------------------------------------------

PhDiMuonCut::PhDiMuonCut()
{
}

PhDiMuonCut::~PhDiMuonCut()
{
}

//------------------------------------------------------------
//=============Event Cuts==============
bool PhDiMuonCut::passVtxCut(TVector3 vtx_BBC)
{
  if( TMath::Abs(vtx_BBC.Z()) > vecMesonFlow::mVtx_Bbc )
  {
    return false;
  }

  return true;
}

bool PhDiMuonCut::passEventPlaneCut(float Psi_FvtxSouth, float Psi_FvtxNorth)
{
  if( !(Psi_FvtxSouth > -900.0 && Psi_FvtxNorth > -900.0) )
  {
    return false;
  }

  return true;
}
//------------------------------------------------------------
//=============Di-Muon Cuts==============
bool PhDiMuonCut::passDiMuonVtxCuts(float rapidity, float vtxchi2, float vtxchi2_fvtxmutr)
{
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin < 0) 
  { // rapidity cut
    return false;
  }
  if(vtxchi2 > vecMesonFlow::mVtxChi2[rapidity_bin]) 
  { // vtxchi2 cut
    return false;
  }
  if(vtxchi2_fvtxmutr > vecMesonFlow::mVtxChi2_Fvtx[rapidity_bin])
  { // vtxchi2_fvtxmutr cut
    return false;
  }

  return true;
}

bool PhDiMuonCut::passDiMuonChargeCuts(short charge, int mode)
{
  if(mode < 2 && TMath::Abs(charge) != vecMesonFlow::mCharge[mode])
  { // only apply for US and LS
    return false;
  }

  return true;
}


bool PhDiMuonCut::isSouthDiMuon(float rapidity)
{ // -2.2 < rapidity < -1.2
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin != 0) 
  { // rapidity cut
    return false;
  }

   return true;
}

bool PhDiMuonCut::isNorthDiMuon(float rapidity)
{ //  1.2 < rapidity < 2.2
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin != 1) 
  { // rapidity cut
    return false;
  }

   return true;
}
//------------------------------------------------------------
//=============Si-Muon Cuts==============
bool PhDiMuonCut::passSiMuonDGCuts(float rapidity, float DG0, float DDG0, float pz)
{
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin < 0) 
  { // rapidity cut
    return false;
  }
  if(DG0 > vecMesonFlow::mDG0[rapidity_bin])
  { // DG0 cuts
    return false;
  }
  if(DDG0 > vecMesonFlow::mDDG0[rapidity_bin])
  { // DDG0 cuts
    return false;
  }
  if(TMath::Abs(pz) < vecMesonFlow::mPzSiMuon[rapidity_bin])
  { // pz cuts
    return false;
  }

  return true;
}

bool PhDiMuonCut::passSiMuonMutrCuts(float rapidity, float trchi2, short ntrhits, float idchi2, short nidhits, short lastgap)
{
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin < 0) 
  { // rapidity cut
    return false;
  }
  if(trchi2 > vecMesonFlow::mMutrChi2[rapidity_bin])
  { // mutr_chi2 cuts
    return false;
  }
  if(ntrhits < vecMesonFlow::mMutrHits[rapidity_bin])
  { // mutr_hits cuts
    return false;
  }
  if(idchi2 > vecMesonFlow::mMuIdChi2[rapidity_bin])
  { // muid_chi2 cuts
    return false;
  }
  if(nidhits < vecMesonFlow::mMuIdHits[rapidity_bin])
  { // muid_hits cuts
    return false;
  }
  if(lastgap <= vecMesonFlow::mLastGap[rapidity_bin])
  { // lastgap cuts
    return false;
  }

  return true;
}

bool PhDiMuonCut::passSiMuonDcaCuts(float rapidity, float dca_r, float dca_z)
{
  int rapidity_bin = getRapidityBin(rapidity);
  if(rapidity_bin < 0) 
  { // rapidity cut
    return false;
  }
  if(TMath::Abs(dca_r) > vecMesonFlow::mDcaR[rapidity_bin])
  { // dca_r cut
    return false;
  }
  if(TMath::Abs(dca_z) > vecMesonFlow::mDcaZ[rapidity_bin])
  { // dca_z cut
    return false;
  }

  return true;
}

//------------------------------------------------------------
//=============utility functions==============
int PhDiMuonCut::getRapidityBin(float rapidity)
{
  // 0: [-2.2,-1.2], 1: [1.2,2.2]
  for(int i_rapidity = 0; i_rapidity < vecMesonFlow::mNumOfRapidity; ++i_rapidity)
  {
    if(rapidity > vecMesonFlow::mRapidityStart[i_rapidity] && rapidity < vecMesonFlow::mRapidityStop[i_rapidity])
    {
      return vecMesonFlow::mRapidityBin[i_rapidity];
    }
    if(TMath::Abs(rapidity-vecMesonFlow::mRapidityStop[i_rapidity]) < FLT_EPSILON) 
    { // upper edge
      return vecMesonFlow::mRapidityBin[i_rapidity];
    }
    if(TMath::Abs(rapidity-vecMesonFlow::mRapidityStart[i_rapidity]) < FLT_EPSILON) 
    { // lower edge
      return vecMesonFlow::mRapidityBin[i_rapidity]; // rapidity_bin == mRapidityStop
    }
  }

  return -1;
}
//------------------------------------------------------------
