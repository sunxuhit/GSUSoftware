#include "RecoEventPlane.h"

#include <TMath.h>

#include <iostream>
#include <cmath>

ClassImp(RecoEventPlane)

//------------------------------------------------------------
RecoEventPlane::RecoEventPlane()
{
  clearRawBbcEventPlane();
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

  // std::cout << "this is calqVector_BbcSouth: input bbcx = " << bbcx << ", bbcy = " << bbcy << "order = " << order << endl;
  // std::cout << "this is calqVector_BbcSouth: output qx = " << qVector.X() << ", qy = " << qVector.Y() << " harmonic = " << harmonic[order] << endl;

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

  // std::cout << "this is calqVector_BbcNorth : input bbcx = " << bbcx << ", bbcy = " << bbcy << "order = " << order << endl;
  // std::cout << "this is calqVector_BbcNorth: output qx = " << qVector.X() << ", qy = " << qVector.Y() << " harmonic = " << harmonic[order] << endl;

  return qVector;
}

//------------------------------------------------------------
// initialize raw Q-vector & weight & number of used pmts
void RecoEventPlane::initRawBbcEventPlane()
{
  std::cout << "initialize Raw BBC Event Plane!" << std::endl;
  clearRawBbcEventPlane();
  printRawBbcEventPlane(0); // print information of 1st EP
  printRawBbcEventPlane(1);
  printRawBbcEventPlane(2);
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
  TVector2 QVector = getQVectorRaw_BbcSouth(order);
  float weight = getQWeight_BbcSouth(order);
  float Qx = QVector.X()/weight;
  float Qy = QVector.Y()/weight;
  float Psi = TMath::ATan2(Qy,Qx);

  return Psi;
}

float RecoEventPlane::calPsiRaw_BbcNorth(int order)
{
  TVector2 QVector = getQVectorRaw_BbcNorth(order);
  float weight = getQWeight_BbcNorth(order);
  float Qx = QVector.X()/weight;
  float Qy = QVector.Y()/weight;
  float Psi = TMath::ATan2(Qy,Qx);

  return Psi;
}
//-----------------BBC Event Plane---------------------

