#include <RQ_OBJECT.h>
#include "TGClient.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TRandom.h"
#include "TGButton.h"
#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1F.h"
#include "TDatime.h"
#include "TTimer.h"
#include "TGProgressBar.h"

class MyMainFrame {
  RQ_OBJECT("MyMainFrame")
  private:
    TGMainFrame *fMain;
    TGHorizontalFrame *hframe;
    TRootEmbeddedCanvas *fEcanvas;

    TGGroupFrame *fGroupFrame;
    TGTextButton *bStartDAQ;
    TGTextButton *bStopDAQ;
    TGTextButton *bResetHistos;
    TGTextButton *bHVStatus;
    TGTextButton *bSaveDataTree;
    TGTextButton *bTestRun;

    TGTab *mTab;
    TGCompositeFrame *fConfiguration;
    TGVButtonGroup *bAmplifier;
    TGCheckButton *bChanEnaAmp[10];
    TGVButtonGroup *bTrigger;
    TGCheckButton *bChanEnaTrig[10];
    TGCompositeFrame *fAllHistos;
    TH1F *fHpx[8] = NULL;
    TCanvas *c_Histo;
    bool isFilled;
    bool fFillHistos;

    TDatime *Time_Start;
    TDatime *Time_Stop;
    long StartTime;
    long StopTime;
    long TestRunTime; // 60s => 1min
    TFile *File_OutPut;

    TGHProgressBar *ProgTimer;

    bool fClose;

  public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();

    // slots
    void HandleButtons(Int_t id = -1);
    void SendAllChecked();
    void SendAllUnChecked();
    void DoClose();

    // functions
    void HV_On();
    void HV_Off();
    void FillHistos();
    void ResetHistos();
    void SaveDataTree(string outputfile = "test.root");
    void ProcessTime();
    void CloseWindow();
};


MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
{
  fClose = kTRUE;
  isFilled = kFALSE;

  fMain = new TGMainFrame(p,w,h); // main frame
  fMain->SetName("HCAL_GUI_Test");
  fMain->Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");

  hframe = new TGHorizontalFrame(fMain,200,200);

  //---------------DAQ Control---------------------
  fGroupFrame = new TGGroupFrame(hframe,"DAQ FEB controls");

  bStartDAQ = new TGTextButton(fGroupFrame,"Start DAQ",415);
  bStartDAQ->Connect("Clicked()","MyMainFrame", this, "HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bStartDAQ, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  bStopDAQ = new TGTextButton(fGroupFrame,"Stop DAQ",416);
  bStopDAQ->Connect("Clicked()","MyMainFrame", this, "HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bStopDAQ, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  bResetHistos = new TGTextButton(fGroupFrame,"Reset Histos",417);
  bResetHistos->Connect("Clicked()","MyMainFrame",this,"HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bResetHistos, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  bHVStatus = new TGTextButton(fGroupFrame,"High Voltage",418);
  bHVStatus->Connect("Clicked()","MyMainFrame",this,"HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bHVStatus, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  bSaveDataTree = new TGTextButton(fGroupFrame,"Save Data TTree",419);
  bSaveDataTree->Connect("Clicked()","MyMainFrame",this,"HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bSaveDataTree, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  bTestRun = new TGTextButton(fGroupFrame,"Test Run",420);
  bTestRun->Connect("Clicked()","MyMainFrame",this,"HandleButtons(Int_t)");
  fGroupFrame->AddFrame(bTestRun, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  ProgTimer = new TGHProgressBar(fGroupFrame,100);
  ProgTimer->ShowPosition();
  fGroupFrame->AddFrame(ProgTimer, new TGLayoutHints(kLHintsCenterX,10,10,10,10));

  hframe->AddFrame(fGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY,2,2,2,2));
  //-----------------------------------------------

  //---------------Histograms Display---------------------
  mTab = new TGTab(hframe,800,400);

  // container of "Configuration"
  fConfiguration = mTab->AddTab("Configuration");
  fConfiguration->SetLayoutManager(new TGHorizontalLayout(fConfiguration));

  bAmplifier = new TGVButtonGroup(fConfiguration,"Enable Amplifier");
  char str[32];
  for(int i=0;i<8;i++)
  {
    sprintf(str,"ch%d",i);
    bChanEnaAmp[i] = new TGCheckButton(bAmplifier,str);
    bChanEnaAmp[i]->SetTextJustify(36);
    bChanEnaAmp[i]->SetMargins(0,0,0,0);
    bChanEnaAmp[i]->SetWrapLength(-1);
    // bChanEnaAmp[i]->SetCommand("SendConfig()");
  }
  bChanEnaAmp[8] = new TGCheckButton(bAmplifier,"All");
  bChanEnaAmp[8]->SetTextJustify(36);
  bChanEnaAmp[8]->SetMargins(0,0,0,0);
  bChanEnaAmp[8]->SetWrapLength(-1);
  bChanEnaAmp[8]->Connect("Clicked()","MyMainFrame",this,"SendAllChecked()");
  bChanEnaAmp[9] = new TGCheckButton(bAmplifier,"None");
  bChanEnaAmp[9]->SetTextJustify(36);
  bChanEnaAmp[9]->SetMargins(0,0,0,0);
  bChanEnaAmp[9]->SetWrapLength(-1);
  bChanEnaAmp[9]->Connect("Clicked()","MyMainFrame",this,"SendAllUnChecked()");

  fConfiguration->AddFrame(bAmplifier, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));
  bAmplifier->Show();
   

  bTrigger = new TGVButtonGroup(fConfiguration,"Enable Trigger");
  for(int i=0;i<8;i++)
  {
    sprintf(str,"ch%d",i);
    bChanEnaTrig[i] = new TGCheckButton(bTrigger,str);
    bChanEnaTrig[i]->SetTextJustify(36);
    bChanEnaTrig[i]->SetMargins(0,0,0,0);
    bChanEnaTrig[i]->SetWrapLength(-1);
    // bChanEnaTrig[i]->SetCommand("SendConfig()");
  }
   
  fConfiguration->AddFrame(bTrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));
  bTrigger->Show();


  // container of "All histos"
  fAllHistos = mTab->AddTab("All histos");
  fAllHistos->SetLayoutManager(new TGVerticalLayout(fAllHistos));

  // embedded canvas
  fEcanvas = new TRootEmbeddedCanvas("ECanvas",fAllHistos,800,400);
  int fEcanvasID = fEcanvas->GetCanvasWindowId();
  c_Histo = new TCanvas("c_Histo",10,10,fEcanvasID);
  c_Histo->Divide(4,2);
  for(int i_pad = 0; i_pad < 8; ++i_pad)
  {
    c_Histo->cd(i_pad+1);
    c_Histo->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Histo->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Histo->cd(i_pad+1)->SetTicks(1,1);
    c_Histo->cd(i_pad+1)->SetGrid(0,0);
    // c_Histo->cd(i_pad+1)->SetLogy();
  }
  fEcanvas->AdoptCanvas(c_Histo);

  fAllHistos->AddFrame(fEcanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  // mTab->SetTab(1);

  hframe->AddFrame(mTab, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));
  //-------------------------------------------------------

  fMain->AddFrame(hframe, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

  fMain->MapSubwindows();
  fMain->Resize(fMain->GetDefaultSize());
  fMain->MapWindow();
}

MyMainFrame::~MyMainFrame() {
  // Clean up used widgets: frames, buttons, layout hints
  fMain->Cleanup();
  delete fMain;
}

void MyMainFrame::SendAllChecked()
{
  for(int i=0; i<8;i++)
  {
     bChanEnaAmp[i]->SetOn(); 
  }
  bChanEnaAmp[9]->SetOn(kFALSE);bChanEnaAmp[8]->SetOn(kFALSE);
}

void MyMainFrame::SendAllUnChecked()
{
  for(int i=0; i<8;i++)
  {
     bChanEnaAmp[i]->SetOn(kFALSE); 
  }
  bChanEnaAmp[9]->SetOn(kFALSE);bChanEnaAmp[8]->SetOn(kFALSE);
}

void MyMainFrame::HandleButtons(Int_t id)
{
   // Handle different buttons.

   if (id == -1) {
      TGButton *btn = (TGButton *) gTQSender;
      id = btn->WidgetId();
   }

   printf("DoButton: id = %d\n", id);

   switch (id) 
   {
     case 415:  // Start DAQ
       HV_On();
       if(bChanEnaAmp[0]->IsOn() || bChanEnaAmp[1]->IsOn() || bChanEnaAmp[2]->IsOn() || bChanEnaAmp[3]->IsOn() || bChanEnaAmp[4]->IsOn() || bChanEnaAmp[5]->IsOn() || bChanEnaAmp[6]->IsOn() || bChanEnaAmp[7]->IsOn())
       {
	 fFillHistos = kTRUE;
	 Time_Start = new TDatime();
	 string Radio_RunStart = Form("Start Run at: %d:%d:%d",Time_Start->GetHour(),Time_Start->GetMinute(),Time_Start->GetSecond());
	 cout << Radio_RunStart.c_str() << endl;
	 FillHistos();
       }
       break;
     case 416:  // Stop DAQ
       fFillHistos = kFALSE;
       Time_Stop = new TDatime();
       string Radio_RunStop = Form("Stop Run at: %d:%d:%d",Time_Stop->GetHour(),Time_Stop->GetMinute(),Time_Stop->GetSecond());
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
       string outputfile = Form("HCALTileTest_%d_%d.root",Time_Stop->GetDate(),Time_Stop->GetTime());
       cout << "Save data to " << outputfile.c_str() << endl;
       SaveDataTree(outputfile);
       break;
     case 420: // test run
       SendAllChecked(); // set configuration
       HV_On(); // turn on High Voltage

       mTab->SetTab(1); // switch to all histos tab 
       ResetHistos(); // reset histograms

       Time_Start = new TDatime(); // broadcast start time
       string Radio_RunStart = Form("Start Test Run at: %d:%d:%d",Time_Start->GetHour(),Time_Start->GetMinute(),Time_Start->GetSecond());
       cout << Radio_RunStart.c_str() << endl;

       StartTime = gSystem->Now(); // get start time in milliseconds 
       TestRunTime = 60000; // in milliseconds
       ProcessTime(); // start taking data

       Time_Stop = new TDatime(); // broadcast stop time
       string Radio_RunStop = Form("Stop Run at: %d:%d:%d",Time_Stop->GetHour(),Time_Stop->GetMinute(),Time_Stop->GetSecond());
       cout << Radio_RunStop.c_str() << endl;
       StopTime = gSystem->Now(); // get start time in milliseconds 
       // cout << "StartTime = " << StartTime << ", StopTime = " << StopTime << endl;
       cout << "time duration = " << (StopTime-StartTime)/1000.0 << endl;

       string outputfile = Form("HCALTileTest_%d_%d.root",Time_Stop->GetDate(),Time_Stop->GetTime());
       cout << "Save data to " << outputfile.c_str() << endl;
       SaveDataTree(outputfile);
       break;
   }
}

void MyMainFrame::HV_On()
{
  Pixel_t green;
  gClient->GetColorByName("green", green);
  bHVStatus->ChangeBackground(green);
}

void MyMainFrame::HV_Off()
{
  Pixel_t white;
  gClient->GetColorByName("white", white);
  bHVStatus->ChangeBackground(white);
}

void MyMainFrame::FillHistos()
{
  // Fill histograms on selected channel till user clicks "Stop Filling" button.

  static int cnt[8];

  if (!isFilled) 
  {
    for(int i_Amp = 0; i_Amp < 8; ++i_Amp )
    {
      string HistName = Form("hpx_%d",i_Amp);
      cout << "Declare Histograms: " << HistName.c_str() << endl;
      fHpx[i_Amp] = new TH1F(HistName.c_str(),"This is the px distribution",100,-4,4);
      fHpx[i_Amp]->SetFillColor(1+i_Amp);
      fHpx[i_Amp]->Reset();
      c_Histo->cd(i_Amp+1);
      fHpx[i_Amp]->Draw();
      c_Histo->cd(i_Amp+1)->Update();
      cnt[i_Amp] = 0;
    }
    isFilled = kTRUE;
  }

  const int kUPDATE = 1000;
  float px, py;

  while (fFillHistos) 
  {
    for(int i_Amp = 0; i_Amp < 8; ++i_Amp )
    {
      if(bChanEnaAmp[i_Amp]->IsOn())
      {
	gRandom->Rannor(px,py); //px and py will be two gaussian random numbers
	cnt[i_Amp]++;
	fHpx[i_Amp]->Fill(px);
	if (!(cnt[i_Amp] % kUPDATE)) 
	{
	  if (cnt[i_Amp] == kUPDATE) 
	  {
	    c_Histo->cd(i_Amp+1);
	    fHpx[i_Amp]->Draw();
	    // fHpxpy->Draw("cont");
	  }
	  c_Histo->cd(i_Amp+1)->Modified();
	  c_Histo->cd(i_Amp+1)->Update();
	  gSystem->ProcessEvents();  // handle GUI events
	}
      }
    }
  }
}

void MyMainFrame::ResetHistos()
{
  if(isFilled)
  {
    for(int i_Amp = 0; i_Amp < 8; ++i_Amp )
    {
      fHpx[i_Amp]->Reset();
      c_Histo->cd(i_Amp+1);
      fHpx[i_Amp]->Draw();
      c_Histo->cd(i_Amp+1)->Update();
    }
  }
}

void MyMainFrame::SaveDataTree(string outputfile)
{
  File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
  File_OutPut->cd();
  for(int i_Amp = 0; i_Amp < 8; ++i_Amp)
  {
    if(fHpx[i_Amp]) fHpx[i_Amp]->Write();
  }
  File_OutPut->Close();
}

void MyMainFrame::CloseWindow()
{
  // Called when window is closed via the window manager.

  gApplication->Terminate();
}

void MyMainFrame::DoClose()
{
  // If fClose is false we are still in event processing loop in DoGo().
  // In that case, set the close flag true and use a timer to call
  // CloseWindow(). This gives us change to get out of the DoGo() loop.
  // Note: calling SendCloseMessage() will not work since that will
  // bring us back here (CloseWindow() signal is connected to this method)
  // with the fClose flag true, which will cause window deletion while
  // still being in the event processing loop (since SendCloseMessage()
  // is directly processed in ProcessEvents() without exiting DoGo()).

  if (fClose)
    CloseWindow();
  else {
    fClose = kTRUE;
    TTimer::SingleShot(150, "MyMainFrame", this, "CloseWindow()");
  }
}

void MyMainFrame::ProcessTime()
{
  // Handle TextRun button.
  // Fill histograms on selected channel till user clicks "Stop Filling" button.

  static int cnt[8];

  if (!isFilled) 
  {
    for(int i_Amp = 0; i_Amp < 8; ++i_Amp )
    {
      string HistName = Form("hpx_%d",i_Amp);
      cout << "Declare Histograms: " << HistName.c_str() << endl;
      fHpx[i_Amp] = new TH1F(HistName.c_str(),"This is the px distribution",100,-4,4);
      fHpx[i_Amp]->SetFillColor(1+i_Amp);
      fHpx[i_Amp]->Reset();
      c_Histo->cd(i_Amp+1);
      fHpx[i_Amp]->Draw();
      c_Histo->cd(i_Amp+1)->Update();
      cnt[i_Amp] = 0;
    }
    isFilled = kTRUE;
  }

  const int kUPDATE = 1;
  float px, py;

  fClose = kFALSE;
  float counter = 0;
  float inc = 1;
  ProgTimer->Reset();
  ProgTimer->SetBarColor("blue");
  fFillHistos = kTRUE;
  cout << "TestRunTime = " << TestRunTime << endl;
  long const DeltaTime = TestRunTime/100;
  cout << "DeltaTime = " << DeltaTime << endl;

  while (fFillHistos) 
  {
    for(int i_Amp = 0; i_Amp < 8; ++i_Amp )
    {
      if(bChanEnaAmp[i_Amp]->IsOn())
      {
	gRandom->Rannor(px,py); //px and py will be two gaussian random numbers
	cnt[i_Amp]++;
	fHpx[i_Amp]->Fill(px);
	if (!(cnt[i_Amp] % kUPDATE)) 
	{
	  if (cnt[i_Amp] == kUPDATE) 
	  {
	    c_Histo->cd(i_Amp+1);
	    fHpx[i_Amp]->Draw();
	    // fHpxpy->Draw("cont");
	  }
	  c_Histo->cd(i_Amp+1)->Modified();
	  c_Histo->cd(i_Amp+1)->Update();
	}
      }
    }
    gSystem->ProcessEvents();  // handle GUI events

    long ContTime = gSystem->Now() - StartTime; // in second
    if( ContTime/DeltaTime > counter ) // update every DeltaTime
    {
      cout << "ContTime = " << ContTime/1000.0 << ", counter = " << counter << endl;
      counter += inc;
      ProgTimer->Increment(inc);
    }

    if(counter > 100)
    {
      fFillHistos = kFALSE;
      HV_Off();
    }

    // if user closed window return
    if (fClose) 
    {
      cout << "stopping..." << endl;
      return;
    }
  }
  fClose = kTRUE;
  return;
}

void HCAL_GUI() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),800,800);
}
