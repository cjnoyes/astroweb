#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "setup.hpp"
#include "OptionsReader.h"

int OptionsReader::readOptions(Options &options) {
	char buf[100];
	int value;
	int done = 0;
	OPTIONS data = options.getOpt();
	
	while (fgets(buf,99,stream) != NULL) {
		char *ptr = strchr(buf,'=');
		if (ptr != NULL) {
		   done = 1;
		   *ptr = '\0';
		   value = atoi(ptr+1);
		   if (strcmp("flags",buf) ==0) {
			   data.flags = value;
		   }
		   else if (strcmp("houses",buf) ==0) {
			   data.house_mode = value;
		   }
		   else if (strcmp("rect",buf) ==0) {
			   data.def_rectmethod = value;
		   }
		   else if (strcmp("code",buf) ==0) {
			   data.chart_code = value;
		   }
		}
		else {
			return 0; 
		}
	}
	options.getOpt()=data;
	return done;
}