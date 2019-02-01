void Fun4All_RecoEventPlane(
		int nEvents = 10000, //
		char *input_file = "AuAu14.list" //
		)
{
	gSystem->Load("../../install/lib/libRecoEventPlane.so");

	Fun4AllServer *se = Fun4AllServer::instance();
	se->Verbosity(0);

	RecoEventPlane *ana = new RecoEventPlane();

	se->registerSubsystem(ana);

	Fun4AllDstInputManager *in = new Fun4AllDstInputManager("DSTin");
	in->fileopen("/direct/phenix+u/xusun/WorkSpace/PHENIX/Fun4All_Test/picodstobject.root");
	// in->AddListFile(input_file);
	se->registerInputManager(in);

	se->run(nEvents);
	se->End();

	delete se;

	cout << "Completed reconstruction." << endl;
}
