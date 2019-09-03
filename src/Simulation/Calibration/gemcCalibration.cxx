#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <cmath> 
#include <math.h> 

#include <TFile.h>
#include <TF1.h>
#include <TMath.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>
#include <TString.h>
#include <Fit/Fitter.h>
#include <Math/Functor.h>

#include "./gemcCalibration.h"
#include "../Material/Material.h"
#include "../Material/mRICH.h"
#include "../RingFinder/RingFinder.h"

using namespace std;

ClassImp(gemcCalibration)

gemcCalibration::gemcCalibration() : mDet("PMT"), is_pmt(true), mAirGap("3mm")
{
  cout<<endl;
  cout<<"gemcCalibration::gemcCalibration() ----- Constructor ! ------"<<endl;
  cout<<endl;
  mMat = new Material(); // initialize the material
  gRandom->SetSeed();
  mHome = getenv("HOME");
}

gemcCalibration::~gemcCalibration()
{
  cout<<"gemcCalibration::~gemcCalibration() ----- Release memory ! ------"<<endl;
  delete mMat;
  delete File_mOutPut;
}

int gemcCalibration::Init()
{
  cout<<"gemcCalibration::Init() ----- Initialization ! ------"<<endl;

  mOutPutFile = Form("%s/WorkSpace/EICPID/OutPut/Simulation/%s/GEMC_Calibration_%s.root",mHome.c_str(),mDet.c_str(),mAirGap.c_str());
  cout<<"gemcCalibration::Init(), create output file: "<< mOutPutFile.c_str() <<endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  initChain();

  // Simple TTree for Marco
  resetSimpleTree();
  initSimpleTree();

  // ring finder
  mRingFinder = new RingFinder();
  mRingFinder->initRingFinder_HT(); // initialize Hough Transform
  mRingFinder->initRingFinder_MF(); // initialize Minuit Fit

  initRingImage();

  return 0;
}

int gemcCalibration::initChain()
{
  string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/gemc/Calibration/%s/",mHome.c_str(),mAirGap.c_str());
  string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/Simulation/%s/Calibration/proton_calibration.list",mHome.c_str(),mDet.c_str());
  // string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/gemc/AngleRun/",mHome.c_str());
  // string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/Simulation/%s/AngleRun/proton_anglerun.list",mHome.c_str(),mDet.c_str());
  
  mChainInPut_Events = new TChain("generated");
  mChainInPut_Tracks = new TChain("eic_rich");

  if (!InPutList.empty())   // if input file is ok
  {
    cout << "Open input database file list: " << InPutList.c_str() << endl;
    ifstream in(InPutList.c_str());  // input stream
    if(in)
    {
      cout << "input database file list is ok" << endl;
      char str[255];       // char array for each file name
      Long64_t entries_save = 0;
      while(in)
      {
	in.getline(str,255);  // take the lines of the file list
	if(str[0] != 0)
	{
	  string addfile;
	  addfile = str;
	  addfile = inputdir+addfile;
	  mChainInPut_Events->AddFile(addfile.c_str(),-1,"generated");
	  mChainInPut_Tracks->AddFile(addfile.c_str(),-1,"eic_rich");
	  long file_entries = mChainInPut_Events->GetEntries();
	  cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	  entries_save = file_entries;
	}
      }
    }
    else
    {
      cout << "WARNING: input database file input is problemtic" << endl;
    }
  }

  // set branch for generated event
  mChainInPut_Events->SetBranchAddress("pid", &gen_pid);
  mChainInPut_Events->SetBranchAddress("px",  &gen_px);
  mChainInPut_Events->SetBranchAddress("py",  &gen_py);
  mChainInPut_Events->SetBranchAddress("pz",  &gen_pz);
  mChainInPut_Events->SetBranchAddress("vx",  &gen_vx);
  mChainInPut_Events->SetBranchAddress("vy",  &gen_vy);
  mChainInPut_Events->SetBranchAddress("vz",  &gen_vz);

  // set branch for simulated tracks
  mChainInPut_Tracks->SetBranchAddress("id",      &trk_id);
  mChainInPut_Tracks->SetBranchAddress("hitn",    &trk_hitn);
  mChainInPut_Tracks->SetBranchAddress("pid",     &trk_pid);
  mChainInPut_Tracks->SetBranchAddress("mpid",    &trk_mpid);
  mChainInPut_Tracks->SetBranchAddress("tid",     &trk_tid);
  mChainInPut_Tracks->SetBranchAddress("mtid",    &trk_mtid);
  mChainInPut_Tracks->SetBranchAddress("otid",    &trk_otid);
  mChainInPut_Tracks->SetBranchAddress("trackE",  &trk_trackE);
  mChainInPut_Tracks->SetBranchAddress("totEdep", &trk_totEdep);
  mChainInPut_Tracks->SetBranchAddress("avg_x",   &trk_avg_x);
  mChainInPut_Tracks->SetBranchAddress("avg_y",   &trk_avg_y);
  mChainInPut_Tracks->SetBranchAddress("avg_z",   &trk_avg_z);
  mChainInPut_Tracks->SetBranchAddress("avg_lx",  &trk_avg_lx);
  mChainInPut_Tracks->SetBranchAddress("avg_ly",  &trk_avg_ly);
  mChainInPut_Tracks->SetBranchAddress("avg_lz",  &trk_avg_lz);
  mChainInPut_Tracks->SetBranchAddress("px",      &trk_px);
  mChainInPut_Tracks->SetBranchAddress("py",      &trk_py);
  mChainInPut_Tracks->SetBranchAddress("pz",      &trk_pz);
  mChainInPut_Tracks->SetBranchAddress("vx",      &trk_vx);
  mChainInPut_Tracks->SetBranchAddress("vy",      &trk_vy);
  mChainInPut_Tracks->SetBranchAddress("vz",      &trk_vz);
  mChainInPut_Tracks->SetBranchAddress("mvx",     &trk_mvx);
  mChainInPut_Tracks->SetBranchAddress("mvy",     &trk_mvy);
  mChainInPut_Tracks->SetBranchAddress("mvz",     &trk_mvz);
  mChainInPut_Tracks->SetBranchAddress("avg_t",   &trk_avg_t);
  mChainInPut_Tracks->SetBranchAddress("in_x",    &trk_in_x);
  mChainInPut_Tracks->SetBranchAddress("in_y",    &trk_in_y);
  mChainInPut_Tracks->SetBranchAddress("in_z",    &trk_in_z);
  mChainInPut_Tracks->SetBranchAddress("in_px",   &trk_in_px);
  mChainInPut_Tracks->SetBranchAddress("in_py",   &trk_in_py);
  mChainInPut_Tracks->SetBranchAddress("in_pz",   &trk_in_pz);
  mChainInPut_Tracks->SetBranchAddress("in_t",    &trk_in_t);
  mChainInPut_Tracks->SetBranchAddress("out_x",   &trk_out_x);
  mChainInPut_Tracks->SetBranchAddress("out_y",   &trk_out_y);
  mChainInPut_Tracks->SetBranchAddress("out_z",   &trk_out_z);
  mChainInPut_Tracks->SetBranchAddress("out_px",  &trk_out_px);
  mChainInPut_Tracks->SetBranchAddress("out_py",  &trk_out_py);
  mChainInPut_Tracks->SetBranchAddress("out_pz",  &trk_out_pz);
  mChainInPut_Tracks->SetBranchAddress("out_t",   &trk_out_t);

  long NumOfEvents = (long)mChainInPut_Events->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;

  return 0;
}

int gemcCalibration::initSimpleTree()
{
  mSimpleTree = new TTree("GEMC","Simple TTree from GEMC");
  mSimpleTree->Branch("numofphotons",&mNumOfPhotons,"numofphotons/I");
  mSimpleTree->Branch("x",mX_Sensor,"x[numofphotons]/F");
  mSimpleTree->Branch("y",mY_Sensor,"y[numofphotons]/F");
  mSimpleTree->Branch("wavelength",mWaveLength,"wavelength[numofphotons]/F");
  mSimpleTree->Branch("pid",mPid,"pid[numofphotons]/I");

  return 1;
}

int gemcCalibration::resetSimpleTree()
{
  mNumOfPhotons = -1;
  for(int i_photons = 0; i_photons < 10000; ++i_photons)
  {
    mPid[i_photons] = -1;
    mWaveLength[i_photons] = -1;
    mX_Sensor[i_photons] = -1;
    mY_Sensor[i_photons] = -1;
  }

  return 1;
}

int gemcCalibration::Make()
{
  // long NumOfEvents = (long)mChainInPut_Events->GetEntries();
  long NumOfEvents = 50000;

  mChainInPut_Events->GetEntry(0);
  mChainInPut_Tracks->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event) // event loop
  { 
    if(i_event%100==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;

    mChainInPut_Events->GetEntry(i_event);  
    mChainInPut_Tracks->GetEntry(i_event);

    const int pid_mom = gen_pid->at(0);
    const double px_mom = gen_px->at(0)/1e3;    //in MeV, convert to GeV
    const double py_mom = gen_py->at(0)/1e3;    //in MeV, convert to GeV
    const double pz_mom = gen_pz->at(0)/1e3;    //in MeV, convert to GeV
    const double vx_mom = gen_vx->at(0);        //in mm
    const double vy_mom = gen_vy->at(0);        //in mm
    const double vz_mom = gen_vz->at(0);        //in mm

    const double momentum = TMath::Sqrt(px_mom*px_mom+py_mom*py_mom+pz_mom*pz_mom);
    // cout << "pid_mom = " << pid_mom << ", pz_mom = " << pz_mom << endl;

    h_mNumOfEvents->Fill(0);

    int NumOfTracks = trk_hitn->size();
    // cout << "NumOfTracks = " << hitn->size() << endl;

    int NumOfPhotons = 0;
    for (int i_track = 0; i_track < NumOfTracks; ++i_track) // track loop
    {
      int pid = trk_pid->at(i_track);
      double pz_out = trk_out_pz->at(i_track);
      int detector_id = trk_id->at(i_track);

      if(isPhoton(pid,i_track) && !isReflection(pz_out,i_track) && isOnPhotonSensor(detector_id,i_track))
      {
	double out_x_generated = trk_out_x->at(i_track);
	double out_y_generated = trk_out_y->at(i_track);
	h_mPhotonGenerated->Fill(out_x_generated,out_y_generated);

	double photonE = trk_trackE->at(i_track);   /// in MeV (GEANT4 default)
	double wavelength = 1240./(photonE*1.e6);  /// MeV->eV,wavelength in "nm"
	h_mWaveLength->Fill(wavelength);

	double quantumEff = 0.0;
	if(is_pmt) quantumEff = mMat->extrapQE_PMT(wavelength); // extract quantum efficiency for PMT
	if(!is_pmt) quantumEff = mMat->extrapQE_MPPC(wavelength); // extrat quantum efficiency for MPPC

	if( quantumEff > gRandom->Uniform(0.0,1.0) )
	{
	  double out_x_input = trk_out_x->at(i_track);
	  double out_y_input = trk_out_y->at(i_track);
	  mX_Sensor[NumOfPhotons] = out_x_input;
	  mY_Sensor[NumOfPhotons] = out_y_input;
	  mWaveLength[NumOfPhotons] = wavelength;
	  mPid[NumOfPhotons] = pid;

	  double delta_x = 0.0;
	  double delta_y = 0.0;

	  double out_x = out_x_input+delta_x;
	  double out_y = out_y_input+delta_y;
	  if( isInSensorPlane(out_x,out_y) )
	  {
	    h_mPhotonDist->Fill(out_x,out_y);
	    NumOfPhotons++;

	    if(i_event == 1024) h_mPhotonDist_SingleEvent->Fill(out_x,out_y);

	    // ring finder
	    h_mRingFinder->Fill(out_x,out_y); // single event distribution => reset for each event
	    int binX = h_mRingFinder->GetXaxis()->FindBin(out_x);
	    int binY = h_mRingFinder->GetYaxis()->FindBin(out_y);
	    mXPixelMap.push_back(binX);
	    mYPixelMap.push_back(binY);
	    h_mRingImage->Fill(binX-1,binY-1); // ring image in pixel plane
	  }
	}
      }
    }

    // Simple TTree for Marco
    mNumOfPhotons = NumOfPhotons;
    mSimpleTree->Fill();
    resetSimpleTree();

    if(NumOfPhotons > 0) // Ring Finder
    {
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

      if(mRingFinder->getNumOfPhotonsOnRing_HT() > 4 && TMath::Abs(RingCenter_HT.X()) < 5.5 && TMath::Abs(RingCenter_HT.Y()) < 5.5)
      {
	h_mNumOfPhotons->Fill(mRingFinder->getNumOfPhotonsOnRing_HT());
	p_mNumOfPhotons->Fill(0.0,mRingFinder->getNumOfPhotonsOnRing_HT());
      }
    }
    mRingFinder->clearRingFinder_HT();
    mRingFinder->clearRingFinder_MF();
    clearRingImage();
  }
  cout << "processed events:  " << NumOfEvents << "/" << NumOfEvents << endl;

  return 0;
}

int gemcCalibration::Finish()
{
  cout<<endl;
  cout<<"gemcCalibration::Finish() ----- Write out tree and histogram to files !------"<<endl;
  cout<<"This is the end of this program !"<<endl;
  if(File_mOutPut != NULL)
  {
    File_mOutPut->cd();
    writeRingImage();
    mRingFinder->writeRingFinder_HT();
    mRingFinder->writeRingFinder_MF();
    writeSimpleTree();
    File_mOutPut->Close();
  }
  return 0;
}

int gemcCalibration::writeSimpleTree()
{
  mSimpleTree->Write();

  return 0;
}

bool gemcCalibration::isPhoton(int pid, int i_track)
{
  if(pid == 0) return true;
  else return false;
}

bool gemcCalibration::isReflection(double pz_out, int i_track)
{
  if(pz_out < 0.0) return true;
  else return false;
}

bool gemcCalibration::isOnAerogel(int detector_id, int i_track)
{
  if(detector_id == 1) return true;
  else return false;
}

bool gemcCalibration::isOnPhotonSensor(int detector_id, int i_track)
{
  if(detector_id == 2) return true;
  else return false;
}

bool gemcCalibration::isInSensorPlane(double out_x, double out_y)
{
  if( !(TMath::Abs(out_x) > 3.65 && TMath::Abs(out_x) < 52.15) ) return false;
  if( !(TMath::Abs(out_y) > 3.65 && TMath::Abs(out_y) < 52.15) ) return false;
  return true;
}

//-----------------------------------------------------------------

int gemcCalibration::initRingImage()
{
  cout<<"gemcCalibration::initRingImage(), initialize database histograms ;"<<endl;

  h_mRingFinder = new TH2D("h_mRingFinder","h_mRingFinder",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mRingImage = new TH2D("h_mRingImage","h_mRingImage",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mPhotonGenerated = new TH2D("h_mPhotonGenerated","h_mPhotonGenerated",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  h_mPhotonDist = new TH2D("h_mPhotonDist","h_mPhotonDist",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  h_mPhotonDist_SingleEvent = new TH2D("h_mPhotonDist_SingleEvent","h_mPhotonDist_SingleEvent",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mNumOfEvents = new TH1D("h_mNumOfEvents","h_mNumOfEvents",1,-0.5,0.5);
  h_mNumOfPhotons = new TH1D("h_mNumOfPhotons","h_mNumOfPhotons",50,-0.5,49.5);
  p_mNumOfPhotons = new TProfile("p_mNumOfPhotons","p_mNumOfPhotons",1,-0.5,0.5);
  h_mWaveLength = new TH1D("h_mWaveLength","h_mWaveLength",5000,-0.5,4999.5);

  clearRingImage();

  return 1;
}

int gemcCalibration::clearRingImage()
{
  h_mRingFinder->Reset();
  mXPixelMap.clear();
  mYPixelMap.clear();

  return 1;
}

int gemcCalibration::writeRingImage()
{
  h_mRingImage->Write();

  h_mPhotonGenerated->Write();
  h_mPhotonDist->Write();
  h_mPhotonDist_SingleEvent->Write();

  h_mNumOfEvents->Write();
  h_mNumOfPhotons->Write();
  p_mNumOfPhotons->Write();
  h_mWaveLength->Write();

  return 1;
}
