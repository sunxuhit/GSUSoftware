#include <iostream>
#include <fstream>
#include <cmath>
#include <TMath.h>

#include "./Calibration.h"
#include "../PixelMap/PixelMap.h"
#include "../BeamFinder/BeamFinder.h"
#include "../RingFinder/RingFinder.h"

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
  delete mBeamFinder;
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

  mBeamFinder = new BeamFinder();
  mBeamFinder->initBeamFinder();

  mRingFinder = new RingFinder();
  mRingFinder->initRingFinder_HT(); // initialize Hough Transform
  mRingFinder->initRingFinder_MF(); // initialize Minuit Fit

  initRingImage();

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

  long NumOfEvents = (long)mChainInPut->GetEntries();
  // long NumOfEvents = 100000;
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

	  // ring finder
	  float out_x = mRingFinder->findPixelCoord(pixel_x);
	  float out_y = mRingFinder->findPixelCoord(pixel_y);
	  // cout << "out_x = " << out_x << ", pixel_x = " << pixel_x << endl;
	  // cout << "out_y = " << out_y << ", pixel_y = " << pixel_y << endl;
	  h_mRingFinder_Display->Fill(out_x,out_y);
	  if(i_event == 1024) h_mRingFinder_SingleEvent->Fill(out_x,out_y);

	  // preparation for BeamFinder and RingFinder
	  h_mRingFinder->Fill(out_x,out_y); // single event distribution => reset for each event
	  mXPixelMap.push_back(pixel_x+1); // convert pixel id to histogram bin number
	  mYPixelMap.push_back(pixel_y+1);
	}
      }
    }
    // cout << "NumOfPhotons = " << NumOfPhotons << endl;

    if(NumOfPhotons > 0) // Beam Finder
    {
      int numOfClusters_5by5 = mBeamFinder->findCluster_5by5(NumOfPhotons,mXPixelMap,mYPixelMap);
      if(numOfClusters_5by5 > 0)
      {
	// cout << "i_event = " << i_event << ", 5by5 numOfClusters = " << numOfClusters_5by5 << endl;
	mBeamFinder->fillBeamFinder_5by5();
	mBeamFinder->fillBeamOn(NumOfPhotons,mXPixelMap,mYPixelMap);
      }
      if(numOfClusters_5by5 == 0)
      {
	// cout << "i_event = " << i_event << ", 3by3 numOfClusters = " << numOfClusters_3by3 << endl;
	int numOfClusters_3by3 = mBeamFinder->findCluster_3by3(NumOfPhotons,mXPixelMap,mYPixelMap);
	if(numOfClusters_3by3 > 0)
	{
	  mBeamFinder->fillBeamFinder_3by3();
	  mBeamFinder->fillBeamOn(NumOfPhotons,mXPixelMap,mYPixelMap);
	}
	if(numOfClusters_3by3 == 0)
	{
	  mBeamFinder->fillBeamOff(NumOfPhotons,mXPixelMap,mYPixelMap);

	  // Ring Finder
	  // Hough Transform
	  mRingFinder->HoughTransform(NumOfPhotons, h_mRingFinder, mXPixelMap, mYPixelMap);
	  TVector2 RingCenter_HT = mRingFinder->getRingCenter_HT();
	  double RingRadius_HT = mRingFinder->getRingRadius_HT();
	  // cout << "Ring Info from HoughTransform: X = " << RingCenter_HT.X() << ", Y = " << RingCenter_HT.Y() << ", R = " << RingRadius_HT << endl;

	  // Minuit Fit
	  mRingFinder->MinuitFit(NumOfPhotons, h_mRingFinder, mXPixelMap, mYPixelMap);
	  TVector2 RingCenter_MF = mRingFinder->getRingCenter_MF();
	  double RingRadius_MF = mRingFinder->getRingRadius_MF();
	  // cout << "Ring Info from MinuitFit: X = " << RingCenter_MF.X() << ", Y = " << RingCenter_MF.Y() << ", R = " << RingRadius_MF << endl;

	  if(mRingFinder->getNumOfPhotonsOnRing_MF() > 4 && TMath::Abs(RingCenter_MF.X()) < 3.0 && TMath::Abs(RingCenter_MF.Y()) < 3.0)
	  {
	    h_mNumOfPhotons->Fill(mRingFinder->getNumOfPhotonsOnRing_MF());
	    p_mNumOfPhotons->Fill(0.0,mRingFinder->getNumOfPhotonsOnRing_MF());

	    for(int i_photon = 0; i_photon < NumOfPhotons; ++i_photon) // fit single photon radiusa distribution
	    {
	      double x_photonHit = h_mRingFinder->GetXaxis()->GetBinCenter(mXPixelMap[i_photon]);
	      double y_photonHit = h_mRingFinder->GetYaxis()->GetBinCenter(mYPixelMap[i_photon]);
	      double x_MinuitFit = RingCenter_MF.X();
	      double y_MinuitFit = RingCenter_MF.Y();
	      double r_MinuitFit = RingRadius_MF;

	      double x_diff = x_photonHit - x_MinuitFit;
	      double y_diff = y_photonHit - y_MinuitFit;
	      double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r_MinuitFit;

	      double sigma_r = 3.0*2; // 2-pixel

	      if( TMath::Abs(r_diff) < sigma_r)
	      {
		double flength = 6.0*25.4; // mm
		double nref = 1.03;

		double r_singlephoton = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff);

		float dist_r = TMath::Sqrt(r_singlephoton*r_singlephoton+flength*flength);
		float sin_theta_air = r_singlephoton/dist_r; // sin(theta_c)*n_c = sin(theta_air)*n_air
		float theta_air = TMath::ASin(sin_theta_air);

		float sin_theta_c = r_singlephoton/(dist_r*nref); // sin(theta_c)*n_c = sin(theta_air)*n_air
		float theta_c = TMath::ASin(sin_theta_c);

		h_mSinglePhoton->Fill(theta_air, theta_c, r_singlephoton);
	      }
	    }
	  }
	  h_mBeamSpotReco->Fill(RingCenter_MF.X(),RingCenter_MF.Y());
	}
      }
    }
    mRingFinder->clearRingFinder_HT();
    mRingFinder->clearRingFinder_MF();
    clearRingImage();

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
  /*
  for(int i_pixel_x = 0; i_pixel_x < mRICH::mNumOfPixels; ++i_pixel_x)
  {
    for(int i_pixel_y = 0; i_pixel_y < mRICH::mNumOfPixels; ++i_pixel_y)
    {
      h_mTDC[i_pixel_x][i_pixel_y]->Write();
    }
  }
  */
  mBeamFinder->writeBeamFinder();
  mRingFinder->writeRingFinder_HT();
  mRingFinder->writeRingFinder_MF();

  writeRingImage();

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

int Calibration::initRingImage()
{
  cout << "Calibration::initRingImage!!" << endl;

  h_mRingFinder = new TH2D("h_mRingFinder","h_mRingFinder",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mRingImage = new TH2D("h_mRingImage","h_mRingImage",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mRingFinder_Display = new TH2D("h_mRingFinder_Display","h_mRingFinder_Display",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  h_mRingFinder_SingleEvent = new TH2D("h_mRingFinder_SingleEvent","h_mRingFinder_SingleEvent",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mNumOfPhotons = new TH1D("h_mNumOfPhotons","h_mNumOfPhotons",50,-0.5,49.5);
  p_mNumOfPhotons = new TProfile("p_mNumOfPhotons","p_mNumOfPhotons",1,-0.5,0.5);

  h_mBeamSpotReco = new TH2D("h_mBeamSpotReco","h_mBeamSpotReco",201,-10.05,10.05,201,-10.05,10.05);
  h_mSinglePhoton = new TH3D("h_mSinglePhoton","h_mSinglePhoton",150,0.10,0.40,150,0.10,0.40,150,29.5,59.5);


  clearRingImage();

  return 1;
}

int Calibration::clearRingImage()
{
  h_mRingFinder->Reset();
  mXPixelMap.clear();
  mYPixelMap.clear();

  return 1;
}

int Calibration::writeRingImage()
{
  h_mRingImage->Write();
  h_mRingFinder_Display->Write();
  h_mRingFinder_SingleEvent->Write();

  h_mNumOfPhotons->Write();
  p_mNumOfPhotons->Write();
  h_mBeamSpotReco->Write();
  h_mSinglePhoton->Write();

  return 1;
}

//--------------------------------------------------------------------
