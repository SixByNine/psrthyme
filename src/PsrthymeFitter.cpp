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

// include header generated by makeheaders
#include "PsrthymeFitter.hpp"

#if INTERFACE
#include <list>
#include <boost/shared_ptr.hpp>

class PsrthymeFitter {
   private:
	  struct Itteration{
		 uint64_t resolution;
		 bool cholesky;
		 bool zoom;
		 Itteration(uint64_t resolution, bool cholesky, bool zoom){
			this->resolution = resolution;
			this->cholesky = cholesky;
			this->zoom = zoom;
		 }
	  };
	  PsrthymeTemplate::Ptr tmpl;
	  std::list<Itteration> itterations;
   public:
	  typedef boost::shared_ptr<PsrthymeFitter> Ptr;
	  PsrthymeFitter() {
		 this->useDefaults();
	  } 
	  void setTemplate(PsrthymeTemplate::Ptr tmpl){
		 this->tmpl = tmpl;
	  }
	  void useDefaults(){
		 this->clear();
		 this->itterations.push_front(Itteration(32,true,true));
		 this->itterations.push_front(Itteration(8,true,true));
		 this->itterations.push_front(Itteration(1,true,false));
		 this->itterations.push_front(Itteration(1,true,false));
		 this->itterations.push_front(Itteration(1,false,false));
	  }
	  void clear(){
		 this->itterations.clear();
	  }
	  void addItteration(uint64_t res, bool cholesky, bool zoom){
		 this->itterations.push_back(Itteration(res,cholesky,zoom));
	  }
	  PsrthymeResult::Ptr fitTo(PsrthymeObservation::Ptr obs);
};

#endif

std::vector<double> getCovarianceFunction (const std::vector<double> &profile){
   uint64_t i,j;
   const uint64_t nbins = profile.size();
   std::vector<double> cov(profile);
   double mean=0;
   for(i = 0; i < nbins; i++){
	  mean+=profile[i];
	  cov[i]=0;
   }
   LOGDBG << "nbins="<<nbins <<std::endl;
   LOGDBG << "mean="<<mean <<std::endl;
   mean /= double(nbins);
   for(j = 0; j < nbins; j++){
	  for(i = 0; i < nbins; i++){
		 cov[j] += (profile[i]-mean)*(profile[(i+j)%nbins]-mean);
	  }
   }
   for(i = 0; i < nbins; i++){
	  cov[i] /= double(nbins);
   }

   return cov;
}



PsrthymeResult::Ptr PsrthymeFitter::fitTo(PsrthymeObservation::Ptr obs){
   std::list<Itteration> ittrs(this->itterations);
   const uint64_t nbins = obs->getNbins();
   const uint64_t nfit = this->tmpl->size()+1;
   std::vector<double> residuals(obs->profile);
   std::vector<double> best_profile(obs->profile);
   LOGDBG << "nit = " << ittrs.size() << std::endl;
   PsrthymeMatrix::Ptr outCVM = PsrthymeMatrix::Ptr(new PsrthymeMatrix(nfit));
   boost::shared_ptr<double[]> outP(new double[nfit]);
   boost::shared_ptr<double[]> outErr(new double[nfit]);

   PsrthymeMatrix::Ptr bestCVM = PsrthymeMatrix::Ptr(new PsrthymeMatrix(nfit));
   boost::shared_ptr<double[]> bestP(new double[nfit]);
   boost::shared_ptr<double[]> bestErr(new double[nfit]);
   
   const double tol = 1.0e-27;

   SparseList::Ptr chisq_space = SparseList::Ptr(new SparseList(0,1,8));
   chisq_space->insert(0,1e99);

	  double best_chisq = std::numeric_limits<double>::max();
	  double best_phase = 0;
   while(ittrs.size() > 0){
	  Itteration itr = ittrs.front();
	  ittrs.pop_front();
	  LOGDBG << "nit = " << ittrs.size() << std::endl;
	  const uint64_t nphase_steps = nbins * itr.resolution;
	  const double prev_best_phase=chisq_space->min();
	  LOGDBG << "prev_phase = " << prev_best_phase << std::endl;
	  const double prev_best_chisq=chisq_space->get(prev_best_phase);
	  LOGDBG << "prev_chisq = " << prev_best_chisq << std::endl;
	  const double chisq_cut = prev_best_chisq*3.0;
	  std::map<uint64_t, std::vector<double> > yvals;
	  std::map<uint64_t, std::vector<double> > white_yvals;

	  best_chisq = std::numeric_limits<double>::max();
	  best_phase = 0;
	  std::map<uint64_t, PsrthymeMatrix::Ptr > designMatricies;
	  std::map<uint64_t, PsrthymeMatrix::Ptr > white_designMatricies;

	  LOGDBG << "Alloc UINV " << std::endl;
	  LOGDBG << "Get covar function" << std::endl;
	  PsrthymeMatrix::Ptr covMatrix = PsrthymeMatrix::Ptr(new PsrthymeMatrix(nbins));
	  std::vector<double> cov = getCovarianceFunction(residuals);
	  logmsg("var(data) = %lf",cov[0]);


	  if (itr.cholesky) {
		 covMatrix->addCVF(cov);
		 covMatrix->addDiagonal(cov[0]*1e-9);
	  } else {
		 covMatrix->addDiagonal(cov[0]);
	  }

	  PsrthymeMatrix::Ptr uinv = PsrthymeMatrix::Ptr(new PsrthymeMatrix(nbins));
	  cholesky_formUinv(uinv->c_arr(),covMatrix->c_arr(), nbins);

	  chisq_space->setResolution(nphase_steps);

	  for(uint64_t iphase = 0; iphase < nphase_steps; iphase++){
		 double phase = double(iphase)/double(nphase_steps); 
		 if(!itr.zoom || chisq_space->get(phase) < chisq_cut){
			// we want to re-do this bin!
			uint64_t ibin = iphase/itr.resolution;
			uint64_t sbin = iphase-ibin*itr.resolution;
			if ( yvals.count(ibin) == 0 ) {
			   // need to generate the rotated profile
			   //	   LOGDBG << "make rotprof for "<< ibin << std::endl;
			   std::vector<double> rotated(obs->profile);
			   std::rotate(rotated.begin(),rotated.begin()+ibin,rotated.end());
			   white_yvals[ibin] = (*uinv)*rotated;
			   yvals[ibin] = rotated;
			}
			if ( designMatricies.count(sbin) == 0 ) {
			   // need to generate the DM for this sub-phase
			   //	   LOGDBG << "make dm for "<< sbin << std::endl;
			   PsrthymeMatrix::Ptr dm = this->tmpl->getDesignMatrix(nbins,double(sbin)/double(nphase_steps));
			   PsrthymeMatrix::Ptr wdm = (*uinv)*dm;
			   designMatricies[sbin] = dm;
			   white_designMatricies[sbin] = wdm;
			}

			/*
			 * double TKleastSquares(double* b, double* white_b,
			 *       double** designMatrix, double** white_designMatrix,
			 *             int n,int nf, double tol, char rescale_errors,
			 *                   double* outP, double* e, double** cvm)
			 */

			double chisq = TKleastSquares(arr(yvals[ibin]),arr(white_yvals[ibin]),
				  designMatricies[sbin]->c_arr(),white_designMatricies[sbin]->c_arr(),
				  nbins,nfit,tol,1,arr(outP),arr(outErr),outCVM->c_arr());
			chisq_space->insert(phase,chisq);
			if(chisq < best_chisq) {
			   logmsg("phase %lf chisq %lg",phase,chisq);
			   best_profile = (*designMatricies[sbin])*outP;
			   std::rotate(best_profile.begin(),best_profile.end()-ibin,best_profile.end());
			   best_chisq = chisq;
			   best_phase = phase;
			   bestP.swap(outP);
			   bestCVM.swap(outCVM);
			   bestErr.swap(outErr);
			}
		 }
	  }

	  std::transform(obs->profile.begin(), obs->profile.end(),
			best_profile.begin(), residuals.begin(), absdiff);
   }

   double error_estimate=0;
   PsrthymeResult::Ptr result = PsrthymeResult::Ptr(new PsrthymeResult());
   result->chisq_space = chisq_space;
   result->tmpl = this->tmpl;
   result->obsn = obs;
   result->amp_values = std::vector<double>(arr(bestP),arr(bestP)+nfit);
   result->amp_errors = std::vector<double>(arr(bestErr),arr(bestErr)+nfit);
   result->amp_cvm = bestCVM;
   result->phase=best_phase;
   result->error=error_estimate;
   return result;
}

double absdiff(double a, double b){
   return fabs(a-b);
}
