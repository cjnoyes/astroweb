// MkFixed.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include "..\newplanets\kep.h"

double DTR = 1.7453292519943295769e-2;
double RTD = 5.7295779513082320877e1;
double RTS = 2.0626480624709635516e5; /* arc seconds per radian */
double STR = 4.8481368110953599359e-6; /* radians per arc second */
double PI = 3.14159265358979323846;
extern double PI;
int fixedsymcnt=0;

/* Standard epochs.  Note Julian epochs (J) are measured in
 * years of 365.25 days.
 */
double J2000 = 2451545.0;	/* 2000 January 1.5 */
double B1950 = 2433282.423;	/* 1950 January 0.923 Besselian epoch */
double J1900 = 2415020.0;	/* 1900 January 0, 12h UT */

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

void write( FILE *stream, struct star *el )
{
fprintf(stream, "{ \"%s\", %8.1lf, %lf, %lf, %lf, %lf, %lf, %2.2lf, 0.0, %le },\n",
  el->obname, el->epoch, el->ra, el->dec, el->px, el->mura, 
  el->mudec, el->v, el->mag );
}

void read( FILE *stream, struct star *el, int mes )
{
double rh, rm, rs, dd, dm, ds, x, z;
int sign, num;

if ( mes )
fscanf( stream, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %s",
	&el->epoch, &rh, &rm, &rs, &dd, &dm, &ds,
	&el->mura, &el->mudec, &el->v, &el->px, &el->mag, &el->obname[0]);
else
fscanf( stream, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %s %d",
	&el->epoch, &rh, &rm, &rs, &dd, &dm, &ds,
	&el->mura, &el->mudec, &el->v, &el->px, &el->mag, &el->obname[0], &num );

x = el->epoch;
if( x == 2000.0 )
	x = J2000;
else if( x == 1950.0 )
	x = B1950;
else if( x == 1900.0 )
	x = J1900;
else
	x = J2000  +  365.25 * (x - 2000.0);
el->epoch = x;

/* read the right ascension */
el->ra = 2.0 * PI * (3600.0*rh + 60.0*rm + rs)/86400.0;

/* read the declination */
sign = 1;
if( (dd < 0.0) || (dm < 0.0) || (ds < 0.0) )
	sign = -1;
z = (3600.0*fabs(dd) + 60.0*fabs(dm) + fabs(ds))/RTS;
el->dec = z;



el->mura *= 15.0/RTS;	/* s/century -> "/century -> rad/century */
el->mudec /= RTS;
z = el->px;
if( z < 1.0 )
	{
	if( z <= 0.0 )
		el->px = 0.0;
	else
		el->px = STR * z;  /* assume px in arc seconds */
	}
else
	{
	el->px = 1.0/(RTS * z);	/* parsecs -> radians */
	}
}


void WriteName(FILE *stream, char *p)
{
	fprintf(stream,"%s\n#+\n", p );
}

void WriteSym(FILE *stream, char *p)
{
   fprintf(stream,"FIX_%s   %d   1   FIXED   %s Planet Text\n", p, fixedsymcnt, p );
   fprintf(stream,"FIX_%s   %d   4   FIXED   %s Planet Text\n", p, fixedsymcnt++, p );
}

void WriteMessSym(FILE *stream, char *p)
{
   fprintf(stream,"MESS_%s   %d   1   MESSIER   %s Planet Text\n", p, fixedsymcnt, p );
   fprintf(stream,"MESS_%s   %d   4   MESSIER   %s Planet Text\n", p, fixedsymcnt++, p );
}

char *getFixedName( char *p, char *buf )
{
 char *st =strchr(p, '(');
 if ( st != NULL )
	 strcpy(buf,st+1);
 else
	 strcpy(buf, p);
 st = strchr(buf,')');
 if ( st != NULL )
	 *st = '\0';
 return buf;
}

int main(int argc, char* argv[])
{
 struct star element;
 FILE *istream, *ostream, *nstream, *sstream, *mstream, *mnstream, *imstream;
 int i;
 char *p, buf[30];

 ostream = fopen("starele.cpp","wt");
 nstream = fopen("fixdnams.ast","wt");
 sstream = fopen("text.sym", "wt");
 mstream = fopen("messierele.cpp","wt");
 mnstream = fopen("messnams.ast","wt");
 imstream = fopen("..\\..\\aa-56\\messier.cat","rt");
 istream = fopen("..\\..\\aa-56\\star.cat","rt");
 fprintf(ostream, "#include <stdio.h>\n");
 fprintf(ostream, "#include <math.h>\n");
 fprintf(ostream, "#include \"kep.h\"\n\n\n");
 fprintf(ostream, "struct star fixedstarelements[] = {\n");
 fprintf(nstream, "# ST__FIXEDN\n");
 fprintf(mstream, "#include <stdio.h>\n");
 fprintf(mstream, "#include <math.h>\n");
 fprintf(mstream, "#include \"kep.h\"\n\n\n");
 fprintf(mstream, "struct star messierelements[] = {\n");
 fprintf(mnstream, "# ST__MESSIER\n");
 
 if ( istream != NULL ) {
	 do {
	   read(istream,&element, 0);
	   element.equinox=0.0;
	   p = getFixedName(element.obname,buf);
	   WriteSym(sstream,p);
	   WriteName(nstream,p);
	   write(ostream,&element);
	 } while ( !feof(istream) && strcmpi(element.obname,"thPer") );
 }
 fixedsymcnt = 0;
 if ( imstream != NULL ) {
	 do {
	   read(imstream,&element,1);
	   element.equinox=0.0;
	   if ( !strcmpi(element.obname,"There_is_no_M102" ) )
		   continue;
	   WriteMessSym(sstream,element.obname);
	   WriteName(mnstream,element.obname);
	   write(mstream,&element);
	 } while ( !feof(imstream) && strcmpi(element.obname,"M110_NGC205") );
 }
 fprintf( ostream, "};\n\n");
 fprintf( mstream, "};\n\n");
 fclose(ostream);
 fclose(istream);
 fclose(nstream);
 fclose(sstream);
 fclose(imstream);
 fclose(mstream);
 fclose(mnstream);
 printf("jul -4000 =%lf\n", caltoj(1900,1,1));
 getche();
}

