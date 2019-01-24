#ifndef Calibration_h
#define Calibration_h

#include <string>

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

#include "../PixelMap/PixelMap.h"

using namespace std;

class Calibration : public TObject
{
  public:
    Calibration();
    ~Calibration();

    // is processing with PMT? true for PMT | false for MPPC
    void is_PMT(bool b)
    {
      is_pmt = b;
    }

    void set_det(std::string detector)
    {
      mDet = detector;
    }

    int Init();
    int InitChain();
    int InitTdcCut();

    int Make();
    int Finish();

  private:
    PixelMap *pixel_map;
    string mDet, mHome;

    bool is_pmt;

    string mOutPutFile;
    TFile *mFile_OutPut;

    static int const NumOfPixel = 33;
    float mTdc_Start;
    float mTdc_Stop;
    TH1F *h_mTDC[NumOfPixel][NumOfPixel]; // 0 for x-pixel | 1 for y-pixel
    TH2F *h_mRingImage;
    TH2F *h_mRingImage_DisPlay;
    TH2F *h_mRingImage_DisPlay_beam;
    TProfile *p_mNumOfPhotons;

    static const int MAXEDGE = 100000;
    const int MAROCPOLARITY = 1; // 1 falling, 0 rising

    TChain *mChainInPut; // branches list below

    unsigned int tTrigNum;
    double tTrigTime;//long unsigned int tTimestamp;
    unsigned int tNedge;

    unsigned int tPolarity[MAXEDGE];
    unsigned int tChannel[MAXEDGE];
    unsigned int tTime[MAXEDGE];
    unsigned int tSlot[MAXEDGE];
    unsigned int tFiber[MAXEDGE];

    void ResetEventData();

    ClassDef(Calibration,1)
};

#endif
