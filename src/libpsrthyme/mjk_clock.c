#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mjk_clock.h"

#if INTERFACE
#ifdef __MACH__
#define clockid_t int
#endif

typedef struct mjk_clock{
   time_t s;
   long n;
   struct timespec last_time;
   char running;
   clockid_t spec;
} mjk_clock_t;
#endif



#ifdef __MACH__
#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(int clk_id, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

mjk_clock_t *init_clock(){
   mjk_clock_t *ret =  (mjk_clock_t*)calloc(1,sizeof(mjk_clock_t));

#ifndef __MACH__
   if(sysconf(_SC_MONOTONIC_CLOCK)){
	  ret->spec =CLOCK_MONOTONIC;
   }else{
	  ret->spec =CLOCK_REALTIME;
   }
#endif
   return ret;
}

void start_clock(mjk_clock_t* clock){
   if(!clock->running){
	  clock_gettime(clock->spec,&clock->last_time);
	  clock->running=1;
   }
}
void stop_clock(mjk_clock_t* clock){
   if(clock->running){
	  long n = clock->last_time.tv_nsec;
	  time_t s = clock->last_time.tv_sec;
	  clock_gettime(clock->spec,&clock->last_time);
	  clock->running=0;
	  clock->n += (clock->last_time.tv_nsec-n);
	  clock->s += (clock->last_time.tv_sec-s);
   }
}
void reset_clock(mjk_clock_t* clock){
   clock->running=0;
   clock->n=0;
   clock->s=0;
   clock_gettime(clock->spec,&clock->last_time);
}
double read_clock(mjk_clock_t* clock){
   return clock->s + clock->n/1e9L;
}




