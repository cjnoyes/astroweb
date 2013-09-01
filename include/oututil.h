#ifndef _OPDEF__
#define _OPDEF__   1

#define opecaps(f,s,b)  _opecaps(f, s, b, cap)
#define opcaps(f,s,b)   _opcaps(f, s, b, cap)
#define opnum(f,n,v)    _opnum( f, n, v, cap)
#define opnum0(f,n,v)   _opnum0( f, n, v, cap)


extern void _opecaps(FILE *f, const char *s, char *b, int &cap);
extern void _opcaps( FILE *f, const char *s, char *b, int &cap);
extern void _opnum( FILE *f, int n, int v, int &cap);
extern void _opnum0( FILE *f, int n, int v, int &cap);


#endif

#define MONTH_DI 1
#define DATES_DP 2
#define ARABP_DI 3
#define FIXED_DI 4
#define HELIO_DI 5

typedef union PIU_ {
      int i;
      void *p;
      } PIU_;


