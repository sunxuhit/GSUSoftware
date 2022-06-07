#ifndef PhDiMuonUtility_h
#define PhDiMuonUtility_h

#include <TObject.h>
#include "Run14AuAuList.h"
#include "PhVecMesonCons.h"
#include <map>

class TH1F;
class TH2F;

class PhDiMuonUtility : public TObject
{
  public:
    PhDiMuonUtility();
    virtual ~PhDiMuonUtility();

    int get_recal_group(int run_num);
    bool isGoodRun(int runId);

    void initRunIndex();
    bool read_in_runIndex();
    int find_runIndex(int runId);

    int getCentralityBin20(float centrality);
    int getCentralityBin10(float centrality);
    int getCentralityBin4(float centrality);
    int getVertexBin(float vtx_z);

  private:
    std::map<int,int> map_runIndex;

    ClassDef(PhDiMuonUtility,1)
};
#endif
