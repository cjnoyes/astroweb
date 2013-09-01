#include <stdafx.h>
#include "astro.h"
#include "useri.h"
#include "flags.h"
#include "dataext.h"
#include "compute.fns"
#include <memory.h>

short harmonic( short min, short harm )
{
  int hrmin = min * harm;
  int outmin = hrmin % (360*60);
  return outmin;
}

void harmonic_calc_plan(short harm, AS_INF *pln, AS_INF *phrm, int count, BITS mode, int year )
{
 AS_INF *plnp, *phrmp;
 int i;

 for ( i=0, plnp=pln, phrmp = phrm; i < count; i++, plnp++, phrmp++ ) {
     phrmp->minutes_total = harmonic( plnp->minutes_total, harm );
	 phrmp->calced = 1;
	 misc_calc(phrmp,mode,year);
	}
 }


void harmonic_calc_chart(  AS_INF *info, AS_INF *hinf, AS_DATA *data, BITS mode, NUM house_proc,
						     DATA_PACKET *dpk )
{
 int end;
 if ( mode & ASTEROID )
	 end = CHIRON;
 else if ( mode & VERT_EAST )
	 end = EAST_POINT;
 else
	 end = SOUTH_NODE;
 do_calc( info, data, mode, house_proc, dpk );
 memset(hinf,0,CHIRON * sizeof(AS_INF));
 harmonic_calc_plan(*dpk->harmonic, info, hinf, end, mode, data->birth_date.year );
 process_aspects( info, hinf, mode, dpk->chart_code );
 process_aspects( hinf, hinf, mode, dpk->chart_code );
 if ( mode & (FULL_MID|HALF_MID) )
    do_main_full_midpoints( info, end, hinf, end, mode, 0 );

}