#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include "julian.h"
#include "return.h"
#include "compute.fns"
#include "datetime.fns"
#include "astro.h"
#include "useri.h"
#include "dataext.h"



extern void julian_update( JULIAN *j );
extern void julian_to_date_tim( JULIAN *j, DATES *d, TIM *t );

double Sun_days_rot = SUN_days_rot;
double Sun_days_min = SUN_days_min;
double Moon_days_rot = MOON_days_rot;
double Moon_days_min = MOON_days_min;
static BIRTH_DB *pRec;


void do_a_return( FILE *stream, JULIAN *j, int pl, int true_val,
				      int reno, char *name, int table, CPtrArray *pDataArray )
{
 int r1, r2, max_err;
 short x;
 DATES d;
 TIM t;

 if ( pl /* Not == SUN */ ) {
    r1 = moon_minutes( j, &x );
    max_err = 10;
    }
 else {
    r1 = sun_minutes( j );
    max_err = 1;
    }
 while ( r1 - true_val > max_err || r1 - true_val < -max_err ) {
       r2 = true_val - r1;
       if ( r2 > 11000 )
	  r2 =(int)((long)true_val - ((long) r1 + 21600L));
       else if ( r2 < -11000 )
	  r2 =(int)(((long)true_val + 21600L) - (long)r1);
       if ( pl /* Not == SUN */ )
	  j->jd_1900 += r2 * Moon_days_min;
       else
	  j->jd_1900 += r2 * Sun_days_min;
       julian_update( j );
       if ( pl /* Not == SUN */ )
	  r1 = moon_minutes( j, &x );
       else
	  r1 = sun_minutes( j );
       if ( x_kb_check() )
	  break;
       }
 julian_to_date_tim( j, &d, &t );
 if ( t.hours > 12 )
    x = 1;
 else
    x = 0;
 if ( pDataArray != NULL && pRec != NULL ) {
   BIRTH_DB *pnew = new BIRTH_DB;
   memcpy(pnew,pRec,sizeof(BIRTH_DB));
   pnew->am_pm = x;
   memcpy(&pnew->birth_data.birth_date,&d,sizeof(DATES));
   memcpy(&pnew->birth_data.birth_time,&t,sizeof(TIM));
   pDataArray->Add((void *)pnew);
   }
 return_print_entry( stream, &d, &t, x, table, name, pl, reno );
}

void do_returns( FILE *stream, BIRTH_DB *birth, RETURN_DATA *rd, in)
{
 double j_start, j_end, jdx;
 JULIAN jb, ja, jx;
 int min, a, b, cnt;
 short x;

 cnt = 1;
 if ( rd->is_prog )
    progress_date( &birth->birth_data, &rd->prog );

 birth->birth_data.hour_adjust = noon_date( &birth->birth_data.birth_date,
       &birth->birth_data.noon_date, &birth->birth_data.noon_time,
       &birth->birth_data.birth_time, birth->birth_data.local_adjust,
       birth->birth_data.local_minutes );
 julian_calc( &birth->birth_data.noon_date, birth->birth_data.hour_adjust,
       birth->birth_data.birth_time.minutes, &jb );
 j_start = julian_date( &rd->start, 0, 0, 1900 );
 j_end = julian_date( &rd->end, 0, 0, 1900 );
 jb.jd_1900 -=  (double) ((birth->birth_data.local_adjust * 60) +
	   birth->birth_data.local_minutes)/ (60.0 * 24);
 julian_update( &jb );
 jdx = jb.jd_1900;
 if ( rd->planet /* Not == SUN */ )
    min = moon_minutes( &jb, &x );
       else
    min = sun_minutes( &jb );
 ja = jb;
 jx = jb;
 ja.jd_1900 -= .5;
 jx.jd_1900 += .5;
 julian_update( &ja );
 julian_update( &jx );
 if ( rd->planet /* Not == SUN */ ) {
    a = moon_minutes( &ja, &x );
    b = moon_minutes( &jx, &x );
    }
       else {
    a = sun_minutes( &ja );
    b = sun_minutes( &jx );
    }
 if ( a - b > 15 )
    b += 21600;
 b -= a;
 if ( rd->planet /* Not == SUN */ )
    Moon_days_min = 1.0 / ((double) b);
       else
    Sun_days_min = 1.0 / ((double) b);
 while ( jdx < j_start ) {
       if ( rd->planet /* Not == SUN */ )
	  jdx += Moon_days_rot;
       else
	  jdx += Sun_days_rot;
       }
 jb.jd_1900 = jdx;
 julian_update( &jb );
 while ( jb.jd_1900 < j_end ) {
       do_a_return( stream, &jb, rd->planet, min,
				      cnt, birth->name, table, pDataArray );
       cnt++;
       if ( rd->planet /* Not == SUN */ )
	  jb.jd_1900 += Moon_days_rot;
       else
	  jb.jd_1900 += Sun_days_rot;
       julian_update( &jb );
       if ( x_kb_check() )
	  break;
       }
}


