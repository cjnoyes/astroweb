#include "stdafx.h"
#include "astro.h"
#include "misccalc.h"
#include "flags.h"
#include "compute.fns"
#include <memory.h>
#include "dataext.h"

HEM   comp_hemispheres;      /* summary calculation variables */
GEN   comp_genders;          /* types defined in misccalc.h */
ELE   comp_elements;
QUAL  comp_qualities;
MDL   comp_modals;
FUNL  comp_functionals;
COUNT comp_aspect_count[15];
NUM   comp_scout_planet;
COUNT comp_sign_count[12];
COUNT comp_house_count[13];

/* function to manage summary calculations for both the individual and
   their partner's charts. mode indicates fields to be totaled */

void do_compat_sum_calc( AS_INF *info, AS_INF *partner, BITS mode )
{
 count_genders( info, &genders );
 count_elements( info, &elements );
 count_qualities( info, &qualities );
 count_signs( info, sign_count );
 if ( mode & HOUSES ) {
    count_hemispheres( info, &hemispheres );
    count_modals( info, &modals );
    count_functional( info, &functionals );
    scout_planet = find_scout_planet( info );
    count_houses( info, house_count );
 }
 if ( mode & ASPECTS )
    count_aspects( info, aspect_count );
 count_genders( partner, &comp_genders );
 count_elements( partner, &comp_elements );
 count_qualities( partner, &comp_qualities );
 count_signs( partner, comp_sign_count );
 if ( mode & HOUSES ) {
    count_hemispheres( partner, &comp_hemispheres );
    count_modals( partner, &comp_modals);
    count_functional( partner, &comp_functionals );
    comp_scout_planet = find_scout_planet( partner );
    count_houses( partner, comp_house_count );
 }
 if ( mode & ASPECTS )
    count_aspects( partner, comp_aspect_count );
}

/* function to manage compatibility processing. This function cross 
   processes one set of figures angainst anothers. House cusps are
   checked out, then aspects are checked out */


void do_compatab( AS_INF *you, AS_INF *partner, BITS mode )
{
 AS_INF *ptr, *ptr2;
 COUNT i;
 NUM maxp;

 if ( mode & ASTEROID )
	 maxp = CHIRON;
 else if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = EAST_POINT;
 else if ( !(mode & NOBIRTHTIM) )
    maxp = PART_FORTU;
 else
    maxp = SOUTH_NODE;

 for ( i = ASCEND, ptr = you, ptr2 = partner; i <= maxp; ++i, ++ptr, ++ptr2 ) {
     if ( !(mode & NOBIRTHTIM) ) {
	ptr2->house = house_number( ptr2->minutes_total );
	ptr2->house_cusp = house_cusp_intercept( ptr2->minutes_total, ptr2->house );
        }
     }
 if ( mode & ASPECTS )
    process_aspects( you, partner, mode & (BITS) ( NOBIRTHTIM | VERT_EAST ), COMPA_CH );

}

/* function to manage processing of compatibilities. Variables starting
   with the letter y refer to you. Variable refering to the letter p
   refers to your partner. 
   If NOBIRTHTIM bits in ymode and pmode are set in either one, they
   are set in BOTH. Then do_calc is called for the first set, the
   house_cusps are copied over to comp_house_cusps to save them for
   future use. Then the othr set is calculated. Then the two different
   sets compatibilities are calculated. */

void do_comp_calc( AS_DATA *ydata, AS_INF *yinf, AS_DATA *pdata, AS_INF *pinf,
	    BITS ymode, BITS pmode, NUM house_proc, DATA_PACKET *dpk )
{        
 BITS mask = ( 0xffff ^ ( BASE_MID | FULL_MID | MID_ASPECT ) );
 short array[13];
 DATA_PACKET xdp;
 BITS xymode = ymode, xpmode = pmode;
 ymode &= mask;
 pmode &= mask;
 
 if ( ymode & NOBIRTHTIM )
    pmode |= NOBIRTHTIM;
 else if ( pmode & NOBIRTHTIM )
    ymode |= NOBIRTHTIM;
 if ( ymode & NOBIRTHTIM )
    xpmode |= NOBIRTHTIM;
 else if ( pmode & NOBIRTHTIM )
    xymode |= NOBIRTHTIM;
 memset(yinf,0,(CHIRON * sizeof(AS_INF)));
 memset(pinf,0,(CHIRON * sizeof(AS_INF)));
 do_calc( yinf, ydata, ymode, house_proc, dpk );
 if ( errno == ENOMEM )
    return;
 memcpy( array, house_cusps, sizeof(MINU) * 13 );
 xdp = *dpk;
 xdp.rdp = dpk->crdp;
 do_calc( pinf, pdata, pmode, house_proc, &xdp );
 memcpy( alt_cusps, house_cusps, sizeof(MINU) * 13 );
 memcpy( house_cusps, array, sizeof(MINU) * 13 );
 if ( errno == ENOMEM )
    return;
 if ( pmode & SUMMARY )
    do_compat_sum_calc( yinf, pinf, pmode );
 do_compatab( yinf, pinf, pmode );
 int mp = SOUTH_NODE;
 if ( (xpmode & ASTEROID) && !(xpmode & NOBIRTHTIM) )
	 mp = CHIRON;
 else if ( (xpmode & VERT_EAST) && !(xpmode & NOBIRTHTIM))
     mp = EAST_POINT;
#ifdef DELUXE
 if ( xpmode & (FULL_MID|HALF_MID) ) {
    do_main_full_midpoints( yinf, mp, pinf, mp, xpmode, 0 );
 }
#endif
 if ( errno == ENOMEM )
    return;
 memcpy( house_cusps, alt_cusps, sizeof(MINU) * 13 );
 do_compatab( pinf, yinf, pmode );
 memcpy( house_cusps, array, sizeof(MINU) * 13 );
}
