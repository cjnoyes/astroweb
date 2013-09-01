#include "stdafx.h"
#include "astro.h"
#include <stdio.h>
#include <stdlib.h>
#include "compute.fns"

/* converted To C++ Brought back from .asm
   for C++Builder */

extern int abs( int );
#define d2m(d) ((d)*60)

/* functions to support equal house system as well as universal
   house testing functions. Also has adjust_mins function and
   house_cusps table used by all systems */


MINU adjust_mins( MINU mins)
{
 register unsigned short m;

 if ( mins < 0 )
    return( mins + d2m(360) );
 else
    m = mins;
 if ( m >= d2m(360) )
    return( m - d2m(360) );
 else
    return( m );
}

/* function to flip minutes d2m(180) if dir is south */

MINU flip_mins( MINU mins, char dir )
{
 register unsigned m = mins;

 if ( dir == 's' || dir == 'S' ) {
    m += d2m(180);
    if ( m >= d2m(360) )
       m -= d2m(360);
    }
 return( m );
}


/* function to compare 2 house cusps and mins to determine if mins
   is between them. returns 1 if mins is in that house 0 if not */

short compare_house_cusps( MINU first, MINU second, unsigned short mins  )
{
 if ( second - first > 0 ) {
    first += d2m(2);
    second += d2m(2);
    mins += d2m(2);
    if ( mins > second - d2m(1) )
       return( 0 );
    else if ( mins <= first - d2m(1) )
       return( 0 );
    else
       return( 1 );
    }
 second += d2m(360);
 if ( mins >= first - d2m(1) )
    return( 1 );
 else if ( mins + d2m(360) <= second - d2m(1) )
    return( 1 );
 else
    return( 0 );
}


/* function to return house number based on mins and
   house_cusps table */

NUM house_number( MINU mins )
{
 COUNT i;

 if ( compare_house_cusps( house_cusps[0], house_cusps[1], mins ) )
    return( 12 );
 for ( i = 1; i <= 11; ++i ) {
     if ( compare_house_cusps( house_cusps[i], house_cusps[i+1], mins ) )
        return( i );
     }
 return( 0 );
}

/* function to determine if a house is on a cusp or is intercepted
   returns 0 if house is intercepted, 1 if house is on cusp */

short house_cusp_intercept( MINU mins, NUM house )
{
 short test;

 test = abs( house_cusps[house] - mins );
 if ( test <= d2m(1) + 20 )
    return( 1 );
 else if ( test >= d2m(358) + 40 )
    return( 1 );
 else
    return( 0 );
}
