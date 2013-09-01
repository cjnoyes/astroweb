#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "DataReader.h"
#include <assert.h>

#define assertMember(var,var2,member) assert(var.member==var2.member)

#define assertDate(var,var2) assertMember(var,var2,day);\
assertMember(var,var2,month);\
assertMember(var,var2,year)

#define assertTim(var,var2) assertMember(var,var2,hours);\
assertMember(var,var2,minutes);\
assertMember(var,var2,seconds)

#define assertDegMin(var,var2) assertMember(var,var,degrees);\
assertMember(var,var,minutes);\
assertMember(var,var,dir)

#define assertMemberVal(var, member, val) assert(var.member==val)

void validateRect(RECT_DATA &one, RECT_DATA &two, char *label) {
	printf("validate %s\n", label);
	assertMember(one,two,birth_time_known);
	assertMember(one,two,rect_system);
}

void validateAsData(AS_DATA &one, AS_Data &two, char *label) {
	printf("validate %s\n", label);
	assertDegMin(one.longitude,two.longitude);
	assertDegMin(one.latitude,two.latitude);
	assertDate(one.birth_date,two.birth_date);
	assertTim(one.birth_time,two.birth_time);
	assertMember(one,two,hour_adjust);
	assertMember(one,two,minutes_adjust);
}

int main( int argc, char *argv[]) {
	
   FILE *stream = fopen("test.in","r");
   if (stream == NULL) {
      printf("stream is NULL\n");
   }
   
   DataReader *reader = new DataReader(stream);
   int rsp = reader->readData();
   printf("ended with code %d\n", rsp);
   assert(rsp==END_RD);
   
   
   
}