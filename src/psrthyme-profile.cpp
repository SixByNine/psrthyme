//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>


// include header generated by makeheaders
#include "psrthyme-profile.hpp"

#if INTERFACE
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
class PsrthymeProfile : public PsrthymeComponent{
   private:
	  std::vector<PsrthymeComponent::Ptr> components;
	  std::string name;
   public:
	  typedef boost::shared_ptr<PsrthymeProfile> Ptr;
	  PsrthymeProfile(std::string name); // constructor
	  double getValue(double phase);
	  void addComponent(PsrthymeComponent::Ptr comp){
		 this->components.push_back(comp);
	  }
	  std::string getName(){
		 return this->name;
	  }
	  uint64_t nprof(){
		return this->components.size();
	  }
	  void write(std::ostream &out);
}

#endif

PsrthymeProfile::PsrthymeProfile(std::string name) {
   this->name = name;
}

double PsrthymeProfile::getValue(double phase){
   double val=0;
   for(std::vector<PsrthymeComponent::Ptr>::iterator it = this->components.begin(); it != this->components.end(); ++it) {
	  val += (*it)->getValue(phase);
   }
   return val;
}

void PsrthymeProfile::write(std::ostream &out){
   out << "\tPROFILE \t" << this->getName() << std::endl;
   for(uint64_t i=0; i<this->components.size(); i++){
	  this->components[i]->write(out);
   }
   out << "\t# END   \t" << this->getName() << std::endl;
}
