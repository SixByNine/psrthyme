#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <TKlog.h>
#include "mjk_cmd.h"

#define STREQ(a,b) (strcmp(a,b)==0)

void getArgs(int *argc, char** argv){
   int i;
   int n=1;
   for (i=1;i< *argc; i++){
	  if(argv[i][0]<5){
		 i+=argv[i][0]-1;
		 continue;
	  }
	  argv[n]=argv[i];
	  n++;
   }
   *argc=n;
}
const char* getS(const char *lo, const char* so, int argc, char** argv, const char* val){
   int i;
   char* ret;
   for (i=1; i < argc; i++){
	  if (STREQ(argv[i],lo) || STREQ(argv[i],so)){
		 ret = *(argv+i+1);
		 argv[i][0]=2;
		 return ret;
	  }
   }
   return val;
}

char getB(const char *lo, const char* so, int argc, char** argv, char val){
   int i;
   char ret = val;
   for (i=1; i < argc; i++){
	  if (STREQ(argv[i],lo) || STREQ(argv[i],so)){
		 ret = !val;
		 argv[i][0]=1;
		 break;
	  }
   }
   return ret;
}


double getF(const char *lo, const char* so, int argc, char** argv, double val){
   const char* str = getS(lo,so,argc,argv,NULL);
   if (str==NULL) return val;
   else return atof(str);
}

int getI(const char *lo, const char* so, int argc, char** argv, int val){
   const char* str = getS(lo,so,argc,argv,NULL);
   if (str==NULL) return val;
   else return atoi(str);
}





