#ifndef PhDiMuonEvent_h
#define PhDiMuonEvent_h

#include "TMath.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"

class PhDiMuonTrack : public TObject
{
  private:
    // Track properties
    // DiMuon Pairs
    float mInvMass;
    float mPx;
    float mPy;
    float mPz;
    float mRapidity;
    short mCharge;
    float mDcaR;
    float mDcaZ;
    float mVtxChi2;
    float mInvMass_fvtx; // fvtx
    float mInvMass_fvtxmutr; // fvtxmutr
    float mPx_fvtxmutr;
    float mPy_fvtxmutr;
    float mPz_fvtxmutr;
    float mRapidity_fvtxmutr;
    float mVtxChi2_fvtxmutr;

    // Tr0
    float mPx_tr0;
    float mPy_tr0;
    float mPz_tr0;
    float mRapidity_tr0;
    float mDG0_tr0;
    float mDDG0_tr0;
    float mDcaR_tr0;
    float mDcaZ_tr0;
    short mTrhits_tr0;
    float mTrchi2_tr0;
    short mNTrhits_tr0;
    short mIdhits_tr0;
    float mIdchi2_tr0;
    short mNIdhits_tr0;
    float mIdx_tr0;
    float mIdy_tr0;
    short mLastgap_tr0;
    float mPx_fvtx_tr0; // fvtx
    float mPy_fvtx_tr0;
    float mPz_fvtx_tr0;
    float mDphi_fvtx_tr0;
    float mDtheta_fvtx_tr0;
    float mDr_fvtx_tr0;
    float mChi2_fvtx_tr0;
    float mPx_fvtxmutr_tr0; // fvtx & mutr
    float mPy_fvtxmutr_tr0;
    float mPz_fvtxmutr_tr0;
    float mChi2_fvtxmutr_tr0;

    // Tr1
    float mPx_tr1;
    float mPy_tr1;
    float mPz_tr1;
    float mRapidity_tr1;
    float mDG0_tr1;
    float mDDG0_tr1;
    float mDcaR_tr1;
    float mDcaZ_tr1;
    short mTrhits_tr1;
    float mTrchi2_tr1;
    short mNTrhits_tr1;
    short mIdhits_tr1;
    float mIdchi2_tr1;
    short mNIdhits_tr1;
    float mIdx_tr1;
    float mIdy_tr1;
    short mLastgap_tr1;
    float mPx_fvtx_tr1; // fvtx
    float mPy_fvtx_tr1;
    float mPz_fvtx_tr1;
    float mDphi_fvtx_tr1;
    float mDtheta_fvtx_tr1;
    float mDr_fvtx_tr1;
    float mChi2_fvtx_tr1;
    float mPx_fvtxmutr_tr1; // fvtx & mutr
    float mPy_fvtxmutr_tr1;
    float mPz_fvtxmutr_tr1;
    float mChi2_fvtxmutr_tr1;


  public:
    PhDiMuonTrack()
    {
      // DiMuon Pairs
      mInvMass           = -999.9;
      mPx                = -999.9;
      mPy                = -999.9;
      mPz                = -999.9;
      mRapidity          = -999.9;
      mCharge            = -999;
      mDcaR              = -999.9;
      mDcaZ              = -999.9;
      mVtxChi2           = -999.9;
      mInvMass_fvtx      = -999.9; // fvtx
      mInvMass_fvtxmutr  = -999.9; // fvtxmutr
      mPx_fvtxmutr       = -999.9;
      mPy_fvtxmutr       = -999.9;
      mPz_fvtxmutr       = -999.9;
      mRapidity_fvtxmutr = -999.9;
      mVtxChi2_fvtxmutr  = -999.9;

      // Tr0
      mPx_tr0                = -999.9;
      mPy_tr0                = -999.9;
      mPz_tr0                = -999.9;
      mRapidity_tr0          = -999.9;
      mDG0_tr0               = -999.9;
      mDDG0_tr0              = -999.9;
      mDcaR_tr0              = -999.9;
      mDcaZ_tr0              = -999.9;
      mTrhits_tr0            = -999;
      mTrchi2_tr0            = -999.9;
      mNTrhits_tr0           = -999;
      mIdhits_tr0            = -999;
      mIdchi2_tr0            = -999.9;
      mNIdhits_tr0           = -999;
      mIdx_tr0               = -999.9;
      mIdy_tr0               = -999.9;
      mLastgap_tr0           = -999;
      mPx_fvtx_tr0           = -999.9; // fvtx
      mPy_fvtx_tr0           = -999.9;
      mPz_fvtx_tr0           = -999.9;
      mDphi_fvtx_tr0         = -999.9;
      mDtheta_fvtx_tr0       = -999.9;
      mDr_fvtx_tr0           = -999.9;
      mChi2_fvtx_tr0         = -999.9;
      mPx_fvtxmutr_tr0       = -999.9; // fvtx & mutr
      mPy_fvtxmutr_tr0       = -999.9;
      mPz_fvtxmutr_tr0       = -999.9;
      mChi2_fvtxmutr_tr0     = -999.9;

      // Tr1
      mPx_tr1                = -999.9;
      mPy_tr1                = -999.9;
      mPz_tr1                = -999.9;
      mRapidity_tr1          = -999.9;
      mDG0_tr1               = -999.9;
      mDDG0_tr1              = -999.9;
      mDcaR_tr1              = -999.9;
      mDcaZ_tr1              = -999.9;
      mTrhits_tr1            = -999;
      mTrchi2_tr1            = -999.9;
      mNTrhits_tr1           = -999;
      mIdhits_tr1            = -999;
      mIdchi2_tr1            = -999.9;
      mNIdhits_tr1           = -999;
      mIdx_tr1               = -999.9;
      mIdy_tr1               = -999.9;
      mLastgap_tr1           = -999;
      mPx_fvtx_tr1           = -999.9; // fvtx
      mPy_fvtx_tr1           = -999.9;
      mPz_fvtx_tr1           = -999.9;
      mDphi_fvtx_tr1         = -999.9;
      mDtheta_fvtx_tr1       = -999.9;
      mDr_fvtx_tr1           = -999.9;
      mChi2_fvtx_tr1         = -999.9;
      mPx_fvtxmutr_tr1       = -999.9; // fvtx & mutr
      mPy_fvtxmutr_tr1       = -999.9;
      mPz_fvtxmutr_tr1       = -999.9;
      mChi2_fvtxmutr_tr1     = -999.9;
    }
    ~PhDiMuonTrack() {}

    // setters
    void setInvMass(float t)           { mInvMass = t; } // DiMuons
    void setPx(float t)                { mPx = t; }
    void setPy(float t)                { mPy = t; }
    void setPz(float t)                { mPz = t; }
    void setRapidity(float t)          { mRapidity = t; }
    void setCharge(short t)            { mCharge = t; }
    void setDcaR(float t)              { mDcaR = t; }
    void setDcaZ(float t)              { mDcaZ = t; }
    void setVtxChi2(float t)           { mVtxChi2 = t; }
    void setInvMass_fvtx(float t)      { mInvMass_fvtx = t; } // fvtx
    void setInvMass_fvtxmutr(float t)  { mInvMass_fvtxmutr = t; } // fvtxmutr
    void setPx_fvtxmutr(float t)       { mPx_fvtxmutr = t; }
    void setPy_fvtxmutr(float t)       { mPy_fvtxmutr = t; }
    void setPz_fvtxmutr(float t)       { mPz_fvtxmutr = t; }
    void setRapidity_fvtxmutr(float t) { mRapidity_fvtxmutr = t; }
    void setVtxChi2_fvtxmutr(float t)  { mVtxChi2_fvtxmutr = t; }

    void setPx_tr0(float t)           { mPx_tr0 = t; } // Tr0
    void setPy_tr0(float t)           { mPy_tr0 = t; }
    void setPz_tr0(float t)           { mPz_tr0 = t; }
    void setRapidity_tr0(float t)     { mRapidity_tr0 = t; }
    void setDG0_tr0(float t)          { mDG0_tr0 = t; }
    void setDDG0_tr0(float t)         { mDDG0_tr0 = t; }
    void setDcaR_tr0(float t)         { mDcaR_tr0 = t; }
    void setDcaZ_tr0(float t)         { mDcaZ_tr0 = t; }
    void setTrhits_tr0(short t)       { mTrhits_tr0 = t; }
    void setTrchi2_tr0(float t)       { mTrchi2_tr0 = t; }
    void setNTrhits_tr0(short t)      { mNTrhits_tr0 = t; }
    void setIdhits_tr0(short t)       { mIdhits_tr0 = t; }
    void setIdchi2_tr0(float t)       { mIdchi2_tr0 = t; }
    void setNIdhits_tr0(short t)      { mNIdhits_tr0 = t; }
    void setIdx_tr0(float t)          { mIdx_tr0 = t; }
    void setIdy_tr0(float t)          { mIdy_tr0 = t; }
    void setLastgap_tr0(short t)      { mLastgap_tr0 = t; }
    void setPxfvtx_tr0(float t)       { mPx_fvtx_tr0 = t; } // fvtx
    void setPyfvtx_tr0(float t)       { mPy_fvtx_tr0 = t; }
    void setPzfvtx_tr0(float t)       { mPz_fvtx_tr0 = t; }
    void setDphifvtx_tr0(float t)     { mDphi_fvtx_tr0 = t; }
    void setDthetafvtx_tr0(float t)   { mDtheta_fvtx_tr0 = t; }
    void setDrfvtx_tr0(float t)       { mDr_fvtx_tr0 = t; }
    void setChi2fvtx_tr0(float t)     { mChi2_fvtx_tr0 = t; }
    void setPxfvtxmutr_tr0(float t)   { mPx_fvtxmutr_tr0 = t; } // fvtxmutr
    void setPyfvtxmutr_tr0(float t)   { mPy_fvtxmutr_tr0 = t; }
    void setPzfvtxmutr_tr0(float t)   { mPz_fvtxmutr_tr0 = t; }
    void setChi2fvtxmutr_tr0(float t) { mChi2_fvtxmutr_tr0 = t; }

    void setPx_tr1(float t)           { mPx_tr1 = t; } // Tr1
    void setPy_tr1(float t)           { mPy_tr1 = t; }
    void setPz_tr1(float t)           { mPz_tr1 = t; }
    void setRapidity_tr1(float t)     { mRapidity_tr1 = t; }
    void setDG0_tr1(float t)          { mDG0_tr1 = t; }
    void setDDG0_tr1(float t)         { mDDG0_tr1 = t; }
    void setDcaR_tr1(float t)         { mDcaR_tr1 = t; }
    void setDcaZ_tr1(float t)         { mDcaZ_tr1 = t; }
    void setTrhits_tr1(short t)       { mTrhits_tr1 = t; }
    void setTrchi2_tr1(float t)       { mTrchi2_tr1 = t; }
    void setNTrhits_tr1(short t)      { mNTrhits_tr1 = t; }
    void setIdhits_tr1(short t)       { mIdhits_tr1 = t; }
    void setIdchi2_tr1(float t)       { mIdchi2_tr1 = t; }
    void setNIdhits_tr1(short t)      { mNIdhits_tr1 = t; }
    void setIdx_tr1(float t)          { mIdx_tr1 = t; }
    void setIdy_tr1(float t)          { mIdy_tr1 = t; }
    void setLastgap_tr1(short t)      { mLastgap_tr1 = t; }
    void setPxfvtx_tr1(float t)       { mPx_fvtx_tr1 = t; } // fvtx
    void setPyfvtx_tr1(float t)       { mPy_fvtx_tr1 = t; }
    void setPzfvtx_tr1(float t)       { mPz_fvtx_tr1 = t; }
    void setDphifvtx_tr1(float t)     { mDphi_fvtx_tr1 = t; }
    void setDthetafvtx_tr1(float t)   { mDtheta_fvtx_tr1 = t; }
    void setDrfvtx_tr1(float t)       { mDr_fvtx_tr1 = t; }
    void setChi2fvtx_tr1(float t)     { mChi2_fvtx_tr1 = t; }
    void setPxfvtxmutr_tr1(float t)   { mPx_fvtxmutr_tr1 = t; } // fvtxmutr
    void setPyfvtxmutr_tr1(float t)   { mPy_fvtxmutr_tr1 = t; }
    void setPzfvtxmutr_tr1(float t)   { mPz_fvtxmutr_tr1 = t; }
    void setChi2fvtxmutr_tr1(float t) { mChi2_fvtxmutr_tr1 = t; }

    // getters
    float getInvMass() const           { return mInvMass; } // DiMuons
    float getPx() const                { return mPx; }
    float getPy() const                { return mPy; }
    float getPz() const                { return mPz; }
    float getRapidity() const          { return mRapidity; }
    short getCharge() const            { return mCharge; }
    float getDcaR() const              { return mDcaR; }
    float getDcaZ() const              { return mDcaZ; }
    float getVtxChi2() const           { return mVtxChi2; }
    float getInvMass_fvtx() const      { return mInvMass_fvtx; } // fvtx
    float getInvMass_fvtxmutr() const  { return mInvMass_fvtxmutr; } // fvtxmutr
    float getPx_fvtxmutr() const       { return mPx_fvtxmutr; }
    float getPy_fvtxmutr() const       { return mPy_fvtxmutr; }
    float getPz_fvtxmutr() const       { return mPz_fvtxmutr; }
    float getRapidity_fvtxmutr() const { return mRapidity_fvtxmutr; }
    float getVtxChi2_fvtxmutr() const  { return mVtxChi2_fvtxmutr; }

    float getPx_tr0() const           { return mPx_tr0; } // Tr0
    float getPy_tr0() const           { return mPy_tr0; }
    float getPz_tr0() const           { return mPz_tr0; }
    float getRapidity_tr0() const     { return mRapidity_tr0; }
    float getDG0_tr0() const          { return mDG0_tr0; }
    float getDDG0_tr0() const         { return mDDG0_tr0; }
    float getDcaR_tr0() const         { return mDcaR_tr0; }
    float getDcaZ_tr0() const         { return mDcaZ_tr0; }
    short getTrhits_tr0() const       { return mTrhits_tr0; }
    float getTrchi2_tr0() const       { return mTrchi2_tr0; }
    short getNTrhits_tr0() const      { return mNTrhits_tr0; }
    short getIdhits_tr0() const       { return mIdhits_tr0; }
    float getIdchi2_tr0() const       { return mIdchi2_tr0; }
    short getNIdhits_tr0() const      { return mNIdhits_tr0; }
    float getIdx_tr0() const          { return mIdx_tr0; }
    float getIdy_tr0() const          { return mIdy_tr0; }
    short getLastgap_tr0() const      { return mLastgap_tr0; }
    float getPxfvtx_tr0() const       { return mPx_fvtx_tr0; } // fvtx
    float getPyfvtx_tr0() const       { return mPy_fvtx_tr0; }
    float getPzfvtx_tr0() const       { return mPz_fvtx_tr0; }
    float getDphifvtx_tr0() const     { return mDphi_fvtx_tr0; }
    float getDthetafvtx_tr0() const   { return mDtheta_fvtx_tr0; }
    float getDrfvtx_tr0() const       { return mDr_fvtx_tr0; }
    float getChi2fvtx_tr0() const     { return mChi2_fvtx_tr0; }
    float getPxfvtxmutr_tr0() const   { return mPx_fvtxmutr_tr0; } // fvtxmutr
    float getPyfvtxmutr_tr0() const   { return mPy_fvtxmutr_tr0; }
    float getPzfvtxmutr_tr0() const   { return mPz_fvtxmutr_tr0; }
    float getChi2fvtxmutr_tr0() const { return mChi2_fvtxmutr_tr0; }

    float getPx_tr1() const           { return mPx_tr1; } // Tr1
    float getPy_tr1() const           { return mPy_tr1; }
    float getPz_tr1() const           { return mPz_tr1; }
    float getRapidity_tr1() const     { return mRapidity_tr1; }
    float getDG0_tr1() const          { return mDG0_tr1; }
    float getDDG0_tr1() const         { return mDDG0_tr1; }
    float getDcaR_tr1() const         { return mDcaR_tr1; }
    float getDcaZ_tr1() const         { return mDcaZ_tr1; }
    short getTrhits_tr1() const       { return mTrhits_tr1; }
    float getTrchi2_tr1() const       { return mTrchi2_tr1; }
    short getNTrhits_tr1() const      { return mNTrhits_tr1; }
    short getIdhits_tr1() const       { return mIdhits_tr1; }
    float getIdchi2_tr1() const       { return mIdchi2_tr1; }
    short getNIdhits_tr1() const      { return mNIdhits_tr1; }
    float getIdx_tr1() const          { return mIdx_tr1; }
    float getIdy_tr1() const          { return mIdy_tr1; }
    short getLastgap_tr1() const      { return mLastgap_tr1; }
    float getPxfvtx_tr1() const       { return mPx_fvtx_tr1; } // fvtx
    float getPyfvtx_tr1() const       { return mPy_fvtx_tr1; }
    float getPzfvtx_tr1() const       { return mPz_fvtx_tr1; }
    float getDphifvtx_tr1() const     { return mDphi_fvtx_tr1; }
    float getDthetafvtx_tr1() const   { return mDtheta_fvtx_tr1; }
    float getDrfvtx_tr1() const       { return mDr_fvtx_tr1; }
    float getChi2fvtx_tr1() const     { return mChi2_fvtx_tr1; }
    float getPxfvtxmutr_tr1() const   { return mPx_fvtxmutr_tr1; } // fvtxmutr
    float getPyfvtxmutr_tr1() const   { return mPy_fvtxmutr_tr1; }
    float getPzfvtxmutr_tr1() const   { return mPz_fvtxmutr_tr1; }
    float getChi2fvtxmutr_tr1() const { return mChi2_fvtxmutr_tr1; }

    ClassDef(PhDiMuonTrack,1)  // A simple track of a particle
};

class PhDiMuonEvent : public TObject
{
  private:
    TVector3 mVtx_Bbc;
    TVector3 mVtx_Fvtx;
    int mRunId;
    int mRunIndex;
    int mEventId;
    int mCentrality;

    float mPsi_BbcSouth_1st;
    float mPsi_BbcNorth_1st;
    float mPsi_BbcSouth_2nd;
    float mPsi_BbcNorth_2nd;
    float mPsi_BbcSouth_3rd;
    float mPsi_BbcNorth_3rd;

    float mPsi_FvtxSouth_1st;
    float mPsi_FvtxNorth_1st;
    float mPsi_FvtxSouth_2nd;
    float mPsi_FvtxNorth_2nd;
    float mPsi_FvtxSouth_3rd;
    float mPsi_FvtxNorth_3rd;

    int mNumOfDiMuons;
    int mNumOfDiMuonsSouth;
    int mNumOfDiMuonsNorth;

    int fNumTracks;

    TClonesArray* fTracks;      //->

  public:

    PhDiMuonEvent()
    {
      mVtx_Bbc.SetXYZ(-999.9,-999.9,-999.9); // vtx BBC
      mVtx_Fvtx.SetXYZ(-999.9,-999.9,-999.9); // vtx FVTX

      mRunId = -1;
      mRunIndex = -1;
      mEventId = -1;
      mCentrality = -1;

      mPsi_BbcSouth_1st = -999.9;
      mPsi_BbcNorth_1st = -999.9;
      mPsi_BbcSouth_2nd = -999.9;
      mPsi_BbcNorth_2nd = -999.9;
      mPsi_BbcSouth_3rd = -999.9;
      mPsi_BbcNorth_3rd = -999.9;

      mPsi_FvtxSouth_1st = -999.9;
      mPsi_FvtxNorth_1st = -999.9;
      mPsi_FvtxSouth_2nd = -999.9;
      mPsi_FvtxNorth_2nd = -999.9;
      mPsi_FvtxSouth_3rd = -999.9;
      mPsi_FvtxNorth_3rd = -999.9;

      mNumOfDiMuons = 0;
      mNumOfDiMuonsSouth = 0;
      mNumOfDiMuonsNorth = 0;

      fNumTracks = 0;

      fTracks = new TClonesArray( "PhDiMuonTrack", 10 );
    }
    ~PhDiMuonEvent()
    {
      delete fTracks;
      fTracks = NULL;
    }

    // -----------------------------------General Information----------------------------------------
    void setVtxBbc(TVector3 r)  { mVtx_Bbc = r; }
    TVector3 getVtxBbc() const  { return mVtx_Bbc; }

    void setVtxFvtx(TVector3 r) { mVtx_Fvtx = r; }
    TVector3 getVtxFvtx() const { return mVtx_Fvtx; }

    void setRunId(int r)        { mRunId = r; }
    int getRunId() const        { return mRunId; }

    void setRunIndex(int r)     { mRunIndex = r; }
    int getRunIndex() const     { return mRunIndex; }

    void setEventId(int  r)     { mEventId = r; }
    int getEventId() const      { return mEventId; }

    void setCentrality(int r)   { mCentrality = r; }
    int getCentrality() const   { return mCentrality; }
    // -----------------------------------General Information----------------------------------------

    // -----------------------------------BBC Event Plane----------------------------------------
    void setPsi_BbcSouth_1st(float r) { mPsi_BbcSouth_1st = r; }
    float getPsi_BbcSouth_1st() const { return mPsi_BbcSouth_1st; }

    void setPsi_BbcNorth_1st(float r) { mPsi_BbcNorth_1st = r; }
    float getPsi_BbcNorth_1st() const { return mPsi_BbcNorth_1st; }

    void setPsi_BbcSouth_2nd(float r) { mPsi_BbcSouth_2nd = r; }
    float getPsi_BbcSouth_2nd() const { return mPsi_BbcSouth_2nd; }

    void setPsi_BbcNorth_2nd(float r) { mPsi_BbcNorth_2nd = r; }
    float getPsi_BbcNorth_2nd() const { return mPsi_BbcNorth_2nd; }

    void setPsi_BbcSouth_3rd(float r) { mPsi_BbcSouth_3rd = r; }
    float getPsi_BbcSouth_3rd() const { return mPsi_BbcSouth_3rd ; }

    void setPsi_BbcNorth_3rd(float r) { mPsi_BbcNorth_3rd = r; }
    float getPsi_BbcNorth_3rd() const { return mPsi_BbcNorth_3rd; }
    // -----------------------------------BBC Event Plane----------------------------------------

    // -----------------------------------FVTX Event Plane----------------------------------------
    void setPsi_FvtxSouth_1st(float r) { mPsi_FvtxSouth_1st = r; }
    float getPsi_FvtxSouth_1st() const { return mPsi_FvtxSouth_1st; }

    void setPsi_FvtxNorth_1st(float r) { mPsi_FvtxNorth_1st = r; }
    float getPsi_FvtxNorth_1st() const { return mPsi_FvtxNorth_1st; }

    void setPsi_FvtxSouth_2nd(float r) { mPsi_FvtxSouth_2nd = r; }
    float getPsi_FvtxSouth_2nd() const { return mPsi_FvtxSouth_2nd; }

    void setPsi_FvtxNorth_2nd(float r) { mPsi_FvtxNorth_2nd = r; }
    float getPsi_FvtxNorth_2nd() const { return mPsi_FvtxNorth_2nd; }

    void setPsi_FvtxSouth_3rd(float r) { mPsi_FvtxSouth_3rd = r; }
    float getPsi_FvtxSouth_3rd() const { return mPsi_FvtxSouth_3rd ; }

    void setPsi_FvtxNorth_3rd(float r) { mPsi_FvtxNorth_3rd = r; }
    float getPsi_FvtxNorth_3rd() const { return mPsi_FvtxNorth_3rd; }
    // -----------------------------------FVTX Event Plane----------------------------------------

    // -----------------------------------Number of Tracks----------------------------------------
    // Full
    void setNumOfDiMuons(int r)      { mNumOfDiMuons = r; }
    int getNumOfDiMuons() const      { return mNumOfDiMuons; }
    // South
    void setNumOfDiMuonsSouth(int r) { mNumOfDiMuonsSouth = r; }
    int getNumOfDiMuonsSouth() const { return mNumOfDiMuonsSouth; }
    // North
    void setNumOfDiMuonsNorth(int r) { mNumOfDiMuonsNorth = r; }
    int getNumOfDiMuonsNorth() const { return mNumOfDiMuonsNorth; }
    // -----------------------------------Number of Tracks----------------------------------------

    PhDiMuonTrack* createTrack()
    {
      if (fNumTracks == fTracks->GetSize())
	fTracks->Expand( fNumTracks + 10 );
      if (fNumTracks >= 10000)
      {
	Fatal( "PhDiMuonEvent::createTrack()", "ERROR: Too many tracks (>10000)!" );
	exit( 2 );
      }

      new((*fTracks)[fNumTracks++]) PhDiMuonTrack;
      return (PhDiMuonTrack*)((*fTracks)[fNumTracks - 1]);
    }
    void clearTrackList()
    {
      fNumTracks   = 0;
      fTracks      ->Clear();
    }
    int getNumTracks() const
    {
      return fNumTracks;
    }
    PhDiMuonTrack* getTrack(int i) const
    {
      return i < fNumTracks ? (PhDiMuonTrack*)((*fTracks)[i]) : NULL;
    }

    ClassDef(PhDiMuonEvent,1)  // A simple event compiled of tracks
};


#endif // PhDiMuonEvent_h
