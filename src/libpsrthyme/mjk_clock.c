#include <stdlib.h>
#include <omp.h>
#include "mjk_clock.h"

#if INTERFACE
typedef struct mjk_clock{
   char running;
   double time;
   double stt;
} mjk_clock_t;
#endif


mjk_clock_t *init_clock(){
   mjk_clock_t *ret =  calloc(1,sizeof(mjk_clock_t));
   return ret;
}

void start_clock(mjk_clock_t* clock){
   if(!clock->running){
	  clock->stt = omp_get_wtime();
	  clock->running=1;
   }
}
void stop_clock(mjk_clock_t* clock){
   if(clock->running){
	  clock->running=0;
	  clock->time += omp_get_wtime() - clock->stt;
   }
}
void reset_clock(mjk_clock_t* clock){
   clock->running=0;
   clock->time=0;
   clock->stt=0;
}
double read_clock(mjk_clock_t* clock){
   return clock->time;
}




