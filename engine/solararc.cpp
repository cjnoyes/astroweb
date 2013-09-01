#include "stdafx.h"
#include "astro.h"
#include "flags.h"
#include "julian.h"
#include "compute.fns" 
#include "datetime.fns"
#include "dataext.h"
#include "errors.fns"
#include "math.h"
#include <string.h>

/* 09/10/92  Modified for C++ Windows */

#define d2m(d) ((d)*60)

static int step;


/* function to setup the direction for the date */

short direct_date( AS_DATA *data, DIREC_DATA *dddata )
{
 double jd, jdx, ipart, fpart;
 DATES dx, t;
 COUNT days, hours, minutes, x, off, sun1, sun2;
 float ofsf;
 JULIAN jul;

 switch( dddata->direct_system ) {
	   case SAD_YEARDAY :
		   days = dddata->direction.day + (dddata->direction.month*30)+
			  ((short)((float)dddata->direction.year*365.25));
		   ofsf = ((float)days/365.25);
		   days = (short)ofsf;
           fpart = modf(ofsf,&ipart);
		   hours = (int)(fpart*24.0);
		   minutes = 0;
           julian_calc(&data->birth_date,0,0,&jul);
		   sun1 = sun_minutes(&jul);
		   memcpy(&dx,&data->birth_date,sizeof(DATES));
		   dx.day += days;
		   adj_date( &dx );
		   julian_calc(&dx,hours,0,&jul);
		   sun2 = sun_minutes(&jul);
		   minutes = sun2-sun1;
		   if ( minutes < 0 )
			   minutes += d2m(360);
		   break;
       case SAD_YEARDEG :
		   days = dddata->direction.day + (dddata->direction.month*30)+
			  ((short)((float)dddata->direction.year*365.25));
		   ofsf = (float)days/365.25;
		   minutes = (short)(ofsf * 60.0);
		   break;
       case SAD_NABOD :
		   days = dddata->direction.day + (dddata->direction.month*30)+
			  ((short)((float)dddata->direction.year*365.25));
		   ofsf = ((float)days/365.25);
		   ofsf *= 0.98564722;
		   minutes = (short)ofsf*60.0;
		   break;
       case SAD_YEARDAY + 100:
           jd = base_julian_date( &dddata->direction );
		   jdx = base_julian_date( &data->birth_date );
		   ofsf = jd-jdx;
		   ofsf /= 365.25;
           days = (short)ofsf;
           fpart = modf(ofsf,&ipart);
		   hours = (int)(fpart*24.0);
		   minutes = 0;
		   julian_calc(&data->birth_date,0,0,&jul);
		   sun1 = sun_minutes(&jul);
		   memcpy(&dx,&data->birth_date,sizeof(DATES));
		   dx.day += days;
		   adj_date( &dx );
		   julian_calc(&dx,hours,0,&jul);
		   sun2 = sun_minutes(&jul);
		   minutes = sun2-sun1;
		   if ( minutes < 0 )
			   minutes += d2m(360);
		   break;
       case SAD_YEARDEG + 100 :
		   jd = base_julian_date( &dddata->direction );
		   jdx = base_julian_date( &data->birth_date );
		   ofsf = jd-jdx;
		   ofsf /= 365.25;
		   minutes = (short)(ofsf * 60.0);
		   break;
       case SAD_NABOD + 100:
		   jd = base_julian_date( &dddata->direction );
		   jdx = base_julian_date( &data->birth_date );
		   ofsf = jd-jdx;
		   ofsf /= 365.25;
		   ofsf *= 0.98564722;
		   minutes = (short)ofsf*60.0;
		   break;
   	   default : 
		   break;
 }
 return( minutes );
}


void direc_process_aspects( AS_INF *planet, AS_INF *sub,
						      BITS mode, NUM maxx )
{
 ASPEC *asp;
 AS_INF *pl;
 NUM    maxp;
 COUNT  pno, aspects;
 MINU   maxorb;
 BITS   hm = mode & HOUSES;

 maxorb = d2m(2);
 if ( mode & ASTEROID )
	 maxp = CHIRON;
 if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
    maxp = VERTEX;
 else if ( !(mode & NOBIRTHTIM) )
    maxp = IM_COELI;
 else
   maxp = NORTH_NODE;
 if ( maxx == -1 )
    maxx = maxp;
 mode &= NOBIRTHTIM;

 for ( pno = ASCEND, pl = planet; pno <= maxx; ++pl, ++pno ) {
	 if ( (!mode || ( pno != ASCEND )) && pl->calced ) {
	asp = new ASPEC[30];
	if ( asp == NULL ) {
	   errno = ENOMEM;
	   pl->aspectr = NULL;
	   pl->no_aspects = 0;
	   return;
	   }
	pl->aspectr = asp;
	aspects = do_aspect( pl, 0, sub, maxorb, mode,
				       ( ( hm ) ? -maxp : maxp), 0 );
	pl->aspectr = (ASPEC *) reallocx( asp, aspects * sizeof(ASPEC) );
	}
      else {
	pl->aspectr = NULL;
	pl->no_aspects = 0;
	}
    }
}


void do_full_direc( AS_DATA *bdata, AS_INF *binf, AS_INF *sinf,
		   BITS bmode, NUM house_proc, DATA_PACKET *dpk )
{
 BITS mask = ( 0xffff ^ ( BASE_MID | FULL_MID | MID_ASPECT ) );

 short hc[13];
 int cc;
 short i, m1, m2, end;
 JULIAN j1;
 AS_INF *ptr, *sptr;
 BITS xbmode = bmode;
 bmode &= mask;


 if ( bmode & ASTEROID )
	 end = CHIRON;
 else if ( bmode & VERT_EAST )
	 end = EAST_POINT;
 else 
	 end = SOUTH_NODE;
 cc = dpk->chart_code;
 dpk->chart_code = NATAL_CH;
 do_calc( binf, bdata, bmode, house_proc, dpk );
 short ofs = direct_date(bdata,dpk->ddp);
 for ( i = 0, ptr = binf, sptr = sinf; i <= end; i++, ptr++, sptr++ ) {
     sptr->minutes_total=adjust_mins(ptr->minutes_total+ofs);
	 misc_calc(sptr,bmode,bdata->birth_date.year);
 }
 dpk->chart_code = cc;
 direc_process_aspects( sinf, binf, bmode, -1 );
 if ( xbmode & (FULL_MID|HALF_MID) ) {
    do_main_full_midpoints( binf, end, sinf, end, xbmode, 0 );
 }
}
