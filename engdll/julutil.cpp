#include "stdafx.h"
#include "julian.h"
#include "astro.h"
#include <math.h>
#include "datetime.fns"
/* routines to handle julian dates */
#include "lngdbl.h"

extern double trunc_val( double val );

void julian_update( JULIAN *j )
{
 double jd = j->jd_1900;

 jd /= 36525.0;
 j->jd_cent = jd;
 j->jd_cent_sq = pow( jd, 2.0 );
 jd = j->jd_1900;
 jd += 2415020.0;
 j->jd_base = jd;
 jd /= 36525.0;
 j->jdb_cent = jd;
 j->jdb_cent_sq = pow( jd, 2.0 );
}

void julian_to_date_tim( JULIAN *j, DATES *d, TIM *t )
{
 double x, a, b, ce, i, f, g, dy;

 x = j->jd_1900 + 0.5;
 i = trunc_val( x );
 f = x - i;
 if ( f == 1.0 ) {
    f = 0.0;
    i += 1.0;
    }
 if ( i > -115860.0 ) {
    a = trunc_val(( i / 36524.25 )+9.9835726e-1)+14;
    i += 1.0 + a - trunc_val( a / 4.0 );
    }
 b = trunc_val((i/365.25)+8.02601e-1);
 ce = i - trunc_val((365.25*b)+7.50001e-1)+416;
 g = trunc_val(ce/30.6001);
 dy = ce-trunc_val(30.6001 * g)+f;
 d->day = (int) dy;
 dy -= (double)d->day;
 if ( f > 13.5 )
    d->month =(int) g - 13;
 else
    d->month =(int) g-1;
 if ( d->month < 3 )
    d->year = (int) b + 1900;
 else
    d->year = (int) b + 1899;
 if ( dy < 0.0 ) {
    dy = -dy;
    if ( d->day > 0 )
       --d->day;
    else if ( d->month > 1 ) {
       --d->month;
       d->day = days_to_month( d->month, d->year );
       }
    else {
       --d->year;
       d->month = 12;
       d->day = 31;
       }
    }
 dy *= 24;
 t->hours = (int)dy;
 dy -= (double) t->hours;
 dy *= 60;
 t->minutes = (int)dy;
 dy -= t->minutes;
 t->seconds = (int) dy;
 d->day += adj_time(t);
 adj_date(d);
}
