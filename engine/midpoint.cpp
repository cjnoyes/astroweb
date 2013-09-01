#include "stdafx.h"
#include "astro.h"
#include "aspect.h"
#include "flags.h"
#include "errors.fns"
#include "compute.fns"
#include <string.h>
/* 09/10/92  Modified for C++ Windows */

#define d2m(d) ((d)*60)
#define MAX_ASPECTS 50

/* These Routines are found in MIDPT.ASM */

MINU midpoint( MINU min1, MINU min2 )
{
 MINU max, min, dif;

 if ( min1 > min2 ) {
    max = min1;
    min = min2;
    }
 else {
    max = min2;
    min = min1;
    }
 dif = max - min;
 dif /= 2;
 return( min + dif );
}


/* procedure to do actual processing of aspects. You pass the planet
   pointer your processing along with the number of the planet, and
   the pointer to the array of planets use for comparison. Comparison
   starts with asptr+planet and continues to asptr+SOUTH_NODE. 
      If aspectr is NULL then space is allocated for 40 aspects as
   starting point. Otherwise space pointed to is resized for 40 aspects.
   Then aspects are calculated. When total number of aspects is known, 
   then space is resized to fit with realoc. */

#define NO_ASPECT -999

void do_mid_aspect( MIDPT *mid_ptr, NUM planet, AS_INF *asptr,
                       NUM maxp, BITS mode, int same )
{
 COUNT pno, aspects;
 NUM aspn;
 MINU orb;
 ASPEC *asp;
 AS_INF *as;
 extern MINU house_cusps[];

 aspects = 0;
 max_asp = 50;
 asp = new ASPEC[51];
 if ( asp == NULL ) {
    errno = ENOMEM;
    mid_ptr->no_aspect = 0;
    mid_ptr->aspectr = NULL;
    return;
    }
 mid_ptr->aspectr = asp;
 aspects = 0;
 for ( pno = ASCEND, as = asptr; pno <= maxp; ++pno, ++as ) {

     if ( !(same && pno == planet) ) {
	orb = find_aspect( mid_ptr->total_minutes, as->minutes_total,
			   &aspn, d2m(2)+30 );
	if ( orb != NO_ASPECT ) {
	   asp->aspect = aspn;
	   asp->planet = pno;
	   asp->orb = orb / 60;
	   ++aspects;
	   ++asp;
	   }
	if ( aspects == MAX_ASPECTS )
	   break;
	}
     }

 mid_ptr->no_aspect = aspects;
 mid_ptr->aspectr = (struct ASPEC *) reallocx( mid_ptr->aspectr,
         aspects * sizeof(ASPEC) );
}

void do_alt_mid_aspect( MIDPT *mid_ptr, AS_INF *asptr,
                       NUM maxp )
{
 COUNT pno, aspects;
 NUM aspn;
 MINU orb;
 ASPEC *asp;
 AS_INF *as;
 extern MINU house_cusps[];

 aspects = 0;
 max_asp = 50;
 asp = new ASPEC[51];
 if ( asp == NULL ) {
    errno = ENOMEM;
    mid_ptr->no_alt_aspect = 0;
    mid_ptr->altaspectr = NULL;
    return;
    }
 mid_ptr->altaspectr = asp;
 aspects = 0;
 for ( pno = ASCEND, as = asptr; pno <= maxp; ++pno, ++as ) {
	orb = find_aspect( mid_ptr->total_minutes, as->minutes_total,
			   &aspn, d2m(1)+30 );
	if ( orb != NO_ASPECT ) {
	   asp->aspect = aspn;
	   asp->planet = pno;
	   asp->orb = orb / 60;
	   ++aspects;
	   ++asp;
	   }
	if ( aspects == MAX_ASPECTS )
	   break;
	}
 mid_ptr->no_alt_aspect = aspects;
 mid_ptr->altaspectr = (struct ASPEC *) reallocx( mid_ptr->altaspectr,
         aspects * sizeof(ASPEC) );
}


/* function to calculate minor degrees and sign. for midpoint Called 
   with pointer. Also tests house, cusps house and sign, house cusp
   intercepts, and minor degrees */ 



void mid_misc_calc( MIDPT *ptr )
{
 ptr->sign = sign( ptr->total_minutes );
 ptr->minor_degrees = minor_degrees( ptr->total_minutes );
 ptr->minor_minutes = minor_minutes( ptr->total_minutes );
 ptr->cusp = sign_cusp( ptr->minor_degrees, ptr->minor_minutes, ptr->sign );
 ptr->house = house_number( ptr->total_minutes );
 ptr->intercepted = house_cusp_intercept( ptr->total_minutes, ptr->house );
 ptr->no_aspect = 0;
 ptr->aspectr = NULL;
}



/* function to do basic midpoint checking ASCEND & MIDHEAVEN, and
   NORTH NODE & SOUTH NODE, and IM COELI and MED COELI. If 
   mode & MID_ASPECT the aspects are found. 
 */

void do_basic_midpts( AS_INF *chart, BITS mode )
{
 MINU mid;
 MIDPT *asc, *node, *coeli;
 AS_INF *ptr;
 NUM maxp;

 if ( mode & NOBIRTHTIM )
    return;
 if ( mode & ASTEROID )
	 maxp = CHIRON;
 else if ( mode & VERT_EAST )
    maxp = VERTEX;
 else
    maxp = PART_FORTU;
 if ( mode & HOUSES )
    maxp = -maxp;
 asc = new MIDPT [2];
 node = new MIDPT [2];
 coeli = new MIDPT [2];
 if ( asc == NULL || node == NULL || coeli == NULL ) {
    errno = ENOMEM;
    chart->no_midpt = 0;
    chart->midptr = NULL;
    (chart + IM_COELI)->no_midpt = 0;
    (chart + IM_COELI)->midptr = NULL;
    (chart + NORTH_NODE)->no_midpt = 0;
    (chart + NORTH_NODE)->midptr = NULL;
    if ( asc != NULL )
       delete asc;
    if ( node != NULL )
       delete node;
    if ( coeli != NULL )
       delete coeli;
    return;
    }
 ptr = chart;
 ptr->no_midpt = 2;
 ptr->midptr = asc;
 mid = midpoint( ptr->minutes_total, ( chart + MED_COELI )->minutes_total );
 asc->second_planet = MED_COELI;
 (asc+1)->second_planet = MED_COELI;
 asc->total_minutes = mid;
 (asc+1)->total_minutes = adjust_mins( mid + d2m(180) );
 mid_misc_calc( asc );
 mid_misc_calc( asc+1 );
 if ( mode & MID_ASPECT ) {
    do_mid_aspect( asc, ASCEND, chart, maxp, mode & NOBIRTHTIM, 1 );
    do_mid_aspect( asc+1, ASCEND, chart, maxp, mode & NOBIRTHTIM, 1 );
    }
 ptr = chart + NORTH_NODE;
 ptr->no_midpt = 2;
 ptr->midptr = node;
 mid = midpoint( ptr->minutes_total, ( chart + SOUTH_NODE )->minutes_total );
 node->second_planet = SOUTH_NODE;
 (node+1)->second_planet = SOUTH_NODE;
 node->total_minutes = mid;
 (node+1)->total_minutes = adjust_mins( mid + d2m(180) );
 mid_misc_calc( node );
 mid_misc_calc( node+1 );
 if ( mode & MID_ASPECT ) {
    do_mid_aspect( node, NORTH_NODE, chart, maxp, mode & NOBIRTHTIM, 1 );
    do_mid_aspect( node, NORTH_NODE, chart, maxp, mode & NOBIRTHTIM, 1 );
    }
 ptr = chart + MED_COELI;
 ptr->no_midpt = 2;
 ptr->midptr = coeli;
 mid = midpoint( ptr->minutes_total, ( chart + IM_COELI )->minutes_total );
 coeli->second_planet = IM_COELI;
 (coeli+1)->second_planet = IM_COELI;
 coeli->total_minutes = mid;
 (coeli+1)->total_minutes = adjust_mins( mid + d2m(180) );
 mid_misc_calc( coeli );
 mid_misc_calc( coeli+1 );                             
 if ( mode & MID_ASPECT ) {
    do_mid_aspect( coeli, MED_COELI, chart, maxp, mode & NOBIRTHTIM, 1 );
    do_mid_aspect( coeli, MED_COELI, chart, maxp, mode & NOBIRTHTIM, 1 );
    }
}


/* function to do basic full midpoint processing for one pair of
   midpoints */

void do_midpoint( MIDPT *ptr, AS_INF *chart1, AS_INF *chart2,
    NUM pno, BITS mode )
{
 MINU mid;
 
 mid = midpoint( chart1->minutes_total, ( chart2 + pno )->minutes_total );
 ptr->total_minutes = mid;
 ptr->second_planet = pno;
 mid_misc_calc( ptr );
 if ( !( mode & HALF_MID ) ) {
    ptr++;
    ptr->total_minutes = adjust_mins( mid + d2m(180) );
    ptr->second_planet = pno;
    mid_misc_calc( ptr );
    }
}

/* function to do a whole block of midpoints. If mode & NOBIRTHTIM
   then aspects are skipped, also midpoints on birth time dependent
   planets ascendant, med coeli, im coeli, and part of fortune are
   skipped
*/

void do_full_midpoints( AS_INF *chart1, AS_INF *chart2, BITS mode )
{
 COUNT no, planet, numpt;
 MIDPT *ptr;
 AS_INF *inf;
 NUM maxp;
 BITS amode = mode;

 if ( mode & ASTEROID )
	 maxp = CHIRON;
 else if ( mode & VERT_EAST )
    maxp = EAST_POINT;
 else
    maxp = PART_FORTU;

 mode &= NOBIRTHTIM;
 for ( planet = ASCEND, inf = chart1;
			   planet < maxp; ++planet, ++inf ) {
     if ( !mode ||  ( planet != ASCEND ) ) {
	  numpt = ( maxp - planet );
	  if ( !(amode & HALF_MID ) )
	     numpt += numpt;
	  ptr = new MIDPT[numpt];
          if ( ptr == NULL && numpt != 0 ) {
	     inf->midptr = NULL;
             inf->no_midpt = 0; 
             errno = ENOMEM;
             return;
             }  
	  memset( ptr, 0, numpt * sizeof( MIDPT ) );
	  inf->no_midpt = numpt;
          inf->midptr = ptr;
	  for ( no = planet+1; no <= maxp; no++, ptr++ ) {
              if ( !mode || ( no != ASCEND ) ) { 
		 do_midpoint( ptr, inf, chart2, no, amode );
		 if ( amode & MID_ASPECT )
		    do_mid_aspect( ptr, planet, chart2, maxp, mode, 1 );
		 if ( !( amode & HALF_MID ) ) {
		    ptr++;
		    ptr->aspectr = NULL;
		    ptr->no_aspect = 0;
		    if ( amode & MID_ASPECT )
		       do_mid_aspect( ptr, planet, chart2, maxp, mode, 1 );
		    }
		 if ( errno == ENOMEM )
		    return;
		 }
              else {
                   ptr->aspectr = NULL;
                   ptr->no_aspect = 0;
		   ptr->total_minutes = -1;
		   if ( !( amode & HALF_MID ) ) {
		      (ptr+1)->aspectr = NULL;
		      (ptr+1)->total_minutes = -1;
		      (ptr+1)->no_aspect = 0;
		      ptr++;
		      }
                   }     
              }
        }
     else {
	  inf->midptr = NULL;
          inf->no_midpt = 0;
          }
    }
}

void calc_midpoints( AS_INF *main, AS_INF *alt, int pno, int maxp1, int maxp2, BITS mode, short &nomid, MIDPT **midptr, int same )
{
 int mc;
 if ( same )
    mc = maxp2-pno;
 else 
    mc = maxp2; 
 if ( same ) 
	 --mc;
 if ( !(mode &HALF_MID ) && same )
	 mc += maxp2-pno;
 else if (!(mode &HALF_MID ))
	 mc+= maxp2;
 if ( same )
	 --mc;
 nomid = mc;
 *midptr = new MIDPT[mc+1];
 memset(*midptr,0,sizeof(MIDPT)*(mc+1));
 AS_INF *plntp = main+pno, *inf_ptr;
 MIDPT *mid;
 int i, j; 

 int st;
 if ( same)
	 st = pno+1;
 else
	 st = 0; 
 for ( i =st , mid = *midptr, inf_ptr=alt; i < maxp2; i++, mid++, inf_ptr++ ) {
     if ( same && i == pno )
		 continue;
     do_midpoint( mid, plntp, alt, i, mode );
	 if ( mode & MID_ASPECT ) {
	    do_mid_aspect( mid, pno, main, maxp1,  mode, same  );
	    if ( !same )
			do_alt_mid_aspect( mid, alt, maxp2 );
		if ( !(mode & HALF_MID) && MID_ASPECT ) {
		   do_mid_aspect( mid+1, pno, main, maxp1,  mode, same );
	       if ( !same )
			  do_alt_mid_aspect( mid+1, alt, maxp2 );

		   }
	    }
	 if ( !(mode & HALF_MID) )
		 ++mid;
	 }

 }

void do_main_full_midpoints( AS_INF *main, NUM maxp1, AS_INF *alt, NUM maxp2, BITS mode, int same )
{
 AS_INF *infptr;
 int i;
 
 for ( i = 0, infptr = main; i < maxp1; i++, infptr++ ) 
	 calc_midpoints( main, alt, i, maxp1, maxp2, mode, infptr->no_midpt, &infptr->midptr, same );
}
