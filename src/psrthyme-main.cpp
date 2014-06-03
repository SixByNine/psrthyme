//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <cpgplot.h>

#include "psrthyme-main.hpp"


int main(int argc, char** argv){

   PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::read("/Users/mkeith/Projects/kwikfit_tests/0525/mjk.tmpl");

   tmpl->write(std::cout);

   return 0;
}
