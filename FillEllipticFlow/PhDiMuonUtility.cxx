#include "PhDiMuonUtility.h"
#include <TMath.h>

#include <TOAD.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <iterator>

ClassImp(PhDiMuonUtility)

//------------------------------------------------------------
PhDiMuonUtility::PhDiMuonUtility()
{
}

PhDiMuonUtility::~PhDiMuonUtility()
{
}
//------------------------------------------------------------
int PhDiMuonUtility::get_recal_group(int run_num)
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

bool PhDiMuonUtility::isGoodRun(int runId)
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

void PhDiMuonUtility::initRunIndex()
{
  map_runIndex.clear();
  bool isOpen_runIndex = read_in_runIndex();
  if(isOpen_runIndex) std::cout << "Run Index read in!" << std::endl;
}

int PhDiMuonUtility::find_runIndex(int runId)
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
    std::cout << "PhDiMuonUtility -> could not find in good run list! & send signal to kill the run!" << std::endl;
    return -999;
  }
  else
  {
    std::cout << "PhDiMuonUtility -> runId: " << it_runId->first << " => runIndex: " << it_runId->second << std::endl;
    return it_runId->second;
  }

  return -999;
}

bool PhDiMuonUtility::read_in_runIndex()
{
  TOAD toad_loader("PhVecMesonAna");
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

int PhDiMuonUtility::getCentralityBin20(float centrality)
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

int PhDiMuonUtility::getCentralityBin10(float centrality)
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

int PhDiMuonUtility::getCentralityBin4(float centrality)
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

int PhDiMuonUtility::getVertexBin(float vtx_z)
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
