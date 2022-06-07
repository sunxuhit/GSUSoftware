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
    void fillHist_InvMass(int pid, int mode, int cent20, int cent4, float pt, float phi, float invmass, float resolution, float reweight);
    void writeHist_InvMass(int pid, int mode);
    //===============di-muon InvMass==============

    //===============di-muon Yields==============
    void initHist_Yields(int pid, int mode);
    void fillHist_Yields(int pid, int mode, int cent20, float invmass, float reweight);
    void fillHist_YieldsRaw(int pid, int mode, int cent20, float invmass, float reweight);
    void writeHist_Yields(int pid, int mode);
    //===============di-muon Yields==============

    //===============di-muon Spectra==============
    void initHist_Spec(int pid, int mode);
    void fillHist_Spec(int pid, int mode, int cent4, float pt, float invmass, float reweight);
    void fillHist_SpecRaw(int pid, int mode, int cent4, float pt, float invmass, float reweight);
    void writeHist_Spec(int pid, int mode);
    //===============di-muon Spectra==============

    //=============utility functions==============
    int getPtBin(float pt);
    int getPhiBin(float phi);
    std::string genKey_InvMass(int pid, int mode, int pt_bin, int cent4, int phi_bin);
    std::string genKey_InvMassRaw(int pid, int mode, int pt_bin, int cent4, int phi_bin);
    std::string genKey_Yields(int pid, int mode, int cent20);
    std::string genKey_YieldsRaw(int pid, int mode, int cent20);
    std::string genKey_Spec(int pid, int mode, int pt_bin, int cent4);
    std::string genKey_SpecRaw(int pid, int mode, int pt_bin, int cent4);
    //=============utility functions==============

    //===============di-muon InvMass==============
    void initHist_QA(int pid, int mode);
    void fillHist_QA(int pid, int mode, float phi);
    void writeHist_QA(int pid, int mode);
    //===============di-muon InvMass==============

  private:
    // elliptic flow analysis
    // 0 = pt bin
    // 1 = centrality: 0-10%, 10%-40%, 40-80%, 0-80%
    // 2 = phi-Psi
    TH1FMap h_mInvMass;
    TH1FMap h_mInvMass_Raw; // raw flow

    // event plane resolution correction
    // 0 = centrality: 0-5%, 5-10%, 10-15% ....75-80%, 80-85%, 85-90%, 90-95%, 95-100%
    TH1FMap h_mYields;
    TH1FMap h_mYields_Raw; // without cuts

    // raw pt spectra
    // 0 = pt bin
    // 2 = centrality: 0-10%, 10%-40%, 40-80%, 0-80%
    TH1FMap h_mSpec;
    TH1FMap h_mSpec_Raw; // without cuts

    // QA for phi-Psi distribution
    TH1F *h_mQA_Psi;

    ClassDef(PhDiMuonHistoManager,1)
};
#endif
