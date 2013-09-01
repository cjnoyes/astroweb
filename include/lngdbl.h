
extern double xtan(double x);
extern double xsin(double x);
extern double xcos(double x);
#define _tan(x) xtan((x))
#define _sin(x) xsin((x))
#define _cos(x) xcos((x))
#ifdef NOLNGDBL
#define LONG_DOUBLE double
#define _acosl(ld)      acos((ld))
#define _asinl(ld)      asin((ld))
#define _atanl(ld)      atan((ld))
#define _atan2l(ld,ld2) atan2((ld),(ld2))
#define _cosl(ld)       xcos((ld))
#define _sinl(ld)       xsin((ld))
#define _tanl(ld)       xtan((ld))
#define _powl(ld,ld2)   pow((ld),(ld2))
#define _fabsl(ld)      fabs((ld))
#define _sqrtl(ld)      sqrt((ld))
#define _logl(ld)       log((ld))
#define _floorl(ld)     floor((ld))
#define _ceill(ld)      ceil((ld))
#define _exp(ld)        exp((ld))
#define _fmodl(ld,ld2)  fmod((ld),(ld2))
#define _modfl(ld,ld2)  modf((ld),(ld2))
#else
extern long double xtanl(long double x);
extern long double xsinl(long double x);
extern long double xcosl(long double x);
#define LONG_DOUBLE long double
#define _acosl(ld)      acosl((ld))
#define _asinl(ld)      asinl((ld))
#define _atanl(ld)      atanl((ld))
#define _atan2l(ld,ld2) atan2l((ld),(ld2))
#define _cosl(ld)       xcosl((ld))
#define _sinl(ld)       xsinl((ld))
#define _tanl(ld)       xtanl((ld))
#define _powl(ld,ld2)   powl((ld),(ld2))
#define _fabsl(ld)      fabsl((ld))
#define _sqrtl(ld)      sqrtl((ld))
#define _logl(ld)       logl((ld))
#define _floorl(ld)     floorl((ld))
#define _ceill(ld)      ceill((ld))
#define _exp(ld)        expl((ld))
#define _fmodl(ld,ld2)  fmodl((ld),(ld2))
#define _modfl(ld,ld2)  modfl((ld),(ld2))
#endif
