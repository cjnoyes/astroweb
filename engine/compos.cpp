#include "stdafx.h"
#include "astro.h"
#include "misccalc.h"
#include "flags.h"
#include "compute.fns"
#include "datetime.fns"
#include "dataext.h"
#include "utlduad.h"
#include "utl.h"

extern int maxplan;

void compos_sidetime( int pmc, int ymc, TIM *s )
{
 int mc;
 double ram;

 mc = midpoint( pmc, ymc );
 ram = min_medcoeli_to_ramc( mc );
 right_asc_rads_to_sid_time( ram, s );
}


void do_compos_sum_calc1( AS_INF *info, AS_INF *partner, BITS mode )
{
 count_genders( info, &genders );
 count_elements( info, &elements );
 count_qualities( info, &qualities );
 count_signs( info, sign_count );
}


void do_compos_sum_calc2( AS_INF *info, AS_INF *partner, BITS mode )
{

 if ( mode & HOUSES ) {
    count_hemispheres( info, &hemispheres );
    count_modals( info, &modals );
    count_functional( info, &functionals );
    scout_planet = find_scout_planet( info );
    count_houses( info, house_count );
 }
 if ( mode & ASPECTS )
    count_aspects( info, aspect_count );
}

/*

int check_composite_houses( int asc )
{
 int x, count = 0, a;

 if ( asc != house_cusps[1] ) {
    house_cusps[1] = asc;
    ++count;
    }
 for ( x = 0; x < 12; ++x ) {
     a = house_cusps[x+1] - house_cusps[x];
     if ( a < 0 && a > (-60*260) ) {
	house_cusps[x+1] += (60 * 180);
	house_cusps[x+1] %= (60 * 360);
	++count;
	}
     else if ( a > (60*100) ) {
	house_cusps[x+1] += (60 * 180);
	house_cusps[x+1] %= (60 * 360);
	++count;
	}
     }
 if ( !count && house_cusps[0] != house_cusps[12] ) {
    house_cusps[0] += (60 * 180);
    house_cusps[0] %= (60 * 360);
    ++count;
    }
 if ( asc != house_cusps[1] ) {
    house_cusps[1] = asc;
    ++count;
    }
 return( count );
}
*/


/* function to manage composite processing. This function cross
   processes one set of figures against anothers. Midpoints are taken
   between two sets of data and the house cusps. */

void do_compos( AS_INF *you, AS_INF *partner, short *array, BITS mode, int year )
{
 AS_INF *ptr, *ptr2;
 COUNT i;
 long min;
 NUM maxp;

 if ( mode & ASTEROID )
	 maxp = CHIRON;
 if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = EAST_POINT;
 else if ( !(mode & NOBIRTHTIM) )
    maxp = PART_FORTU;
 else
    maxp = SOUTH_NODE;

 for ( i = SUN, ptr = you+1, ptr2 = partner+1; i <= maxp; ++i, ++ptr, ++ptr2 ) {
     if ( i == PART_FORTU ) {
	  if( !(mode & NOBIRTHTIM) ) {
	    min = (you + ASCEND )->minutes_total;
	    min += (you + MOON )->minutes_total;
	    min -= (you + SUN)->minutes_total;
	    if ( min < 0L )
	       min += d2m(360L);
	    min %= d2m(360L);
	    ptr->minutes_total = (short) min;
	    }
	  }
     else if ( i != MED_COELI && i != IM_COELI)
	   ptr->minutes_total = midpoint( ptr2->minutes_total, ptr->minutes_total );
     if ( ( i == SOUTH_NODE || i == IM_COELI) &&
		      ptr->minutes_total == (ptr-1)->minutes_total )
	   ptr->minutes_total = adjust_mins( ptr->minutes_total+(180 * 60) );
     }
 for ( i = ASCEND, ptr = you; i <= maxp; ++i, ++ptr )
     misc_calc( ptr, mode, year );
}


/* function to manage processing of compatibilities. Variables starting
   with the letter y refer to you. Variable refering to the letter p
   refers to your partner. 
   If NOBIRTHTIM bits in ymode and pmode are set in either one, they
   are set in BOTH. Then do_calc is called for the first set, the
   house_cusps are copied over to comp_house_cusps to save them for
   future use. Then the othr set is calculated. Then the two different
   sets compatibilities are calculated. */

void do_composite_calc( AS_DATA *ydata, AS_INF *yinf, AS_DATA *pdata, AS_INF *pinf,
	    BITS ymode, BITS pmode, NUM house_proc, DATA_PACKET *dpk )
{        
 BITS mask = ( 0xffff ^ ( BASE_MID | FULL_MID | MID_ASPECT | ASPECTS ) );
 short flag;
 DATA_PACKET xdp;
 TIM st;

 flag = ymode | ASPECTS;
 ymode &= mask;
 pmode &= mask;
 
 xdp = *dpk;
 if ( dpk->chart_code & RELOC_CH )
    xdp.chart_code ^= RELOC_CH;
 xdp.chart_code |= CSIDTM_X;
 do_calc( yinf, ydata, ymode, house_proc, &xdp );
 xdp.rdp = dpk->crdp;
 do_calc( pinf, pdata, pmode, house_proc, &xdp );
 compos_sidetime( pinf[MED_COELI].minutes_total, yinf[MED_COELI].minutes_total, &st );
 xdp = *dpk;
 xdp.chart_code |= XCSIDTM_;
 ydata->sidereal = st;
 do_calc( yinf, ydata, ymode, house_proc, &xdp );
 if ( errno == ENOMEM )
    return;
 xdp.rdp = dpk->crdp;
 pdata->sidereal = st;
 do_calc( pinf, pdata, pmode, house_proc, &xdp );
 do_compos( yinf, pinf, (short *)house_cusps, ymode, ydata->birth_date.year );
 if ( ymode & EXT_CALCS ) {
    do_extended_misc_calc( yinf, birth_ext, ymode );
    }
 if ( flag ) {
    process_aspects( yinf, yinf, ymode, COMPO_CH );
    }
 if ( pmode & SUMMARY ) {
    do_compos_sum_calc1( yinf, pinf, pmode );
    do_compos_sum_calc2( yinf, pinf, pmode );
    }
}
