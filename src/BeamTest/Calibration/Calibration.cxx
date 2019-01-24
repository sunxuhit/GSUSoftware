#include <iostream>
#include <fstream>
#include <cmath>
#include "./Calibration.h"

ClassImp(Calibration)

// Calibration::Calibration(string outputfile)
Calibration::Calibration() : mDet("PMT"), is_pmt(true), mTdc_Start(2000.0), mTdc_Stop(2050.0)
{
  cout << "Calibration::Calibration() ----- Constructor ! ------" << endl;
  mHome = getenv("HOME");
}

Calibration::~Calibration()
{
  cout << "Calibration::~Calibration() ----- Release memory ! ------" << endl;
  delete pixel_map;
}

int Calibration::Init()
{
  mOutPutFile = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/%s/BeamTest_Calibration.root",mHome.c_str(),mDet.c_str());
  cout << "Calibration::Init(), create output file: "<< mOutPutFile.c_str() <<endl;
  mFile_OutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  pixel_map = new PixelMap();
  if( is_pmt ) 
  {
    cout << "Initialize Pixel Map for PMT runs: " << endl;
    pixel_map->Init_PixelMap_PMT();
  }
  if( !is_pmt ) 
  {
    cout << "Initialize Pixel Map for MPPC runs: " << endl;
    pixel_map->Init_PixelMap_MPPC();
  }

  InitChain();

  InitTdcCut();

  for(int i_pixel_x = 0; i_pixel_x < NumOfPixel; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < NumOfPixel; ++i_pixel_y)
    {
      string HistName = Form("h_mTDC_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
      // cout << HistName.c_str() << endl;
      h_mTDC[i_pixel_x][i_pixel_y] = new TH1F(HistName.c_str(),HistName.c_str(),5000,-0.5,4999.5);
    }
  }
  h_mRingImage = new TH2F("h_mRingImage","h_mRingImage",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  h_mRingImage_DisPlay = new TH2F("h_mRingImage_DisPlay","h_mRingImage_DisPlay",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);
  h_mRingImage_DisPlay_beam = new TH2F("h_mRingImage_DisPlay_beam","h_mRingImage_DisPlay_beam",NumOfPixel,-0.5,32.5,NumOfPixel,-0.5,32.5);

  cout << "Initialized QA histograms. " << endl;

  p_mNumOfPhotons = new TProfile("p_mNumOfPhotons","p_mNumOfPhotons",1,-0.5,0.5);

  cout << "Initialized TProfile for Number of Photons. " << endl;

  return 0;
}

int Calibration::InitChain()
{
 string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/tdc/",mHome.c_str());
 string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/%s/Calibration/proton_calibration.list",mHome.c_str(),mDet.c_str());

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

int Calibration::InitTdcCut()
{
  string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/%s/Calibration/",mHome.c_str(),mDet.c_str());
  string inputfile;
  if( is_pmt )
  {
    inputfile = inputdir + "richTimeCuts.root";
  }
  if( !is_pmt )
  {
    inputfile = inputdir + "sipmTimeCuts.root";
  }

  float NumOfRuns = 0.0;
  float SumTdcStart = 0.0;
  float SumTdcStop  = 0.0;

  TFile *File_mTDC = TFile::Open(inputfile.c_str());
  TH2F *h_mTimeCuts = (TH2F*)File_mTDC->Get("h_mTimeCuts")->Clone();
  TH1F *h_mTdcStart = (TH1F*)h_mTimeCuts->ProjectionY("h_mTdcStart",1,1)->Clone();
  TH1F *h_mTdcStop  = (TH1F*)h_mTimeCuts->ProjectionY("h_mTdcStop",2,2)->Clone();
  TH1F *h_mRunId    = (TH1F*)h_mTimeCuts->ProjectionY("h_mRunId",3,3)->Clone();
  for(int i_bin = 1; i_bin < h_mRunId->GetNbinsX(); ++i_bin)
  {
    float runId     = h_mRunId->GetBinContent(i_bin);
    float tdc_start = h_mTdcStart->GetBinContent(i_bin);
    float tdc_stop  = h_mTdcStop->GetBinContent(i_bin);
    if(runId > 0)
    {
      NumOfRuns++;
      SumTdcStart += tdc_start;
      SumTdcStop  += tdc_stop;
      cout << "runId = " << runId << ", tdc_start = " << tdc_start << ", tdc_stop = " << tdc_stop << ", NumOfRuns = " << NumOfRuns << endl;
    }
  }
  File_mTDC->Close();

  mTdc_Start = floor(SumTdcStart/NumOfRuns);
  mTdc_Stop  = ceil(SumTdcStop/NumOfRuns);

  cout << "mean_tdc_Start = " << SumTdcStart/NumOfRuns << ", mean_tdc_Stop = " << SumTdcStop/NumOfRuns << endl;
  cout << "mTdc_Start set to " << mTdc_Start << " and mTdc_Stop set to " << mTdc_Stop << endl;

  return 0;
}

int Calibration::Make()
{
  cout << " this is Calibration::Make" << endl;

  // long NumOfEvents = (long)mChainInPut->GetEntries();
  long NumOfEvents = 5000;
  mChainInPut->GetEntry(0);
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(NumOfEvents>20)if(i_event%(NumOfEvents/100)==0)printf("Processing Event %6d\n",i_event);
    ResetEventData();
    mChainInPut->GetEntry(i_event);

    if(tNedge>MAXEDGE){
      printf("Event to big: %u edges vs %u array size...skip\n",tNedge,MAXEDGE);
      continue;
    }

    int NumOfPhotons = 0;
    for(unsigned int i_photon = 0; i_photon < tNedge; ++i_photon)
    {
      int slot = tSlot[i_photon];
      int fiber = tFiber[i_photon];
      int channel = tChannel[i_photon];
      int asic = channel/64;
      int pin = channel%64;

      int pixel_x = -1;
      int pixel_y = -1;

      if(asic == 0) 
      {
	// corresponding to empty MPPC 
	continue;
      }

      if(tSlot[i_photon] < 3 || tSlot[i_photon] > 7)
      {
	printf("%s EVT %d Data Error: bad slot %d \n",__FUNCTION__,i_event,slot);
	continue;
      }
      if(tFiber[i_photon] > 31)
      {
	printf("%s EVT %d Data Error: bad fiber %d \n",__FUNCTION__,i_event,fiber);
	continue;
      }
      if(tChannel[i_photon] > 191)
      {
	printf("%s EVT %d Data Error: bad channel %d \n",__FUNCTION__,i_photon,channel); 
	continue;
      }

      if( is_pmt ) 
      {
	pixel_x = pixel_map->get_Pixel_x_PMT(slot, fiber, asic, pin);
	pixel_y = pixel_map->get_Pixel_y_PMT(slot, fiber, asic, pin);
      }
      if( !is_pmt ) 
      {
	pixel_x = pixel_map->get_Pixel_x_MPPC(slot, fiber, asic, pin);
	pixel_y = pixel_map->get_Pixel_y_MPPC(slot, fiber, asic, pin);
      }

      if(pixel_x < 0 || pixel_y < 0) return -1;

      if(tPolarity[i_photon] == MAROCPOLARITY) 
      {
	h_mTDC[pixel_x][pixel_y]->Fill(tTime[i_photon]);
      }

      if(tPolarity[i_photon] == MAROCPOLARITY && tTime[i_photon] > mTdc_Start && tTime[i_photon] < mTdc_Stop)
      {
	h_mRingImage->Fill(pixel_x,pixel_y);
	if(i_event > 1024 && i_event < 1124)
	{
	  h_mRingImage_DisPlay_beam->Fill(pixel_x,pixel_y);
	}
	bool is_beam = (pixel_x > 12 && pixel_x < 20) && (pixel_y > 12 && pixel_y < 20);
	if( !is_beam ) // exclude beam spot
	{
	  if(i_event > 1024 && i_event < 1124)
	  {
	    h_mRingImage_DisPlay->Fill(pixel_x,pixel_y);
	  }
	  NumOfPhotons++;
	}
      }
    }
    // cout << "NumOfPhotons = " << NumOfPhotons << endl;
    p_mNumOfPhotons->Fill(0.0,NumOfPhotons);
  }
  printf("Processed events %ld\n",NumOfEvents);

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
  h_mRingImage_DisPlay->Write();
  h_mRingImage_DisPlay_beam->Write();
  p_mNumOfPhotons->Write();
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

//--------------------------------------------------------------------
