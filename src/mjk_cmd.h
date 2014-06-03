/* This file was automatically generated.  Do not edit! */
typedef struct mjk_clock mjk_clock;
#if defined(__MACH__)
#define clockid_t int
#endif
struct mjk_clock {
   time_t s;
   long n;
   struct timespec last_time;
   char running;
   clockid_t spec;
};
typedef struct mjk_clock mjk_clock_t;
double read_clock(mjk_clock_t *clock);
void reset_clock(mjk_clock_t *clock);
void stop_clock(mjk_clock_t *clock);
void start_clock(mjk_clock_t *clock);
mjk_clock_t *init_clock();
int getI(const char *lo,const char *so,int argc,char **argv,int val);
double getF(const char *lo,const char *so,int argc,char **argv,double val);
char getB(const char *lo,const char *so,int argc,char **argv,char val);
const char *getS(const char *lo,const char *so,int argc,char **argv,const char *val);
void getArgs(int *argc,char **argv);
#if defined(__MACH__)
int clock_gettime(int clk_id,struct timespec *t);
#endif
#define STREQ(a,b) (strcmp(a,b)==0)
#define INTERFACE 0
