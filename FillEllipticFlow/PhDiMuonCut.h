#ifndef PhDiMuonCut_h
#define PhDiMuonCut_h

#include <TObject.h>
#include <TVector3.h>
#include "PhVecMesonCons.h"

class PhDiMuonCut : public TObject
{
  public:
    PhDiMuonCut();
    virtual ~PhDiMuonCut();

    //----------------------------------------------------
    //===================event cut========================
    bool passVtxCut(TVector3 vtx_BBC); // BBC vtx cut
    //----------------------------------------------------

    //----------------------------------------------------
    //===================track cut========================
    // di-muon cuts
    bool passDiMuonVtxCuts(float rapidity, float vtxchi2, float vtxchi2_fvtxmutr);
    bool passDiMuonChargeCuts(short charge, int mode);
    bool isSouthDiMuon(float rapidity); // -2.2 < rapidity < -1.2
    bool isNorthDiMuon(float rapidity); //  1.2 < rapidity < 2.2

    // si-muon cuts
    bool passSiMuonDGCuts(float rapidity, float DG0, float DDG0, float pz);
    bool passSiMuonMutrCuts(float rapidity, float trchi2, short ntrhits, float idchi2, short nidhits, short lastgap);
    bool passSiMuonDcaCuts(float rapidity, float dca_r, float dca_z);

    //=============utility functions==============
    int getRapidityBin(float rapidity);

  private:

    ClassDef(PhDiMuonCut,1)
};
#endif

