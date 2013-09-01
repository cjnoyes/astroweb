#include "stdafx.h"
#include "astro.h"
#include "compute.fns"
#include "utlduad.h"
#include "flags.h"

char duad_rulers[] = { MARS, VENUS, MERCURY, MOON, SUN, MERCURY,
   VENUS, PLUTO, JUPITER, SATURN, URANUS, NEPTUNE };

char navas_ofs[] = { 0, 8, 4, 0, 8, 4, 0, 8, 4, 0, 8, 4 };

char decan_rulers[12][3] = {
   { MARS, SUN, JUPITER }, { VENUS, MERCURY, SATURN },
   { MERCURY, VENUS, URANUS }, { MOON, PLUTO, NEPTUNE },
   { SUN, JUPITER, MARS }, { MERCURY, SATURN, VENUS },
   { VENUS, URANUS, MERCURY }, { PLUTO, NEPTUNE, MOON },
   { JUPITER, MARS, SUN }, { SATURN, VENUS, MERCURY },
   { URANUS, MERCURY, VENUS }, { NEPTUNE, MOON, PLUTO } };

char decan_sub_rulers[12][3] = {
   { -1, -1, -1 }, { -1, -1, -1 },
   { -1, -1, SATURN }, { -1, MARS, JUPITER },
   { -1, -1, -1 }, { -1, -1, -1 },
   { -1, SATURN, -1 }, { MARS, JUPITER, -1 },
   { -1, -1, -1 }, { -1, -1, -1 },
   { SATURN, -1, -1 }, { JUPITER, -1, MARS } };




int duad( int min )
{
 min %= 1800;
 return( (min / 150)+1 );
}

int navasma( int min )
{
 min %= 1800;
 return( (min / 200)+1 );
}

int _duad_ruler_x( int duad, int *sub )
{
 *sub = -1;
 if ( duad == 8 )
    *sub = MARS;
 else if ( duad == 11 )
    *sub = SATURN;
 else if ( duad == 12 )
    *sub = JUPITER;
 return( duad_rulers[duad-1] );
}

int duad_ruler( int min, int *sub )
{
 int d, s, r;
 s = min / 1800;
 min %= 1800;
 d = (min / 150)+1;
 r = d + s;
 if ( r > 12 )
    r -= 12;
 return( _duad_ruler_x( r, sub ) );
}

int navasma_sign( int min )
{
 int n, s;
 s = min / 1800;
 min %= 1800;
 n = (min / 200);
 n += s + navas_ofs[s];
 if ( n > PISCES )
    n %= 12;
 return( n );
}


int decan_ruler( int min, int sign, int *sub )
{
 int d = decanates( min );
 if ( sign > PISCES || sign < ARIES || min < 0 || min > 21600 ) {
    *sub = -1;
    return( -1 );
    }
 *sub = decan_sub_rulers[sign][d];
 return( decan_rulers[sign][d] );
}

int extended_misc_calc( int planet, AS_INF *a, AS_INF_EXT *x, int sun)
{
 int m = a->minutes_total;
 int md = a->degree_minor;
 int s = a->sign;
 unsigned f = 0;
 int r;

 x->duad = duad(m);
 x->duad_ruler = duad_ruler( m, &x->duad_ruler2 );
 x->navasma = navasma( m );
 x->navasma_sign = navasma_sign( m );
 if ( planet != SUN && is_combust( sun, m ) > 0 )
    f |= IS_COMBUST;
 if ( is_home_sign( planet, s ) > 0 )
    f |= IS_HOME;
 if ( is_detriment_sign( planet, s ) > 0 )
    f |= IS_DETRIMENT;
 if ( is_ruler_planet( planet, s ) > 0 )
    f |= IS_RULER;
 r = is_exalted_sign( planet, md, s );
 if ( r == 1 )
    f |= IS_EXALTED;
 if ( r == 2 )
    f |= IS_EXACT;
 r = is_fall_sign( planet, md, s );
 if ( r == 1 )
    f |= IS_FALL;
 if ( r == 2 )
    f |= IS_EXACT;
 x->flags = f;
 x->decan = decanates( md ) +1;
 x->decan_ruler = decan_ruler( md, s, &x->decan_ruler2 );
 return( 0 );
}

void do_extended_misc_calc( AS_INF *inf, AS_INF_EXT *ext, int mode )
{
 AS_INF *ap;
 AS_INF_EXT *aep;
 int i, s, maxplan;
 if ( mode & ASTEROID )
	 maxplan = CHIRON;
 else if ( mode & VERT_EAST )
	 maxplan = EAST_POINT;
 else 
	 maxplan = IM_COELI;
 s = (inf+SUN)->minutes_total;
 for ( i = 0, ap = inf, aep = ext; i < maxplan; ++i, ++ap, ++aep )
     extended_misc_calc( i, ap, aep, s);
}
