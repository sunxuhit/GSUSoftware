#include <iostream>
#include <fstream>
#include "./Calibration.h"

ClassImp(Calibration)

// Calibration::Calibration(string outputfile)
Calibration::Calibration(const string &det, const string &mode)
{
  cout << "Calibration::Calibration() ----- Constructor ! ------" << endl;
  mDet = det;
  mMode = mode;
  mHome = getenv("HOME");
}

Calibration::~Calibration()
{
  cout << "Calibration::~Calibration() ----- Release memory ! ------" << endl;
}

int Calibration::Init()
{
  mOutPutFile = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/%s/BeamTest_%s.root",mHome.c_str(),mDet.c_str(),mMode.c_str());
  cout << "Calibration::Init(), create output file: "<< mOutPutFile.c_str() <<endl;
  mFile_OutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  pixel_map = new PixelMap();
  if(mDet == "PMT") 
  {
    cout << "Initialize Pixel Map for PMT runs: " << endl;
    pixel_map->Init_PixelMap_PMT();
  }
  if(mDet == "MPPC") 
  {
    cout << "Initialize Pixel Map for MPPC runs: " << endl;
    pixel_map->Init_PixelMap_MPPC();
  }

  InitChain();

  cout << "Initialize QA histograms: " << endl;

  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      string HistName = Form("h_mTDC_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
      cout << HistName.c_str() << endl;
      h_mTDC[i_pixel_x][i_pixel_y] = new TH1F(HistName.c_str(),HistName.c_str(),5000,-0.5,4999.5);
    }
  }
  h_mRingImage = new TH2F("h_mRingImage","h_mRingImage",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);

  return 0;
}

int Calibration::InitChain()
{
 string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/tdc/",mHome.c_str());
 string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/%s/%s/proton_calibration_1.list",mHome.c_str(),mDet.c_str(),mMode.c_str());

 mChainInPut = new TChain("data");

 if (!InPutList.empty())   // if input file is ok
 {
   cout << "Open input probability file list" << endl;
   ifstream in(InPutList.c_str());  // input stream
   if(in)
   {
     cout << "input file probability list is ok" << endl;
     char str[255];       // char array for each file name
     Long64_t entries_save = 0;
     while(in)
     {
       in.getline(str,255);  // take the lines of the file list
       if(str[0] != 0)
       {
	 string addfile;
	 addfile = str;
	 addfile = inputdir + addfile + "/sspRich.root";
	 mChainInPut->AddFile(addfile.c_str(),-1,"data");
	 Long64_t file_entries = mChainInPut->GetEntries();
	 cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	 entries_save = file_entries;
       }
     }
   }
   else
   {
     cout << "WARNING: input probability file input is problemtic" << endl;
   }
 }

 mChainInPut->SetBranchAddress("evt",&tTrigNum);
 mChainInPut->SetBranchAddress("trigtime",&tTrigTime);
 mChainInPut->SetBranchAddress("nedge",&tNedge);
 mChainInPut->SetBranchAddress("slot",tSlot);
 mChainInPut->SetBranchAddress("fiber",tFiber);
 mChainInPut->SetBranchAddress("ch",tChannel);
 mChainInPut->SetBranchAddress("pol",tPolarity);
 mChainInPut->SetBranchAddress("time",tTime);

 long NumOfEvents = (long)mChainInPut->GetEntries();
 cout << "total number of events: " << NumOfEvents << endl;

 return 0;
}

int Calibration::Make()
{
  cout << " this is Calibration::Make" << endl;

  // long NumOfEvents = (long)mChainInPut->GetEntries();
  long NumOfEvents = 1000;
  mChainInPut->GetEntry(0);
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(NumOfEvents>20)if(i_event%(NumOfEvents/10)==0)printf("Processing Event %6d\n",i_event);
    ResetEventData();
    mChainInPut->GetEntry(i_event);

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

      int pixel_x = pixel_map->get_Pixel_x(slot, fiber, asic, pin, mDet);
      int pixel_y = pixel_map->get_Pixel_y(slot, fiber, asic, pin, mDet);

      if(pixel_x < 0 || pixel_y < 0) return -1;

      if(tPolarity[i_photon] == MAROCPOLARITY) h_mTDC[pixel_x][pixel_y]->Fill(tTime[i_photon]);

      if(tPolarity[i_photon] == MAROCPOLARITY && tTime[i_photon] > tdc_Start && tTime[i_photon] < tdc_Stop)
      {
	h_mRingImage->Fill(pixel_x,pixel_y);
      }
    }
  }
  printf("Processed events %d\n",NumOfEvents);

  return 0;
}

int Calibration::Finish()
{
  cout << " this is Calibration::Finish" << endl;
  mFile_OutPut->cd();
  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      h_mTDC[i_pixel_x][i_pixel_y]->Write();
    }
  }
  h_mRingImage->Write();
  mFile_OutPut->Close();

  return 0;
}

void Calibration::ResetEventData()
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

/*
int main()
{
  Calibration *Calibration_mRICH = new Calibration();

  Calibration_mRICH->Init();
  Calibration_mRICH->Make();
  Calibration_mRICH->Finish();

  cout << "This is the end of pid!!!" << endl;

  return 0;
}
*/
