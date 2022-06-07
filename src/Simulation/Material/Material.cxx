#include "./Material.h"

ClassImp(Material)

Material::Material()
{
  const double qe_PMT[45] = { 
    11.50,16.83,22.45,28.18,32.87,33.37,33.63,33.73,33.46,33.12,
    32.74,32.42,32.45,31.54,30.76,29.76,28.77,27.59,26.16,24.31,
    22.48,20.93,19.67,18.49,16.50,13.05,10.11,8.36 ,7.15 ,6.12 ,
    5.19 ,4.29 ,3.46 ,2.69 ,2.00 ,1.41 ,0.95 ,0.60 ,0.35 ,0.19 ,
    0.10 ,0.05 ,0.02 ,0.01 ,0.00};

  const double lambda_PMT[45] = { 
    260,270,280,290,300,310,320,330,340,350,
    360,370,380,390,400,410,420,430,440,450,
    460,470,480,490,500,510,520,530,540,550,
    560,570,580,590,600,610,620,630,640,650,
    660,670,680,690,700};

  for(int i=0; i<45; i++){
    std::pair<double, double> tmp (lambda_PMT[i], qe_PMT[i]);
    QE_PMT.push_back(tmp);
  }
  
  const double qe_MPPC[45] = {  
    11.50,16.83,22.45,28.18,32.87,33.37,33.63,33.73,33.46,33.12,
    32.74,32.42,32.45,31.54,30.76,29.76,28.77,27.59,26.16,24.31,
    22.48,20.93,19.67,18.49,16.50,13.05,10.11,8.36 ,7.15 ,6.12 ,
    5.19 ,4.29 ,3.46 ,2.69 ,2.00 ,1.41 ,0.95 ,0.60 ,0.35 ,0.19 ,
    0.10 ,0.05 ,0.02 ,0.01 ,0.00};

  const double lambda_MPPC[45] = { 
    260,270,280,290,300,310,320,330,340,350,
    360,370,380,390,400,410,420,430,440,450,
    460,470,480,490,500,510,520,530,540,550,
    560,570,580,590,600,610,620,630,640,650,
    660,670,680,690,700};

  for(int i=0; i<45; i++){
    std::pair<double, double> tmp (lambda_MPPC[i], qe_MPPC[i]);
    QE_MPPC.push_back(tmp);
  }
}

double Material::extrapQE_PMT(double wl)
{
  double QE = -999.;
  
  int iBin = -1;
  for(unsigned int i=0; i<QE_PMT.size()-1; i++){
    if(wl>=QE_PMT.at(i).first && wl<QE_PMT.at(i+1).first){
      iBin = i;
      break;
    }
  }
  if(iBin>=0 && iBin<QE_PMT.size()){
    double x1 = QE_PMT.at(iBin).first;
    double x2 = QE_PMT.at(iBin+1).first;
    double y1 = QE_PMT.at(iBin).second;
    double y2 = QE_PMT.at(iBin+1).second;
    QE = y1 + (wl-x1)*(y2-y1)/(x2-x1);
  }
  if(QE>0.) return QE/100.;
  else return 0;
}

double Material::extrapQE_MPPC(double wl)
{
  double QE = -999.;
  
  int iBin = -1;
  for(unsigned int i=0; i<QE_MPPC.size()-1; i++){
    if(wl>=QE_MPPC.at(i).first && wl<QE_MPPC.at(i+1).first){
      iBin = i;
      break;
    }
  }
  if(iBin>=0 && iBin<QE_MPPC.size()){
    double x1 = QE_MPPC.at(iBin).first;
    double x2 = QE_MPPC.at(iBin+1).first;
    double y1 = QE_MPPC.at(iBin).second;
    double y2 = QE_MPPC.at(iBin+1).second;
    QE = y1 + (wl-x1)*(y2-y1)/(x2-x1);
  }
  if(QE>0.) return QE/100.;
  else return 0;
}
