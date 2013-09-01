#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "utlduad.h"
#include "setup.hpp"
#include <memory.h>
#include "compile.hpp"
#include "paths.hpp"
#include "errors.fns"
#include "files.h"

int main(int argc, char *argv[]) {
  char type[25], job[25];
  
  if (argc < 4) {
	  alert_box("","astroweb --type <type> --job <key> [--home <dir>]\n");
  }
  if ((argc-1) %2 == 1) {
     alert_box("","Parameters must be in pairs\n");
	 return 2;
  }
  for (int i =1; i < argc-1; i+=2) {
      if (strcmp("--type",argv[i])==0) {
		  strcpy(type,argv[i+1]);
	  }
	  else if (strcmp("--job",argv[i])==0) {
		  strcpy(job,argv[i+1]);
		  setSession(job);
	  }
	  else if (strcmp("--home",argv[i])==0) {
		  setHomeDir(argv[i+1]);
	  }
  }
  atexit(closeFiles);
  doCompileChart(type);
  return 0;
}

