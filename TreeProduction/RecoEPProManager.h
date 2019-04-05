#ifndef RecoEPProManager_h
#define RecoEPProManager_h

#include <TObject.h>
#include <TVector2.h>
#include "PhVecMesonCons.h"
#include <string>
#include <map>

class TProfile;
class TProfile2D;

class RecoEPProManager : public TObject
{
  public:
    RecoEPProManager();
    virtual ~RecoEPProManager();

    //===============BBC Event Plane ReCenter=========================
    void initPro_BbcReCenter();
    void fillPro_BbcSouthReCenter(TVector2 QVecRaw_BbcSouth, float QWeight_BbcSouth, int order, int vtx4, int runId, int cent20);
    void fillPro_BbcNorthReCenter(TVector2 QVecRaw_BbcNorth, float QWeight_BbcNorth, int order, int vtx4, int runId, int cent20);
    void writePro_BbcReCenter();
    //===============BBC Event Plane ReCenter=========================

    //===============BBC Event Plane Shift=========================
    void initPro_BbcShift();
    void fillPro_BbcSouthShift(float PsiReCenter_BbcSouth, int order, int vtx4, int runId, int cent20);
    void fillPro_BbcNorthShift(float PsiReCenter_BbcNorth, int order, int vtx4, int runId, int cent20);
    void writePro_BbcShift();
    //===============BBC Event Plane Shift=========================


    //===============BBC Event Plane Resolution=========================
    void initPro_BbcResolution();
    void fillPro_BbcResolution(float PsiReCenter_BbcSouth, float PsiReCenter_BbcNorth, int order, int cent20);
    void writePro_BbcResolution();
    //===============BBC Event Plane Resolution=========================

  private:
    //===============BBC Event Plane ReCenter=========================
    // ReCenter Correction | x-axis is runId, y-axis is centrality (every 5%)
    // 0 is flow harmonic | 1 is vertex bin
    TProfile2D *p_mQx_BbcSouth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQy_BbcSouth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQx_BbcNorth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQy_BbcNorth[3][vecMesonFlow::mNumOfVertex];
    //===============BBC Event Plane ReCenter=========================

    //===============BBC Event Plane Shift=========================
    // Shift Correction | x-axis is runId, y-axis is centrality (every 5%)
    // 0 is flow harmonic | 1 is vertex bin | 2 is order of shift correction
    TProfile2D *p_mCos_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mSin_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mCos_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mSin_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    //===============BBC Event Plane Shift=========================

    //===============BBC Event Plane Resolution=========================

    TProfile *p_mResolution_BbcSub[3]; // PsiShift_BbcNorth - PsiShift_BbcSouth

    //===============BBC Event Plane Resolution=========================

    ClassDef(RecoEPProManager,1)
};

#endif
