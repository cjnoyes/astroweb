#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "astro.h"
#include "useri.h"
#include "fptr.h"
#include "graphdat.hpp"
#include "paths.hpp"
#include "filelst.hpp"
#include "output.fns"
#include "errors.fns"
#include "flags.h"
#include "aspect.h"
#include "dataext.h"
#include "charcons.h"

#define NEWVER 1

/* function to save graphics data to disk file. Type controls the type
   of data is used. If type is NATAL_GRAPH_FILE then only natal
   information is saved. otherwise both sets of info are saved. If
   type is natal the other info can simply be set to null */

int save_graph_data( char *file_name, int type, BIRTH_DB *natal_bd,
     BIRTH_DB *other_bd, AS_INF *natal_inf, AS_INF *other_inf,
     BITS mode1, BITS mode2, FILE_DB *fptr, int house_proc )
{
 int i, j, file, nas = 0;
 AS_INF *ptr, **rp, *tr;
#ifndef NEWVER
 O_GR_DAT data;
 O_GR_ASPT *aspt = NULL, *ax = NULL;
 O_GR_CHART gchart;
#else
 GR_DAT data;
 GR_ASPT *aspt = NULL, *ax = NULL;
 GR_CHART gchart;
#endif
 ASPEC *ap;
 DATES *d;
#ifndef NEWVER
 if ( ( file = write_fptr( file_name, (NFDB *)fptr, 2 ) ) == -1 ) {
#else
 if ( ( file = write_fptr( file_name, (NFDB *)fptr, 3 ) ) == -1 ) {
#endif
    do_error( file_name );
    AstFileList.remFile( file_name );
    return( -1 );
    }
 chsize( file, (long) sizeof( FPTR ) );
#ifndef NEWVER
 memset( &data, 0, sizeof( O_GR_DAT ) );
#else
 memset( &data, 0, sizeof( GR_DAT ) );
#endif
 if ( mode1 & VERT_EAST && !(mode1 & NOBIRTHTIM) )
    data.maxpt = EAST_POINT;
 else if ( !(mode1 & NOBIRTHTIM) )
    data.maxpt = PART_FORTU;
 else
    data.maxpt = SOUTH_NODE;
 data.type = type;
#ifdef NEWVER
 if ( mode1 & SIDEREAL )
    strcpy( data.trop_side, "Sidereal" );
 else
    strcpy( data.trop_side, "Tropical" );
 strcpy( data.house_proc, housmod[house_proc] );
 data.version=0xffff;
 data.num_data = data.maxpt;
#endif
 strcpy( data.name1, natal_bd->name );
 strcpy( data.date1, make_date( &natal_bd->birth_data.birth_date ) );
 strcpy( data.time1, make_time( &natal_bd->birth_data.birth_time,
    natal_bd->am_pm  ) );
 memcpy( data.natal_house_cusps, &house_cusps[1], 12 * sizeof( int ) );
 memcpy( data.other_house_cusps, &alt_cusps[1], 12 * sizeof( int ) );
 if ( !( mode1 & HOUSES ) )
    data.natal_house_cusps[0] = -1;
 for ( i = 0, ptr = natal_inf; i <= data.maxpt; ++i, ++ptr ) {
     if ( ptr->retrograde && ptr->minutes_total != 1 )
	data.natal_minutes[i] = -ptr->minutes_total;
     else if  ( ptr->retrograde && ptr->minutes_total == 1 )
	data.natal_minutes[i] = -2;
     else
	data.natal_minutes[i] = ptr->minutes_total;
     }
 if ( type != NATAL_GRAPH_FILE && type != PROGRESS_GRAPH_FILE
      && type != NUMERIC_GRAPH_FILE && type != RELOC_GRAPH_FILE &&
      type != TRANSITS_GRAPH_FILE ) {
     strcpy( data.name2, other_bd->name );
     strcpy( data.date2, make_date( &other_bd->birth_data.birth_date ) );
     strcpy( data.time2, make_time( &other_bd->birth_data.birth_time,
	 other_bd->am_pm ) );
    if ( type != COMPOS_GRAPH_FILE && type != COMPTR_GRAPH_FILE ) {
#ifdef NEWVER
       data.num_other = data.maxpt;
#endif
       for ( i = 0, ptr = other_inf; i <= data.maxpt; ++i, ++ptr ) {
	   if ( ptr->retrograde && ptr->minutes_total != 1 )
	      data.other_minutes[i] = -ptr->minutes_total;
	   else if  ( ptr->retrograde && ptr->minutes_total == 1 )
	      data.other_minutes[i] = -2;
	   else
	      data.other_minutes[i] = ptr->minutes_total;
	   }
       }
    if ( type == COMPTR_GRAPH_FILE )
       data.num_charts = transit_data.num_transits;
    }
 else if ( type == PROGRESS_GRAPH_FILE ) {
     sprintf( data.date2, "%-2.2d/%-2.2d/%-2.2d",
	 progress_data.offset.year,
	 progress_data.offset.month,
	 progress_data.offset.day );
     for ( i = 0, ptr = other_inf; i <= data.maxpt; ++i, ++ptr ) {
	 if ( ptr->retrograde && ptr->minutes_total != 1 )
	    data.other_minutes[i] = -ptr->minutes_total;
	 else if  ( ptr->retrograde && ptr->minutes_total == 1 )
	    data.other_minutes[i] = -2;
	 else
	    data.other_minutes[i] = ptr->minutes_total;
	 }
#ifdef NEWVER
     data.num_other = data.maxpt;
#endif
     }
 else if ( type == TRANSITS_GRAPH_FILE )
    data.num_charts = transit_data.num_transits;

 if ( mode1 & NOBIRTHTIM )
       data.birth_time_known_1 = 0;
    else
       data.birth_time_known_1 = 1;
 if ( mode2 & NOBIRTHTIM )
       data.birth_time_known_2 = 0;
    else
       data.birth_time_known_2 = 1;
 if ( type == NATAL_GRAPH_FILE || type == NUMERIC_GRAPH_FILE ) {
    strcpy( data.name2, natal_bd->location );
    sprintf( data.date2, "%3d%c%2d", natal_bd->birth_data.longitude.degrees,
       natal_bd->birth_data.longitude.dir, natal_bd->birth_data.longitude.minutes );
    sprintf( data.time2, "%2d%c%2d", natal_bd->birth_data.latitude.degrees,
       natal_bd->birth_data.latitude.dir, natal_bd->birth_data.latitude.minutes );
    }
 if ( (type != NATAL_GRAPH_FILE && type != NUMERIC_GRAPH_FILE &&
	type != COMPOS_GRAPH_FILE ) || !(mode1 & ASPECTS) )
    data.num_aspects = 0;
 else {
    nas = 0;
    for ( i = 0, ptr = natal_inf; i < data.maxpt; ++ptr, ++i ) {
	ap = ptr->aspectr;
	for ( j = 0; j < ptr->no_aspects; ++j, ++ap ) {
	    if ( ap->aspect <= SQUARED || ap->aspect == QUINCUNX
		 || ap->aspect == SEMISQUARE )
	       ++nas;
	    }
	}
    data.num_aspects = nas;
#ifndef NEWVER
    aspt = new O_GR_ASPT[nas + 2];
#else
    aspt = new GR_ASPT[nas + 2];
#endif
    if ( aspt == NULL ) {
       do_mem_error( 0 );
       data.num_aspects = 0;
       }
    ax = aspt;
    for ( i = 0, ptr = natal_inf; i < data.maxpt; ++ptr, ++i ) {
	ap = ptr->aspectr;
	for ( j = 0; j < ptr->no_aspects; ++j, ++ap ) {
	/*  if ( ap->aspect != CONJUNCT && ap->aspect <= SQUARED ) { */
	    if ( ap->aspect <= SQUARED || ap->aspect == QUINCUNX ||
		 ap->aspect == SEMISQUARE ) {
	       if ( ap->aspect == SQUARED || ap->aspect == OPPOSITION ||
			 ap->aspect == SEMISQUARE ) {
		  ax->first = -ptr->minutes_total;
		  if ( !ax->first )
		     ax->first = -1;
		  }
	       else
		  ax->first = ptr->minutes_total;
	       ax->second = (natal_inf+ap->planet)->minutes_total;
#ifdef NEWVER
	       ax->aspect = ap->aspect;
#endif
	       ++ax;
	       }
	    }
	}
    }
#ifndef NEWVER
 if ( write( file, (char *) &data, sizeof( O_GR_DAT )) != sizeof( O_GR_DAT ) ) {
#else
 if ( write( file, (char *) &data, sizeof( GR_DAT )) != sizeof( GR_DAT ) ) {
#endif
    do_error( file_name );
    close( file );
    AstFileList.remFile( file_name );
    return( -1 );
    }
 if ( data.num_aspects ) {
#ifndef NEWVER
    if ( write( file, (char *) aspt, data.num_aspects * sizeof( O_GR_ASPT) )
	  != data.num_aspects * sizeof( O_GR_ASPT ) ) {
#else
    if ( write( file, (char *) aspt, data.num_aspects * sizeof( GR_ASPT) )
	  != data.num_aspects * sizeof( GR_ASPT ) ) {
#endif
       do_error( file_name );
       close( file );
       AstFileList.remFile( file_name );
       return( -1 );
       }
    }
 if ( aspt != NULL )
    delete aspt;
 if ( type == TRANSITS_GRAPH_FILE || type == COMPTR_GRAPH_FILE ) {
    for ( j = 0, d = date_ptr, trp = trans_ptr; j < data.num_charts; ++j, ++d, ++trp ) {
	strcpy( gchart.date, make_date( d ) );
#ifdef NEWVER
	gchart.maxpt = SOUTH_NODE;
#endif
	if ( j > 0 ) {
	   for ( i = 0; i < transit_data.start_planet; ++i, ++ptr )
	      gchart.chart_minutes[i] = -1;
	   }
	for ( i = (j?transit_data.start_planet:SUN), tr = *trp;
			   i <= (j?transit_data.end_planet:SOUTH_NODE); ++i, ++tr ) {
	 if ( tr->retrograde && tr->minutes_total != 1 )
	    gchart.chart_minutes[i] = -tr->minutes_total;
	 else if  ( ptr->retrograde && tr->minutes_total == 1 )
	    gchart.chart_minutes[i] = -2;
	 else
	    gchart.chart_minutes[i] = tr->minutes_total;
	 }
	if ( j > 0 ) {
	   for ( i = transit_data.end_planet+1; i <= data.maxpt; ++i )
	      gchart.chart_minutes[i] = -1;
	   }
        else {
	   for ( i = SOUTH_NODE+1; i <= data.maxpt; ++i )
	      gchart.chart_minutes[i] = -1;
	   }

#ifndef NEWVER
	write( file, (char *) &gchart, sizeof( O_GR_CHART ) );
#else
	write( file, (char *) &gchart, sizeof( GR_CHART ) );
#endif
	}
    }
#ifdef NEWVER
/*
 GR_DATALST *pData = new GR_DATALST[data.maxpt];
 if ( pData == NULL ) {
    if ( aspt == NULL ) {
       do_mem_error( 0 );
       data.num_data = 0;
       data.num_other = 0;
       }
    }
 else {
    GR_DATALST *pd;
    for ( i = 0, pd = pData, ptr = natal_inf;
				    i < data.maxpt; ++ptr, ++i, pd++ ) {
	pd->min = ptr->minutes_total;
	if ( !( mode1 & HOUSES ) )
	   pd->house = ptr->house;
	else
	   pd->house = -1;
	if ( !( mode1 & MISCELL ) )
	   pd->decan = ptr->misc_code & 3;
	else
	   pd->decan = -1;
	}
    write( file, (char *) pData, sizeof( GR_DATALST ) );
    if ( data.num_other ) {
       GR_DATALST *pdx;
       for ( i = 0, pdx = pData, ptr = other_inf;
				       i < data.maxpt; ++ptr, ++i, pdx++ ) {
	   pdx->min = ptr->minutes_total;
	   if ( !( mode1 & HOUSES ) )
	      pdx->house = ptr->house;
	   else
	      pdx->house = -1;
	   if ( !( mode1 & MISCELL ) )
	      pdx->decan = ptr->misc_code & 3;
	   else
	      pdx->decan = -1;
	   }
       write( file, (char *) pData, sizeof( GR_DATALST ) );
       }
    delete pData;
    }
*/
#endif
 close( file );
 return( 0 );
}
