//  Copyright (C) 2014 Michael J. Keith, University of Manchester
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <algorithm>
#include <inttypes.h>
#include <c++pgplot/pgplot.hpp>
#include <TKlog.h>
#include "debug.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>

// include header generated by makeheaders
#include "PsrthymePlotter.hpp"

#if INTERFACE
#include <string>
#include <vector>
class PsrthymePlotter {
   private: 
	  std::vector<std::string> devices;
   public:
	  PsrthymePlotter(std::string dev) {
		 this->devices.push_back(dev);
	  }
	  void addDevice(std::string dev){
		 this->devices.push_back(dev);
	  }
	  void clearDevices(){
		 this->devices.clear();
	  }
	  void plot(PsrthymeResult::Ptr result);
}

#endif


void PsrthymePlotter::plot(PsrthymeResult::Ptr result){

   double chisq_min = result->chisq_space->get(result->chisq_space->min())/double(result->nfree);
   PgPlot pgplot;
   pgplot.createGrid(3,1);
   pgplot.setRowHeight(0,0.2);
   pgplot.setRowHeight(1,0.15);
   pgplot.setRowHeight(2,0.6);

   // Lower plot, it's the pulse profile
   PgPlotPane::Ptr mainPlot = pgplot.getPane(2,0);
   mainPlot->set_xlim(-0.5,0.5);

   PgPlotData::Ptr profile = PgPlotData::blank();
   mainPlot->datasets.push_back(profile);
   profile->x = result->obsn->getPhase();
   profile->y = result->obsn->getNormalisedProfile();

   PgPlotData::Ptr tmpl = PgPlotData::blank();
   tmpl->x = result->obsn->getPhase();
   tmpl->y = result->best_profile;
   mainPlot->datasets.push_back(tmpl);

   PgPlotData::Ptr resid = PgPlotData::blank();
   resid->x = result->obsn->getPhase();
   resid->y = result->residual;
   mainPlot->datasets.push_back(resid);
   mainPlot->xlab="Phase";
   mainPlot->ylab="Amplitude";

   // med plot, the chisq.
   PgPlotPane::Ptr midPlot = pgplot.getPane(1,0);
   midPlot->set_xlim(-0.5,0.5);
   midPlot->draw_xlab=false;
   midPlot->ylab="Chisq";

   PgPlotData::Ptr chisq = PgPlotData::blank();
   result->chisq_space->toArray(chisq->x, chisq->y);
   result->reduceChisq(chisq->y);

   midPlot->datasets.push_back(chisq);

   PgPlotData::Ptr cov = PgPlotData::blank();
   cov->x = result->obsn->getPhase();
   cov->y = result->data_cov;
   double cov_min = *std::min_element(cov->y.begin(),cov->y.end());
   double cov_max = *std::max_element(cov->y.begin(),cov->y.end());
   for(uint64_t i=0; i < cov->y.size(); i++){
	  cov->y[i] = (cov->y[i] - cov_min)/(cov_max-cov_min);
	  cov->y[i] *= chisq_min*5;
   }


   midPlot->datasets.push_back(cov);



   // top plot, the zoomed chisq.
   PgPlotPane::Ptr topPlot = pgplot.getPane(0,0);
   topPlot->ylab="Chisq";

   PgPlotData::Ptr chizoom = PgPlotData::blank();

   result->chisqZoom(result->phase, result->error*5.0, chizoom->x,chizoom->y);

   result->reduceChisq(chizoom->y);



   PgPlotData::Ptr chifit = PgPlotData::blank();

   chifit->x = result->chisq_fit_x;
   chifit->y = result->chisq_fit_y;
   result->reduceChisq(chifit->y);

   topPlot->datasets.push_back(chizoom);
   topPlot->datasets.push_back(chifit);




   //colours and stuff
   profile->setPlotType(PgPlotData::HIST);
   profile->lineColorIndex=PgPlot::SKY;

   tmpl->setPlotType(PgPlotData::HIST);
   tmpl->lineColorIndex=PgPlot::SEA;

   resid->setPlotType(PgPlotData::HIST);
   resid->lineColorIndex=PgPlot::RED;

   chisq->setPlotType(PgPlotData::LINE);
   chisq->lineColorIndex=PgPlot::RED;

   cov->setPlotType(PgPlotData::LINE);
   cov->lineColorIndex=PgPlot::SKY;

   logdbg("%lg chisq_min",chisq_min);
   midPlot->set_ylim(0,chisq_min*5);

   topPlot->title="XX";
   chizoom->setPlotType(PgPlotData::LINE);
   chizoom->addPlotType(PgPlotData::POINT);
   chizoom->pointType=PgPlot::CIRCLE1;
   chizoom->lineColorIndex=PgPlot::RED;
   chizoom->pointColorIndex=PgPlot::ORANGE;

   chifit->setPlotType(PgPlotData::LINE);
   chifit->lineColorIndex=PgPlot::SEA;
   chifit->lineType=4;
   topPlot->set_ylim(0,chisq_min*3);

   logdbg("nplot %ld",result->chisq_space->size());
   double best_chisq = result->chisq/double(result->nfree);
   boost::format format;
   format = boost::format("\\(2199) Chisq=%.3f dP=%.3g \\(2233) %.1g \\(2199)");
   format % best_chisq % (result->phase) % result->error;

   PgPlotText::Ptr txt;
   if (result->phase > 0){
	  txt = PgPlotText::Ptr(new PgPlotText(format.str(),-0.5,best_chisq*2));
	  txt->just = 0.0;
		 txt->size=0.8;
   } else {
	  txt = PgPlotText::Ptr(new PgPlotText(format.str(),0.5,best_chisq*2));
	  txt->just = 1.0;
		 txt->size=0.8;
   }
   midPlot->annotations.push_back(txt);
   double mx = *std::max_element(result->obsn->getNormalisedProfile().begin(), result->obsn->getNormalisedProfile().end());
   double mn = *std::min_element(result->obsn->getNormalisedProfile().begin(), result->obsn->getNormalisedProfile().end());
   double dy = (mx-mn)/20;
   for (uint64_t i =0; i < result->tmpl->size(); ++i){
	  format = boost::format("\\(2199) %s %.2f \\(2233) %.2f \\(2199)");
	  format % result->tmpl->at(i)->getName() % result->amp_values[i] % result->amp_errors[i];
	  if (result->phase > 0){
		 txt = PgPlotText::Ptr(new PgPlotText(format.str(),-0.5, mx-(i+1)*dy));
		 txt->just = 0.0;
		 txt->size=0.8;
	  } else {
		 txt = PgPlotText::Ptr(new PgPlotText(format.str(), 0.5, mx-(i+1)*dy));
		 txt->just = 1.0;
		 txt->size=0.8;
	  }
	  mainPlot->annotations.push_back(txt);
   }


   BOOST_FOREACH (std::string dev, devices){
	  pgplot.show(dev);
   }
}

