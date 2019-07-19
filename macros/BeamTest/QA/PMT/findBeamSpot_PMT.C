#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>
#include <vector>
#include <vector>
#include <algorithm>

#define MAXEDGE 100000

// #define H13700MAP "h13700.txt"
#define H13700MAP "../H13700_180degree_v2.txt"

//MAROC configuration polarity (from ssptest_TDCAll.c)
#define MAROCPOLARITY 1

// global variables for display
int x_mRICH[256];
int y_mRICH[256];
int xp_mRICH[4]; //  PMT coordinates X coordinate of pixel 1 of each mapmt
int yp_mRICH[4]; // Y coordinate of pixel 1 of each mapmt

// translation map MAROC/hamamatsu
int maroc2anode[] = {60,58,59,57,52,50,51,49,44,42,43,41,36,34,35,33,28,26,27,25,20,18,19,17,12,10,11,9,4,2,3,1,5,7,6,8,13,15,14,16,21,23,22,24,29,31,30,32,37,39,38,40,45,47,46,48,53,55,54,56,61,63,62,64};
int maroc2h13700[384];

unsigned int tTrigNum;
double tTrigTime;//long unsigned int tTimestamp;
unsigned int tNedge;

unsigned int tPolarity[MAXEDGE];
unsigned int tChannel[MAXEDGE];
unsigned int tTime[MAXEDGE];
unsigned int tSlot[MAXEDGE];
unsigned int tFiber[MAXEDGE];

void InitDisplay_mRICH();
void ResetEventData();
int GetPMT_mRICH(int slot,int fiber,int asic);
void GenCoord_mRICH(int ipmt, int x1, int y1);
int GetPixel_mRICH(int fiber, int asic, int maroc_channel); // return pixel number
float findPixelCoord(int pixel); // return pixel position

void findBeamSpot_PMT(const int runID = 182, const int eventID = 120)
{
  int debug = 1;
  string mode = "Calibration";
  if(runID < 172) mode = "PositionScan";
  if(runID >= 196 && runID <= 215) mode = "AngleRun";
  if(runID >= 266 && runID <= 316) mode = "ThresholdScan";
  if(runID >= 344 && runID <= 380) mode = "MesonRun";

  float tdc_Start = 2000.0;
  float tdc_Stop  = 2050.0;
  if(runID > 343 && runID < 381) // meson run 344-380
  {
    tdc_Start = 490.0;
    tdc_Stop  = 590.0;
  }

  // beam spot
  int beam_x_start = -1;
  int beam_x_stop  = -1;
  int beam_y_start = -1;
  int beam_y_stop  = -1;

  int off_x_start = 0;
  int off_x_stop  = 4;
  int off_y_start = 0;
  int off_y_stop  = 4;

  if(runID >= 172 && runID <= 192) // Calibration
  {
    beam_x_start = 14;
    beam_x_stop  = 18;
    beam_y_start = 12;
    beam_y_stop  = 20;
  }
  if(runID == 16 || (runID >= 18 && runID <= 21)) // PositionScan 
  {
    beam_x_start = 0;
    beam_x_stop  = 5;
    beam_y_start = 27;
    beam_y_stop  = 32;
  }
  if(runID >= 22 && runID <= 35) // PositionScan 
  {
    beam_x_start = 5;
    beam_x_stop  = 11;
    beam_y_start = 20;
    beam_y_stop  = 26;
  }
  if((runID >= 42 && runID <= 51) || (runID >= 54 && runID <= 64)) // PositionScan 
  {
    beam_x_start = 13;
    beam_x_stop  = 15;
    beam_y_start = 17;
    beam_y_stop  = 20;
  }
  if((runID >= 65 && runID <= 70) || (runID >= 88 && runID <= 102)) // PositionScan 
  {
    beam_x_start = 14;
    beam_x_stop  = 18;
    beam_y_start = 12;
    beam_y_stop  = 15;
  }
  if(runID >= 103 && runID <= 122) // PositionScan 
  {
    beam_x_start = 16;
    beam_x_stop  = 19;
    beam_y_start = 12;
    beam_y_stop  = 15;
  }
  if(runID >= 123 && runID <= 142) // PositionScan 
  {
    beam_x_start = 21;
    beam_x_stop  = 26;
    beam_y_start = 4;
    beam_y_stop  = 11;

  }
  if(runID >= 144 && runID <= 163) // PositionScan 
  {
    beam_x_start = 28;
    beam_x_stop  = 32;
    beam_y_start = 0;
    beam_y_stop  = 4;
  }

  InitDisplay_mRICH();

  int const NumOfPixel = 33;
  const double Pixels[NumOfPixel+1] = {-50.5,-47.5,-44.5,-41.5,-38.5,-35.5,-32.5,-29.5,-26.5,-23.5,-20.5,-17.5,-14.5,-11.5,-8.5,-5.5,-2.5,2.5,5.5,8.5,11.5,14.5,17.5,20.5,23.5,26.5,29.5,32.5,35.5,38.5,41.5,44.5,47.5,50.5};

  string inputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/tdc/richTDC_run%d/sspRich.root",runID);
  TFile *File_InPut = TFile::Open(inputfile.c_str());

  float tdc[NumOfPixel][NumOfPixel][2]; // 0 for raising edge | 1 for falling edge
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      tdc[i_pixel_x][i_pixel_y][0] = -999.9;
      tdc[i_pixel_x][i_pixel_y][1] = -999.9;
    }
  }

  // beam spot finder
  std::vector<int> mXBeamMap; // corresponding binX number for each photon hit
  std::vector<int> mYBeamMap; // corresponding binY number for each photon hit
  std::vector<float> mTimeDuration; // corresponding time duration for each photon hit

  //---------------------ring display------------------------- 
  TH2D *h_mRingImage = new TH2D("h_mRingImage","h_mRingImage",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);

  //---------------------cluster display------------------------- 
  TH2D *h_mCluster = new TH2D("h_mCluster","h_mCluster",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_mCluster_1st = new TH2D("h_mCluster_1st","h_mCluster_1st",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_mCluster_2nd = new TH2D("h_mCluster_2nd","h_mCluster_2nd",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  TH2D *h_mCluster_3rd = new TH2D("h_mCluster_3rd","h_mCluster_3rd",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);

  unsigned int pol=MAROCPOLARITY; // 1 falling, 0 rising

  TTree * tree_mRICH = (TTree*)File_InPut->Get("data");
  tree_mRICH->SetBranchAddress("evt",&tTrigNum);
  tree_mRICH->SetBranchAddress("trigtime",&tTrigTime);
  tree_mRICH->SetBranchAddress("nedge",&tNedge);
  tree_mRICH->SetBranchAddress("slot",tSlot);
  tree_mRICH->SetBranchAddress("fiber",tFiber);
  tree_mRICH->SetBranchAddress("ch",tChannel);
  tree_mRICH->SetBranchAddress("pol",tPolarity);
  tree_mRICH->SetBranchAddress("time",tTime);

  int NumOfEvents = tree_mRICH->GetEntries();
  if(NumOfEvents > 50000) NumOfEvents = 50000; // for test only
  printf("NEntries %d\n",NumOfEvents);

  tree_mRICH->GetEntry(0);
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    // if(i_event != 1024 && i_event != 4096 && i_event != 18134) continue;
    if(i_event != eventID) continue;

    mXBeamMap.clear(); // clear beam finder
    mYBeamMap.clear();
    mTimeDuration.clear();

    if(NumOfEvents>20)if(i_event%(NumOfEvents/10)==0)printf("Processing Event %6d\n",i_event);
    ResetEventData();
    tree_mRICH->GetEntry(i_event);

    if(tNedge>MAXEDGE){
      printf("Event to big: %u edges vs %u array size...skip\n",tNedge,MAXEDGE);
      continue;
    }

    for(unsigned int i_photon = 0; i_photon < tNedge; ++i_photon)
    {
      int slot = tSlot[i_photon];
      int fiber = tFiber[i_photon];
      int channel = tChannel[i_photon];
      int asic = channel/64;
      int pin = channel%64;

      if(tSlot[i_photon] < 3 || tSlot[i_photon] > 7){printf("%s EVT %d Data Error: bad slot %d \n",__FUNCTION__,i_event,slot);continue;}
      if(tFiber[i_photon] > 31){printf("%s EVT %d Data Error: bad fiber %d \n",__FUNCTION__,i_event,fiber);continue;}
      if(tChannel[i_photon] > 191){printf("%s EVT %d Data Error: bad channel %d \n",__FUNCTION__,i_photon,channel); continue;}

      int pmt = GetPMT_mRICH(slot,fiber,asic);
      GenCoord_mRICH(pmt, xp_mRICH[pmt-1], yp_mRICH[pmt-1]);
      int pixel = GetPixel_mRICH(fiber, asic, pin);
      int pixel_x = x_mRICH[pixel-1];
      int pixel_y = y_mRICH[pixel-1];

      int polarity = tPolarity[i_photon];

      if(tdc[pixel_x][pixel_y][polarity] < 0)
      {
	if(polarity == 1 && tTime[i_photon] > tdc_Start && tTime[i_photon] < tdc_Stop)
	{
	  tdc[pixel_x][pixel_y][polarity] = tTime[i_photon]; // save tdc of falling edge first
	}
	if(polarity == 0 && tdc[pixel_x][pixel_y][1] > tdc_Start && tdc[pixel_x][pixel_y][1] < tdc_Stop)
	{
	  tdc[pixel_x][pixel_y][polarity] = tTime[i_photon]; // save corresponding tdc of raising edge
	}
      }
    }

    int NumOfPhotons = 0;
    for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x) // fill time duration
    {
      for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
      {
	if(tdc[i_pixel_x][i_pixel_y][1] > tdc_Start && tdc[i_pixel_x][i_pixel_y][1] < tdc_Stop && tdc[i_pixel_x][i_pixel_y][0] > 0)
	{
	  float time_duration = tdc[i_pixel_x][i_pixel_y][0] - tdc[i_pixel_x][i_pixel_y][1];

	  mXBeamMap.push_back(i_pixel_x);
	  mYBeamMap.push_back(i_pixel_y);
	  mTimeDuration.push_back(time_duration);
	  NumOfPhotons++;

	  h_mRingImage->Fill(i_pixel_x,i_pixel_y);
	}
	tdc[i_pixel_x][i_pixel_y][0] = -999.9; // reset tdc array
	tdc[i_pixel_x][i_pixel_y][1] = -999.9;
      }
    }

    /*
    cout << "i_event = " << i_event << ", number of photons = " << NumOfPhotons << ", number of pixels = " << mXBeamMap.size() << endl;
    for(int i_pixel = 0; i_pixel < mXBeamMap.size(); ++i_pixel)
    {
      cout << "pixel_x = " << mXBeamMap[i_pixel] << ", pixel_y = " << mYBeamMap[i_pixel] << ", time = " << mTimeDuration[i_pixel] << endl;
    }
    */
    // cout << "i_event = " << i_event << ", number of photons = " << NumOfPhotons << ", number of pixels = " << mTimeDuration.size() << endl;
    const int mNumOfPhotons = NumOfPhotons;
    int beam_counter[mNumOfPhotons];
    int mNumOfCluster = 0;

    std::vector<int> mXClusterMap; // corresponding binX number for each cluster
    std::vector<int> mYClusterMap; // corresponding binY number for each cluster
    std::vector<int> mRankMap; // number of surrounding photons 
    mXClusterMap.clear(); // clear beam finder
    mYClusterMap.clear();
    mRankMap.clear();

    // beam finder
    for(int i_beam = 0; i_beam < mTimeDuration.size(); ++i_beam) // first round of cluster
    {
      beam_counter[i_beam] = 0;
      for(int i_cluster = 0; i_cluster < mTimeDuration.size(); ++i_cluster)
      {
	int delta_x = TMath::Abs(mXBeamMap[i_cluster]-mXBeamMap[i_beam]);
	int delta_y = TMath::Abs(mYBeamMap[i_cluster]-mYBeamMap[i_beam]);
	if(delta_x < 3 && delta_y < 3) // 5*5 pixel cluster
	{
	  beam_counter[i_beam]++;
	}
      }
      if(beam_counter[i_beam] > 4)
      {
	mXClusterMap.push_back(mXBeamMap[i_beam]);
	mYClusterMap.push_back(mYBeamMap[i_beam]);
	mRankMap.push_back(beam_counter[i_beam]);
      }
    }

    if(mRankMap.size() == 0) 
    {
      cout << "no cluster found in event: " << i_event << endl;
      continue;
    }

    for(int i_cluster = 0; i_cluster < mRankMap.size(); ++i_cluster)
    {
      h_mCluster->Fill(mXClusterMap[i_cluster],mYClusterMap[i_cluster],mRankMap[i_cluster]);
    }

    std::vector<int>::iterator it_max_1st = max_element(std::begin(mRankMap),std::end(mRankMap)); // most ranked pixel
    int cluster_1st = std::distance(mRankMap.begin(), it_max_1st);
    // std::cout << "max element at: " << cluster_1st << endl;
    // cout << "max rank = " << mRankMap[cluster_1st] << endl;

    std::vector<int> mXCluster_1st;
    std::vector<int> mYCluster_1st;
    std::vector<int> mRank_1st;
    mXCluster_1st.clear();
    mYCluster_1st.clear();
    mRank_1st.clear();

    std::vector<int> mXCluster_rest;
    std::vector<int> mYCluster_rest;
    std::vector<int> mRank_rest;
    mXCluster_rest.clear();
    mYCluster_rest.clear();
    mRank_rest.clear();

    for(int i_cluster = 0; i_cluster < mXClusterMap.size(); ++i_cluster)
    {
      int delta_x = TMath::Abs(mXClusterMap[i_cluster]-mXClusterMap[cluster_1st]);
      int delta_y = TMath::Abs(mYClusterMap[i_cluster]-mYClusterMap[cluster_1st]);
      if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
      {
	mXCluster_1st.push_back(mXClusterMap[i_cluster]);
	mYCluster_1st.push_back(mYClusterMap[i_cluster]);
	mRank_1st.push_back(mRankMap[i_cluster]);
	cout << "1st cluster Ranking: " << mRankMap[i_cluster] << endl;
      }
      else
      {
	mXCluster_rest.push_back(mXClusterMap[i_cluster]);
	mYCluster_rest.push_back(mYClusterMap[i_cluster]);
	mRank_rest.push_back(mRankMap[i_cluster]);
	// cout << "rest cluster Ranking: " << mRankMap[i_cluster] << endl;
      }
    }
    if(mRank_1st.size() > 0) mNumOfCluster++;

    for(int i_cluster = 0; i_cluster < mRank_1st.size(); ++i_cluster) // fill 1st cluster
    {
      h_mCluster_1st->Fill(mXCluster_1st[i_cluster],mYCluster_1st[i_cluster],mRank_1st[i_cluster]);
    }

    std::vector<int> mXCluster_2nd;
    std::vector<int> mYCluster_2nd;
    std::vector<int> mRank_2nd;
    mXCluster_2nd.clear();
    mYCluster_2nd.clear();
    mRank_2nd.clear();

    std::vector<int> mXCluster_3rd;
    std::vector<int> mYCluster_3rd;
    std::vector<int> mRank_3rd;
    mXCluster_3rd.clear();
    mYCluster_3rd.clear();
    mRank_3rd.clear();

    if(mRank_rest.size() > 0)
    {
      std::vector<int>::iterator it_max_2nd = max_element(std::begin(mRank_rest),std::end(mRank_rest)); // most ranked pixel in 2nd cluster
      int cluster_2nd = std::distance(mRank_rest.begin(), it_max_2nd);
      // std::cout << "max element at: " << cluster_2nd << endl;
      // cout << "max rank = " << mRank_rest[cluster_2nd] << endl;

      for(int i_cluster = 0; i_cluster < mXCluster_rest.size(); ++i_cluster)
      {
	int delta_x = TMath::Abs(mXCluster_rest[i_cluster]-mXCluster_rest[cluster_2nd]);
	int delta_y = TMath::Abs(mYCluster_rest[i_cluster]-mYCluster_rest[cluster_2nd]);
	if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
	{
	  mXCluster_2nd.push_back(mXCluster_rest[i_cluster]);
	  mYCluster_2nd.push_back(mYCluster_rest[i_cluster]);
	  mRank_2nd.push_back(mRank_rest[i_cluster]);
	  cout << "2nd cluster Ranking: " << mRank_rest[i_cluster] << endl;
	}
	else
	{
	  mXCluster_3rd.push_back(mXCluster_rest[i_cluster]);
	  mYCluster_3rd.push_back(mYCluster_rest[i_cluster]);
	  mRank_3rd.push_back(mRank_rest[i_cluster]);
	  cout << "3rd cluster Ranking: " << mRank_rest[i_cluster] << endl;
	}
      }
    }
    if(mRank_2nd.size() > 0) mNumOfCluster++;
    if(mRank_3rd.size() > 0) mNumOfCluster++;

    for(int i_cluster = 0; i_cluster < mRank_2nd.size(); ++i_cluster) // fill 2nd cluster
    {
      h_mCluster_2nd->Fill(mXCluster_2nd[i_cluster],mYCluster_2nd[i_cluster],mRank_2nd[i_cluster]);
    }
    for(int i_cluster = 0; i_cluster < mRank_3rd.size(); ++i_cluster) // fill 3rd cluster
    {
      h_mCluster_3rd->Fill(mXCluster_3rd[i_cluster],mYCluster_3rd[i_cluster],mRank_3rd[i_cluster]);
    }

    cout << mNumOfCluster << " cluster(s) found in event: " << i_event << endl;
  }
  printf("Processed events %d\n",NumOfEvents);

  string outputfile = Form("/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/PMT/%s/BeamSpot_run%d.root",mode.c_str(),runID);
  TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
  h_mRingImage->Write();
  h_mCluster->Write();
  h_mCluster_1st->Write();
  h_mCluster_2nd->Write();
  h_mCluster_3rd->Write();
  File_OutPut->Close();
}

//----------------------------------
void InitDisplay_mRICH()
{
  int debug = 1;
  double var[1];
  const char * hname = H13700MAP;
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

//----------------------------------
void ResetEventData()
{
  tTrigNum=0;
  tTrigTime=0;
  tNedge=0;
  for(unsigned int j=0;j<MAXEDGE;j++)
  {
    tPolarity[j]=-1;
    tChannel[j]=-1;
    tTime[j]=-1;
    tSlot[j]=-1;
    tFiber[j]=-1;
  }
}

//------------------------------
int GetPMT_mRICH(int slot,int fiber,int asic)
{
  if(fiber==0 || fiber==1)return 1;
  if(fiber==2 || fiber==3)return 2;
  if(fiber==4 || fiber==5)return 3;
  if(fiber==6 || fiber==7)return 4;
}

//------------------------------
int GetPixel_mRICH(int fiber, int asic, int maroc_channel)
{
 int k=0;
 if(fiber==1 || fiber==3 || fiber==5 || fiber==7)k=1;
 int i = k*192 + asic*64 + maroc_channel;
//  if(maroc2h13700[i]==0)printf("getpixel fiber %d  asic %d ch %d  -->  ii  %d  %d \n",fiber,asic,maroc_channel,i, maroc2h13700[i]);
 return maroc2h13700[i];
}

//------------------------------
void GenCoord_mRICH(int ipmt, int x1, int y1)
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

float findPixelCoord(int pixel)
{
  float out_coord = -999.9;

  const int mNumOfPixels = 33; // 16*2 3mm-pixels + 1 2*2mm-glasswindow+1mm-gap
  const double mPixels[mNumOfPixels+1] = {-50.5,-47.5,-44.5,-41.5,-38.5,-35.5,-32.5,-29.5,-26.5,-23.5,-20.5,-17.5,-14.5,-11.5,-8.5,-5.5,-2.5,2.5,5.5,8.5,11.5,14.5,17.5,20.5,23.5,26.5,29.5,32.5,35.5,38.5,41.5,44.5,47.5,50.5};

  if(pixel < 0 || pixel > 32) return -999.9; // out of photon sensor

  float out_low = mPixels[pixel];
  float out_high = mPixels[pixel+1];

  out_coord = 0.5*(out_low+out_high);

  return out_coord;
}
