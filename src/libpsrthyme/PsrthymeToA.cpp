#include <PsrthymeToA.hpp>

#include <TKlog.h>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#if INTERFACE
#include <MJD.h>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <string>
#include <map>
class PsrthymeToA {
   private:
	  MJD toa;
	  double error;
	  double freq;
	  std::string name;
	  PsrthymeTelescope::Ptr telescope;
   public:
	  std::map<std::string,std::string> flags;
	  typedef boost::shared_ptr<PsrthymeToA> Ptr;
	  PsrthymeToA(const std::string &name, const MJD &toa, double error_s,double freq, PsrthymeTelescope::Ptr telescope) {
		 this->toa = toa;
		 this->error = error_s;
		 this->freq = freq;
		 this->telescope = telescope;
		 this->name=name;
	  };
	  void setFlag(std::string flag, std::string value){
		 flag.erase(remove_if(flag.begin(), flag.end(), isspace), flag.end());
		 value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
		 this->flags[flag] = value;
	  }
	  void clearFlag(std::string flag){
		 flag.erase(remove_if(flag.begin(), flag.end(), isspace), flag.end());
		 this->flags.erase(flag);
	  }
	  std::string toString() const;
};
#endif

std::string PsrthymeToA::toString() const{
   typedef std::pair<const std::string, std::string> strpair;

   boost::format fmt(" %s \t %.5f \t %s \t %.5f \t %s");

   fmt % this->name % this->freq % this->toa.printdays(15) % (this->error*1e6) % this->telescope->getCode();
   std::string out = fmt.str();
   fmt = boost::format(" -%|-.30| %|-.30|");
   BOOST_FOREACH(strpair p, this->flags){
	  fmt % p.first % p.second;
	  out.append(fmt.str());
   }
   return out;
}
