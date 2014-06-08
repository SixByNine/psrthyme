//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <cpgplot.h>
#include <TKlog.h>
#include <limits>
#include <algorithm>
#include <boost/foreach.hpp>

// include header generated by makeheaders
#include "PgPlotPane.hpp"

#if INTERFACE
#include <boost/shared_ptr.hpp>
#include <string>
#include <utility>
#include <vector>
class PgPlotPane {
   private:
	  std::pair<double,double> xlim;
	  std::pair<double,double> ylim;
   public:
	  double x0;
	  double y0;
	  typedef boost::shared_ptr<PgPlotPane> Ptr;
	  PgPlotPane(double,double,double,double); // constructor
	  double height;
	  double width;
	  double margin;
	  double axsize;
	  bool draw_xlab;
	  bool draw_ylab;
	  bool draw_xtic;
	  bool draw_ytic;
	  bool visible;
	  bool auto_xlim;
	  bool auto_ylim;
	  std::string xlab;
	  std::string ylab;
	  std::string title;
	  std::vector<PgPlotData::Ptr> datasets;
	  std::vector<PgPlotText::Ptr> annotations;
	  void draw();
	  void set_xlim(double x0, double x1){
		 this->auto_xlim=false;
		 this->xlim.first=x0;
		 this->xlim.second=x1;
	  }
	  void set_ylim(double y0, double y1){
		 this->auto_ylim=false;
		 this->ylim.first=y0;
		 this->ylim.second=y1;
	  }
};

#endif

PgPlotPane::PgPlotPane(double x0, double y0, double w, double h) {
   this->width=w;
   this->height=h;
   this->visible=true;
   this->x0=x0;
   this->y0=y0;
   this->margin=1.0;
   this->axsize=1.0;
   if(w < 0.3 || h < 0.3){
	  this->axsize=0.5;
   }
   this->draw_xlab=true;
   this->draw_ylab=true;
   this->draw_xtic=true;
   this->draw_ytic=true;
   this->auto_xlim=true;
   this->auto_ylim=true;
}

void PgPlotPane::draw(){
   if (this->auto_xlim){
	  double xmin =  std::numeric_limits<float>::max();
	  double xmax = -std::numeric_limits<float>::max();
	  BOOST_FOREACH (PgPlotData::Ptr dat, this->datasets){
		 xmax = std::max(xmax,dat->xmax());
		 xmin = std::min(xmin,dat->xmin());
	  }
	  double xrange = xmax-xmin;
	  this->xlim.first=xmin-0.01*xrange;
	  this->xlim.second=xmax+0.01*xrange;
   }
   if (this->auto_ylim){
	  double ymin =  std::numeric_limits<float>::max();
	  double ymax = -std::numeric_limits<float>::max();
	  BOOST_FOREACH (PgPlotData::Ptr dat, this->datasets){
		 ymax = std::max(ymax,dat->ymax());
		 ymin = std::min(ymin,dat->ymin());
	  }
	  double yrange = ymax-ymin;
	  this->ylim.first=ymin-0.01*yrange;
	  this->ylim.second=ymax+0.01*yrange;
   }
   double xmargin=margin*0.8*0.095*axsize;
   double ymargin=margin*1.2*0.095*axsize;
   if (!draw_xlab)ymargin=0;
   if (!draw_ylab)xmargin=0;
   logdbg("X '%s' %d",this->xlab.c_str(),this->xlab.length());
   if (this->xlab.length()==0) ymargin/=2.0;
   if (this->ylab.length()==0) xmargin/=2.0;
   if (this->visible) {
	  logdbg("Plotting x:%lg %lg y:%lg %lg",xlim.first,xlim.second,ylim.first,ylim.second);
	  logdbg("VP x:%lg %lg y:%lg %lg",x0+xmargin,x0+width,y0+ymargin,y0+height);
	  cpgsch(this->axsize);
	  cpgsvp(x0+xmargin,x0+width,y0+ymargin,y0+height);
	  cpgswin(xlim.first,xlim.second,ylim.first,ylim.second);
	  cpgbox("BC",0,0,"BC",0,0);
	  if(draw_xtic)cpgbox("BTS",0,0,"",0,0);
	  if(draw_ytic)cpgbox("",0,0,"BTS",0,0);
	  if(draw_xlab){
		 cpgbox("N",0,0,"",0,0);
		 cpglab(this->xlab.c_str(),"","");
	  }
	  if(draw_ylab){
		 cpgbox("",0,0,"N",0,0);
		 cpglab("",this->ylab.c_str(),"");
	  }
	  BOOST_FOREACH(PgPlotData::Ptr dat, this->datasets){
		 dat->draw();
	  }

	  BOOST_FOREACH(PgPlotText::Ptr txt, this->annotations){
		 txt->draw();
	  }
   }
}
