//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <inttypes.h>
#include <cpgplot.h>
#include <TKlog.h>

// include header generated by makeheaders
#include "PgPlot.hpp"

#if INTERFACE
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#define grid_t boost::multi_array<PgPlotPane::Ptr, 2>
class PgPlot {
   private:
	  double defaultColWidth;
	  double defaultRowHeight;
	  uint64_t nrow;
	  uint64_t ncol;
	  grid_t::extent_gen extents;
	  std::string dev;
	  grid_t grid;
	  double maxsz;
	  void reset();
	  void fixPanes();
   public:
	  enum PgColor {PAPER=0,INK=1,RED=2,GREEN=3,BLUE=4,
		 CYAN=5,MAGENTA=6,YELLOW=7,
	     ORANGE=8, GRASS=9, SEA=10, SKY=11, PURPLE=12, 
		 PINK=13, DKGREY=14, LTGREY=15 };

	  enum PgSymb {BOX=0, DOT=1, PLUS=2, ASTERIX=3,
		 CIRCLE=4,CROSS=5, BOX2=6, TRIANGLE=7, OPLUS=8, ODOT=9,
		 FLAREBOX=10, DIAMOND=11, STAR5=12, FILLTRIANGLE=13,
		 OUTLINEPLUS=14, STAR6=15, FILLBOX=16, FILLCIRCLE=17,
		 FILLSTAR5=18, BIGBOX=19, CIRCLE1=20, CIRCLE2=21, 
		 CIRCLE3=22, CIRCLE4=23, CIRCLE5=24, CIRCLE6=25, CIRCLE7=26, 
		 CIRCLE8=27,LEFT=28, RIGHT=29, UP=30, DOWN=31};

	  typedef boost::shared_ptr<PgPlot> Ptr;
	  PgPlot(); // constructor
	  void createGrid(uint64_t nrow, uint64_t ncol);
	  void show();
	  void show(std::string device);
	  PgPlotPane::Ptr getPane(uint64_t r, uint64_t c){
		 r = nrow-r-1;
		 //c = ncol-c-1;
		 this->grid[r][c]->visible = true;
		 return this->grid[r][c];
	  }
	  void setColWidth(uint64_t col, double width){
		 this->defaultColWidth=this->maxsz*width;
		 for(uint64_t irow = 0; irow< this->nrow; irow++){
			this->grid[irow][col]->width=width;
		 }
		 this->fixPanes();
	  }
	  void setRowHeight(uint64_t row, double height){
		 row = nrow-row-1;
		 this->defaultRowHeight=this->maxsz*height;
		 for(uint64_t icol = 0; icol<this->ncol; icol++){
			this->grid[row][icol]->height=height;
		 }
		 this->fixPanes();
	  }


	  void setDevice(std::string dev){
		 this->dev = dev;
	  }

};

#endif

PgPlot::PgPlot() {
   this->maxsz=0.999;
   this->defaultColWidth=this->maxsz;
   this->defaultRowHeight=this->maxsz;
   this->grid.resize(this->extents[1][1]);
   PgPlotPane::Ptr pane = PgPlotPane::Ptr(
		 new PgPlotPane(0,0,this->defaultColWidth,this->defaultRowHeight)
		 );
   this->grid[0][0]=pane;

   this->dev="/xs";
   this->nrow=1;
   this->ncol=1;
}

void PgPlot::createGrid(uint64_t nrow, uint64_t ncol){
   this->defaultColWidth = this->maxsz/double(ncol);
   this->defaultRowHeight = this->maxsz/double(nrow);
   this->grid.resize(this->extents[nrow][ncol]);
   this->nrow=nrow;
   this->ncol=ncol;
   for(uint64_t irow = 0; irow<nrow; irow++){
	  double y0=irow*this->defaultRowHeight;
	  for(uint64_t icol = 0; icol<ncol; icol++){
		 double x0=icol*this->defaultColWidth;
		 PgPlotPane::Ptr pane = PgPlotPane::Ptr(
			   new PgPlotPane(x0,y0,this->defaultColWidth,this->defaultRowHeight)
			   );
		 pane->visible = false;
		 this->grid[irow][icol]=pane;
	  }
   }
}
void PgPlot::reset(){
   cpgsci(1);
   cpgslw(1);
   cpgsch(1);
   cpgsls(1);
}

void PgPlot::fixPanes(){
   double y0=0;
   for(uint64_t irow = 0; irow<this->nrow; irow++){
	  for(uint64_t icol = 0; icol<this->ncol; icol++){
		 this->grid[irow][icol]->y0=y0;
	  }
	  y0+=this->grid[irow][0]->height;
   }
   double x0=0;
   for(uint64_t icol = 0; icol<this->ncol; icol++){
	  double w = this->grid[0][icol]->width;
	  for(uint64_t irow = 0; irow<this->nrow; irow++){
		 this->grid[irow][icol]->x0=x0;
	  }
	  x0+=w;
   }

}

void PgPlot::show(){
   show(this->dev);
}
void PgPlot::show(std::string device){
   cpgopen(device.c_str());
   cpgeras();
   logmsg("show() nrow=%ld ncol=%ld",this->nrow,this->ncol);
   for(uint64_t irow = 0; irow<this->nrow; irow++){
	  for(uint64_t icol = 0; icol<this->ncol; icol++){
		 this->reset();
		 this->grid[irow][icol]->draw();
	  }
   }
   cpgclos();
}
