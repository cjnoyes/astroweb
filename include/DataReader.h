#ifndef _DATAREADER_H_
#define _DATAREADER_H_
#pragma once

#define END_RD      0
#define SECTION_RD  1
#define VARIABLE_RD 2
#define ERROR_RD    3
#define COMMENT_RD  4

class DataReader {
protected:
	FILE *stream;
	char section[35];
	char key[25];
	char value[100];
	int parseInt(short &num) { num = atoi(value); return 1; };
	int readBirth(AS_DATA &rec);
	int parseDate(DATES &date);
	int parseTime(TIM &time);
	int parseLongLat(DEG_MIN &degs);
	int readRect(RECT_DATA &rect);
	int readTrans(TRANS_DATA &trans);
	int readProg(PROGR_DATA &prog);
	int readLine();
public:
	DataReader(FILE *in) { stream = in; }
	virtual ~DataReader() {}
	int readData();
};




#endif
