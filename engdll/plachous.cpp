#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "ascend.h"
#include "compute.fns"
#include "lngdbl.h"

/* FUNCTIONS TO SUPPORT PLACIDUS HOUSE ROUTINES */

/* contents of ascend.h  */

/* these variabler are in radians, or their own natural range
   where they are functions. I calculated functions ahead of time to
   save steps later. Becomes useful in koch_house and in plac_hous
   where multiple iterations of basic formulas are called 

extern double right_asc_mh;
extern double sin_obliquity;
extern double cos_obliquity;
extern double cos_obliquity;
extern double tan_obliquity;
extern double tan_latitude;
extern double med_coeli;

#define PI 3.141516

*/

#define MAXERR 0.000174

/* function to do a placidus cusp. It is called with the cusp
   number, direction North or South and the last house cusp
   degree. The last house cusp degree is used to determine
   and correct the quadrant the house is in. The dir variable
   is used in the call to flip_mins for southern hemisphere
   variables. Quadrant correction is needed because of the 
   functioning of the tan/atan function. It is very difficult
   to distinguish from a degree in the first quad from the
   third quad, and the second from the fourth. Attempt is 
   to create a contiguous array of degrees allowing for gap
   when we cross over 360/0 point */ 

void do_placidus_cusp( NUM cusp_no, char dir, MINU last_min )
{
 double f, ra1, ra2, temp, err;
 COUNT x = 1;
 MINU min, dif;

 ra2 = -99.0;            /* out of range to trip if */
 switch( cusp_no ) {     /* all per formula */
       case 11 :
            f = 3.0;
            ra1 = right_asc_mh + ( PI / 6.0 );
            break;
       case 12 :
            f = 1.5;
            ra1 = right_asc_mh + ( PI / 3.0 );
            break;
       case 2 :
            f = 1.5;
            ra1 = right_asc_mh + ( PI / 1.5 );
            break;
       case 3 :
            f = 3.0;
            ra1 = right_asc_mh + ( PI / (5.0/6.0) );
            break;
            }
 do {
    if ( ra2 != -99.0 )
       ra1 = ra2; 
    temp = _sin( ra1 ) * tan_obliquity * tan_latitude;
    if ( cusp_no > 10 ) 
       temp = -temp;
    if ( temp > 1.0 )
       temp -= 1.0;
    else if ( temp < -1.0 )
       temp += 1.0;
    temp = acos( temp );
    if ( temp < 0.0 )
       temp += ( 2.0 * PI ); 
    temp /= f;
    ra2 = right_asc_mh;
    if ( cusp_no < 10 ) 
       ra2 += ( PI - temp );
    else
       ra2 += temp;  
    err = ra1 - ra2;
    }
 while ( fabs( err ) >= MAXERR );
 temp = atan2( _tan( ra2 ), cos_obliquity );
 min = rads_2_mins( temp );
 if ( min < 0 )
    min += d2m(360);
 min = flip_mins( min, dir );
 dif = min - last_min;
 if ( dif > d2m(90) )              /* if so we are in the wrong quadrant */
    min = adjust_mins( min + d2m(180) );
 else if ( dif < 0 && dif > d2m(-270) )   /* if so wrong quadrant */
    min = adjust_mins( min + d2m(180) );
 house_cusps[cusp_no] = min;
}


void placidus_house_table( MINU *asc, MINU *mc, char dir  )
{
 *asc = flip_mins( *asc, dir );
 *mc = flip_mins( *mc, dir );
 do_placidus_cusp( 11, dir, *mc );
 do_placidus_cusp( 12, dir, house_cusps[11] );
 do_placidus_cusp( 2, dir, *asc );
 do_placidus_cusp( 3, dir, house_cusps[2] );
 house_cusps[1] = *asc;
 house_cusps[10] = *mc;
 house_cusps[4] = adjust_mins( *mc + d2m(180) );
 house_cusps[0] = house_cusps[12];
 house_cusps[7] = adjust_mins( house_cusps[1] + d2m(180) );
 house_cusps[6] = adjust_mins( house_cusps[12] + d2m(180) );
 house_cusps[5] = adjust_mins( house_cusps[11] + d2m(180) );
 house_cusps[8] = adjust_mins( house_cusps[2] + d2m(180) );
 house_cusps[9] = adjust_mins( house_cusps[3] + d2m(180) );
}
