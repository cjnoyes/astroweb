#include "stdafx.h"
#include "astro.h"
#include "misccalc.h"
#include "flags.h"
#include "compute.fns"
#include "aspect.h"
#include <memory.h>
#include <string.h>
/* functions to do miscellaneous summary calculations */

#define d2m(d) (d)*60

HEM   hemispheres;
GEN   genders;
ELE   elements;
QUAL  qualities;
MDL   modals;
FUNL  functionals;
COUNT aspect_count[15];
NUM   scout_planet;
COUNT sign_count[12];
COUNT house_count[13];
int maxplan;

/* function to count number of feminine signs odd signs are feminine */

void count_genders( AS_INF *info, struct GEN *gp )
{
 COUNT i, f, m;
 AS_INF *pt;

 for ( i = SUN, f = 0, m = 0, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
	if ( pt->sign & 1 )
           ++f;
        else
           ++m;
     }
 gp->masculine = m;
 gp->feminine = f;
}

/* function to count elements fire, earth, water, and air. */

void count_elements( AS_INF *info, struct ELE *elemp )
{
 COUNT i, f, e, w, a;
 AS_INF *pt;
 register int val;


 f = e = w = a = 0;    /* initialize counters;  */

 for ( i = SUN, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
	val = pt->sign;
        switch( val )  {
	      case ARIES :
	      case LEO :
	      case SAGITARIUS :
		   ++f;
                   break;
	      case TAURUS :
	      case VIRGO :
	      case CAPRICORN :
		   ++e;
                   break;
	      case GEMINI :
	      case LIBRA :
	      case AQUARIUS :
		   ++a;
                   break;
	      case CANCER :
	      case SCORPIO :
	      case PISCES :
		   ++w;
                   break;
                   }
     }
 elemp->fire = f;
 elemp->earth = e;
 elemp->air = a;
 elemp->water = w;
}          

/* function to count qualities of signs. Cardinal = 0, fixed = 1, 
   mutable = 2. Add 3 to get each group */


void count_qualities( AS_INF *info, struct QUAL *qualp )
{ 
 COUNT i, c, f, m;
 AS_INF *pt;
 register int val;


 c = f = m = 0; /* initialize counters */
 for ( i = SUN, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
     val = pt->sign;
     switch( val ) {
	   case ARIES :
	   case LIBRA :
	   case CANCER :
	   case CAPRICORN :
		++c;
		break;
	   case TAURUS :
	   case LEO :
	   case SCORPIO :
	   case AQUARIUS :
		++f;
		break;
	   case GEMINI :
	   case VIRGO :
	   case SAGITARIUS :
	   case PISCES :
		++m;
		break;
	   }
     }
 qualp->cardinal = c;
 qualp->fixed = f;
 qualp->_mutable = m;
}

/* function to count hemispheres houses are in. house 4-9 are in east,
   10 - 3 are in west, 1-6 are in north and 7-12 are in the south 
   works by breaking houses up into quadrants and adding them */

void count_hemispheres( AS_INF *info, struct HEM *hemp )
{
 COUNT i, n, e, s, w;
 AS_INF *pt;
 register int quad;

 n = e = w = s = 0;

 for ( i = SUN, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
     quad = pt->house ;
     if ( quad < 7 )
	++n;
     else
	++s;
     if ( quad < 4 || quad > 9 )
	++e;
     else
	++w;
     }
 hemp->north = n;
 hemp->south = s;
 hemp->east = e;
 hemp->west = w;
}

/* function to functional houses individual, relative, temporal, terminal */


void count_functional( AS_INF *info, struct FUNL *fnc )
{
 COUNT i, in, re, tm, tr;
 AS_INF *pt;
 NUM val;

 in = re = tm = tr = 0;    /* initialize counters;  */

 for ( i = SUN, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
     val = pt->house;
     switch( val )  {
	   case  1 :
	   case  5 :
	   case  9 :
		 ++in;
		 break;
	   case 3 :
	   case 7 :
	   case 11 :
		++re;
		break;
	   case 2 :
	   case 6 :
	   case 10 :
		++tm;
		break;
	   case 4 :
	   case 8 :
	   case 12 :
		++tr;
		break;
		}
     }
 fnc->individual = in;
 fnc->relative = re;
 fnc->temporal = tm;
 fnc->terminal = tr;
}          

/* function to count modal houses, angular, cadent, and succedent */   

void count_modals( AS_INF *info, struct MDL *mod )
{ 
 COUNT i, c, a, s;
 AS_INF  *pt;
 register int val;

 i = c = a = s = 0; /* initialize counters */
 for ( i = 0, pt = info + SUN; i <= maxplan; ++i, ++pt ) {
     val = pt->house;
     switch( val ) {
	   case 3 :
	   case 6 :
	   case 9 :
	   case 12 :
		++c;
		break;
	   case 1 :
	   case 4 :
	   case 7 :
	   case 10 :
		++a;
		break;
	   case 2 :
	   case 5 :
	   case 8 :
	   case 11 :
		++s;
		break;
	   }
     }
 mod->cadent = c;
 mod->angular= a;
 mod->succedent = s;
}



/* function to find the scout planet. By definition the scout planet is
   the planet which has the highest house number, ie is the last to rise */

NUM find_scout_planet( AS_INF *info )
{
 COUNT i;
 register AS_INF *pt;
 NUM pl, mi;
 register MINU sun;

 pl = 0, mi = 0;
 sun = (info+SUN)->minutes_total;
 for ( i = MOON, pt = info + MOON; i <= maxplan; ++i, ++pt ) {
     if ( pt->minutes_total < sun && pt->minutes_total > mi ) {
	pl = i;
	mi = pt->minutes_total;
        }
     }
 if ( pl == 0 ) {   /* this takes care of situations where the sun is
		       in a low degree and nothing preceeds it. */
    mi = sun;
    sun += d2m(360);
    for ( i = MOON, pt = info + MOON; i <= PLUTO; ++i, ++pt ) {
	if ( pt->minutes_total > mi ) {
	   pl = i;
	   mi = pt->minutes_total;
	   }
	}
    }
 return( pl );
}
/* function to count aspects */

void count_aspects( AS_INF *info, COUNT *array )
{
 AS_INF *inf;
 ASPEC  *as;
 COUNT i, j, *arp;
 
 for ( i = 0, arp = array; i <= 8; i++, arp++ )
     *arp = 0;
 for ( i = SUN, inf = info; i <= maxplan; ++i, ++inf ) {
         if ( inf->no_aspects ) {
            for ( j = 1, as = inf->aspectr; j <= inf->no_aspects; ++j, ++as ) 
                ++(*( array + as->aspect ) );
            }
     }
}


void count_signs( AS_INF *info, COUNT *ar )
{
 int i;
 AS_INF * inf;

 memset( ar, 0, 12 * sizeof( short ) );

 for ( i = SUN, inf = info + SUN; i <= maxplan; ++i, ++inf )
     ar[inf->sign]++;

}


void count_houses( AS_INF near *info, COUNT *ar )
{
 int i;
 AS_INF * inf;

 memset( ar, 0, 13 * sizeof( short ) );

 for ( i = SUN, inf = info + SUN; i <= maxplan; ++i, ++inf )
     ar[inf->house]++;
}


/* main calling procedures for doing miscellaneous summary calcs */

void do_summary_calc( AS_INF near *info, BITS mode )
{
 if ( mode & ASTEROID )
	 maxplan = CHIRON;
 else if ( mode & VERT_EAST )
	 maxplan = EAST_POINT;
 else 
	 maxplan = IM_COELI;
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
}
