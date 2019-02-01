//local
#include "RecoEventPlane.h"

//PHENIX
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>
#include <DiMuonContainer.h>
#include <DiMuon.h>

#include <Bbc.hh>
#include <BbcRaw.h>
#include <BbcOut.h>
#include <BbcCalib.hh>
#include <BbcGeo.hh>

#include "VtxOut.h"
#include <PHPoint.h>

//stl
#include <iostream>
#include <ostream>

//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TMath.h>

using namespace std;

RecoEventPlane::RecoEventPlane() :
		_n_events_processed(0),
		_eval_file_name("RecoEventPlaneEval.root"),
		_fout(NULL),
		_T(NULL),
		_mass(-999)
 {
}

int RecoEventPlane::Init(PHCompositeNode *topNode) {

	_n_events_processed = 0;

	// init output structure
	_fout = TFile::Open(_eval_file_name.data(), "recreate");
	if(!_fout) cout<<"Cann't create " <<_eval_file_name.data() <<endl;
	if(!_T) _T = new TTree("T", "RecoEventPlane Eval Tree");

	_T->Branch("mass", &_mass,"mass/F");

	bbccalib = new BbcCalib();
	bbcgeo = new BbcGeo();

	h_mEP_BbcSouth = new TH1F("h_mEP_BbcSouth","h_mEP_BbcSouth",360,-TMath::Pi(),TMath::Pi());
	h_mEP_BbcNorth = new TH1F("h_mEP_BbcNorth","h_mEP_BbcNorth",360,-TMath::Pi(),TMath::Pi());

	return EVENT_OK;
}

int RecoEventPlane::InitRun(PHCompositeNode *topNode) {
	return EVENT_OK;
}

int RecoEventPlane::process_event(PHCompositeNode *topNode) {

	_n_events_processed++;

	DiMuonContainer* dimuons = findNode::getClass<DiMuonContainer>(topNode, "DiMuonContainer");

	if (!dimuons) {
		cout << PHWHERE
				<< "RecoEventPlane:: DiMuonContainer not in Node Tree"
				<< endl;
		return ABORTRUN;
	}

	for(unsigned int idimu = 0; idimu<dimuons->get_nDiMuons();++idimu) {
		_mass = -9999;
		DiMuon* dimu = dimuons->get_DiMuon(idimu);
		_mass = dimu->get_mass();

		// cout<<"===========================\n";
		// cout<<"event: "<<_n_events_processed<<": mass: "<<_mass<<endl;
		// cout<<"===========================\n";

		_T->Fill();
	}

	VtxOut *vtxout = findNode::getClass<VtxOut>(topNode,"VtxOut");
	if(!vtxout) 
	{
	  cout << "No Vtxout information!" << endl;
	  return 1;
	}

	BbcRaw *bbcraw = findNode::getClass<BbcRaw>(topNode,"BbcRaw");
	if (!bbcraw)
	{
	  cout << PHWHERE << "Could not find BbcRaw !" << endl;
	  return 1;
	}

	float Qx_South = 0.0;
	float Qy_South = 0.0;
	float Qw_South = 0.0;

	float Qx_North = 0.0;
	float Qy_North = 0.0;
	float Qw_North = 0.0;

	for (int ipmt=0; ipmt<128; ipmt++) 
	{
	  short adc = bbcraw->get_Adc(ipmt);
	  short tdc = bbcraw->get_Tdc0(ipmt);
	  float time0 = bbccalib->getHitTime0(ipmt, tdc, adc);
	  float charge = bbccalib->getCharge(ipmt, adc);
	  float bbcx= bbcgeo->getX(ipmt);
	  float bbcy= bbcgeo->getY(ipmt);
	  float bbcz= bbcgeo->getZ(ipmt);

	  if (time0 > 0 && charge > 0 && bbcz < 0) // south
	  {
	    float phi=atan2(bbcy,bbcx);
	    float weight = charge;
	    Qx_South += weight*cos(2.0*phi);
	    Qy_South += weight*sin(2.0*phi);
	    Qw_South += weight;
	    // cout << "ipmt = " << ipmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
	  }
	  if (time0 > 0 && charge > 0 && bbcz > 0)  // north
	  {
	    float phi=atan2(bbcy,bbcx);
	    float weight = charge;
	    Qx_North += weight*cos(2.0*phi);
	    Qy_North += weight*sin(2.0*phi);
	    Qw_North += weight;
	    // cout << "ipmt = " << ipmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
	  }
	}

	float Psi_South = atan2(Qy_South,Qx_South)/2.0;
	h_mEP_BbcSouth->Fill(Psi_South);

	float Psi_North = atan2(Qy_North,Qx_North)/2.0;
	h_mEP_BbcNorth->Fill(Psi_North);

	return EVENT_OK;
}

int RecoEventPlane::End(PHCompositeNode *topNode) {

	_fout->cd();
	_T->Write();
	h_mEP_BbcSouth->Write();
	h_mEP_BbcNorth->Write();
	_fout->Close();

	return EVENT_OK;
}
