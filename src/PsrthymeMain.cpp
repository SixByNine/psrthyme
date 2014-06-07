//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>

#include "PsrthymeMain.hpp"


int main(int argc, char** argv){

   PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("../../kwikfit/examples/psr1.tmpl");

   tmpl->write(std::cout);

   PsrthymeObservation::Ptr obs = PsrthymeObservation::Ptr(new PsrthymeObservation("../../kwikfit/examples/psr1.asc"));
		 obs->read();

   PsrthymeFitter::Ptr fitter = PsrthymeFitter::Ptr(new PsrthymeFitter());
   fitter->clear();
   fitter->addItteration(4,false,false);
   fitter->addItteration(4,true,false);
   fitter->addItteration(8,true,true);
   fitter->addItteration(32,true,true);
   fitter->addItteration(128,true,true);
   fitter->setTemplate(tmpl);

   PsrthymeResult::Ptr result = fitter->fitTo(obs);
   PsrthymePlotter plt("2/xs");
   plt.plot(result);
   logmsg("PHASE = %lf",result->phase);
   logmsg("ERROR = %lf",result->error);

   return 0;
}
