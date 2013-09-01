#include <stdafx.h>
#include "astro.h"
#include "flags.h"

void fixup_mcic_polar( short &mc, short &ic, short lat, int flags, short &retr)
{
	if ( flags && lat > 68 ) {
	   int tmp = mc;
	   mc = ic;
	   ic = tmp;
	   retr = 1;
	}
	else
	   retr = 0;
}