#include <fftw3.h>
#include "convolve.hpp"


LOCAL void fourier(const int N, double* fw, fftw_complex* ft){
   fftw_plan p;
   p = fftw_plan_dft_r2c_1d(N, fw, ft, FFTW_MEASURE);
   fftw_execute(p);
   fftw_destroy_plan(p);
}

LOCAL void ifourier(const int N, fftw_complex* ft, double* fw){
   fftw_plan p;
   p = fftw_plan_dft_c2r_1d(N, ft, fw, FFTW_MEASURE);
   fftw_execute(p);
   fftw_destroy_plan(p);
}

void convolve(const int N, double* fw, double* gw, double* hw){
   fftw_complex *ft, *gt, *ht;
   const int ncomp=N/2+1;

   ft = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));
   gt = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));
   ht = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*(ncomp)));

   fourier(N, fw, ft);
   fourier(N, gw, gt);
   const double scale=1.0/N/N;

   for(int t=0;t<ncomp;t++){
	  ht[t][0] = (ft[t][0]*gt[t][0] - ft[t][1]*gt[t][1])*scale;
	  ht[t][1] = (ft[t][0]*gt[t][1] + ft[t][1]*gt[t][0])*scale;
   }

   ifourier(N, ht, hw);

   fftw_free(ft);
   fftw_free(gt);
   fftw_free(ht);
}
