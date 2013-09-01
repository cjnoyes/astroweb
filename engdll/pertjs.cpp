#include "stdafx.h"
#include <math.h>
#include "astro.h"
#include "utl.h"
#include "julian.h"
#include "plans.h"
#include "planpert.fns"
#include "lngdbl.h"

extern TM_TYPE *jp;
extern T1_TYPE *j1p;

/* local static declarations */
static void p_jup_mean_long( DELTA *d );
static void p_jup_eccen( DELTA *d );
static void p_jup_perh_ma( PLAN_ELE *p, DELTA *d );
static void p_sat_ml( PLAN_ELE *p, DELTA *d );
static void p_sat_eccen( DELTA *d );
static void p_sat_perh_ma( PLAN_ELE *p, DELTA *d );
static void sat_semi_maj( DELTA *d );


/* Perturbations for Jupiter and Saturn */

static void p_jup_mean_long( DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t =  ( 3.31364e-1 - ( 1.0281e-2 + 4.692e-3 * j1 ) * j1 ) * j1p->sin_j5;
 t += ( 3.228e-3 - ( 6.4436e-2 - 2.075e-3 * j1 ) * j1 ) * j1p->cos_j5;
 t -= ( 3.083e-3 + ( 2.75e-4 - 4.89e-4 * j1 ) * j1 ) * j1p->sin_2j5;
 t += ( 2.472e-3 * j1p->sin_j6 + 1.3619e-2 * j1p->sin_j7 )
   +  ( 1.8472e-2 * j1p->sin_2j7 + 6.717e-3 * j1p->sin_3j7 );
 t += 2.775e-3 * j1p->sin_4j7 + 6.417e-3 * j1p->sin_2j7 * j1p->sin_j3;
 t += (( 7.275e-3 - 1.253e-3 * j1 ) * j1p->sin_j7 * j1p->sin_j3 )
   +  ( 2.439e-3 * j1p->sin_3j7 * j1p->sin_j3 );
 t -= ( 3.5681e-2 + 1.208e-3 * j1 ) * j1p->sin_j7 * j1p->cos_j3;
 t -= 3.767e-3 * j1p->cos_2j7 * j1p->sin_j3;
 t -= ( 3.3839e-2 + 1.125e-3 * j1 ) * j1p->cos_j7 * j1p->sin_j3;
 t -= 4.261e-3 * j1p->sin_2j7 * j1p->cos_j3;
 t += (( 1.161e-3 * j1 - 6.333e-3 ) * j1p->cos_j7 * j1p->cos_j3 )
   +  ( 2.178e-3 * j1p->cos_j3 - 6.675e-3 * j1p->cos_2j7 * j1p->cos_j3 );
 t -= 2.664e-3 * j1p->cos_3j7 * j1p->cos_j3 - 2.572e-3 *
	j1p->sin_j7 * j1p->sin_2j3;
 t -= 3.567e-3 * j1p->sin_2j7 * j1p->sin_2j3 + 2.094e-3 *
	j1p->cos_j7 * j1p->cos_2j3;
 t += 3.342e-3 * j1p->cos_2j7 * j1p->cos_2j3;
 d->mean_long = d2r( t );
 return;
}

static void p_jup_eccen( DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = ( 3606.0 + (130.0 - 43.0 * j1 ) * j1 ) * j1p->sin_j5;
 t += ( 1289.0 - 580.0 * j1 ) * j1p->cos_j5 - 6764.0 * j1p->sin_j7 * j1p->sin_j3;
 t -= ( 1110.0 * j1p->sin_2j7 * j1p->sin_j3 - 224.0 * j1p->sin_3j7 *
	j1p->sin_j3 - 204.0 * j1p->sin_j3 );
 t += ( 1284.0 + 116.0 * j1 ) * j1p->cos_j7 * j1p->sin_j3 + 188.0 *
	j1p->cos_2j7 * j1p->sin_j3;
 t += ( 1460.0 + 130.0 * j1 ) * j1p->sin_j7 * j1p->cos_j3 + 224.0 *
	j1p->sin_2j7 * j1p->cos_j3;
 t -= 817.0 * j1p->cos_j3 + 6074.0 * j1p->cos_j3 * j1p->cos_j7 + 992.0 *
	j1p->cos_2j7 * j1p->cos_j3;
 t += 508.0 * j1p->cos_3j7 * j1p->cos_j3 + 230.0 * j1p->cos_4j7 *
	j1p->cos_j3 + 108.0 * j1p->cos_5j7 * j1p->cos_j3;
 t -= ( 956.0 + 73.0 * j1 ) * j1p->sin_j7 * j1p->sin_2j3 + 448.0 *
	j1p->sin_2j7 * j1p->sin_2j3;
 t += 137.0 * j1p->sin_3j7 * j1p->sin_2j3 + ( 108.0 * j1 - 997.0 ) *
	j1p->cos_j7 * j1p->sin_2j3;
 t += 480.0 * j1p->cos_2j7 * j1p->sin_2j3 + 148.0 * j1p->cos_3j7 *
	j1p->sin_2j3;
 t += ( 99.0 * j1 - 956.0 ) * j1p->sin_j7 * j1p->cos_2j3 + 490.0 *
	j1p->sin_2j7 * j1p->cos_2j3;
 t += ( 158.0 * j1p->sin_3j7 * j1p->cos_2j3 + 179.0 * j1p->cos_2j3 )
   +  (( 1024.0 + 75.0 * j1 ) * j1p->cos_j7 * j1p->cos_2j3 - 437.0 *
	j1p->cos_2j7 * j1p->cos_2j3 );
 t -= 132.0 * j1p->cos_3j7 * j1p->cos_2j3;
 d->eccentricity = t * 1.0e-7;
}

static void p_jup_perh_ma( PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = ( 7.192e-3 - 3.147e-3 * j1 ) * j1p->sin_j5 - 4.344e-3 * j1p->sin_j3;
 t += (( j1 * ( 1.97e-4 * j1 - 6.75e-4 ) - 2.0428e-2 ) * j1p->cos_j5 )
   +  ( 3.4036e-2 * j1p->cos_j7 * j1p->sin_j3 );
 t += ( 7.269e-3 + 6.72e-4 * j1 ) * j1p->sin_j7 * j1p->sin_j3;
 t += 5.614e-3 * j1p->cos_2j7 * j1p->sin_j3 + 2.964e-3 *
	j1p->cos_3j7 * j1p->sin_j3;
 t += 3.7761e-2 * j1p->sin_j7 + j1p->cos_j3 + 6.158e-3 *
	j1p->sin_2j7 * j1p->cos_j3;
 t -= 6.603e-3 * j1p->cos_j7 * j1p->cos_j3 - 5.356e-3 *
	j1p->sin_j7 * j1p->sin_2j3;
 t += 2.722e-3 * j1p->sin_2j7 * j1p->sin_2j3 + 4.483e-3 *
	j1p->cos_j7 * j1p->sin_2j3;
 t -= 2.642e-3 * j1p->cos_2j7 * j1p->sin_2j3 + 4.403e-3 *
	j1p->sin_j7 * j1p->cos_2j3;
 t -= 2.536e-3 * j1p->sin_2j7 * j1p->cos_2j3 + 5.547e-3 *
	j1p->cos_j7 * j1p->cos_2j3;
 t -= 2.689e-3 * j1p->cos_2j7 * j1p->cos_2j3;
 d->perhelion = t;
 d->mean_anomaly = d->mean_long - ( d2r(t ) / (p+3)->eccentricity );
 d->mean_anomaly *= 1.0e-2;
}

void p_jupiter( JULIAN *j, PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t;

 if ( !make_j1(j, JUPITER ) )
    return;
 p_jup_mean_long( d );
 p_jup_eccen( d );
 p_jup_perh_ma( p, d );

 t =  205.0 * j1p->cos_j7 - 263.0 * j1p->cos_j5 + 693.0 * j1p->cos_2j7 +
	312.0 * j1p->cos_3j7;
 t += 147.0 * j1p->cos_4j7 + 299.0 * j1p->sin_j7 * j1p->sin_j3 + 181.0 *
	j1p->cos_2j7 * j1p->sin_j3;
 t += 204.0 * j1p->sin_2j7 * j1p->cos_j3 + 111.0 * j1p->sin_3j7 *
	j1p->cos_j3 - 337.0 * j1p->cos_j7 * j1p->cos_j3;
 t -= 111.0 * j1p->cos_2j7 * j1p->cos_j3;
 d->semi_major = t * 1.0e-6;
}

static void p_sat_ml( PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = 7.581e-3 * j1p->sin_2j5 - 7.986e-3 *j1p->sin_j6;
 t -= 1.48811e-1 * j1p->sin_j7 - 4.0786e-2 * j1p->sin_2j7;
 t -= ( 8.14181e-1 - ( 1.815e-2 - 1.6714e-2 * j1 ) * j1 ) * j1p->sin_j5;
 t -= ( 1.0497e-2 - ( 1.60906e-1 - 4.1e-3 * j1 ) * j1 ) * j1p->cos_j5;
 t -= 1.5208e-2 * j1p->sin_3j7 - 6.339e-3 * j1p->sin_4j7;
 t -= 6.244e-3 * j1p->sin_j3 - 1.65e-2 * j1p->sin_2j7 * j1p->sin_j3;
 t += ( 8.931e-3 + 2.728e-3 * j1 ) * j1p->sin_j7* j1p->sin_j3;
 t -= 5.775e-3 * j1p->sin_3j7 * j1p->sin_j3;
 t += (( 8.1344e-2 + 3.026e-3 * j1 ) * j1p->cos_j7 * j1p->sin_j3 )
   +  (1.5019e-2 * j1p->cos_2j7 * j1p->sin_j3 );
 t += (( 8.5581e-2 + 2.494e-3 * j1 ) * j1p->sin_j7 * j1p->cos_j3 )
   +  ( 1.4394e-2 *j1p->cos_2j7 * j1p->cos_j3 );
 t += ( 2.5328e-2 - 3.117e-3 * j1 ) * j1p->cos_j7 * j1p->cos_j3;
 t += 6.319e-3 * j1p->cos_3j7 + 6.369e-3 * j1p->sin_j7 * j1p->sin_2j3;
 t += 9.156e-3 * j1p->sin_2j7 * j1p->sin_2j3 + 7.525e-3 *
	j1p->sin_3j8 * j1p->sin_2j3;
 t -= 5.236e-3 * j1p->cos_j7 * j1p->cos_2j3 - 7.736e-3 *
	j1p->cos_2j7 * j1p->cos_2j3;
 t -= 7.528e-3 * j1p->cos_3j8 * j1p->cos_2j3;
 d->mean_long = d2r( t );
}

static void p_sat_eccen( DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = ( -7927.0 + ( 2548.0 + 91.0 * j1 ) * j1 ) * j1p->sin_j5;
 t += ( ( 13381.0 + ( 1226.0 - 253.0 * j1 ) * j1 ) * j1p->cos_j5 )
   +  ( ( 248.0 - 121.0 * j1 ) * j1p->sin_2j5 );
 t -= ( 305.0 + 91.0 * j1 ) * j1p->cos_2j5 + 412.0 * j1p->sin_2j7;
 t += 12415.0 * j1p->sin_j3 + ( 390.0 - 617.0 * j1 ) *
	j1p->sin_j7 * j1p->sin_j3;
 t += (( 165.0 - 204.0 * j1 ) * j1p->sin_2j7 * j1p->sin_j3 )
   +  ( 26599.0 * j1p->cos_j7 * j1p->sin_j3 - 4687.0 *
	j1p->cos_2j7 * j1p->sin_j3 );
 t -= 1870.0 * j1p->cos_3j7 * j1p->sin_j3 - 821.0 *
	j1p->cos_4j7 * j1p->sin_j3;
 t -= 377.0 * j1p->cos_5j7 * j1p->sin_j3 + 497.0 *
	j1p->cos_2j8 * j1p->sin_j3;
 t += ( 163.0 - 611.0 * j1) * j1p->cos_j3 - 12696.0 *
	j1p->sin_j7 * j1p->cos_j3;
 t -= 4200.0 * j1p->sin_2j7 * j1p->cos_j3 - 1503.0 *
	j1p->sin_3j7 * j1p->cos_j3;
 t -= 619.0 * j1p->sin_4j7 * j1p->cos_j3 - 268.0 *
	j1p->sin_5j7 * j1p->cos_j3;
 t -= ( 282.0 + 1306.0 * j1 ) * j1p->cos_j7 * j1p->cos_j3;
 t += ( ( -86.0 + 230.0 * j1 ) * j1p->cos_2j7 * j1p->cos_j3 )
   +  ( 461.0 * j1p->sin_2j8 * j1p->cos_j3 - 350.0 * j1p->sin_2j3 )
   +  ( ( 2211.0 - 286.0 * j1 ) * j1p->sin_j7 * j1p->sin_2j3 );
 t -= 2208.0 * j1p->sin_2j7 * j1p->sin_2j3 - 568.0 *
	j1p->sin_3j7 * j1p->sin_2j3;
 t -= 346.0 * j1p->sin_4j7 * j1p->sin_2j3 - ( 2780.0 + 222.0 * j1 ) *
	j1p->cos_j7 * j1p->sin_2j3;
 t += ( 2022.0 + 263.0 * j1 ) * j1p->cos_2j7 * j1p->sin_2j3 + 248.0 *
	j1p->cos_3j7 * j1p->sin_2j3;
 t += 242.0 * j1p->sin_3j8 * j1p->sin_2j3 + 467.0 * j1p->cos_3j8 *
	j1p->sin_2j3 - 490.0 * j1p->cos_2j3;
 t -= ( 2842.0 + 279.0 * j1 ) * j1p->sin_j7 * j1p->cos_2j3;
 t += ( 128.0 + 226.0 * j1 ) * j1p->sin_2j7 * j1p->cos_2j3 + 224.0 *
	j1p->sin_3j7 * j1p->cos_2j3;
 t += (( -1594.0 + 282.0 * j1 ) * j1p->cos_j7 * j1p->cos_2j3 )
   +  (( 2162.0 - 207.0 * j1 ) * j1p->cos_2j7 * j1p->cos_2j3 );
 t += ( 561.0 * j1p->cos_3j7 * j1p->cos_2j3 + 343.0 *
	j1p->cos_4j7 * j1p->cos_2j3 )
   +  ( 469.0 * j1p->sin_3j8 * j1p->cos_2j3 - 242.0 *
	j1p->cos_3j8 * j1p->cos_2j3 );
 t -= 205.0 * j1p->sin_j7 * j1p->sin_3j3 + 262.0 * j1p->sin_3j7 *
	j1p->sin_3j3 + 208.0 * j1p->cos_j7 * j1p->cos_3j3;
 t -= 271.0 * j1p->cos_3j7 * j1p->cos_3j3 - 382.0 *
	j1p->cos_3j7 * j1p->sin_4j3;
 t -= 376.0 * j1p->sin_3j7 * j1p->cos_4j3;
 d->eccentricity = t * 1.0e-7;
}

static void p_sat_perh_ma( PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = ( 7.7108e-2 + ( 7.186e-3 - 1.533e-3 * j1 ) * j1 ) *
	j1p->sin_j5 - 7.075e-3 * j1p->sin_j7;
 t += ( 4.5803e-2 - ( 1.4766e-2 + 5.36e-4 * j1 ) * j1 ) *
	j1p->cos_j5 - 7.2586e-2 * j1p->cos_j3;
 t -= 7.5825e-2 * j1p->sin_j7 * j1p->sin_j3 - 2.4839e-2 *
	j1p->sin_2j7 * j1p->sin_j3;
 t -= 8.631e-3 * j1p->sin_3j7 * j1p->sin_j3 - 1.50383e-1 *
	j1p->cos_j7 * j1p->cos_j3;
 t += 2.6897e-2 * j1p->cos_2j7 * j1p->cos_j3 + 1.0053e-2 *
	j1p->cos_3j7 * j1p->cos_j3;
 t -= ( 1.3597e-2 + 1.719e-3 * j1 ) * j1p->sin_j7 * j1p->sin_2j3;
 t += 1.1981e-2 * j1p->sin_2j7 * j1p->cos_2j3;
 t -= ( 7.742e-3 - 1.517e-3 * j1 ) * j1p->cos_j7 * j1p->sin_2j3;
 t += ( 1.3586e-2 - 1.375e-3 * j1 ) * j1p->cos_2j7 * j1p->cos_2j3;
 t -= ( 1.3667e-2 - 1.239e-3 * j1 ) * j1p->sin_j7 * j1p->cos_2j3;
 t += ( 1.4861e-2 + 1.136e-3 * j1 ) * j1p->cos_j7 * j1p->cos_2j3;
 t -= ( 1.3064e-2 + 1.628e-3 * j1) * j1p->cos_2j7 * j1p->cos_2j3;
 d->perhelion = t;
 d->mean_anomaly = d->mean_long - ( d2r(t) / (p+4)->eccentricity);
}

static void p_sat_semi_maj( DELTA *d )
{
 LONG_DOUBLE t, j1 = jp->j1;

 t = 572.0 * j1p->sin_j5 - 1590.0 * j1p->sin_2j7 * j1p->cos_j3 +
	2933.0 * j1p->cos_j5;
 t -= 647.0 * j1p->sin_3j7 * j1p->cos_j3 + 33629.0 * j1p->cos_j7 - 344.0 *
	j1p->sin_4j7 * j1p->cos_j3;
 t -= 3081.0 * j1p->cos_2j7 + 2885.0 * j1p->cos_j7 * j1p->cos_j3 -
	1423.0 * j1p->cos_3j7;
 t += (2172.0 + 102.0 * j1) * j1p->cos_2j7 * j1p->cos_j3 -
	671.0 * j1p->cos_4j7;
 t += 296.0 * j1p->cos_3j7 * j1p->cos_j3 - 320.0 * j1p->cos_5j7 -
	267.0 * j1p->sin_2j7 * j1p->sin_2j3;
 t += 1098.0 * j1p->sin_j3 - 778.0 * j1p->cos_j7 * j1p->sin_2j3 -
	2812.0 * j1p->sin_j7 * j1p->sin_j3;
 t += 495.0 * j1p->cos_2j7 * j1p->sin_2j3 + 688.0 * j1p->sin_2j7 * j1p->sin_j3;
 t += 250.0 * j1p->cos_3j7 * j1p->sin_2j3 - 393.0 * j1p->sin_3j7 * j1p->sin_j3;
 t -= 856.0 * j1p->sin_j7 * j1p->cos_2j3 - 228.0 * j1p->sin_4j7 *
	j1p->sin_j3 + 441.0 * j1p->sin_2j7 * j1p->cos_2j3;
 t += 2138.0 * j1p->cos_j7 * j1p->sin_j3 + 296.0 * j1p->cos_2j7 *
	j1p->cos_2j3 - 999.0 * j1p->cos_2j7 * j1p->sin_j3;
 t += 211.0 * j1p->cos_3j7 * j1p->cos_2j3 - 642.0 * j1p->cos_3j7 *
	j1p->sin_j3 - 427.0 * j1p->sin_j7 * j1p->sin_3j3;
 t -= 325.0 * j1p->cos_4j7 * j1p->sin_j3 + 398.0 * j1p->sin_3j7 *
	j1p->sin_3j3 - 890.0 * j1p->cos_j3;
 t += 344.0 * j1p->cos_j7 * j1p->cos_3j3 + 2206.0 * j1p->sin_j7 *
	j1p->cos_j3 - 427.0 * j1p->cos_3j7 * j1p->cos_3j3;
 d->semi_major = t * 1.0e-6;
}


void p_saturn( PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t;

 p_sat_ml( p, d );
 p_sat_eccen( d );
 p_sat_perh_ma( p, d );
 p_sat_semi_maj( d );
 t =  7.47e-4 * j1p->cos_j7 * j1p->sin_j3 + 1.069e-3 *
	j1p->cos_j7 * j1p->cos_j3;
 t += 2.108e-3 * j1p->sin_2j7 * j1p->sin_2j3 + 1.261e-3 *
	j1p->cos_2j7 * j1p->sin_2j3;
 t += 1.236e-3 * j1p->sin_2j7 * j1p->cos_2j3 - 2.075e-3 *
	j1p->cos_2j7 * j1p->cos_2j3;
 d->helio_latitude = d2r( t );
 return;
}
