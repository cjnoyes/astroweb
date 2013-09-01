#include <stdafx.h>
#include <math.h>
/* The following program reduces the heliocentric equatorial
 * rectangular coordinates of the earth and object that
 * were computed by kepler() and produces apparent geocentric
 * right ascension and declination.
 */

#include "kep.h"

#define ELECT  8

static struct orbit *elements[ELECT] = {&mercury, &venus, &mars, &jupiter, &saturn, &uranus, &neptune, &pluto};


int reduce( struct orbit *elemnt, double *q, double *e, double *polar, double TDT )
{
double p[3], temp[3];
double a, b, s;
int i;

/* Save the geometric coordinates at TDT
 */
for( i=0; i<3; i++ )
	temp[i] = q[i];

lonlat( q, TDT, polar, 1 );


/* Find Euclidean vectors between earth, object, and the sun
 */
for( i=0; i<3; i++ )
	p[i] = q[i] - e[i];

angles( p, q, e );



/* Find unit vector from earth in direction of object
 */
for( i=0; i<3; i++ )
	{
	p[i] /= EO;
	temp[i] = p[i];
	}
/* Precession of the equinox and ecliptic
 * from J2000.0 to ephemeris date
 */
precess( p, TDT, -1 );

/* Ajust for nutation
 * at current ecliptic.
 */
epsiln( TDT );
nutate( TDT, p );

/* Geocentric ecliptic longitude and latitude.  */
 for( i=0; i<3; i++ )
	p[i] *= EO;
 lonlat( p, TDT, q, 0 );

return(0);
}


int doplanet(double TDT, struct orbit *eleobj, double *robj, double *ear, double *polar, double *eclip)
{
  /* calculate heliocentric position of the object */
  kepler( TDT, eleobj, robj, polar, eclip );
  /* apply correction factors and print apparent place */
  reduce( eleobj, robj, ear, polar, TDT );
  return 0;
}

int dotheplanet( double TDT, int i, double *robj, double *polar, double *eclip )
{
 return( doplanet( TDT, elements[i], robj, rearth, polar, eclip ) );
}