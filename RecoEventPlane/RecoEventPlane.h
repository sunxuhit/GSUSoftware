#ifndef __RecoEventPlane_H__
#define __RecoEventPlane_H__

#include "SubsysReco.h"

class PHCompositeNode;
class BbcCalib;
class BbcGeo;

class TTree;
class TFile;
class TH1F;
class TH2F;

class RecoEventPlane: public SubsysReco {
public:

	RecoEventPlane();

	//! global initialization
	int Init(PHCompositeNode *topNode);

	//! Run initialization
	int InitRun(PHCompositeNode *topNode);

	//! event method
	int process_event(PHCompositeNode *topNode);

	//! global termination
	int End(PHCompositeNode *topNode);

	const std::string& get_eval_file_name() const {
		return _eval_file_name;
	}

	void set_eval_file_name(const std::string& evalFileName) {
		_eval_file_name = evalFileName;
	}

private:

	//!
	int _n_events_processed;

	//! Eval Tree Output
	std::string _eval_file_name;
	TFile* _fout;

	// vertex QA
	TH1F *h_mVtZ;
	TH2F *h_mVtR;
	TH1F *h_mVtZ_Bbc;
	TH1F *h_mVtZ_Zdc;

	BbcCalib *bbccalib;
	BbcGeo *bbcgeo;

	TH1F *h_mAdc_Bbc[128]; // raw adc distribution
	TH1F *h_mCharge_Bbc[128]; // (adc-ped)*gain_adc/gain_pmt

	TH2F *h_mGeoXY_BbcSouth; // BBC XY charge map for south
	TH1F *h_mGeoZ_BbcSouth; // BBC Z distribution map for south

	TH2F *h_mGeoXY_BbcNorth; // BBC 2-D charge map for north 
	TH1F *h_mGeoZ_BbcNorth; // BBC Z distribution map for south

	TH1F *h_mEP1st_BbcSouth;
	TH1F *h_mEP1st_BbcNorth;
	TH1F *h_mEP2nd_BbcSouth;
	TH1F *h_mEP2nd_BbcNorth;
};

#endif //__H_RecoEventPlane_H__
