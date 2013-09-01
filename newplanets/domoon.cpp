#include <stdafx.h>
#include <math.h>
/*
 * The entry, domoon(), is intended to be called by the AA.ARC
 * ephemeris calculator.  It calculates the Moon's apparent position.
 * Corrections for nutation and light time are included, but the
 * required routines for this are given as separate files in AA.ARC.
 * Note that the difference between Ephemeris and Universal
 * time is significant here, since the Moon moves in right
 * ascension at the rate of about 2s/minute.
 *
 */
#include "kep.h"

extern double aearth, au;

static double FAR ra;	/* Right Ascension */
static double FAR dec;	/* Declination */
double FAR Rearth;

/* Calculate geometric position of the Moon and apply
 * approximate corrections to find apparent position,
 * phase of the Moon, etc. for AA.ARC.
 */
int domoon(double TDT, double *moonpol)
{
int i, prtsav;
double ra0, dec0;
double x, y, z, lon0;
double pp[3], qq[3], pe[3], re[3], moonpp[3];

/* Geometric equatorial coordinates of the earth.  */
for (i = 0; i < 3; i++)
	re[i] = rearth[i];

/* Run the orbit calculation twice, at two different times,
 * in order to find the rate of change of R.A. and Dec.
 */

/* Calculate for 0.001 day ago
 */
//prtsav = prtflg;
//prtflg = 0; /* disable display */
moonll(TDT-0.001, moonpp, moonpol);
ra0 = ra;
dec0 = dec;
lon0 = moonpol[0];


/* Calculate for present instant.
 */
moonll(TDT, moonpp, moonpol);


/* Get apparent coordinates for the earth.  */
z = re[0] * re[0] + re[1] * re[1] + re[2] * re[2];
z = sqrt(z);
for (i = 0; i < 3; i++)
  re[i] /= z;
//annuab( re );  /* aberration of light.  */
/* pe[0] -= STR * (20.496/(RTS*pe[2])); */
precess( re, TDT, -1 );
nutate( TDT, re );
for (i = 0; i < 3; i++)
  re[i] *= z;
//lonlat( re, TDT, pe, 0 );
//prtflg = prtsav; /* reenable display */

/* Find sun-moon-earth angles */
for( i=0; i<3; i++ )
	qq[i] = re[i] + moonpp[i];
angles( moonpp, qq, re );

return(0);
}

/* Calculate apparent latitude, longitude, and horizontal parallax
 * of the Moon at Julian date J.
 */
int moonll(double J, double *rect, double *pol)
{
double cosB, sinB, cosL, sinL, y, z;
double qq[3], pp[3];
int i;

/* Compute obliquity of the ecliptic, coseps, and sineps.  */
epsiln( J );
/* Get geometric coordinates of the Moon.  */
gmoon (J, rect, pol);
/* Post the geometric ecliptic longitude and latitude, in radians,
 * and the radius in au.
 */
obpolar[0] = pol[0];
obpolar[1] = pol[1];
obpolar[2] = pol[2];

/* Light time correction to longitude,
 * about 0.7".
 */
pol[0] -= 0.0118 * DTR * Rearth / pol[2];

/* convert to equatorial system of date */
cosB = cos(pol[1]);
sinB = sin(pol[1]);
cosL = cos(pol[0]);
sinL = sin(pol[0]);
rect[0] = cosB*cosL;
rect[1] = coseps*cosB*sinL - sineps*sinB;
rect[2] = sineps*cosB*sinL + coseps*sinB;

/* Rotate to J2000. */
precess( rect, J, 1 );


/* Find Euclidean vectors and angles between earth, object, and the sun
 */
for( i=0; i<3; i++ )
	{
	pp[i] = rect[i] * pol[2];
	qq[i] = rearth[i] + pp[i];
	}
angles( pp, qq, rearth );

/* Make rect a unit vector.  */
/* for (i = 0; i < 3; i++) */
/*  rect[i] /= EO; */

/* Correct position for light deflection.
   (Ignore.)  */
/* relativity( rect, qq, rearth ); */

/* Aberration of light.
   The Astronomical Almanac (Section D, Daily Polynomial Coefficients)
   seems to omit this, even though the reference ephemeris is inertial.  */
/* annuab (rect); */

/* Precess to date.  */
precess( rect, J, -1 );

/* Correct for nutation at date TDT.
 */
nutate( J, rect );

/* Apparent geocentric right ascension and declination.  */
ra = zatan2(rect[0],rect[1]);
dec = asin(rect[2]);

/* For apparent ecliptic coordinates, rotate from the true
   equator into the ecliptic of date.  */
cosL = cos(eps+nuto);
sinL  = sin(eps+nuto);
y = cosL * rect[1] + sinL * rect[2];
z = -sinL * rect[1] + cosL * rect[2];
pol[0] = zatan2( rect[0], y );
pol[1] = asin(z);

/* Restore earth-moon distance.  */
for( i=0; i<3; i++ )
  rect[i] *= EO;

return(0);
}
