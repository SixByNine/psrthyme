

#include "c++pgplot/pgplot.hpp"

int main(int argc, char** argv){
   PgPlot plt;
   plt.createGrid(2,2);
   plt.setRowHeight(0,0.4);
   plt.setRowHeight(1,0.6);
   //
   PgPlotData::Ptr data = PgPlotData::Ptr(new PgPlotData());
   plt.getPane(1,1)->datasets.push_back(data);
   plt.getPane(1,1)->ylab="testy";
   plt.getPane(0,1)->datasets.push_back(data);
   plt.getPane(0,1)->ylab="meaty";
   plt.getPane(0,1)->xlab="beaty";
   plt.getPane(0,1)->axsize=0.75;
   data->x.push_back(0);
   data->x.push_back(0.5);
   data->x.push_back(0.6);
   data->x.push_back(0.7);
   data->x.push_back(1.0);
   data->y.push_back(1);
   data->y.push_back(1.4);
   data->y.push_back(1.6);
   data->y.push_back(2);
   data->y.push_back(1.5);
   data->lineColorIndex=PgPlot::PURPLE;
   data->pointColorIndex=PgPlot::ORANGE;
   data->pointType=PgPlot::ODOT;
   plt.getPane(1,0)->datasets.push_back(data);
   plt.getPane(1,0)->draw_ylab=false;

   data = PgPlotData::Ptr(new PgPlotData());
data->x.push_back(0);
   data->x.push_back(0.5);
   data->x.push_back(0.6);
   data->x.push_back(0.7);
   data->x.push_back(1.0);
   data->y.push_back(1);
   data->y.push_back(1.4);
   data->y.push_back(1.6);
   data->y.push_back(2);
   data->y.push_back(1.5);
   data->lineColorIndex=PgPlot::GREEN;
   
   plt.getPane(0,0)->datasets.push_back(data);
   plt.getPane(0,0)->draw_ylab=false;
   plt.getPane(0,0)->draw_xlab=false;
   plt.show();

   return 0;
}

