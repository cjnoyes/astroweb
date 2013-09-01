#ifndef _CHARTWRITER_H_
#define _CHARTWRITER_H_
#pragma once

class ChartWriter {
protected:
	FILE *stream;
	int writeLine(char *key, char *buff);
	int writeSection(char *key);
	int writeHouses(char *key, short *houses);
	int writeAspect(char *key, int planet, ASPEC &aspect );
	int writeAspects(char *sec, AS_INF *data, int count);
	int writeAspects(char *key, int index, int planet, AS_INF &data);
	int writePlanet(char *key, int index, AS_INF &data);
	int writePlanets(char *, AS_INF *data, int count, int start=0);
	int writeTransitDates();
	int writeTransitChart(int index);
public:
	ChartWriter( FILE *_stream ) { stream = _stream; }
	~ChartWriter() {}
	int writeNatal(int maxp);
	int writeCompat(int maxp);
	int writeProgr(int maxp);
	int writeTrans(int maxp);
};

#endif



