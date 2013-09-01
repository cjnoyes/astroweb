
#include <math.h>
#include "astro.h"
#include "compute.fns"
#include "utl.h"
#include <float.h>
#include "lngdbl.h"

/* fuction that returns a Truncated double argument such that
   trunc_val(-21.7) = -21.0, and trunc_val(21.7) = 21.0 */

double trunc_val( double val )
{
 if ( val < 0.0 )
    return( ceil( val ) );
 else
    return( floor( val ) );
}

double check_dbl( double d, int *flip )
{
 *flip = 0;

 if ( d > (PI/2.0) ) {
    do {
       *flip ^= 1;
       d -= PI;
       } while (d > PI/2.0);
    }
 else if ( d < -(PI/2.0) ) {
    do {
       *flip ^= 1;
       d += PI;
       } while (d < -(PI/2.0) );
    }
 return( d );
}
#ifndef NOLNGDBL
long double check_dbll( long double d, int *flip )
{
 *flip = 0;

 if ( d > (PI/2.0) ) {
    do {
       *flip ^= 1;
       d -= PI;
       } while (d > PI/2.0);
    }
 else if ( d < -(PI/2.0) ) {
    do {
       *flip ^= 1;
       d += PI;
       } while (d < -(PI/2.0) );
    }
 return( d );
}
#endif

double xtan( double t )
{
 if ( t <= -(PI/2.0) ) {
    while ( t < -(PI/2.0) )
	t += PI;
    }
 else if ( t > (PI/2.0) ) {
    while ( t > PI/2.0 )
	t -= PI;
    }
 return( tan( t ) );
}

#ifndef NOLNGDBL
LONG_DOUBLE xtanl( LONG_DOUBLE t )
{
 if ( t <= -(PI/2.0) ) {
    while ( t < -(PI/2.0) )
	t += PI;
    }
 else if ( t > (PI/2.0) ) {
    while ( t > PI/2.0 )
	t -= PI;
    }
 return( tanl( t ) );
}
#endif

double xsin( double t )
{
 int f;
 t = check_dbl(t, &f );
 t = sin( t );
 if ( f )
    return( -t );
 else
    return( t );
}

#ifndef NOLNGDBL
LONG_DOUBLE xsinl( LONG_DOUBLE t )
{
 int f;
 t = check_dbll(t, &f );
 t = sinl( t );
 if ( f )
    return( -t );
 else
    return( t );
}
#endif

double xcos( double t )
{
 int f;
 t = check_dbl(t, &f );
 t = cos( t );
 if ( f )
    return( -t );
 else
    return( t );
}

#ifndef NOLNGDBL
LONG_DOUBLE xcosl( LONG_DOUBLE t )
{
 int f;
 t = check_dbll(t, &f );
 t = cosl( t );
 if ( f )
    return( -t );
 else
    return( t );
}
#endif

/* function that completes fmod in library. If the number involved needs
   to be fmod'ed then fmod is called otherwise it is skipped. If the
   remaining number is negative then divisor is added to that number.
   Returns the corrected number. Used for degrees and radians which need
   to be kept within a circle */

double fmodulus( double num, double divisor )
{
 if ( fabs( num ) >= divisor )
    num = fmod( num, divisor );
 if ( num < 0.0 )
    num += divisor;
 return( num );
}


double atan2rads( double x, double y )
{
 double temp1, temp2;

 if ( x != 0.0 ) {
    temp1 = y / x;
    temp2 = atan( temp1 );
    if ( x < 0.0 )
       temp2 += PI;
    else if ( y < 0.0 )
       temp2 += (2.0 * PI );
    }
 else if ( x > 0.0 )
    return( PI/2.0 );
 else if ( x < 0.0 )
    return( PI+(PI/2.0) );
 else
    return( 0.0 );

 return( temp2 );
}


#define EPSILON 0.00000001

/* solve keppler's equation for the eccentric anomaly. see page 85 in
   "Practical Astronomy with your calculator". eccen_val is E, delta_ecc
   is delta E. Takes quanities in radians and returns in radians */

double kepler_eccentric_anomaly( double mean_anomaly, double eccentricity )
{
 double delta_ecc, eccen_val, result;

 eccen_val = mean_anomaly - ( TWO_PI * floor( mean_anomaly / TWO_PI ) );
 result = eccen_val - ( eccentricity * _sin( eccen_val ) ) - mean_anomaly;
 while ( fabs( result ) > EPSILON ) {
    delta_ecc = result / ( 1 - ( eccentricity * _cos( eccen_val ) ) );
    eccen_val -= delta_ecc;
    result = eccen_val - ( eccentricity * _sin( eccen_val ) ) - mean_anomaly;
    }
 return( eccen_val );
}

/* find true anomaly. see page 84 in "Practical Astronomy with your
   calculator". Returns answer in radians. NOTE: change from earlier
   versions, caller MUST have called kepler_ecc...() before calling
   this function. This function no longer expects mean_anomaly as it's
   first argument. */

double true_anomaly( double eccentricity, double kepler )
{
 double _true, temp1, temp2;

 temp1 = 1.0 + eccentricity;
 temp2 = 1.0 - eccentricity;
 temp1 /= temp2;
 temp1 = pow( temp1, 0.5 );
 temp2 = _tan( kepler / 2.0 );
 _true = atan( temp1 * temp2 ) * 2.0;
 return( _true );
 }
