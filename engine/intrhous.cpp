#include "stdafx.h"
#include "astro.h"
#include "flags.h"
#include "compute.fns"
#include <string.h>

short intercept_sign_table[12];


short is_sign_intercept( short sign )
{
 return( intercept_sign_table[sign] );
}

void do_intercept_sign_table( short system )
{
 int i;

 if ( system == EQUAL ) {
    memset( intercept_sign_table, 0, 24 );
    return;
    }
 for ( i = 0; i < 12; ++i )
     intercept_sign_table[i] = 1;
 for ( i = 1; i < 12; ++i )
     intercept_sign_table[ sign( house_cusps[i] ) ] = 0;
}
