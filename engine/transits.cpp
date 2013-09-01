#include "stdafx.h"
#include "astro.h"
#include "flags.h"
#include "compute.fns"
#include "datetime.fns"
#include "dataext.h"
#include "errors.fns"
#include <string.h>

/* file to handle generating a group of transits */

AS_INF **trans_ptr = NULL; /* pointer to an array of pointers
                              num_transits wide, each pointing to 
			      an array of AS_INFs end_planet -
			      starting_planet wide
			      The first though is a full 18 AS_INFs wide */
DATES *date_ptr = NULL;    /* pointer to an array of transit dates,
                              width is num_transits */
                             
void do_a_transit( AS_INF near *binf, AS_DATA *tdata, AS_INF *tinf,
	       BITS mode, NUM house_mode, DATA_PACKET *dpk, int tno )
{
 AS_INF *ptr, *t;
 COUNT i;
 TRANS_DATA *tdp = dpk->tdp;
 NUM maxp;

 if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = EAST_POINT;
 else
    maxp = SOUTH_NODE;
 if ( maxp > tdp->end_planet )
    maxp = tdp->end_planet;
 do_calc( tinf, tdata, mode & ( 0xffff ^ ASPECTS ), house_mode, dpk );
 memcpy( house_cusps, alt_cusps, 13 * sizeof( MINU ) );
 for ( i = tdp->start_planet, ptr = tinf + tdp->start_planet; i <= maxp;
					     ++i, ++ptr ) {
	 if (!ptr->calced )
		 return;
     if ( !(mode & NOBIRTHTIM) || ( i != ASCEND ) ) {
	ptr->house = house_number( ptr->minutes_total );
	ptr->house_cusp = house_cusp_intercept( ptr->minutes_total,
	      ptr->house );
	ptr->intercepted = is_sign_intercept( sign( ptr->minutes_total ) );
        }
     }
 if ( mode & ASPECTS ) {
    process_aspects( tinf, binf, mode & NOBIRTHTIM, TRANS_CH );
    if ( tno > 1 ) {
       for ( i = 0, t = tinf; i < tdp->start_planet; i++, t++ ) {
	   if ( t->aspectr != NULL )
	      delete t->aspectr;
	   t->no_aspects = 0;
	   t->aspectr = NULL;
	   }
       for ( i = tdp->end_planet+1, t = tinf+i; i <= maxp; i++, t++ ) {
	      if ( !t->calced )
			  continue;
		  if ( t->aspectr != NULL )
	        delete t->aspectr;
	   t->no_aspects = 0;
	   t->aspectr = NULL;
	   }
       }
    }
}

void date_just( AS_DATA *tdata, DATES *newd, NUM days, NUM months )
{
 tdata->birth_date.day += days;
 tdata->birth_date.month += months;
 adj_date( &tdata->birth_date );
 memcpy( newd, &tdata->birth_date, sizeof(DATES) );
}

void manage_transits( AS_INF *binf, AS_INF *tinf, AS_DATA *bdata,
    AS_DATA *tdata, BITS bmode, BITS tmode, NUM house_mode, DATA_PACKET *dp )
{
 short save[13], save2[13];
 COUNT num, x;
 BITS mask = ( 0xffff ^ ( BASE_MID | FULL_MID | MID_ASPECT | SUMMARY ) );

 AS_INF **ptr, *iptr;
 DATES tdate, *dptr;

 memcpy( &tdate, &tdata->birth_date, sizeof(DATES) );
 bmode &= mask;
 tmode &= mask;
 
 if ( bmode & NOBIRTHTIM )
    tmode |= NOBIRTHTIM;
 else if ( tmode & NOBIRTHTIM )
    bmode |= NOBIRTHTIM;
 trans_ptr = new AS_INF *[dp->tdp->num_transits+1];
 date_ptr = new DATES[dp->tdp->num_transits+1];
 if ( date_ptr == NULL || trans_ptr == NULL ) { /* not enought memory */
    if ( date_ptr != NULL ) {
       delete date_ptr;
       date_ptr = NULL;
       }
    if ( trans_ptr != NULL ) {
       delete trans_ptr;
       trans_ptr = NULL;
       }
    errno = ENOMEM;
    return;
    }
 memset( (void *)trans_ptr, 0, (dp->tdp->num_transits+1) * sizeof(struct AS_INF *) );
 if ( !(dp->chart_code & COMPO_CH) ) {
    do_calc( binf, bdata, bmode &( 0xffff ^ ASPECTS ), house_mode, dp );
    memcpy( alt_cusps, house_cusps, 13 * sizeof( MINU ) );
    }
 else {
    memcpy( save, house_cusps, 13 * sizeof( MINU ) );
    memcpy( save2, alt_cusps, 13 * sizeof( MINU ) );
    }
 num = (dp->tdp->end_planet - dp->tdp->start_planet)+2;
 for ( x = 1, ptr = trans_ptr, dptr = date_ptr; x <= dp->tdp->num_transits;
						 x++, ptr++, dptr++ ) {
     if ( x == 1 )
	iptr = new AS_INF[SOUTH_NODE-SUN+1];
     else
	iptr = new AS_INF[num+2];
     if ( iptr == NULL ) {
        errno = ENOMEM;
        return;
        }
     if ( x == 1 )
	memset( iptr, 0, ((SOUTH_NODE-SUN+1) * sizeof( AS_INF )));
     else
	memset( iptr, 0, ((num+2) * sizeof( AS_INF )));
     *ptr = iptr;
     if ( x == 1 )
	memcpy( dptr, &tdata->birth_date, sizeof(DATES) );
     else
	date_just( tdata, dptr, dp->tdp->interval_date.day, dp->tdp->interval_date.month );
     do_a_transit( binf, tdata, tinf, tmode, house_mode, dp, x );
     if ( errno == ENOMEM )
        return; 
     if ( x == 1 )
	memcpy( iptr, tinf+SUN, (SOUTH_NODE-SUN+1) * sizeof( AS_INF ) );
     else
	memcpy( iptr, tinf+dp->tdp->start_planet, (num+2) * sizeof( AS_INF ) );
     }
 if ( !(dp->chart_code & COMPO_CH) ) {
    memcpy( save, house_cusps, 13 * sizeof( MINU ) );
    memcpy( house_cusps, alt_cusps, 13 * sizeof( MINU ) );
    memcpy( alt_cusps, save, 13 * sizeof( MINU ) );
    }
 else {
    memcpy( house_cusps, save, 13 * sizeof( MINU ) );
    memcpy( alt_cusps, save2, 13 * sizeof( MINU ) );
    }
 memcpy( &tdata->birth_date, &tdate, sizeof(DATES) );
 }
