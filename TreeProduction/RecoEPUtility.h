#ifndef RecoEPUtility_h
#define RecoEPUtility_h

#include <TObject.h>
#include "Run14AuAuList.h"
#include "PhVecMesonCons.h"
#include <map>

class TH1F;
class TH2F;

class RecoEPUtility : public TObject
{
  public:
    RecoEPUtility();
    virtual ~RecoEPUtility();

    bool isGoodRun(int runId);

    void initRunIndex();
    bool read_in_runIndex();
    int find_runIndex(int runId);

    int getCentralityBin20(float centrality);
    int getCentralityBin10(float centrality);
    int getCentralityBin4(float centrality);
    int getVertexBin(float vtx_z);

    //------------BBC Event Plane---------------
    void initBBC();

    bool read_in_recal_consts();
    float get_recal_charge(int PmtIndx, int run_num, int ADC);

    bool read_in_phiweight_corrections();
    float get_phiweight_correction(int PmtIndx, int run_num, float centrality);

    int get_recal_group(int run_num);
    float get_pedestal(int PmtIndx, int run_num);
    float get_mip(int PmtIndx, int run_num);
    bool isSaturatePMT(int PmtIndx);
    bool isBadPMT(int PmtIndx, int run_num);
    //------------BBC Event Plane---------------

  private:

    float const_pedestal[128][GoodRunList::ngrp];
    float const_mip[128][GoodRunList::ngrp];
    float phi_weight[128][GoodRunList::ngrp][vecMesonFlow::mNumOfCentrality20];

    std::map<int,int> map_runIndex;

    ClassDef(RecoEPUtility,1)
};
#endif
