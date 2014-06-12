#include <PsrthymeToA.hpp>

#include <TKlog.h>
#include <boost/format.hpp>
#if INTERFACE
#include <mjd.h>
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
	  PsrthymeToA(const MJD &toa, double error_s,double freq, PsrthymeTelescope::Ptr telescope) {
		 this->toa = toa;
		 this->error = error_s;
		 this->freq = freq;
		 this->telescope = telescope;
		 this->name="t";
	  };
	  void setFlag(std::string flag, std::string value){
		 this->flags[flag] = value;
	  }
	  void clearFlag(std::string flag){
		 this->flags.erase(flag);
	  }
	  std::string toString() const;
};
#endif

std::string PsrthymeToA::toString() const{
   boost::format fmt(" %s \t %.5f \t %s \t %.5f \t %s");

   fmt % this->name % this->freq % this->toa.printdays(15) % (this->error*1e6) % this->telescope->getCode();
   return fmt.str();
}
