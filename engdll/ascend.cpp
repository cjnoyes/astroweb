#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include <stdlib.h>
#include "compute.fns"
#include "datetime.fns"
#include "julian.h"
#include "utl.h"
#include "lngdbl.h"

/* these variables are in radians */
double obliquity;
double right_asc_mh;
double sin_obliquity;
double cos_obliquity;
double tan_obliquity;
double tan_latitude;
double med_coeli;
double asc_rads;
double latitude;

/* function to derive obliquity of the ecliptic, which is in the
   neighborhood of 23 degrees, 27', but this function adjusts it to
   be exact for the given year and sets up variables. This function
   assumes that the julian date for the given day has already been
   calculated. */

void setup_obliquity( JULIAN *ptr )
{
 double temp, eps;

 temp = ( -1.81e-3 * ptr->jd_cent ) + 5.9e-3;
 temp *= ptr->jd_cent;
 temp += 4.6845e1;
 temp *= ptr->jd_cent;
 eps = 2.345229444e1 - ( temp / 3600.0 );
 obliquity = eps;
 eps = d2r( eps );
 sin_obliquity = _sin( eps );
 cos_obliquity = _cos( eps );
 tan_obliquity = _tan( eps );
}

/* convert radians to minutes */

MINU rads_2_mins( double rads )
{
 double temp;

 temp = rads * (360.0 * 60.0);
 temp /= ( 2.0 * PI );
 temp += .01;
 return( (MINU) temp );

}
/* function to divide numerator and divisor and do atan of result.
   also determines quadrant of result and adjusts result so minutes
   are in range of d2m(0-360) */

MINU div_2_minutes( double numerator, double divisor )
{
 double res, atres;

 if ( divisor != 0.0 )
    res = numerator / divisor;
 else if ( numerator > 0.0 )
    return( d2m(90) );
 else if ( numerator < 0.0 )
    return( d2m(270) );
 else 
    return( 0 );
 atres = atan( res );
 if ( divisor < 0.0 )
    atres += PI;
 else if ( numerator < 0.0 )
    atres += 2.0 * PI;
 return( rads_2_mins( atres ) );
}

/* function to divide numerator and divisor and do atan of result.
   also determines quadrant of result and adjusts result so minutes
   are in range of d2m(0-360) */

double div_2_rads( double numerator, double divisor )
{
 double res, atres;

 if ( divisor != 0.0 )
    res = numerator / divisor;
 else if ( numerator > 0.0 )
    return( d2m(90) );
 else if ( numerator < 0.0 )
    return( d2m(270) );
 else 
    return( 0 );
 atres = atan( res );
 if ( divisor < 0.0 )
    atres += PI;
 else if ( numerator < 0.0 )
    atres += 2.0 * PI;
 return( atres );
}


double degrees_2_rads( DEGR degs, MINU mins )
{
 double degrees;

 if ( degs < 0 )
    degs = abs( degs );
 if ( degs > 360 )
    degs %= 360;
 if ( mins < 0 )
    mins = abs( mins );
 if ( mins > 360 )
    mins %= 60;
 degrees = (double) degs;
 degrees += (double) mins / 60.0;
 return( degrees * (( 2.0 * PI ) / 360.0 ) );
}

void sidereal_2_angular( TIM *tp, DEGR *degs, MINU *mins, MINU *secs )
{
 short side;

 side = tp->hours * 15;
 side += tp->minutes / 4;
 *degs = side;
 side = tp->minutes % 4;
 side *= 60;
 side += tp->seconds;
 *mins = side / 4;
 *secs = side % 4;
}


MINU comp_med_coeli( TIM *tp )
{
 DEGR degs, m;
 MINU mins, mc, secs;
 double x;

 sidereal_2_angular( tp, &degs, &mins, &secs );  /* convert sidereal time to angular */
 right_asc_mh = degrees_2_rads( degs, mins ); /* change to radians */
 x = (((double) secs)/60.0)/60;
 x *= ( PI / 180.0 );
 right_asc_mh += x;
 if ( right_asc_mh > ( PI + PI ) )
    right_asc_mh -= ( PI + PI );
 mc = div_2_minutes( _tan( right_asc_mh ), cos_obliquity );
 med_coeli = div_2_rads( _tan( right_asc_mh ), cos_obliquity );
 m = abs( (mc / 60) - degs );
 if ( m > 150 && m < 210 ) {
    mc += d2m(180);
    med_coeli += PI;
    }
 mc %= d2m(360);
 med_coeli = fmodulus(med_coeli,TWO_PI);
 return( mc );
}


MINU calc_ascendant( DEG_MIN *lat )
{
 double temp, temp2, temp3;

 latitude = degrees_2_rads( lat->degrees, lat->minutes ); 
 tan_latitude = _tan(latitude);
 temp = _cos( right_asc_mh );
 temp2 = ( sin_obliquity * tan_latitude );
 temp3 = ( cos_obliquity * _sin( right_asc_mh ) );
 temp2 += temp3;
 if ( temp2 == 0.0 && temp3 == 0.0 )
    asc_rads = PI/2.0;
 else
    asc_rads= div_2_rads( temp, -temp2 );
 if ( temp2 == 0.0 && temp3 == 0.0 )
    return( d2m(90) );
 else
    return( div_2_minutes( temp, -temp2 ) );
}
