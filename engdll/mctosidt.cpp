#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "ascend.h"
#include "compute.fns"
#include "datetime.fns"
#include "ascend.h"
#include "lngdbl.h"

TIM *right_asc_rads_to_sid_time( double ram, TIM *buf)
{
 ram *= (180 / PI );
 ram /= 15.0;

 buf->hours = (int)floor(ram);
 ram -= (double)buf->hours;
 ram *= 60.0;
 buf->minutes = (int)floor(ram);
 ram -= (double)buf->minutes;
 ram *= 60.0;
 buf->seconds = (int)ram;
 return( buf );
}

/* function to extract hours and minutes from sidereal time for a given date */

void calc_side_to_hm( DATES *dp, double xside, short *hours, short *minutes )
{
 double side;

 side = calc_base_side( dp );
 side = fmodulus( side, 24.0 );
 xside -= side;
 if ( xside < 0.0 )
    xside += 24.0;
 xside /= 1.0027777;
 *hours = (int) floor(xside);
 xside -= (double) *hours;
 xside *= 60.0;
 *minutes = (int) xside;
}

/* function to de-process sidereal time to extract hours and minutes from
   the sidereal time for a given date */

void sidereal_time_to_gmt_hrs( DATES *dp, HRS *hours, MINU *min,  DEG_MIN *longitude,
	 TIM *sidereal, char hem )
{
 double side;
 TIM local;

 proc_longitude( longitude, &local );
 if ( longitude->dir == 'e' || longitude->dir == 'E' ) {
    sidereal->hours -= local.hours;  /* make sidereal at gmt */
    sidereal->minutes -= local.minutes;
    sidereal->seconds -= local.seconds;
    }
 else {
    sidereal->hours += local.hours;
    sidereal->minutes += local.minutes;
    sidereal->seconds += local.seconds;
    }

 if ( hem == 's' || hem == 'S' ) {
    sidereal->hours -= 12;
    }
 adj_time( sidereal );
 side = (double)sidereal->seconds;
 side /= 60.0;
 side += (double)sidereal->minutes;
 side /= 60.0;
 side += (double) sidereal->hours;
 side =  fmodulus( side, 24.0 );
 calc_side_to_hm( dp, side, (short *)hours, (short *)min );
}


void hs_min_to_TIM( DATES *dp, TIM *time,  DEG_MIN *lng, DEG_MIN *lat,
	 int hs_min, int hs_sys, int hs, int loc_hours, int loc_minutes  )
{
 double ramc;
 MINU h, m;
 TIM st;

 ramc = house_to_ramc( hs+1, hs_min, lat, hs_sys );
 right_asc_rads_to_sid_time( ramc, &st );
 sidereal_time_to_gmt_hrs( dp, &h, &m, lng, &st, lat->dir );
 time->hours = h;
 time->hours -= loc_hours;
 time->minutes = m;
 time->minutes -= loc_minutes;
 time->seconds = 0;
 adj_time( time );
}
