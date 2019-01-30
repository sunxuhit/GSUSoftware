//local
#include "RecoEventPlane.h"

//PHENIX
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>
#include <DiMuonContainer.h>
#include <DiMuon.h>

//stl
#include <iostream>
#include <ostream>

//ROOT
#include <TFile.h>
#include <TTree.h>

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

		cout<<"===========================\n";
		cout<<"event: "<<_n_events_processed<<": mass: "<<_mass<<endl;
		cout<<"===========================\n";

		_T->Fill();
	}

	return EVENT_OK;
}

int RecoEventPlane::End(PHCompositeNode *topNode) {

	_fout->cd();
	_T->Write();
	_fout->Close();

	return EVENT_OK;
}
