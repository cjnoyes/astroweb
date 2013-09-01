#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "DataReader.h"
#include <assert.h>

int DataReader::readBirth(AS_DATA &rec) {
	int type=0;
	do {
		type = readLine();
		if (type == VARIABLE_RD) {
			if (strcmp(key,"date")==0) {
				parseDate(rec.birth_date);
			}
			else if (strcmp(key,"time")==0) {
				parseTime(rec.birth_time);
			}
			else if (strcmp(key,"long")==0) {
				parseLongLat(rec.longitude);
			}
			else if (strcmp(key,"lat")==0) {
				parseLongLat(rec.latitude);
			}
			else if (strcmp(key,"tzhours")==0) {
				parseInt(rec.hour_adjust);
			}
			else if (strcmp(key,"tzminutes")==0) {
				parseInt(rec.minutes_adjust);
			}
		}
	} while (type==VARIABLE_RD);
	return type;
}

int DataReader::parseDate(DATES &date) {
	return sscanf(value,"%hd/%hd/%hd", &date.month,&date.day, &date.year);
}

int DataReader::parseTime(TIM &time) {
	return sscanf(value,"%hd:%hd:%hd", &time.hours, &time.minutes, &time.seconds);
}

int DataReader::parseLongLat(DEG_MIN &degs) {
	return sscanf(value,"%hd %hd %c", &degs.degrees, &degs.minutes, &degs.dir );
}

int DataReader::readRect(RECT_DATA &rect) {
	int type=0;
	do {
		type = readLine();
		if (type == VARIABLE_RD) {
			if (strcmp(key,"known")==0) {
			   parseInt(rect.birth_time_known);
			}
			else if (strcmp(key,"system")==0) {
			   parseInt(rect.rect_system);
			}
		}
	} while (type==VARIABLE_RD);
  return type;
}

int DataReader::readTrans(TRANS_DATA &trans) {
	int type=0;
	do {
		type = readLine();
		if (type == VARIABLE_RD) {
			if (strcmp(key,"count")==0) {
				parseInt(trans.num_transits);
			}
			else if (strcmp(key,"start")==0) {
				parseInt(trans.start_planet);
			}
			else if (strcmp(key,"end")==0) {
				parseInt(trans.end_planet);
			}
			else if (strcmp(key,"offset")==0) {
				parseDate(trans.interval_date);
			}
		}
	} while (type==VARIABLE_RD);
    return type;
}

int DataReader::readProg(PROGR_DATA &prog) {
	int type=0;
	do {
		type = readLine();
		if (type == VARIABLE_RD) {
			if (strcmp(key,"system")==0) {
				parseInt(prog.progress_system);
			}
			else if (strcmp(key,"date")==0) {
			    parseDate(prog.offset);
			}
		}
	} while (type==VARIABLE_RD);
	return type;
}

int DataReader::readData() {
	int type = 0;
	assert(stream != NULL);
	type=readLine();
	do {
		if (type==COMMENT_RD) {
			type=readLine();
		}
		if (type==SECTION_RD) {
			if (strcmp("[Birth]",section)==0) {
				type=readBirth(birth_rec.birth_data);
			}
			else if (strcmp("[Comp]",section)==0) {
				type=readBirth(comp_rec.birth_data);
			}
			else if (strcmp("[Trans]",section)==0) {
				type=readBirth(trans_rec.birth_data);
			}
			else if (strcmp("[BirthRect]",section)==0) {
				type=readRect(birth_rectd);
			}
			else if (strcmp("[CompRect]",section)==0) {
				type=readRect(comp_rectd);
			}
			else if (strcmp("[TransData]",section)==0) {
				type=readTrans(transit_data);
			}
			else if (strcmp("[ProgrData]",section)==0) {
				type=readProg(progress_data);
			}
		}
	} while (type != END_RD);
	return type;
}

int DataReader::readLine() {
	char buffer[150];
	char *ptr = NULL, *ptr2;
	do {
	   ptr =fgets(buffer,149,stream);
	   if (ptr != NULL) {
		   while (*ptr ==' ') {
		      ptr++;
		   }			   
	   }
	   else {
		   return END_RD;
	   }
	   ptr2 = ptr;
	   while (*ptr2 != '\0' && *ptr2 != '\n' && *ptr2 != '\r') {
		   ptr2++;
	   }
	   *ptr2='\0';
	} while (ptr != NULL && strlen(ptr) == 0);
	if (ptr == NULL) {
	   return END_RD;
	}
	else if (ptr[0]=='[') {
	   strcpy(section,buffer);
	   return SECTION_RD;
	}
	else {
		ptr2 = strchr(ptr,'=');
		if (ptr2==NULL) {
		   return ERROR_RD;
		}
		*ptr2 = '\0';
		strcpy(value,ptr2+1);
		strcpy(key,ptr);
		return VARIABLE_RD;
	}	
}
