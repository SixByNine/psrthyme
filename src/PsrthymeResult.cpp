//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>

// include header generated by makeheaders
#include "PsrthymeResult.hpp"

#if INTERFACE
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
class PsrthymeResult {
   public:
	  SparseList::Ptr chisq_space;
	  PsrthymeTemplate::Ptr tmpl;
	  PsrthymeProfile::Ptr obsn;
	  std::vector<double> amp_values;
	  std::vector<double> amp_errors;
	  PsrthymeMatrix::Ptr amp_cvm;
	  double phase;
	  double error;
	  PsrthymeResult() {
	  };
	  typedef boost::shared_ptr<PsrthymeResult> Ptr;
};
#endif

