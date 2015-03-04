//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <algorithm>
#include <TKcholesky.h>
#include <TKfit.h>
#include <TKlog.h>
#include <cmath>
#include <limits>
#include <map>
#include <boost/foreach.hpp>

// include header generated by makeheaders
#include "PsrthymeLMFitter.hpp"
#define TIMEIT 1

#if INTERFACE
#include <list>
#include <boost/shared_ptr.hpp>

class PsrthymeLMFitter : public LevMar, public PsrthymeGenericFitter {
   private:
	  PsrthymeTemplate::Ptr tmpl;
	  PsrthymeProfile::Ptr obs;
   public:
	  typedef boost::shared_ptr<PsrthymeLMFitter> Ptr;

	  PsrthymeLMFitter() {
	  } 

	  void setTemplate(PsrthymeTemplate::Ptr tmpl){
		 this->tmpl = tmpl;
	  }
	  PsrthymeResult::Ptr fitTo(PsrthymeProfile::Ptr obs);

	  // overide LevMar functions
	  bool hasJacobian(){
		 return false;
	  }

	  std::vector<double> evaluate(std::vector<double> p){
		 std::vector<double> amps(p.begin()+1,p.begin()+this->tmpl->size()+2);
		 PsrthymeMatrix::Ptr designMatrix = this->tmpl->getDesignMatrix(this->obs->getNbins(), p[0]);
		 std::vector<double> outprof = designMatrix*amps;
		 return outprof;
	  }

	  std::vector<double> jacobian(std::vector<double> p){
		 std::vector<double> ret(0);
		 return ret;
	  }
	  uint32_t getMaxIterations(){
		 return 100;
	  }

}

#endif

using std::vector;

PsrthymeResult::Ptr PsrthymeLMFitter::fitTo(PsrthymeProfile::Ptr obs){
   this->obs = obs;
   // params:
   // [0] = phase
   // [1..tmpl->size()-1] = amplitudes
   // [tmpl->size()] = baseline
   vector<double> params(this->tmpl->size()+2,1);
   params[0]=0;
   double best_chisq=0;
   uint64_t nfit = params.size();
   std::vector<double> residuals(obs->getNormalisedProfile());
   
   
   SparseList::Ptr chisq_space = SparseList::Ptr(new SparseList(0,1,4));
   chisq_space->insert(0,1);
   chisq_space->insert(0.25,1.5);
   chisq_space->insert(0.5,1.3);
   chisq_space->insert(0.75,1.1);

   PsrthymeMatrix covar = LevMar::doFit(params,residuals);

   std::vector<double> best_profile = this->evaluate(params);

   double best_phase=params[0];



   PsrthymeResult::Ptr result = PsrthymeResult::Ptr(new PsrthymeResult());
   result->phase=best_phase;
   result->chisq = best_chisq;
   std::transform(obs->getNormalisedProfile().begin(), obs->getNormalisedProfile().end(),
		 best_profile.begin(), residuals.begin(), PsrthymeFitter::diff);

   result->tmpl = this->tmpl;
   result->obsn = obs;
   result->amp_values = std::vector<double>(params.begin()+1,params.begin()+1+this->tmpl->size());

   for (uint32_t i=0; i < this->tmpl->size(); i++){
   result->amp_errors.push_back(sqrt(covar[i+1][i+1]));
   }

   result->chisq_space = chisq_space;
   //result->amp_cvm = bestCVM;
   result->residual = residuals;
   result->error = sqrt(covar[0][0]);
   result->best_profile = best_profile;
   result->nfree=obs->getNbins()-nfit-1;
   result->nfit=nfit;
   result->phase=best_phase;
   result->chisq = best_chisq;

   return result;

}




