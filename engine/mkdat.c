#include <stdio.h>

char buf[200];
char name[50], model;
double inclination, ascend, perhelion, semi_major, daily_motion,
       eccentricity, mean_anomaly, magnitude, angular_size, arc_sec,
       equinox_year;
int ep_y, ep_m, ep_d;

int doread( FILE *stream )
{
 char c;

 c = fgetc(stream);
 if ( c == '*' ) {
    fgets( buf, 199, stream );
    return( 0 );
    }
 else if ( c == EOF )
    return( EOF );
 else if ( c != '\n' )
 ungetc( c, stream );
 fscanf( stream, "%[^,],e,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d/%d/%d,%lf,",
   name, &inclination, &ascend, &perhelion, &semi_major, &daily_motion,
   &eccentricity, &mean_anomaly, &ep_m, &ep_d, &ep_y, equinox_year );
 mean_anomaly += perhelion;
 daily_motion /= 9.856263e-3;
 model = fgetc(stream);
 if ( model != 'g' )
    ungetc( model, stream );
 fscanf( stream, "%lf,%lf", &magnitude, &angular_size );
 c = fgetc(stream);
 if ( c != ',' ) {
    ungetc( c, stream );
    return( 1 );
    }
 fscanf( stream, "%lf", &arc_sec );
 return( 1 );
}

int dowrite( FILE *stream )
{
 fprintf( stream, "/*---%s---%d/%d/%d---*/\n", name, ep_m, ep_d, ep_y );
 fprintf( stream, "%le, %le, 0.0, 0.0,\n", mean_anomaly, daily_motion );
 fprintf( stream, "%le, 0.0, 0.0, 0.0,\n", perhelion );
 fprintf( stream, "%le, 0.0, 0.0, 0.0,\n", eccentricity );
 fprintf( stream, "%le, 0.0, 0.0, 0.0,\n", inclination );
 fprintf( stream, "%le, 0.0, 0.0, 0.0,\n", ascend );
 fprintf( stream, "%le,\n", semi_major );
}

void main()
{
 FILE *istream, *ostream;
 int r;

 istream = fopen( "ephem.db", "rt" );
 ostream = fopen( "ephem.dat", "wt" );
 do {
    r = doread(istream);
    if ( r && r != EOF )
       dowrite(ostream);
    } while ( r != EOF );
 fclose(istream);
 fclose(ostream);
}
