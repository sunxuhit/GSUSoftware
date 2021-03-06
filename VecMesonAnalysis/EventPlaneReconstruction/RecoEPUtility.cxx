#include "RecoEPUtility.h"
#include <TMath.h>

#include <TOAD.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <iterator>

ClassImp(RecoEPUtility)

//------------------------------------------------------------
RecoEPUtility::RecoEPUtility()
{
  for (int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    for (int i_grp = 0; i_grp < GoodRunList::ngrp; ++i_grp)
    {
      const_pedestal[i_pmt][i_grp] = -9999;
      const_mip[i_pmt][i_grp] = -9999;
      for (int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
      {
	phi_weight[i_pmt][i_grp][i_cent] = 1.0;
      }
    }
  }
}

RecoEPUtility::~RecoEPUtility()
{
}
//------------------------------------------------------------

bool RecoEPUtility::isGoodRun(int runId)
{
  int grp = get_recal_group(runId);
  if(grp == 1) return std::find(GoodRunList::runno_grp1,GoodRunList::runno_grp1+GoodRunList::nrun_grp1,runId) != GoodRunList::runno_grp1+GoodRunList::nrun_grp1;
  if(grp == 2) return std::find(GoodRunList::runno_grp2,GoodRunList::runno_grp2+GoodRunList::nrun_grp2,runId) != GoodRunList::runno_grp2+GoodRunList::nrun_grp2;
  if(grp == 3) return std::find(GoodRunList::runno_grp3,GoodRunList::runno_grp3+GoodRunList::nrun_grp3,runId) != GoodRunList::runno_grp3+GoodRunList::nrun_grp3;
  if(grp == 4) return std::find(GoodRunList::runno_grp4,GoodRunList::runno_grp4+GoodRunList::nrun_grp4,runId) != GoodRunList::runno_grp4+GoodRunList::nrun_grp4;
  if(grp == 5) return std::find(GoodRunList::runno_grp5,GoodRunList::runno_grp5+GoodRunList::nrun_grp5,runId) != GoodRunList::runno_grp5+GoodRunList::nrun_grp5;
  if(grp == 6) return std::find(GoodRunList::runno_grp6,GoodRunList::runno_grp6+GoodRunList::nrun_grp6,runId) != GoodRunList::runno_grp6+GoodRunList::nrun_grp6;
  if(grp == 7) return std::find(GoodRunList::runno_grp7,GoodRunList::runno_grp7+GoodRunList::nrun_grp7,runId) != GoodRunList::runno_grp7+GoodRunList::nrun_grp7;
  if(grp == 8) return std::find(GoodRunList::runno_grp8,GoodRunList::runno_grp8+GoodRunList::nrun_grp8,runId) != GoodRunList::runno_grp8+GoodRunList::nrun_grp8;
  if(grp == 9) return std::find(GoodRunList::runno_grp9,GoodRunList::runno_grp9+GoodRunList::nrun_grp9,runId) != GoodRunList::runno_grp9+GoodRunList::nrun_grp9;
  if(grp == 10) return std::find(GoodRunList::runno_grp10,GoodRunList::runno_grp10+GoodRunList::nrun_grp10,runId) != GoodRunList::runno_grp10+GoodRunList::nrun_grp10;
  if(grp == 11) return std::find(GoodRunList::runno_grp11,GoodRunList::runno_grp11+GoodRunList::nrun_grp11,runId) != GoodRunList::runno_grp11+GoodRunList::nrun_grp11;
  if(grp == 12) return std::find(GoodRunList::runno_grp12,GoodRunList::runno_grp12+GoodRunList::nrun_grp12,runId) != GoodRunList::runno_grp12+GoodRunList::nrun_grp12;
  if(grp == 13) return std::find(GoodRunList::runno_grp13,GoodRunList::runno_grp13+GoodRunList::nrun_grp13,runId) != GoodRunList::runno_grp13+GoodRunList::nrun_grp13;
  if(grp == 14) return std::find(GoodRunList::runno_grp14,GoodRunList::runno_grp14+GoodRunList::nrun_grp14,runId) != GoodRunList::runno_grp14+GoodRunList::nrun_grp14;
  if(grp == 15) return std::find(GoodRunList::runno_grp15,GoodRunList::runno_grp15+GoodRunList::nrun_grp15,runId) != GoodRunList::runno_grp15+GoodRunList::nrun_grp15;
  if(grp == 16) return std::find(GoodRunList::runno_grp16,GoodRunList::runno_grp16+GoodRunList::nrun_grp16,runId) != GoodRunList::runno_grp16+GoodRunList::nrun_grp16;

  return false;
}

void RecoEPUtility::initRunIndex()
{
  map_runIndex.clear();
  bool isOpen_runIndex = read_in_runIndex();
  if(isOpen_runIndex) std::cout << "Run Index read in!" << std::endl;
}

int RecoEPUtility::find_runIndex(int runId)
{
   // print map_runIndex content:
  /*
  for (std::map<int,int>::iterator it=map_runIndex.begin(); it!=map_runIndex.end(); ++it)
  {
    std::cout << it->first << " => " << it->second << '\n';
  }
  */

  std::map<int,int>::iterator it_runId = map_runIndex.find(runId);
  if(it_runId == map_runIndex.end()) 
  {
    std::cout << "RecoEPUtility -> could not find in good run list! & send signal to kill the run!" << std::endl;
    return -999;
  }
  else
  {
    std::cout << "RecoEPUtility -> runId: " << it_runId->first << " => runIndex: " << it_runId->second << std::endl;
    return it_runId->second;
  }

  return -999;
}

bool RecoEPUtility::read_in_runIndex()
{
  TOAD toad_loader("PhEventPlaneMaker");
  std::string inputfile = toad_loader.location("runIndex.txt");
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  std::ifstream file_runIndex ( inputfile.c_str() );
  if ( !file_runIndex.is_open() )
  {
    std::cout << "Abort. Fail to read in run Index file: " << inputfile << std::endl;
    exit(0);
  }

  int temp_runId = 0, temp_runIndex = 0;
  std::cout << "reading run Index: " << std::endl;
  while (file_runIndex >> temp_runId >> temp_runIndex)
  {
    // std::cout << "runId = " << temp_runId << ", runIndex = " << temp_runIndex << std::endl;
    map_runIndex[temp_runId] = temp_runIndex;
  }
  file_runIndex.close();

  return true;
}

int RecoEPUtility::getCentralityBin20(float centrality)
{
  // 0: (0%,5%], 1: (5%-10%], 2: (10%,15%], ......, 18: (90%,95%], 19: (95%,100%]
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality20; ++i_cent)
  {
    if(centrality > vecMesonFlow::mCentStart20[i_cent] && centrality < vecMesonFlow::mCentStop20[i_cent])
    {
      return vecMesonFlow::mCentrality20[i_cent];
    }
    if(TMath::Abs(centrality-vecMesonFlow::mCentStop20[i_cent]) < 1e-10) return vecMesonFlow::mCentrality20[i_cent]; // centrality == mCentStop20
  }

  return -1;
}

int RecoEPUtility::getCentralityBin10(float centrality)
{
  // 0: (0%,5%], 1: (5%-10%], 2: (10%,20%], ......, 7: (60%,70%], 8: (70%,80%]
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality10; ++i_cent)
  {
    if(centrality > vecMesonFlow::mCentStart10[i_cent] && centrality < vecMesonFlow::mCentStop10[i_cent])
    {
      return vecMesonFlow::mCentrality10[i_cent];
    }
    if(TMath::Abs(centrality-vecMesonFlow::mCentStop10[i_cent]) < 1e-10) return vecMesonFlow::mCentrality10[i_cent]; // centrality == mCentStop10
  }

  return -1;
}

int RecoEPUtility::getCentralityBin4(float centrality)
{
  // 0: (0%,10%], 1: (10%-40%], 2: (40%,80%], 3: (80%,100%]
  for(int i_cent = 0; i_cent < vecMesonFlow::mNumOfCentrality4; ++i_cent)
  {
    if(centrality > vecMesonFlow::mCentStart4[i_cent] && centrality < vecMesonFlow::mCentStop4[i_cent])
    {
      return vecMesonFlow::mCentrality4[i_cent];
    }
    if(TMath::Abs(centrality-vecMesonFlow::mCentStop4[i_cent]) < 1e-10) return vecMesonFlow::mCentrality4[i_cent]; // centrality == mCentStop4
  }

  return -1;
}

int RecoEPUtility::getVertexBin(float vtx_z)
{
  // 0: [-10.0,-5.0], 1: (-5.0,0.0], 2: (0.0,5.0], 3: (5.0,10.0]
  for(int i_vtx = 0; i_vtx < vecMesonFlow::mNumOfVertex; ++i_vtx)
  {
    if(vtx_z > vecMesonFlow::mVtxStart[i_vtx] && vtx_z < vecMesonFlow::mVtxStop[i_vtx])
    {
      return vecMesonFlow::mVertex[i_vtx];
    }
    if(TMath::Abs(vtx_z-vecMesonFlow::mVtxStop[i_vtx]) < 1e-10) return vecMesonFlow::mVtxStop[i_vtx]; // vtx_z == mVtxStop
  }
  if(TMath::Abs(vtx_z-vecMesonFlow::mVtxStart[0]) < 1e-10) return vecMesonFlow::mVertex[0]; // vtx_z == mVtxStart lower edge

  return -1;
}

//------------BBC Event Plane---------------
void RecoEPUtility::initBBC()
{
  bool bbc_recal = read_in_recal_consts();
  if(bbc_recal) std::cout << "BBC recalibration constants read in!" << std::endl;
  bool bbc_phiweight = read_in_phiweight_corrections();
  if(bbc_phiweight) std::cout << "BBC phi weight corrections read in!" << std::endl;
}

bool RecoEPUtility::read_in_recal_consts()
{
  TOAD toad_loader("PhEventPlaneMaker");
  std::string inputfile = toad_loader.location("BbcRecalConsts.txt");
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  std::ifstream read_consts( inputfile.c_str() );
  if ( !read_consts.is_open() )
  {
    std::cout<<"Abort. Fail to read in BBC recalibration constants: "<<inputfile<<std::endl;
    exit(0);
  }

  int _pmt = 0, _rungrp = 0;
  float _ped = 0, _mip = 0;
  std::cout << "reading BBC recalibration constants: " << std::endl;
  while (read_consts >> _pmt >> _rungrp >> _ped >> _mip)
  {
    const_pedestal[_pmt][_rungrp] = _ped;
    const_mip[_pmt][_rungrp] = _mip;
    // std::cout << "i_pmt = " << _pmt << ", run group = " << _rungrp << ", ped = " << _ped << ", mip = " << _mip << std::endl;
  }
  read_consts.close();

  return true;
}

float RecoEPUtility::get_recal_charge(int PmtIndx, int run_num, int ADC)
{
  int _grp = get_recal_group(run_num);
  float _ped = const_pedestal[PmtIndx][_grp-1];
  float _mip = const_mip[PmtIndx][_grp-1];

  return (ADC-_ped)/(_mip-_ped);
}

bool RecoEPUtility::read_in_phiweight_corrections()
{
  TOAD toad_loader("PhEventPlaneMaker");
  std::string inputfile = toad_loader.location("BbcPhiWeightCorrection.txt");
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  std::ifstream read_correction ( inputfile.c_str() );
  if ( !read_correction.is_open() )
  {
    std::cout << "Abort. Fail to read in BBC recalibration constants: " << inputfile << std::endl;
    exit(0);
  }

  int temp_grp = 0, temp_cent = 0, temp_pmt = 0;
  float temp_ratio = 0;
  std::cout << "reading correction factors: " << std::endl;
  while (read_correction >> temp_grp >> temp_cent >> temp_pmt >> temp_ratio)
  {
    if(temp_grp > 0)
    {
      phi_weight[temp_pmt][temp_grp-1][temp_cent] = 1.0/temp_ratio;
      // std::cout << "i_pmt = " << temp_pmt << ", run group = " << temp_grp << ", cent20 = " << temp_cent << ", phi_weight = " << 1.0/temp_ratio << std::endl;
    }
  }
  read_correction.close();

  return true;
}

float RecoEPUtility::get_phiweight_correction(int PmtIndx, int run_num, float centrality)
{
  // for BBC, compatible with offset correction
  int _icent = getCentralityBin20(centrality);
  int _igrp = get_recal_group(run_num);

  return phi_weight[PmtIndx][_igrp-1][_icent];
}

int RecoEPUtility::get_recal_group(int run_num)
{
  if (run_num <= GoodRunList::runno_grp1[GoodRunList::nrun_grp1-1]) return 1;
  else if (run_num <= GoodRunList::runno_grp2[GoodRunList::nrun_grp2-1]) return 2;
  else if (run_num <= GoodRunList::runno_grp3[GoodRunList::nrun_grp3-1]) return 3;
  else if (run_num <= GoodRunList::runno_grp4[GoodRunList::nrun_grp4-1]) return 4;
  else if (run_num <= GoodRunList::runno_grp5[GoodRunList::nrun_grp5-1]) return 5;
  else if (run_num <= GoodRunList::runno_grp6[GoodRunList::nrun_grp6-1]) return 6;
  else if (run_num <= GoodRunList::runno_grp7[GoodRunList::nrun_grp7-1]) return 7;
  else if (run_num <= GoodRunList::runno_grp8[GoodRunList::nrun_grp8-1]) return 8;
  else if (run_num <= GoodRunList::runno_grp9[GoodRunList::nrun_grp9-1]) return 9;
  else if (run_num <= GoodRunList::runno_grp10[GoodRunList::nrun_grp10-1]) return 10;
  else if (run_num <= GoodRunList::runno_grp11[GoodRunList::nrun_grp11-1]) return 11;
  else if (run_num <= GoodRunList::runno_grp12[GoodRunList::nrun_grp12-1]) return 12;
  else if (run_num <= GoodRunList::runno_grp13[GoodRunList::nrun_grp13-1]) return 13;
  else if (run_num <= GoodRunList::runno_grp14[GoodRunList::nrun_grp14-1]) return 14;
  else if (run_num <= GoodRunList::runno_grp15[GoodRunList::nrun_grp15-1]) return 15;
  else if (run_num <= GoodRunList::runno_grp16[GoodRunList::nrun_grp16-1]) return 16;

  return 0; // no match
}

float RecoEPUtility::get_pedestal(int PmtIndx, int run_num)
{
  int _grp = get_recal_group(run_num);

  return const_pedestal[PmtIndx][_grp-1];
}

float RecoEPUtility::get_mip(int PmtIndx, int run_num)
{
  int _grp = get_recal_group(run_num);

  return const_mip[PmtIndx][_grp-1];
}

bool RecoEPUtility::isSaturatePMT(int PmtIndx)
{
  if (PmtIndx==13) return true;
  if (PmtIndx==19) return true;
  if (PmtIndx==26) return true;
  if (PmtIndx==69) return true;
  if (PmtIndx==75) return true;
  if (PmtIndx==87) return true;
  if (PmtIndx==113) return true;
  if (PmtIndx==121) return true;

  return false;
}

bool RecoEPUtility::isBadPMT(int PmtIndx, int run_num)
{
  // need more tuning in the calibration constants
  if (PmtIndx==62) return true;
  if (PmtIndx==64) return true;
  if (PmtIndx==65) return true;
  if (PmtIndx==70) return true;
  if (PmtIndx==71) return true;
  if (PmtIndx==72) return true;
  if (PmtIndx==108) return true;
  if (PmtIndx==118) return true;
  if (PmtIndx==120) return true;
  if (PmtIndx==121) return true;
  if (PmtIndx==122) return true;
  if (PmtIndx==125) return true;
  if (PmtIndx==126) return true;
  if (PmtIndx==127) return true;

  // unstable behavior in the last run group
  int _grp = get_recal_group(run_num);
  if (PmtIndx==19 && run_num>=414505 && run_num<=414732) return true;
  if (PmtIndx==51 && _grp==16) return true;
  if (PmtIndx==94 && _grp==16) return true;
  if (PmtIndx==115 && _grp==16) return true;

  // completelt abnormal Adc distribution (i.e. stuck bit)
  if (PmtIndx==14 && run_num>=411644 && run_num<=411649) return true;
  if (PmtIndx==46 && run_num>=409432 && run_num<=409443) return true;
  if (PmtIndx==67 && run_num==410104) return true;
  if (PmtIndx==110 && run_num==413665) return true;

  return false;
}
//------------BBC Event Plane---------------
