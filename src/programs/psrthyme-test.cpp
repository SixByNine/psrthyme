//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>

#include <psrthyme/psrthyme.hpp>
#include <psrthyme/thymechive.hpp>
#include "psrthyme-test.hpp"


int main(int argc, char** argv){
   Psrthyme::setup();
   std::cout << PsrthymeTelescope::getTelescopeFromId("PARKES")->getCode() << std::endl;
   std::cout << PsrthymeTelescope::getTelescopeFromId("jb")->getCode() << std::endl;
   std::cout << PsrthymeTelescope::getTelescopeFromId("7")->getCode() << std::endl;
   std::cout << PsrthymeTelescope::getTelescopeFromId("8")->getCode() << std::endl;

   ThymeChiveArchive::Ptr archive = 
	  ThymeChiveArchive::Ptr(
			new ThymeChiveArchive("/Users/mkeith/Projects/AGL/2032+4127/DATA/J100203_183904.FTp"));

   std::cout << archive->get_MJD() << std::endl;
   exit(1);

   PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("../../../kwikfit/examples/psr1.tmpl");

   tmpl->write(std::cout);

   PsrthymeProfile::Ptr obs = PsrthymeProfile::Ptr(new PsrthymeProfile("../../../kwikfit/examples/psr1.asc"));
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
