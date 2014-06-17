//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>
#include <psrthyme/psrthyme.hpp>
#include "pt-toa.hpp"

int main(int argc, char** argv){

   std::stringstream cmdline;
   for(uint64_t i=0; i < argc; i++)cmdline << argv[i] << " ";

   const char* std_filename = getS("--std","-s",argc,argv,"psr.tmpl");
   debugFlag = getB("--debug","", argc,argv,0);
   const char* grdev = getS("--dev","-D", argc,argv,"/xs");
   const char* outfile = getS("--out","-o", argc,argv,"");
   bool plot = getB("--plot","-p", argc,argv,0);
   bool turns = getB("--turns","-R", argc,argv,0);


   logdbg("initialise psrthyme routines");
   Psrthyme::setup();

   logdbg("read template");
   const PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read(std_filename);

   logdbg("initialise fitter");
   PsrthymeFitter::Ptr fitter = PsrthymeFitter::Ptr(new PsrthymeFitter());
   fitter->setTemplate(tmpl);

   std::ostream *out = &std::cout;
   if (outfile[0] != '\0'){
	  logdbg("open output file");
	  out = new std::ofstream(outfile);
   }
   

   logdbg("infiles...");
   getArgs(&argc, argv);
   logdbg("There are  %d files to process",argc-1);
   *out << "FORMAT 1" << std::endl;
   *out << "# Produced by psrthyme (M. Keith 2014, University of Manchester)" << std::endl;
   std::string str = (boost::format("%.75s") % cmdline.str()).str();
   *out << "# " << str; 
   if (cmdline.str().size() > 75) *out << "...";
   *out << std::endl;

   for (uint64_t iarg=1; iarg < argc; iarg++){

	  logdbg("reading '%s'",argv[iarg]);
	  PsrthymeArchive::Ptr archive = PsrthymeArchive::Ptr( new PsrthymeArchive(argv[iarg]));
	  for (uint64_t isub=0; isub < archive->getNsub(); isub++){
		 for (uint64_t ichan=0; ichan < archive->getNchans(); ichan++){
			logdbg("isub=%d ichan=%d",isub,ichan);

			PsrthymeProfile::Ptr obs = archive->getProfile(0,0,0);
			logdbg("DOING THE FIT");
			PsrthymeResult::Ptr result = fitter->fitTo(obs);
			logdbg("DONE FIT");
			if (turns){
			   *out << argv[iarg] << "\t" << result->phase << "\t" << result->error << std::endl; 
			} else {
			   PsrthymeToA toa = result->getToA();
			   *out << toa.toString() << std::endl;
			}
			if (plot){
			   PsrthymePlotter plt(grdev);
			   plt.plot(result);
			}

		 }
	  }
   }


   return 0;
}
