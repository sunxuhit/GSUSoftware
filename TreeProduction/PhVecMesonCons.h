#ifndef PhVecMesonCons_h
#define PhVecMesonCons_h

namespace vecMesonFlow // binning used in flow analysis
{
  // 0: (0%,5%], 1: (5%-10%], 2: (10%,15%], ......, 18: (90%,95%], 19: (95%,100%]
  // const int mNumOfCentralities = 20;
  // const int mCentStart[mNumOfCentralities] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};
  // const int mCentStop[mNumOfCentralities] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
  // const int mCentrality[mNumOfCentralities] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

  const int mNumOfCentralities = 4;
  const int mCentStart[mNumOfCentralities] = {0,10,40,80};
  const int mCentStop[mNumOfCentralities] = {10,40,80,100};
  const int mCentrality[mNumOfCentralities] = {0,1,2,3};

}
#endif
