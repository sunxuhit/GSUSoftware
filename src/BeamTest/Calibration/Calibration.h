#ifndef Calibration_h
#define Calibration_h

#include <string>

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"

#include "../PixelMap/PixelMap.h"

using namespace std;

class Calibration : public TObject
{
  public:
    Calibration(const string &det = "PMT", const string &mode = "Calibration");
    ~Calibration();

    int Init();
    int InitChain();

    int Make();
    int Finish();

  private:
    PixelMap *pixel_map;
    string mDet, mMode, mHome;

    string mOutPutFile;
    TFile *mFile_OutPut;

    static int const NumOfPixel = 33;
    float const tdc_Start = 2000.0;
    float const tdc_Stop  = 2050.0;
    TH1F *h_mTDC[NumOfPixel][NumOfPixel]; // 0 for x-pixel | 1 for y-pixel
    TH2F *h_mRingImage;

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
