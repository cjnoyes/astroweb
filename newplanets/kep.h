/* General definitions for aa program */

#include <stdio.h>
#include "plantbl.h"

struct orbit
	{
	char obname[16]; /* name of the object */
	double epoch;	/* epoch of orbital elements */
	double i;	/* inclination	*/
	double W;	/* longitude of the ascending node */
	double w;	/* argument of the perihelion */
	double a;	/* mean distance (semimajor axis) */
	double dm;	/* daily motion */
	double ecc;	/* eccentricity */
	double M;	/* mean anomaly */
	double equinox;	/* epoch of equinox and ecliptic */
	double mag;	/* visual magnitude at 1AU from earth and sun */
	double sdiam;	/* equatorial semidiameter at 1au, arc seconds */
/* The following used by perterbation formulas: */
	struct plantbl *ptable;
	double L;	/* computed mean longitude */
	double r;	/* computed radius vector */
	double plat;	/* perturbation in ecliptic latitude */
	};

struct star
	{
	char obname[32];	/* Object name (31 chars) */
	double epoch;		/* Epoch of coordinates */
	double ra;		/* Right Ascension, radians */
	double dec;		/* Declination, radians */
	double px;		/* Parallax, radians */
	double mura;		/* proper motion in R.A., rad/century */
	double mudec;		/* proper motion in Dec., rad/century */
	double v;		/* radial velocity, km/s */
	double equinox;		/* Epoch of equinox and ecliptic */
	double mag;		/* visual magnitude */
	};
/* Note the items for a star are in different measurement units
 * in the ASCII file description.
 */
/* aa.c */
extern double DTR;
extern double RTD;
extern double RTS;
extern double STR;
extern double PI;
extern double J2000;
extern double B1950;
extern double J1900;
extern double JD;
extern double TDT;
extern double UT;
extern double FAR dradt;
extern double FAR ddecdt;
extern int objnum, jdflag, prtflg;
extern double obpolar[];
extern double FAR eapolar[];
extern double FAR rearth[];
extern double dp[];
/* angles.c */
extern double FAR SE;
extern double FAR SO;
extern double FAR EO;
extern double FAR pq;
extern double FAR ep;
extern double FAR qe;

#define NOFIXEDSTAR 58
#define NOMESSIER   109

extern struct orbit earth, mercury, venus, mars, jupiter, saturn, uranus, neptune, pluto;
extern struct star fixedstarelements[];
#ifdef MESSIER
extern struct star messierelements[];
#endif
/* nutate.c */
extern double jdnut, nutl, nuto;
/* epsiln.c */
extern double jdeps, eps, coseps, sineps;
/* vearth.c */
extern double jvearth, vearth[];



/* DEBUG = 1 to enable miscellaneous printouts. */
#define DEBUGEPH 0

/* Get ANSI C prototypes, if you want them.  */

#include "protos.h"
#define ANSIPROT

