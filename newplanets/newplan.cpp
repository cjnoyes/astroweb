#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kep.h"
#include "astro.h"


void GetPlanetData( int plan, AS_INF *inf, double jul )
{
	double deg;
	int min;
	double rec[3], pol[3], hel[3], tdt, ut;
    update(tdt, jul, ut, 2);
	jul = tdt;
	switch ( plan ) {
	case SUN :
		initearth(jul);
		rec[1] = 0;
		dosun(jul,rec[0]);
		break;
	case MOON :
		domoon(jul,rec);
		break;
	default:
		dotheplanet( jul, plan-MERCURY, rec, pol, hel );
		break;
	}
 inf->declination = rec[1]*RTD;
 deg = rec[0]*RTD;
 inf->longitude = deg;
 min = deg*60.0;
 inf->minutes_total = min;
 inf->dec_minutes_total = abs(inf->declination * 60.0);
}
#ifdef PRO
void GetHelioPlanetData( int plan, AS_INF *inf, double jul )
{
 double deg;
 int min;
 double rec[3], pol[3], ecl[3], tdt, ut;
 update(tdt, jul, ut, 2);
 jul = tdt;
 dotheplanet( jul, plan-MERCURY, rec, pol, ecl );
 inf->declination = ecl[1]*RTD;
 deg = ecl[0]*RTD;
 inf->longitude = deg;
 min = deg*60.0;
 inf->minutes_total = min;
 inf->dec_minutes_total = abs(inf->declination * 60.0);
}
void ProcessFixedStar(double jul, int fix, AS_INF *inf)
{
 double rec[3], deg, tdt, ut;
 int min;
 update(tdt, jul, ut, 2);
 jul = tdt;
 dostar(fix, jul,rec );
 inf->declination = rec[1]*RTD;
 deg = rec[0]*RTD;
 inf->longitude = deg;
 min = deg*60.0;
 inf->minutes_total = min;
 min = rec[1] * RTD * 60.0;
 inf->dec_minutes_total = min;
}
#ifdef MESSIER
void ProcessMessier(double jul, int mes, AS_INF *inf)
{
 double rec[3], deg, tdt, ut;
 int min;
 update(tdt, jul, ut, 2);
 jul = tdt;
 domessier(mes, jul,rec );
 inf->declination = rec[1]*RTD;
 deg = rec[0]*RTD;
 inf->longitude = deg;
 min = deg*60.0;
 inf->minutes_total = min;
 min = rec[1] * RTD * 60.0;
 inf->dec_minutes_total = min;
}
#endif
#endif