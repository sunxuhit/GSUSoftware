#include <iostream>
#include "../../include/BeamTest/Calibration.h"
#include "../../include/BeamTest/PixelMap.h"

// Calibration::Calibration(string outputfile)
Calibration::Calibration()
{
  cout << "Calibration::Calibration() ----- Constructor ! ------" << endl;
  // mOutPutFile = outputfile;
}

Calibration::~Calibration()
{
  cout << "Calibration::~Calibration() ----- Release memory ! ------" << endl;
}

int Calibration::Init()
{
  cout << "Calibration::Init(), create output file: "<< mOutPutFile.c_str() <<endl;
  // mFile_OutPut = new TFile(mOutPutFile.c_str(),"RECREATE");
  InitChain();

  return 0;
}

int Calibration::InitChain()
{
 string inputdir = "/Users/xusun/WorkSpace/EICPID/Data/BeamTest_mRICH/tdc/";
 string InPutList = "/Users/xusun/WorkSpace/EICPID/BeamTest_mRICH/list/PMT/Calibration/proton_calibration_1.list";

 mChainInPut = new TChain("data");

 if (!InPutList.empty())   // if input file is ok
 {
   cout << "Open input probability file list" << endl;
   ifstream in(InPutList.c_str());  // input stream
   if(in)
   {
     cout << "input file probability list is ok" << endl;
     char str[255];       // char array for each file name
     Long64_t entries_save = 0;
     while(in)
     {
       in.getline(str,255);  // take the lines of the file list
       if(str[0] != 0)
       {
	 string addfile;
	 addfile = str + "/sspRich.root";
	 addfile = inputdir+addfile;
	 mChainInPut->AddFile(addfile.c_str(),-1,"data");
	 Long64_t file_entries = mChainInPut->GetEntries();
	 cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	 entries_save = file_entries;
       }
     }
   }
   else
   {
     cout << "WARNING: input probability file input is problemtic" << endl;
   }
 }

 mChainInPut->SetBranchAddress("evt",&tTrigNum);
 mChainInPut->SetBranchAddress("trigtime",&tTrigTime);
 mChainInPut->SetBranchAddress("nedge",&tNedge);
 mChainInPut->SetBranchAddress("slot",tSlot);
 mChainInPut->SetBranchAddress("fiber",tFiber);
 mChainInPut->SetBranchAddress("ch",tChannel);
 mChainInPut->SetBranchAddress("pol",tPolarity);
 mChainInPut->SetBranchAddress("time",tTime);

 long NumOfEvents = (long)mChainInPut->GetEntries();
 cout << "total number of events: " << NumOfEvents << endl;

 return 0;
}

int Calibration::Make()
{
  cout << " this is Calibration::Make" << endl;

  return 0;
}

int Calibration::Finish()
{
  cout << " this is Calibration::Finish" << endl;

  return 0;
}
