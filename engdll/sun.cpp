#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "astro.h"
#include "ascend.h"
#include "compute.fns"
#include "datetime.fns"
#include "julian.h"
#include "utl.h"
#include "lngdbl.h"

/* global Variables read by planetary routines */

double sun_longitude;
double sun_radius_vector;
double sun_mean_anomaly;

/* macros making certain repetitive calcuations simpler and neater
   code to the right of '|' is Basic from book. lc variable x or y
   are macro parameters. Cap variables are Basic Variables used
   T is Julian Centuries, T2 is Julian Centuries Squared */

/*      murem(x)      |   B = 360 * ( A - INT(A) )                 */
/*      murem1x(x)    |   A = x * T : B = 360 * ( A - INT(A) )
			  result is B                              */

#define murem1x(x)        (murem((x)*j->jd_cent))

/*      murem1r(x,y)  |   A = x * T : B = 360 * ( A - INT(A) )
		      |   FNRAD( y + B )                           */

#define murem1r(x,y)      d2r((y)+murem1x(x))

/*      muxt2(x)      |   x * T2 + B                               */

#define muxt2(x)          ((x)*j->jd_cent_sq)+temp

/* uses 34 double consts 272 bytes */

/* routine to calculate the position of the sun. Takes julian date
   struct and returns sun minutes of longitude

   Aliases to variables used in "Astronomy With Your Personal Computer"
	temp.................B     mean_long............LS
	mean_anomal_deg......MS    mean_anomaly_rads....MA
	eccen_anomaly........EA    true_anomal..........NU
	eccentricity.........S     delta_long...........DL
	delta_rad_vec........DR
   */

MINU sun_minutes( JULIAN *j )
{
 double temp, mean_long, mean_anomal_deg, mean_anomal_rads;
 double eccen_anomaly, true_anomal, eccentricity;
 double a1, b1, c1, d1, e1, h1, delta_long, delta_rad_vec;
 double sin_b1;

 /* Note: variable named temp is used in
	  muxt2() and murem1r macros */

 temp = murem1x( 1.000021359e2 );
 mean_long = 2.7969668e2 + muxt2( 3.025e-4 );
 temp = murem1x( 9.999736042e1 );
 mean_anomal_deg = 3.5847583e2 - muxt2( 1.5e-4 + (3.3e-6 * j->jd_cent) );
 eccentricity = 1.675104e-2 - (4.18e-5 * j->jd_cent) -
      (1.26e-7 * j->jd_cent_sq);
 mean_anomal_rads = d2r( mean_anomal_deg );
 sun_mean_anomaly = mean_anomal_rads;
 eccen_anomaly = kepler_eccentric_anomaly( mean_anomal_rads, eccentricity );
 true_anomal = true_anomaly( eccentricity, eccen_anomaly );

 a1 = murem1r( 6.255209472e1, 153.23 );
 b1 = murem1r( 1.251041894e2, 216.57 );
 c1 = murem1r( 9.156766028e1, 312.69 );
 d1 = murem1r( 1.236853095e3, 350.74 - (1.44e-3 * j->jd_cent_sq) );
 e1 = d2r( 231.19 + (20.2 * j->jd_cent) );
 h1 = murem1r( 1.831353208e2, 353.4);

 /* perturbations to longitude */
 sin_b1 = _sin( b1 );

 delta_long = (1.34e-3 * _cos(a1)) + (1.54e-3 * sin_b1);
 delta_long += (2e-3 * _cos(c1)) + (1.79e-3 * _sin(d1));
 delta_long += 1.78e-3 * _sin( e1 );

 /* perturbations to radius vector. Used for planetary calculations */
 delta_rad_vec = (5.43e-6 * _sin(a1)) + (1.575e-5 * sin_b1);
 delta_rad_vec += (1.627e-5 * _sin(c1)) + (3.076e-5 * _cos(d1));
 delta_rad_vec += 9.27e-6 * _sin(h1);

 sun_longitude = true_anomal +
      d2r( mean_long - mean_anomal_deg + delta_long );
 sun_longitude = fmodulus( sun_longitude, TWO_PI );
 sun_radius_vector = 1.0000002 * (1.0 - (eccentricity *
      _cos(eccen_anomaly) ) ) + delta_rad_vec;

 return( r2m(sun_longitude) );
}
