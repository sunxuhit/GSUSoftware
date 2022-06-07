#include <iostream>
#include <string> 
#include "./BeamFinder.h"

using namespace std;

ClassImp(BeamFinder)

BeamFinder::BeamFinder()
{
  cout << "BeamFinder::BeamFinder() ----- Constructor ! ------" << endl;

  mVerbosity = false;
  mVerbosity_QA = false;
}

BeamFinder::~BeamFinder()
{
  cout << "BeamFinder::~BeamFinder() ----- Release memory ! ------" << endl;
}

int BeamFinder::clearClusterFinder_5by5()
{
  mXClusterMap_5by5.clear();
  mYClusterMap_5by5.clear();
  mRankMap_5by5.clear();

  mXCluster_5by5_1st.clear();
  mYCluster_5by5_1st.clear();
  mRank_5by5_1st.clear();
  mMeanX_5by5_1st = -1;
  mMeanY_5by5_1st = -1;

  mXCluster_5by5_2nd.clear();
  mYCluster_5by5_2nd.clear();
  mRank_5by5_2nd.clear();
  mMeanX_5by5_2nd = -1;
  mMeanY_5by5_2nd = -1;

  mXCluster_5by5_3rd.clear();
  mYCluster_5by5_3rd.clear();
  mRank_5by5_3rd.clear();
  mMeanX_5by5_3rd = -1;
  mMeanY_5by5_3rd = -1;

  return 1;
}

int BeamFinder::clearClusterFinder_3by3()
{
  mXClusterMap_3by3.clear();
  mYClusterMap_3by3.clear();
  mRankMap_3by3.clear();

  mXCluster_3by3_1st.clear();
  mYCluster_3by3_1st.clear();
  mRank_3by3_1st.clear();
  mMeanX_3by3_1st = -1;
  mMeanY_3by3_1st = -1;

  mXCluster_3by3_2nd.clear();
  mYCluster_3by3_2nd.clear();
  mRank_3by3_2nd.clear();
  mMeanX_3by3_2nd = -1;
  mMeanY_3by3_2nd = -1;

  mXCluster_3by3_3rd.clear();
  mYCluster_3by3_3rd.clear();
  mRank_3by3_3rd.clear();
  mMeanX_3by3_3rd = -1;
  mMeanY_3by3_3rd = -1;

  return 1;
}

int BeamFinder::findCluster_5by5(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfClusters = 0;
  clearClusterFinder_5by5();

  const int NumOfPhotons = numOfPhotons;
  int beam_counter[NumOfPhotons];

  // beam finder
  for(int i_beam = 0; i_beam < NumOfPhotons; ++i_beam) // first round of cluster
  {
    beam_counter[i_beam] = 0;
    for(int i_cluster = 0; i_cluster < NumOfPhotons; ++i_cluster)
    {
      int delta_x = TMath::Abs(xPixel[i_cluster]-xPixel[i_beam]);
      int delta_y = TMath::Abs(yPixel[i_cluster]-yPixel[i_beam]);
      if(delta_x < 3 && delta_y < 3) // 5*5 pixel cluster
      {
	beam_counter[i_beam]++;
      }
    }
    if(mVerbosity_QA) cout << "beam_counter = " << beam_counter[i_beam] << endl;
    if(beam_counter[i_beam] > 4)
    {
      mXClusterMap_5by5.push_back(xPixel[i_beam]-1); // convert histogram bin number to pixel id
      mYClusterMap_5by5.push_back(yPixel[i_beam]-1);
      mRankMap_5by5.push_back(beam_counter[i_beam]);
    }
  }

  if(mRankMap_5by5.size() == 0) 
  {
    if(mVerbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;
    return NumOfClusters;
  }

  std::vector<int>::iterator it_max_1st = max_element(std::begin(mRankMap_5by5),std::end(mRankMap_5by5)); // most ranked pixel
  int cluster_1st = std::distance(mRankMap_5by5.begin(), it_max_1st);
  if(mVerbosity_QA) std::cout << "max element at: " << cluster_1st << endl;
  if(mVerbosity_QA) cout << "max rank = " << mRankMap_5by5[cluster_1st] << endl;

  std::vector<int> XCluster_rest;
  std::vector<int> YCluster_rest;
  std::vector<int> Rank_rest;
  XCluster_rest.clear();
  YCluster_rest.clear();
  Rank_rest.clear();

  for(int i_cluster = 0; i_cluster < mRankMap_5by5.size(); ++i_cluster)
  {
    int delta_x = TMath::Abs(mXClusterMap_5by5[i_cluster]-mXClusterMap_5by5[cluster_1st]);
    int delta_y = TMath::Abs(mYClusterMap_5by5[i_cluster]-mYClusterMap_5by5[cluster_1st]);
    if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
    {
      mXCluster_5by5_1st.push_back(mXClusterMap_5by5[i_cluster]);
      mYCluster_5by5_1st.push_back(mYClusterMap_5by5[i_cluster]);
      mRank_5by5_1st.push_back(mRankMap_5by5[i_cluster]);
      if(mVerbosity_QA) cout << "1st cluster Ranking: " << mRankMap_5by5[i_cluster] << endl;
    }
    else
    {
      XCluster_rest.push_back(mXClusterMap_5by5[i_cluster]);
      YCluster_rest.push_back(mYClusterMap_5by5[i_cluster]);
      Rank_rest.push_back(mRankMap_5by5[i_cluster]);
      // cout << "rest cluster Ranking: " << mRankMap_5by5[i_cluster] << endl;
    }
  }
  if(mRank_5by5_1st.size() > 0) 
  {
    NumOfClusters++;
    std::vector<int>::iterator it_1st = max_element(std::begin(mRank_5by5_1st),std::end(mRank_5by5_1st)); // most ranked pixel
    int beam_1st = std::distance(mRank_5by5_1st.begin(), it_1st);
    mMeanX_5by5_1st = mXCluster_5by5_1st[beam_1st];
    mMeanY_5by5_1st = mYCluster_5by5_1st[beam_1st];
  }

  if(Rank_rest.size() > 0)
  {
    std::vector<int>::iterator it_max_2nd = max_element(std::begin(Rank_rest),std::end(Rank_rest)); // most ranked pixel in 2nd cluster
    int cluster_2nd = std::distance(Rank_rest.begin(), it_max_2nd);
    if(mVerbosity_QA) std::cout << "max element at: " << cluster_2nd << endl;
    if(mVerbosity_QA) cout << "max rank = " << Rank_rest[cluster_2nd] << endl;

    for(int i_cluster = 0; i_cluster < Rank_rest.size(); ++i_cluster)
    {
      int delta_x = TMath::Abs(XCluster_rest[i_cluster]-XCluster_rest[cluster_2nd]);
      int delta_y = TMath::Abs(YCluster_rest[i_cluster]-YCluster_rest[cluster_2nd]);
      if(delta_x < 6 && delta_y < 6) // 2 adjacent 5*5 pixel cluster => 1 cluster
      {
	mXCluster_5by5_2nd.push_back(XCluster_rest[i_cluster]);
	mYCluster_5by5_2nd.push_back(YCluster_rest[i_cluster]);
	mRank_5by5_2nd.push_back(Rank_rest[i_cluster]);
	if(mVerbosity_QA) cout << "2nd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
      else
      {
	mXCluster_5by5_3rd.push_back(XCluster_rest[i_cluster]);
	mYCluster_5by5_3rd.push_back(YCluster_rest[i_cluster]);
	mRank_5by5_3rd.push_back(Rank_rest[i_cluster]);
	if(mVerbosity_QA) cout << "3rd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
    }
    if(mRank_5by5_2nd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_2nd = max_element(std::begin(mRank_5by5_2nd),std::end(mRank_5by5_2nd)); // most ranked pixel
      int beam_2nd = std::distance(mRank_5by5_2nd.begin(), it_2nd);
      mMeanX_5by5_2nd = mXCluster_5by5_2nd[beam_2nd];
      mMeanY_5by5_2nd = mYCluster_5by5_2nd[beam_2nd];
    }

    if(mRank_5by5_3rd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_3rd = max_element(std::begin(mRank_5by5_3rd),std::end(mRank_5by5_3rd)); // most ranked pixel
      int beam_3rd = std::distance(mRank_5by5_3rd.begin(), it_3rd);
      mMeanX_5by5_3rd = mXCluster_5by5_3rd[beam_3rd];
      mMeanY_5by5_3rd = mYCluster_5by5_3rd[beam_3rd];
    }
  }

  if(mVerbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;

  return NumOfClusters;
}

int BeamFinder::findCluster_3by3(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  int NumOfClusters = 0;
  clearClusterFinder_3by3();

  const int NumOfPhotons = numOfPhotons;
  int beam_counter[NumOfPhotons];

  // beam finder
  for(int i_beam = 0; i_beam < NumOfPhotons; ++i_beam) // first round of cluster
  {
    beam_counter[i_beam] = 0;
    for(int i_cluster = 0; i_cluster < NumOfPhotons; ++i_cluster)
    {
      int delta_x = TMath::Abs(xPixel[i_cluster]-xPixel[i_beam]);
      int delta_y = TMath::Abs(yPixel[i_cluster]-yPixel[i_beam]);
      if(delta_x < 2 && delta_y < 2) // 3*3 pixel cluster
      {
	beam_counter[i_beam]++;
      }
    }
    if(mVerbosity_QA) cout << "beam_counter = " << beam_counter[i_beam] << endl;
    if(beam_counter[i_beam] > 2)
    {
      mXClusterMap_3by3.push_back(xPixel[i_beam]-1); // convert histogram bin number to pixel id
      mYClusterMap_3by3.push_back(yPixel[i_beam]-1);
      mRankMap_3by3.push_back(beam_counter[i_beam]);
    }
  }

  if(mRankMap_3by3.size() == 0) 
  {
    if(mVerbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;
    return NumOfClusters;
  }

  std::vector<int>::iterator it_max_1st = max_element(std::begin(mRankMap_3by3),std::end(mRankMap_3by3)); // most ranked pixel
  int cluster_1st = std::distance(mRankMap_3by3.begin(), it_max_1st);
  if(mVerbosity_QA) std::cout << "max element at: " << cluster_1st << endl;
  if(mVerbosity_QA) cout << "max rank = " << mRankMap_3by3[cluster_1st] << endl;

  std::vector<int> XCluster_rest;
  std::vector<int> YCluster_rest;
  std::vector<int> Rank_rest;
  XCluster_rest.clear();
  YCluster_rest.clear();
  Rank_rest.clear();

  for(int i_cluster = 0; i_cluster < mRankMap_3by3.size(); ++i_cluster)
  {
    int delta_x = TMath::Abs(mXClusterMap_3by3[i_cluster]-mXClusterMap_3by3[cluster_1st]);
    int delta_y = TMath::Abs(mYClusterMap_3by3[i_cluster]-mYClusterMap_3by3[cluster_1st]);
    if(delta_x < 4 && delta_y < 4) // 2 adjacent 3*3 pixel cluster => 1 cluster
    {
      mXCluster_3by3_1st.push_back(mXClusterMap_3by3[i_cluster]);
      mYCluster_3by3_1st.push_back(mYClusterMap_3by3[i_cluster]);
      mRank_3by3_1st.push_back(mRankMap_3by3[i_cluster]);
      if(mVerbosity_QA) cout << "1st cluster Ranking: " << mRankMap_3by3[i_cluster] << endl;
    }
    else
    {
      XCluster_rest.push_back(mXClusterMap_3by3[i_cluster]);
      YCluster_rest.push_back(mYClusterMap_3by3[i_cluster]);
      Rank_rest.push_back(mRankMap_3by3[i_cluster]);
      // cout << "rest cluster Ranking: " << mRankMap_3by3[i_cluster] << endl;
    }
  }
  if(mRank_3by3_1st.size() > 0) 
  {
    NumOfClusters++;
    std::vector<int>::iterator it_1st = max_element(std::begin(mRank_3by3_1st),std::end(mRank_3by3_1st)); // most ranked pixel
    int beam_1st = std::distance(mRank_3by3_1st.begin(), it_1st);
    mMeanX_3by3_1st = mXCluster_3by3_1st[beam_1st];
    mMeanY_3by3_1st = mYCluster_3by3_1st[beam_1st];
  }

  if(Rank_rest.size() > 0)
  {
    std::vector<int>::iterator it_max_2nd = max_element(std::begin(Rank_rest),std::end(Rank_rest)); // most ranked pixel in 2nd cluster
    int cluster_2nd = std::distance(Rank_rest.begin(), it_max_2nd);
    if(mVerbosity_QA) std::cout << "max element at: " << cluster_2nd << endl;
    if(mVerbosity_QA) cout << "max rank = " << Rank_rest[cluster_2nd] << endl;

    for(int i_cluster = 0; i_cluster < Rank_rest.size(); ++i_cluster)
    {
      int delta_x = TMath::Abs(XCluster_rest[i_cluster]-XCluster_rest[cluster_2nd]);
      int delta_y = TMath::Abs(YCluster_rest[i_cluster]-YCluster_rest[cluster_2nd]);
      if(delta_x < 4 && delta_y < 4) // 2 adjacent 5*5 pixel cluster => 1 cluster
      {
	mXCluster_3by3_2nd.push_back(XCluster_rest[i_cluster]);
	mYCluster_3by3_2nd.push_back(YCluster_rest[i_cluster]);
	mRank_3by3_2nd.push_back(Rank_rest[i_cluster]);
	if(mVerbosity_QA) cout << "2nd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
      else
      {
	mXCluster_3by3_3rd.push_back(XCluster_rest[i_cluster]);
	mYCluster_3by3_3rd.push_back(YCluster_rest[i_cluster]);
	mRank_3by3_3rd.push_back(Rank_rest[i_cluster]);
	if(mVerbosity_QA) cout << "3rd cluster Ranking: " << Rank_rest[i_cluster] << endl;
      }
    }
    if(mRank_3by3_2nd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_2nd = max_element(std::begin(mRank_3by3_2nd),std::end(mRank_3by3_2nd)); // most ranked pixel
      int beam_2nd = std::distance(mRank_3by3_2nd.begin(), it_2nd);
      mMeanX_3by3_2nd = mXCluster_3by3_2nd[beam_2nd];
      mMeanY_3by3_2nd = mYCluster_3by3_2nd[beam_2nd];
    }

    if(mRank_3by3_3rd.size() > 0) 
    {
      NumOfClusters++;
      std::vector<int>::iterator it_3rd = max_element(std::begin(mRank_3by3_3rd),std::end(mRank_3by3_3rd)); // most ranked pixel
      int beam_3rd = std::distance(mRank_3by3_3rd.begin(), it_3rd);
      mMeanX_3by3_3rd = mXCluster_3by3_3rd[beam_3rd];
      mMeanY_3by3_3rd = mYCluster_3by3_3rd[beam_3rd];
    }
  }

  if(mVerbosity) cout << NumOfClusters << " cluster(s) found in event" << endl;

  return NumOfClusters;
}

int BeamFinder::initBeamFinder()
{
  cout << "initBeamFinder: initialized histograms for Beam Finder!" << endl;
  h_mRingImage_on = new TH2D("h_mRingImage_on","h_mRingImage_on",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mRingImage_off = new TH2D("h_mRingImage_off","h_mRingImage_off",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  h_mBeamFinder_5by5_Display = new TH2D("h_mBeamFinder_5by5_Display","h_mBeamFinder_5by5_Display",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_1st = new TH2D("h_mBeamFinder_5by5_1st","h_mBeamFinder_5by5_1st",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_2nd = new TH2D("h_mBeamFinder_5by5_2nd","h_mBeamFinder_5by5_2nd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_5by5_3rd = new TH2D("h_mBeamFinder_5by5_3rd","h_mBeamFinder_5by5_3rd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  h_mBeamFinder_3by3_Display = new TH2D("h_mBeamFinder_3by3_Display","h_mBeamFinder_3by3_Display",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_1st = new TH2D("h_mBeamFinder_3by3_1st","h_mBeamFinder_3by3_1st",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_2nd = new TH2D("h_mBeamFinder_3by3_2nd","h_mBeamFinder_3by3_2nd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);
  h_mBeamFinder_3by3_3rd = new TH2D("h_mBeamFinder_3by3_3rd","h_mBeamFinder_3by3_3rd",mRICH::mNumOfPixels,-0.5,32.5,mRICH::mNumOfPixels,-0.5,32.5);

  return 1;
}

int BeamFinder::fillBeamFinder_5by5()
{
  int NumOfClusters = 0;
  if(mMeanX_5by5_1st > -1 && mMeanY_5by5_1st > -1)
  {
    h_mBeamFinder_5by5_1st->Fill(mMeanX_5by5_1st,mMeanY_5by5_1st);
    h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_1st,mMeanY_5by5_1st);
    NumOfClusters++;
  }
  if(mMeanX_5by5_2nd > -1 && mMeanY_5by5_2nd > -1)
  {
    h_mBeamFinder_5by5_2nd->Fill(mMeanX_5by5_2nd,mMeanY_5by5_2nd);
    h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_2nd,mMeanY_5by5_2nd);
    NumOfClusters++;
  }
  if(mMeanX_5by5_3rd > -1 && mMeanY_5by5_3rd > -1)
  {
    h_mBeamFinder_5by5_3rd->Fill(mMeanX_5by5_3rd,mMeanY_5by5_3rd);
    h_mBeamFinder_5by5_Display->Fill(mMeanX_5by5_3rd,mMeanY_5by5_3rd);
    NumOfClusters++;
  }

  return NumOfClusters;
}

int BeamFinder::fillBeamFinder_3by3()
{
  int NumOfClusters = 0;
  if(mMeanX_3by3_1st > -1 && mMeanY_3by3_1st > -1)
  {
    h_mBeamFinder_3by3_1st->Fill(mMeanX_3by3_1st,mMeanY_3by3_1st);
    h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_1st,mMeanY_3by3_1st);
    NumOfClusters++;
  }
  if(mMeanX_3by3_2nd > -1 && mMeanY_3by3_2nd > -1)
  {
    h_mBeamFinder_3by3_2nd->Fill(mMeanX_3by3_2nd,mMeanY_3by3_2nd);
    h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_2nd,mMeanY_3by3_2nd);
    NumOfClusters++;
  }
  if(mMeanX_3by3_3rd > -1 && mMeanY_3by3_3rd > -1)
  {
    h_mBeamFinder_3by3_3rd->Fill(mMeanX_3by3_3rd,mMeanY_3by3_3rd);
    h_mBeamFinder_3by3_Display->Fill(mMeanX_3by3_3rd,mMeanY_3by3_3rd);
    NumOfClusters++;
  }

  return NumOfClusters;
}

int BeamFinder::fillBeamOn(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  for(int i_photon = 0; i_photon < numOfPhotons; ++i_photon)
  {
    h_mRingImage_on->Fill(xPixel[i_photon]-1,yPixel[i_photon]-1);
  }

  return 1;
}

int BeamFinder::fillBeamOff(int numOfPhotons, std::vector<int> xPixel, std::vector<int> yPixel)
{
  for(int i_photon = 0; i_photon < numOfPhotons; ++i_photon)
  {
    h_mRingImage_off->Fill(xPixel[i_photon]-1,yPixel[i_photon]-1);
  }

  return 1;
}

int BeamFinder::writeBeamFinder()
{
  h_mRingImage_on->Write();
  h_mRingImage_off->Write();

  h_mBeamFinder_5by5_Display->Write();
  h_mBeamFinder_5by5_1st->Write();
  h_mBeamFinder_5by5_2nd->Write();
  h_mBeamFinder_5by5_3rd->Write();

  h_mBeamFinder_3by3_Display->Write();
  h_mBeamFinder_3by3_1st->Write();
  h_mBeamFinder_3by3_2nd->Write();
  h_mBeamFinder_3by3_3rd->Write();

  return 1;
}
