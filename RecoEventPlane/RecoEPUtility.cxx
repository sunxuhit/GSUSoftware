#include "RecoEPUtility.h"
#include <TOAD.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

ClassImp(RecoEPUtility)

//------------------------------------------------------------
RecoEPUtility::RecoEPUtility()
{
}

RecoEPUtility::~RecoEPUtility()
{
}
//------------------------------------------------------------

bool RecoEPUtility::read_in_recal_consts()
{
  TOAD toad_loader("RecoEventPlane");
  std::string fname = toad_loader.location("BbcRecalConsts.txt");
  // std::string fname = "/direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/RecoEventPlane/BbcRecalConsts.txt";
  std::cout << "fname = " << fname.c_str() << std::endl;
  std::ifstream read_consts( fname.c_str() );
  if ( !read_consts.is_open() )
  {
    std::cout<<"Abort. Fail to read in BBC recalibration constants: "<<fname<<std::endl;
    exit(0);
  }

  int _pmt = 0, _rungrp = 0;
  float _ped = 0, _mip = 0;
  std::cout<<"reading BBC recalibration constants: "<<std::endl;
  while (read_consts >> _pmt >> _rungrp >> _ped >> _mip)
  {
    const_pedestal[_pmt][_rungrp] = _ped;
    const_mip[_pmt][_rungrp] = _mip;
  }
  read_consts.close();

  return true;
}

int RecoEPUtility::get_recal_group(int run_num)
{
  if (run_num <= runno_grp1[nrun_grp1-1]) return 1;
  else if (run_num <= runno_grp2[nrun_grp2-1]) return 2;
  else if (run_num <= runno_grp3[nrun_grp3-1]) return 3;
  else if (run_num <= runno_grp4[nrun_grp4-1]) return 4;
  else if (run_num <= runno_grp5[nrun_grp5-1]) return 5;
  else if (run_num <= runno_grp6[nrun_grp6-1]) return 6;
  else if (run_num <= runno_grp7[nrun_grp7-1]) return 7;
  else if (run_num <= runno_grp8[nrun_grp8-1]) return 8;
  else if (run_num <= runno_grp9[nrun_grp9-1]) return 9;
  else if (run_num <= runno_grp10[nrun_grp10-1]) return 10;
  else if (run_num <= runno_grp11[nrun_grp11-1]) return 11;
  else if (run_num <= runno_grp12[nrun_grp12-1]) return 12;
  else if (run_num <= runno_grp13[nrun_grp13-1]) return 13;
  else if (run_num <= runno_grp14[nrun_grp14-1]) return 14;
  else if (run_num <= runno_grp15[nrun_grp15-1]) return 15;
  else if (run_num <= runno_grp16[nrun_grp16-1]) return 16;

  return 0; // no match
}

float RecoEPUtility::get_recal_charge(int PmtIndx, int run_num, int ADC)
{
  int _grp = get_recal_group(run_num);
  float _ped = const_pedestal[PmtIndx][_grp-1];
  float _mip = const_mip[PmtIndx][_grp-1];

  return (ADC-_ped)/(_mip-_ped);
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
