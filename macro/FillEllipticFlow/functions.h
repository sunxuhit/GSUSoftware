#include <TMath.h>
#include <TF1.h>

double func_bkg_LS(double *x_val, double *par)
{ // like-sign bkg 
  double x = x_val[0];
  double p0_LS = par[0];
  double p1_LS = par[1];
  double p2_LS = par[2];
  double p3_LS = par[3];
  double p4_LS = par[4];

  // double bkg_LS = p2_LS/pow(exp(-1.0*(p0_LS*x+p1_LS*pow(x,2)))+x/p3_LS,p4_LS);
  double bkg_LS = p2_LS * pow(exp(-1.0*(p0_LS*x+p1_LS*pow(x,2)))+x/p3_LS,p4_LS);

  return bkg_LS;
}

double func_bkg_Corr(double *x_val, double *par)
{ // correlated bkg
  double x = x_val[0];
  double p0_Corr = par[0];
  double p1_Corr = par[1];

  double bkg_Corr = p0_Corr * exp(p1_Corr * x);

  return bkg_Corr;
}

double func_sig_US(double *x_val, double *par)
{
  // Crystal Ball lineshape plus exponential background
  // Used for chisq fit to mixed event background data

  double sig_US;
  double x = x_val[0];

  // The four CB parameters (alpha, n, x_mean, sigma) plus normalization (N) are:
  double p0_US = par[0]; // alpha
  double p1_US = par[1]; // n
  double p2_US = par[2]; // x_mean
  double p3_US = par[3]; // sigma
  double p4_US = par[4]; // norm

  // we need:
  double A = pow( (p1_US/TMath::Abs(p0_US)),p1_US) * exp(-pow(p0_US,2)/2.0);
  double B = p1_US/TMath::Abs(p0_US) - TMath::Abs(p0_US);
  double C = (p1_US/TMath::Abs(p0_US))*(1.0/(p1_US-1.0))*exp(-p0_US*p0_US/2.0);
  double D = TMath::Sqrt(TMath::PiOver2())*(1.0+erf(TMath::Abs(p0_US)/TMath::Sqrt(2.0)));
  double N = 1.0/(p3_US*(C+D));

  // The Crystal Ball function is:
  if( (x-p2_US)/p3_US > -p0_US)
  {
    sig_US = p4_US * N * exp( -pow(x-p2_US,2) / (2.0*pow(p3_US,2)));
  }
  else
  {
    sig_US = p4_US * N * A * pow(B - (x-p2_US)/p3_US, -p1_US);
  }

  return sig_US;
}

double func_spec_US(double *x_val, double *par)
{
  // Crystal Ball lineshape plus exponential background plus mixed event combinatorial background
  // Used for LL fit to foreground data

  double x = x_val[0];

  // sig with Crystal Ball function
  double sig_US;
  // The four CB parameters (alpha, n, x_mean, sigma) plus normalization (N) are:
  double p0_US = par[0]; // alpha
  double p1_US = par[1]; // n
  double p2_US = par[2]; // x_mean
  double p3_US = par[3]; // sigma
  double p4_US = par[4]; // norm

  // we need:
  double A = pow( (p1_US/TMath::Abs(p0_US)),p1_US) * exp(-pow(p0_US,2)/2.0);
  double B = p1_US/TMath::Abs(p0_US) - TMath::Abs(p0_US);
  double C = (p1_US/TMath::Abs(p0_US))*(1.0/(p1_US-1.0))*exp(-p0_US*p0_US/2.0);
  double D = TMath::Sqrt(TMath::PiOver2())*(1.0+erf(TMath::Abs(p0_US)/TMath::Sqrt(2.0)));
  double N = 1.0/(p3_US*(C+D));

  // The Crystal Ball function is:
  if( (x-p2_US)/p3_US > -p0_US)
  {
    sig_US = p4_US * N * exp( -pow(x-p2_US,2) / (2.0*pow(p3_US,2)));
  }
  else
  {
    sig_US = p4_US * N * A * pow(B - (x-p2_US)/p3_US, -p1_US);
  }

  // Correlated exponential background
  double p0_Corr = par[5];
  double p1_Corr = par[6];

  double bkg_Corr = p0_Corr * exp(p1_Corr * x);

  // like-sign background
  double p0_LS = par[7];
  double p1_LS = par[8];
  double p2_LS = par[9];
  double p3_LS = par[10];
  double p4_LS = par[11];

  // double bkg_LS = p2_LS/pow(exp(-1.0*(p0_LS*x+p1_LS*pow(x,2)))+x/p3_LS,p4_LS);
  double bkg_LS = p2_LS * pow(exp(-1.0*(p0_LS*x+p1_LS*pow(x,2)))+x/p3_LS,p4_LS);

  double spec_US = sig_US +bkg_Corr +bkg_LS;

  return spec_US;
}

double func_spec_Exp(double *x_val, double *par)
{
  // Crystal Ball lineshape plus exponential background
  // Used for chisq fit to mixed event background data

  double x = x_val[0];

  // sig with Crystal Ball function
  double sig_US;
  // The four CB parameters (alpha, n, x_mean, sigma) plus normalization (N) are:
  double p0_US = par[0]; // alpha
  double p1_US = par[1]; // n
  double p2_US = par[2]; // x_mean
  double p3_US = par[3]; // sigma
  double p4_US = par[4]; // norm

  // we need:
  double A = pow( (p1_US/TMath::Abs(p0_US)),p1_US) * exp(-pow(p0_US,2)/2.0);
  double B = p1_US/TMath::Abs(p0_US) - TMath::Abs(p0_US);
  double C = (p1_US/TMath::Abs(p0_US))*(1.0/(p1_US-1.0))*exp(-p0_US*p0_US/2.0);
  double D = TMath::Sqrt(TMath::PiOver2())*(1.0+erf(TMath::Abs(p0_US)/TMath::Sqrt(2.0)));
  double N = 1.0/(p3_US*(C+D));

  // The Crystal Ball function is:
  if( (x-p2_US)/p3_US > -p0_US)
  {
    sig_US = p4_US * N * exp( -pow(x-p2_US,2) / (2.0*pow(p3_US,2)));
  }
  else
  {
    sig_US = p4_US * N * A * pow(B - (x-p2_US)/p3_US, -p1_US);
  }

  // Correlated exponential background
  double p0_Corr = par[5];
  double p1_Corr = par[6];

  double bkg_Corr = p0_Corr * exp(p1_Corr * x);

  double spec_Exp = sig_US + bkg_Corr;

  return spec_Exp;
}


/*
double Upscalc(double *xx, double *par)
{
  double x = xx[0];
  
  // The input parameters are:
  double alpha1s = par[0];
  double n1s = par[1];
  double sigma1s = par[2];
  double m1s = par[3];
  double m2s = par[4];
  double m3s = par[5];
  double N1s = par[6];
  double N2s = par[7];
  double N3s = par[8];
  double Nexp = par[9];
  double slope = par[10];

  // Construct the Y(1S) CB shape

  TF1 *f1 = new TF1("f1",CBcalc,7,11,5);
  f1->SetParameter(0,alpha1s);
  f1->SetParameter(1,n1s);
  f1->SetParameter(2,m1s);
  f1->SetParameter(3,sigma1s);
  f1->SetParameter(4,N1s);
  
  TF1 *f2 = new TF1("f2",CBcalc,7,11,5);
  f2->SetParameter(0,alpha1s);
  f2->SetParameter(1,n1s);
  f2->SetParameter(2,m2s);
  f2->SetParameter(3,sigma1s);
  f2->SetParameter(4,N2s);

  TF1 *f3 = new TF1("f3",CBcalc,7,11,5);
  f3->SetParameter(0,alpha1s);
  f3->SetParameter(1,n1s);
  f3->SetParameter(2,m3s);
  f3->SetParameter(3,sigma1s);
  f3->SetParameter(4,N3s);

  TF1 *fexp = new TF1("fexp","[0]*exp([1]*x)",7,11);
  fexp->SetParameter(0,Nexp);
  fexp->SetParameter(1,slope);
  double mass = f1->Eval(x) + f2->Eval(x) + f3->Eval(x) + fexp->Eval(x);

  return mass;
}
*/
