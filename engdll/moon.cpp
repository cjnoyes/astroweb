#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "astro.h"
#include "utl.h"
#include "julian.h"
#include "lngdbl.h"

extern double fmodulus( double n, double div );
extern void julian_calc( DATES *d, int h, int m, JULIAN *j );

/* uses 88+[6] double consts = 704+[48] bytes */

/* Macros used to reduce source code and make it easier to follow */
/* note the parens in the next two macros should be unbalanced */
#define muxt2(x)      ((LONG_DOUBLE) (x) * t ) ) * t2 )
#define muxt(x)       ((LONG_DOUBLE) (x) * t ) ) * t )

#define muabn(a,b)    ( (a) * sin_a ) + sin_b + ( (b) * sin_as_node )

static double cns[6] = {  2.732158213e1,  3.652596407e2,
				2.755455094e1,  2.953058868e1,
				2.721222039e1,  6.798363307e3  };


/* Function to get the moon's minutes, taking a pointer to the julian
   date structure, and returns the asc_node via a pointer.

   Routine comes from "Astronomy With Your Personal Computer".

   Variable name Aliases to book:

	mean_long..........LD   mean_anom_sun.....MS
	mean_anom_moon.....MD   mean_elong_moon...DE
	mean_dst_as_node...DE   ascend_node........N
	sin_as_node........SN   sin_a..............SA
	sin_b..............SB   sin_c..............SC
	m[0]...............M1   m[1]...............M2
	m[2]...............M3   m[3]...............M4
	m[4]...............M5   m[5]...............M6
	longitude..........L    longitude_moon.....LAM  */

MINU moon_minutes( JULIAN *j, MINU *asc_node )
{
 int i;
 LONG_DOUBLE mean_long, mean_anom_moon, mean_anom_sun, mean_elong_moon;
 LONG_DOUBLE tms, tde, tf, tmd, fde, a, sin_a;
 LONG_DOUBLE m[6], ascend_node, mean_dst_as_node;
 LONG_DOUBLE b, sin_b, c, sin_c, sin_as_node, de_m_f_x2, de_m_ms_x2;
 LONG_DOUBLE f_p_de_x2, fde_m_ms, md_p_de, md_p_tde;
 double t = j->jd_cent, djd = j->jd_1900, t2 = j->jd_cent_sq;
 LONG_DOUBLE e, e2, longitude, longitude_moon;

 for ( i = 0; i < 6; ++i ) {
     m[i] = djd / cns[i];
     m[i] = murem(m[i]);
     }
 /* Note: Parens are balanced in macro */
 mean_long = 2.70434164e2 + m[0] - ( ( 1.133e-3 - muxt2( 1.9e-6 );
 mean_anom_sun = 3.58475833e2 + m[1] - ( ( 1.5e-4 + muxt2( 3.3e-6 );
 mean_anom_moon = 2.96104608e2 + m[2] + ( ( 9.192e-3 + muxt2( 1.44e-5 );
 mean_elong_moon = 3.50737486e2 + m[3] - ( ( 1.436e-3 - muxt2( 1.9e-6 );
 mean_dst_as_node = 11.250889 + m[4] - ( ( 3.211e-3 - muxt2( 3.0e-7 );
 ascend_node = 2.59183275e2 - m[5] + ( ( 2.078e-3 + muxt2( 2.2e-5 );

 sin_as_node = _sinl( d2r( ascend_node ) );
 a = d2r( 51.2 + ( 20.2 * t ) );
 sin_a = _sinl( a );

 /* Note: Parens on next line are balanced in macro */
 b = 346.56 + ( ( 132.87 - muxt( 9.1731e-3 );

 sin_b = 3.964e-3 * _sinl( d2r( b ) );
 c = d2r( ascend_node + 275.05 - ( 2.3 * t ) );
 sin_c = _sinl( c );
 mean_anom_sun -= 1.778e-3 * sin_a;
 mean_dst_as_node += sin_b - ( 2.4691e-2 * sin_as_node ) - ( 4.328e-3 * sin_c );


 /* Note: Parens on next line is balanced in macro */
 e = 1 - ( ( 2.495e-3 + muxt( 7.52e-6 );

 ascend_node  = 60.0 * fmodulus( ascend_node, 360.0 );
 *asc_node = (MINU) ascend_node;

 mean_anom_sun = d2r( mean_anom_sun );
 mean_dst_as_node = d2r( mean_dst_as_node );
 mean_anom_moon += muabn( 8.17e-4, 2.541e-3 );
 mean_anom_moon = d2r( mean_anom_moon );
 mean_long += muabn( 2.33e-4, 1.964e-3 );
 mean_long = d2r( mean_long );
 mean_elong_moon += muabn( 2.011e-3, 1.964e-3 );
 mean_elong_moon = d2r( mean_elong_moon );

 /* These are multiplications by two or four */
 tms = mean_anom_sun + mean_anom_sun;         /* 2.0 * mean_anom_sun */
 tde = mean_elong_moon + mean_elong_moon;     /* 2.0 * mean_elong_moon  */
 tmd = mean_anom_moon + mean_anom_moon;       /* 2.0 * mean_anom_moon   */
 tf  = mean_dst_as_node + mean_dst_as_node;   /* 2.0 * mean_dst_as_node */
 fde = tde + tde;                             /* 4.0 * mean_elong_moon  */
 e2  = e * e;

 /* Equivalent temporary variables for calculations. */
 de_m_f_x2  = tde - tf;   /* 2.0 * ( mean_elong_moon - mean_dst_as_node ) */
 de_m_ms_x2 = tde - tms;  /* 2.0 * ( mean_elong_moon - mean_anom_sun ) */
 f_p_de_x2  = tf + tde;   /* 2.0 * ( mean_dst_as_node + mean_elong_moon ) */
 fde_m_ms   = fde - mean_anom_sun;
 md_p_de    = mean_anom_moon + mean_elong_moon;
 md_p_tde   = mean_anom_moon + tde;

 longitude = ( 6.28875 * _sinl( mean_anom_moon ) )
   + ( 1.274018 * _sinl( tde - mean_anom_moon ) );
 longitude += (6.58309e-1 * _sinl( tde )) + (2.13616e-1 * _sinl( tmd ) );
 longitude -= ( e * 1.85596e-1 * _sinl( mean_anom_sun ) );
 longitude -= 1.14336e-1 * _sinl( tf );
 longitude += ( 5.8793e-2 * _sinl( 2.0 * ( mean_elong_moon - mean_anom_moon ) ) )
   +  ( 5.7212e-2 * e * _sinl( tde - mean_anom_sun - mean_anom_moon ) );
 longitude += ( 5.332e-2 * _sinl( md_p_tde ) )
   +  ( 4.5874e-2 * e * _sinl( tde - mean_anom_sun ) )
   +  ( 4.0124e-2 * e * _sinl( mean_anom_moon - mean_anom_sun ) );
 longitude -= ( 3.4718e-2 * _sinl( mean_elong_moon ) )
   -  ( e * 3.0465e-2 * _sinl( mean_anom_sun + mean_anom_moon ) );
 longitude += 1.5326e-2 * _sinl( de_m_f_x2 );
 longitude -= 1.2528e-2 * _sinl( tf + mean_anom_moon );
 longitude -= 1.098e-2 * _sinl( tf - mean_anom_moon );
 longitude += 1.0674e-2 * _sinl( fde - mean_anom_moon )
   +  ( 1.0034e-2 * _sinl( tmd + mean_anom_moon  ) )
   +  ( 8.548e-3 * _sinl( fde - tmd ) );
 longitude -= e * 7.91e-3 * _sinl( mean_anom_sun - md_p_tde );
 longitude -= e * 6.783e-3 * _sinl( tde + mean_anom_sun );
 longitude += 5.162e-3 * _sinl( mean_anom_moon - mean_elong_moon )
   +  ( e * 5e-3 * _sinl( mean_anom_sun + mean_elong_moon ) )
   +  ( 3.862e-3 * _sinl( fde ) )
   +  ( e * 4.049e-3 * _sinl( md_p_tde - mean_anom_sun ) );
 longitude += 3.996e-3 * _sinl( 2.0 * ( md_p_de ) )
   +  ( 3.665e-3 * _sinl( tde - tmd + mean_anom_moon ) )
   +  ( e * 2.695e-3 * _sinl( tmd - mean_anom_sun ) );
 longitude += 2.602e-3 * _sinl( mean_anom_moon - f_p_de_x2 );
 longitude += e * 2.396e-3 * _sinl( 2.0 * (mean_elong_moon - mean_anom_moon)
	- mean_anom_sun );
 longitude -= 2.349e-3 * _sinl( md_p_de );
 longitude += e2 * 2.249e-3 * _sinl( de_m_ms_x2 );
 longitude -= e * 2.125e-3 * _sinl( tmd + mean_anom_sun );
 longitude -= e2 * 2.079e-3 * _sinl( tms );
 longitude += e2 * 2.059e-3 * _sinl( de_m_ms_x2 - mean_anom_moon );
 longitude -= 1.773e-3 * _sinl( mean_anom_moon + de_m_f_x2 );
 longitude -= 1.595e-3 * _sinl( f_p_de_x2 );
 longitude += e * 1.22e-3 * _sinl( fde_m_ms - mean_anom_moon );
 longitude -= 1.11e-3 * _sinl( 2.0 * ( mean_anom_moon + mean_dst_as_node ) );
 longitude += 8.92e-4 * _sinl( mean_anom_moon - tde + mean_elong_moon );
 longitude -= e * 8.11e-4 * _sinl( mean_anom_sun + md_p_tde );
 longitude += e * 7.61e-4 * _sinl( fde_m_ms - tmd )
   +  ( e2 * 7.04e-4 * _sinl( mean_anom_moon - 2.0 * ( mean_anom_sun + mean_elong_moon ) ) );
 longitude += e * 6.93e-4 * _sinl( mean_anom_sun - 2.0 * ( mean_anom_moon - mean_elong_moon ) )
   +  ( e * 5.98e-4 * _sinl( de_m_f_x2 - mean_anom_sun ) );
 longitude += 5.5e-4 * _sinl( mean_anom_moon + fde ) + 5.38e-4 * _sin( tmd + tmd );  /* 4 * mean_anom_moon */
 longitude += e * 5.21e-4 * _sinl( fde_m_ms )
   +  ( 4.86e-4 * _sinl( tmd - mean_elong_moon ) )
   +  ( e2 * 7.17e-4 * _sinl( mean_anom_moon - tms ) );
 longitude_moon = mean_long + d2r( longitude );
 longitude_moon = fmodulus( longitude_moon, TWO_PI );
 return( r2m( longitude_moon ) );
}
