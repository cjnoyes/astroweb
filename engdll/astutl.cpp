#include "stdafx.h"
#include "astro.h"

static char home_signs[] = { LEO, CANCER, GEMINI, TAURUS,
  ARIES, SAGITARIUS, CAPRICORN, AQUARIUS, PISCES, SCORPIO };

static char exalt_signs[] = { ARIES, TAURUS, GEMINI, PISCES, CAPRICORN,
  CANCER, LIBRA, AQUARIUS, LEO, PISCES, GEMINI, SAGITARIUS };

static char rulers[] = { MARS, VENUS, MERCURY, MOON, SUN, MERCURY, VENUS,
  PLUTO, JUPITER, SATURN, URANUS, NEPTUNE };

static char exalt_deg[] = { 19, 3, 15, 27, 28, 15, 21, -1, -1, -1, 3, 3 };

int home_sign( int planet, int *s2 )
{
 *s2 = -1;
 if ( planet > PLUTO || !planet )
    return( -1 );
 if ( planet == MERCURY )
    *s2 = VIRGO;
 else if ( planet == VENUS )
    *s2 = LIBRA;
 return((int) home_signs[planet-1] );
}

int is_home_sign( int planet, int sign )
{
 int i = -1, j;

 j = home_sign( planet, &i );
 if ( j == -1 )
    return( 0 );
 if ( sign == j || sign == i )
    return( 1 );
 else
    return( 0 );
}


int detriment_sign( int planet, int *s2 )
{
 int x;
 *s2 = -1;
 if ( planet > PLUTO || !planet )
    return( -1 );
 if ( planet == MERCURY )
    *s2 = PISCES;
 else if ( planet == VENUS )
    *s2 = ARIES;
 x = (int)home_signs[planet-1];
 x += LIBRA;
 if ( x > PISCES )
    x -= 12;
 return( x );
}

int is_detriment_sign( int planet, int sign )
{
 int i = -1, j;

 j = detriment_sign( planet, &i );
 if ( j == -1 )
    return( 0 );
 if ( sign == j || sign == i )
    return( 1 );
 else
    return( 0 );
}


int exalted_sign( int planet, int *deg, int *s2 )
{
 *s2 = -1;
 *deg = -1;
 if ( planet > SOUTH_NODE || !planet )
    return( -1 );
 if ( planet == URANUS )
    *s2 = SCORPIO;
 *deg = (int)exalt_deg[planet-1];
 return((int)exalt_signs[planet-1] );
}

int is_exalted_sign( int planet, int deg, int sign )
{
 int i = -1, j, d;

 j = exalted_sign( planet, &d, &i );
 if ( j == -1 )
    return( 0 );
 if ( sign == j || sign == i ) {
    if ( d == deg )
       return( 2 );
    else
       return( 1 );
    }
 else
    return( 0 );
}

int fall_sign( int planet, int *deg, int *s2 )
{
 int x;

 *s2 = -1;
 *deg = -1;
 if ( planet > SOUTH_NODE || !planet )
    return( -1 );
 if ( planet == URANUS )
    *s2 = TAURUS;
 *deg = (int)exalt_deg[planet-1];
 x = (int)exalt_signs[planet-1];
 x += LIBRA;
 if ( x > PISCES )
    x -= 12;
 return( x );
}

int is_fall_sign( int planet, int deg, int sign )
{
 int i = -1, j, d;

 j = fall_sign( planet, &d, &i );
 if ( j == -1 )
    return( 0 );
 if ( sign == j || sign == i ) {
    if ( d == deg )
       return( 2 );
    else
       return( 1 );
    }
 else
    return( 0 );
}

int ruler_planet( int sign )
{
 if ( sign < ARIES || sign > PISCES )
    return( -1 );
 return( (int)rulers[sign] );
}


int is_ruler_planet( int planet, int sign )
{
 if ( ruler_planet( sign ) == planet )
    return( 1 );
 else
    return( 0 );
}


int is_combust( int sun_min, int pl_min )
{
 if ( pl_min > sun_min ) {
    if ( pl_min - sun_min < 511 )
       return( 1 );
    else if ( pl_min - sun_min > 21089 )
       return( 1 );
    else
       return( 0 );
    }
 else {
    if ( sun_min - pl_min < 511 )
       return( 1 );
    else if ( sun_min - pl_min > 21089 )
       return( 1 );
    else
       return( 0 );
    }
 return( 0 );
}
