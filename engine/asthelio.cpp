#include "stdafx.h"
#include <float.h>
#include <errno.h>
#include <math.h>
#include <setjmp.h>
#include <memory.h>
#include <signal.h>
#include "astro.h"
#include "useri.h"
#include "flags.h"
#include "compute.fns"
#include "datetime.fns"
#include "errors.fns"
#include "utl.h"
#include "julian.h"
#include "dataext.h"
#include <setjmp.h>

extern jmp_buf j;

extern double helio_long;

extern void GetHelioPlanetData( int plan, AS_INF *inf, double jul );

extern void fpehnd(int sig, int num );

void helio_misc_calc( AS_INF  *ptr, BITS code, short year )
{
 if ( code & SIDEREAL )
    ptr->minutes_total = tropical_2_sidereal( ptr->minutes_total, year );
 ptr->sign = sign( ptr->minutes_total );
 ptr->degree_minor = minor_degrees( ptr->minutes_total );
 ptr->minutes_minor = minor_minutes( ptr->minutes_total );
 ptr->cusp = sign_cusp( ptr->degree_minor, ptr->minutes_minor, ptr->sign );
 if ( code & MISCELL )
    ptr->misc_code = decanates( ptr->degree_minor ) +1;
  if ( ptr->dec_minutes_total ) {
    ptr->dec_degrees = (int)ptr->declination;
	ptr->dec_minutes_minor = ptr->dec_minutes_total % 60;
 }
 else {
	ptr->dec_degrees = 0;
	ptr->dec_minutes_minor = 0;
 }
}

/* function to call other modules and do calculations.
   enter with pointer to array of AS_INF type of structure
   and pointer to AS_DATA type of structure. Also supply
   operating code which selects various options and house_proc
   code which calls house processing routines */

void helio_do_calc( AS_INF  *info, AS_DATA *data, BITS mode, DATA_PACKET *dpk )
{
 long l;
 COUNT i;
 JULIAN jul;
 AS_INF  *inf_ptr;
 int bm;
 short dummy;

 if ( setjmp( j ) ) {        /* Math Error Occurred */
    return;
    }
 if ( signal( SIGFPE, (void(*)(int))fpehnd ) == SIG_ERR ) {
    return;
    }
 if ( mode & NOBIRTHTIM ){
    bm = dpk->rdp->trial.minutes;
    data->hour_adjust = noon_date( &data->birth_date, &data->noon_date,
	    &data->noon_time, &dpk->rdp->trial,
	    data->local_adjust, data->local_minutes );
    julian_calc( &data->noon_date, data->hour_adjust, bm +
	     ( data->hour_adjust >= 0 ? data->local_minutes :
	     -data->local_minutes ), &jul );
    }
 else {
    bm = data->birth_time.minutes;
    data->hour_adjust = noon_date( &data->birth_date, &data->noon_date,
	    &data->noon_time, &data->birth_time,
	    data->local_adjust, data->local_minutes );
    julian_calc( &data->noon_date, data->hour_adjust, data->birth_time.minutes +
	     data->local_minutes, &jul);
    }
 inf_ptr = info + SUN;
 l = sun_minutes( &jul );
 l += d2m(180);
 if ( l > d2m(360) )
    l -= d2m(360);
 inf_ptr->minutes_total = l;
 inf_ptr->longitude = (double) l/60.0;
 inf_ptr->declination = 0;
 inf_ptr->dec_minutes_total = 0;
 ++inf_ptr;
 ++inf_ptr;
 for ( i = MERCURY; i <= PLUTO; ++i, ++inf_ptr ) {
	 inf_ptr->calced = 1;
	 GetHelioPlanetData( i, inf_ptr, jul.jd_4000bc );
	 //planet_minutes( i, &jul, &dummy );
     //inf_ptr->minutes_total = r2m( helio_long );
     }
 for ( i = SUN, inf_ptr = info; i <= PLUTO; ++i, ++inf_ptr ) {
     if ( i != MOON )
	helio_misc_calc( inf_ptr, mode, data->noon_date.year );
     }
 
}
