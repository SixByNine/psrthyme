#include <algorithm>
#include <cpgplot.h>
#include <iostream>
#include <boost/foreach.hpp>


#include "debug.hpp"

#if INTERFACE
#include <vector>
class debug{
   public:
	  static void plot(const std::vector<double> &in,const char* str);
};
#endif

void debug::plot(const std::vector<double> &in, const char* str){
   cpgopen("/xs");
   cpgsvp(0.1,0.9,0.1,0.9);
   cpgswin(0,in.size(),*std::min_element(in.begin(),in.end()),*std::max_element(in.begin(),in.end()));
   cpgbox("ABCTSN",0,0,"ABCTSN",0,0);
   cpglab("","",str);
 
   cpgmove(0,in[0]);
   float i=0;
	  BOOST_FOREACH (double it, in){
	  cpgsci(2);
	  cpgdraw(i,it);
	  cpgsci(1);
	  cpgpt1(i,it,1);
	  i+=1;
   }

   cpgclos();
     std::cin.ignore();

   
}

