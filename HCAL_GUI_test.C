void GSUGUI()
{

  // main frame
  TGMainFrame *fMainFrame1314 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
  fMainFrame1314->SetName("fMainFrame1314");
  fMainFrame1314->SetLayoutBroken(kTRUE);

  // composite frame
  TGCompositeFrame *fMainFrame1560 = new TGCompositeFrame(fMainFrame1314,1329,789+100,kVerticalFrame);
  fMainFrame1560->SetName("fMainFrame1560");
  fMainFrame1560->SetLayoutBroken(kTRUE);

  // composite frame
  TGCompositeFrame *fMainFrame1241 = new TGCompositeFrame(fMainFrame1560,1329,789+100,kVerticalFrame);
  fMainFrame1241->SetName("fMainFrame1241");

  // vertical frame
  TGVerticalFrame *fVerticalFrame734 = new TGVerticalFrame(fMainFrame1241,1327,787+100,kVerticalFrame);
  fVerticalFrame734->SetName("fVerticalFrame734");

  // status bar
  Int_t parts[] = {15, 15, 15, 15, 15, 25};
  fStatusBar739 = new TGStatusBar(fVerticalFrame734,1327,18);
  fStatusBar739->SetName("fStatusBar739");
  fStatusBar739->SetParts(parts, 6);
  fVerticalFrame734->AddFrame(fStatusBar739, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));



  // horizontal frame
  TGHorizontalFrame *fHorizontalFrame768 = new TGHorizontalFrame(fVerticalFrame734,1350,765+100,kHorizontalFrame);
  fHorizontalFrame768->SetName("fHorizontalFrame768");

  // "DAQ FEB controls" group frame
  TGGroupFrame *fGroupFrame679 = new TGGroupFrame(fHorizontalFrame768,"DAQ FEB controls");
  TGTextButton *fTextButton680 = new TGTextButton(fGroupFrame679,"Update Config");
  fTextButton680->SetTextJustify(36);
  fTextButton680->SetMargins(0,0,0,0);
  fTextButton680->SetWrapLength(-1);
  fTextButton680->Resize(123,22);
  fTextButton680->SetCommand("UpdateConfig()");
  fGroupFrame679->AddFrame(fTextButton680, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,37,0));

  TGTextButton *bStartDAQ = new TGTextButton(fGroupFrame679,"Start DAQ");
  bStartDAQ->SetTextJustify(36);
  bStartDAQ->SetMargins(0,0,0,0);
  bStartDAQ->SetWrapLength(-1);
  bStartDAQ->Resize(123,22);
  bStartDAQ->SetCommand("if(RunOn==0) StartDAQ();");
  fGroupFrame679->AddFrame(bStartDAQ, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,2,2));


  TGTextButton *bStopDAQ = new TGTextButton(fGroupFrame679,"Stop DAQ");
  bStopDAQ->SetTextJustify(36);
  bStopDAQ->SetMargins(0,0,0,0);
  bStopDAQ->SetWrapLength(-1);
  bStopDAQ->Resize(123,22);
  bStopDAQ->SetCommand("RunOn=0; StopDAQ();");
  fGroupFrame679->AddFrame(bStopDAQ, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop | kLHintsExpandX,0,0,2,2));

  TGTextButton *bResetHistos = new TGTextButton(fGroupFrame679,"Reset Histos");
  bResetHistos->SetTextJustify(36);
  bResetHistos->SetMargins(0,0,0,0);
  bResetHistos->SetWrapLength(-1);
  bResetHistos->Resize(123,22);
  bResetHistos->SetCommand("Reset()");
  fGroupFrame679->AddFrame(bResetHistos, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));

  /*
  TGTextButton *bHVStatus = new TGTextButton(fGroupFrame679,"High Voltage",418);
  bHVStatus->SetTextJustify(36);
  bHVStatus->SetMargins(0,0,0,0);
  bHVStatus->SetWrapLength(-1);
  bHVStatus->Resize(123,22);
  bHVStatus->SetCommand("HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bHVStatus, new TGLayoutHints(kLHintsCenterX,10,10,10,10));
  */

  TGTextButton *fTextButton78 = new TGTextButton(fGroupFrame679,"SiPM HV ON");
  fTextButton78->SetTextJustify(36);
  fTextButton78->SetMargins(0,0,0,0);
  fTextButton78->SetWrapLength(-1);
  fTextButton78->Resize(123,22);
  fTextButton78->SetCommand("HVON()");
  fGroupFrame679->AddFrame(fTextButton78, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));

  TGTextButton *fTextButton88 = new TGTextButton(fGroupFrame679,"SiPM HV OFF");
  fTextButton88->SetTextJustify(36);
  fTextButton88->SetMargins(0,0,0,0);
  fTextButton88->SetWrapLength(-1);
  fTextButton88->Resize(123,22);
  fGroupFrame679->AddFrame(fTextButton88, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));
  fTextButton88->SetCommand("HVOF()");

  fNumberEntry755 = new TGNumberEntry(fGroupFrame679, (Double_t) 250,6,-1,(TGNumberFormat::EStyle) 5,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,1023);
  fGroupFrame679->AddFrame(fNumberEntry755, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,62,2));
  TGTextButton *bSetThresholdAll = new TGTextButton(fGroupFrame679,"Set Threshold all");
  bSetThresholdAll->SetTextJustify(36);
  bSetThresholdAll->SetMargins(0,0,0,0);
  bSetThresholdAll->SetWrapLength(-1);
  bSetThresholdAll->Resize(123,22);
  bSetThresholdAll->SetCommand("GUI_UpdateThresholdAll()");
  fGroupFrame679->AddFrame(bSetThresholdAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,2,2));

  fUpdateHisto = new TGCheckButton(fGroupFrame679,"Auto update histograms");
  fUpdateHisto->SetTextJustify(36);
  fUpdateHisto->SetMargins(0,0,0,0);
  fUpdateHisto->SetWrapLength(-1);
  fUpdateHisto->SetCommand("UpdateHisto()");
  fGroupFrame679->AddFrame(fUpdateHisto, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fUpdateHisto->SetOn();

  TGTextButton *bSaveDataTree = new TGTextButton(fGroupFrame679,"Save data tree");
  bSaveDataTree->SetTextJustify(36);
  bSaveDataTree->SetMargins(0,0,0,0);
  bSaveDataTree->SetWrapLength(-1);
  bSaveDataTree->Resize(123,22);
  bSaveDataTree->SetCommand("tr->SaveAs(\"mppc.root\");");
  fGroupFrame679->AddFrame(fTextButton111, new TGLayoutHints(kLHintsLeft| kLHintsCenterX  | kLHintsTop | kLHintsExpandX,0,0,2,2));

  // tab widget
  fTab683 = new TGTab(fHorizontalFrame768,1187,761+100);

  // container of "Configuration"
  TGCompositeFrame *fCompositeFrame686;
  fCompositeFrame686 = fTab683->AddTab("Configuration");
  fCompositeFrame686->SetLayoutManager(new TGHorizontalLayout(fCompositeFrame686));
  char str[32];

  TGVButtonGroup* fButtonGroup2=new TGVButtonGroup(fCompositeFrame686,"Enable Amplifier");
  for(int i=0;i<NumOfChannels;i++)
  {
    sprintf(str,"ch%d",i);
    fChanEnaAmp[i] = new TGCheckButton(fButtonGroup2,str);
    fChanEnaAmp[i]->SetTextJustify(36);
    fChanEnaAmp[i]->SetMargins(0,0,0,0);
    fChanEnaAmp[i]->SetWrapLength(-1);
    fChanEnaAmp[i]->SetCommand("SendConfig()");
  }
  fChanEnaAmp[NumOfChannels] = new TGCheckButton(fButtonGroup2,"All");
  fChanEnaAmp[NumOfChannels]->SetTextJustify(36);
  fChanEnaAmp[NumOfChannels]->SetMargins(0,0,0,0);
  fChanEnaAmp[NumOfChannels]->SetWrapLength(-1);
  fChanEnaAmp[NumOfChannels]->SetCommand("SendAllCheckedGSU()");
  fChanEnaAmp[NumOfChannels+1] = new TGCheckButton(fButtonGroup2,"None");
  fChanEnaAmp[NumOfChannels+1]->SetTextJustify(36);
  fChanEnaAmp[NumOfChannels+1]->SetMargins(0,0,0,0);
  fChanEnaAmp[NumOfChannels+1]->SetWrapLength(-1);
  fChanEnaAmp[NumOfChannels+1]->SetCommand("SendAllUnCheckedGSU()");

  fCompositeFrame686->AddFrame(fButtonGroup2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fButtonGroup2->Show();

  TGVButtonGroup* fButtonGroup3=new TGVButtonGroup(fCompositeFrame686,"Enable Trigger");
  for(int i=0;i<NumOfChannels;i++)
  {
    sprintf(str,"ch%d",i);
    fChanEnaTrig[i] = new TGCheckButton(fButtonGroup3,str);
    fChanEnaTrig[i]->SetTextJustify(36);
    fChanEnaTrig[i]->SetMargins(0,0,0,0);
    fChanEnaTrig[i]->SetWrapLength(-1);
    fChanEnaTrig[i]->SetCommand("SendConfig()");
  }
  fChanEnaTrig[NumOfChannels] = new TGCheckButton(fButtonGroup3,"OR32");
  fChanEnaTrig[NumOfChannels]->SetTextJustify(36);
  fChanEnaTrig[NumOfChannels]->SetMargins(0,0,0,0);
  fChanEnaTrig[NumOfChannels]->SetWrapLength(-1);
  fChanEnaTrig[NumOfChannels]->SetCommand("SendConfig()");

  fCompositeFrame686->AddFrame(fButtonGroup3, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  fButtonGroup3->Show();

  TGGroupFrame* fGains=new TGGroupFrame(fCompositeFrame686,"HG preamp gain/bias");
  fGains->SetLayoutManager(new TGMatrixLayout(fGains, 32, 3, 0, kLHintsLeft | kLHintsTop));
  for(int i=0;i<NumOfChannels;i++)
  {
    sprintf(str," CH %d",i);
    fChanGain[i] = new TGNumberEntry(fGains, (Double_t) i,2,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,64);
    fChanGain[i]->Connect("ValueSet(Long_t)", 0, 0,  "SendConfig()");
    fChanGain[i]->SetHeight(20);
    fChanBias[i] = new TGNumberEntry(fGains, (Double_t) i,3,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 1,(TGNumberFormat::ELimit) 2,0,256);
    fChanBias[i]->Connect("ValueSet(Long_t)", 0, 0,  "SendConfig()");
    fChanBias[i]->SetHeight(20);
    fGains->AddFrame(new TGLabel(fGains,str));
    fGains->AddFrame(fChanGain[i]);
    fGains->AddFrame(fChanBias[i]);

  }
  fCompositeFrame686->AddFrame(fGains, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


  // container of "All histos"
  TGCompositeFrame *fCompositeFrame720;
  fCompositeFrame720 = fTab683->AddTab("All histos");
  fCompositeFrame720->SetLayoutManager(new TGVerticalLayout(fCompositeFrame720));

  // embedded canvas
  TRootEmbeddedCanvas *fRootEmbeddedCanvas721 = new TRootEmbeddedCanvas(0,fCompositeFrame720,1179,732+100);
  Int_t wfRootEmbeddedCanvas721 = fRootEmbeddedCanvas721->GetCanvasWindowId();
  c = new TCanvas("c", 10, 10, wfRootEmbeddedCanvas721);    c->Divide(2,4);

  fRootEmbeddedCanvas721->AdoptCanvas(c);
  fCompositeFrame720->AddFrame(fRootEmbeddedCanvas721, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  fTab683->SetTab(2);

  fTab683->Resize(fTab683->GetDefaultSize());
  fHorizontalFrame768->AddFrame(fTab683, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  fVerticalFrame734->AddFrame(fHorizontalFrame768, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  fMainFrame1241->AddFrame(fVerticalFrame734, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));

  fMainFrame1560->AddFrame(fMainFrame1241, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
  fMainFrame1241->MoveResize(0,0,1329,789+100);

  fMainFrame1314->AddFrame(fMainFrame1560, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
  fMainFrame1560->MoveResize(0,0,1329,789+100);

  fMainFrame1314->SetMWMHints(kMWMDecorAll,
      kMWMFuncAll,
      kMWMInputModeless);
  fMainFrame1314->MapSubwindows();

  fMainFrame1314->Resize(fMainFrame1314->GetDefaultSize());
  fMainFrame1314->MapWindow();
  fMainFrame1314->Resize(1329,789+100);
}  

void HandleButtons(Int_t id)
{
   // Handle different buttons.

   if (id == -1) {
      TGButton *btn = (TGButton *) gTQSender;
      id = btn->WidgetId();
   }

   printf("DoButton: id = %d\n", id);
   string Radio_RunStart;
   string Radio_RunStop;
   string outputfile;

   switch (id) 
   {
     case 415:  // Start DAQ
       HV_On();
       if(bChanEnaAmp[0]->IsOn() || bChanEnaAmp[1]->IsOn() || bChanEnaAmp[2]->IsOn() || bChanEnaAmp[3]->IsOn() || bChanEnaAmp[4]->IsOn() || bChanEnaAmp[5]->IsOn() || bChanEnaAmp[6]->IsOn() || bChanEnaAmp[7]->IsOn())
       {
	 fFillHistos = kTRUE;
	 Time_Start = new TDatime();
	 Radio_RunStart = Form("Start Run at: %d:%d:%d",Time_Start->GetHour(),Time_Start->GetMinute(),Time_Start->GetSecond());
	 cout << Radio_RunStart.c_str() << endl;
	 FillHistos();
       }
       break;
     case 416:  // Stop DAQ
       fFillHistos = kFALSE;
       Time_Stop = new TDatime();
       Radio_RunStop = Form("Stop Run at: %d:%d:%d",Time_Stop->GetHour(),Time_Stop->GetMinute(),Time_Stop->GetSecond());
       cout << Radio_RunStop.c_str() << endl;
       HV_Off();
       break;
     case 417: // Rest Histograms
       ResetHistos();
       break;
     case 418: // HV status
       cout << "Do not play with High Voltage!!" << endl;
       break;
     case 419: // Save Data TTree
       if(!Time_Stop) Time_Stop = new TDatime();
       outputfile = Form("HCALTileTest_%d_%d.root",Time_Stop->GetDate(),Time_Stop->GetTime());
       cout << "Save data to " << outputfile.c_str() << endl;
       SaveDataTree(outputfile);
       break;
     case 420: // test run
       SendAllChecked(); // set configuration
       HV_On(); // turn on High Voltage

       mTab->SetTab(1); // switch to all histos tab 
       ResetHistos(); // reset histograms

       Time_Start = new TDatime(); // broadcast start time
       Radio_RunStart = Form("Start Test Run at: %d:%d:%d",Time_Start->GetHour(),Time_Start->GetMinute(),Time_Start->GetSecond());
       cout << Radio_RunStart.c_str() << endl;

       StartTime = gSystem->Now(); // get start time in milliseconds 
       TestRunTime = 60000; // in milliseconds
       ProcessTime(); // start taking data

       Time_Stop = new TDatime(); // broadcast stop time
       Radio_RunStop = Form("Stop Run at: %d:%d:%d",Time_Stop->GetHour(),Time_Stop->GetMinute(),Time_Stop->GetSecond());
       cout << Radio_RunStop.c_str() << endl;
       StopTime = gSystem->Now(); // get start time in milliseconds 
       // cout << "StartTime = " << StartTime << ", StopTime = " << StopTime << endl;
       cout << "time duration = " << (StopTime-StartTime)/1000.0 << endl;

       outputfile = Form("HCALTileTest_%d_%d.root",Time_Stop->GetDate(),Time_Stop->GetTime());
       cout << "Save data to " << outputfile.c_str() << endl;
       SaveDataTree(outputfile);
       break;
   }
}

void SendAllCheckedGSU()
{
  for(int i=0; i<NumOfChannels;i++)
  {
    fChanEnaAmp[i]->SetOn(); 
    ConfigSetBit(bufSCR,1144,633+i*15,0); 
  }

  SetDstMacByIndex(BoardToMon);  
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
  fChanEnaAmp[NumOfChannels+1]->SetOn(kFALSE);fChanEnaAmp[NumOfChannels]->SetOn(kFALSE);
}
void SendAllUnCheckedGSU()
{
  for(int i=0; i<NumOfChannels;i++)
  {
    fChanEnaAmp[i]->SetOn(kFALSE); 
    ConfigSetBit(bufSCR,1144,633+i*15,1); 
  }

  SetDstMacByIndex(BoardToMon);  
  t->SendCMD(t->dstmac,FEB_WR_SCR,0x0000,bufSCR);
  fChanEnaAmp[NumOfChannels+1]->SetOn(kFALSE);fChanEnaAmp[NumOfChannels]->SetOn(kFALSE);
}


