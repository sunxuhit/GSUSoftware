#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <cmath> 
#include <math.h> 
#include "TFile.h"
#include "TF1.h"
#include "TMath.h"
#include "TTree.h"
#include "TChain.h"
#include "TRandom3.h"
#include "TString.h"
#include "./gemcCalibration.h"
#include "../Material/Material.h"
#include "../Material/mRICH.h"

using namespace std;

ClassImp(gemcCalibration)

gemcCalibration::gemcCalibration() : mDet("PMT"), is_pmt(true)
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

  mOutPutFile = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/OutPut/Simulation/%s/GEMC_Calibration.root",mHome.c_str(),mDet.c_str());
  cout<<"gemcCalibration::Init(), create output file: "<< mOutPutFile.c_str() <<endl;
  File_mOutPut = new TFile(mOutPutFile.c_str(),"RECREATE");

  initChain();
  initHistograms();
  initGausSmearing();
  initRingFinder();

  return 0;
}

int gemcCalibration::initChain()
{
  string inputdir = Form("%s/WorkSpace/EICPID/Data/BeamTest_mRICH/gemc/Calibration/",mHome.c_str());
  string InPutList = Form("%s/WorkSpace/EICPID/BeamTest_mRICH/list/Simulation/%s/Calibration/proton_calibration.list",mHome.c_str(),mDet.c_str());
  
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

int gemcCalibration::initHistograms()
{
  cout<<"gemcCalibration::initHistograms(), initialize database histograms ;"<<endl;

  h_mNumOfEvents = new TH1D("h_mNumOfEvents","h_mNumOfEvents",1,-0.5,0.5);

  h_mPhotonDist = new TH2D("h_mPhotonDist","h_mPhotonDist",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mPhotonGenerated = new TH2D("h_mPhotonGenerated","h_mPhotonGenerated",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);

  h_mWaveLength = new TH1D("h_mWaveLength","h_mWaveLength",5000,-0.5,4999.5);

  h_mNumOfPhotonsDist = new TH1D("h_mNumOfPhotonsDist","h_mNumOfPhotonsDist",50,-0.5,49.5);

  p_mNumOfPhotons = new TProfile("p_mNumOfPhotons","p_mNumOfPhotons",1,-0.5,0.5);

  return 0;
}

int gemcCalibration::initGausSmearing()
{
  f_mGaus = new TF1("f_mGaus","gaus",-20.0,20.0);
  f_mGaus->SetParameter(0,1.0);
  f_mGaus->SetParameter(1,0.0);
  f_mGaus->SetParameter(2,1.0);

  h_mXGausSmearing = new TH2D("h_mXGausSmearing","h_mXGausSmearing",121,-60.5,60.5,121,-60.5,60.5);
  h_mYGausSmearing = new TH2D("h_mYGausSmearing","h_mYGausSmearing",121,-60.5,60.5,121,-60.5,60.5);

  return 0;
}

int gemcCalibration::Make()
{
  long NumOfEvents = (long)mChainInPut_Events->GetEntries();

  mChainInPut_Events->GetEntry(0);
  mChainInPut_Tracks->GetEntry(0);

  for(int i_event = 0; i_event < 1024; ++i_event) // test event loop
    // for(int i_event = 0; i_event < NumOfEvents; ++i_event) // event loop
  { 
    if(i_event%1000==0) cout << "processing event:  " << i_event << " ;"<<endl;

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
	  double delta_x = GausSmearing(f_mGaus);
	  double delta_y = GausSmearing(f_mGaus);

	  double out_x = out_x_input+delta_x;
	  double out_y = out_y_input+delta_y;
	  if( isInSensorPlane(out_x,out_y) )
	  {
	    h_mPhotonDist->Fill(out_x,out_y);
	    NumOfPhotons++;

	    // ring finder
	    h_mNumOfPhotons->Fill(0);
	    h_mRingFinder->Fill(out_x,out_y); // single event distribution => reset for each event
	    int binX = h_mRingFinder->GetXaxis()->FindBin(out_x);
	    int binY = h_mRingFinder->GetYaxis()->FindBin(out_y);
	    mXPixelMap.push_back(binX);
	    mYPixelMap.push_back(binY);
	  }
	  h_mXGausSmearing->Fill(out_x_input,out_x);
	  h_mYGausSmearing->Fill(out_y_input,out_y);
	}
      }
    }

    // Number of Photons
    h_mNumOfPhotonsDist->Fill(NumOfPhotons);
    p_mNumOfPhotons->Fill(0.0,NumOfPhotons);

    // ring finder
    HoughTransform(h_mNumOfPhotons,h_mRingFinder, mXPixelMap, mYPixelMap);
    clearRingFinder();
  }

  return 0;
}

int gemcCalibration::Finish()
{
  cout<<endl;
  cout<<"gemcCalibration::Finish() ----- Write out tree and histogram to files !------"<<endl;
  cout<<"This is the end of this program !"<<endl;
  if(File_mOutPut != NULL){
    File_mOutPut->cd();
    writeHistograms();
    writeGausSmearing();
    writeRingFinder();
    File_mOutPut->Close();
  }
  return 0;
}

int gemcCalibration::writeHistograms()
{
  h_mNumOfEvents->Write();
  h_mPhotonDist->Write();
  h_mPhotonGenerated->Write();
  h_mWaveLength->Write();
  h_mNumOfPhotonsDist->Write();
  p_mNumOfPhotons->Write();

  return 0;
}

int gemcCalibration::writeGausSmearing()
{
  h_mXGausSmearing->Write();
  h_mYGausSmearing->Write();

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

double gemcCalibration::GausSmearing(TF1 *f_gaus)
{
  double delta_pos = f_gaus->GetRandom();
  return delta_pos;
}

bool gemcCalibration::isInSensorPlane(double out_x, double out_y)
{
  if( !(TMath::Abs(out_x) > 2.5 && TMath::Abs(out_x) < mRICH::mHalfWidth-2.0) ) return false;
  if( !(TMath::Abs(out_y) > 2.5 && TMath::Abs(out_y) < mRICH::mHalfWidth-2.0) ) return false;
  return true;
}

//-----------------------------------------------------------------

int gemcCalibration::initRingFinder()
{
  h_mNumOfPhotons = new TH1D("h_mNumOfPhotons","h_mNumOfPhotons",1,-0.5,0.5);
  h_mRingFinder = new TH2D("h_mRingFinder","h_mRingFinder",mRICH::mNumOfPixels,mRICH::mPixels,mRICH::mNumOfPixels,mRICH::mPixels);
  h_mHoughTransform = new TH3D("h_mHoughTransform","h_mHoughTransform",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,1.0*mRICH::mHalfWidth);

  clearRingFinder();

  h_mQA_HT = new TH3D("h_mQA_HT","h_mQA_HT",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,2.0*mRICH::mHalfWidth);
  h_mCherenkovRing = new TH3D("h_mCherenkovRing","h_mCherenkovRing",210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,210,-1.0*mRICH::mHalfWidth,mRICH::mHalfWidth,105,0,2.0*mRICH::mHalfWidth);
  h_mNumOfCherenkovPhotons = new TH2D("h_mNumOfCherenkovPhotons","h_mNumOfCherenkovPhotons",100,-0.5,99.5,100,-0.5,99.5);

  return 1;
}

int gemcCalibration::clearRingFinder()
{
  h_mNumOfPhotons->Reset();
  h_mRingFinder->Reset();
  mXPixelMap.clear();
  mYPixelMap.clear();

  h_mHoughTransform->Reset();

  return 1;
}

int gemcCalibration::writeRingFinder()
{
  h_mQA_HT->Write();
  h_mCherenkovRing->Write();
  h_mNumOfCherenkovPhotons->Write();

  return 1;
}


bool gemcCalibration::findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov)
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

bool gemcCalibration::isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5) return true;
  if(TMath::Abs(firstHit.X()-thirdHit.X())  < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y())  < 1e-5) return true;
  if(TMath::Abs(secondHit.X()-thirdHit.X()) < 1e-5 && TMath::Abs(secondHit.Y()-thirdHit.Y()) < 1e-5) return true;
  if(firstHit.Mod() > 1000.0 || secondHit.Mod() > 1000.0 || thirdHit.Mod() > 1000.0) return true; // not proper initialized

  return false;
}

bool gemcCalibration::isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit)
{
  if(TMath::Abs(firstHit.X()-secondHit.X()) < 1e-5 && TMath::Abs(firstHit.X()-thirdHit.X()) < 1e-5) return true;
  if(TMath::Abs(firstHit.Y()-secondHit.Y()) < 1e-5 && TMath::Abs(firstHit.Y()-thirdHit.Y()) < 1e-5) return true;

  double slope12 = (firstHit.Y()-secondHit.Y())/(firstHit.X()-secondHit.X());
  double slope13 = (firstHit.Y()-thirdHit.Y())/(firstHit.X()-thirdHit.X());

  if(TMath::Abs(slope12-slope13) < 1e-5) return true;

  return false;
}

bool gemcCalibration::isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform)
{
  double x_diff = photonHit.X() - x_HoughTransform;
  double y_diff = photonHit.Y() - y_HoughTransform;
  double r_diff = TMath::Sqrt(x_diff*x_diff+y_diff*y_diff) - r_HoughTransform;

  double sigma_x = 1.5;
  double sigma_y = 1.5;
  double sigma_r = TMath::Sqrt(sigma_x*sigma_x+sigma_y*sigma_y);

  if( TMath::Abs(r_diff) < sigma_r) return true;

  return false;
}

int gemcCalibration::HoughTransform(TH1D *h_NumOfPhotons, TH2D *h_RingFinder, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfPhotons = h_NumOfPhotons->GetEntries();
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
  }
  // cout << "NumOfPhotons = " << NumOfPhotons << ", NumOfPhotonsOnRing = " << NumOfPhotonsOnRing << endl;
  h_mNumOfCherenkovPhotons->Fill(NumOfPhotons,NumOfPhotonsOnRing);

  return 0;
}
