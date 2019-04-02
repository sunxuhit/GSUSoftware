#ifndef RecoEPProManager_h
#define RecoEPProManager_h

#include <TObject.h>
#include <TVector2.h>
#include "PhVecMesonCons.h"

class TProfile2D;

class RecoEPProManager : public TObject
{
  public:
    RecoEPProManager();
    virtual ~RecoEPProManager();

    //===============BBC Event Plane ReCenter=========================
    void initPro_BbcReCenter();
    void fillPro_BbcSouthReCenter(TVector2 QVec_BbcSouth, float QWeight_BbcSouth, int order, int vtx4, int runId, int cent20);
    void fillPro_BbcNorthReCenter(TVector2 QVec_BbcNorth, float QWeight_BbcNorth, int order, int vtx4, int runId, int cent20);
    void writePro_BbcReCenter();
    //===============BBC Event Plane ReCenter=========================


  private:
    //===============BBC Event Plane ReCenter=========================
    // ReCenter Correction | x-axis is runId, y-axis is centrality (every 5%)
    TProfile2D *p_mQx_BbcSouth[3][vecMesonFlow::mNumOfVertex]; // 0 is flow harmonic | 1 is vertex bin
    TProfile2D *p_mQy_BbcSouth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQx_BbcNorth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQy_BbcNorth[3][vecMesonFlow::mNumOfVertex];
    //===============BBC Event Plane ReCenter=========================

    ClassDef(RecoEPProManager,1)
};

#endif
