#include "stdafx.h"
#include <float.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include "astro.h"
#include "useri.h"
#include "flags.h"
#include "compute.fns"
#include "datetime.fns"
#include "errors.fns"
#include "ascend.h"
#include "julian.h"
#include "dataext.h"
#include "utlduad.h"
#include <setjmp.h>
#include <float.h>
#include "asteroiddata.h"
#include "asteroidcalc.h"
#include <string.h>

extern void GetPlanetData( int plan, AS_INF *inf, double jul );

/* 09/10/92  Modified for C++ Windows */

jmp_buf j;
extern double sun_longitude;
#define RESET_ENGINE 999

#ifdef USEDLL
extern void get_house_cusps( short *c );
#endif

/*
int _matherr( struct _exception *e )
{
 static char *errs[] = { "", "Domain", "Singularity", "Overflow",
   "Underflow", "Total Loss of Significance", "Partial Loss of Significance" };

 if ( (!strcmpi(e->name, "acos" ) || !strcmpi(e->name, "asin" )) && e->type == DOMAIN ) {
    alert_box( "MATH ERROR!!", "Too High Latitude for Ascendant, "
		 "or Too Low Latitude for Vertex!!" );
    e->retval = 0.0;
    errno = EDOM;
    }
 else {
    if ( e->type && e->type <= _PLOSS ) {
       alert_box( "MATH ERROR!!", "%s Error in %s--Args are: %lf and %lf, retval = %lf--"
		  "Call Technical Support with These Details!!\n",
		  errs[e->type], e->name, e->arg1, e->arg2, e->retval );
       e->retval = 0.0;
       errno = EDOM;
       }
    else
       return( 0 );
    }
 _fpreset();
 longjmp( j, 3 );
 return( 1 );
}
*/

typedef void (*sigtype)(int sig);

void fpehnd(int sig, int num )
{
 static char *errs[] = {"", "Invalid", "Denormal", "Divide By Zero",
      "Overflow", "Underflow", "Inexact", "UnEmulated Instruction",
      "Negative Square Root", "", "Stack Overflow", "Stack Underflow", "" };
 if ( num >= _FPE_INVALID || num <= _FPE_EXPLICITGEN ) {
    num %= 0x80;
    alert_box( "Math Error!!", "Error: %s--Call Technical Support and "
       "Report this Error!!", errs[num] );
    }
 else
    return;
 errno = EDOM;
 _fpreset();
 longjmp( j, 2 );
 return;
}

/* module to do do calcualtions and call other modules */

/* function to determine if a sign is critical. Returns 1
   if critical 0 if not */
/* **** These are now in DOMISCA.ASM **** */
short critical_degree( DEGR deg, NUM sign )
{
 if ( sign == ARIES || sign == CANCER || 
      sign == LIBRA || sign == CAPRICORN )
    if ( deg == 0 || deg == 13 || deg == 26 ) 
       return( 1 );
    else
       return( 0 );  
 else if ( sign == TAURUS || sign == LEO || 
           sign == SCORPIO || sign == AQUARIUS )
    if ( deg == 9 || deg == 21 )
       return( 1 );
    else
       return( 0 );
 else if ( sign == GEMINI || sign == VIRGO ||
           sign == SAGITARIUS || sign == PISCES ) 
    if ( deg == 4 || deg == 17 )
       return( 1 );
    else
       return( 0 );
 else
    return( 0 );
}


/* function to calculate minor degrees and sign. called with
   pointer. If code is non-zero degrees is converted from
   tropical to sidereal. Also tests house, cusps house and sign,
   and tests for various other functions */

/*  **** Also in DOMISCA.ASM **** */

void misc_calc( AS_INF *ptr, BITS code, short year )
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
 if ( ptr->longitude == 0.0 )
	 ptr->longitude = (double)ptr->minutes_total/60.0;
 if ( ptr->dec_minutes_total ) {
    ptr->dec_degrees = (int)ptr->declination;
	ptr->dec_minutes_minor = ptr->dec_minutes_total % 60;
 }
 else {
	ptr->dec_degrees = 0;
	ptr->dec_minutes_minor = 0;
 }
}


/* function to call other modules and do calculations.
   enter with pointer to array of AS_INF type of structure
   and pointer to AS_DATA type of structure. Also supply
   operating code which selects various options and house_proc
   code which calls house processing routines */

void do_calc( AS_INF *info, AS_DATA *data, BITS mode, NUM house_proc,
						     DATA_PACKET *dpk )
{
 COUNT i;
 LMIN min;
 DEGR degrees, d = 0;
 JULIAN jul;
 AS_INF *inf_ptr;
 TIM sid_time;
 DEG_MIN ver_lat, rlt, rln;
 short bm, bbm, bha;
 static DEG_MIN z_lat = { 0,0,'N' };
 DATES nd;
 short polarmode;
 
 polarmode = house_proc & (POLARSWAP|POLARKEEP);
 house_proc &= 0xf;
 if ( setjmp( j ) ) {        /* Math Error Occurred */
    return;
    }
 if ( signal( SIGFPE, (sigtype)fpehnd ) == SIG_ERR ) {
    return;
    }
 if ( !data->longitude.dir || !data->latitude.dir ) {
    alert_box( "", "Longitude or Latitutde Are Not Set!" );
    }
#ifdef DELUXE
 if ( dpk->chart_code & RELOC_CH ) {
    rln = data->longitude;
    rlt = data->latitude;
    /* rm = data->local_adjust;
       rh = data->local_minutes; */
    if ( !dpk->relocp->longitude.dir || !dpk->relocp->latitude.dir ) {
    alert_box( "", "Relocation Longitude or Latitutde Are Not Set!" );
    }
    data->longitude = dpk->relocp->longitude;
    data->latitude = dpk->relocp->latitude;
    /* data->local_adjust = dpk->relocp->hrs_offset;
       data->local_minutes = dpk->relocp->min_offset; */
    }
#endif
 if ( mode & NOBIRTHTIM ) {
 if ( dpk->rdp->rect_system == KNOWN_ANGL ) {
       data->hour_adjust = noon_date( &data->birth_date, &data->noon_date,
	   &data->noon_time, &dpk->rdp->trial,
	   data->local_adjust, data->local_minutes );
#ifdef DELUXE
       if ( dpk->chart_code & PROGR_CH )
	  progress_date( data, dpk->pdp );
#endif
       julian_calc( &data->birth_date, data->hour_adjust, data->noon_time.minutes, &jul );
       setup_obliquity( &jul );
       hs_min_to_TIM( &data->noon_date, &dpk->rdp->trial, &data->longitude,
	   &data->latitude, dpk->rdp->total_minutes, house_proc,
	   dpk->rdp->angle, data->local_adjust, data->local_minutes );
       }
    if ( dpk->rdp->trial.hours > 12 )
		dpk->rdp->am_pm = 1;
	else
        dpk->rdp->am_pm = 0;
    bm = data->noon_time.minutes;
    data->hour_adjust = noon_date( &data->birth_date, &data->noon_date,
	    &data->noon_time, &dpk->rdp->trial,
	    data->local_adjust, data->local_minutes );
    bbm = bm;
    nd = data->noon_date;
    bha = data->hour_adjust;
#ifdef DELUXE
    if ( dpk->chart_code & PROGR_CH )
       progress_date( data, dpk->pdp );
#endif
    julian_calc( &data->noon_date, data->hour_adjust,
	 data->noon_time.minutes, &jul );
    }
 else {
    bm = data->noon_time.minutes;
    data->hour_adjust = noon_date( &data->birth_date, &data->noon_date,
	    &data->noon_time, &data->birth_time,
	    data->local_adjust, data->local_minutes );
    bbm = bm;
    nd = data->noon_date;
    bha = data->hour_adjust;
#ifdef DELUXE
    if ( dpk->chart_code & PROGR_CH )
       progress_date( data, dpk->pdp );
#endif
    julian_calc( &data->noon_date, data->hour_adjust, data->noon_time.minutes, &jul);
    }
 if ( !( dpk->chart_code & XCSIDTM_ ) ) {
    if ( dpk->chart_code & PROGR_CH )
       sidereal_time( &data->noon_date, bha, bbm,
	     &data->longitude, &data->sidereal, data->latitude.dir );
    else
       sidereal_time( &data->noon_date, data->hour_adjust, data->noon_time.minutes,
	    &data->longitude, &data->sidereal, data->latitude.dir );
    }
 setup_obliquity(&jul );
 if ( mode & VERT_EAST && data->latitude.degrees > 30 ) {
				/* calculate vertex & east point */
    memcpy( &sid_time, &data->sidereal, sizeof(TIM) );
    memcpy( &ver_lat, &data->latitude, sizeof(DEG_MIN) );
    ver_lat.degrees = 90 - ver_lat.degrees;
    if ( ver_lat.minutes ) {
       ver_lat.degrees--;
       ver_lat.minutes = 60 - ver_lat.minutes;
       }
    if ( sid_time.hours > 11 )
       sid_time.hours -= 12;
    else
       sid_time.hours += 12;
    comp_med_coeli( &sid_time );
    (info + VERTEX )->minutes_total = flip_mins(calc_ascendant( &ver_lat ), data->latitude.dir );
    (info + VERTEX )->retrograde = 0;
    comp_med_coeli( &data->sidereal );
    (info + EAST_POINT )->minutes_total = flip_mins(calc_ascendant( &z_lat ), data->latitude.dir );
    (info + EAST_POINT )->retrograde = 0;
	(info + VERTEX )->longitude=0;
	(info + VERTEX )->declination = 0;
	(info + VERTEX )->dec_minutes_total=0;
	(info + EAST_POINT )->longitude=0;
	(info + EAST_POINT )->declination = 0;
	(info + EAST_POINT )->dec_minutes_total=0;
    (info + EAST_POINT )->calced = 1;
	(info + VERTEX )->calced=1;
    }
 (info + MED_COELI )->minutes_total = comp_med_coeli( &data->sidereal );
 (info + IM_COELI )->minutes_total = adjust_mins( flip_mins(
 (info + MED_COELI )->minutes_total+d2m(180), data->latitude.dir ) );
 (info + MED_COELI )->longitude=0;
 (info + MED_COELI )->declination = 0;
 (info + MED_COELI )->dec_minutes_total=0;
 (info + IM_COELI )->longitude=0;
 (info + IM_COELI )->declination = 0;
 (info + IM_COELI )->dec_minutes_total=0;
 (info + MED_COELI )->calced=1;
 (info + IM_COELI )->calced=1;
 inf_ptr = info;
 inf_ptr->minutes_total = calc_ascendant( &data->latitude );
 inf_ptr->retrograde = 0;
 inf_ptr->longitude=0;
 inf_ptr->declination = 0;
 inf_ptr->dec_minutes_total=0;
 (info + IM_COELI )->retrograde = 0;
#ifdef DELUXE
 if (  dpk->chart_code & CSIDTM_X ) {
    if ( dpk->chart_code & RELOC_CH ) {
       data->longitude = rln;
       data->latitude = rlt;
       }
    return;
    }
 #endif
 if ( mode & HOUSES ) {
    switch( house_proc ) {
	   case EQUAL :
	       equal_house_table( &inf_ptr->minutes_total,
		     &(info + MED_COELI)->minutes_total, data->latitude.dir );
                break;
           case KOCH :
		koch_house_table( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.dir );
                break;
           case PLACIDUS :
		placidus_house_table( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.dir );
                break;
           case M_HOUSE :
		m_house( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.dir );
                break;
	   case REGIOMONT :
		do_regiomont_houses( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.dir );
		break;
	   case COMPANUS :
		do_companus_houses( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.degrees,
		    data->latitude.minutes, data->latitude.dir );
		break;
	   case MORINUS :
		do_morinus_houses( &inf_ptr->minutes_total,
		    &(info + MED_COELI )->minutes_total, data->latitude.dir );
		break;
	   case PORPHYRYX :
        porphyryx_houses();
        break;
       case TOPOCENTRIC :
		topocentric_houses();
		break;
       case MERIDIAN :
		meridian_houses();   
	   case IMPORT :
                break;
	   }
    }
 inf_ptr = info + SUN;
 inf_ptr->minutes_total = sun_minutes( &jul );
 GetPlanetData( SUN, inf_ptr, jul.jd_4000bc );
 inf_ptr->retrograde = 0;
 inf_ptr->calced=1;
 if ( dpk->rdp->rect_system == SOLARCHART && mode & NOBIRTHTIM ) {
      degrees = inf_ptr->minutes_total;
      equal_house_table( &degrees, &d, data->latitude.dir );
      info->minutes_total = degrees;
      (info + MED_COELI )->minutes_total = house_cusps[10];
      (info + MED_COELI )->retrograde = 0;
      (info + IM_COELI )->minutes_total = house_cusps[4];
      (info + IM_COELI )->retrograde = 0;
	  (info + MED_COELI )->longitude=0;
	  (info + MED_COELI )->calced=1;
	  (info + MED_COELI )->declination = 0;
	  (info + MED_COELI )->dec_minutes_total=0;
	  (info + IM_COELI )->longitude=0;
	  (info + IM_COELI )->declination = 0;
	  (info + IM_COELI )->dec_minutes_total=0;
      (info + IM_COELI )->calced=1;
      }
 else if ( dpk->rdp->rect_system == FLATCHART && mode & NOBIRTHTIM ) {
      degrees = 0;
      equal_house_table( &degrees, &d, data->latitude.dir );
      info->minutes_total = degrees;
	  info->calced =1;
      (info + MED_COELI )->minutes_total = house_cusps[10];
      (info + MED_COELI )->retrograde = 0;
      (info + IM_COELI )->minutes_total = house_cusps[4];
      (info + IM_COELI )->retrograde = 0;
	  (info + MED_COELI )->longitude=0;
	  (info + MED_COELI )->declination = 0;
	  (info + MED_COELI )->dec_minutes_total=0;
	  (info + IM_COELI )->longitude=0;
	  (info + IM_COELI )->declination = 0;
	  (info + IM_COELI )->dec_minutes_total=0;
	  (info + MED_COELI )->calced=1;
	  (info + IM_COELI )->calced=1;
      }
 fixup_mcic_polar((info + MED_COELI )->minutes_total, (info + IM_COELI )->minutes_total,
	 data->latitude.degrees, polarmode & POLARSWAP, info->retrograde );
 if ( mode & HOUSES ) {
#ifdef USEDLL
     get_house_cusps(house_cusps);
#endif
     do_intercept_sign_table( house_proc );
      if ( errno == EDOM ) {
#ifdef DELUXE
	 if ( dpk->chart_code & RELOC_CH ) {
	    data->longitude = rln;
	    data->latitude = rlt;
	    /* data->local_adjust = rm;
	       data->local_minutes = rh; */
	    }
#endif
	 return;
	 }
      if ( mode & SIDEREAL ) {
	 for ( i = 0; i <= 12; ++i )
	     house_cusps[i] = tropical_2_sidereal( house_cusps[i],
		 data->noon_date.year );
	 }
    }
 #ifdef DELUXE
 if ( dpk->chart_code & RELOC_CH ) {
     data->longitude = rln;
     data->latitude = rlt;
    }
 #endif
 ++inf_ptr;
 inf_ptr->minutes_total = moon_minutes( &jul,
		  &(info + NORTH_NODE)->minutes_total );
 inf_ptr->retrograde = 0;
 GetPlanetData( MOON, inf_ptr, jul.jd_4000bc );
 (info + NORTH_NODE)->retrograde=1;
 (info + SOUTH_NODE)->calced=1;
 (info + MOON)->calced=1;
 (info + NORTH_NODE)->calced=1;
 (info + SOUTH_NODE)->retrograde=1;
 (info + NORTH_NODE )->longitude=0;
 (info + NORTH_NODE )->declination = 0;
 (info + NORTH_NODE )->dec_minutes_total=0;
 (info + SOUTH_NODE )->longitude=0;
 (info + SOUTH_NODE )->declination = 0;
 (info + SOUTH_NODE )->dec_minutes_total=0;
 ++inf_ptr;
 for ( i = MERCURY; i <= PLUTO; ++i, ++inf_ptr ) {
     inf_ptr->minutes_total = planet_minutes( i, &jul,
					  &inf_ptr->retrograde );
	 GetPlanetData( i, inf_ptr, jul.jd_4000bc );
	 inf_ptr->calced = 1;
     }
 inf_ptr = info+SOUTH_NODE;
 inf_ptr->minutes_total = adjust_mins( (info + NORTH_NODE )->minutes_total
	 + d2m(180) );
 if( !(mode & NOBIRTHTIM) ) {
   min = (info + ASCEND )->minutes_total;
   min += (info + MOON )->minutes_total;
   min -= (info + SUN)->minutes_total;
   if ( min < 0L )
      min += d2m(360L);
   min %= d2m(360L);
   (info + PART_FORTU )->minutes_total = (short) min;
   (info + PART_FORTU )->retrograde = 0;
   (info + PART_FORTU )->longitude=0;
   (info + PART_FORTU )->declination = 0;
   (info + PART_FORTU )->dec_minutes_total=0;
   (info + PART_FORTU )->calced=1;
   }
#ifdef DELUXE
 if ( mode & ASTEROID ) {
	asteroidcalc *calc = new asteroidcalc(&jul,(LONG_DOUBLE)sun_longitude);
	for ( int jj = CUPIDO; jj <= CHIRON; jj++ ) {
        if ( calc->calcnext(info[jj].minutes_total,info[jj].retrograde, info[jj].longitude, info[jj].declination) == FALSE )
			break;
	    info[jj].dec_minutes_total = (short)(info[jj].declination * 60.0);	
	    info[jj].calced = 1;
	}
    delete calc;	
 }
#endif
 int endplan;
 if ( mode & ASTEROID )
    endplan = CHIRON;
 else
    endplan = EAST_POINT;
 for ( i = ASCEND, inf_ptr = info; i <= endplan; ++i, ++inf_ptr )
     misc_calc( inf_ptr, mode, data->noon_date.year );
 if ( mode & ASPECTS ) {
    process_aspects( info, info, mode, dpk->chart_code );
    if ( errno == ENOMEM ) {
       return;
       }
    }
#ifdef DELUXE
 if ( mode & BASE_MID ) {
    do_basic_midpts( info, mode & (MID_ASPECT | VERT_EAST|ASTEROID) );
    if ( errno == ENOMEM ) {
       return;
       }
    }
 else if ( mode & FULL_MID || mode & HALF_MID ) {
     int mp = SOUTH_NODE;
	 if ( mode & ASTEROID )
		 mp = CHIRON;
	 else if ( mode & VERT_EAST )
		 mp = EAST_POINT;
	 do_main_full_midpoints( info, mp, info, mp, mode,  1 );
	 //do_full_midpoints( info, info, mode & (MID_ASPECT | NOBIRTHTIM | VERT_EAST | HALF_MID | FULL_MID | ASTEROID ) );
    if ( errno == ENOMEM ) {
       return;
       }
    }
#endif
 if ( mode & EXT_CALCS )
    do_extended_misc_calc( info, birth_ext, mode );
 if ( mode & SUMMARY )
    do_summary_calc( info, mode & ( NOBIRTHTIM | HOUSES | VERT_EAST | ASTEROID| ASPECTS ) );

 if ( !(mode & HOUSES ) )
    house_cusps[1] = -60; /* signal to asgraphc.exe that we have no houses */
}
