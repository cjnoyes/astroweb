#include <stdafx.h>
#include <math.h>
/* Calculate and display apparent coordinates of the sun at the
 * time given by the external variables TDT, UT.
 * Before calling this routine, the geometric position of the
 * earth must be calculated and put in rearth[].
 */

#include "kep.h"
extern struct orbit earth;
extern double Rearth, aearth, au, Clightaud, Clight;

int initearth(double TDT )
{
 double rec[3];
 Clightaud = 86400.0 * Clight / au;
 Rearth = 0.001 * aearth / au;
 kepler( TDT, &earth, rearth, eapolar, rec );
 return( 1 );
}


int dosun(double TDT, double &rads)
{
double r, x, y, t;
double ecr[3], rec[3], pol[3], ecl[3];
int i;


/* Display ecliptic longitude and latitude.
 */
for( i=0; i<3; i++ )
	ecr[i] = -rearth[i];
r = eapolar[2];
lonlat( ecr, TDT, pol, 1 );


/* Philosophical note: the light time correction really affects
 * only the Sun's barymetric position; aberration is due to
 * the speed of the Earth.  In Newtonian terms the aberration
 * is the same if the Earth is standing still and the Sun moving
 * or vice versa.  Thus the following is actually wrong, but it
 * differs from relativity only in about the 8th decimal.
 * It should be done the same way as the corresponding planetary
 * correction, however.
 */
pol[2] = r;
for( i=0; i<2; i++ )
	{
	t = pol[2]/173.1446327;
/* Find the earth at time TDT - t */
	kepler( TDT-t, &earth, ecr, pol, ecl );
	}
r = pol[2];

for( i=0; i<3; i++ )
	{
	x = -ecr[i];
	y = -rearth[i];
	ecr[i] = x;	/* position t days ago */
	rec[i] = y;	/* position now */
	pol[i] = y - x; /* change in position */
	}

/* precess to equinox of date
 */
precess( ecr, TDT, -1 );

for( i=0; i<3; i++ )
    rec[i] = ecr[i];

/* Nutation.
 */
epsiln( TDT );
nutate( TDT, ecr );


/* Show it in ecliptic coordinates */

y  =  coseps * rec[1]  +  sineps * rec[2];
y = zatan2( rec[0], y ) + nutl;
rads = y;

return(0);
}
