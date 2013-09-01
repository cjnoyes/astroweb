#include "stdafx.h"
#include <string.h>
#include "astro.h"

int quit=0;


void set_quit( int q )
{
 quit = q;
}


void get_house_cusps( short *c )
{
 memcpy(c, house_cusps, 26 );
}
