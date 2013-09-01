#include "stdafx.h"
#include <math.h>
#include "plans.h"
#include "julian.h"
#include "planpert.fns"
#include "utl.h"
#include "lngdbl.h"
extern TM_TYPE *jp;
extern T1_TYPE *j1p;

/* Perturbations for Uranus and Neptune */

void p_uranus( PLAN_ELE *p, DELTA *d, JULIAN *j )
{
 LONG_DOUBLE t, j1 = jp->j1;
 LONG_DOUBLE j10, j11, j12, tj12, sin_j11, cos_j11;
 LONG_DOUBLE sin_j4, cos_j4, sin_2j4, cos_2j4;

 make_j1( j, 0 );
 j10 = jp->j4 - jp->j2;
 j11 = jp->j4 - jp->j3;
 j12 = jp->j8 - jp->j4;
 tj12 = j12 + j12;
 sin_j11 = _sinl( j11 );
 cos_j11 = _cosl( j11 );
 t = jp->j4;
 sin_j4 = _sinl( t );
 cos_j4 = _cosl( t );
 t += t;
 sin_2j4 = _sinl( t );
 cos_2j4 = _sinl( t );

 t =  ( 8.64319e-1 - 1.583e-3 * j1 ) * j1p->sin_j9;
 t += ( 8.2222e-2 - 6.833e-3 * j1 ) * j1p->cos_j9;
 t += 3.6017e-2 * j1p->sin_2j9 - 3.019e-3 * j1p->cos_2j9;
 t += 8.122e-3 * _sinl(jp->j6);
 d->mean_long = d2r( t );

 t = 1.20303e-3 * j1p->sin_j9 + 6.197e-3 * j1p->sin_2j9;
 t += ( 1.9472e-2 - 9.47e-4 * j1 ) * j1p->cos_j9;
 d->perhelion = t;
 d->mean_anomaly = d->mean_long - ( d2r( t ) /(p+5)->eccentricity );

 t = ( 163.0 * j1 - 3349.0) * j1p->sin_j9 + 20981.0 * j1p->cos_j9;
 t += 1311.0 * j1p->cos_2j9;
 d->eccentricity = t * 1.0e-7;

 d->semi_major = 3.825e-3 * j1p->cos_j9;

 t =  ( 1.0122e-2 - 9.88e-4 * j1 ) * _sinl( jp->j4 + j11 );
 t += ( -3.8581e-2 + ( 2.031e-3 - 1.91e-3 * j1 ) * j1 ) * _cosl( jp->j4 + j1 );
 t += ( 3.4964e-2 - ( 1.038e-3 - 8.68e-4 * j1 ) * j1 ) *
	_cosl( jp->j4 + jp->j4 + j1 );
 t += 5.594e-3 * _sinl( jp->j4 + tj12 + j12 ) - 1.4808e-2 * _sinl( j10 );
 t -= 5.794e-3 * sin_j11 + 2.347e-3 * cos_j11;
 t += 9.872e-3 * _sinl( j12 ) + 8.303e-3 * _sinl( tj12 );
 t -= 4.308e-3 * _sinl( tj12 + j12 );
 d->longitude = t;

 t =  ( 4.58e-4 * sin_j11 - 6.42e-4 * cos_j11 - 5.17e-4 *
	_cosl( tj12 + tj12 ) ) * sin_j4;
 t -= ( 3.47e-4 * sin_j11 + 8.53e-4 * cos_j11 + 5.17e-4 *
	_sinl( 4.0 * j11 ) ) * cos_j4;
 t += 4.03e-4 * ( _cos( tj12 ) * sin_2j4 + _sinl( tj12 ) * cos_2j4 );
 d->helio_latitude = d2r( t );

 t = -25948.0 + 4985.0 * _cosl( j10 ) - 1230.0 * cos_j4;
 t += 3354.0 * cos_j11 + 904.0 * _cosl( tj12 );
 t += 894.0 * ( _cosl( j12 ) - _cosl( tj12 + j12 ) );
 t += ( 5795.0 * cos_j4 - 1165.0 * sin_j4 + 1388.0 * cos_2j4 ) * sin_j11;
 t += ( 1351.0 * cos_j4 + 5702.0 * sin_j4 + 1388.0 * sin_2j4) * cos_j11;
 d->radius_vector = t * 1.0e-6;

 return;
}


void p_neptune( PLAN_ELE *p, DELTA *d )
{
 double t, j1 = jp->j1;
 double j10, j11, j12, tj12, sin_2j12, cos_2j12;
 double cos_j8, sin_j8;

 j10 = jp->j8 - jp->j2;
 j11 = jp->j8 - jp->j3;
 j12 = jp->j8 - jp->j4;
 tj12 = j12 + j12;
 sin_2j12 = _sin( tj12 );
 cos_2j12 = _cos( tj12 );
 cos_j8 = _cos(jp->j8 );
 sin_j8 = _sin(jp->j8 );

 t =  ( 1.089e-3 * j1 - 5.89833e-1) * j1p->sin_j9;
 t += ( 4.658e-3 * j1 - 5.6094e-2 ) * j1p->cos_j9;
 t -= 2.4286e-2 * j1p->sin_2j9;
 d->mean_long = d2r( t );

 t =  2.4039e-2 * j1p->sin_j9 - 2.5303e-2 * j1p->cos_j9;
 t += 6.206e-3 * j1p->sin_2j9 - 5.992e-3 * j1p->cos_2j9;
 d->perhelion = t;
 d->mean_anomaly = d->mean_long - ( d2r( d->perhelion ) / (p+6)->eccentricity );

 t = 4389.0 * j1p->sin_j9 + 1129.0 * j1p->sin_2j9 + 4262.0 * j1p->cos_j9
      + 1089.0 * j1p->cos_2j9;
 d->eccentricity = t * 1.0e-7;

 d->semi_major = 1.0e-6 * ( 8189.0 * j1p->cos_j9 - 817.0 * j1p->sin_j9
      + 781.0 * j1p->cos_2j9 );

 t =  -9.556e-3 * _sin( j10 ) - 5.178e-3 * _sin( j11 ) + 2.572e-3 * sin_2j12;
 t -= 2.972e-3 * cos_2j12 * sin_j8 - 2.833e-3 * sin_2j12 * cos_j8;
 d->longitude = t;

 t = 3.36e-4 * cos_2j12 * sin_j8 + 3.64e-4 * sin_2j12 * cos_j8;
 d->helio_latitude = d2r( t );

 t =  -40596.0 + 4992.0 * _cos( j10 ) + 2744.0 * _cos( j11 ) +
      2044.0 * _cos( j12 ) + 1051.0 * cos_2j12;
 d->radius_vector = t * 1.0e-6;

 remove_j();
 remove_j1();
 return;
}
