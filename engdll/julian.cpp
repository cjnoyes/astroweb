#include "stdafx.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "julian.h"
#include "astro.h"
#include <math.h>
#include "datetime.fns"
/* routines to handle julian dates */



extern double trunc_val( double val );

/* function to return raw julian day number from DATES struct.
   These dates are epoch 1900.0 */

double base_julian_date( DATES *dp )
{
 int m=0, d=0, y=0, a=0;
 double jul = 0.0;

 assert(dp!=NULL);
 d = dp->day;
 if ( dp->month < 3 ) {
    y = dp->year -1;
    m = dp->month + 12;
    }
 else {
    y = dp->year;
    m = dp->month;
    }
 if ( dp->year < 0 )
    y++;
 if (( y > 1582 ) || ( y == 1582 && m > 10 ) ||     /* gregorian calendar */
	    ( y == 1582 && m == 10 && d > 15 ) ) {
     a = y / 100;
     jul = (double) (( 2 - a )+(a/4));
     }
 if ( y < 0 ) {
    jul += trunc_val( (365.25 * (double) y)-0.75 ) - 694025;
    }
 else {
    jul += floor( 365.25 * (double) y ) - 694025;
    }
 jul += (double) d;
 jul -= 0.5;
 jul += floor( 30.6001 * (double)(m+1));
 return( jul );
}

/* return full julian date based on date and number of hours. Also subtracts
   julian date of epoch if epoch == 1980. Allows for more epochs if nec.
   Frequently called epochs are added by using constants */


double julian_date( DATES *dp, int hours, int minutes, int epoch )
{
 static double jul_1989;
 static int set_1989=0;
 double jul = 0;
 DATES t;

 assert( dp != NULL );
 jul = base_julian_date( dp );
 jul += (double)( hours / 24.0 );
 jul += (double)( minutes / 1440.0 );
 jul += 0.5;

 switch( epoch ) {
     case 1980 :
	  return( jul - 29220 );
	  break;
     case 1900 :
	  return( jul );
	  break;
     case 0 :
	  return( jul + 694025 );
	  break;
     case 1989 :  /* For Asteroids */
	  if ( !set_1989 ) {
	     t.year = 1989;
	     t.month = 10;
	     t.day = 1;
	     jul_1989 = base_julian_date( &t );
	     set_1989 = 1;
	     }
	  return( jul - jul_1989 );
	  break;
	 case -4000:
      return( jul + 2415020 );
	  break;
	 case -4713:
	  return( jul + 2415020 );
	  break;
     default :
	  t.year = epoch;
	  t.month = 1;
	  t.day = 1;
	  return( jul - base_julian_date( &t ) );
	  break;
     }
}

void julian_calc( DATES *dp, int hours, int minutes, JULIAN *j )
{
 LONG_DOUBLE jd=0.0;

 assert( dp != NULL && j != NULL );
 assert( hours>=0 && hours < 24 && minutes>=0 && minutes < 60 );
 jd = julian_date( dp, hours, minutes, 1900 );
 j->jd_1900 = jd;
 jd /= 36525.0;
 j->jd_cent = jd;
 j->jd_cent_sq = (jd * jd);
 jd = j->jd_1900;
 jd += 2415020.0;
 j->jd_base = jd;
 jd /= 36525.0;
 j->jdb_cent = jd;
 j->jdb_cent_sq = ( jd * jd );
 jd = julian_date( dp, hours, minutes, 1989 );
 j->jd_1989 = jd;
 jd /= 36525.0;
 j->jd_cent1989 = jd;
 j->jd_cent_sq1989 = (jd * jd);
 j->jd_4000bc = julian_date( dp, hours, minutes, -4000 );
}
