#ifndef PixelMap_h
#define PixelMap_h

#include <string>

using namespace std;

class PixelMap
{
  public:
    PixelMap();
    ~PixelMap();

    void Init_PixelMap_PMT();
    void GenCoord_PMT(int ipmt, int x1, int y1);

    void Init_PixelMap_MPPC();
    void GenCoord_MPPC(int ipmt, int x1, int y1);

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
};

#endif
