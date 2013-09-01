#ifndef _SETUP___HPP_
#define _SETUP___HPP_

typedef struct OPTIONS {
    unsigned short flags;
    short house_mode;
    short charts;
    int chart_code;
    short datafile;
	short def_rectmethod;
    } OPTIONS;

class Options {
private:
    OPTIONS op;
public:
    Options() { op.datafile = -1; }
    ~Options() {}
    OPTIONS &getOpt() { return op; }
    void setOpt( unsigned short fl, short hm, short ch, short chc, short dr ) {
		op.flags = fl; op.house_mode = hm; op.charts = ch; op.chart_code = chc; op.def_rectmethod=dr; }
    unsigned short &getFlags() { return op.flags; }
    short &getHouseMode() { return op.house_mode; }
    short &getCharts() { return op.charts; }
    int &getChartCode() { return op.chart_code; }
    short &getDatafile() { return op.datafile; }
	short &getDefRectMethod() { return op.def_rectmethod; }
	};

// Useful Macros

#define is_set(x,n)   (((x)&(n))?1:0)
#define set_it(v,n,x) v|=((x)?(n):0)


#endif /* _SETUP___HPP_ */

