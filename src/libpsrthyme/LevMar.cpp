//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include "levmar/levmar.h"
#include <TKlog.h>

// include header generated by makeheaders
#include "LevMar.hpp"

#if EXPORT_INTERFACE
#include <boost/shared_ptr.hpp>
#include <vector>

class LevMar {
   private:
   public:

	  virtual bool hasJacobian(){
		 return false;
	  }
	  virtual std::vector<double> evaluate(std::vector<double> p) = 0;
	  virtual std::vector<double> jacobian(std::vector<double> p){
		 std::vector<double> ret(0);
		 return ret;
	  }

	  virtual uint32_t getMaxIterations(){
		 return 100;
	  }
	  PsrthymeMatrix doFit(std::vector<double> &params, const std::vector<double> &yvals);
};

#endif

using std::vector;
// PRIVATE callback functions to pass to the levmar library
extern "C" void lm_callback_func_d(double *p, double *hx, int m, int n, void *adata);
extern "C" void lm_callback_jacf_d(double *p, double *j, int m, int n, void *adata);

PsrthymeMatrix LevMar::doFit(vector<double> &params, const vector<double> &yvals){
   PsrthymeMatrix covar(params.size(),params.size());
   vector<double> y(yvals.begin(),yvals.end());

   if(this->hasJacobian()){
	  dlevmar_der(lm_callback_func_d,lm_callback_jacf_d,
			&params[0], &y[0],
			params.size(), y.size(),
			this->getMaxIterations(),NULL,NULL,NULL,covar.c_arr()[0],(void*)this);
   } else {
	  dlevmar_dif(lm_callback_func_d,
			&params[0], &y[0],
			params.size(), y.size(),
			this->getMaxIterations(),NULL,NULL,NULL,covar.c_arr()[0],(void*)this);

   }
   return covar;
}


/**
 * Callbacks from C to C++
 */
void lm_callback_func_d(double *p, double *hx, int m, int n, void *adata){
   const vector<double> _p = vector<double>(p,p+m);
   vector<double>_hx = ((LevMar*)adata)->evaluate(_p);
   std::copy(_hx.begin(),_hx.end(),hx);
}
void lm_callback_jacf_d(double *p, double *j, int m, int n, void *adata){
   const vector<double> _p = vector<double>(p,p+m);
   vector<double>_j = ((LevMar*)adata)->jacobian(_p);
   std::copy(_j.begin(),_j.end(),j);
}

