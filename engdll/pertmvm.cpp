#include "stdafx.h"
#include "math.h"
#include "utl.h"
#include "julian.h"
#include "plans.h"
#include "planpert.fns"
#include "lngdbl.h"
extern double sun_mean_anomaly;

/* Do Perturbations of Mercury Venus and Mars. These functions take
   a pointer to the block of planetary data, and a pointer to a
   Delta type structure to hold the results */

void p_mercury( PLAN_ELE *p, DELTA *d )
{
 double t, m, a, tha, ta, tm, fm, tm4;

 /* Temporary Variables to reduce redundant float calcs */
 a = p->mean_anomaly;      /* MAP(1) */
 m = (p+1)->mean_anomaly;  /* MAP(2) */
 ta = a + a;               /* a * 2 */
 tha = ta + a;             /* m * 3 */
 tm = m + m;               /* m * 2 */
 fm = tm + tm + m;         /* m * 5 */
 tm4 = (p+3)->mean_anomaly; /* MAP(4) */
 tm4 += tm4;                /* MAP(4) * 2 */

 t =  ( 2.04e-3 * _cos( fm - ta + 2.1328e-1 ) )
   +  ( 1.03e-3 * _cos( tm - a	- 2.8046 ) )
   +  ( 9.1e-4 * _cos( tm4 - a - 6.4582e-1 ) )
   +  ( 7.8e-4 * _cos( fm - tha + 1.7692e-1 ) );
 d->longitude = t;

 t =  ( 7.525e-6 * _cos( tm4 - a + 9.25251e-1 ) )
   +  ( 6.802e-6 * _cos( fm - tha - 4.53642 ) )
   +  ( 5.457e-6 * _cos( tm - ta  - 1.24246 ) )
   +  ( 3.569e-6 * _cos( fm - a  - 1.35699 ) );
 d->radius_vector = t;

 return;
 }

void p_venus( JULIAN *j, PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t;
 double m, s, tm, thm, ths, ts, m4, fs, frs, frm;

 /* Temporary Variables to reduce redundant float calcs */
 s = sun_mean_anomaly;    /* MAS */
 m = (p+1)->mean_anomaly; /* MAP(2) */
 ts = s + s;              /* MAS * 2 */
 ths = ts + s;            /* MAS * 3 */
 fs = ts + ths;           /* MAS * 5 */
 frs = ts + ts;            /* MAS * 4 */
 tm = m + m;              /* MAP(2) * 2 */
 thm = tm + m;            /* MAP(2) * 3 */
 frm = tm + tm;           /* MAP(2) * 4 */
 m4 = (p+3)->mean_anomaly;  /* MAP(4) */

 t = 7.7e-4 * _sinl( 4.1406 * j->jd_cent * 2.6227 );
 t = d2r( t );
 d->mean_long = t;
 d->mean_anomaly = t;

 t =  ( 3.13e-3 * _cosl( ts - tm - 2.587 ) )
   +  ( 1.98e-3 * _cosl( ths - thm + 4.4768e-2 ) )
   +  ( 1.36e-3 * _cosl( s - m - 2.0788 ) )
   +  ( 9.6e-4 * _cosl( ths - tm - 2.3721 ) )
   +  ( 8.2e-4 * _cosl( m4 - m - 3.6318 ) );
 d->longitude = t;

 t =  ( 2.2501e-5 * _cosl( ts - tm - 1.01592 ) )
   +  ( 1.9045e-5 * _cosl( ths - thm + 1.61577 ) )
   +  ( 6.887e-6 * _cosl( m4 - m - 2.06106 ) );
 t += ( 5.172e-6 * _cosl( s - m - 5.08065e-1 ) )
   +  ( 3.62e-6 * _cosl( fs - frm - 1.81877 ) )
   +  ( 3.283e-6 * _cosl( frs - frm + 1.10851 ) )
   +  ( 3.074e-6 * _cosl( m4 + m4 - tm - 9.62846e-1 ) );
 d->radius_vector = t;

 return;
}


void p_mars( JULIAN *j, PLAN_ELE *p, DELTA *d )
{
 LONG_DOUBLE t;
 double m, s, a, ta, tha, tm, thm, ths, ts, m2, fa, fra, b;

 /* Temporary Variables to reduce redundant float calcs */
 s = sun_mean_anomaly;     /* MAS */
 m = (p+3)->mean_anomaly;  /* MAP(4) */
 a = (p+2)->mean_anomaly;  /* MAP(3) */
 ts = s + s;               /* MAS * 2 */
 ths = ts + s;             /* MAS * 3 */
 tm = m + m;               /* MAP(4) * 2 */
 thm = tm + m;             /* MAP(4) * 3 */
 m2 = (p+1)->mean_anomaly; /* MAP(2) */
 ta = a + a;               /* MAP(3) * 2 */
 tha = ta + a;             /* MAP(3) * 3 */
 fra = ta + ta;            /* MAP(3) * 4 */
 fa = ta + tha;            /* MAP(3) * 2 */

 b = thm - fra + fra + ts + ts;
 t = -1 * ( 1.133e-2 * _sinl( b ) + 9.33e-3 * _cosl( b ) );
 t = d2r( t );
 d->mean_long = t;
 d->mean_anomaly = t;

 t =  ( 7.05e-3 * _cosl( m - a - 8.5448e-1 ) )
   +  ( 6.07e-3 * _cosl( tm - a - 3.2873 ) )
   +  ( 4.45e-3 * _cosl( tm - ta - 3.3492 ) );
 t += ( 3.88e-3 * _cosl( s - ta + 3.5771e-1 ) )
   +  ( 2.38e-3 * _cosl( s - a + 6.1256e-1 ) )
   +  ( 2.04e-3 * _cosl( ts - tha + 2.7688 ) );
 t += ( 1.77e-3 * _cosl( tha - m2 - 1.0053 ) )
   +  ( 1.36e-3 * _cosl( ts - fra + 2.6894 ) )
   +  ( 1.04e-3 * _cosl( m + 3.0749e-1 ) );
 d->longitude = t;

 t = 5.3227e-5 * _cosl( m - a + 7.17864e-1 ) + 5.0989e-5
      * _cosl( tm - ta - 1.77997 );
 t += ( 3.8278e-5 * _cosl( tm - a - 1.71617 ) )
   +  ( 1.5996e-5 * _cosl( s - m - 9.69618e-1 ) )
   +  ( 1.7464e-5 * _cosl( ts - tha + 1.19768 ) + 8.966e-6 *
	_cosl ( m - ta - 7.61225e-1 ) );
 t += ( 7.914e-6 * _cosl( thm - ta - 3.43887 ) )
   +  ( 7.004e-6 * _cosl( tm - tha - 1.79573 ) )
   +  ( 6.62e-6 * _cosl( s - ta + 1.97575 ) );
 t += ( 4.93e-6 * _cosl( thm - tha - 1.33069 ) )
   +  ( 4.693e-6 * _cosl( ths - fa + 3.22665 ) )
   +  ( 4.571e-6 * _cosl( ts - fra + 4.27086 ) )
   +  ( 4.409e-6 * _cosl( thm - a - 2.02158 ) );
 d->radius_vector = t;
 return;
}
