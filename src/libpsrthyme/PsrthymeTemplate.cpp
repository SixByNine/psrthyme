//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

// include header generated by makeheaders
#include "PsrthymeTemplate.hpp"

#if INTERFACE
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
class PsrthymeTemplate : public std::vector<PsrthymeTemplateSection::Ptr>{
    private:
        std::string name;
        PsrthymeTemplate(){}; // constructor
    public:
        typedef boost::shared_ptr<PsrthymeTemplate>  Ptr;
        static PsrthymeTemplate::Ptr read(const char* filename);
        void read(std::istream &infile);
        PsrthymeMatrix::Ptr getDesignMatrix(uint64_t nbins, double offset_phase);
        void write(std::ostream &out);
        void addProfile(PsrthymeTemplateSection::Ptr profile){
            this->push_back(profile);
        }
};



#define strings_equal(a,b) (a.compare(b)==0)
#endif


bool isNumber(std::string str){
    try 
    {
        double x = boost::lexical_cast<double>(str); // double could be anything with >> operator.
        return true;
    }
    catch(...) { 
        return false;
    }
}

PsrthymeTemplate::Ptr PsrthymeTemplate::read(const char* filename){
    PsrthymeTemplate::Ptr tmpl = PsrthymeTemplate::Ptr(new PsrthymeTemplate());
    std::ifstream infile(filename);
    tmpl->read(infile);
    return tmpl;
}

void PsrthymeTemplate::read(std::istream &infile){
    std::string line;
    std::string str;

    uint64_t iComp=-1;
    uint64_t iProf=-1;

    while (std::getline(infile, line)) {
        boost::trim(line);


        if (line[0]=='#' || line[0]=='\0')continue;

        std::stringstream ss(line);

        ss >> str; // read the first keyword


        if (strings_equal(str,"KWTEMPLATE") || strings_equal(str,"TEMPLATE")){
            ss >> this->name;
            continue;
        }
        if (strings_equal(str,"KWPROFILE") || strings_equal(str,"PROFILE")){
            ss >> str;
            PsrthymeTemplateSection::Ptr profile = PsrthymeTemplateSection::Ptr(new PsrthymeTemplateSection(str));
            this->addProfile(profile);
            continue;
        }

        if (strings_equal(str,"VON_MISES") || isNumber(str) ){
            double height,centre,conc;
            if(!strings_equal(str,"VON_MISES")){
                ss.seekg(0);
            }
            ss >> centre;
            ss >> conc;
            ss >> height;
            VonMisesComponent::Ptr component = VonMisesComponent::Ptr(new VonMisesComponent(height,centre,conc));
            this->back()->addComponent(component);
        }
        if (strings_equal(str,"EXGAUSS")){
            double mu,sigma,lambda,amp;
            ss >> mu;
            ss >> sigma;
            ss >> lambda;
            ss >> amp;
            ExpModGaussComponent::Ptr component = ExpModGaussComponent::Ptr(new ExpModGaussComponent(mu,sigma,lambda,amp));
            this->back()->addComponent(component);
        }

    }
}



void PsrthymeTemplate::write(std::ostream &out){
    out << "TEMPLATE \t" << this->name << std::endl;
    uint64_t iComp,iProf;
    for (iProf=0; iProf < this->size(); iProf++){
        this->at(iProf)->write(out);
    }

    out << "# END    \t"<<this->name << std::endl;
}

PsrthymeMatrix::Ptr PsrthymeTemplate::getDesignMatrix(uint64_t nbins,double offset_phase){
    PsrthymeMatrix::Ptr dm(new PsrthymeMatrix(nbins,this->size()+1));
    double dp = 0.5/double(nbins);
    for (uint64_t ibin=0; ibin < nbins; ibin++){
        // We reference the leading edge of the bin as phase 0
        // so add half a bin to get to centre of a bin
        double phase = PsrthymeResult::correctPhase((double(ibin)+0.5)/double(nbins) - offset_phase);
        for (uint64_t iProf=0; iProf < this->size(); iProf++){
            (*dm)[ibin][iProf] = this->at(iProf)->getValue(phase-dp,phase+dp);
        }
        (*dm)[ibin][this->size()]=1;
    }

    return dm;
}

