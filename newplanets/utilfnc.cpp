#include <stdafx.h>
#include <math.h>
#include "kep.h"

/* Reduce x modulo 360 degrees
 */
double mod360(double x)
{
long k;
double y;

k = (long) (x/360.0);
y = x  -  k * 360.0;
while( y < 0.0 )
	y += 360.0;
while( y > 360.0 )
	y -= 360.0;
return(y);
}




/* Reduce x modulo 2 pi
 */
#define TPI (2.0*PI)
double modtp(double x)
{
double y;

y = floor( x/TPI );
y = x - y * TPI;
while( y < 0.0 )
	y += TPI;
while( y >= TPI )
	y -= TPI;
return(y);
}


/* Get operator to type in hours, minutes, and seconds
 */
static int hours = 0;
static int minutes = 0;
static double seconds = 0.0;


/*
 * Convert change in rectangular coordinatates to change
 * in right ascension and declination.
 * For changes greater than about 0.1 degree, the
 * coordinates are converted directly to R.A. and Dec.
 * and the results subtracted.  For small changes,
 * the change is calculated to first order by differentiating
 *   tan(R.A.) = y/x
 * to obtain
 *    dR.A./cos**2(R.A.) = dy/x  -  y dx/x**2
 * where
 *    cos**2(R.A.)  =  1/(1 + (y/x)**2).
 *
 * The change in declination arcsin(z/R) is
 *   d asin(u) = du/sqrt(1-u**2)
 *   where u = z/R.
 *
 * p0 is the initial object - earth vector and
 * p1 is the vector after motion or aberration.
 *
 */

int deltap( double *p0, double *p1, double *dr, double *dd )
{
double dp[3], A, B, P, Q, x, y, z;
int i;

P = 0.0;
Q = 0.0;
z = 0.0;
for( i=0; i<3; i++ )
	{
	x = p0[i];
	y = p1[i];
	P += x * x;
	Q += y * y;
	y = y - x;
	dp[i] = y;
	z += y*y;
	}

A = sqrt(P);
B = sqrt(Q);

if( (A < 1.e-7) || (B < 1.e-7) || (z/(P+Q)) > 5.e-7 )
	{
	P = zatan2( p0[0], p0[1] );
	Q = zatan2( p1[0], p1[1] );
	Q = Q - P;
	while( Q < -PI )
		Q += 2.0*PI;
	while( Q > PI )
		Q -= 2.0*PI;
	*dr = Q;
	P = asin( p0[2]/A );
	Q = asin( p1[2]/B );
	*dd = Q - P;
	return(0);
	}


x = p0[0];
y = p0[1];
if( x == 0.0 )
	{
	*dr = 1.0e38;
	}
else
	{
	Q = y/x;
	Q = (dp[1]  -  dp[0]*y/x)/(x * (1.0 + Q*Q));
	*dr = Q;
	}

x = p0[2]/A;
P = sqrt( 1.0 - x*x );
*dd = (p1[2]/B - x)/P;
return(0);
}
