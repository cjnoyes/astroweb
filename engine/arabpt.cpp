#include "stdafx.h"
#include <stdlib.h>
#include <memory.h>
#include "astro.h"
#include "arabpt.h"
#include "compute.fns"
#include "errors.fns"
#include "flags.h"
#include "utl.h"
#include "aspect.h"
#include "charcons.h"

/* 09/10/92  Modified for C++ Windows */

extern int report_table;

#define MAX_ASPECTS 15

ARAB_PART arabic_data[N_ARAB_PART] = {
  ASCEND, MERCURY, SUN,    ASCEND, VENUS, SUN,    ASCEND, MARS, SUN,
  ASCEND, JUPITER, SUN,    ASCEND, SATURN, SUN,   ASCEND, URANUS, SUN,
  ASCEND, NEPTUNE, SUN,    ASCEND, PLUTO, SUN,    ASCEND, SUN, MOON,
  ASCEND, MERCURY, MOON,   ASCEND, VENUS, MOON,   ASCEND, JUPITER, MOON,
  ASCEND, SUN, MOON,       ASCEND, MOON, MERCURY, ASCEND, MARS, MERCURY,
  ASCEND, MOON, VENUS,     ASCEND, SATURN, VENUS, ASCEND, VENUS, MARS,
  ASCEND, JUPITER, MARS,   ASCEND, MOON, SATURN,  ASCEND, JUPITER, SATURN,
  ASCEND, PART_FORTU, SATURN,                     ASCEND, MOON, MERCURY,
  ASCEND, MOON, URANUS,                           ASCEND, PART_FORTU, NEPTUNE };

char *arabic_names[N_ARAB_PART] = {
  "Comemerce (Caduceus)", "Love (Heart)", "Passion (Sword)",
  "Increase (Pomegranate)", "Fate or Part of Responsibility (Hourglass)",
  "Catastrophe (Lightning Flash)", "Treachery (Trident)",
  "Organization (Gavel)", "Spirit", "Faith", "Female Children",
  "Male Children", "Servants", "Understanding", "the Mother",
  "Fortune of Husbandry", "Drama (Plays)", "Discord", "Inheritance",
  "the Father, Part of Profession", "Sickness", "Brothers and Sisters (Brethren)",
  "Sudden Advancement", "Friends", "Imprisonment (or Confinement) and Sorrow" };


AS_INF *arabic_parts = NULL;
int arabic_count = 0;

AS_INF *getArabicParts(void)
{
 return( (AS_INF *)arabic_parts );
}

int getArabicCount()
{
 return( arabic_count );
}

void close_arabic_pts(void)
{
 int i;
 AS_INF *p;
 
 if ( arabic_parts == NULL )
    return;
 for ( i = 0, p = arabic_parts; i < arabic_count; i++, p++ ) {
     if ( p->aspectr != NULL )
	delete p->aspectr;
     }
 delete arabic_parts;
 arabic_parts = NULL;
 arabic_count = 0;
}

const char *arabic_part_str( int a )
{
 if ( a < 0 || a >= N_ARAB_PART )
    return( "ERROR!!" );
  else {
    return( arabic_names[a] );
    }
}


void arabic_misc_calc( AS_INF *ptr, BITS code, short year )
{

 if ( code & SIDEREAL )
    ptr->minutes_total = tropical_2_sidereal( ptr->minutes_total, year );
 ptr->sign = sign( ptr->minutes_total );
 ptr->degree_minor = minor_degrees( ptr->minutes_total );
 ptr->minutes_minor = minor_minutes( ptr->minutes_total );
 ptr->cusp = sign_cusp( ptr->degree_minor, ptr->minutes_minor, ptr->sign );
 if ( code & MISCELL )
    ptr->misc_code = decanates( ptr->degree_minor ) +1;
 if ( code & HOUSES ) {
    ptr->house = house_number( ptr->minutes_total );
    ptr->house_cusp = house_cusp_intercept( ptr->minutes_total, ptr->house );
    }
 if ( code & MISCELL ) {
    if ( ptr->degree_minor > 28 )
       ptr->misc_code |= ANARET_DEG;
    if ( critical_degree( ptr->degree_minor, ptr->sign ) )
       ptr->misc_code |= CRITICAL;
    }
 ptr->calced = 1;
}


/* procedure to do actual processing of aspects. You pass the planet
   pointer your processing along with the number of the planet, and
   the pointer to the array of planets use for comparison. Comparison
   starts with asptr+planet and continues to asptr+VERT_EAST, if mode
   is 0 then we have no birth time and we must skip certain birth-time
   dependant planets in comparison */

COUNT do_arab_aspect( AS_INF near  *arab_ptr, AS_INF near *asptr, DEGR maxorb,
							BITS mode, NUM maxp )
{
 COUNT pno, aspects;
 NUM aspn=0;
 DEGR orb;
 AS_INF *asp;
 ASPEC *ar_as, *as = NULL;
 NUM mp = maxp;
 int maxplan;
 if ( mode & ASTEROID )
	 maxplan = CHIRON;
 else if ( mode & VERT_EAST )
	 maxplan = EAST_POINT;
 else 
	 maxplan = IM_COELI;

 ar_as = arab_ptr->aspectr;
 aspects = 0;
 aspects = 0;
  if ( mode & EXT_ASPECTS )
    max_asp = 13;
 else
    max_asp = high_asp;
 for ( pno = 0, asp = asptr; pno <= mp; ++pno, ++asp ) {
     orb = find_aspect( arab_ptr->minutes_total, asp->minutes_total,
	       &aspn, ( (pno != ASCEND && pno <= maxplan) ? maxorb : d2m(5) ) );
     if ( orb != NO_ASPECT && aspn >= 0 && aspn <= max_asp ) {
	   if ( asp_map[aspn] || mode & EXT_ASPECTS ) {
	   as = ar_as+aspects;
	   as->aspect = aspn;
	   as->planet = pno;
	   as->orb = orb / 60;
	   ++aspects;
	   if ( aspects == MAX_ASPECTS )
	      return( MAX_ASPECTS );
       }
     }
   }
 arab_ptr->no_aspects = aspects;
 return( aspects );
 }


int do_arabic_part( int i, AS_INF near *arab, unsigned short mode, int year  )
{
 long l;
 int aspects, maxp;
 ASPEC *asp;

 if ( mode & ASTEROID )
	 maxp = CHIRON;
 else if ( mode & VERT_EAST )
    maxp = VERTEX;
 else
    maxp = PART_FORTU;
 l = (long) birth_inf[arabic_data[i].base].minutes_total;
 l += (long) birth_inf[arabic_data[i].plus].minutes_total;
 l += (long) birth_inf[arabic_data[i].minus].minutes_total;
 if ( l < 0L )
    l += 21600L;
 else if ( l > 21600L )
    l -= 21600L;
 if ( l > 21600L )
    l -= 21600L;
 arab->minutes_total = (int) l;
 misc_calc( arab, mode, year );
 if ( mode & ASPECTS ) {
    asp = new ASPEC[30];
    if ( asp == NULL ) {
       errno = ENOMEM;
       arab->aspectr = NULL;
       arab->no_aspects = 0;
       return( -1 );
       }
    memset( asp, 0, (30) * sizeof(ASPEC));
    arab->aspectr = asp;
    aspects = do_arab_aspect( arab, (AS_INF near *)birth_inf, 0, mode, maxp );
    arab->aspectr = (ASPEC *)reallocx( asp, aspects * sizeof(ASPEC) );
    }
 else {
    arab->aspectr = NULL;
    arab->no_aspects = 0;
    }
 return( 0 );
}


void do_arabic_parts(unsigned short mode, int year )
{
 int i;
 AS_INF *arab;

 arabic_parts = (AS_INF *) new AS_INF[N_ARAB_PART];
 if (  arabic_parts == NULL ) {
    errno = ENOMEM;
    return;
    }
 for ( i = 0, arab = arabic_parts; i < N_ARAB_PART; ++i, ++arab ) {
     do_arabic_part( i, arab, mode, year );
     if ( errno == ENOMEM ) {
	arabic_count = i-1;
	return;
	}
     }
  arabic_count = N_ARAB_PART;
  if ( mode & (FULL_MID|HALF_MID)) {
     int mp = SOUTH_NODE;
	 if ( mode & ASTEROID )
		 mp = CHIRON;
	 else if ( mode & VERT_EAST )
		 mp = EAST_POINT;
	 do_main_full_midpoints(  arabic_parts, arabic_count, birth_inf, mp, mode, 0 );
     }
}
