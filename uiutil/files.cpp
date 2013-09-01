#include <stdafx.h>
#include <stdio.h>
#include <files.h>
#include "errors.fns"
#include <paths.hpp>
#include <stdlib.h>
#include <string.h>

static char *sessionkey = "default";
static FILE *config;
static FILE *input;
static FILE *output;
static char buffer[150];


void closeFiles() {
   if (config != NULL) {
      fclose(config);
	  config = NULL;
   }
   if (input != NULL) {
      fclose(input);
	  input = NULL;
   }
   if (output != NULL) {
      fclose(output);
	  output = NULL;
   }
}

static char * formatFile(char *ext) {
   char temp[30];
   sprintf(temp,"%s.%s", sessionkey, ext);
   strcpy(buffer,set_path(temp,USER_F));
   return buffer;
}

void setSession(char *session) {
	sessionkey = session;
}

char *getSession() {
	return sessionkey;
}

FILE* getConfigFile() {
	if (config != NULL) {
	   return config;
	}
	else {
	   config = fopen(formatFile("conf"),"r");
	   if (config == NULL) {
	      do_error(buffer);
		  exit(-98);
	   }
	   return config;
	}
}

FILE* getInputFile() {
	if (input != NULL) {
	   return input;
	}
	else {
	   input = fopen(formatFile("in"),"r");
	   if (input == NULL) {
	      do_error(buffer);
		  exit(-98);
	   }
	   return input;
	}
}

FILE* getOutputFile() {
	if (output != NULL) {
	   return output;
	}
	else {
	   output = fopen(formatFile("dat"),"w");
	   if (output == NULL) {
	      do_error(buffer);
		  exit(-98);
	   }
	   return output;
	}
}

FILE* getErrorFile() {
	return stderr;
}
