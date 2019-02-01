#ifndef __RecoEventPlane_H__
#define __RecoEventPlane_H__

#include "SubsysReco.h"

class PHCompositeNode;
class BbcCalib;
class BbcGeo;

class TTree;
class TFile;
class TH1F;

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
	TTree* _T;
	float _mass;

	BbcCalib *bbccalib;
	BbcGeo *bbcgeo;
	TH1F *h_mEP_BbcSouth;
	TH1F *h_mEP_BbcNorth;
};

#endif //__H_RecoEventPlane_H__
