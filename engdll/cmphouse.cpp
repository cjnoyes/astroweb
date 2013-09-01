#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "compute.fns"
#include "ascend.h"
#include "lngdbl.h"

static double cos_latitude;

MINU companus_cusp( int house, char dir )
{
 int minutes;
 double ko, dn, x, y;

 ko = degrees_2_rads( ( 30 * house ) + 60, 0 );
 ko += 0.00000001;
 dn = atan ( _tan( ko ) * cos_latitude );
 if ( dn < 0.0 )
    dn += PI;
 if ( _sin( ko ) < 0.0 )
    dn += PI;
 y = _sin( right_asc_mh + dn );
 x = ( _cos( right_asc_mh + dn ) * cos_obliquity ) -
	( _sin( dn ) * tan_latitude * sin_obliquity );
 minutes = div_2_minutes( y, x );
 return( flip_mins( minutes, dir ) );
}

void do_companus_houses( MINU *asc, MINU *mc,
				 DEGR lat_degs, MINU lat_mins, char dir )
{
 int x;

 cos_latitude = _cos( degrees_2_rads( lat_degs, lat_mins ) );
 *asc = flip_mins( *asc, dir );
 *mc = flip_mins( *mc, dir );
 for ( x = 1; x <= 12; ++x )
     house_cusps[x] = companus_cusp( x, dir );
 house_cusps[0] = house_cusps[12];
}
