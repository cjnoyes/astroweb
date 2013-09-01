/* placidus & koch house system setup functions */
#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "ascend.h"
#include "compute.fns"
#include "lngdbl.h"

#define OA_11 0  /* OA_11 - OA_3 is a range of ints from 0 to 4 */
#define OA_3  4  /* corresponding to array obl_asc[] below */

/* more globals in radians */
double obl_asc[5];                    /* table of oblique ascentions */ 

/* function to do related setup functions per formula.

   admc    is  ascentional difference of mc
   oamc    is  oblique ascention of mc  
   dsamc   is  diurnal semi arc of mc 
   arc     is  dsamc / 3  */

void setup_koch(void)
{
 double temp, admc, oamc,
 dsamc, arc;
 COUNT x, i;

 temp = _sin( right_asc_mh );  /* find ascentional differentce of mc */
 temp *= tan_obliquity;    /* per formula */
 temp *= tan_latitude;
 if ( temp > 1.0 )
    temp -= 1.0;
 else if ( temp < -1.0 )
    temp += 1.0;
 admc = asin( temp );
 oamc = right_asc_mh - admc;
 dsamc = ( PI / 2.0 ) + admc;
 arc = dsamc / 3.0;
 for ( x = 1, i = OA_11; i <= OA_3; ++x, ++i  )
     obl_asc[i] = oamc + ( arc * (double) x );
}


/* function to calculate single house cusp from data above. Takes
   obl_no, house_no which are indexes to olique ascentions and houses 
   Temporary variables which are used are: top equates to top of equation, 
   left is left half of bottom, right is right half of bottom */


void calc_house_cusp( NUM obl_no, NUM house_no, char dir )
{
 double top, left, right;

 top = _sin( obl_asc[obl_no] );
 left = _cos( obl_asc[obl_no] );
 left *= cos_obliquity;
 right = tan_latitude;
 right *= sin_obliquity;
 left -= right;
 house_cusps[house_no] = flip_mins( div_2_minutes( top, left ), dir );
}

/* function to do house cusp calcualtions calculates key house_cusps
   using calc_house_cusp() and calculates remainder by adding d2m(180) and
   adjusting result. */
 
void koch_house_table( MINU *asc, MINU *mc, char dir  )
{
 setup_koch();
 *asc = flip_mins( *asc, dir );
 *mc = flip_mins( *mc, dir );
 calc_house_cusp( (short)0, (short)11, dir );
 calc_house_cusp( (short)1, (short)12, dir );
 calc_house_cusp( (short)2, (short)1, dir );
 calc_house_cusp( (short)3, (short)2, dir );
 calc_house_cusp( (short)4, (short)3, dir );
 house_cusps[10] = *mc;
 house_cusps[4] = adjust_mins( *mc + d2m(180) );
 house_cusps[0] = house_cusps[12];
 house_cusps[7] = adjust_mins( house_cusps[1] + d2m(180) );
 house_cusps[6] = adjust_mins( house_cusps[12] + d2m(180) );
 house_cusps[5] = adjust_mins( house_cusps[11] + d2m(180) );
 house_cusps[8] = adjust_mins( house_cusps[2] + d2m(180) );
 house_cusps[9] = adjust_mins( house_cusps[3] + d2m(180) );
 }
