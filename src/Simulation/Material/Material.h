#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <utility>
#include <TObject.h>

class Material : public TObject
{
 public:
  Material();
  ~Material() {}
  
  double extrapQE_PMT(double wl);
  double extrapQE_MPPC(double wl);
  
 private:
  std::vector< std::pair<double, double> > QE_PMT;
  std::vector< std::pair<double, double> > QE_MPPC;

  ClassDef(Material,1)
};
#endif
