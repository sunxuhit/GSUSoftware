void Fun4All_PhEventPlaneMaker(
    int nEvents = 1000000, //
    char *input_file = "AuAu14_BBCEP_test.list" //
    )
{
  gSystem->Load("../../install/lib/libPhEventPlaneMaker.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc =  recoConsts::instance();
  rc->set_IntFlag("RUN_SELECTION", 14);
  rc->set_IntFlag("SYSTEM_SELECTION", 0);
  rc->set_DoubleFlag("BBCZCUT_VAL", 10);
  rc->set_IntFlag("ANA_MODE",0); // fill re-center parameter & raw event plane
  // rc->set_IntFlag("ANA_MODE",1); // fill shift parameter & re-centered event plane
  // rc->set_IntFlag("ANA_MODE",2); // calculate resolution & fill shifted event plane
  rc->set_IntFlag("QA_BBC",0);
  rc->set_IntFlag("DEBUG_BBC",0);

  std::string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/LocalTest/file_AuAu200GeV_RawEP_ReCenterPar.root";
  // std::string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/LocalTest/file_AuAu200GeV_ReCenterEP_ShiftPar.root";
  // std::string outputfile = "/direct/phenix+u/xusun/WorkSpace/PHENIX/output/LocalTest/file_AuAu200GeV_ShiftEP_Resoluiton.root";
  PhEventPlaneMaker *ana = new PhEventPlaneMaker(outputfile.c_str());

  se->registerSubsystem(ana);

  Fun4AllDstInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->AddListFile(input_file);
  se->registerInputManager(in);

  se->run(nEvents);
  se->End();

  delete se;

  cout << "Completed reconstruction." << endl;
}
