//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <cpgplot.h>

#include "PsrthymeMain.hpp"


int main(int argc, char** argv){

   PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("../../kwikfit/examples/psr1.tmpl");

   tmpl->write(std::cout);

   PsrthymeProfile::Ptr obs = PsrthymeProfile::Ptr(new PsrthymeProfile("../../kwikfit/examples/psr1.asc"));
		 obs->read();

   PsrthymeFitter::Ptr fitter = PsrthymeFitter::Ptr(new PsrthymeFitter());
   //fitter->clear();
   //fitter->addItteration(1,false,false);
   fitter->setTemplate(tmpl);

   fitter->fitTo(obs);

   return 0;
}
