#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "ascend.h"
#include "compute.fns"
#include "flags.h"
#include "lngdbl.h"

/* Modified for Windows */

extern MINU regio_cusp( int house, char dir );
extern MINU companus_cusp( int house, char dir );

int check_min( int min, int chk )
{
 if ( min < (30*60) && chk > (300*60) )
    return( chk - (360*60) );
 else if ( min > (300*60) && chk < (30*60) )
    return( chk + (360*60) );
 else
    return( chk );
}


double medcoeli_to_ramc( double mcr )
{
 double tmp;
 tmp = _tan(mcr);
 tmp *= cos_obliquity;
 tmp = atan(tmp);
 if ( tmp < 0 )
    tmp += PI + PI;
 if ( mcr > PI && tmp < PI )
    tmp += PI;
 else if ( mcr < PI && tmp > PI )
    tmp -= PI;
 return(tmp);
}

double min_medcoeli_to_ramc( MINU mc )
{
 double mcr;

 mcr = degrees_2_rads( mc / 60, mc % 60 );
 return(medcoeli_to_ramc(mcr));
}

double find_ramc_from_asc( double asc, DEG_MIN *lat )
{
 MINU r, xasc;
 double res, t, d;

 xasc = (int)(asc * 60.0);
 d = 0.0;
 t = asc - 90.0;
 if ( t < 0.0 )
    t += 360.0;
 t *= PI;
 t /= 180.0;
 asc *= PI;
 asc /= 180.0;
 if ( t < 0.0 )
    t += (PI * 2.0);
 right_asc_mh = t;
 do {
    right_asc_mh -= (d * 6.66e-1);
    if ( right_asc_mh > (PI * 2.0) )
       right_asc_mh -= (PI * 2.0);
    else if ( right_asc_mh < 0.0 )
       right_asc_mh += (PI * 2.0);
    r = check_min( xasc, calc_ascendant( lat ) );
    res = (double) r;
    res *= ( PI / 180.0 );
    res /= 60.0;
    d = res - asc;
    if ( x_kb_check() )
       return( right_asc_mh );
    } while ( d > 0.00005 || d < -0.00005 );
 if ( right_asc_mh > (PI * 2.0) )
    right_asc_mh -= (PI * 2.0);
 else if ( right_asc_mh < 0.0 )
    right_asc_mh += (PI * 2.0);
 return( right_asc_mh );
}


double comp_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
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
    r = check_min( hs_min, companus_cusp( house, lat->dir ) );
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

double plac_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
{
 MINU r, x_min;
 double res, t, d;

 x_min = (hs_min-(30*60));
 if ( x_min < 0 )
    x_min += 21600;
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
    do_placidus_cusp( house, lat->dir, x_min );
    r = check_min( hs_min, house_cusps[house] );
    res = (double) r;
    res *= ( PI / 180.0 );
    res /= 60.0;
    d = res - t;
    if ( d > ( PI * 0.666 ) )
       d -= PI;
    else if ( d < -( PI * 0.666 ) )
       d += PI;
    if ( x_kb_check() )
       return( right_asc_mh );
    } while ( d > 0.00005 || d < -0.00005 );
 if ( right_asc_mh > (PI * 2.0) )
    right_asc_mh -= (PI * 2.0);
 else if ( right_asc_mh < 0.0 )
    right_asc_mh += (PI * 2.0);
 return( right_asc_mh );
}

double koch_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
{
 MINU r;
 double res, t, d;
 static short obl_nos[] = { 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

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
    setup_koch();
    calc_house_cusp( obl_nos[house], house, lat->dir );
    r = check_min( hs_min, house_cusps[house] );
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

double regio_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
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
    r = check_min( hs_min, regio_cusp( house, lat->dir ) );
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

double morinus_house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, double iramc )
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
    r = check_min( hs_min, morinus_cusp( house, lat->dir ) );
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


double house_to_ramc( int house, MINU hs_min, DEG_MIN *lat, int hs_sys )
{
 double d, drmc;
 static short ev[] = { 0, 0, -1800, -3600, -5400, -7200, -9000, -10800, -12600,
       -14400, -16200, -18000, -19800  };
 static MINU mv[] = { 0, -7200, -9000, -10800, 10800, 9000, 7200, 5400, 3600,
       1800, 0, -1800, -3600 };
 MINU m;

 switch( house ) {
       case 1 :
	    if ( hs_sys != M_HOUSE ) {
	       d = (double)hs_min / 60.0;
	       return(find_ramc_from_asc( d, lat ));
	       }
	    break;
       case 4 :
	    if ( hs_sys != EQUAL ) {
	       if ( hs_min > ( 180 * 60 ) )
		  return(min_medcoeli_to_ramc( hs_min - (180 * 60)));
	       else
		  return(min_medcoeli_to_ramc( hs_min + (180 * 60)));
	       }
	    break;
       case 5 :
	    house = 11;
	    hs_min = flip_mins( hs_min, 'S' );
	    break;
       case 6 :
	    house = 12;
	    hs_min = flip_mins( hs_min, 'S' );
	    break;
       case 7 :
	    if ( hs_sys != M_HOUSE ) {
	       d = (double)hs_min / 60.0;
	       if ( d > 180.0 )
		  d -= 180.0;
	       else
		  d += 180.0;
	       return( find_ramc_from_asc(d,lat));
	       }
	    break;
       case 8 :
	    house = 2;
	    hs_min = flip_mins( hs_min, 'S' );
	    break;
       case 9 :
	    house = 3;
	    hs_min = flip_mins( hs_min, 'S' );
	    break;
       case 10 :
	    if ( hs_sys != EQUAL )
	       return( min_medcoeli_to_ramc(hs_min));
	    break;
       case 13 :
	    d = (double)hs_min / 60.0;
	    return(find_ramc_from_asc( d, lat ));
	    break;
       case 14 :
	    hs_min = flip_mins( hs_min, 'S' );
	    d = (double)hs_min / 60.0;
	    return(find_ramc_from_asc( d, lat ));
	    break;
       case 15 :
	    return( min_medcoeli_to_ramc(hs_min));
	    break;
       case 16 :
	    if ( hs_min > ( 180 * 60 ) )
	       return( min_medcoeli_to_ramc(hs_min - (180 * 60)));
	    else
	       return( min_medcoeli_to_ramc(hs_min + (180 * 60)));
	    break;
       default :
	    break;
       }
 if ( hs_sys != EQUAL || hs_sys != M_HOUSE ) {
    m = hs_min + mv[house];
    if ( m < 0 )
       m += 21600;
    else if ( m > 21599 )
       m -= 21600;
    drmc = min_medcoeli_to_ramc( m );
    }
 switch( hs_sys ) {
       case EQUAL :
	    m = hs_min + ev[house];
	    if ( m < 0 )
	       m += 21600;
	    d = (double) ((double)m / 60.0);
	    return( find_ramc_from_asc( d, lat ) );
	    break;
       case PLACIDUS :
	    return(plac_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
       case M_HOUSE :
	    m = hs_min + mv[house];
	    if ( m < 0 )
	       m += 21600;
	    else if ( m > 21599 )
	       m -= 21600;
	    return( min_medcoeli_to_ramc( m ) );
	    break;
       case COMPANUS :
	    return(comp_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
       case REGIOMONT :
	    return(regio_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
       case KOCH :
	    return(koch_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
       case MORINUS :
	    return(morinus_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
	   case PORPHYRYX :
	    return(porphyryx_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
	   case TOPOCENTRIC :
	    return(topocentric_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;
       case MERIDIAN :
	    return(meridian_house_to_ramc( house, hs_min, lat, drmc ) );
	    break;

       default :
	    break;
       }
 return( 0 );
}
