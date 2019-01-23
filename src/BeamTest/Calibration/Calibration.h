#ifndef Calibration_h
#define Calibration_h

#include <string>

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "../PixelMap/PixelMap.h"

using namespace std;

class Calibration : public TObject
{
  public:
    Calibration();
    ~Calibration();

    int Init();
    int InitChain();

    int Make();
    int Finish();

  private:
    PixelMap *pixel_map;

    string mOutPutFile;
    TFile *mFile_OutPut;

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

    ClassDef(Calibration,1)
};

#endif
