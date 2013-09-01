#include "stdafx.h"
#include "astro.h"
#include <stdio.h>
#include <stdlib.h>
#include "compute.fns"

#define d2m(d) ((d)*60)

/* setup house cusp table based on equal house system */

void equal_house_table( MINU *asc, MINU *mc, char dir )
{
 COUNT x;
 unsigned a;

 *asc = adjust_mins( flip_mins( *asc, dir ) );
 *mc = adjust_mins( flip_mins( *mc, dir ) );
 house_cusps[0] = adjust_mins( *asc - d2m(30) );
 house_cusps[1] = *asc;
 house_cusps[0] = adjust_mins( *asc - d2m(30) );
 for ( x = 2, a = *asc + d2m(30); x <= 12; x++, a += d2m(30) ) {
     a = adjust_mins( a );
     house_cusps[x] = a;
     }

}


void m_house( MINU *asc, MINU *mc, char dir )
{
 COUNT x;
 unsigned mi;

 *asc = adjust_mins( flip_mins( *asc, dir ) );
 *mc = adjust_mins( flip_mins( *mc, dir ) );
 house_cusps[10] = *mc;
 house_cusps[11] = adjust_mins( *mc + d2m(30) );
 house_cusps[12] = adjust_mins( *mc + d2m(60) );
 for ( x = 1, mi = *mc +d2m(90); x <= 9; mi += d2m(30), ++x ) {
     mi = adjust_mins( mi );
     house_cusps[x] = mi;
     }
 house_cusps[0] = house_cusps[12];
}
