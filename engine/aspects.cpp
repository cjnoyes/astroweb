#include "stdafx.h"
#include "flags.h"
#include "astro.h"
#include "aspect.h"
#include "useri.h"
#include "dataext.h"
#include "compute.fns"
#include <stdlib.h>
#include "errors.fns"

/* 09/10/92  Modified for C++ Windows */

/* routines for generating aspect information */

#define NUM_ASPECT 12
#define d2m(d) ((d)*60)
#define MAX_ASPECTS 30

int max_asp;
int high_asp;
short inc_hscsp, other_orb, hscsp_orb;

/* The Commented out Routines and Data are in the file ASPEC.ASM */

//int max_asp;
static int maxorb;
static ASP * aspptr;

FILE *teststream;

ASP aspect_def[NUM_ASPECT+1] = {
	 CONJUNCT,     0,         d2m(7),
	 SEXTILE,      d2m(60),   d2m(7),
	 TRINE,        d2m(120),  d2m(7),
	 OPPOSITION,   d2m(180),  d2m(7),
	 SQUARED,      d2m(90),   d2m(7),
	 SEMISEXT,     d2m(30),   d2m(2),
	 SEMISQUARE,   d2m(45),   d2m(2),
	 SEMISESQUAD,  d2m(135),  d2m(2),
	 QUINCUNX,     d2m(150),  d2m(2),
	 BIQUINTILE,   d2m(144),  d2m(2),
	 QUINTILE,     d2m(72),   d2m(2),
	 TREDECILE,    d2m(108),  d2m(2),
	 DECILE,       d2m(36),   d2m(2)
       /*  SEPTILE,      d2m(51),   d2m(2) */
	 };

char asp_map[] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* function to test aspect. Returns NO_ASPECT where there is no
   aspect, otherwise returns orb of aspect, mode defines max
   orb. If maxorb is 0 then orb is limited only by aspect_def,
   otherwise orb is +maxorb/-maxorb */


DEGR test_aspect( DEGR deg1, DEGR deg2 )
{
 DEGR dif, orb;

  dif = deg1 - deg2;
 if ( dif < 0 )
   dif = -dif;
 if ( dif > d2m(188) )
    dif = d2m(360) - dif;
 dif -= aspptr->degs;
 if ( dif < 0 )
    dif = -dif;
 if ( !maxorb ) 
    orb = aspptr->orb;
 else 
    orb = maxorb;
 if ( dif <= orb )
    return( dif );
 else
    return( NO_ASPECT );
}

/* function to do repeated calls to test_aspect */

DEGR find_aspect( DEGR deg1, DEGR deg2, NUM *aspect, int maxiorb )
{
 short i, orb;

 maxorb = maxiorb;
 i = 0;
 orb = NO_ASPECT;
 aspptr = aspect_def;
 while ( ( orb = test_aspect( deg1, deg2 ) ) == NO_ASPECT
       && i < max_asp ) {
	     ++aspptr;
             ++i;
             }  
 if ( orb == NO_ASPECT )
    *aspect = -1;
 else {
    *aspect = aspptr->aspect;
	//fprintf( teststream, "i=%d,asp=%d,d=%d,%d,o=%d,deg=%d,ao=%d\n",i,aspptr->aspect, deg1, deg2, orb, aspptr->degs, aspptr->orb );
 } 
return( orb );
}



/* procedure to do actual processing of aspects. You pass the planet
   pointer your processing along with the number of the planet, and
   the pointer to the array of planets use for comparison. Comparison
   starts with asptr+planet and continues to asptr+VERT_EAST, if mode
   is 0 then we have no birth time and we must skip certain birth-time
   dependant planets in comparison */

COUNT do_aspect( AS_INF *pla_ptr, NUM planet, AS_INF *asptr,
		   DEGR maxorb, BITS mode, NUM maxp, int startp )
{
 COUNT pno, aspects;
 NUM aspn = 99;
 DEGR orb = NO_ASPECT;
 AS_INF  *asp;
 ASPEC *as;
 extern DEGR house_cusps[];
 NUM mp = abs(maxp);
 
 //fprintf(teststream,"pla=%d,stp=%d\n",planet,startp);
 aspects = 0;
  if ( mode & EXT_ASPECTS )
    max_asp = 12;
 else
    max_asp = high_asp;
  for ( pno = startp, asp = asptr + startp; pno <= mp; ++pno, ++asp ) {
	  //fprintf(teststream,"pno=%d\n", pno );
	  if ( (!(mode & NOBIRTHTIM) || ( pno != ASCEND )) && pla_ptr->calced && asp->calced ) {
	orb = find_aspect( pla_ptr->minutes_total, asp->minutes_total,
		 &aspn, ( (pno != ASCEND && (pno <= PLUTO || (pno>= CUPIDO && pno<= CHIRON) )) ? maxorb : other_orb ) );

	if ( orb != NO_ASPECT && aspn >= 0 && aspn <= NUM_ASPECT ) {
	   if ( asp_map[aspn] || mode & EXT_ASPECTS ) {
              as = pla_ptr->aspectr+aspects;
	      as->aspect = aspn;
	      as->planet = pno;
	      as->orb = orb / 60;
	      ++aspects;
	      if ( ( planet == MED_COELI+1 && pno == IM_COELI )
		 || ( planet == NORTH_NODE+1 && pno == SOUTH_NODE ) )
		 --aspects;
	      if ( aspects == MAX_ASPECTS )
		 return( MAX_ASPECTS );
	      }
	  }
       }
     }
 if ( maxp <= 0 && inc_hscsp ) {
    for ( pno = HOUSE_1; pno <= HOUSE_12; pno++ ) {
	//fprintf(teststream,"pl=%d,pno=%d\n", planet, pno );
	orb = find_aspect( pla_ptr->minutes_total, house_cusps[pno - HOUSE_1+1],
	     &aspn, hscsp_orb );
	if ( orb != NO_ASPECT && ( asp_map[aspn] || mode & EXT_ASPECTS ) ) {
	   as = pla_ptr->aspectr+aspects;
	   as->aspect = aspn;
	   as->planet = pno;
	   //fprintf(teststream, "pn=%d\n", pno );
	   as->orb = orb / 60;
	   ++aspects;
	   if ((planet == ASCEND+1 && (pno == HOUSE_1 || pno == HOUSE_1+6)) ||
	       (planet == MED_COELI+1 && (pno == HOUSE_1+3 || pno == HOUSE_1+9)) ||
		   (planet == IM_COELI+1 && (pno == HOUSE_1+3 || pno == HOUSE_1+9))) {
	      --aspects;
		   //fprintf(teststream,"dec\n");
		   }
	   if ( aspects == MAX_ASPECTS )
	      return( MAX_ASPECTS );
	   }
	}
    }

 pla_ptr->no_aspects = aspects;
 return( aspects );
 }


/* function to process aspects. If aspectr is NULL then space is allocated
   for 15 aspects as starting point. Otherwise space pointed to is resized
   for 15 aspects. Then aspects are calculated. When total number of
   aspects is known, then space is resized to fit with realoc. This process
   is followed for all members of the array that main points to.
   If mode is set then we have no birthtime so we have to skip those
   planets which depend on birth time. */

void process_aspects( AS_INF  *planet, AS_INF  *sub, BITS mode, int chart_code )
{
 ASPEC *asp;
 AS_INF *pl;
 COUNT  pno, aspects;
 MINU   maxorb, xmaxorb;
 NUM    maxp, startp;
 BITS   hm = mode & HOUSES;

 maxorb = xmaxorb = 0;
 if ( mode & ASTEROID )
    maxp = CHIRON;
 else if ( mode & VERT_EAST )
    maxp = EAST_POINT;
 else
    maxp = IM_COELI;

 mode &= (NOBIRTHTIM|EXT_ASPECTS);
 
 //teststream = fopen("c:\\asp.log","wt");
 for ( pno = ASCEND, pl = planet; pno <= maxp; ++pl, ++pno ) {
	 if ( !pl->calced )
		 continue;
	 if ( !(mode & NOBIRTHTIM) || ( pno != ASCEND ) ) {
	asp = new ASPEC[30];
        if ( asp == NULL ) {
           errno = ENOMEM;
           pl->aspectr = NULL;
           pl->no_aspects = 0;   
           return;
           }   
        pl->aspectr = asp;
	if ( !xmaxorb ) {
           if ( pno == ASCEND || pno > CHIRON || (pno > PLUTO && pno < CUPIDO) )
	      maxorb = d2m(5);
           else
              maxorb = 0; 
           }
	else
	   maxorb = xmaxorb;
	if ( chart_code == TRANS_CH || chart_code == COMPA_CH || chart_code == SOLAR_CH || chart_code == HARMON_CH )
	   startp = 0;
	else
	   startp = pno+1;
	aspects = do_aspect( pl, pno+1, sub, maxorb, mode,
				   ( ( hm ) ? -maxp : maxp), startp );
	if ( !aspects ) {
	   delete asp;
	   pl->aspectr=NULL;
	   pl->no_aspects = 0;
	   }
	else if ( asp != NULL ) {
	   pl->aspectr =(ASPEC *) reallocx( asp, aspects * sizeof(ASPEC) );
       pl->no_aspects = aspects;  
	  }
    else {
        pl->aspectr = NULL;
        pl->no_aspects = 0;
        }
     }
   }
 //fclose( teststream );
}
