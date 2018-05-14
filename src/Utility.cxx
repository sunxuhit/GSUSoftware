#include "../include/Utility.h"
#include "../include/mRICH.h"
#include "TMath.h"

Utility::Utility()
{
  creatParticleIdMap();
  creatParticleMisIdMap();
}

Utility::~Utility()
{
  /* */
}

void Utility::creatParticleIdMap()
{
  mParticleIdMap.clear();
  mParticleIdMap[211]   = "piplus";
  mParticleIdMap[321]   = "Kplus";
  mParticleIdMap[2212]  = "proton";
  mParticleIdMap[-211]  = "piminus";
  mParticleIdMap[-321]  = "Kminus";
  mParticleIdMap[-2212] = "antiproton";
}

void Utility::creatParticleMisIdMap()
{
  mParticleMisIdMap.clear();
  mParticleMisIdMap[211]    = std::pair<std::string,std::string>("Kplus","proton");
  mParticleMisIdMap[321]    = std::pair<std::string,std::string>("piplus","proton");
  mParticleMisIdMap[2212]   = std::pair<std::string,std::string>("piplus","Kplus");
  mParticleMisIdMap[-211]   = std::pair<std::string,std::string>("Kminus","antiproton");
  mParticleMisIdMap[-321]   = std::pair<std::string,std::string>("piminus","antiproton");
  mParticleMisIdMap[-2212]  = std::pair<std::string,std::string>("piminus","Kminus");
}

std::string Utility::get_IdentifiedParticle(int pid)
{
  return mParticleIdMap[pid];
}

std::pair<std::string,std::string> Utility::get_misIdentifiedParticle(int pid)
{
  return mParticleMisIdMap[pid];
}

int Utility::get_indexSpaceX(double vx)
{
  const double delta_vx = (mRICH::mVx_stop-mRICH::mVx_start)/mRICH::mNumOfIndexSpaceX; 

  int index = -1;
  if(vx > mRICH::mVx_start && vx < mRICH::mVx_stop)
  {
    for(int i_index = 0; i_index < mRICH::mNumOfIndexSpaceX; ++i_index)
    {
      if(vx > mRICH::mVx_start+i_index*delta_vx && vx <= mRICH::mVx_start+(i_index+1)*delta_vx)
      {
	index = i_index;
      }
    }
  }

  return index;
}

int Utility::get_indexSpaceY(double vy)
{
  const double delta_vy = (mRICH::mVy_stop-mRICH::mVy_start)/mRICH::mNumOfIndexSpaceY; 

  int index = -1;
  if(vy > mRICH::mVy_start && vy < mRICH::mVy_stop)
  {
    for(int i_index = 0; i_index < mRICH::mNumOfIndexSpaceY; ++i_index)
    {
      if(vy > mRICH::mVy_start+i_index*delta_vy && vy <= mRICH::mVy_start+(i_index+1)*delta_vy)
      {
	index = i_index;
      }
    }
  }

  return index;
}

int Utility::get_indexMomentumP(double px, double py, double pz)
{
  const double momentum = TMath::Sqrt(px*px+py*py+pz*pz); // in GeV

  const double delta_p = (mRICH::mMomP_stop-mRICH::mMomP_start)/mRICH::mNumOfIndexMomentumP; 

  int index = -1;
  if(momentum > mRICH::mMomP_start && momentum < mRICH::mMomP_stop)
  {
    for(int i_index = 0; i_index < mRICH::mNumOfIndexMomentumP; ++i_index)
    {
      if(momentum > mRICH::mMomP_start+i_index*delta_p && momentum <= mRICH::mMomP_start+(i_index+1)*delta_p)
      {
	index = i_index;
      }
    }
  }

  return index;
}
