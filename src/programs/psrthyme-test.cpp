//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>
#include <boost/foreach.hpp>

#include <psrthyme/psrthyme.hpp>
#include "psrthyme-test.hpp"


int main(int argc, char** argv){
   Psrthyme::setup();
   PsrthymeArchive::Ptr archive = 
	  PsrthymeArchive::Ptr(
			new PsrthymeArchive("/Users/mkeith/Projects/AGL/2032+4127/DATA/J100203_183904.testy"));

   std::cout << archive->getStartTime() << std::endl;
   std::cout << archive->getTelescope() << std::endl;

//  PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("../../../kwikfit/examples/psr1.tmpl");
   PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("/Users/mkeith/Projects/AGL/2032+4127/DATA/paas.tmpl");

   tmpl->write(std::cout);

//   PsrthymeProfile::Ptr obs = PsrthymeProfile::readASCII("../../../kwikfit/examples/psr1.asc");
   PsrthymeProfile::Ptr obs = archive->getProfile(0,0,0);



   PsrthymeFitter::Ptr fitter = PsrthymeFitter::Ptr(new PsrthymeFitter());
   fitter->clear();
   fitter->addItteration(4,false,false);
//   fitter->addItteration(4,false,false);
//   fitter->addItteration(32,false,false);
   fitter->addItteration(4,true,false);
   fitter->addItteration(4,true,false);
   fitter->addItteration(4,true,false);
   fitter->addItteration(4,true,false);
//   fitter->addItteration(8,true,true);
fitter->addItteration(32,true,true);
//fitter->addItteration(128,true,true);
   fitter->setTemplate(tmpl);

   PsrthymeResult::Ptr result = fitter->fitTo(obs);
   PsrthymePlotter plt("2/xs");
   plt.plot(result);
   logmsg("PHASE = %lf",result->phase);
   logmsg("ERROR = %lf",result->error);

   PsrthymeToA toa = result->getToA();
   std::cout << toa.toString() << std::endl;

   return 0;
}
