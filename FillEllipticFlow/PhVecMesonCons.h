#ifndef PhVecMesonCons_h
#define PhVecMesonCons_h

namespace vecMesonFlow // binning used in flow analysis
{
  //====================================================================
  //==========basic binning==========
  // 0: (0%,5%], 1: (5%-10%], 2: (10%,15%], ......, 18: (90%,95%], 19: (95%,100%]
  const int mNumOfCentrality20 = 20;
  const float mCentStart20[mNumOfCentrality20]  = {0.0, 5.0,10.0,15.0,20.0,25.0,30.0,35.0,40.0,45.0,50.0,55.0,60.0,65.0,70.0,75.0,80.0,85.0,90.0,95.0};
  const float mCentStop20[mNumOfCentrality20]   = {5.0,10.0,15.0,20.0,25.0,30.0,35.0,40.0,45.0,50.0,55.0,60.0,65.0,70.0,75.0,80.0,85.0,90.0,95.0,100.0};
  const int mCentrality20[mNumOfCentrality20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19};

  // 0: (0%,5%], 1: (5%-10%], 2: (10%,20%], ......, 7: (60%,70%], 8: (70%,80%], 9: [80%,100%]
  const int mNumOfCentrality10 = 10;
  const float mCentStart10[mNumOfCentrality10]  = {0.0, 5.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0};
  const float mCentStop10[mNumOfCentrality10]   = {5.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,100.0};
  const int mCentrality10[mNumOfCentrality10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  // 0: (0%,10%], 1: (10%-40%], 2: (40%,80%], 3: (80%,100%]
  const int mNumOfCentrality4 = 4;
  const float mCentStart4[mNumOfCentrality4]  = { 0.0,10.0,40.0,80.0};
  const float mCentStop4[mNumOfCentrality4]   = {10.0,40.0,80.0,100.0};
  const int mCentrality4[mNumOfCentrality4]   = {0, 1, 2, 3};

  // 0: [-10.0,-5.0], 1: (-5.0,0.0], 2: (0.0,5.0], 3: (5.0,10.0]
  const int mNumOfVertex = 4;
  const float mVtxStart[mNumOfVertex] = {-10.0,-5.0,0.0,5.0};
  const float mVtxStop[mNumOfVertex]  = { -5.0, 0.0,5.0,10.0};
  const int mVertex[mNumOfVertex]     = {0, 1, 2, 3};

  const float mHarmonic[3] = {1.0,2.0,3.0};
  //====================================================================

  //====================================================================
  //==========binning in elliptic flow analysis==========
  const int mList_Delta = 40;
  const std::string mPID[2] = {"JPsi","Phi"};
  const std::string mEventType[3] = {"US","LS","ME"};

  const int mNumOfPt = 4; // pt bining
  const float mPtStart[mNumOfPt] = {0.0,2.0,4.0,6.0};
  const float mPtStop[mNumOfPt] = {2.0,4.0,6.0,10.0};
  const int mPtBin[mNumOfPt] = {0, 1, 2, 3};

  const int mNumOfPhi = 7; // phi bining
  const float mPhiStart[mNumOfPhi] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0}; // * 0.5*TMath::Pi()
  const float mPhiStop[mNumOfPhi] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0};
  const int mPhiBin[mNumOfPhi] = {0, 1, 2, 3, 4, 5, 6};

  const int mNumOfPsi = 3; // phi bining
  const float mPsiStart[mNumOfPsi] = {-3.0,-1.0,1.0}; // *0.5*TMath::Pi()
  const float mPsiStop[mNumOfPsi] = {-1.0, 1.0,3.0};
  const int mPsiBin[mNumOfPsi] = {0, 1, 2};

  //====================================================================
}
#endif
