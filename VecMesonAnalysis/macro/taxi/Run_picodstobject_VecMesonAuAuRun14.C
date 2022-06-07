#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void Run_picodstobject_VecMesonAuAuRun14(const char *outFile = "picodstobject.root")
{
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libcompactCNT.so");
  gSystem->Load("libmutoo_subsysreco");
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("libMWGOO");
  gSystem->Load("libmutrg");
  gSystem->Load("libRPCalibRun.so");
  // For event plane angle calibration, private version by Hachiya for Run 14 Au+Au  //gSystem->Load("librpc_subsysreco");
  //gSystem->Load("librpc_muotrackreco");
  gSystem->Load("librecal");
  gSystem->Load("libpicodst_object.so");
  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  // FVTX Geometry release 8                                       
  //TFvtxGlobalParCntrl::set_bool_par("geom_use_calibration_database", false);
  //TFvtxGlobalParCntrl::set_string_par("geom_root_file_path","/direct/phenix+subsys+fvtx/jinhuang/geometry/release10/");
  //TFvtxGlobalParCntrl::set_string_par("geom_root_file_name", "fvtxgeom_release10.root");
  // Use beam-average position for x,y: 
  //TFvtxGlobalParCntrl::set_bool_par("beam_use_average_xy", true);
  // TFvtxGlobalParCntrl::set_bool_par("use_svx", true);
  // TFvtxGlobalParCntrl::set_bool_par("is_pp", true);

  MuonReadbackDST *mutoo = new MuonReadbackDST();
  mutoo->Verbosity( 0 );
  se->registerSubsystem( mutoo );
  
  FvtxReadbackDST *fvtxoo = new FvtxReadbackDST();
  fvtxoo->Verbosity(0);
  se->registerSubsystem(fvtxoo);
 
  //mFillFVTXTrackletContainer* mtracklet = new mFillFVTXTrackletContainer();
  //se->registerSubsystem(mtracklet);
 
  //mFillEvtHistos* mevt = new mFillEvtHistos("mFillEvtHistos",outFile);
  //se->registerSubsystem(mevt);

  /*
    VERSIONS FOR SingleMuonContainer
    1- very basic single muon
    2- minimum single muon information in order to reconstruct di-muons
    3- now is the same as version2, it is supposed to be an extended object for HF analysis
    4- first version with FVTX variables
    5- version with FVTX plus a minimum RPC information
    6- full FVTX+RPC object for W analysis 
    7- version with a complete set of FVTX variables
    9- version with FVTX swapped matching variables (DEFAULT)
  */

  mFillSingleMuonContainer* msngl = new mFillSingleMuonContainer();
  msngl->set_is_sim(false);
  msngl->set_bbcz_cut(15.0);
  msngl->set_lastgap_cut(4);
  msngl->set_DG0_cut(20);
  msngl->set_DDG0_cut(15);
  msngl->set_chi2_cut(10);
  msngl->set_nidhits_cut(5);
  se->registerSubsystem(msngl);

  /*mFvtxPostProductionAlignment* align = new mFvtxPostProductionAlignment();
  se->registerSubsystem(align);
  align->set_align_what(mFvtxPostProductionAlignment::DG0);*/

  /*mFvtxMuTrAssociation* mfvtxmutr = new mFvtxMuTrAssociation();
  se->registerSubsystem(mfvtxmutr);
  mfvtxmutr->set_nsigmas_fvtxmutr_match(9999.);
  mfvtxmutr->set_chi2_fvtx_cut(9999.);
  mfvtxmutr->set_min_nhits_fvtx(2);*/
  
  mFillDiMuonContainer* mdi = new mFillDiMuonContainer(true);
  mdi->set_bbcz_cut(15);
  mdi->set_mass_cut(0.2);
  mdi->set_is_sim(false);
  mdi->set_is_pp(false);
  se->registerSubsystem(mdi);
  
  //mFillMutrRefitSingleMuonContainer* msngl_MutrRefit = new mFillMutrRefitSingleMuonContainer();
  //se->registerSubsystem(msngl_MutrRefit);
  
  //Reaction Plane Node
  recoConsts *rc = recoConsts::instance();  
  rc->set_IntFlag("RPCALIB_READFROMDB", 0);
  rc->set_CharFlag("RPCALIB_CALIBFILENAME", "");

  rc->set_IntFlag("RP_SKIP_RECENTERING", 0); // do recentering
  rc->set_IntFlag("RP_SKIP_FLATTENING", 0); // do flattening

  rc->set_IntFlag("RPCALIB_READFROMDB", 0);
  rc->set_CharFlag("RPCALIB_CALIBFILENAME", "");

  rc->set_IntFlag("RP_SKIP_RECENTERING", 0); // do recentering
  rc->set_IntFlag("RP_SKIP_FLATTENING", 0); // do flattening

  RPReadCalibTree *readT = new RPReadCalibTree();
  readT->setTreeFileRecent("RP_recent_run14pro106_newcent_merge.root");
  readT->setTreeFileFlat("RP_flat_run14pro106_newcent_merge.root");
  readT->setTOADname("hachiya/15.08");
  se->registerSubsystem(readT);
  
  //mFillReactionPlaneContainer* mrp = new mFillReactionPlaneContainer(2); //1, 2, 3
  //se->registerSubsystem(mrp);
 
  Fun4AllOutputManager *outpico = new Fun4AllDstOutputManager("outpico",outFile);
  outpico->Verbosity(0);
  outpico->AddNode("RunHeader");
  outpico->AddNode("EventHeader");
  outpico->AddNode("Sync");
  outpico->AddNode("VtxOut");
  outpico->AddNode("TrigLvl1");
  outpico->AddNode("PHGlobal"); 
  outpico->AddNode("DiMuonContainer");                
  outpico->AddEventSelector("mFillDiMuonContainer"); 
  outpico->AddNode("ReactionPlaneObject");
  se->registerOutputManager(outpico);
  //outpico->Print();
}

void 
InputData(vector<string> &indata)
{
  //indata.push_back("CNT");
  indata.push_back("DST_EVE");
  indata.push_back("MWG");
  indata.push_back("muondst");
  return;
}
