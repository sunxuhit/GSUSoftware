#include "RecoEventPlane.h"

#include <TFile.h>
#include <TProfile2D.h>
#include <TMath.h>

#include <TOAD.h>

#include <iostream>
#include <cmath>

ClassImp(RecoEventPlane)

//------------------------------------------------------------
RecoEventPlane::RecoEventPlane()
{
  clearRawBbcEventPlane();
  File_mInPutReCenter = NULL;
}

RecoEventPlane::~RecoEventPlane()
{
}

//-----------------BBC Event Plane---------------------

//------------------------------------------------------------
// get phi angle of pmt
float RecoEventPlane::getPhi_BbcSouth(float bbcx, float bbcy)
{
  const float xoff_BbcSouth = -0.5;
  const float yoff_BbcSouth = 0.0;
  float phi = TMath::ATan2(bbcy-yoff_BbcSouth,bbcx-xoff_BbcSouth);

  return phi;
}

float RecoEventPlane::getPhi_BbcNorth(float bbcx, float bbcy)
{
  const float xoff_BbcNorth = 7.0;
  const float yoff_BbcNorth = 0.0;
  float phi = TMath::ATan2(bbcy-yoff_BbcNorth,bbcx-xoff_BbcNorth);

  return phi;
}

//------------------------------------------------------------
// calcualte raw q-vector 
TVector2 RecoEventPlane::calqVector_BbcSouth(float bbcx, float bbcy, int order)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  const float phi = getPhi_BbcSouth(bbcx,bbcy);
  TVector2 qVector(0.0,0.0);

  const float qx = TMath::Cos(harmonic[order]*phi);
  const float qy = TMath::Sin(harmonic[order]*phi);
  qVector.Set(qx,qy);

  // std::cout << "this is calqVector_BbcSouth: input bbcx = " << bbcx << ", bbcy = " << bbcy << ", phi = " << phi << ", order = " << order << std::endl;
  // std::cout << "this is calqVector_BbcSouth: output qx = " << qVector.X() << ", qy = " << qVector.Y() << ", harmonic = " << harmonic[order] << std::endl;

  return qVector;
}

TVector2 RecoEventPlane::calqVector_BbcNorth(float bbcx, float bbcy, int order)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  const float phi = getPhi_BbcNorth(bbcx,bbcy);
  TVector2 qVector(0.0,0.0);

  const float qx = TMath::Cos(harmonic[order]*phi);
  const float qy = TMath::Sin(harmonic[order]*phi);
  qVector.Set(qx,qy);

  // std::cout << "this is calqVector_BbcNorth: input bbcx = " << bbcx << ", bbcy = " << bbcy << ", phi = " << phi  << ", order = " << order << std::endl;
  // std::cout << "this is calqVector_BbcNorth: output qx = " << qVector.X() << ", qy = " << qVector.Y() << ", harmonic = " << harmonic[order] << std::endl;

  return qVector;
}

//------------------------------------------------------------
// initialize raw Q-vector & weight & number of used pmts
void RecoEventPlane::initRawBbcEventPlane()
{
  std::cout << "initialize Raw BBC Event Plane!" << std::endl;
  clearRawBbcEventPlane();
  // printRawBbcEventPlane(0); // print information of 1st EP
  // printRawBbcEventPlane(1);
  // printRawBbcEventPlane(2);
}

void RecoEventPlane::clearRawBbcEventPlane()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    mQVectorRaw_BbcSouth[i_order].Set(0.0,0.0);
    mQVectorRaw_BbcNorth[i_order].Set(0.0,0.0);
    mQWeight_BbcSouth[i_order] = 0.0;
    mQWeight_BbcNorth[i_order] = 0.0;
    mNumOfPmts_BbcSouth[i_order] = 0;
    mNumOfPmts_BbcNorth[i_order] = 0;
  }
}

void RecoEventPlane::printRawBbcEventPlane(int order)
{
  std::string Order[3] = {"1st","2nd","3rd"};
  std::cout << "print information of " << Order[order] << " Event Plane: " << std::endl;

  std::cout << "mQVectorRaw_BbcSouth.X =  " << mQVectorRaw_BbcSouth[order].X() << std::endl;
  std::cout << "mQVectorRaw_BbcSouth.Y =  " << mQVectorRaw_BbcSouth[order].Y() << std::endl;
  std::cout << "mQWeight_BbcSouth =  " << mQWeight_BbcSouth[order] << std::endl;
  std::cout << "mNumOfPmts_BbcSouth =  " << mNumOfPmts_BbcSouth[order] << std::endl;

  std::cout << "mQVectorRaw_BbcNorth.X =  " << mQVectorRaw_BbcNorth[order].X() << std::endl;
  std::cout << "mQVectorRaw_BbcNorth.Y =  " << mQVectorRaw_BbcNorth[order].Y() << std::endl;
  std::cout << "mQWeight_BbcNorth =  " << mQWeight_BbcNorth[order] << std::endl;
  std::cout << "mNumOfPmts_BbcNorth =  " << mNumOfPmts_BbcNorth[order] << std::endl;

  std::cout << std::endl;
}

//------------------------------------------------------------
// calcualte raw Q-vector & weight & number of used pmts
void RecoEventPlane::addQVectorRaw_BbcSouth(float bbcx, float bbcy, float weight, int order)
{
  mQVectorRaw_BbcSouth[order] += weight*calqVector_BbcSouth(bbcx,bbcy,order);
  mQWeight_BbcSouth[order] += weight;
  mNumOfPmts_BbcSouth[order] += 1;
}

void RecoEventPlane::addQVectorRaw_BbcNorth(float bbcx, float bbcy, float weight, int order)
{
  mQVectorRaw_BbcNorth[order] += weight*calqVector_BbcNorth(bbcx,bbcy,order);
  mQWeight_BbcNorth[order] += weight;
  mNumOfPmts_BbcNorth[order] += 1;
}

//------------------------------------------------------------
// return raw Q-vector
TVector2 RecoEventPlane::getQVectorRaw_BbcSouth(int order)
{
  return mQVectorRaw_BbcSouth[order];
}

TVector2 RecoEventPlane::getQVectorRaw_BbcNorth(int order)
{
  return mQVectorRaw_BbcNorth[order];
}

//------------------------------------------------------------
// return weight
float RecoEventPlane::getQWeight_BbcSouth(int order)
{
  return mQWeight_BbcSouth[order];
}

float RecoEventPlane::getQWeight_BbcNorth(int order)
{
  return mQWeight_BbcNorth[order];
}

//------------------------------------------------------------
// return number of used pmts 
unsigned int RecoEventPlane::getNumOfPmts_BbcSouth(int order)
{
  return mNumOfPmts_BbcSouth[order];
}

unsigned int RecoEventPlane::getNumOfPmts_BbcNorth(int order)
{
  return mNumOfPmts_BbcNorth[order];
}

//------------------------------------------------------------
// calculate raw Psi
float RecoEventPlane::calPsiRaw_BbcSouth(int order)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  TVector2 QVector = getQVectorRaw_BbcSouth(order);
  float weight = getQWeight_BbcSouth(order);
  float Qx = QVector.X()/weight;
  float Qy = QVector.Y()/weight;
  float Psi = TMath::ATan2(Qy,Qx)/harmonic[order];

  return Psi;
}

float RecoEventPlane::calPsiRaw_BbcNorth(int order)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  TVector2 QVector = getQVectorRaw_BbcNorth(order);
  float weight = getQWeight_BbcNorth(order);
  float Qx = QVector.X()/weight;
  float Qy = QVector.Y()/weight;
  float Psi = TMath::ATan2(Qy,Qx)/harmonic[order];

  return Psi;
}
//-----------------BBC Event Plane---------------------


//===============ReCenter BBC Event Plane====================
bool RecoEventPlane::readPro_ReCenter()
{
  TOAD toad_loader("PhVecMesonMaker");
  std::string input_recenter = toad_loader.location("file_AuAu200GeV_ReCenter.root");
  std::cout << "inputfile = " << input_recenter.c_str() << std::endl;
  File_mInPutReCenter = TFile::Open(input_recenter.c_str());
  if(!File_mInPutReCenter->IsOpen())
  {
    std::cout << "Could NOT find ReCenter Parameters file! & drop the run!" << std::endl;
    return false;
  }

  std::cout << "Read in ReCenter Correction Profiles!" << std::endl;
  const std::string Order[3] = {"1st","2nd","3rd"};
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
    {
      std::string ProName;

      ProName = Form("p_mQx_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcSouth[i_order][i_vtx] = (TProfile2D*)File_mInPutReCenter->Get(ProName.c_str());

      ProName = Form("p_mQy_BbcSouth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcSouth[i_order][i_vtx] = (TProfile2D*)File_mInPutReCenter->Get(ProName.c_str());

      ProName = Form("p_mQx_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQx_BbcNorth[i_order][i_vtx] = (TProfile2D*)File_mInPutReCenter->Get(ProName.c_str());

      ProName = Form("p_mQy_BbcNorth_%s_Vtx_%d",Order[i_order].c_str(),i_vtx);
      p_mQy_BbcNorth[i_order][i_vtx] = (TProfile2D*)File_mInPutReCenter->Get(ProName.c_str());
    }
  }

  return true;
}

bool RecoEventPlane::closePro_ReCenter()
{
  File_mInPutReCenter->Close();

  if(File_mInPutReCenter->IsOpen())
  {
    std::cout << "Could NOT find ReCenter Parameters file! & drop the run!" << std::endl;
    return false;
  }

  return true;
}

// initialize raw Q-vector & weight & number of used pmts
void RecoEventPlane::initReCenterBbcEventPlane()
{
  std::cout << "initialize ReCentered BBC Event Plane!" << std::endl;
  clearReCenterBbcEventPlane();
  // printRawBbcEventPlane(0); // print information of 1st EP
  // printRawBbcEventPlane(1);
  // printRawBbcEventPlane(2);
}

void RecoEventPlane::clearReCenterBbcEventPlane()
{
  for(int i_order = 0; i_order < 3; ++i_order)
  {
    mQVectorReCenter_BbcSouth[i_order].Set(0.0,0.0);
    mQVectorReCenter_BbcNorth[i_order].Set(0.0,0.0);
  }
}

void RecoEventPlane::printReCenterBbcEventPlane(int order, int vtx4, int runId, int cent20)
{
  std::string Order[3] = {"1st","2nd","3rd"};
  std::cout << "print information of " << Order[order] << " Event Plane: " << std::endl;

  std::cout << "mQVectorReCenter_BbcSouth.X =  " << mQVectorReCenter_BbcSouth[order].X() << std::endl;
  std::cout << "mQVectorReCenter_BbcSouth.Y =  " << mQVectorReCenter_BbcSouth[order].Y() << std::endl;
  float mean_Qx_South = p_mQx_BbcSouth[order][vtx4]->GetBinContent(p_mQx_BbcSouth[order][vtx4]->FindBin((double)runId,(double)cent20));
  float mean_Qy_South = p_mQy_BbcSouth[order][vtx4]->GetBinContent(p_mQy_BbcSouth[order][vtx4]->FindBin((double)runId,(double)cent20));
  std::cout << "mean_Qx_South = " << mean_Qx_South << ", mean_Qy_South = " << mean_Qy_South << std::endl;

  std::cout << "mQVectorReCenter_BbcNorth.X =  " << mQVectorReCenter_BbcNorth[order].X() << std::endl;
  std::cout << "mQVectorReCenter_BbcNorth.Y =  " << mQVectorReCenter_BbcNorth[order].Y() << std::endl;
  float mean_Qx_North = p_mQx_BbcNorth[order][vtx4]->GetBinContent(p_mQx_BbcNorth[order][vtx4]->FindBin((double)runId,(double)cent20));
  float mean_Qy_North = p_mQy_BbcNorth[order][vtx4]->GetBinContent(p_mQy_BbcNorth[order][vtx4]->FindBin((double)runId,(double)cent20));
  std::cout << "mean_Qx_North = " << mean_Qx_North << ", mean_Qy_North = " << mean_Qy_North << std::endl;

  std::cout << std::endl;
}

TVector2 RecoEventPlane::getQVectorReCenter_BbcSouth(int order, int vtx4, int runId, int cent20)
{
  float mean_Qx = p_mQx_BbcSouth[order][vtx4]->GetBinContent(p_mQx_BbcSouth[order][vtx4]->FindBin((double)runId,(double)cent20));
  float mean_Qy = p_mQy_BbcSouth[order][vtx4]->GetBinContent(p_mQy_BbcSouth[order][vtx4]->FindBin((double)runId,(double)cent20));

  TVector2 QVector_Raw = getQVectorRaw_BbcSouth(order);
  float weight = getQWeight_BbcSouth(order);
  float Qx = QVector_Raw.X()/weight - mean_Qx; // re-center correction
  float Qy = QVector_Raw.Y()/weight - mean_Qy;

  mQVectorReCenter_BbcSouth[order].Set(Qx,Qy);

  return mQVectorReCenter_BbcSouth[order];
}

TVector2 RecoEventPlane::getQVectorReCenter_BbcNorth(int order, int vtx4, int runId, int cent20)
{
  float mean_Qx = p_mQx_BbcNorth[order][vtx4]->GetBinContent(p_mQx_BbcNorth[order][vtx4]->FindBin((double)runId,(double)cent20));
  float mean_Qy = p_mQy_BbcNorth[order][vtx4]->GetBinContent(p_mQy_BbcNorth[order][vtx4]->FindBin((double)runId,(double)cent20));

  TVector2 QVector_Raw = getQVectorRaw_BbcNorth(order);
  float weight = getQWeight_BbcNorth(order);
  float Qx = QVector_Raw.X()/weight - mean_Qx; // re-center correction
  float Qy = QVector_Raw.Y()/weight - mean_Qy;

  mQVectorReCenter_BbcNorth[order].Set(Qx,Qy);

  return mQVectorReCenter_BbcNorth[order];
}

//------------------------------------------------------------
// calculate re-centered Psi
float RecoEventPlane::calPsiReCenter_BbcSouth(int order, int vtx4, int runId, int cent20)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  TVector2 QVector = getQVectorReCenter_BbcSouth(order,vtx4,runId,cent20);
  float Qx = QVector.X();
  float Qy = QVector.Y();
  float Psi = TMath::ATan2(Qy,Qx)/harmonic[order];

  return Psi;
}

float RecoEventPlane::calPsiReCenter_BbcNorth(int order, int vtx4, int runId, int cent20)
{
  const float harmonic[3] = {1.0,2.0,3.0};
  TVector2 QVector = getQVectorReCenter_BbcNorth(order,vtx4,runId,cent20);
  float Qx = QVector.X();
  float Qy = QVector.Y();
  float Psi = TMath::ATan2(Qy,Qx)/harmonic[order];

  return Psi;
}

//===============ReCenter BBC Event Plane====================
