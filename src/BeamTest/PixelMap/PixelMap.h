#ifndef PixelMap_h
#define PixelMap_h

#include "TObject.h"

class PixelMap : public TObject
{
  public:
    PixelMap();
    ~PixelMap();

    void Init_PixelMap_PMT();
    void GenCoord_PMT(int ipmt, int x1, int y1);
    int get_Pixel_x_PMT(int slot, int fiber, int asic, int pin);
    int get_Pixel_y_PMT(int slot, int fiber, int asic, int pin);

    void Init_PixelMap_MPPC();
    void GenCoord_MPPC(int ipmt, int x1, int y1);
    int get_Pixel_x_MPPC(int slot, int fiber, int asic, int pin);
    int get_Pixel_y_MPPC(int slot, int fiber, int asic, int pin);

    int GetPMT_mRICH(int slot,int fiber,int asic);
    int GetPixel_mRICH(int fiber, int asic, int maroc_channel);


  private:
    // global variables for display
    int x_mRICH[256];
    int y_mRICH[256];
    int xp_mRICH[4]; //  PMT coordinates X coordinate of pixel 1 of each mapmt
    int yp_mRICH[4]; // Y coordinate of pixel 1 of each mapmt

    // translation map MAROC/hamamatsu
    int maroc2anode[64] = {60,58,59,57,52,50,51,49,44,42,43,41,36,34,35,33,28,26,27,25,20,18,19,17,12,10,11,9,4,2,3,1,5,7,6,8,13,15,14,16,21,23,22,24,29,31,30,32,37,39,38,40,45,47,46,48,53,55,54,56,61,63,62,64};
    int maroc2h13700[384];

    ClassDef(PixelMap,1)
};

#endif
