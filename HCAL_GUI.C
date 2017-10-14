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
#include "TGNumberEntry.h"
#include "TGLabel.h"

#define MAXPACKLEN 1500
#define NumOfChannels 8

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
    TGCompositeFrame  *fConfiguration;

    TGVButtonGroup    *bAmplifier;
    TGCheckButton     *bChanEnaAmp[10];

    TGVButtonGroup    *bTrigger;
    TGCheckButton     *bChanEnaTrig[NumOfChannels];

    TGGroupFrame      *fGainBias;
    TGHorizontalFrame *fLabel, *fGainBiasSub[NumOfChannels];
    TGNumberEntry     *nGain[NumOfChannels];
    TGNumberEntry     *nBias[NumOfChannels];
    TGLabel           *lChannel[NumOfChannels];
    TGLayoutHints     *lhGainBias;
    UChar_t bufPMR[MAXPACKLEN];
    UChar_t bufSCR[MAXPACKLEN];
    int Verbose=0;

    TGCompositeFrame *fAllHistos;
    TH1F *fHpx[NumOfChannels];
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

    void UpdateConfig();
    int ReadBitStream(const char * fname, UChar_t* buf);
    bool ConfigGetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index);
    UChar_t ConfigGetGain(int channel);
    UChar_t ConfigGetBias(int channel);
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
  for(int i_amp = 0; i_amp < NumOfChannels;++i_amp)
  {
    sprintf(str,"ch%d",i_amp);
    bChanEnaAmp[i_amp] = new TGCheckButton(bAmplifier,str);
    bChanEnaAmp[i_amp]->SetTextJustify(36);
    bChanEnaAmp[i_amp]->SetMargins(0,0,0,0);
    bChanEnaAmp[i_amp]->SetWrapLength(-1);
    // bChanEnaAmp[i]->SetCommand("SendConfig()");
  }
  bChanEnaAmp[NumOfChannels] = new TGCheckButton(bAmplifier,"All");
  bChanEnaAmp[NumOfChannels]->SetTextJustify(36);
  bChanEnaAmp[NumOfChannels]->SetMargins(0,0,0,0);
  bChanEnaAmp[NumOfChannels]->SetWrapLength(-1);
  bChanEnaAmp[NumOfChannels]->Connect("Clicked()","MyMainFrame",this,"SendAllChecked()");
  bChanEnaAmp[NumOfChannels+1] = new TGCheckButton(bAmplifier,"None");
  bChanEnaAmp[NumOfChannels+1]->SetTextJustify(36);
  bChanEnaAmp[NumOfChannels+1]->SetMargins(0,0,0,0);
  bChanEnaAmp[NumOfChannels+1]->SetWrapLength(-1);
  bChanEnaAmp[NumOfChannels+1]->Connect("Clicked()","MyMainFrame",this,"SendAllUnChecked()");

  fConfiguration->AddFrame(bAmplifier, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));
  bAmplifier->Show();
   

  bTrigger = new TGVButtonGroup(fConfiguration,"Enable Trigger");
  for(int i_trig = 0; i_trig < NumOfChannels; ++i_trig)
  {
    sprintf(str,"ch%d",i_trig);
    bChanEnaTrig[i_trig] = new TGCheckButton(bTrigger,str);
    bChanEnaTrig[i_trig]->SetTextJustify(36);
    bChanEnaTrig[i_trig]->SetMargins(0,0,0,0);
    bChanEnaTrig[i_trig]->SetWrapLength(-1);
    // bChanEnaTrig[i]->SetCommand("SendConfig()");
  }
   
  fConfiguration->AddFrame(bTrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));
  bTrigger->Show();

  fGainBias = new TGGroupFrame(fConfiguration,"default config");
  lhGainBias = new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,2,2,2,2);
  fLabel = new TGHorizontalFrame(fGainBias, 200, 30);
  fLabel->AddFrame(new TGLabel(fLabel,"    "),lhGainBias);
  fLabel->AddFrame(new TGLabel(fLabel,"gain"),lhGainBias);
  fLabel->AddFrame(new TGLabel(fLabel,"bias"),lhGainBias);
  fGainBias->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,2,2));

  for(int i_num = 0; i_num < NumOfChannels; ++i_num)
  {
    sprintf(str,"ch%d",i_num);
    fGainBiasSub[i_num] = new TGHorizontalFrame(fGainBias, 200, 30);
    lChannel[i_num] = new TGLabel(fGainBiasSub[i_num],str);

    nGain[i_num] = new TGNumberEntry(fGainBiasSub[i_num], (Double_t)i_num,3,-1,(TGNumberFormat::EStyle)0,(TGNumberFormat::EAttribute)1,(TGNumberFormat::ELimit)2,0,64);
    nGain[i_num]->SetHeight(20);

    nBias[i_num] = new TGNumberEntry(fGainBiasSub[i_num],(Double_t)i_num,3,-1,(TGNumberFormat::EStyle)0,(TGNumberFormat::EAttribute)1,(TGNumberFormat::ELimit)2,0,256);
    nBias[i_num]->SetHeight(20);

    fGainBiasSub[i_num]->AddFrame(lChannel[i_num],lhGainBias);
    fGainBiasSub[i_num]->AddFrame(nGain[i_num],lhGainBias);
    fGainBiasSub[i_num]->AddFrame(nBias[i_num],lhGainBias);
    fGainBias->AddFrame(fGainBiasSub[i_num], new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,2,2,2,2));
  }

  fConfiguration->AddFrame(fGainBias, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,2,2,2,2));


  // container of "All histos"
  fAllHistos = mTab->AddTab("All histos");
  fAllHistos->SetLayoutManager(new TGVerticalLayout(fAllHistos));

  // embedded canvas
  fEcanvas = new TRootEmbeddedCanvas("ECanvas",fAllHistos,800,400);
  int fEcanvasID = fEcanvas->GetCanvasWindowId();
  c_Histo = new TCanvas("c_Histo",10,10,fEcanvasID);
  c_Histo->Divide(4,2);
  for(int i_pad = 0; i_pad < NumOfChannels; ++i_pad)
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
  for(int i=0; i<NumOfChannels;i++)
  {
     bChanEnaAmp[i]->SetOn(); 
  }
  bChanEnaAmp[9]->SetOn(kFALSE);bChanEnaAmp[NumOfChannels]->SetOn(kFALSE);
}

void MyMainFrame::SendAllUnChecked()
{
  for(int i=0; i<NumOfChannels;i++)
  {
     bChanEnaAmp[i]->SetOn(kFALSE); 
  }
  bChanEnaAmp[9]->SetOn(kFALSE);bChanEnaAmp[NumOfChannels]->SetOn(kFALSE);
}

void MyMainFrame::HandleButtons(Int_t id)
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

  static int cnt[NumOfChannels];

  if (!isFilled) 
  {
    for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp )
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
    for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp )
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
    for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp )
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
  for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp)
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

  static int cnt[NumOfChannels];

  if (!isFilled) 
  {
    for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp )
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
    for(int i_Amp = 0; i_Amp < NumOfChannels; ++i_Amp )
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

    long ContTime = gSystem->Now();
    ContTime -= StartTime; // in second
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

int MyMainFrame::ReadBitStream(const char * fname, UChar_t* buf) // read CITIROC SC bitstream into the buffer, buf[MAXPACKLEN]
{
  FILE *file = fopen(fname, "r");
  if(file<=0) return 0;
  char line[128];
  char bit;
  int ptr, byteptr;
  int bitlen=0;
  char ascii[MAXPACKLEN];
  while (fgets(line, sizeof(line), file)) {
    bit=1; ptr=0; byteptr=0;
    // printf("%d: %s",bitlen,line);

    while(bit!=0x27 && bit!=0 && ptr<sizeof(line) && bitlen<MAXPACKLEN) // ASCII(0x27)= '
    {
      bit=line[ptr];
      ptr++;
      if(bit==0x20 || bit==0x27) continue; //ignore spaces and apostrophe
      if(Verbose) printf("%c",bit);
      ascii[bitlen]=bit;
      bitlen++;
    }
    //  printf("\n");
  }
  printf("HCAL_GUI::ReadBitStream: %d bits read from file %s.\n",bitlen,fname);
  fclose(file);
  memset(buf,0,MAXPACKLEN); //reset buffer
  // Now encode ASCII bitstream into binary
  for(ptr=bitlen-1;ptr>=0;ptr--)
  {
    byteptr=(bitlen-ptr-1)/8;
    if(ascii[ptr]=='1')  buf[byteptr] |= (1 << (7-ptr%8));
    //   if((ptr%8)==0) printf("bitpos=%d buf[%d]=%02x\n",ptr,byteptr,buf[byteptr]);
  }
  return bitlen;
}

bool MyMainFrame::ConfigGetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index)
{
  UChar_t byte;
  UChar_t mask;
  byte=buffer[(bitlen-1-bit_index)/8];
  mask= 1 << (7-bit_index%8);
  byte=byte & mask;
  if(byte!=0) return true; else return false;
}

UChar_t MyMainFrame::ConfigGetGain(int channel)
{
  UChar_t val=0;
  for(int b=0;b<6;b++)
  {
    val=val << 1;
    if(ConfigGetBit(bufSCR,1144,619+channel*15+b)) 
    {
      val=val+1;
    }
  }
  return val;
}

UChar_t MyMainFrame::ConfigGetBias(int channel)
{
  UChar_t val=0;
  for(int b=0;b<8;b++)
  {
    val=val << 1;
    if(ConfigGetBit(bufSCR,1144,331+channel*9+b)) val=val+1;
  }
  return val;
}

void MyMainFrame::UpdateConfig()
{
  char bsname[32];
  // cout << "Open file: CITIROC_PROBEbitstream.txt" << endl;
  // cout << endl;
  // this->ReadBitStream("CITIROC_PROBEbitstream.txt",bufPMR);
  // if(!(this->ReadBitStream(bsname,bufSCR))) this->ReadBitStream("CITIROC_SC_DEFAULT.txt",bufSCR);
  if(!(this->ReadBitStream(bsname,bufSCR))) 
  {
    cout << "Open file: CITIROC_SC_PROFILE1.txt" << endl;
    this->ReadBitStream("CITIROC_SC_PROFILE1.txt",bufSCR);
  }
  // printf("%s \n",bufSCR);
  for(int i=0; i<NumOfChannels;i++) cout << "gain of channle "  << i << " is " << (int)ConfigGetGain(i) << endl;
  for(int i=0; i<NumOfChannels;i++) cout << "bias of channle " << i << " is " << (int)ConfigGetBias(i) << endl;
  for(int i=0; i<NumOfChannels;i++) nGain[i]->SetNumber(ConfigGetGain(i));
  for(int i=0; i<NumOfChannels;i++) nBias[i]->SetNumber(ConfigGetBias(i));
}

void HCAL_GUI() {
  // Popup the GUI...
  MyMainFrame *myGUI = new MyMainFrame(gClient->GetRoot(),800,800);
  myGUI->UpdateConfig();
}
