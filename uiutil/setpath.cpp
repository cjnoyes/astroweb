#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "paths.hpp"

static char buffer[120] = {""};
static char home[120] = {""};

void setHomeDir(char *pHome) {
   strcpy(home,pHome);
}


char *getBaseDir() {
  static char buf[75];
  char buf2[50];
  char *pHome = getenv("ASTROWEB_HOME");
  if (pHome != NULL) {
     strcpy(buf,pHome);
	 return buf;
  }
  else if (strlen(home) > 0) {
	  return home;
  }
  else {
     strcpy(buf,getcwd(buf2,49));
	 return buf;
  }
}

void removeDots( char *buf )
{
}


char *set_path( char *name, int type )
{
 strcpy(buffer, getBaseDir());
 if (buffer[strlen(buffer)-1] != DIRSEPCH) {
    strcat(buffer,DIRSEP);
 }
 switch (type) {
   case DATA_F:
	   strcat(buffer,"data" DIRSEP);
	   break;
   case USER_F:
	   strcat(buffer,"session" DIRSEP);
	   break;
  }
  strcat(buffer, name);
  return buffer;
}
