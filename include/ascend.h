/* these variables are in radians, or their own natural range
   where they are functions. I calculated functions ahead of time to
   save steps later. Becomes useful in koch_house and in plac_hous
   where multiple iterations of basic formulas are called */

extern double right_asc_mh;
extern double sin_obliquity;
extern double cos_obliquity;
extern double cos_obliquity;
extern double tan_obliquity;
extern double tan_latitude;
extern double med_coeli;
extern double asc_rads;
extern double latitude;
extern double obliquity;
#ifndef PI
#include "utl.h"
#endif
