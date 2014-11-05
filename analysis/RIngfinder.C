#include "TFile.h"
#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TVector3.h"
#include "TMath.h"

#include <vector>
#include <iostream>

using namespace std;

//Define a function which computes the center of the ring from 3 points.
//X:
double RFindX (double X1, double Y1, double X2, double Y2, double X3, double Y3)
{
  double D = 2*(X1*(Y2-Y3)+X2*(Y3-Y1)+X3*(Y1-Y2));
  double x = ((X1*X1+Y1*Y1)*(Y2-Y3)+(X2*X2+Y2*Y2)*(Y3-Y1)+(X3*X3+Y3*Y3)*(Y1-Y2))/D;
  return x;
}

//Y:
double RFindY (double X1, double Y1, double X2, double Y2, double X3, double Y3)
{
  double D = 2*(X1*(Y2-Y3)+X2*(Y3-Y1)+X3*(Y1-Y2));
  double y = ((X1*X1+Y1*Y1)*(X3-X2)+(X2*X2+Y2*Y2)*(X1-X3)+(X3*X3+Y3*Y3)*(X2-X1))/D;
  return y;
}

//Define a function which computes the radius of the ring from 3 points.
double RFindR (TVector3 P1, TVector3 P2, TVector3 P3)
{
  double r = ((P1-P2).Mag()*(P2-P3).Mag()*(P3-P1).Mag())/(2*((P1-P2).Cross(P2-P3)).Mag());
  return r;
}



void Ringfinder(string inputfile)
{
gROOT->Reset();
gStyle->SetPalette(1);
gStyle->SetOptStat(0);

char thefile[200];
sprintf(thefile, "%s",inputfile.substr(0,inputfile.rfind(".")).c_str());

char outputfile[200];
sprintf(outputfile, "%s_ringht.root",thefile);

TFile *ff=new TFile(inputfile.c_str());
TH2D *hPhotoDetXY = (TH2D*) ff->Get("hPhotoDetXY");

  std::vector<TVector3> PseudoPoints;
  std::vector<TVector3> J1_PseudoPoints;
  std::vector<TVector3> J2_PseudoPoints;
  std::vector<TVector3> J3_PseudoPoints;
  std::vector<TVector3> J4_PseudoPoints;
  std::vector<double> Y1;
  std::vector<double> X1;
  std::vector<double> Y2;
  std::vector<double> X2;
  std::vector<double> Y3;
  std::vector<double> X3;
  std::vector<double> Y4;
  std::vector<double> X4;
  std::vector<double> AllX;
  std::vector<double> AllY;
  std::vector<double> CX;
  std::vector<double> CY;

  Y1.clear();
  X1.clear();
  Y2.clear();
  X2.clear();
  Y3.clear();
  X3.clear();
  Y4.clear();
  X4.clear();
  AllX.clear();
  AllY.clear();
  PseudoPoints.clear();
  J1_PseudoPoints.clear();
  J2_PseudoPoints.clear();
  J3_PseudoPoints.clear();
  J4_PseudoPoints.clear();
  CX.clear();
  CY.clear();

  int nbins=hPhotoDetXY->GetXaxis()->GetNbins();
  TH3D *HoughF = new TH3D ("HoughF","HoughF",88,-44.,44.,88,-44,44,50,0,50);;
  TH1D *xerr, *yerr, *rerr;

  static int first=1;
     if (first)
       {
         first = 0;      
	 xerr = new TH1D ("xerr","xerr",200,-5,5);
         yerr = new TH1D ("yerr","yerr",200,-5,5);
         rerr = new TH1D ("rerr","rerr",200,-3,3);
       }
  
  HoughF->Reset();
  
 
  double binw=88./nbins;   /// bin width for photonsensor
 
  //J1 section:--------------------X < 0 && Y < 0---------------------------
  for(unsigned int i=0; i<nbins/2; i++)   /// X axis
  {
	  for(unsigned int j=0; j<nbins/2; j++) /// Y axis
	  {
		  double weight = hPhotoDetXY->GetBinContent(i+1,j+1);
		  if(weight>0){
			  Y1.push_back((-44.+binw*(j+0.5)));
			  X1.push_back((-44.+binw*(i+0.5)));
		  }
	  }
  }
  cout<< "J1 hits collection done !" <<endl;

  //J2 section:--------------------X > 0 && Y < 0---------------------------
  for(unsigned int i=nbins/2; i<nbins; i++)   /// X axis
  {
	  for(unsigned int j=0; j<nbins/2; j++) /// Y axis
	  {
		  double weight = hPhotoDetXY->GetBinContent(i+1,j+1);
		  if(weight>0){
			  Y2.push_back((-44.+binw*(j+0.5)));
			  X2.push_back((-44.+binw*(i+0.5)));
		  }
	  }
  }
  cout<< "J2 hits collection done!" <<endl;

  //J3 section:--------------------X < 0 && Y > 0---------------------------
  for(unsigned int i=0; i<nbins/2.; i++)   /// X axis
  {
	  for(unsigned int j=nbins/2; j<nbins; j++) /// Y axis
	  {
		  double weight = hPhotoDetXY->GetBinContent(i+1,j+1);
		  if(weight>0){
			  Y3.push_back((-44.+binw*(j+0.5)));
			  X3.push_back((-44.+binw*(i+0.5)));
		  }
	  }
  }
  cout<< "J3 hits collection done!" <<endl;

  //J4 section:--------------------X > 0 && Y > 0---------------------------
  for(unsigned int i=nbins/2; i<nbins; i++)   /// X axis
  {
	  for(unsigned int j=nbins/2; j<nbins; j++) /// Y axis
	  {
		  double weight = hPhotoDetXY->GetBinContent(i+1,j+1);
		  if(weight>0){
			  Y4.push_back((-44.+binw*(j+0.5)));
			  X4.push_back((-44.+binw*(i+0.5)));
		  }
	  }
  }
  cout<< "J4 hits collection done!" <<endl;

  cout<< "J1 size: "<<X1.size()<<" X "<<Y1.size()<<"\t J2 size: "<<X2.size()<<" X "<<Y2.size()
      << "\t J3 size: "<<X3.size()<<" X "<<Y3.size()<<"\t J4 size: "<<X4.size()<<" X "<<Y4.size()<<endl;

  if(X1.size()!=Y1.size() || X2.size()!=Y2.size() || X3.size()!=Y3.size() || X4.size()!=Y4.size())
  cout<<"fatal error happened !!!"<<endl;
//Finding the ring:----------------------------------------------------------------------------------

//Forming J1234 PseudoPoints:
   
  for(unsigned int s=0; s<Y1.size(); s++) J1_PseudoPoints.push_back(TVector3(X1[s],Y1[s],0));
  for(unsigned int s=0; s<Y2.size(); s++) J2_PseudoPoints.push_back(TVector3(X2[s],Y2[s],0));
  for(unsigned int s=0; s<Y3.size(); s++) J3_PseudoPoints.push_back(TVector3(X3[s],Y3[s],0));
  for(unsigned int s=0; s<Y4.size(); s++) J4_PseudoPoints.push_back(TVector3(X4[s],Y4[s],0));

  cout<< "J1 PseudoPoints size: "<<J1_PseudoPoints.size()<<"\t J2 PseudoPoints size: "<<J2_PseudoPoints.size()
      << "\t J3 PseudoPoints size: "<<J3_PseudoPoints.size()<<"\t J4 PseudoPoints size: "<<J4_PseudoPoints.size()<<endl;
 
  //Selecting 3 PseudoPoints in three different quadrants:
  //J1,J2,J3:
  cout<< "Looking for rings in (J1 J2 J3) ...." <<endl;
  if(J1_PseudoPoints.size() >= 1 && J2_PseudoPoints.size() >= 1 && J3_PseudoPoints.size() >= 1)
    {
      for(unsigned int i=0; i<J1_PseudoPoints.size(); i++)
	{
	  for(unsigned int j=0; j<J2_PseudoPoints.size(); j++)
	    {
	      for(unsigned int k=0; k<J3_PseudoPoints.size(); k++)
		{
		  
		  double Rad  = RFindR(J1_PseudoPoints[i],J2_PseudoPoints[j],J3_PseudoPoints[k]);
		  double Xcen = RFindX(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J2_PseudoPoints[j].X(),J2_PseudoPoints[j].Y(),J3_PseudoPoints[k].X(),J3_PseudoPoints[k].Y());
		  double Ycen = RFindY(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J2_PseudoPoints[j].X(),J2_PseudoPoints[j].Y(),J3_PseudoPoints[k].X(),J3_PseudoPoints[k].Y());
		  HoughF->Fill(Xcen,Ycen,Rad);
 
	//	  cout<<"X1: "<<J1_PseudoPoints[i].X() <<"  Y1: "<<J1_PseudoPoints[i].Y()<<endl;
	//	  cout<<"X2: "<<J2_PseudoPoints[j].X() <<"  Y2: "<<J2_PseudoPoints[j].Y()<<endl;
	//	  cout<<"X3: "<<J3_PseudoPoints[k].X() <<"  Y3: "<<J3_PseudoPoints[k].Y()<<endl;
	//	  cout<<"xcen ="<< Xcen <<"; ycen = "<< Ycen <<"; Rad = "<<Rad <<endl;
		}
	    }
	}
    }
      
  //J1,J2,J4:
  cout<< "Looking for rings in (J1 J2 J4) ...." <<endl;
  if(J1_PseudoPoints.size() >= 1 && J2_PseudoPoints.size() >= 1 && J4_PseudoPoints.size() >= 1)
    {
      for(unsigned int i=0; i<J1_PseudoPoints.size(); i++)
	{
	  for(unsigned int j=0; j<J2_PseudoPoints.size(); j++)
	    {
	      for(unsigned int k=0; k<J4_PseudoPoints.size(); k++)
		{
		  double Rad  = RFindR(J1_PseudoPoints[i],J2_PseudoPoints[j],J4_PseudoPoints[k]);
		  double Xcen = RFindX(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J2_PseudoPoints[j].X(),J2_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  double Ycen = RFindY(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J2_PseudoPoints[j].X(),J2_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  HoughF->Fill(Xcen,Ycen,Rad);
		}
	    }
	}
    }
  
  //J1,J3,J4:
  cout<< "Looking for rings in (J1 J3 J4) ...." <<endl;
  if(J1_PseudoPoints.size() >= 1 && J3_PseudoPoints.size() >= 1 && J4_PseudoPoints.size() >= 1)
    {
      for(unsigned int i=0; i<J1_PseudoPoints.size(); i++)
	{
	  for(unsigned int j=0; j<J3_PseudoPoints.size(); j++)
	    {
	      for(unsigned int k=0; k<J4_PseudoPoints.size(); k++)
		{
		  double Rad  = RFindR(J1_PseudoPoints[i],J3_PseudoPoints[j],J4_PseudoPoints[k]);
		  double Xcen = RFindX(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J3_PseudoPoints[j].X(),J3_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  double Ycen = RFindY(J1_PseudoPoints[i].X(),J1_PseudoPoints[i].Y(),J3_PseudoPoints[j].X(),J3_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  HoughF->Fill(Xcen,Ycen,Rad);
		}
	    }
	}
    }
  
  //J2,J3,J4;
  cout<< "Looking for rings in (J2 J3 J4) ...." <<endl;
  if(J2_PseudoPoints.size() >= 1 && J3_PseudoPoints.size() >= 1 && J4_PseudoPoints.size() >= 1)
    {
      for(unsigned int i=0; i<J2_PseudoPoints.size(); i++)
	{
	  for(unsigned int j=0; j<J3_PseudoPoints.size(); j++)
	    {
	      for(unsigned int k=0; k<J4_PseudoPoints.size(); k++)
		{
		  double Rad  = RFindR(J2_PseudoPoints[i],J3_PseudoPoints[j],J4_PseudoPoints[k]);
		  double Xcen = RFindX(J2_PseudoPoints[i].X(),J2_PseudoPoints[i].Y(),J3_PseudoPoints[j].X(),J3_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  double Ycen = RFindY(J2_PseudoPoints[i].X(),J2_PseudoPoints[i].Y(),J3_PseudoPoints[j].X(),J3_PseudoPoints[j].Y(),J4_PseudoPoints[k].X(),J4_PseudoPoints[k].Y());
		  HoughF->Fill(Xcen,Ycen,Rad);
		}
	    }
	}
    }

  /*
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DO NOT ERASE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Forming PseudoPoints:
  for(unsigned int ps=0; ps<fr->AllX.size(); ps++)
    {
      for(unsigned int s=0; s<fr->AllY.size(); s++)
	{
	   fr->PseudoPoints.push_back(TVector3(fr->AllX[ps].Cor(),fr->AllY[s].Cor(),0));
	}
    }
    
  //Full Combinatorial:
   for(unsigned int i=0; i<fr->PseudoPoints.size()-2; i++)
     {
       for(unsigned int j=i+1; j<fr->PseudoPoints.size()-1; j++)
 	{
 	  for(unsigned int k=j+1; k<fr->PseudoPoints.size(); k++)
 	    {
 	      double Rad = RFindR(fr->PseudoPoints[i],fr->PseudoPoints[j],fr->PseudoPoints[k]);
 	      double Xcen = RFindX(fr->PseudoPoints[i].X(),fr->PseudoPoints[i].Y(),fr->PseudoPoints[j].X(),fr->PseudoPoints[j].Y(),fr->PseudoPoints[k].X(),fr->PseudoPoints[k].Y());
 	      double Ycen = RFindY(fr->PseudoPoints[i].X(),fr->PseudoPoints[i].Y(),fr->PseudoPoints[j].X(),fr->PseudoPoints[j].Y(),fr->PseudoPoints[k].X(),fr->PseudoPoints[k].Y());
 	      HoughF->Fill(Xcen,Ycen,Rad);
 	      HoughFxy->Fill(Xcen,Ycen);
 	      HoughFyr->Fill(Ycen,Rad);
 	      HoughFxr->Fill(Xcen,Rad);
	      }
 	}
     }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  */      
  int xbin, ybin, zbin;
  xbin=0; ybin=0; zbin=0;
  HoughF->GetMaximumBin(xbin, ybin, zbin);
  // cout << "xbin= " << xbin << " ybin= " << ybin << " zbin= " << zbin << endl;

  TAxis *xaxis = HoughF->GetXaxis();  
  double Xrec=xaxis->GetBinCenter(xbin);
  //cout<<"Xrec= "<< Xrec <<endl;

  TAxis *yaxis = HoughF->GetYaxis();  
  double Yrec=yaxis->GetBinCenter(ybin);
  //cout<<"Yrec= "<< Yrec <<endl;

  TAxis *zaxis = HoughF->GetZaxis();  
  double Rrec=zaxis->GetBinCenter(zbin);
  //cout<<"Rrec= "<< Rrec <<endl;

/*   double Xerr = fr->CX[0].Cor()-Xrec;  
   double Yerr = fr->CY[0].Cor()-Yrec;  
   double Rerr = fr->Ring[0].R()-Rrec;  
*/

//  if(fabs(Xerr) < 3*fr->Ring[0].SigmaX() && fabs(Yerr) < 3*fr->Ring[0].SigmaY() && fabs(Rerr) < 3*fr->Ring[0].SigmaR()) fr->Nsuc.push_back(1);
       
//    double Xdif = fabs(fr->Ring[0].Xmean())-fabs(Xerr); //cout<<"Xdif= " <<Xdif<<endl;
//    double Ydif = fabs(fr->Ring[0].Ymean())-fabs(Yerr); //cout<<"Ydif= " <<Ydif<<endl;
//    double Rdif = fabs(fr->Ring[0].Rmean())-fabs(Rerr); //cout<<"Rdif= " <<Rdif<<endl;

//    if(fabs(Xdif) < 3*fr->Ring[0].SigmaX() && fabs(Ydif) < 3*fr->Ring[0].SigmaY() && fabs(Rdif) < 3*fr->Ring[0].SigmaR()) fr->Nsuc_off.push_back(1);
/*
   xerr->Fill(Xerr);
   yerr->Fill(Yerr);
   rerr->Fill(Rerr);
  */ 

  TFile *file = new TFile(outputfile,"recreate");
  HoughF->Write();
  file->Close();
}
