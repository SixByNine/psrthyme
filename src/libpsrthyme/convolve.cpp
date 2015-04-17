#include <complex>
#include <cstring>
#include <fftw3.h>
#include "convolve.hpp"


void convolve(const int N, const double* fw, const double* gw, double* hw){
   fftw_complex *ft, *gt, *ht;
   double *fi, *gi;
   double *ho;
   const int ncomp=N/2+1;

   fi = reinterpret_cast<double*>(fftw_malloc(sizeof(double)*(N)));
   gi = reinterpret_cast<double*>(fftw_malloc(sizeof(double)*(N)));
   ho = reinterpret_cast<double*>(fftw_malloc(sizeof(double)*(N)));

   ft = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));
   gt = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));
   ht = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));

   fftw_plan fP = fftw_plan_dft_r2c_1d(N, fi, ft, FFTW_MEASURE);
   fftw_plan gP = fftw_plan_dft_r2c_1d(N, gi, gt, FFTW_MEASURE);
   fftw_plan hP = fftw_plan_dft_c2r_1d(N, ht, ho, FFTW_MEASURE);

   memcpy(fi,fw,sizeof(double)*N);
   memcpy(gi,gw,sizeof(double)*N);

   fftw_execute(fP);
   fftw_execute(gP);
   const double scale=1.0/N/N;

   for(int t=0;t<ncomp;t++){
	  ht[t][0] = (ft[t][0]*gt[t][0] - ft[t][1]*gt[t][1])*scale;
	  ht[t][1] = (ft[t][0]*gt[t][1] + ft[t][1]*gt[t][0])*scale;
   }

   fftw_execute(hP);

   memcpy(hw,ho,sizeof(double)*N);

   fftw_destroy_plan(fP);
   fftw_destroy_plan(gP);
   fftw_destroy_plan(hP);

   fftw_free(ft);
   fftw_free(gt);
   fftw_free(ht);
   fftw_free(fi);
   fftw_free(gi);
}
