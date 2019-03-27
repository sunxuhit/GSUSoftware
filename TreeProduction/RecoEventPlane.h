#ifndef RecoEventPlane_h
#define RecoEventPlane_h

#include <TObject.h>
#include <TVector2.h>


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

    //-----------------BBC Event Plane---------------------

  private:

    //-----------------BBC Event Plane---------------------
    TVector2 mQVectorRaw_BbcSouth[3]; // 0 for 1st, 1 for 2nd, 2 for 3rd
    TVector2 mQVectorRaw_BbcNorth[3];
    float mQWeight_BbcSouth[3]; 
    float mQWeight_BbcNorth[3];
    unsigned int mNumOfPmts_BbcSouth[3];
    unsigned int mNumOfPmts_BbcNorth[3];
    //-----------------BBC Event Plane---------------------

    ClassDef(RecoEventPlane,1)
};
#endif
