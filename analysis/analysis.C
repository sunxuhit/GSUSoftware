#include <iostream> 
#include <fstream>
#include <cmath> 
#include <math.h> 
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMinuit.h>
#include <TPaveText.h>
#include <TText.h>
#include <TSystem.h>
#include <TArc.h>

using namespace std;

///// extrapolate the quantum efficiency for materials: Bialkali, GaAsP, and GaAs
///// from the "proposal for eRD11 project"
double extrapQE_SbKCs(double wl);
double extrapQE_GaAsP(double wl);
double extrapQE_GaAs(double wl);

double extrapQE_SbKCs(double wl)
{
/// Bialkali (antimony-potassium-caesium Sb-K-Cs)
double photon_wl_SbKCs[70]={258.71, 264.25, 269.78, 273.47, 275.32, 277.16, 279.00, 282.70, 284.54, 286.39,				  288.24, 291.93, 295.62, 299.34, 306.69, 312.23, 323.30, 334.37, 345.44, 358.36,
			    406.34, 413.73, 424.80, 430.33, 437.72, 445.10, 452.48, 461.71, 469.10, 480.16,
			    487.54, 494.93, 504.15, 511.53, 517.07, 522.61, 526.30, 529.99, 531.83, 537.37,
		  	    541.06, 546.60, 552.13, 557.67, 565.05, 572.43, 581.66, 587.20, 592.73, 598.27,
			    603.81, 607.50, 611.19, 614.88, 618.57, 622.26, 627.80, 629.64, 633.33, 637.02,
			    640.72, 646.25, 651.79, 655.48, 659.17, 662.86, 666.55, 670.24, 673.93, 677.62 };
/// Quantum Efficiency for Bialkali (%)
double QE_SbKCs[70] = {	1.97 , 2.96 , 3.91 , 4.94 , 5.97 , 6.91 , 7.99 , 8.85 , 9.95 , 10.71,
			12.0 , 13.33, 15.20, 16.59, 18.65, 19.77, 21.58, 22.88, 24.61, 24.97,
			25.34, 24.97, 24.25, 23.90, 23.21, 22.55, 21.58, 20.35, 19.20, 17.59,
			16.35, 15.65, 14.76, 13.92, 13.33, 12.39, 11.35, 10.40, 9.66 , 8.60,
			7.76 , 7.01 , 6.14 , 5.63 , 5.01 , 4.39 , 3.74 , 3.43 , 3.00 , 2.75,
			2.38 , 2.15 , 1.91 , 1.70 , 1.51 , 1.31 , 1.10 , 0.99 , 0.87 , 0.75,
			0.65 , 0.52 , 0.39 , 0.35 , 0.30 , 0.25 , 0.21 , 0.17 , 0.13 , 0.11 };

	double QE = -999.;

	int npara = 70; int iBin = -1;
	for(int i=0; i<npara-1; i++){
		if(wl>=photon_wl_SbKCs[i] && wl<photon_wl_SbKCs[i+1]){
			iBin = i;
			break;
		}
	}
	if(iBin>=0 && iBin<npara){
		double x1 = photon_wl_SbKCs[iBin];
		double x2 = photon_wl_SbKCs[iBin+1];
		double y1 = QE_SbKCs[iBin];
		double y2 = QE_SbKCs[iBin+1];
		QE = y1 + (wl-x1)*(y2-y1)/(x2-x1);
	}

	if(QE>0.) return QE/100.;
	else return 0;
}

double extrapQE_GaAsP(double wl)
{
/// GaAsP
double photon_wl_GaAsP[62]={  360.21, 362.05, 363.90, 365.74, 367.59, 369.44, 371.28, 376.82, 380.51, 386.04,
			      389.74, 393.43, 400.81, 406.34, 411.88, 417.42, 426.64, 437.72, 450.63, 459.86,
			      470.93, 485.70, 498.62, 518.92, 542.91, 555.83, 570.59, 579.82, 590.89, 600.12,
			      609.34, 618.57, 625.95, 631.49, 637.02, 642.56, 648.10, 653.63, 659.17, 662.86,
			      666.55, 670.24, 675.78, 679.47, 681.32, 683.16, 686.85, 688.70, 690.54, 694.23,
			      696.08, 697.92, 699.77, 701.62, 705.31, 707.15, 710.84, 712.69, 714.53, 716.38,
			      718.22, 720.07 };

/// Quantum Efficiency for GaAsP (%)
double QE_GaAsP[62] = { 0.39 , 0.59 , 1.37 , 1.97 , 2.96 , 4.33 , 5.88 , 7.54 , 9.39 , 12.94,
			16.59, 19.77, 24.97, 29.32, 32.48, 37.04, 39.27, 41.63, 44.13, 46.11,
			47.48, 48.89, 49.61, 49.61, 48.18, 47.48, 46.11, 44.79, 43.50, 42.25,
			40.44, 38.70, 37.04, 35.46, 33.94, 32.48, 31.09, 29.76, 28.07, 26.86,
			25.71, 24.25, 22.55, 21.27, 19.77, 18.38, 15.88, 14.13, 12.94, 10.55,
			8.35 , 7.01 , 6.06 , 5.16 , 4.46 , 3.97 , 3.33 , 1.75 , 1.20 , 0.81,
			0.51 , 0.37 };

	double QE = -999.;

	int npara = 62; int iBin = -1;
	for(int i=0; i<npara-1; i++){
		if(wl>=photon_wl_GaAsP[i] && wl<photon_wl_GaAsP[i+1]){
			iBin = i;
			break;
		}
	}
	if(iBin>=0 && iBin<npara){
		double x1 = photon_wl_GaAsP[iBin];
		double x2 = photon_wl_GaAsP[iBin+1];
		double y1 = QE_GaAsP[iBin];
		double y2 = QE_GaAsP[iBin+1];
		QE = y1 + (wl-x1)*(y2-y1)/(x2-x1);
	}
	if(QE>0.) return QE/100.;
	else return 0;
}

double extrapQE_GaAs(double wl)
{
/// GaAs
double photon_wl_GaAs[63]={ 380.51, 382.35, 384.20, 386.04, 389.74, 393.43, 397.12, 400.81, 404.50, 410.03,
			413.73, 419.26, 424.80, 426.64, 432.18, 437.72, 443.25, 454.33, 463.55, 474.63,
			482.01, 491.23, 500.46, 507.84, 515.23, 531.83, 555.83, 600.11, 697.92, 764.36,
			799.42, 817.88, 828.95, 840.02, 845.56, 851.10, 858.48, 864.01, 867.71, 871.40,
			875.09, 876.93, 880.62, 882.47, 884.31, 886.16, 888.01, 889.85, 891.70, 893.54,
			895.39, 897.23, 899.08, 900.92, 904.61, 906.46, 908.30, 910.15, 912.00, 913.84,	
			915.69, 917.53, 919.38 };

/// Quantum Efficiency for GaAs (%)
double QE_GaAs[63] = { 	0.93 , 1.08 , 1.39 , 1.72 , 2.34 , 2.96 , 3.53 , 4.02 , 4.94 , 5.97,
			6.91 , 7.99 , 9.12 , 9.95 , 11.02, 12.39, 13.33, 14.98, 16.11, 18.11,
			19.20, 20.35, 21.27, 22.22, 22.88, 23.56, 24.25, 24.97, 24.97, 23.90,
			23.21, 22.88, 22.55, 21.58, 20.65, 19.77, 18.92, 17.33, 15.88, 14.34,
			12.94, 11.69, 10.86, 9.81 , 8.85 , 7.99 , 6.91 , 6.15 , 4.94 , 3.80,
			3.05 , 2.31 , 1.70 , 1.33 , 1.11 , 1.00 , 0.91 , 0.77 , 0.59 , 0.49,
			0.30 , 0.20 , 0.11 };
	double QE = -999.;

	int npara = 63; int iBin = -1;
	for(int i=0; i<npara-1; i++){
		if(wl>=photon_wl_GaAs[i] && wl<photon_wl_GaAs[i+1]){
			iBin = i;
			break;
		}
	}
	if(iBin>=0 && iBin<npara){
		double x1 = photon_wl_GaAs[iBin];
		double x2 = photon_wl_GaAs[iBin+1];
		double y1 = QE_GaAs[iBin];
		double y2 = QE_GaAs[iBin+1];
		QE = y1 + (wl-x1)*(y2-y1)/(x2-x1);
	}
	if(QE>0.) return QE/100.;
	else return 0;
}



void analysis(string input_filename)
{
gROOT->Reset();
gStyle->SetPalette(1);
gStyle->SetOptStat(0);

const double DEG=180./3.1415926;

char the_filename[200];
sprintf(the_filename, "%s",input_filename.substr(0,input_filename.rfind(".")).c_str());

char output_filename[200];
sprintf(output_filename, "%s_hist.root",the_filename);
TFile *outputfile=new TFile(output_filename, "recreate");
TH2D *hnHitPerEvtvsMom = new TH2D ("hnHitPerEvtvsMom","hnHitPerEvtvsMom",100,0.,10.,50,0.,50.);
TH2D *hRadiusvsMom = new TH2D ("hRadiusvsMom","hRadiusvsMom",400,0.,10.,200,0.,20.);
TH2D *hResvsMom = new TH2D ("hResvsMom","hResvsMom",400,0.,10.,250,0.,0.25);
TH2D *hResvsRadius = new TH2D ("hResvsRadius","hResvsRadius",200,0.,20.,200,0.,5.);

TH2D *hPhotoDetXY = new TH2D ("hPhotoDetXY","hPhotoDetXY",88,-44.,44.,88,-44.,44.);

TH1D *hPhotonWL = new TH1D ("hPhotonWL","hPhotonWL",100,250.,750.);
TH1D *hPhotonE = new TH1D ("hPhotonE","hPhotonE",100,0.,5.);
TH2D *hnPhotonElvsnHits_SbKCs = new TH2D ("hnPhotonElvsnHits_SbKCs","hnPhotonElvsnHits_SbKCs",50,0.,50.,100,0.,25.);
TH2D *hnPhotonElvsnHits_GaAsP = new TH2D ("hnPhotonElvsnHits_GaAsP","hnPhotonElvsnHits_GaAsP",50,0.,50.,100,0.,25.);
TH2D *hnPhotonElvsnHits_GaAs = new TH2D ("hnPhotonElvsnHits_GaAs","hnPhotonElvsnHits_GaAs",50,0.,50.,100,0.,25.);
TH2D *hPhotoDetXY_Weight1Evt_SbKCs = new TH2D ("hPhotoDetXY_Weight1Evt_SbKCs","hPhotoDetXY_Weight1Evt_SbKCs",88,-44.,44.,88,-44.,44.);
TH2D *hPhotoDetXY_Weight1Evt_GaAsP = new TH2D ("hPhotoDetXY_Weight1Evt_GaAsP","hPhotoDetXY_Weight1Evt_GaAsP",88,-44.,44.,88,-44.,44.);
TH2D *hPhotoDetXY_Weight1Evt_GaAs = new TH2D ("hPhotoDetXY_Weight1Evt_GaAs","hPhotoDetXY_Weight1Evt_GaAs",88,-44.,44.,88,-44.,44.);

TFile *file=new TFile(input_filename.c_str());
if (file->IsZombie()) {
    cout << "Error opening file" << input_filename << endl;
    exit(-1);
}
else cout << "open file " << input_filename << endl;    

TTree *tree_header = (TTree*) file->Get("header");
vector <int> *evn=0,*evn_type=0;
vector <double> *beamPol=0;
vector <int> *var1=0,*var2=0,*var3=0,*var4=0,*var5=0,*var6=0,*var7=0,*var8=0;
tree_header->SetBranchAddress("evn",&evn);
tree_header->SetBranchAddress("evn_type",&evn_type);
tree_header->SetBranchAddress("beamPol",&beamPol);
tree_header->SetBranchAddress("var1",&var1);
tree_header->SetBranchAddress("var2",&var2);
tree_header->SetBranchAddress("var3",&var3);
tree_header->SetBranchAddress("var4",&var4);
tree_header->SetBranchAddress("var5",&var5);
tree_header->SetBranchAddress("var6",&var6);
tree_header->SetBranchAddress("var7",&var7);
tree_header->SetBranchAddress("var8",&var8);

TTree *tree_generated = (TTree*) file->Get("generated");
vector <int> *gen_pid=0;
vector <double> *gen_px=0,*gen_py=0,*gen_pz=0,*gen_vx=0,*gen_vy=0,*gen_vz=0;
tree_generated->SetBranchAddress("pid",&gen_pid);
tree_generated->SetBranchAddress("px",&gen_px);
tree_generated->SetBranchAddress("py",&gen_py);
tree_generated->SetBranchAddress("pz",&gen_pz);
tree_generated->SetBranchAddress("vx",&gen_vx);
tree_generated->SetBranchAddress("vy",&gen_vy);
tree_generated->SetBranchAddress("vz",&gen_vz);

TTree *tree_flux = (TTree*) file->Get("flux");
vector<int> *flux_id=0,*flux_hitn=0;
vector<int> *flux_pid=0,*flux_mpid=0,*flux_tid=0,*flux_mtid=0,*flux_otid=0;
vector<double> *flux_trackE=0,*flux_totEdep=0,*flux_avg_x=0,*flux_avg_y=0,*flux_avg_z=0,*flux_avg_lx=0,*flux_avg_ly=0,*flux_avg_lz=0,*flux_px=0,*flux_py=0,*flux_pz=0,*flux_vx=0,*flux_vy=0,*flux_vz=0,*flux_mvx=0,*flux_mvy=0,*flux_mvz=0,*flux_avg_t=0;
tree_flux->SetBranchAddress("hitn",&flux_hitn);
tree_flux->SetBranchAddress("id",&flux_id);
tree_flux->SetBranchAddress("pid",&flux_pid);
tree_flux->SetBranchAddress("mpid",&flux_mpid);
tree_flux->SetBranchAddress("tid",&flux_tid);
tree_flux->SetBranchAddress("mtid",&flux_mtid);
tree_flux->SetBranchAddress("otid",&flux_otid);
tree_flux->SetBranchAddress("trackE",&flux_trackE);
tree_flux->SetBranchAddress("totEdep",&flux_totEdep);
tree_flux->SetBranchAddress("avg_x",&flux_avg_x);
tree_flux->SetBranchAddress("avg_y",&flux_avg_y);
tree_flux->SetBranchAddress("avg_z",&flux_avg_z);
tree_flux->SetBranchAddress("avg_lx",&flux_avg_lx);
tree_flux->SetBranchAddress("avg_ly",&flux_avg_ly);
tree_flux->SetBranchAddress("avg_lz",&flux_avg_lz);
tree_flux->SetBranchAddress("px",&flux_px);
tree_flux->SetBranchAddress("py",&flux_py);
tree_flux->SetBranchAddress("pz",&flux_pz);
tree_flux->SetBranchAddress("vx",&flux_vx);
tree_flux->SetBranchAddress("vy",&flux_vy);
tree_flux->SetBranchAddress("vz",&flux_vz);
tree_flux->SetBranchAddress("mvx",&flux_mvx);
tree_flux->SetBranchAddress("mvy",&flux_mvy);
tree_flux->SetBranchAddress("mvz",&flux_mvz);
tree_flux->SetBranchAddress("avg_t",&flux_avg_t);

cout << tree_header->GetEntries() << " " << tree_generated->GetEntries() << endl;

int nevent = (int)tree_generated->GetEntries();
int nselected = 0;
cout << "nevent " << nevent << endl;


int filled=0;
for (Int_t i=0;i<nevent;i++) { 

  if(i%100==0) cout << "processing event:  " << i << " ;"<<endl;

  tree_header->GetEntry(i);
  
  tree_generated->GetEntry(i);  

  int pid_gen=0;
  double theta_gen=0,phi_gen=0,p_gen=0,px_gen=0,py_gen=0,pz_gen=0,vx_gen=0,vy_gen=0,vz_gen=0;      
//  cout<< "tree generated size: "<< gen_pid->size() <<";    tree flux size:  "<< flux_hitn->size() <<endl;
  for (int j=0;j<gen_pid->size();j++) {
//    cout << gen_pid->at(j) << " " << gen_px->at(j) << " " << gen_py->at(j) << " " << gen_pz->at(j) << " " << gen_vx->at(j) << " " << gen_vy->at(j) << " " << gen_vz->at(j) << endl; 
      pid_gen=gen_pid->at(j);
      px_gen=gen_px->at(j)/1e3;    	//in MeV, convert to GeV
      py_gen=gen_py->at(j)/1e3;		//in MeV, convert to GeV
      pz_gen=gen_pz->at(j)/1e3;      	//in MeV, convert to GeV
      vx_gen=gen_vx->at(j)/1e1;    	//in mm, convert to cm
      vy_gen=gen_vy->at(j)/1e1;		//in mm, convert to cm
      vz_gen=gen_vz->at(j)/1e1;     	//in mm, convert to cm
      p_gen=sqrt(px_gen*px_gen+py_gen*py_gen+pz_gen*pz_gen);
      theta_gen=acos(pz_gen/p_gen)*DEG;  	//in deg
      phi_gen=atan2(py_gen,px_gen)*DEG;     	//in deg            
  }  
  
    tree_flux->GetEntry(i);    
    
    hnHitPerEvtvsMom->Fill(p_gen,flux_hitn->size());
    if(pid_gen==11 && p_gen<0) continue; ///electron
    if(pid_gen==13 && p_gen<0.2) continue; ///muon
    if(pid_gen==-211 && p_gen<0.4) continue; ///pi-
    if(pid_gen==-321 && p_gen<1.) continue; ///kaon-
    if(pid_gen==2212 && p_gen<3.) continue; ///proton
    if(flux_hitn->size()<=5) continue; /// num of hit cut
/*
    double R0x = -999., R0y = -999.; /// primary particle x,y position on photon detector
    for (Int_t j=0;j<flux_hitn->size();j++) {
	    if(flux_pid->at(j)!=0) {
		    R0x = flux_avg_x->at(j);
		    R0y = flux_avg_y->at(j);
	    }
    }
*/
    double Rmean = 0., Rsigma = 0.; /// Cherenkov radiation ring radius on photon detector
    int nphoton = 0;
    for (Int_t j=0;j<flux_hitn->size();j++) {
	    if(flux_pid->at(j)==0) {
		    Rmean = Rmean+sqrt( pow(flux_avg_x->at(j),2) + pow(flux_avg_y->at(j),2) );
		    nphoton++;
	    }
    }
    Rmean = Rmean/nphoton;

    for (Int_t j=0;j<flux_hitn->size();j++) {
	    if(flux_pid->at(j)==0) {
		    double Rj = sqrt( pow(flux_avg_x->at(j),2) + pow(flux_avg_y->at(j),2) );
		    Rsigma = Rsigma + pow((Rj-Rmean),2);
	    }
    }
    Rsigma = sqrt(Rsigma/nphoton);

    hRadiusvsMom->Fill(p_gen,Rmean);
    hResvsMom->Fill(p_gen,Rsigma/Rmean);
    hResvsRadius->Fill(Rmean,Rsigma);

    for (Int_t j=0;j<flux_hitn->size();j++) {
	    if(flux_pid->at(j)==0) {
		hPhotoDetXY->Fill(flux_avg_x->at(j),flux_avg_y->at(j));
	    }
    }

    double nphoton_all(0), nel_SbKCs(0.0), nel_GaAsP(0.0), nel_GaAs(0.0);
    for (Int_t j=0;j<flux_hitn->size();j++) {
	    if(flux_pid->at(j)==0) {
		nphoton_all++;
		double photonE = flux_trackE->at(j);   /// in MeV (GEANT4 default)
		double wavelength = 1240./(photonE*1.e6);  /// MeV -> eV, wavelength will be in "nm"
		hPhotonWL->Fill(wavelength);
		hPhotonE->Fill(photonE*1.e6);

		double QE_SbKCs = extrapQE_SbKCs(wavelength);
		double QE_GaAsP = extrapQE_GaAsP(wavelength);
		double QE_GaAs  = extrapQE_GaAs(wavelength);

		nel_SbKCs = nel_SbKCs + QE_SbKCs; 
		nel_GaAsP = nel_GaAsP + QE_GaAsP; 
		nel_GaAs = nel_GaAs + QE_GaAs; 
	    }
    }
    hnPhotonElvsnHits_SbKCs->Fill(nphoton_all,nel_SbKCs);
    hnPhotonElvsnHits_GaAsP->Fill(nphoton_all,nel_GaAsP);
    hnPhotonElvsnHits_GaAs->Fill(nphoton_all,nel_GaAs);
	

    if(!filled && p_gen>5.){
	    for (Int_t j=0;j<flux_hitn->size();j++) {
		    if(flux_pid->at(j)==0) {
			    double photonE = flux_trackE->at(j);   /// in MeV (GEANT4 default)
			    double wavelength = 1240./(photonE*1.e6);  /// MeV -> eV, wavelength will be in "nm"
			    double QE_SbKCs = extrapQE_SbKCs(wavelength);
			    double QE_GaAsP = extrapQE_GaAsP(wavelength);
			    double QE_GaAs  = extrapQE_GaAs(wavelength);

			    hPhotoDetXY_Weight1Evt_SbKCs->Fill(flux_avg_x->at(j),flux_avg_y->at(j),QE_SbKCs);
			    hPhotoDetXY_Weight1Evt_GaAsP->Fill(flux_avg_x->at(j),flux_avg_y->at(j),QE_GaAsP);
			    hPhotoDetXY_Weight1Evt_GaAs->Fill(flux_avg_x->at(j),flux_avg_y->at(j),QE_GaAs);
		    }
	    }
	    filled=true;
    }
/*
    for (Int_t j=0;j<flux_hitn->size();j++) {
    //       cout << "flux " << j << " !!! " << flux_id->at(j) << " " << flux_pid->at(j) << " " << flux_mpid->at(j) << " " << flux_tid->at(j) << " " << flux_mtid->at(j) << " " << flux_trackE->at(j) << " " << flux_totEdep->at(j) << " " << flux_avg_x->at(j) << " " << flux_avg_y->at(j) << " " << flux_avg_z->at(j) << " " << flux_avg_lx->at(j) << " " << flux_avg_ly->at(j) << " " << flux_avg_lz->at(j) << " " << flux_px->at(j) << " " << flux_py->at(j) << " " << flux_pz->at(j) << " " << flux_vx->at(j) << " " << flux_vy->at(j) << " " << flux_vz->at(j) << " " << flux_mvx->at(j) << " " << flux_mvy->at(j) << " " << flux_mvz->at(j) << " " << flux_avg_t->at(j) << endl;           
   //  int detector_ID=flux_id->at(j)/1000000;
   // int subdetector_ID=(flux_id->at(j)%1000000)/100000;
   // int subsubdetector_ID=((flux_id->at(j)%1000000)%100000)/10000;
   // cout << detector_ID << " " << subdetector_ID << " "  << subsubdetector_ID << endl;  
    }
*/
}
file->Close();

outputfile->Write();
outputfile->Flush();
}
