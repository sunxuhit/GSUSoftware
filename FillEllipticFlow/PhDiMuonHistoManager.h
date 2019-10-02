#ifndef PhDiMuonHistoManager_h
#define PhDiMuonHistoManager_h

#include <TObject.h>
#include <map>
#include "./PhVecMesonCons.h"

class TH1F;

typedef std::map<std::string,TH1F*> TH1FMap;

class PhDiMuonHistoManager : public TObject
{
  public:
    PhDiMuonHistoManager();
    virtual ~PhDiMuonHistoManager();

    // void InitHist(int pid, int mode);
    // void FillHist();
    // void WriteHist(int pid, int mode);

    //===============di-muon InvMass==============
    void initHist_InvMass(int pid, int mode);
    // void fillHist_InvMass(int pid, int mode, int cent20, int cent4, float pt, float phi);
    void writeHist_InvMass(int pid, int mode);
    //===============di-muon InvMass==============

    //===============di-muon Yields==============
    // void initHist_Yields(int pid, int mode);
    // void fillHist_Yields();
    // void writeHist_Yields(int pid, int mode);
    //===============di-muon Yields==============

    //===============di-muon Spectra==============
    // void initHist_Spec(int pid, int mode);
    // void fillHist_Spec();
    // void writeHist_Spec(int pid, int mode);
    //===============di-muon Spectra==============

    //=============utility functions==============
    int getPtBin(float pt);
    int getPhiBin(float phi);
    //=============utility functions==============


  private:
    // elliptic flow analysis
    // 0 = pt bin
    // 1 = centrality: 0-80%, 0-10%, 10%-40%, 40-80%
    // 2 = phi-Psi
    TH1FMap h_mInvMass;
    TH1FMap h_mInvMass_Raw; // raw flow

    // event plane resolution correction
    // 0 = centrality: 0-5%, 5-10%, 10-15% ....75-80%, 80-85%, 85-90%, 90-95%, 95-100%
    TH1FMap h_mYields;

    // raw pt spectra
    // 0 = pt bin
    // 1 = pt bin finer
    // 2 = centrality: 0-80%, 0-10%, 10%-40%, 40-80%
    TH1FMap h_mSpec;


    ClassDef(PhDiMuonHistoManager,1)
};
#endif
