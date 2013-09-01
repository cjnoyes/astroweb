#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include "astro.h"
#include "kep.h"
// newplanets.cpp : Defines the entry point for the console application.
//
extern double julian_date( DATES *dp, int hours, int minutes, int epoch );

#define CTPLAN 8
char *plans[CTPLAN] = {"MERC","VENS","MARS","JUPI","SATR","URAN","NEPT","PLUT"};

extern double RTD;

double caltoj( int year, int month, double day )
{
long y, a, b, c, e, m;
double J;


/* The origin should be chosen to be a century year
 * that is also a leap year.  We pick 4801 B.C.
 */
y = year + 4800;
if( year < 0 )
	{
	y += 1;
	}

/* The following magic arithmetic calculates a sequence
 * whose successive terms differ by the correct number of
 * days per calendar month.  It starts at 122 = March; January
 * and February come after December.
 */
m = month;
if( m <= 2 )
	{
	m += 12;
	y -= 1;
	}
e = (306 * (m+1))/10;

a = y/100;	/* number of centuries */
if( year <= 1582L )
	{
	if( year == 1582L )
		{
		if( month < 10 )
			goto julius;
		if( month > 10)
			goto gregor;
		if( day >= 15 )
			goto gregor;
		}
julius:
	printf( " Julian Calendar assumed.\n" );
	b = -38;
	}
else
	{ /* -number of century years that are not leap years */
gregor:
	b = (a/4) - a;
	}

c = (36525L * y)/100; /* Julian calendar years and leap years */

/* Add up these terms, plus offset from J 0 to 1 Jan 4801 B.C.
 * Also fudge for the 122 days from the month algorithm.
 */
J = b + c + e + day - 32167.5;
return( J );
}



void printdegs(char *name, double *rad)
{
 static char *sigs[] = {"ARI","TAU","GEM","CAN","LEO","VIR","LIB","SCO","SAG","CAP","AQU","PIS", "ERR"};
 double deg = rad[0] * RTD;
 int min =  deg * 60.0;
 int mdeg = (int)deg % 30;
 int sig = (int)deg/30;
 int mmin = min%60;
 if ( sig > 11 )
	 sig = 12;
 double dec = rad[1] * RTD;
 printf( "%20s: %d%s%d\t%lf\t%lf\n", name, mdeg, sigs[sig], mmin, dec, rad[2] );
}


/*
int main(int argc, char ** argv)
{
 DATES dt;
 double pol[3], rec[3];
 dt.day = atoi(argv[2]);
 dt.month = atoi(argv[1]);
 dt.year = atoi( argv[3] );
 int hour = atoi(argv[4] );
 double jd = caltoj(dt.year,dt.month,dt.day);
 jd += (double)hour / 24.0;
 pol[1] = 0;
 pol[2] = 0;
 initearth(jd);
 dosun(jd,pol[0]);
 printdegs("SUN ",pol);
 domoon(jd,pol);
 printdegs("MOON",pol);
 for ( int i = 0; i < CTPLAN; i++ ) {
     dotheplanet( jd, i, pol, rec );
     printdegs(plans[i],pol);
 }
 for ( int j = 0; j < NOFIXEDSTAR; j++ ) {
     dostar(  j, jd, pol );
	 printdegs(fixedstarelements[j].obname,pol);
 }
 getche();

}
*/
