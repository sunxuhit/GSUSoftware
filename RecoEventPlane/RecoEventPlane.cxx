//local
#include "RecoEventPlane.h"

//PHENIX
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <phool.h>
#include <PHGlobal.h>

#include <Bbc.hh>
#include <BbcRaw.h>
#include <BbcOut.h>
#include <BbcCalib.hh>
#include <BbcGeo.hh>

#include <VtxOut.h>
#include <PHPoint.h>

//stl
#include <iostream>
#include <ostream>
#include <string>

//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TString.h>

using namespace std;

RecoEventPlane::RecoEventPlane() :
		_n_events_processed(0),
		_eval_file_name("RecoEventPlaneEval.root"),
		_fout(NULL)
{
}

int RecoEventPlane::Init(PHCompositeNode *topNode) 
{
  _n_events_processed = 0;

  // init output structure
  _fout = TFile::Open(_eval_file_name.data(), "recreate");
  if(!_fout) cout<<"Cann't create " <<_eval_file_name.data() <<endl;

  h_mVtZ = new TH1F("h_mVtZ","h_mVtZ",201,-100.5,100.5);
  h_mVtR = new TH2F("h_mVtR","h_mVtR",101,-5.05,5.05,101,-5.05,5.05);
  h_mVtZ_Bbc = new TH1F("h_mVtZ_Bbc","h_mVtZ_Bbc",201,-100.5,100.5);
  h_mVtZ_Zdc = new TH1F("h_mVtZ_Zdc","h_mVtZ_Zdc",201,-100.5,100.5);

  bbccalib = new BbcCalib();
  bbcgeo = new BbcGeo();

  for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    string HistName;

    HistName = Form("h_mADC_Bbc_%d",i_pmt);
    h_mAdc_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),5000,0,10000);

    HistName = Form("h_mCharge_Bbc_%d",i_pmt);
    h_mCharge_Bbc[i_pmt] = new TH1F(HistName.c_str(),HistName.c_str(),100,0,50);
  }

  h_mGeoXY_BbcSouth = new TH2F("h_mGeoXY_BbcSouth","h_mGeoXY_BbcSouth",40,-200.0,200.0,40,-200.0,200.0);
  h_mGeoZ_BbcSouth  = new TH1F("h_mGeoZ_BbcSouth","h_mGeoZ_BbcSouth",1000,-2000,2000);

  h_mGeoXY_BbcNorth = new TH2F("h_mGeoXY_BbcNorth","h_mGeoXY_BbcNorth",40,-200.0,200.0,40,-200.0,200.0);
  h_mGeoZ_BbcNorth  = new TH1F("h_mGeoZ_BbcNorth","h_mGeoZ_BbcNorth",1000,-2000,2000);

  h_mEP1st_BbcSouth = new TH1F("h_mEP1st_BbcSouth","h_mEP1st_BbcSouth",360,-TMath::Pi(),TMath::Pi());
  h_mEP1st_BbcNorth = new TH1F("h_mEP1st_BbcNorth","h_mEP1st_BbcNorth",360,-TMath::Pi(),TMath::Pi());

  h_mEP2nd_BbcSouth = new TH1F("h_mEP2nd_BbcSouth","h_mEP2nd_BbcSouth",360,-TMath::Pi(),TMath::Pi());
  h_mEP2nd_BbcNorth = new TH1F("h_mEP2nd_BbcNorth","h_mEP2nd_BbcNorth",360,-TMath::Pi(),TMath::Pi());

  return EVENT_OK;
}

int RecoEventPlane::InitRun(PHCompositeNode *topNode) 
{
  return EVENT_OK;
}

int RecoEventPlane::process_event(PHCompositeNode *topNode) 
{
  _n_events_processed++;

  if(_n_events_processed%100==0) cout << "processing events:  " << _n_events_processed << "/" << 10000 << endl;

  VtxOut *vtxout = findNode::getClass<VtxOut>(topNode,"VtxOut");
  if(!vtxout) 
  {
    cout << "No Vtxout information!" << endl;
    return 1;
  }

  PHPoint vertex = vtxout->get_Vertex();
  h_mVtZ->Fill(vertex.getZ());
  h_mVtR->Fill(vertex.getX(),vertex.getY());

  float BBC_vtx_Z = vtxout->get_BbcVertex();
  h_mVtZ_Bbc->Fill(BBC_vtx_Z);

  float ZDC_vtx_Z = vtxout->get_ZdcVertex();
  h_mVtZ_Zdc->Fill(ZDC_vtx_Z);

  // cout << "vertex_x = " << vertex.getX() << ", vertex_y = " << vertex.getY() << ", vertex_z = " << vertex.getZ() << endl;
  // cout << " BBC_z = " << BBC_vtx_Z << endl;
  // cout << " ZDC_z = " << ZDC_vtx_Z << endl;
  // cout << endl;

  BbcRaw *bbcraw = findNode::getClass<BbcRaw>(topNode,"BbcRaw");
  if (!bbcraw)
  {
    cout << PHWHERE << "Could not find BbcRaw !" << endl;
    return 1;
  }

  float Q1x_South = 0.0;
  float Q1y_South = 0.0;

  float Q1x_North = 0.0;
  float Q1y_North = 0.0;

  float Q2x_South = 0.0;
  float Q2y_South = 0.0;
  float Qw_South = 0.0;

  float Q2x_North = 0.0;
  float Q2y_North = 0.0;
  float Qw_North = 0.0;

  for (int i_pmt=0; i_pmt<128; i_pmt++) 
  {
    short adc = bbcraw->get_Adc(i_pmt);
    short tdc = bbcraw->get_Tdc0(i_pmt);
    float time0 = bbccalib->getHitTime0(i_pmt, tdc, adc);
    float charge = bbccalib->getCharge(i_pmt, adc);
    float bbcx= bbcgeo->getX(i_pmt);
    float bbcy= bbcgeo->getY(i_pmt);
    float bbcz= bbcgeo->getZ(i_pmt);

    if (time0 > 0 && charge > 0 && bbcz < 0) // south
    {
      h_mAdc_Bbc[i_pmt]->Fill(adc);
      h_mCharge_Bbc[i_pmt]->Fill(charge);
      h_mGeoXY_BbcSouth->Fill(bbcx,bbcy,charge);
      h_mGeoZ_BbcSouth->Fill(bbcz);

      float phi=atan2(bbcy,bbcx);
      float weight = charge;

      Q1x_South += weight*cos(1.0*phi);
      Q1y_South += weight*sin(1.0*phi);

      Q2x_South += weight*cos(2.0*phi);
      Q2y_South += weight*sin(2.0*phi);

      Qw_South += weight;
      // cout << "i_pmt = " << i_pmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
    }
    if (time0 > 0 && charge > 0 && bbcz > 0)  // north
    {
      h_mAdc_Bbc[i_pmt]->Fill(adc);
      h_mCharge_Bbc[i_pmt]->Fill(charge);
      h_mGeoXY_BbcNorth->Fill(bbcx,bbcy,charge);
      h_mGeoZ_BbcNorth->Fill(bbcz);

      float phi=atan2(bbcy,bbcx);
      float weight = charge;


      Q1x_North += weight*cos(1.0*phi);
      Q1y_North += weight*sin(1.0*phi);

      Q2x_North += weight*cos(2.0*phi);
      Q2y_North += weight*sin(2.0*phi);

      Qw_North += weight;
      // cout << "i_pmt = " << i_pmt << ", phi = " << phi << ", bbcz = " << bbcz << ", charge = " << charge << endl;
    }
  }

  float Psi_1st_South = atan2(Q1y_South/Qw_South,Q1x_South/Qw_South);
  h_mEP1st_BbcSouth->Fill(Psi_1st_South);

  float Psi_1st_North = atan2(Q1y_North/Qw_North,Q1x_North/Qw_North);
  h_mEP1st_BbcNorth->Fill(Psi_1st_North);

  float Psi_2nd_South = atan2(Q2y_South/Qw_South,Q2x_South/Qw_South)/2.0;
  h_mEP2nd_BbcSouth->Fill(Psi_2nd_South);

  float Psi_2nd_North = atan2(Q2y_North/Qw_North,Q2x_North/Qw_North)/2.0;
  h_mEP2nd_BbcNorth->Fill(Psi_2nd_North);

  return EVENT_OK;
}

int RecoEventPlane::End(PHCompositeNode *topNode) 
{
  _fout->cd();
  h_mVtZ->Write();
  h_mVtR->Write();
  h_mVtZ_Bbc->Write();
  h_mVtZ_Zdc->Write();

  for(int i_pmt = 0; i_pmt < 128; ++i_pmt)
  {
    h_mAdc_Bbc[i_pmt]->Write();
    h_mCharge_Bbc[i_pmt]->Write();
  }

  h_mGeoXY_BbcSouth->Write();
  h_mGeoZ_BbcSouth->Write();

  h_mGeoXY_BbcNorth->Write();
  h_mGeoZ_BbcNorth->Write();


  h_mEP1st_BbcSouth->Write();
  h_mEP1st_BbcNorth->Write();

  h_mEP2nd_BbcSouth->Write();
  h_mEP2nd_BbcNorth->Write();

  _fout->Close();

  return EVENT_OK;
}
