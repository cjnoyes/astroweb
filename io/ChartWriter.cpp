#include <stdio.h>
#include <string.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "ChartWriter.h"

int ChartWriter::writeLine(char *key, char *buff) {
	return fprintf(stream, "%s=%s\n", key, buff );
}

int ChartWriter::writeSection(char *key) {
	return fprintf(stream,"[%s]\n",key);
}

int ChartWriter::writeHouses(char *sec, short *houses) {
	writeSection(sec);
	for (int i =0; i < 12; i++) {
	    fprintf(stream,"house%d=%d\n", i+1, houses[i]);
	}
	return 1;
}

int ChartWriter::writeAspect(char *key, int planet, ASPEC &aspect ) {
	return fprintf(stream,"%s=\"%d,%d,%d,%d\"\n", key, planet, aspect.planet, aspect.aspect, aspect.orb );
}

int ChartWriter::writePlanet(char *key, int index, AS_INF &data) {
return fprintf(stream,"%s=\"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\"\n",key, index, data.minutes_total, data.sign, data.cusp, data.degree_minor, data.minutes_minor, data.house, data.house_cusp, data.intercepted,
   data.retrograde, data.misc_code);
}

int ChartWriter::writeAspects(char *key, int index, int planet, AS_INF &data ) {
   char buf[30];
   
   for (int i =0; i < data.no_aspects; i++, index++) {
	   sprintf(buf,"%s%d",key,index+1);
	   writeAspect(buf,planet,data.aspectr[i]);
   }
   return index;
}

int ChartWriter::writeAspects(char *sec, AS_INF *data, int count) {
	int index = 0;
	writeSection(sec);
	for (int i = 0; i < count; i++ ) {
	    index = writeAspects("aspect", index, i+1, data[i]);
	}
	return 1;
}

int ChartWriter::writePlanets(char *sec, AS_INF *data, int count, int start) {
	char buf[30];
	
	writeSection(sec);
	for (int i = start; i < count; i++ ) {
	   sprintf(buf,"planet%d",i+1);
	   writePlanet(buf,i+1,data[i]);
	}
	return 1;
}

int ChartWriter::writeNatal(int maxpt) {
	writePlanets("Planets", birth_inf, maxpt);
	writeHouses("Houses", house_cusps);
	writeAspects("Aspects", birth_inf, maxpt);
	return 1;
}

int ChartWriter::writeCompat(int maxpt) {
	writeNatal(maxpt);
	writePlanets("OtherPlanets", alt_inf, maxpt);
	writeHouses("OtherHouses", alt_cusps);
	writeAspects("OtherAspects", alt_inf, maxpt);
	return 1;
}

int ChartWriter::writeProgr(int maxpt) {
	writeNatal(maxpt);
	writeHouses("OtherHouses", alt_cusps);
	writePlanets("OtherPlanets", prog, maxpt);
	writeAspects("OtherAspects", prog, maxpt);
	return 1;
}

int ChartWriter::writeTrans(int maxpt) {
	writeNatal(maxpt);
	writeTransitDates();
	for (int i = 0; i < transit_data.num_transits; i++) {
		writeTransitChart(i);
	}
	return 1;
} 

int ChartWriter::writeTransitDates() {
	writeSection("TransitDates");
	for (int i = 0; i < transit_data.num_transits; i++) {
		DATES dp = date_ptr[i];
	    fprintf(stream,"date%d=\"%d/%d/%d\"\n",i+1,(int)dp.month,(int)dp.day,(int)dp.year); 
	}
	return 1;
}


int ChartWriter::writeTransitChart(int index) {
	char buf[30];
	int maxpt = transit_data.end_planet - transit_data.start_planet;
	maxpt++;
	AS_INF *ptr = trans_ptr[index];
	sprintf(buf,"TransitPlanets-%d",index+1);
	writePlanets(buf, ptr, maxpt);
	sprintf(buf,"TransitAspects-%d",index+1);
	writeAspects(buf, ptr, maxpt);
	return 1;
}



































