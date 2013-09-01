#include <stdafx.h>
#include <math.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "utl.h"
#include "compute.fns"
#include "ascend.h"



extern int check_min( int min, int chk );
MINU porphryx_house(int i)
{
	double x; 

	if ( i > 6 )
		return( (porphryx_house(i-6)+(180*60))%(360*60));
	if ( i == 5 || i == 6 )
	   x = asc_rads - med_coeli;
	else
	   x = fmodulus(PI+med_coeli, TWO_PI)-asc_rads;
    if ( x < 0.0 )
	   x += TWO_PI;
    double y = x / 3;
 
	switch ( i ) {
	case 1 :
		return (rads_2_mins(asc_rads));
		break;
	case 2 :
	case 3 :
		return( rads_2_mins(fmodulus(asc_rads+((double)(i-1)*y), TWO_PI)));
		break;
	case 4 :
		return ( rads_2_mins(fmodulus(med_coeli+PI,TWO_PI)));
		break;
	case 5 :
	case 6 :
		return( rads_2_mins(fmodulus(PI+med_coeli+((double)(i-4) * y),TWO_PI)));
		break;
	default:
		return ( 0 );
		break;
	}
 return ( 0 );
}

void porphyryx_houses()
{
 for ( int i = 1; i <= 12; i++ )
	 house_cusps[i] = porphryx_house(i);
 house_cusps[0]=house_cusps[12];
}



MINU meridian_house( int i )
{
	double d;
	d = d2r((30*i)+60);
	double y = ( sin(right_asc_mh+d));
	double x= atan(y/(cos(right_asc_mh+d) * cos_obliquity));
	if ( x < 0.0 ) 
		x += PI;
	if ( y < 0 )
		x += PI;
    return ( rads_2_mins(fmodulus(x,TWO_PI)));
}

void meridian_houses()
{
 for ( int i = 1; i <= 12; i ++ )
	 house_cusps[i] = meridian_house(i);
 house_cusps[0]=house_cusps[12];

}

double topocentric_sub(double oa, double la )
{
 double x1 = atan(tan(la)/cos(oa));
 double y = x1 + d2r(obliquity);
 double lo = atan(cos(x1)*tan(oa)/cos(y));
 if ( lo < 0.0 )
	 lo += PI;
 if ( sin(oa)<0.0)
	 lo += PI;
 return( lo );
}

MINU topocentric_house( int i) 
{
 static double p1, p2;
 static int times[7] = {0,3,4,5,0,1,2};
 if ( i == 1 ) {
    p1 = atan(tan_latitude/3.0);
    p2 = atan((tan_latitude/3.0)*2);
	}
 if ( i > 6 )
	 return ( (topocentric_house(i-6)+(180*60))%(360*60) );
 double oa = fmodulus(right_asc_mh+d2r((30*times[i])),TWO_PI);
 switch(i) {
 case 1 :
	 return( rads_2_mins(asc_rads));
	 break;
 case 2 :
	 return( rads_2_mins(fmodulus( topocentric_sub(oa,p2), TWO_PI)));
	 break;
 case 3 :
	 return( rads_2_mins(fmodulus( topocentric_sub(oa,p1), TWO_PI)));
	 break;
 case 4 :
	 return(rads_2_mins(fmodulus(med_coeli+PI,TWO_PI) ) );
	 break;
 case 5 :
	 return( rads_2_mins(fmodulus( topocentric_sub(oa,p1)+PI, TWO_PI)));
	 break;
 case 6 :
	 return( rads_2_mins(fmodulus( topocentric_sub(oa,p2)+PI, TWO_PI)));
	 break;
 default :
	 return( 0 );
	 break;
 
 }


}


void topocentric_houses()
{
 int i;
 for ( i = 1; i <= 12; i++ )
	 house_cusps[i] = topocentric_house(i);
 house_cusps[0]=house_cusps[12];

}

double topocentric_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
{
 MINU r;
 double res, t, d;

 d = 0.0;
 right_asc_mh = iramc;
 t = (( (double) hs_min * PI ) / ( 180.0 * 60.0 ) );
 if ( t > (PI * 2.0) )
    t -= (PI * 2.0);
 do {
    right_asc_mh -= (d * 6.66e-1);
    if ( right_asc_mh > (PI * 2.0) )
       right_asc_mh -= (PI * 2.0);
    else if ( right_asc_mh < 0.0 )
       right_asc_mh += (PI * 2.0);
    r = check_min( hs_min, topocentric_house( house) );
    res = (double) r;
    res *= ( PI / 180.0 );
    res /= 60.0;
    d = res - t;
    if ( x_kb_check() )
       return( right_asc_mh );
    } while ( d > 0.00005 || d < -0.00005 );
 if ( right_asc_mh > (PI * 2.0) )
    right_asc_mh -= (PI * 2.0);
 else if ( right_asc_mh < 0.0 )
    right_asc_mh += (PI * 2.0);
 return( right_asc_mh );
}

double porphyryx_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
{
 MINU r;
 double res, t, d;

 d = 0.0;
 right_asc_mh = iramc;
 t = (( (double) hs_min * PI ) / ( 180.0 * 60.0 ) );
 if ( t > (PI * 2.0) )
    t -= (PI * 2.0);
 do {
    right_asc_mh -= (d * 6.66e-1);
    if ( right_asc_mh > (PI * 2.0) )
       right_asc_mh -= (PI * 2.0);
    else if ( right_asc_mh < 0.0 )
       right_asc_mh += (PI * 2.0);
    r = check_min( hs_min, porphryx_house( house) );
    res = (double) r;
    res *= ( PI / 180.0 );
    res /= 60.0;
    d = res - t;
    if ( x_kb_check() )
       return( right_asc_mh );
    } while ( d > 0.00005 || d < -0.00005 );
 if ( right_asc_mh > (PI * 2.0) )
    right_asc_mh -= (PI * 2.0);
 else if ( right_asc_mh < 0.0 )
    right_asc_mh += (PI * 2.0);
 return( right_asc_mh );
}

double meridian_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
{
 MINU r;
 double res, t, d;

 d = 0.0;
 right_asc_mh = iramc;
 t = (( (double) hs_min * PI ) / ( 180.0 * 60.0 ) );
 if ( t > (PI * 2.0) )
    t -= (PI * 2.0);
 do {
    right_asc_mh -= (d * 6.66e-1);
    if ( right_asc_mh > (PI * 2.0) )
       right_asc_mh -= (PI * 2.0);
    else if ( right_asc_mh < 0.0 )
       right_asc_mh += (PI * 2.0);
    r = check_min( hs_min, meridian_house( house) );
    res = (double) r;
    res *= ( PI / 180.0 );
    res /= 60.0;
    d = res - t;
    if ( x_kb_check() )
       return( right_asc_mh );
    } while ( d > 0.00005 || d < -0.00005 );
 if ( right_asc_mh > (PI * 2.0) )
    right_asc_mh -= (PI * 2.0);
 else if ( right_asc_mh < 0.0 )
    right_asc_mh += (PI * 2.0);
 return( right_asc_mh );
}
