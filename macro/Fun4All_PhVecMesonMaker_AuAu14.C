void Fun4All_PhVecMesonMaker_AuAu14(
    int mode = 1, int nEvents = 5, //
    char *input_file = "AuAu14_BBCEP_test.list" //
    )
{
  gSystem->Load("../../install/lib/libPhVecMesonMaker.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc =  recoConsts::instance();
  rc->set_IntFlag("RD_RUN_SELECTION", 14);
  rc->set_IntFlag("RD_SYSTEM_SELECTION", 0);
  rc->get_IntFlag("EP_DEBUG", 0);
  rc->get_IntFlag("EP_CALIB", 0);
  rc->get_DoubleFlag("RD_BBCZCUT_VAL", 10);
  rc->get_IntFlag("EP_BBC", 0);

  PhVecMesonMaker *ana = new PhVecMesonMaker();
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
