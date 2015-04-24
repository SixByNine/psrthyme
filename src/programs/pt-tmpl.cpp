//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <TKlog.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>

#include <fstream>
#include <iostream>
#include <psrthyme/psrthyme.hpp>
#include "pt-tmpl.hpp"

#include "help/pt-tmpl.help"

int main(int argc, char** argv){

   std::stringstream cmdline;
   for(uint64_t i=0; i < argc; i++)cmdline << argv[i] << " ";
   if (argc==1 || getB("--help","-h",argc,argv,0)){
	  std::cout << argv[0] << std::endl;
	  std::cout.write(reinterpret_cast<char*>(help_pt_tmpl_txt),help_pt_tmpl_txt_len);
	  exit(1);
   }


   const char* std_filename = getS("--std","-s",argc,argv,"");
   const char* filename = getS("--file","-f",argc,argv,"");
   const char* grdev = getS("--dev","-D", argc,argv,"/xs");

   logmsg("initialise psrthyme routines");
   Psrthyme::setup();
   PsrthymeTemplate::Ptr tmpl;
   if(strlen(std_filename)>0){
	  logmsg("read template");
	  tmpl = PsrthymeTemplate::read(std_filename);
   }
   PsrthymeArchive::Ptr archive;
   if(strlen(filename)>0){
	  logmsg("read archive");
	  archive = PsrthymeArchive::Ptr( new PsrthymeArchive(filename));
   }

   logmsg("Start template editor");
   PsrthymeTemplateEditor editor(grdev,tmpl,archive);
   editor.run();

   return 0;
}
