#include <stdafx.h>
#include <math.h>
/* This program reduces catalogue coordinates of a star
 * to apparent place.
 *
 * - S. L. Moshier, November, 1987
 */

#include "kep.h"

int rstar( struct star *el, double TDT, double *polar )
{
double p[3], q[3], e[3], m[3], temp[3];
double T, vpi, epoch;
double cosdec, sindec, cosra, sinra;
int i;


/* Convert from RA and Dec to equatorial rectangular direction
 */
loop:
cosdec = cos( el->dec );
sindec = sin( el->dec );
cosra = cos( el->ra );
sinra = sin( el->ra );
q[0] = cosra * cosdec;
q[1] = sinra * cosdec;
q[2] = sindec;

/* space motion */
vpi = 21.094952663 * el->v * el->px;
m[0] =    -el->mura * cosdec * sinra
	- el->mudec * sindec * cosra
	+       vpi * q[0];

m[1] =     el->mura * cosdec * cosra
	- el->mudec * sindec * sinra
	+       vpi * q[1];

m[2] =    el->mudec * cosdec
	+       vpi * q[2];

epoch = el->epoch;

/* Convert FK4 to FK5 catalogue */

if( epoch == B1950 )
	{
	fk4fk5( q, m, el );
	goto loop;
	}


for( i=0; i<3; i++ )
	e[i] = rearth[i];

/* precess the earth to the star epoch */
precess( e, epoch, -1 );

/* Correct for proper motion and parallax
 */
T = (TDT - epoch)/36525.0;
for( i=0; i<3; i++ )
	{
	p[i] = q[i]  +  T * m[i]  -  el->px * e[i];
	}

/* precess the star to J2000 */
precess( p, epoch, 1 );
/* reset the earth to J2000 */
for( i=0; i<3; i++ )
	e[i] = rearth[i];

/* Find Euclidean vectors between earth, object, and the sun
 * angles( p, q, e );
 */
angles( p, p, e );

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
for( i=0; i<3; i++ )
	p[i] *= EO;
lonlat( p, TDT, polar, 0 );

return(0);
}


int dostar(int i, double j, double *pol )
{
 struct star elements;
 elements = fixedstarelements[i];
 rstar( &elements, j, pol );
 return( 1 );
}
#ifdef MESSIER
int domessier(int i, double j, double *pol )
{
 struct star elements;
 elements = messierelements[i];
 rstar( &elements, j, pol );
 return( 1 );
}
#endif