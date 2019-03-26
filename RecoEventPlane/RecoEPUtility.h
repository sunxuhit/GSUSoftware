#ifndef RecoEPUtility_h
#define RecoEPUtility_h

#include <TObject.h>
#include "Run14AuAuList.h"

class TH1F;
class TH2F;

class RecoEPUtility : public TObject
{
  public:
    RecoEPUtility();
    virtual ~RecoEPUtility();

    bool read_in_recal_consts();
    int get_recal_group(int run_num);
    float get_recal_charge(int PmtIndx, int run_num, int ADC);
    float get_pedestal(int PmtIndx, int run_num);
    float get_mip(int PmtIndx, int run_num);
    bool isSaturatePMT(int PmtIndx);
    bool isBadPMT(int PmtIndx, int run_num);

  private:

    float const_pedestal[128][GoodRunList::ngrp];
    float const_mip[128][GoodRunList::ngrp];

    ClassDef(RecoEPUtility,1)
};
#endif
