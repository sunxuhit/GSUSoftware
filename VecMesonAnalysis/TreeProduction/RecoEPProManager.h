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

    //===============BBC Event Plane Resolution=========================
    void initPro_BbcResolution();
    void fillPro_BbcResolution(float Psi_BbcSouth, float Psi_BbcNorth, int order, int runIndex, int cent20, int cent10);
    void writePro_BbcResolution();
    //===============BBC Event Plane Resolution=========================

    //===============FVTX Event Plane Resolution=========================
    void initPro_FvtxResolution();
    void fillPro_FvtxResolution(float Psi_FvtxSouth, float Psi_FvtxNorth, int order, int runIndex, int cent20, int cent10);
    void writePro_FvtxResolution();
    //===============FVTX Event Plane Resolution=========================

  private:
    //===============BBC Event Plane Resolution=========================

    TProfile *p_mResolution20_BbcSub[3]; // PsiShift_BbcNorth - PsiShift_BbcSouth
    TProfile *p_mResolution10_BbcSub[3]; // PsiShift_BbcNorth - PsiShift_BbcSouth
    // Resolution | x-axis is runIndex, y-axis is centrality (every 5%)
    TProfile2D *p_mResQA_BbcSub[3]; // PsiShift_BbcNorth - PsiShift_BbcSouth vs. runIndex

    TProfile *p_mResolution20_FvtxSub[3];
    TProfile *p_mResolution10_FvtxSub[3];
    TProfile2D *p_mResQA_FvtxSub[3];

    //===============BBC Event Plane Resolution=========================

    ClassDef(RecoEPProManager,1)
};

#endif
