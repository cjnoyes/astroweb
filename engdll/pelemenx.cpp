#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <math.h>
#include "paths.hpp"
#include "errors.fns"
#include "astro.h"
#include "useri.h"
#include "julian.h"
#include "utl.h"
#include "plans.h"
#include "planpert.fns"

#define PLAN_COUNT 8
extern char *data_name( char * );

/* 09/10/92  Modified for C++ Windows */

extern double fmodulus( double, double );
extern double sun_longitude;

char *getElementFile()
{
 return( "ORBITELE.DAT" );
}

/* Polynomial orbital data for planets.

   data now in MKPELEMT.C data written to ORBITELE.DAT in data dir

ORBIT_ELE orbitals[8] = {
	Mercury
	178.179078, 415.2057519, 3.011e-4, 0.0,
	75.899697, 1.5554889, 2.947e-4, 0.0,
	2.0561421e-1, 2.046e-5, 3e-8, 0.0,
	7.002881, 1.8608e-3, -1.83e-5, 0.0,
	47.145944, 1.1852083, 1.739e-4, 0.0,
	3.870986e-1,

	Venus
	342.767053, 162.553664, 3.097e-4, 0.0,
	130.163833, 1.4080361, -9.764e-4, 0.0,
	6.82069e-3, -4.774e-5, 9.1e-8, 0.0,
	3.393631, 1.0058e-3, -1e-6, 0.0,
	75.779647, 8.9985e-1, 4.1e-4, 0.0,
	7.233316e-1,

	Mars
	293.737334, 53.171376642, 3.107e-4, 0.0,
	3.34218203e2, 1.8407584, 1.299e-4, -1.19e-6,
	9.33129e-2, 9.2064e-5,7.7e-8, 0.0,
	1.850333, -6.75e-4, 1.26e-5, 0.0,
	48.786442, 7.709917e-1, -1.4e-6,  -5.33e-6,
	1.5236883,

	Jupiter
	238.049257, 8.434172183, 3.347e-4, -1.65e-6,
	1.2720972e1, 1.6099617, 1.05627e-3, -3.43e-6,
	4.833475e-2, 1.6418e-4, -4.676e-7, -1.7e-9,
	1.308736, -5.6961e-3, 3.9e-6, 0.0,
	99.443414, 1.01053, 3.5222e-4, -8.51e-6,
	5.202561,

	Saturn
	266.564377, 3.398638567, 3.245e-4, -5.8e-6,
	9.1098214e1, 1.9584158, 8.2636e-4, 4.61e-6,
	5.589232e-2, -3.455e-4, -7.28e-7, -7.4e-10,
	2.492519, -3.9189e-3, -1.549e-5, 4e-8,
	112.790414, 8.731951e-1, -1.5218e-4, -5.31e-6,
	9.554747,

	Uranus
	244.19747, 1.194065406, 3.16e-4, -6e-7,
	1.71548692e2, 1.4844328, 2.372e-4, -6.1e-7,
	4.63444e-2, -2.658e-5, 7.7e-8, 0.0,
	7.72464e-1, 6.253e-4,3.95e-5, 0.0,
	73.477111, 4.986678e-1, 1.3117e-3, 0.0,
	19.21814,

	Neptune
	84.457994, 6.107942056e-1, 3.205e-4, -6e-7,
	4.6727364e1, 1.4245744, 3.9082e-4, -6.05e-7,
	8.99704e-3, 6.33e-6, -2e-9, 0.0,
	1.779242, -9.5436e-3, -9.1e-6, 0.0,
	130.681389, 1.098935, 2.4987e-4,-4.718e-6,
	30.10957,

	Pluto
	95.3113544, 3.980332167e-1, 0.0, 0.0,
	224.017, 0.0, 0.0, 0.0,
	2.5515e-1, 0.0, 0.0, 0.0,
	17.1329, 0.0, 0.0, 0.0,
	110.191, 0.0, 0.0, 0.0,
	39.8151  };
*/

/* Note: the memory pointed to by this pointer should be freed and this
   pointer reset to NULL after all planetary routines are computed */

PLAN_ELE *plan_ptr = NULL;

/* The earth's longitude in radians. Found only once by adding sun's
   longitude + PI. The sun routine must have been called first */

double earth_longitude = 0.0;

/* Macro to simplify repetitive calculations. Note use of Token Pasting
   Operator ## taking member name as an argument */


#define xmul(x)  a = op->x;\
      pp->x = ((((long double) a[3] * j->jd_cent + (long double)a[2] ) \
      * j->jd_cent + (long double)a[1] ) * j->jd_cent + (long double)a[0] )

#define xmula(x)  a = op->x;\
      pp->x = ((((long double) a[3] * j->jd_cent1989 + (long double)a[2] ) \
      * j->jd_cent1989 + (long double)a[1] ) * j->jd_cent1989 + (long double)a[0] )


/* Function to allocate space for and do preparatory work of polynomials,
   using 4 terms for each argument. Complexity in this function is hidden,
   using macro above. If enough memory for the planetary data pointed to by
   plan_ptr is not found, this function returns 0, and sets errno to
   ENOMEM indicating an error, otherwise it returns 1 */

int prepare_planetary_data( JULIAN *j )
{
 char *fi;
 FILE *stream;
 double *a;
 long double tmp, aa;
 int i;
 ORBIT_ELE *op;
 PLAN_ELE *pp;

 plan_ptr = new PLAN_ELE[PLAN_COUNT];
 op = new ORBIT_ELE;
 if ( plan_ptr == NULL || op == NULL ) {
    errno = ENOMEM;
    do_mem_error( 0 );
    if ( op != NULL )
       delete op;
    if ( plan_ptr != NULL ) {
       delete plan_ptr;
       plan_ptr = NULL;
       }
    return( 0 );
    }
 fi = data_name("ORBITELE.DAT");
 if ( ( stream = fopen( fi, "rb" ) ) == NULL ) {
    do_error( fi );
    delete plan_ptr;
    delete op;
    plan_ptr = NULL;
    return( 0 );
    }
 for ( i = 0, pp = plan_ptr; i < 8; ++i, ++pp ) {
     fread( op, sizeof( ORBIT_ELE ), 1, stream );
     a = op->mean_long;
     aa = (long double) a[1] * j->jd_cent;
     aa = murem( aa );
     tmp = (long double)a[0] + (long double)aa +
	    (( ( (long double) a[3] * j->jd_cent ) + (long double)a[2] ) * j->jd_cent_sq );
     pp->mean_long = fmodulus( tmp, 360.0 );
     pp->mean_daily = ( a[1] * 9.856263e-3 ) + ( a[2] + a[3] ) / 36525.0;
     /* Using xmul() macro to reduce multiple operations. Note the macro
	takes the member name in the structure as an argument. See macro
	above for details */
     xmul(perhelion);
     xmul(eccentricity);
     xmul(inclination);
     xmul(ascend_node);
     pp->semi_major = op->semi_major;
     pp->mean_anomaly = d2r( pp->mean_long - pp->perhelion );
     }
 fclose( stream );
 delete op;
 earth_longitude = sun_longitude + PI;
 return( 1 );
}

/* function to free planetary memory and reset the pointer */

void reset_plan_ptr(void)
{
 if ( plan_ptr != NULL )
    delete plan_ptr;
 plan_ptr = NULL;
}
