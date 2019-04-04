#ifndef RecoEventPlane_h
#define RecoEventPlane_h

#include <TObject.h>
#include <TVector2.h>
#include "PhVecMesonCons.h"

class TFile;
class TProfile2D;

class RecoEventPlane : public TObject
{
  public:
    RecoEventPlane();
    virtual ~RecoEventPlane();

    //-----------------BBC Event Plane---------------------
    float getPhi_BbcSouth(float bbcx, float bbcy); 
    float getPhi_BbcNorth(float bbcx, float bbcy); 

    TVector2 calqVector_BbcSouth(float bbcx, float bbcy, int order); // order: 0 for 1st, 1 for 2nd, 2 for 3rd
    TVector2 calqVector_BbcNorth(float bbcx, float bbcy, int order);

    //===============Raw BBC Event Plane=========================
    void initRawBbcEventPlane();
    void clearRawBbcEventPlane();
    void printRawBbcEventPlane(int order);

    void addQVectorRaw_BbcSouth(float bbcx, float bbcy, float weight, int order);
    void addQVectorRaw_BbcNorth(float bbcx, float bbcy, float weight, int order);

    TVector2 getQVectorRaw_BbcSouth(int order);
    TVector2 getQVectorRaw_BbcNorth(int order);
    float getQWeight_BbcSouth(int order);
    float getQWeight_BbcNorth(int order);
    unsigned int getNumOfPmts_BbcSouth(int order);
    unsigned int getNumOfPmts_BbcNorth(int order);

    float calPsiRaw_BbcSouth(int order);
    float calPsiRaw_BbcNorth(int order);
    //===============Raw BBC Event Plane=========================

    //===============ReCenter BBC Event Plane====================
    bool readPro_ReCenter(); // read in ReCenter parameters
    bool closePro_ReCenter(); // close ReCenter parameters

    void initReCenterBbcEventPlane();
    void clearReCenterBbcEventPlane();
    void printReCenterBbcEventPlane(int order, int vtx4, int runId, int cent20);

    TVector2 getQVectorReCenter_BbcSouth(int order, int vtx4, int runId, int cent20);
    TVector2 getQVectorReCenter_BbcNorth(int order, int vtx4, int runId, int cent20);

    float calPsiReCenter_BbcSouth(int order, int vtx4, int runId, int cent20);
    float calPsiReCenter_BbcNorth(int order, int vtx4, int runId, int cent20);
    //===============ReCenter BBC Event Plane====================

    //===============Shift BBC Event Plane====================
    bool readPro_Shift(); // read in Shift parameters
    bool closePro_Shift(); // close Shift parameters

    float shiftAngle(float PsiShift_raw, int order); // shift angle to correct range
    float calPsiShift_BbcSouth(float PsiReCenter_BbcSouth, int order, int vtx4, int runId, int cent20);
    float calPsiShift_BbcNorth(float PsiReCenter_BbcNorth, int order, int vtx4, int runId, int cent20);
    //===============Shift BBC Event Plane====================

    //-----------------BBC Event Plane---------------------

  private:

    //-----------------BBC Event Plane---------------------
    //===============Raw BBC Event Plane=========================
    TVector2 mQVectorRaw_BbcSouth[3]; // 0 for 1st, 1 for 2nd, 2 for 3rd
    TVector2 mQVectorRaw_BbcNorth[3];
    float mQWeight_BbcSouth[3]; 
    float mQWeight_BbcNorth[3];
    unsigned int mNumOfPmts_BbcSouth[3];
    unsigned int mNumOfPmts_BbcNorth[3];
    //===============Raw BBC Event Plane=========================

    //===============ReCenter BBC Event Plane====================
    TFile *File_mInPutReCenter; // read in re-center parameters
    TProfile2D *p_mQx_BbcSouth[3][vecMesonFlow::mNumOfVertex]; // 0 is flow harmonic | 1 is vertex bin
    TProfile2D *p_mQy_BbcSouth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQx_BbcNorth[3][vecMesonFlow::mNumOfVertex];
    TProfile2D *p_mQy_BbcNorth[3][vecMesonFlow::mNumOfVertex];

    TVector2 mQVectorReCenter_BbcSouth[3]; // 0 for 1st, 1 for 2nd, 2 for 3rd
    TVector2 mQVectorReCenter_BbcNorth[3];
    //===============ReCenter BBC Event Plane====================

    //===============Shift BBC Event Plane====================
    TFile *File_mInPutShift; // read in re-center parameters
    TProfile2D *p_mCos_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mSin_BbcSouth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mCos_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];
    TProfile2D *p_mSin_BbcNorth[3][vecMesonFlow::mNumOfVertex][vecMesonFlow::mNumOfShiftOrder];

    //===============Shift BBC Event Plane====================

    //-----------------BBC Event Plane---------------------

    ClassDef(RecoEventPlane,1)
};
#endif
