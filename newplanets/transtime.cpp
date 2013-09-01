#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include "kep.h"
#include "julian.h"
#include "datetime.fns"
#include "astro.h"

double GetMinutesDays( int planet, int &valid )
{
 struct orbit *ele;
 valid = 1;
 switch ( planet ) {
	  case MOON :
		  return( (360.0/28.0)*60.0 );
		  break;
	  case SUN :
		  ele = earth;
		  break;
	  case MERCURY :
	  case VENUS :
	  case MARS :
	  case JUPITER :
	  case SATURN :
	  case URANUS :
	  case NEPTUNE :
	  case PLUTO :
		  ele = elements[planet-MERCURY]
		  break;
	  default :
		  valid = 0;
		  return( 0.0 );
      }
 return ( (1.0/ *ele.dm)*60.0 );
 }

double GetDaySpan( int planet, int min, int &valid )
{
 
 double minday = GetMinutesDays(planet, valid );
 if ( !valid )
	 return ( -1 );
 return( (double) min / minday );
}

BOOL GetTransStartEndDates( int planet, int orbst, int orbed, JULIAN &jul, DATES &start, DATES &end, TIM &tstart, TIM &tend )
{
 int valid;
 double dsst, dsed, jstart, jend;
 JULIAN jx;
 dsst = GetDaySpan(planet, orbst, valid );
 dsed = GetDaySpan(planet, orbed, valid );
 if ( !valid )
	 return ( FALSE );
 jstart = jul.jd_1900 - dsst;
 jend = jul.jd_1900 + dsed;
 jx = jul;
 jx.jd_1900 = jstart;
 julian_update(&jx);
 julian_to_date_tim( &jx, &start, &tstart );
 jx.jd_1900 = jend;
 julian_update(&jx);
 julian_to_date_tim( &jx, &end, &tend );
}

