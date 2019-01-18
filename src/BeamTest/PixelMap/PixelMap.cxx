#include <iostream>
#include "./PixelMap.h"

PixelMap::PixelMap()
{
  cout << "PixelMap::PixelMap() ----- Constructor ! ------" << endl;
}

PixelMap::~PixelMap()
{
  cout << "PixelMap::~PixelMap() ----- Release memory ! ------" << endl;
}

void PixelMap::Init_PixelMap_PMT()
{
  int debug = 1;
  double var[1];
  const char * hname = "./H13700_180degree_v2.txt";
  int anode, asic, pin, channel;

  //Right PMT side (front view)
  xp_mRICH[0]=32;
  yp_mRICH[0]=0;
  xp_mRICH[1]=32;
  yp_mRICH[1]=17;

  //Left PMT side (front view)
  xp_mRICH[2]=0;
  yp_mRICH[2]=32;
  xp_mRICH[3]=0;
  yp_mRICH[3]=15;

  FILE* fin = fopen(hname,"r");
  if(!fin) return ;
  while(fscanf(fin,"%lf",var)!=EOF){
    anode   = (int)var[0];
    fscanf(fin,"%lf",var);
    asic   = (int)var[0];
    fscanf(fin,"%lf",var);
    pin    = (int)var[0];
    int tmp;
    if(asic==2)tmp=1;
    if(asic==1)tmp=2;

    if(anode<=128){
      channel = asic*64 + pin;
    }else{
      channel = 192+asic*64 + pin;
      // channel = 191+asic*64 + pin;
    }

    if(channel<384){ maroc2h13700[channel]=anode;
      if(debug==1)printf("H13700 anode %4d  asic %2d  pin %4d  -->  ch %4d maroc %4d \n",anode, asic, pin, channel, maroc2h13700[channel]);
      // if(channel == 128) cout << "maroc2h13700 = " << maroc2h13700[channel] << endl;
    }
  }
}

void PixelMap::GenCoord_PMT(int ipmt, int x1, int y1)
{
  int j;
  int debug=1;
  int rw; // row
  int cm; // column
  for(j=0;j<256;j++)x_mRICH[j]=0;
  for(j=0;j<256;j++)y_mRICH[j]=0;

  for(j=0;j<256;j++){
    rw=(int) j/16.;
    cm=j%16;
    if(ipmt<3){
      x_mRICH[j]=x1-cm; // PMT
      y_mRICH[j]=y1+rw;
    }else{
      x_mRICH[j]=x1+cm; // PMT
      y_mRICH[j]=y1-rw;
    }
    // if(debug)if(j==0||j==255)printf("PMT %2d  Pixel %2d  -->  rw %3d  cm  %3d  X %3d Y %3d\n",ipmt, j+1,rw, cm,x_mRICH[j],y_mRICH[j]);
  }
}

void PixelMap::Init_PixelMap_MPPC()
{
  int debug = 1;
  double var[1];
  const char * hname = "./H13700_180degree_v2.txt";
  int anode, asic, pin, channel;

  //Right MPPC side (front view)
  xp_mRICH[0]=17;
  yp_mRICH[0]=0;
  xp_mRICH[1]=17;
  yp_mRICH[1]=17;

  //Left MPPC side (front view)
  xp_mRICH[2]=15;
  yp_mRICH[2]=32;
  xp_mRICH[3]=15;
  yp_mRICH[3]=15;

  FILE* fin = fopen(hname,"r");
  if(!fin) return ;
  while(fscanf(fin,"%lf",var)!=EOF){
    anode   = (int)var[0];
    fscanf(fin,"%lf",var);
    asic   = (int)var[0];
    fscanf(fin,"%lf",var);
    pin    = (int)var[0];
    int tmp;
    if(asic==2)tmp=1;
    if(asic==1)tmp=2;

    if(anode<=128){
      channel = asic*64 + pin;
    }else{
      channel = 192+asic*64 + pin;
      // channel = 191+asic*64 + pin;
    }

    if(channel<384){ maroc2h13700[channel]=anode;
      if(debug==1)printf("H13700 anode %4d  asic %2d  pin %4d  -->  ch %4d maroc %4d \n",anode, asic, pin, channel, maroc2h13700[channel]);
      // if(channel == 128) cout << "maroc2h13700 = " << maroc2h13700[channel] << endl;
    }
  }
}

void PixelMap::GenCoord_MPPC(int ipmt, int x1, int y1)
{

  int j;
  int debug=1;
  int rw; // row
  int cm; // column
  for(j=0;j<256;j++)x_mRICH[j]=0;
  for(j=0;j<256;j++)y_mRICH[j]=0;

  for(j=0;j<256;j++){
    rw=(int) j/16.;
    cm=j%16;
    if(ipmt<3){
      x_mRICH[j]=x1+cm; // MPPC
      y_mRICH[j]=y1+rw;
    }else{
      x_mRICH[j]=x1-cm; // MPPC
      y_mRICH[j]=y1-rw;
    }
    // if(debug)if(j==0||j==255)printf("PMT %2d  Pixel %2d  -->  rw %3d  cm  %3d  X %3d Y %3d\n",ipmt, j+1,rw, cm,x_mRICH[j],y_mRICH[j]);
  }
}

int PixelMap::GetPMT_mRICH(int slot,int fiber,int asic)
{
  if(fiber==0 || fiber==1)return 1;
  if(fiber==2 || fiber==3)return 2;
  if(fiber==4 || fiber==5)return 3;
  if(fiber==6 || fiber==7)return 4;
}

int PixelMap::GetPixel_mRICH(int fiber, int asic, int maroc_channel)
{
 int k=0;
 if(fiber==1 || fiber==3 || fiber==5 || fiber==7)k=1;
 int i = k*192 + asic*64 + maroc_channel;
//  if(maroc2h13700[i]==0)printf("getpixel fiber %d  asic %d ch %d  -->  ii  %d  %d \n",fiber,asic,maroc_channel,i, maroc2h13700[i]);
 return maroc2h13700[i];
}

