#include "stdafx.h"
#include "astro.h"
#include "flags.h"
#include "julian.h"
#include "compute.fns" 
#include "datetime.fns"
#include "dataext.h"
#include "errors.fns"
#include "math.h"
#include <assert.h>
#include <memory.h>
#include <string.h>
/* 09/10/92  Modified for C++ Windows */

#define d2m(d) ((d)*60)

static int step;

AS_INF * moon_series = NULL, * prog = NULL,
       * prog_moon_series = NULL; /* for progression moon analysis
       for each month, and for progressed to progressed aspects, and
       for aspects of moon versus progressed planets */

/* function to progress a date. Function implements Secondary, Tertiary,
   minor progressions. Once date is progressed, then date is adjusted
   to normal limits. If an error exists in date function returns 1
   otherwise it returns 0 indicating no error */

int progress_date( AS_DATA *data, PROGR_DATA *pdata )
{
 double jd, jdx;
 DATES dx, t;
 COUNT days, hours, minutes, x;

 switch( pdata->progress_system ) {
       case SECONDARY_PR :
       case SECONDARY_PR +100 :
	    t = pdata->offset;
	    if ( pdata->progress_system >= 100 ) {
	       jd = base_julian_date( &t );
	       jdx = base_julian_date( &data->noon_date );
	       jd -= jdx;
	       jd /= 365.25;
	       days = (int) jd;
	       jd -= (double)days;
	       jd *= 24.0;
	       hours = (int) jd;
	       jd -= (double)hours;
	       jd *= 60;
	       minutes = (int) jd;
	       }
	    else {
	       days = t.year;
	       hours = t.month * 2;
	       minutes = t.day * 2;
	       }
	    break;
       case MINOR_PR :
       case MINOR_PR + 100 :
       case TERTIARY_PR:
       case TERTIARY_PR + 100:
	    t = pdata->offset;
	    if ( pdata->progress_system >= 100 ) {
	       jdx = base_julian_date( &pdata->offset );
	       }
	    else {
	       dx = data->noon_date;
	       dx.year += t.year;
	       dx.month += t.month;
	       dx.day += t.day;
	       adj_date( &dx );
	       jdx = base_julian_date( &dx );
	       }
	    jd = base_julian_date( &data->noon_date );
	    jdx -= jd;
	    switch( pdata->progress_system ) {
		  case MINOR_PR :
		  case MINOR_PR + 100 :
		       jdx *= 27.32;
		       jdx /= 365.2422;
		       break;
		  case TERTIARY_PR:
		  case TERTIARY_PR+100:
		       jdx /= 27.32;
		       break;
		  }
	    days = (COUNT) jdx;
	    jdx -= (double) days;
	    jdx *= 24.0;
	    hours = (COUNT)jdx;
	    jdx -= (double) hours;
	    jdx *= 60.0;
	    minutes = (COUNT)jdx;
	    break;
       default :
	    break;
       }
 data->minutes_adjust += minutes;
 if ( data->minutes_adjust > 59 ) {
    ++hours;
    data->minutes_adjust %= 60;
    }
 data->hour_adjust += hours;
 if ( data->hour_adjust > 23 ) {
    ++days;
    data->hour_adjust %= 24;
    }
 data->noon_date.day += days;
 x = adj_date( &data->noon_date );
 x = adj_date( &data->noon_date );
 return( x );
}

/* Function to produce a moon series for a progressed moon. A separate 
   moon calculation is calculated for each month four prior to the
   month and one year afterward. Presently moon is stepped 1 degree, 4min
   for each month. */

void make_moon_series( MINU moon, BITS mode, short year )
{
 COUNT i;
 MINU md;
 
 for ( i = 4, md = moon; i >= 0; --i, md -= step ) {
     (moon_series + i)->minutes_total = adjust_mins( md );
     (moon_series + i)->calced=1;
 }
 for ( i = 5, md = moon + step; i <= 15; ++i, md += step ) {
     (moon_series + i)->minutes_total = adjust_mins( md );
	 (moon_series + i)->calced=1;
 }
 for ( i = 0; i <= 15; ++i ) {
     misc_calc( (moon_series + i), mode, year );
     }
}

/* function to process aspects For Progressions. Space is allocated
   intitially for 15 aspects as starting point. Then aspects are
   calculated. When total number of aspects is known, then space
   is resized to fit with realoc. This process is followed for
   all members of the array that main points to. If mode is set
   then we have no birthtime so we have to skip those planets
   which depend on birth time. maxx determines the maximum number
   of elements that are indexed. This is necessary and different
   from the variable maxp in that a moon series only has 16 enries,
   where a normal AS_INF array has 18 max. */

void prog_process_aspects( AS_INF *planet, AS_INF *sub,
						      BITS mode, NUM maxx )
{
 ASPEC *asp;
 AS_INF *pl;
 NUM    maxp;
 COUNT  pno, aspects;
 MINU   maxorb;
 BITS   hm = mode & HOUSES;
 //extern FILE *teststream;
 
 maxorb = d2m(2);
 if ( mode & ASTEROID )
	 maxp = CHIRON;
 if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = VERTEX;
 else if ( !(mode & NOBIRTHTIM) )
    maxp = IM_COELI;
 else
   maxp = NORTH_NODE;
 if ( maxx == -1 )
    maxx = maxp;
 mode &= NOBIRTHTIM;

 for ( pno = SUN, pl = planet; pno < maxx; ++pl, ++pno ) {
	 if ( (!mode || ( pno != ASCEND )) && pl->calced ) {
    //fprintf(teststream,"pno=%d\n", pno );
	asp = new ASPEC[30];
	if ( asp == NULL ) {
	   errno = ENOMEM;
	   pl->aspectr = NULL;
	   pl->no_aspects = 0;
	   return;
	   }
	memset( asp, 0, 30*sizeof(ASPEC));
	pl->aspectr = asp;
	aspects = do_aspect( pl, pno, sub, maxorb, mode,
				       ( ( hm ) ? -maxp : maxp), 0 );
	if ( !aspects ) {
	   delete asp;
	   pl->aspectr = NULL;
	   pl->no_aspects = 0;
	   }
	else {
	   assert( aspects > 0 );
	   assert(aspects < 30 );
	   pl->aspectr = (ASPEC *) reallocx( asp, aspects * sizeof(ASPEC) );
	   pl->no_aspects = aspects;
	   }
	}
    else {
	  pl->aspectr = NULL;
	  pl->no_aspects = 0;
	  }
    }
}


/* function to process aspects For Progressions. Space is allocated
   intitially for 15 aspects as starting point. Then aspects are
   calculated. When total number of aspects is known, then space
   is resized to fit with realoc. This process is followed for
   all members of the array that main points to. If mode is set
   then we have no birthtime so we have to skip those planets
   which depend on birth time. maxx determines the maximum number
   of elements that are indexed. This is necessary and different
   from the variable maxp in that a moon series only has 16 enries,
   where a normal AS_INF array has 18 max. */

void prog_process_ms_aspects( AS_INF *planet, AS_INF *ms,
						      BITS mode, NUM maxx )
{
 ASPEC *asp;
 AS_INF *pl, *mspt;
 NUM    maxp;
 COUNT  pno, msno, aspects;
 MINU   maxorb;
 BITS   hm = mode & HOUSES;
 //extern FILE *teststream;
 
 //fprintf(teststream,"enter ms asp\n");
 maxorb = d2m(2);
 if ( mode & ASTEROID )
	 maxp = CHIRON;
 if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = VERTEX;
 else if ( !(mode & NOBIRTHTIM) )
    maxp = IM_COELI;
 else
   maxp = NORTH_NODE;
 mode &= NOBIRTHTIM;
 if ( !mode )
	 return;
 //fprintf(teststream,"start loop\n");
 for ( msno = 0, mspt = ms; msno < maxx; ++mspt, ++msno ) {
	  //fprintf(teststream,"msno=%d\n", msno,pno );
	  asp = new ASPEC[30];
	  if ( asp == NULL ) {
	     errno = ENOMEM;
	     pl->aspectr = NULL;
	     pl->no_aspects = 0;
	     return;
	     }
	   memset( asp, 0, 30*sizeof(ASPEC));
	   mspt->aspectr = asp;
	   aspects = do_aspect( mspt, MOON, planet, maxorb, mode,
				       ( ( hm ) ? -maxp : maxp), 0 );
	   if ( !aspects ) {
	      delete asp;
	      pl->aspectr = NULL;
	      pl->no_aspects = 0;
	      }
	  else {
	     assert( aspects > 0 );
	     assert(aspects < 30 );
	     mspt->aspectr = (ASPEC *) reallocx( asp, aspects * sizeof(ASPEC) );
	     mspt->no_aspects = aspects;
	     }
       }
}

/* Function to to full Series Progressions including moon Series and
   aspects for Moon Series. Variable begining with P refer to progresed
   and b refer to birth*/

void do_full_progr( AS_DATA *bdata, AS_INF *binf, AS_INF *pinf,
		   BITS bmode, BITS pmode, NUM house_proc, DATA_PACKET *dpk )
{
 BITS mask = ( 0xffff ^ ( BASE_MID | FULL_MID | MID_ASPECT | ASPECTS  ) );

 short hc[13];
 int cc;
 short i, m1, m2;
 JULIAN j1;
 AS_INF *ptr;
 //extern FILE *teststream;
 bmode &= mask;
 pmode &= mask;

 moon_series =(AS_INF *) new AS_INF[17];
 memset( moon_series, 0, 17 * sizeof(AS_INF));
 prog = (AS_INF *) new AS_INF[CHIRON];
 memset( prog, 0, CHIRON * sizeof(AS_INF));
 prog_moon_series = (AS_INF *)new AS_INF[17];
 memset( prog_moon_series, 0, 17 * sizeof(AS_INF));
 if ( moon_series == NULL || prog == NULL || prog_moon_series == NULL ) {
    errno = ENOMEM;
    return;     
    }
 if ( bmode & NOBIRTHTIM )
    pmode |= NOBIRTHTIM;
 else if ( pmode & NOBIRTHTIM )
    bmode |= NOBIRTHTIM;
 cc = dpk->chart_code;
 dpk->chart_code |= PRGCS_CH;
 do_calc( pinf, bdata, pmode, house_proc, dpk );
 julian_calc( &bdata->noon_date, bdata->hour_adjust,
       bdata->birth_time.minutes, &j1 );
 memcpy( alt_cusps, house_cusps, 13 * sizeof( int ) );
 dpk->chart_code = NATAL_CH;
 if ( cc & RELOC_CH )
    dpk->chart_code |= RELOC_CH;
 do_calc( binf, bdata, bmode, house_proc, dpk );
 dpk->chart_code = cc;
 if ( dpk->chart_code & PRGCS_CH ) {
    memcpy( hc, house_cusps, 13 * sizeof( int ) );
    memcpy( house_cusps, alt_cusps, 13 * sizeof( int ) );
    }
 if ( bmode & HOUSES ) {
    for ( ptr = pinf, i = 0; i < 18; i++, ptr++ ) {
	ptr->house = house_number( ptr->minutes_total );
	ptr->house_cusp = house_cusp_intercept( ptr->minutes_total,
	      ptr->house );
	ptr->intercepted = is_sign_intercept( ptr->sign );
	}
    }
 prog_process_aspects( pinf, binf, bmode, -1 );
 if ( errno == ENOMEM )
    return;
 m1 = moon_minutes( &j1, &i );
 j1.jd_1900 += 1.0;
 julian_update( &j1 );
 m2 = moon_minutes( &j1, &i );
 if ( m2 - m1 < 0 )
    m2 += (360*60);
 step = (m2-m1)/12;
 make_moon_series( ( pinf+ MOON )->minutes_total,
               bmode, bdata->birth_date.year );
 memcpy( prog_moon_series, moon_series, (16 * sizeof(AS_INF)) );
 //fputs("aspms1\n", teststream);
 prog_process_ms_aspects( moon_series, binf, bmode, 16 );
 if ( errno == ENOMEM )
    return;
 //fputs("aspms2\n", teststream);
 prog_process_ms_aspects( prog_moon_series, pinf, bmode, 16 );
 memcpy( prog, pinf, (CHIRON * sizeof(AS_INF)) );
 //fputs("aspprg\n", teststream);
 prog_process_aspects( prog, prog, bmode, -1 );
 if ( dpk->chart_code & PRGCS_CH ) {
    memcpy( house_cusps, hc, 13 * sizeof( short ) );
    }
 //fputs("fine\n", teststream);
}
