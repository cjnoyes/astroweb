#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "astro.h"
#include "datetime.fns"
#include "compute.fns"

/* returns 1 if year is a leap year */

static BOOL is_leap( short year )
{
 if ( year % 4 == 0 && year % 100 != 0 )
    return( 1 );
 else if ( year % 400 == 0 )
    return( 1 );
 else 
    return( 0 );

}

/* returns number of days to month */

short days_to_month( short month, short year )
{
  static const short day_to_month[16] = { 31, 28, 31, 30, 31, 30, 31, 31, 30,
                                    31, 30, 31, 31, 28, 31, 30 };
  assert( month>0 && month <= 12 );
  if ( month < 1 )
       month += 12;
  if ( month != 2 )
     return( day_to_month[ month - 1 ] );
  else if ( is_leap( year ) )
     return( 29 );
  else
     return( 28 );
}

/* returns 0 if date is valid, 1 if month is invalid, 2 if day is invalid
   3 if year is invalid */

STAT bad_date( DATES *d )
{
 assert(d!=NULL);
 if ( d->month < 1 || d->month > 12 )
    return( 1 );
 if ( d->year < 1585 ) {
    if ( d->day < 1 || d->day > 31 )
       return( 2 );
    else
       return( 0 );
    }
 else if ( d->day >= 1 && d->day <= days_to_month( d->month, d->year ) )
    return( 0 );
 else if ( d->month == 2 && is_leap( d->year ) && d->day == 29 )
    return( 0 );
 else
    return( 2 );
}

/* returns 0 if time is valid by normal 12 hour clock standards
   1 if hours are not 2 if minutes are not. If mode is 0 12 hour clock
   is assumed. If mode is not a 24 hour clock is assumed */

STAT bad_time( TIM *t, short mode )
{
 HRS max;

 assert(t!=NULL);
 if ( mode )
    max = 24;
 else
    max = 12;
 if ( t->hours < 0 || t->hours > max )
    return( 1 );
 else if ( t->minutes < 0 || t->minutes > 59 )
    return( 2 );
 else
    return( 0 );
}

/* returns number of days difference from start month for month/day/year 
   passes pointer to date structure dp */ 

short day_number( short st_month, DATES *dp )
{
 short days=0, total=0;
 static const short v[12] = { 0, 1, -1, 0, 0, 1, 1, 2, 3, 3, 4, 4 };
 
 assert(dp!=NULL);
 assert(st_month >= 1 && st_month <= 12);
 days = (( st_month -1 ) * 30 ) + v[st_month-1];
 total = (( dp->month -1 ) * 30 ) + v[dp->month-1] + dp->day;
 if ( is_leap( dp->year ) ) {
    if ( dp->month > 2 )
       ++total;
    else if ( st_month > 2 )
       ++days;
    }
 return( (total-days) );
}

/* functions to create, generate  and manage sidereal time
   undefined behavior before 700 a.d. */

double calc_base_side(DATES *dp )
{
 DATES dt;
 double jd, xjd, t, t0, r, r1, b;

 assert(dp!=NULL);
 dt.year = dp->year;
 dt.day = 1;
 dt.month = 1;
 jd = base_julian_date( &dt );
 xjd = base_julian_date( dp );
 t = jd / 36525.0;
 r = 6.6460656 + ( 5.1262e-2 * t ) + ( 2.581e-5 * t * t );
 r1 = 2400.0 * ( t - (( dp->year - 1900 )/100.0));
 b = 24.0 - r - r1;
 t0 = ( 6.57098e-2 * ( xjd - jd ) ) - b;
 return( t0 );
}

/* function to calculate sidereal time */

double calc_sidereal_t( DATES *dp, int hours, int minutes )
{
 double side, t;

 assert(dp!=NULL);
 side = calc_base_side( dp );
 t = (double) ( hours + 12 ) + ((double) minutes / 60.0);
 t *= 1.0027777;
 side += t;
 return( fmodulus( side, 24.0 ) );
}

/* function to test if time rolls over. If time rolls over in any
   part of time structure it is adjusted. If date rolls over, the
   number of days rolled is returned by the function. */

NUM adj_time( TIM *tp )
{
 NUM dayroll = 0;

 assert(tp!=NULL);
 if ( tp->seconds < 0 ) {
    tp->minutes -= ( abs( tp->seconds) / 60 ) +1;
    tp->seconds = 60 - (abs( tp->seconds ) % 60);
    }
 else if ( tp->seconds > 59 ) {
    tp->minutes += ( tp->seconds / 60 );
    tp->seconds %= 60;
    }
 if ( tp->minutes < 0 ) {
    tp->hours -= ( abs( tp->minutes ) / 60 ) +1; 
    tp->minutes = 60 - ( abs( tp->minutes ) % 60 );
    }
 else if ( tp->minutes > 59 ) {
    tp->hours += ( tp->minutes / 60 );
    tp->minutes %= 60;
    }
 if ( tp->hours < 0 ) {
    dayroll = -(( abs( tp->hours ) / 24 ) +1);
    tp->hours = 24 - ( abs( tp->hours ) % 24 );
    }
 else if ( tp->hours > 23 ) {
    dayroll = tp->hours / 24;
    tp->hours %= 24;
  }
 return( dayroll );

}

/* function to process local sidereal time by longitude */

void proc_longitude( DEG_MIN *longitude, TIM *local )
{
 float total;

 assert(longitude != NULL && local != NULL);
 total = ((float)longitude->minutes * 4.0) / 60.0;
 total += (float)longitude->degrees * 4.0;
 local->hours = (int) (total / 60.0);
 total -= (float) (local->hours * 60);
 total *= 60.0;
 local->minutes = (int) (total / 60.0);
 local->seconds = ((int) total) % 60;
}

/* procedure to generate sideral time sidereal for given date dp,
   for hours past noon, longitude, hemisphere north or south hem 
   returns sidereal time in time structure */

void sidereal_time( DATES *dp, HRS hours, MINU min,  DEG_MIN *longitude,
         TIM *sidereal, char hem )
{
 double side=0.0, dummy=0.0;
 TIM local = {0,0,0};
 int sd=0;

 assert( dp != NULL && longitude != NULL && sidereal != NULL);
 assert( hours < 24 && hours >= 0 && min >= 0 && min < 60 && (hem=='N'||hem=='S'));
 side = calc_sidereal_t( dp, hours, min );
 sidereal->hours = (int) side;
 side = modf( side, &dummy  );
 sd = (int) (side * 3600.0);
 sidereal->minutes = sd / 60;
 sidereal->seconds = sd % 60;
 proc_longitude( longitude, &local );
 if ( longitude->dir == 'e' || longitude->dir == 'E' ) {
    sidereal->hours += local.hours;  /* make sidereal at place of birth */
    sidereal->minutes += local.minutes;
    sidereal->seconds += local.seconds;
    }
 else {
    sidereal->hours -= local.hours;
    sidereal->minutes -= local.minutes;
    sidereal->seconds -= local.seconds;
    }
 adj_time( sidereal );
 if ( hem == 's' || hem == 'S' ) {
    sidereal->hours += 12;
    }
 adj_time( sidereal );
 return;
}

/* function to test if date rolls over. If date rolls over in any
   part of date structure it is adjusted. if date is greater than
   99 or less than 10 it is reset to max or min val and -1 is returned
   to flag error. Otherwise returns 0  */

STAT adj_date( DATES *dp )
{
 short year = dp->year;
 BOOL flag = 0;

 if ( dp->month < 1 ) {
    dp->year--;
    dp->month += 12;
    }
 else if ( dp->month > 12 ) {
    dp->year++;
    dp->month -= 12;
    }
 if ( dp->day < 1 ) {
    dp->month--;
    if ( dp->month == 0 )
       dp->month = 12;
    dp->day += days_to_month( dp->month, year );
    }
 else if ( dp->day > days_to_month( dp->month, year ) ) {
    dp->month++;
    dp->day -= days_to_month( dp->month-1, year );
    }
 if ( dp->month < 1 ) {
    dp->year--;
    dp->month += 12;
    }
 else if ( dp->month > 12 ) {
    dp->year++;
    dp->month -= 12;
    }
 if ( dp->year < 1585 ) {
    dp->year = 1585;
    flag = -1;
    }
 else if ( dp->year > 2100 ) {
    dp->year = 2100;
    flag = -1;
    }
 return( flag );
}


/* function to generate noon date noon_date for given date dp, also adjusts
   time for noon_date using time supplied based on local adjustment, using
   local adjusts to gmt and returns hours past noon on given noon_date */

HRS noon_date( DATES *dp, DATES *noon_date, TIM *noon, TIM *tp,
					     HRS local_hours, MINU local_min )
{
 short hours = 0;
 NUM roll=0;
 STAT err=0;

 assert(dp!=NULL && noon_date != NULL && noon != NULL && tp != NULL);
 assert( local_hours <= 12 && local_hours >= -12 );
 assert( local_min >= 0 && local_min < 60 );
 memcpy( noon_date, dp, sizeof( DATES ) );
 memcpy( noon, tp, sizeof( TIM ) );
 noon->hours += local_hours;             /* adjust to gmt */
 if ( local_hours < 0 )
    noon->minutes -= local_min;
 else
    noon->minutes += local_min;
 roll = adj_time( noon );          /* normalize and check for day roll */
 noon_date->day += roll;           /* if so adjust the date */
 err = adj_date( noon_date );      /* normalize the date and check */
 if ( noon->hours + hours < 12 ) {
    --noon_date->day;
    noon->hours += 24;
    err = adj_date( noon_date );
    }
 hours += noon->hours - 12;
 return( hours );         /* return hours past noon */
}
