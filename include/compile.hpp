#ifndef _COMPILE_HPP_
#define _COMPILE_HPP_
#pragma once


enum ChartType { DefaultChart=1,BirthSgnOnly,BirthSgnHse,SgnHseAsp,Complete,
	CompleteBMid,CompleteFMid,Compatibility,Transits,Progress,Numeric, Helio, 
	CReturn, Reloc, Compos, ComposTrans,Arabic,Fixed,Problem,
	SolarArc,Harmonic,Messier,CReturnReloc,CReturnTrans };

class ChartCompiler {
protected:
      FILE *stream;
      int mode, ftype;
      enum ChartType chartType;
      Options op;
      DATA_PACKET dp;
      char comment_buf[80];
	  int maxpt;
public:
      ChartCompiler(enum ChartType tp)  { chartType = tp; stream = NULL;}
      virtual ~ChartCompiler() {memset(birth_inf,0,sizeof(AS_INF)*30); memset(alt_inf,0,sizeof(AS_INF)*30); memset(birth_ext,0,sizeof(AS_INF_EXT)*30); memset(house_cusps,0,sizeof(short)*12); memset(alt_cusps,0,sizeof(short)*12);}
      int testErrno();
	  int setup();
      virtual int compile() {return 1;}
      virtual int writeData() {return 1;}
      virtual int runChart();
      virtual int cleanUp(int ok);
      };

class NatalChartCompiler : public ChartCompiler {
public:
      NatalChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~NatalChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };

class CompChartCompiler : public ChartCompiler {
public:
	CompChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~CompChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };

class ProgChartCompiler : public ChartCompiler {
public:
      ProgChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~ProgChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };

class SolarChartCompiler : public ChartCompiler {
public:
      SolarChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~SolarChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };
class HarmonicChartCompiler : public ChartCompiler {
public:
      HarmonicChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~HarmonicChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };
class TransChartCompiler : public ChartCompiler {
public:
      TransChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~TransChartCompiler() {}
      virtual int compile();
      virtual int writeData();
      };

class ReturnChartCompiler : public ChartCompiler {
public:
      ReturnChartCompiler(enum ChartType tp) : ChartCompiler(tp) {}
      virtual ~ReturnChartCompiler() {}
      virtual int doSetup();
      virtual int writeData();
      };
//extern int x_kb_check();
	  
extern int doCompileChart(char *type);

#endif /* _COMPILE_HPP_ */
