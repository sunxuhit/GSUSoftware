#include <iostream>
#include <fstream>
#include <cmath>
#include <TMath.h>

#include "./Calibration.h"
#include "../PixelMap/PixelMap.h"

using namespace std;

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
  mOutPutFile = Form("%s/WorkSpace/EICPID/OutPut/BeamTest/%s/BeamTest_Calibration.root",mHome.c_str(),mDet.c_str());
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

  initChain(); // initialize input list

  initTdcCut(); // initialize TDC cuts

  initTimeDurationCut(); // initialize Time Duration Cuts

  for(int i_pixel_x = 0; i_pixel_x < mRICH::mNumOfPixels; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < mRICH::mNumOfPixels; ++i_pixel_y)
    {
      string HistName = Form("h_mTDC_pixelX_%d_pixelY_%d",i_pixel_x,i_pixel_y);
      // cout << HistName.c_str() << endl;
      h_mTDC[i_pixel_x][i_pixel_y] = new TH1D(HistName.c_str(),HistName.c_str(),5000,-0.5,4999.5);
      mTimeDuration[i_pixel_x][i_pixel_y][0] = -999.9;
      mTimeDuration[i_pixel_x][i_pixel_y][1] = -999.9;
    }
  }

  h_mRingImage = new TH2D("h_mRingImage","h_mRingImage",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  // h_mRingImage_DisPlay = new TH2D("h_mRingImage_DisPlay","h_mRingImage_DisPlay",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  // h_mRingImage_DisPlay_beam = new TH2D("h_mRingImage_DisPlay_beam","h_mRingImage_DisPlay_beam",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  cout << "Initialized QA histograms. " << endl;

  h_mNumOfPhotons = new TH1D("h_mNumOfPhotons","h_mNumOfPhotons",50,-0.5,49.5);

  p_mNumOfPhotons = new TProfile("p_mNumOfPhotons","p_mNumOfPhotons",1,-0.5,0.5);

  cout << "Initialized TProfile for Number of Photons. " << endl;

  initBeamFinder();
  initRingFinder();

  return 0;
}

int Calibration::initChain()
{
  string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/tdc/",mHome.c_str());
  string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/BeamTest/%s/Calibration/proton_calibration.list",mHome.c_str(),mDet.c_str());

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

int Calibration::initTdcCut()
{
  cout << endl;
  cout << "initialize TDC Cuts!!" << endl;
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

  float NumOfRuns   = 0.0;
  float SumTdcMean  = 0.0;
  float SumTdcSigma = 0.0;
  // float SumTdcStart = 0.0;
  // float SumTdcStop  = 0.0;

  TFile *File_mTDC = TFile::Open(inputfile.c_str());
  TH2D *h_mTimeCuts = (TH2D*)File_mTDC->Get("h_mTimeCuts")->Clone();
  TH1D *h_mTdcMean  = (TH1D*)h_mTimeCuts->ProjectionY("h_mTdcMean",1,1)->Clone();
  TH1D *h_mTdcSigma = (TH1D*)h_mTimeCuts->ProjectionY("h_mTdcSigma",2,2)->Clone();
  TH1D *h_mRunId    = (TH1D*)h_mTimeCuts->ProjectionY("h_mRunId",3,3)->Clone();
  // TH1D *h_mTdcStart = (TH1D*)h_mTimeCuts->ProjectionY("h_mTdcStart",1,1)->Clone();
  // TH1D *h_mTdcStop  = (TH1D*)h_mTimeCuts->ProjectionY("h_mTdcStop",2,2)->Clone();
  for(int i_bin = 1; i_bin < h_mRunId->GetNbinsX(); ++i_bin)
  {
    float runId     = h_mRunId->GetBinContent(i_bin);
    float tdc_mean  = h_mTdcMean->GetBinContent(i_bin);
    float tdc_sigma = h_mTdcSigma->GetBinContent(i_bin);
    // float tdc_start = h_mTdcStart->GetBinContent(i_bin);
    // float tdc_stop  = h_mTdcStop->GetBinContent(i_bin);
    if(runId > 0)
    {
      NumOfRuns++;
      SumTdcMean  += tdc_mean;
      SumTdcSigma += tdc_sigma;
      cout << "runId = " << runId << ", tdc_mean = " << tdc_mean << ", tdc_sigma = " << tdc_sigma << ", NumOfRuns = " << NumOfRuns << endl;
      // SumTdcStart += tdc_start;
      // SumTdcStop  += tdc_stop;
      // cout << "runId = " << runId << ", tdc_start = " << tdc_start << ", tdc_stop = " << tdc_stop << ", NumOfRuns = " << NumOfRuns << endl;
    }
  }
  File_mTDC->Close();

  float nSigma = 3.0;
  float mean_tdc_Start = SumTdcMean/NumOfRuns - nSigma*SumTdcSigma/NumOfRuns; 
  float mean_tdc_Stop  = SumTdcMean/NumOfRuns + nSigma*SumTdcSigma/NumOfRuns;

  mTdc_Start = floor(mean_tdc_Start);
  mTdc_Stop  = ceil(mean_tdc_Stop);
  // mTdc_Start = floor(SumTdcStart/NumOfRuns);
  // mTdc_Stop  = ceil(SumTdcStop/NumOfRuns);

  cout << "tdc_mean = " << SumTdcMean/NumOfRuns << ", tdc_sigma = " << SumTdcSigma/NumOfRuns << ", nSigma = " << nSigma << endl;
  cout << "mean_tdc_Start = " << mean_tdc_Start  << ", mean_tdc_Stop = " << mean_tdc_Stop << endl;
  cout << "mTdc_Start set to " << mTdc_Start << " and mTdc_Stop set to " << mTdc_Stop << endl;

  return 0;
}

int Calibration::initTimeDurationCut()
{
  cout << endl;
  cout << "initialize Time Duration Cuts!!" << endl;
  string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/QA/%s/Calibration/",mHome.c_str(),mDet.c_str());
  string inputfile;
  if( is_pmt )
  {
    inputfile = inputdir + "richTimeDurationCuts.root";
  }
  if( !is_pmt )
  {
    inputfile = inputdir + "sipmTimeDurationCuts.root";
  }

  float NumOfRuns   = 0.0;
  float SumTimeMean  = 0.0;
  float SumTimeSigma = 0.0;

  TFile *File_mTimeDuration = TFile::Open(inputfile.c_str());
  TH2D *h_mTimeDurationCuts = (TH2D*)File_mTimeDuration->Get("h_mTimeDurationCuts")->Clone();
  TH1D *h_mTimeMean  = (TH1D*)h_mTimeDurationCuts->ProjectionY("h_mTimeMean",1,1)->Clone();
  TH1D *h_mTimeSigma = (TH1D*)h_mTimeDurationCuts->ProjectionY("h_mTimeSigma",2,2)->Clone();
  TH1D *h_mRunId    = (TH1D*)h_mTimeDurationCuts->ProjectionY("h_mRunId",3,3)->Clone();
  for(int i_bin = 1; i_bin < h_mRunId->GetNbinsX(); ++i_bin)
  {
    float runId     = h_mRunId->GetBinContent(i_bin);
    float time_mean  = h_mTimeMean->GetBinContent(i_bin);
    float time_sigma = h_mTimeSigma->GetBinContent(i_bin);
    if(runId > 0)
    {
      NumOfRuns++;
      SumTimeMean  += time_mean;
      SumTimeSigma += time_sigma;
      cout << "runId = " << runId << ", time_mean = " << time_mean << ", time_sigma = " << time_sigma << ", NumOfRuns = " << NumOfRuns << endl;
    }
  }
  File_mTimeDuration->Close();

  float nSigma = 3.0;
  float mean_time_Low = SumTimeMean/NumOfRuns - nSigma*SumTimeSigma/NumOfRuns; 
  float mean_time_High = SumTimeMean/NumOfRuns + nSigma*SumTimeSigma/NumOfRuns;

  mTime_Low = floor(mean_time_Low);
  mTime_High = ceil(mean_time_High);

  cout << "time_mean = " << SumTimeMean/NumOfRuns << ", time_sigma = " << SumTimeSigma/NumOfRuns << ", nSigma = " << nSigma << endl;
  cout << "mean_time_Low = " << mean_time_Low << ", mean_time_High = " << mean_time_High << endl;
  cout << "mTime_Low set to " << mTime_Low << " and mTime_High set to " << mTime_High << endl;

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
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    ResetEventData();
    mChainInPut->GetEntry(i_event);

    if(tNedge>MAXEDGE){
      printf("Event to big: %u edges vs %u array size...skip\n",tNedge,MAXEDGE);
      continue;
    }

    //--------------prepare Time Duration--------------------
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

      int polarity = tPolarity[i_photon];
      mTimeDuration[pixel_x][pixel_y][polarity] = tTime[i_photon];

      if(mTimeDuration[pixel_x][pixel_y][polarity] < 0) // cut off second late pulse
      {
	if(polarity == 1)
	{
	  mTimeDuration[pixel_x][pixel_y][polarity] = tTime[i_photon]; // save tdc of falling edge first
	}
	if(polarity == 0 && mTimeDuration[pixel_x][pixel_y][1] > 0)
	{
	  mTimeDuration[pixel_x][pixel_y][polarity] = tTime[i_photon]; // save corresponding tdc of raising edge
	}
      }
    }
    //--------------prepare Time Duration--------------------

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

      float time_duration = mTimeDuration[pixel_x][pixel_y][0] - mTimeDuration[pixel_x][pixel_y][1];
      // if(time_duration > mTime_Low && time_duration < mTime_High)
      if(time_duration > 35)
      {
	if(tPolarity[i_photon] == MAROCPOLARITY && tTime[i_photon] > mTdc_Start && tTime[i_photon] < mTdc_Stop)
	{
	  h_mRingImage->Fill(pixel_x,pixel_y);
	  NumOfPhotons++;
	  /*
	     if(i_event == 1024)
	     {
	     h_mRingImage_DisPlay_beam->Fill(pixel_x,pixel_y);
	     }
	     bool is_beam = (pixel_x > 12 && pixel_x < 20) && (pixel_y > 12 && pixel_y < 20);
	     if( !is_beam ) // exclude beam spot
	     {
	  // if(i_event > 1024 && i_event < 1025)
	  if(i_event == 1024)
	  {
	  h_mRingImage_DisPlay->Fill(pixel_x,pixel_y);
	  }
	  }
	  */

	  // ring finder
	  float out_x = findPixelCoord(pixel_x);
	  float out_y = findPixelCoord(pixel_y);
	  // cout << "out_x = " << out_x << ", pixel_x = " << pixel_x << endl;
	  // cout << "out_y = " << out_y << ", pixel_y = " << pixel_y << endl;
	  h_mRingFinder->Fill(out_x,out_y); // single event distribution => reset for each event
	  h_mRingFinder_Display->Fill(out_x,out_y);
	  if(i_event == 1024) h_mRingFinder_SingleEvent->Fill(out_x,out_y);
	  mXPixelMap.push_back(pixel_x);
	  mYPixelMap.push_back(pixel_y);
	}
      }
    }
    // cout << "NumOfPhotons = " << NumOfPhotons << endl;

    if(NumOfPhotons > 0) // Beam Finder
    {
      int numOfClusters_5by5 = findCluster_5by5(NumOfPhotons,mXPixelMap,mYPixelMap);
      if(numOfClusters_5by5 > 0)
      {
	// cout << "i_event = " << i_event << ", 5by5 numOfClusters = " << numOfClusters_5by5 << endl;
	if(numOfClusters_5by5 == 1) 
	{
	  h_mBeamFinder_5by5_1st->Fill(mMeanX_5by5_1st,mMeanY_5by5_1st);
	  h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_1st,mMeanY_5by5_1st);
	}
	if(numOfClusters_5by5 == 2) 
	{
	  h_mBeamFinder_5by5_2nd->Fill(mMeanX_5by5_2nd,mMeanY_5by5_2nd);
	  h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_2nd,mMeanY_5by5_2nd);
	}
	if(numOfClusters_5by5 == 3) 
	{
	  h_mBeamFinder_5by5_3rd->Fill(mMeanX_5by5_2nd,mMeanY_5by5_2nd);
	  h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_3rd,mMeanY_5by5_3rd);
	}
      }
      if(numOfClusters_5by5 == 0)
      {
	// cout << "i_event = " << i_event << ", 3by3 numOfClusters = " << numOfClusters_3by3 << endl;
	int numOfClusters_3by3 = findCluster_3by3(NumOfPhotons,mXPixelMap,mYPixelMap);
	if(numOfClusters_3by3 == 1) 
	{
	  h_mBeamFinder_3by3_1st->Fill(mMeanX_3by3_1st,mMeanY_3by3_1st);
	  h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_1st,mMeanY_3by3_1st);
	}
	if(numOfClusters_3by3 == 2) 
	{
	  h_mBeamFinder_3by3_2nd->Fill(mMeanX_3by3_2nd,mMeanY_3by3_2nd);
	  h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_2nd,mMeanY_3by3_2nd);
	}
	if(numOfClusters_3by3 == 3) 
	{
	  h_mBeamFinder_3by3_3rd->Fill(mMeanX_3by3_2nd,mMeanY_3by3_2nd);
	  h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_3rd,mMeanY_3by3_3rd);
	}
	if(numOfClusters_3by3 == 0)
	{
	  for(int i_photon = 0; i_photon < NumOfPhotons; ++i_photon)
	  {
	    h_mRingImage_off->Fill(mXPixelMap[i_photon],mYPixelMap[i_photon]);
	  }

	  if(NumOfPhotons > 0) // Ring Finder
	  {
	    h_mNumOfPhotons->Fill(NumOfPhotons);
	    p_mNumOfPhotons->Fill(0.0,NumOfPhotons);

	    // ring finder
	    HoughTransform(NumOfPhotons, h_mRingFinder, mXPixelMap, mYPixelMap);
	  }
	}
      }
    }
    clearRingFinder();

    for(int i_pixel_x = 0; i_pixel_x < mRICH::mNumOfPixels; ++i_pixel_x) // fill time duration
    {
      for(int i_pixel_y = 0; i_pixel_y < mRICH::mNumOfPixels; ++i_pixel_y)
      {
	mTimeDuration[i_pixel_x][i_pixel_y][0] = -999.9; // reset tdc array
	mTimeDuration[i_pixel_x][i_pixel_y][1] = -999.9;
      }
    }
  }
  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 0;
}

int Calibration::Finish()
{
  cout << " this is Calibration::Finish" << endl;
  mFile_OutPut->cd();
  for(int i_pixel_x = 0; i_pixel_x < mRICH::mNumOfPixels; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < mRICH::mNumOfPixels; ++i_pixel_y)
    {
      h_mTDC[i_pixel_x][i_pixel_y]->Write();
    }
  }
  h_mRingImage->Write();
  // h_mRingImage_DisPlay->Write();
  // h_mRingImage_DisPlay_beam->Write();
  h_mNumOfPhotons->Write();
  p_mNumOfPhotons->Write();

  writeBeamFinder();
  writeRingFinder();

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

int Calibration::initRingFinder()
{
  h_mRingFinder = new TH2D("h_mRingFinder","h_mRingFinder",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  // h_mHoughTransform = new TH3D("h_mHoughTransform","h_mHoughTransform",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,1.0*mRICH::mHalfWidth);
  h_mHoughTransform = new TH3D("h_mHoughTransform","h_mHoughTransform",105,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,1.0*mRICH::mHalfWidth);

  clearRingFinder();

  h_mQA_HT = new TH3D("h_mQA_HT","h_mQA_HT",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,2.0*mRICH::mHalfWidth);
  h_mRingFinder_Display = new TH2D("h_mRingFinder_Display","h_mRingFinder_Display",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  h_mRingFinder_SingleEvent = new TH2D("h_mRingFinder_SingleEvent","h_mRingFinder_SingleEvent",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mCherenkovRing = new TH3D("h_mCherenkovRing","h_mCherenkovRing",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,2.0*mRICH::mHalfWidth);
  h_mNumOfCherenkovPhotons = new TH3D("h_mNumOfCherenkovPhotons","h_mNumOfCherenkovPhotons",50,-0.5,49.5,50,-0.5,49.5,210,0,2.0*mRICH::mHalfWidth);
  h_mNumOfPhotons_OnOffRing = new TH2D("h_mNumOfPhotons_OnOffRing","h_mNumOfPhotons_OnOffRing",50,-0.5,49.5,50,-0.5,49.5);

  return 1;
}

int Calibration::clearRingFinder()
{
  h_mRingFinder->Reset();
  mXPixelMap.clear();
  mYPixelMap.clear();

  h_mHoughTransform->Reset();

  return 1;
}

int Calibration::writeRingFinder()
{
  h_mQA_HT->Write();
  h_mRingFinder_Display->Write();
  h_mRingFinder_SingleEvent->Write();
  h_mCherenkovRing->Write();
  h_mNumOfCherenkovPhotons->Write();
  h_mNumOfPhotons_OnOffRing->Write();

  return 1;
}

bool Calibration::findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov)
{
  // check if 3 hit points are at same position or collinear
  if(isSamePosition(firstHit,secondHit,thirdHit) || isCollinear(firstHit,secondHit,thirdHit) ) return false;

  double a = firstHit.X() - secondHit.X(); // a = x1 - x2
  double b = firstHit.Y() - secondHit.Y(); // b = y1 - y2
  double c = firstHit.X() - thirdHit.X();  // c = x1 - x3
  double d = firstHit.Y() - thirdHit.Y();  // d = y1 - y3
  double e = ((firstHit.X()*firstHit.X() - secondHit.X()*secondHit.X()) + (firstHit.Y()*firstHit.Y() - secondHit.Y()*secondHit.Y()))/2.0;  //e = ((x1*x1 - x2*x2) + (y1*y1 - y2*y2))/2.0;
  double f = ((firstHit.X()*firstHit.X() - thirdHit.X()*thirdHit.X()) + (firstHit.Y()*firstHit.Y() - thirdHit.Y()*thirdHit.Y()))/2.0;  //f = ((x1*x1 - x3*x3) + (y1*y1 - y3*y3))/2.0;
  double det = b*c - a*d;

  x_Cherenkov = -(d*e - b*f)/det;
  y_Cherenkov = -(a*f - c*e)/det;
  r_Cherenkov = TMath::Sqrt((x_Cherenkov-firstHit.X())*(x_Cherenkov-firstHit.X())+(y_Cherenkov-firstHit.Y())*(y_Cherenkov-firstHit.Y()));

  return true;
}

bool Calibration::isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5) return true;
  if(TMath::Abs(firstHit.X()-thirdHit.X())  < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y())  < 1e-5) return true;
  if(TMath::Abs(secondHit.X()-thirdHit.X()) < 1e-5 && TMath::Abs(secondHit.Y()-thirdHit.Y()) < 1e-5) return true;
  if(firstHit.Mod() > 1000.0 || secondHit.Mod() > 1000.0 || thirdHit.Mod() > 1000.0) return true; // not proper initialized

  return false;
}

bool Calibration::isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.X()-thirdHit.X()) < 1e-5) return true;
  if(TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y()) < 1e-5) return true;

  double slope12 = (firstHit.Y()-secondHit.Y())/(firstHit.X()-secondHit.X());
  double slope13 = (firstHit.Y()-thirdHit.Y())/(firstHit.X()-thirdHit.X());

  if(TMath::Abs(slope12-slope13) < 1e-5) return true;

  return false;
}

bool Calibration::isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform)
{
  double x_diff = photonHit.X() - x_HoughTransform;
  double y_diff = photonHit.Y() - y_HoughTransform;
  double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r_HoughTransform;

  // double sigma_x = 6.0;
  // double sigma_y = 6.0;
  // double sigma_r = TMath::Sqrt(sigma_x*sigma_x+sigma_y*sigma_y);
  double sigma_r = 6.0;

  if( TMath::Abs(r_diff) < sigma_r) return true;

  return false;
}

int Calibration::HoughTransform(int numOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfPhotons = numOfPhotons;
  if(NumOfPhotons < 3) return 0;
  float NumOfCombinations = TMath::Factorial(NumOfPhotons)/(TMath::Factorial(3)*TMath::Factorial(NumOfPhotons-3));
  // cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfCombinations = " << NumOfCombinations << endl;
  for(int i_hit_1st = 0; i_hit_1st < NumOfPhotons-2; ++i_hit_1st)
  {
    TVector2 firstHit(-999.9,-999.9);
    double x_firstHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_1st]);
    double y_firstHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_1st]);
    firstHit.Set(x_firstHit,y_firstHit);
    for(int i_hit_2nd = i_hit_1st+1; i_hit_2nd < NumOfPhotons-1; ++i_hit_2nd)
    {
      TVector2 secondHit(-999.9,-999.9);
      double x_secondHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_2nd]);
      double y_secondHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_2nd]);
      secondHit.Set(x_secondHit,y_secondHit);
      for(int i_hit_3rd = i_hit_2nd+1; i_hit_3rd < NumOfPhotons; ++i_hit_3rd)
      {
	TVector2 thirdHit(-999.9,-999.9);
	double x_thirdHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit_3rd]);
	double y_thirdHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit_3rd]);
	thirdHit.Set(x_thirdHit,y_thirdHit);

	double x_Cherenkov = -999.9;
	double y_Cherenkov = -999.9;
	double r_Cherenkov = -999.9;

	bool ringStatus = findRing(firstHit,secondHit,thirdHit, x_Cherenkov, y_Cherenkov, r_Cherenkov);
	if(ringStatus) 
	{
	  h_mQA_HT->Fill(x_Cherenkov,y_Cherenkov,r_Cherenkov);
	  h_mHoughTransform->Fill(x_Cherenkov,y_Cherenkov,r_Cherenkov);
	  // cout << "firstHit.x = " << firstHit.X() << ", firstHit.y = " << firstHit.Y() << endl;
	  // cout << "secondHit.x = " << secondHit.X() << ", secondHit.y = " << secondHit.Y() << endl;
	  // cout << "thirdHit.x = " << thirdHit.X() << ", thirdHit.y = " << thirdHit.Y() << endl;
	  // cout << "x_Cherenkov = " << x_Cherenkov << ", y_Cherenkov = " << y_Cherenkov << ", r_Cherenkov = " << r_Cherenkov << endl;
	  // cout << endl;
	}
      }
    }
  }

  int hBin_x = -1;
  int hBin_y = -1;
  int hBin_r = -1;
  int NumOfPhotonsOnRing = 0;

  int globalBin = h_mHoughTransform->GetMaximumBin(hBin_x,hBin_y,hBin_r);
  int maxVote = h_mHoughTransform->GetBinContent(globalBin);
  if(globalBin > 0)
  {
    double x_HoughTransform = h_mHoughTransform->GetXaxis()->GetBinCenter(hBin_x);
    double y_HoughTransform = h_mHoughTransform->GetYaxis()->GetBinCenter(hBin_y);
    double r_HoughTransform = h_mHoughTransform->GetZaxis()->GetBinCenter(hBin_r);
    h_mCherenkovRing->Fill(x_HoughTransform,y_HoughTransform,r_HoughTransform);
    // cout << "hBin_x = " << hBin_x << ", hBin_y = " << hBin_y << ", hBin_r = " << hBin_r << ", globalBin = " << globalBin << ", with maxVote = " << maxVote << endl;
    // cout << "x_HoughTransform = " << x_HoughTransform << ", y_HoughTransform = " << y_HoughTransform << ", r_HoughTransform = " << r_HoughTransform << endl;

    for(int i_hit = 0; i_hit < NumOfPhotons; ++i_hit)
    {
      TVector2 photonHit;
      double x_photonHit = h_RingFinder->GetXaxis()->GetBinCenter(xPixel[i_hit]);
      double y_photonHit = h_RingFinder->GetYaxis()->GetBinCenter(yPixel[i_hit]);
      photonHit.Set(x_photonHit,y_photonHit);
      if( isOnRing(photonHit,x_HoughTransform,y_HoughTransform,r_HoughTransform) )
      {
	NumOfPhotonsOnRing++;
      }
    }
    // cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfPhotonsOnRing = " << NumOfPhotonsOnRing << endl;

    if(NumOfPhotonsOnRing > 4 && TMath::Abs(x_HoughTransform) < 5.5 && TMath::Abs(y_HoughTransform) < 5.5)
    // if(TMath::Abs(x_HoughTransform) < 5.5 && TMath::Abs(y_HoughTransform) < 5.5)
    {
      h_mNumOfCherenkovPhotons->Fill(NumOfPhotons,NumOfPhotonsOnRing,r_HoughTransform);
      h_mNumOfPhotons_OnOffRing->Fill(NumOfPhotonsOnRing,NumOfPhotons-NumOfPhotonsOnRing);
    }
  }

  return 0;
}

float Calibration::findPixelCoord(int pixel)
{
  float out_coord = -999.9;

  if(pixel < 0 || pixel > 32) return -999.9; // out of photon sensor

  float out_low = mRICH::mPixels[pixel];
  float out_high = mRICH::mPixels[pixel+1];

  out_coord = 0.5*(out_low+out_high);

  return out_coord;
}

//--------------------------------------------------------------------

int Calibration::initBeamFinder()
{
  h_mRingImage_on = new TH2D("h_mRingImage_on","h_mRingImage_on",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mRingImage_off = new TH2D("h_mRingImage_off","h_mRingImage_off",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  h_mBeamFinder_5by5_Display = new TH2D("h_mBeamFinder_5by5_Display","h_mBeamFinder_5by5_Display",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_1st = new TH2D("h_mBeamFinder_5by5_1st","h_mBeamFinder_5by5_1st",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_2nd = new TH2D("h_mBeamFinder_5by5_2nd","h_mBeamFinder_5by5_2nd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_3rd = new TH2D("h_mBeamFinder_5by5_3rd","h_mBeamFinder_5by5_3rd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  h_mBeamFinder_3by3_Display = new TH2D("h_mBeamFinder_3by3_Display","h_mBeamFinder_3by3_Display",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_1st = new TH2D("h_mBeamFinder_3by3_1st","h_mBeamFinder_3by3_1st",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_2nd = new TH2D("h_mBeamFinder_3by3_2nd","h_mBeamFinder_3by3_2nd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_3rd = new TH2D("h_mBeamFinder_3by3_3rd","h_mBeamFinder_3by3_3rd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  return 1;
}

int Calibration::clearClusterFinder_5by5()
{
  mXClusterMap_5by5.clear();
  mYClusterMap_5by5.clear();
  mRankMap_5by5.clear();

  mXCluster_5by5_1st.clear();
  mYCluster_5by5_1st.clear();
  mRank_5by5_1st.clear();
  mMeanX_5by5_1st = -1;
  mMeanY_5by5_1st = -1;

  mXCluster_5by5_2nd.clear();
  mYCluster_5by5_2nd.clear();
  mRank_5by5_2nd.clear();
  mMeanX_5by5_2nd = -1;
  mMeanY_5by5_2nd = -1;

  mXCluster_5by5_3rd.clear();
  mYCluster_5by5_3rd.clear();
  mRank_5by5_3rd.clear();
  mMeanX_5by5_3rd = -1;
  mMeanY_5by5_3rd = -1;

  return 1;
}

int Calibration::clearClusterFinder_3by3()
{
  mXClusterMap_3by3.clear();
  mYClusterMap_3by3.clear();
  mRankMap_3by3.clear();

  mXCluster_3by3_1st.clear();
  mYCluster_3by3_1st.clear();
  mRank_3by3_1st.clear();
  mMeanX_3by3_1st = -1;
  mMeanY_3by3_1st = -1;

  mXCluster_3by3_2nd.clear();
  mYCluster_3by3_2nd.clear();
  mRank_3by3_2nd.clear();
  mMeanX_3by3_2nd = -1;
  mMeanY_3by3_2nd = -1;

  mXCluster_3by3_3rd.clear();
  mYCluster_3by3_3rd.clear();
  mRank_3by3_3rd.clear();
  mMeanX_3by3_3rd = -1;
  mMeanY_3by3_3rd = -1;

  return 1;
}

int Calibration::writeBeamFinder()
{
  h_mRingImage_on->Write();
  h_mRingImage_off->Write();


  h_mBeamFinder_5by5_Display->Write();
  h_mBeamFinder_5by5_1st->Write();
  h_mBeamFinder_5by5_2nd->Write();
  h_mBeamFinder_5by5_3rd->Write();

  h_mBeamFinder_3by3_Display->Write();
  h_mBeamFinder_3by3_1st->Write();
  h_mBeamFinder_3by3_2nd->Write();
  h_mBeamFinder_3by3_3rd->Write();

  return 1;
}

int Calibration::findCluster_5by5(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  bool verbosity = false;
  bool verbosity_QA = false;
  int NumOfClusters = 0;
  clearClusterFinder_5by5();

  const int NumOfPhotons = numOfPhotons;
  int beam_counter[NumOfPhotons];

  // beam finder
  for(int i_beam = 0; i_beam < NumOfPhotons; ++i_beam) // first round of cluster
  {
    beam_counter[i_beam] = 0;
    for(int i_cluster = 0; i_cluster < NumOfPhotons; ++i_cluster)
    {
      int delta_x = TMath::Abs(xPixel[i_cluster]-xPixel[i_beam]);
      int delta_y = TMath::Abs(yPixel[i_cluster]-yPixel[i_beam]);
      if(delta_x < 3 && delta_y < 3) // 5*5 pixel cluster
      {
	beam_counter[i_beam]++;
      }
    }
    if(verbosity_QA) cout << "beam_counter = " << beam_counter[i_beam] << endl;
    if(beam_counter[i_beam] > 4)
    {
      mXClusterMap_5by5.push_back(xPixel[i_beam]);
      mYClusterMap_5by5.push_back(yPixel[i_beam]);
      mRankMap_5by5.push_back(beam_counter[i_beam]);
    }
  }

  if(mRankMap_5by5.size() == 0) 
  {
    if(verbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;
    return NumOfClusters;
  }

  std::vector<int>::iterator it_max_1st = max_element(std::begin(mRankMap_5by5),std::end(mRankMap_5by5)); // most ranked pixel
  int cluster_1st = std::distance(mRankMap_5by5.begin(), it_max_1st);
  if(verbosity_QA) std::cout << "max element at: " << cluster_1st << endl;
  if(verbosity_QA) cout << "max rank = " << mRankMap_5by5[cluster_1st] << endl;

  std::vector<int> XCluster_rest;
  std::vector<int> YCluster_rest;
  std::vector<int> Rank_rest;
  XCluster_rest.clear();
  YCluster_rest.clear();
  Rank_rest.clear();

  for(int i_cluster = 0; i_cluster < mRankMap_5by5.size(); ++i_cluster)
  {
    int delta_x = TMath::Abs(mXClusterMap_5by5[i_cluster]-mXClusterMap_5by5[cluster_1st]);
    int delta_y = TMath::Abs(mYClusterMap_5by5[i_cluster]-mYClusterMap_5by5[cluster_1st]);
    if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
    {
      mXCluster_5by5_1st.push_back(mXClusterMap_5by5[i_cluster]);
      mYCluster_5by5_1st.push_back(mYClusterMap_5by5[i_cluster]);
      mRank_5by5_1st.push_back(mRankMap_5by5[i_cluster]);
      if(verbosity_QA) cout << "1st cluster Ranking: " << mRankMap_5by5[i_cluster] << endl;
    }
    else
    {
      XCluster_rest.push_back(mXClusterMap_5by5[i_cluster]);
      YCluster_rest.push_back(mYClusterMap_5by5[i_cluster]);
      Rank_rest.push_back(mRankMap_5by5[i_cluster]);
      // cout << "rest cluster Ranking: " << mRankMap_5by5[i_cluster] << endl;
    }
  }
  if(mRank_5by5_1st.size() > 0) 
  {
    NumOfClusters++;
    std::vector<int>::iterator it_1st = max_element(std::begin(mRank_5by5_1st),std::end(mRank_5by5_1st)); // most ranked pixel
    int beam_1st = std::distance(mRank_5by5_1st.begin(), it_1st);
    mMeanX_5by5_1st = mXCluster_5by5_1st[beam_1st];
    mMeanY_5by5_1st = mYCluster_5by5_1st[beam_1st];
  }

  if(Rank_rest.size() > 0)
  {
    std::vector<int>::iterator it_max_2nd = max_element(std::begin(Rank_rest),std::end(Rank_rest)); // most ranked pixel in 2nd cluster
    int cluster_2nd = std::distance(Rank_rest.begin(), it_max_2nd);
    if(verbosity_QA) std::cout << "max element at: " << cluster_2nd << endl;
    if(verbosity_QA) cout << "max rank = " << Rank_rest[cluster_2nd] << endl;

    for(int i_cluster = 0; i_cluster < Rank_rest.size(); ++i_cluster)
    {
      int delta_x = TMath::Abs(XCluster_rest[i_cluster]-XCluster_rest[cluster_2nd]);
      int delta_y = TMath::Abs(YCluster_rest[i_cluster]-YCluster_rest[cluster_2nd]);
      if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
      {
	mXCluster_5by5_2nd.push_back(XCluster_rest[i_cluster]);
	mYCluster_5by5_2nd.push_back(YCluster_rest[i_cluster]);
	mRank_5by5_2nd.push_back(Rank_rest[i_cluster]);
	if(verbosity_QA) cout << "2nd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
      else
      {
	mXCluster_5by5_3rd.push_back(XCluster_rest[i_cluster]);
	mYCluster_5by5_3rd.push_back(YCluster_rest[i_cluster]);
	mRank_5by5_3rd.push_back(Rank_rest[i_cluster]);
	if(verbosity_QA) cout << "3rd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
    }
    if(mRank_5by5_2nd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_2nd = max_element(std::begin(mRank_5by5_2nd),std::end(mRank_5by5_2nd)); // most ranked pixel
      int beam_2nd = std::distance(mRank_5by5_2nd.begin(), it_2nd);
      mMeanX_5by5_2nd = mXCluster_5by5_2nd[beam_2nd];
      mMeanY_5by5_2nd = mYCluster_5by5_2nd[beam_2nd];
    }

    if(mRank_5by5_3rd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_3rd = max_element(std::begin(mRank_5by5_3rd),std::end(mRank_5by5_3rd)); // most ranked pixel
      int beam_3rd = std::distance(mRank_5by5_3rd.begin(), it_3rd);
      mMeanX_5by5_3rd = mXCluster_5by5_3rd[beam_3rd];
      mMeanY_5by5_3rd = mYCluster_5by5_3rd[beam_3rd];
    }
  }

  if(verbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;

  return NumOfClusters;
}

int Calibration::findCluster_3by3(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  bool verbosity = false;
  bool verbosity_QA = false;
  int NumOfClusters = 0;
  clearClusterFinder_3by3();

  const int NumOfPhotons = numOfPhotons;
  int beam_counter[NumOfPhotons];

  // beam finder
  for(int i_beam = 0; i_beam < NumOfPhotons; ++i_beam) // first round of cluster
  {
    beam_counter[i_beam] = 0;
    for(int i_cluster = 0; i_cluster < NumOfPhotons; ++i_cluster)
    {
      int delta_x = TMath::Abs(xPixel[i_cluster]-xPixel[i_beam]);
      int delta_y = TMath::Abs(yPixel[i_cluster]-yPixel[i_beam]);
      if(delta_x < 2 && delta_y < 2) // 3*3 pixel cluster
      {
	beam_counter[i_beam]++;
      }
    }
    if(verbosity_QA) cout << "beam_counter = " << beam_counter[i_beam] << endl;
    if(beam_counter[i_beam] > 2)
    {
      mXClusterMap_3by3.push_back(xPixel[i_beam]);
      mYClusterMap_3by3.push_back(yPixel[i_beam]);
      mRankMap_3by3.push_back(beam_counter[i_beam]);
    }
  }

  if(mRankMap_3by3.size() == 0) 
  {
    if(verbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;
    return NumOfClusters;
  }

  std::vector<int>::iterator it_max_1st = max_element(std::begin(mRankMap_3by3),std::end(mRankMap_3by3)); // most ranked pixel
  int cluster_1st = std::distance(mRankMap_3by3.begin(), it_max_1st);
  if(verbosity_QA) std::cout << "max element at: " << cluster_1st << endl;
  if(verbosity_QA) cout << "max rank = " << mRankMap_3by3[cluster_1st] << endl;

  std::vector<int> XCluster_rest;
  std::vector<int> YCluster_rest;
  std::vector<int> Rank_rest;
  XCluster_rest.clear();
  YCluster_rest.clear();
  Rank_rest.clear();

  for(int i_cluster = 0; i_cluster < mRankMap_3by3.size(); ++i_cluster)
  {
    int delta_x = TMath::Abs(mXClusterMap_3by3[i_cluster]-mXClusterMap_3by3[cluster_1st]);
    int delta_y = TMath::Abs(mYClusterMap_3by3[i_cluster]-mYClusterMap_3by3[cluster_1st]);
    if(delta_x < 4 && delta_y < 4) // 2 adjacent 3*3 pixel cluster => 1 cluster
    {
      mXCluster_3by3_1st.push_back(mXClusterMap_3by3[i_cluster]);
      mYCluster_3by3_1st.push_back(mYClusterMap_3by3[i_cluster]);
      mRank_3by3_1st.push_back(mRankMap_3by3[i_cluster]);
      if(verbosity_QA) cout << "1st cluster Ranking: " << mRankMap_3by3[i_cluster] << endl;
    }
    else
    {
      XCluster_rest.push_back(mXClusterMap_3by3[i_cluster]);
      YCluster_rest.push_back(mYClusterMap_3by3[i_cluster]);
      Rank_rest.push_back(mRankMap_3by3[i_cluster]);
      // cout << "rest cluster Ranking: " << mRankMap_3by3[i_cluster] << endl;
    }
  }
  if(mRank_3by3_1st.size() > 0) 
  {
    NumOfClusters++;
    std::vector<int>::iterator it_1st = max_element(std::begin(mRank_3by3_1st),std::end(mRank_3by3_1st)); // most ranked pixel
    int beam_1st = std::distance(mRank_3by3_1st.begin(), it_1st);
    mMeanX_3by3_1st = mXCluster_3by3_1st[beam_1st];
    mMeanY_3by3_1st = mYCluster_3by3_1st[beam_1st];
  }

  if(Rank_rest.size() > 0)
  {
    std::vector<int>::iterator it_max_2nd = max_element(std::begin(Rank_rest),std::end(Rank_rest)); // most ranked pixel in 2nd cluster
    int cluster_2nd = std::distance(Rank_rest.begin(), it_max_2nd);
    if(verbosity_QA) std::cout << "max element at: " << cluster_2nd << endl;
    if(verbosity_QA) cout << "max rank = " << Rank_rest[cluster_2nd] << endl;

    for(int i_cluster = 0; i_cluster < Rank_rest.size(); ++i_cluster)
    {
      int delta_x = TMath::Abs(XCluster_rest[i_cluster]-XCluster_rest[cluster_2nd]);
      int delta_y = TMath::Abs(YCluster_rest[i_cluster]-YCluster_rest[cluster_2nd]);
      if(delta_x < 4 && delta_y < 4) // 2 adjacent 5*5 pixel cluster => 1 cluster
      {
	mXCluster_3by3_2nd.push_back(XCluster_rest[i_cluster]);
	mYCluster_3by3_2nd.push_back(YCluster_rest[i_cluster]);
	mRank_3by3_2nd.push_back(Rank_rest[i_cluster]);
	if(verbosity_QA) cout << "2nd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
      else
      {
	mXCluster_3by3_3rd.push_back(XCluster_rest[i_cluster]);
	mYCluster_3by3_3rd.push_back(YCluster_rest[i_cluster]);
	mRank_3by3_3rd.push_back(Rank_rest[i_cluster]);
	if(verbosity_QA) cout << "3rd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
    }
    if(mRank_3by3_2nd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_2nd = max_element(std::begin(mRank_3by3_2nd),std::end(mRank_3by3_2nd)); // most ranked pixel
      int beam_2nd = std::distance(mRank_3by3_2nd.begin(), it_2nd);
      mMeanX_3by3_2nd = mXCluster_3by3_2nd[beam_2nd];
      mMeanY_3by3_2nd = mYCluster_3by3_2nd[beam_2nd];
    }

    if(mRank_3by3_3rd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_3rd = max_element(std::begin(mRank_3by3_3rd),std::end(mRank_3by3_3rd)); // most ranked pixel
      int beam_3rd = std::distance(mRank_3by3_3rd.begin(), it_3rd);
      mMeanX_3by3_3rd = mXCluster_3by3_3rd[beam_3rd];
      mMeanY_3by3_3rd = mYCluster_3by3_3rd[beam_3rd];
    }
  }

  if(verbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;

  return NumOfClusters;
}
