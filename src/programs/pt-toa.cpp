//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <fftw3.h>

#include <fstream>
#include <iostream>
#include <psrthyme/psrthyme.hpp>
#include "pt-toa.hpp"

#include "help/pt-toa.help"

int main(int argc, char** argv){

   std::stringstream cmdline;
   for(uint64_t i=0; i < argc; i++)cmdline << argv[i] << " ";
   if (argc==1 || getB("--help","-h",argc,argv,0)){
	  std::cout << argv[0] << std::endl;
	  std::cout.write(reinterpret_cast<char*>(help_pt_toa_txt),help_pt_toa_txt_len);
	  exit(1);
   }


   const char* std_filename = getS("--std","-s",argc,argv,"psr.tmpl");
   debugFlag = getB("--debug","", argc,argv,0);
   const char* grdev = getS("--dev","-D", argc,argv,"/xs");
   const char* outfile = getS("--out","-o", argc,argv,"");
   bool nlf = getB("--lmfit","-l", argc,argv,0);
   bool plot = getB("--plot","-p", argc,argv,0);
   bool turns = getB("--turns","-R", argc,argv,0);
      fftw_import_wisdom_from_filename("psrthyme.wis");

   logdbg("initialise psrthyme routines");
   Psrthyme::setup();

   logdbg("read template");
   const PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read(std_filename);

   logdbg("initialise fitter");
   PsrthymeGenericFitter::Ptr fitter;
  
   if (nlf){
	  logmsg("Using LM non-linear fitter");
	  PsrthymeLMFitter::Ptr lm_fitter = (PsrthymeLMFitter::Ptr(new PsrthymeLMFitter()));
	  lm_fitter->enable_scatter = (bool)getB("--scatter","", argc,argv,0);
	  lm_fitter->enable_smear = (bool)getB("--nosmear","", argc,argv,1);
	  lm_fitter->initial_nit = getI("--init_lm_itrs","", argc,argv,4);
	  lm_fitter->final_nit = getI("--init_lm_itrs","", argc,argv,25);

	  fitter = lm_fitter;
   } else {
	  fitter = (PsrthymeGenericFitter::Ptr(new PsrthymeFitter()));
   }
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

	  logmsg("reading '%s'",argv[iarg]);
	  PsrthymeArchive::Ptr archive = PsrthymeArchive::Ptr( new PsrthymeArchive(argv[iarg]));
	  for (uint64_t isub=0; isub < archive->getNsub(); isub++){
		 for (uint64_t ichan=0; ichan < archive->getNchans(); ichan++){
			logdbg("isub=%d ichan=%d",isub,ichan);

			PsrthymeProfile::Ptr obs = archive->getProfile(isub,ichan,0);
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
   fftw_export_wisdom_to_filename("psrthyme.wis");

   fftw_cleanup_threads();

   return 0;
}
