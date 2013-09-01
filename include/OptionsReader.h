#ifndef _OPTIONSREADER_H_
#define _OPTIONSREADER_H_
#pragma once

class OptionsReader {
protected:
	FILE *stream;
public:
	OptionsReader(FILE *in) { stream = in; }
	virtual ~OptionsReader() {}
	int readOptions(Options &options);
};


#endif