/* regiomontanus house cusps */
#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "compute.fns"
#include "ascend.h"
#include "lngdbl.h"

short regio_cusp( int house, char dir )
{
 int minutes;

 double inrads, x, y;
 inrads = degrees_2_rads( ( 30 * house ) + 60, 0 );
 y = _sin( right_asc_mh + inrads );
 x = atan( y / (( _cos( right_asc_mh + inrads ) * cos_obliquity )
       - ( _sin( inrads ) * tan_latitude * sin_obliquity ) ) );
 if (x < 0.0 )
    x += PI;
 if ( y < 0.0 )
    x += PI;
 minutes = rads_2_mins( x );
 return( flip_mins( minutes, dir ) );
}

void do_regiomont_houses( MINU *asc, MINU *mc, char dir )
{
 int x;

 *asc = flip_mins( *asc, dir );
 *mc = flip_mins( *mc, dir );
 for ( x = 1; x <= 12; ++x )
     house_cusps[x] = regio_cusp( x, dir );
 house_cusps[0] = house_cusps[12];
}
