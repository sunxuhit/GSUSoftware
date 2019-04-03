#ifndef PhVecMesonCons_h
#define PhVecMesonCons_h

namespace vecMesonFlow // binning used in flow analysis
{
  // 0: (0%,5%], 1: (5%-10%], 2: (10%,15%], ......, 18: (90%,95%], 19: (95%,100%]
  const int mNumOfCentrality20 = 20;
  const int mCentStart20[mNumOfCentrality20]  = {0, 5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};
  const int mCentStop20[mNumOfCentrality20]   = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
  const int mCentrality20[mNumOfCentrality20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19};

  // 0: (0%,5%], 1: (5%-10%], 2: (10%,20%], ......, 7: (60%,70%], 8: (70%,80%]
  const int mNumOfCentrality10 = 10;
  const int mCentStart10[mNumOfCentrality10]  = {0, 5,10,20,30,40,50,60,70,80};
  const int mCentStop10[mNumOfCentrality10]   = {5,10,20,30,40,50,60,70,80,100};
  const int mCentrality10[mNumOfCentrality10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  // 0: (0%,10%], 1: (10%-40%], 2: (40%,80%], 3: (80%,100%]
  const int mNumOfCentrality4 = 4;
  const int mCentStart4[mNumOfCentrality4]  = { 0,10,40,80};
  const int mCentStop4[mNumOfCentrality4]   = {10,40,80,100};
  const int mCentrality4[mNumOfCentrality4] = { 0, 1, 2, 3};

  // 0: [-10.0,-5.0], 1: (-5.0,0.0], 2: (0.0,5.0], 3: (5.0,10.0]
  const int mNumOfVertex = 4;
  const float mVtxStart[mNumOfVertex] = {-10.0,-5.0,0.0,5.0};
  const float mVtxStop[mNumOfVertex]  = { -5.0, 0.0,5.0,10.0};
  const int mVertex[mNumOfVertex]     = {0, 1, 2, 3};

  // shift correction 20 terms
  // const int mNumOfShiftOrder = 20; 
  // const float mShiftOrder[mNumOfShiftOrder] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0};

  // shift correction 10 terms
  const int mNumOfShiftOrder = 10; 
  const float mShiftOrder[mNumOfShiftOrder] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};

  // shift correction 5 terms
  // const int mNumOfShiftOrder = 5; 
  // const float mShiftOrder[mNumOfShiftOrder] = {1.0,2.0,3.0,4.0,5.0};
  const float mHarmonic[3] = {1.0,2.0,3.0};
}
#endif
