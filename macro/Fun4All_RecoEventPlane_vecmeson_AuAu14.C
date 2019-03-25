void Fun4All_RecoEventPlane_vecmeson_AuAu14(
    int mode = 1, int nEvents = 10, //
    char *input_file = "AuAu14_BBCEP_test.list" //
    )
{
  gSystem->Load("../../install/lib/libRecoEventPlane.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  RecoEventPlane *ana = new RecoEventPlane();
  ana->set_ana_mode(mode);
  // if(mode == 0) ana->set_eval_file_name("RawEP.root");
  // if(mode == 1) ana->set_eval_file_name("ReCenterEP.root");

  se->registerSubsystem(ana);

  Fun4AllDstInputManager *in = new Fun4AllDstInputManager("DSTin");
  // in->fileopen("/direct/phenix+u/xusun/WorkSpace/PHENIX/Fun4All_Test/picodstobject.root");
  in->AddListFile(input_file);
  se->registerInputManager(in);

  se->run(nEvents);
  se->End();

  delete se;

  cout << "Completed reconstruction." << endl;
}
