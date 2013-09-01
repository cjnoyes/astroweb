// asteroidcalc.cpp: implementation of the asteroidcalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "astro.h"
#include "julian.h"
#include "utl.h"
#include "lngdbl.h"
#include "asteroiddata.h"
#include "errors.fns"
#include "compute.fns"
#include "asteroidcalc.h"
#include <math.h>
#include <stdio.h>

extern double kepler_eccentric_anomaly( double ma, double ecc);
extern double true_anomaly( double eccen, double kep );
extern double fmodulus( double num, double div );
extern double sun_longitude;
extern double sun_radius_vector;
extern double sun_mean_anomaly;
extern double retrograde( double prv, double pl, double el );
extern char *data_name( char * );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*

  class asteroidcalc  
{
asteroid_elements elements;
asteroid_data calc_data;
FILE *stream;
int count;
int index;
public:
	asteroidcalc();
	BOOL calcnext(int &min, int &ret);
	virtual ~asteroidcalc() { if (stream != NULL ) fclose( stream ); }

};

*/

asteroidcalc::asteroidcalc(JULIAN *j, LONG_DOUBLE sun)
{
 jul = j;
 char *p =data_name("asteroid.dat");
 stream = fopen(p,OPENMODEREAD);
 if ( stream == NULL ) {
    do_error(p);
	count=0;
	index=0;
	return;
  }
 earth_long = sun + PI;
 if ( earth_long > TWO_PI )
    earth_long -= TWO_PI;
 index = 0;
 fread((void *)&count,4,1,stream);
}

BOOL asteroidcalc::calcnext(short &min, short &retr, double &longt, double &lati)
{
	LONG_DOUBLE *a;
	if ( index < count ) {
	 ++index;
	 fread((void *)&elements,sizeof(asteroid_elements),1,stream);
     xmul(mean_anomaly);
	 xmul(eccentricity);
	 xmul(perhelion);
	 xmul(ascend_node);
	 xmul(inclination);
	 calc_data.semi_major = d2r(elements.semi_major);
     LONG_DOUBLE kepler = kepler_eccentric_anomaly( calc_data.mean_anomaly, calc_data.eccentricity);	
	 LONG_DOUBLE true_anom = true_anomaly( calc_data.eccentricity, kepler );
	 LONG_DOUBLE radius_vector =  calc_data.semi_major
		* ( 1.0 - calc_data.eccentricity* calc_data.eccentricity )
		/ ( 1.0 + calc_data.eccentricity * _cosl( true_anom ) );
	 LONG_DOUBLE longitude = true_anom + calc_data.perhelion;
	 LONG_DOUBLE lo = longitude - calc_data.ascend_node;
     LONG_DOUBLE cos_lo = _cosl( lo );
	 LONG_DOUBLE sin_lo = _sinl( lo );
     LONG_DOUBLE psi = _asinl( sin_lo * _sinl( calc_data.inclination ) );
     lati = r2d(psi);
	 LONG_DOUBLE lpd = _atanl( ( sin_lo * _cosl( calc_data.inclination )) / cos_lo ) + calc_data.ascend_node;
     if ( cos_lo < 0.0 )
		lpd += PI;
	 if ( lpd > TWO_PI )
		lpd -= TWO_PI;
	 else if ( lpd < 0.0)
		lpd += TWO_PI;
	 LONG_DOUBLE helio_long = lpd;
	 longt = r2d(lpd);
	 LONG_DOUBLE rpd = radius_vector * _cosl( psi );
     LONG_DOUBLE ll = lpd - earth_long;
     double xx = retrograde( radius_vector, lpd, earth_long );
     if ( xx < 0.0 )
        retr = 1;
     else if ( xx == 0.0 )
        retr = 2;
	 else
        retr = 0;
     LONG_DOUBLE aslongitude = _atanl( sun_radius_vector * _sinl( ll ) /
	    ( rpd - sun_radius_vector * _cosl( ll ) ) ) + lpd;
     aslongitude = fmodulus( aslongitude, TWO_PI );
	 if ( aslongitude < 0.0 )
		 aslongitude += TWO_PI;
     min = r2m( aslongitude ); 
    return true;
	}
	else
       return false;
}
