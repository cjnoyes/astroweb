#include "stdafx.h"
#include <io.h>
#include "astro.h"
#include "useri.h"
#include "flags.h"
#include "textptr.h"
#include "output.fns"
#include "dataext.h"
#include "charcons.h"
#include "utlduad.h"
#include "report.h"
#include "oututil.h"
#include "outfunc.fns"
#include "paths.hpp"
#include "aspect.h"

char sign_rulers[] = { MARS, VENUS, MERCURY, MOON, SUN, MERCURY,
   VENUS, PLUTO, JUPITER, SATURN, URANUS, NEPTUNE };

int report_table = -1;

/* 09/15/92  Modified for C++ Windows */

#define NDIAG          1
#define REPORT_NAME     "NATAL"
#define SUMMARY_NAME    "SUMMARY"

/* Semi-global purely so we don't have to get these more than 1x */
static const char *rppl, *rphs, *rpah, *rpas, *rpmsc;


/* function to print out a single aspect entry each on a single line */

int print_aspect_entry( FILE *stream, NUM planet, ASPEC *ptr, int aspfil, char *name, char *rpt )
{
 int e;
 e = output_aspects( stream, name, planet, ptr, rpt );
 if ( aspfil != -1 ) /* show the text */
    get_transfer_text_asp( ptr->planet, planet, ptr->aspect, aspfil, stream );
 return( e );
}
#ifdef DELUXE
int print_midpoint_entry( FILE *stream, NUM planet, MIDPT *ptr,
		 BITS mode, int file, char *name, int table, int global )
{
 const char *rpm;
 COUNT x;
 ASPEC *as;
 if ( ptr->total_minutes == -1 )
    return( 1 );
 rpm = get_report( table, global, CH__MID_PLANET );
 if ( rpm == NULL )
    return( 1 );
 if ( output_midpoint_sign( stream, planet, ptr, name, (char *)rpm ) )
    return( 1 );
 if ( file != -1 ) /* show the text */
    get_transfer_text_asp( planet, ptr->second_planet, ptr->sign, file, stream );
 if ( mode & HOUSES ) {
    rpm = get_report( table, global, CH__MID_HOUSE );
    if ( rpm == NULL )
       return( 1 );
    if ( output_midpoint_house( stream, planet, ptr, name, (char *)rpm ) )
       return( 1 );
    }
 else
    fputc( '\n', stream );
 if ( ptr->no_aspect > 0 ) {
    rpm = get_report( table, global, CH__MID_ASPT );
    if ( rpm == NULL )
       return( 1 );
    if ( output_title( stream, name, "", "", (char *)rpm ) )
       return( 1 );
    if ( ptr->no_aspect ) {
       rpm = get_report( table, global, CH__MID_ASPE );
       if ( rpm == NULL )
	  return( 1 );
       for ( x = 1, as = ptr->aspectr; x <= ptr->no_aspect; ++x, ++as ) {
	   if ( print_aspect_entry( stream, planet, as, -1, name, (char *)rpm ) )
	      return( 1 );
	   }
       }
    fputc( '\n', stream );
    }
return( 1 );
}

#endif
/* function to print out each entry. Takes pointer to block, and number
   of block entry as planet */

int print_entry( FILE *stream, AS_INF *inf, NUM planet, BITS mode,
    int fil, int aspfil, int hsfil, int midfile, AS_INF_EXT *aie,
    char *name, char *comment, int table, int global )
{
 COUNT i, ruler;
 ASPEC *as;
 MIDPT *mid;
 const char *rps;
 static char fmt5[] = { "<%s:>\x80" };

 ruler = sign_rulers[sign(house_cusps[inf->house])];
 if ( output_planet_sign( stream, planet, inf,
			-1, NULL, mode, name, comment, (char *)rppl ) )
    return( 1 );
 if ( mode & TEXT ) {
    if ( inf->cusp == -1 ) {
       get_transfer_text( inf->sign, planet, fil, stream );
       if ( inf->retrograde )
	  get_transfer_text( 12, planet, fil, stream );
       }
    else {
       fprintf( stream, (char *)fmt5, sign_str( inf->sign ) );
       get_transfer_text( inf->sign, planet, fil, stream );
       fprintf( stream, (char *)fmt5, sign_str( inf->cusp ) );
       get_transfer_text( inf->cusp, planet, fil, stream );
       if ( inf->retrograde )
	  get_transfer_text( 12, planet, fil, stream );
       }
    }
 if ( mode & MISCELL ) {
    if ( output_planet_sign( stream, planet, inf,
			   -1, NULL, mode, name, comment, (char *)rpmsc ) )
       return( 1 );
    if ( mode & TEXT )
       get_transfer_text( inf->sign, (inf->misc_code & 3)+29, fil, stream );
    }
 if ( mode & EXT_CALCS ) {
    rps = get_report( table, global, CH__PLAN_EXT );
    if ( rps == NULL )
       return( 1 );
    if ( output_planet_sign_ext( stream, planet, aie, name, (char *)rps ) )
       return( 1 );
    }
 if ( mode & HOUSES && planet != ASCEND && planet != MED_COELI &&
	  planet != IM_COELI ) {
    if ( output_planet_house( stream, planet, inf, name, comment, (char *)rphs ) )
       return( 1 );
    if ( mode & TEXT )
       get_transfer_text( inf->house, planet, hsfil, stream );
	   get_transfer_text( ruler, 13, hsfil, stream );
    }
 if ( mode & ASPECTS ) {
    if ( inf->no_aspects > 0 ) {
       if ( output_title( stream, name, "", "", (char *)rpah ) )
	  return( 1 );
       for ( as = inf->aspectr, i = 1; i <= inf->no_aspects; ++i, ++as )
	   if ( print_aspect_entry( stream, planet, as, ((mode & TEXT)?aspfil:-1), name, (char *)rpas ) )
	      return( 1 );
       }
    }
#ifdef DELUXE
 if ( inf->no_midpt > 0 ) {
    rps = get_report( table, global, CH__MIDPT_HD );
    if ( rps == NULL )
       return( 1 );
    if ( output_title( stream, name, comment, "", (char *)rps ) )
       return( 1 );
    for ( mid = inf->midptr, i = 1; i <= inf->no_midpt; ++i, ++mid )
	print_midpoint_entry( stream, planet, mid, mode, midfile, name, table, global );
    }
#endif
 fputc( '\n', stream );
 return( 1 );
}

void do_house_sign( DEGR minutes, int *minor, int *sign, int *cusp )
{
 *minor = minor_degrees( minutes );
 *sign = sign( minutes );
 *cusp = sign_cusp( *minor, minutes % 60, *sign );
}

/* function to manage printing of results */

void print_results( FILE *stream, AS_INF *inf_ptr, BIRTH_DB *data,
       AS_INF_EXT *aie_ptr, BITS mode, NUM house_mode,
       DATA_PACKET *dpk, char *comment )
{
 COUNT i;
 AS_INF *inf;
 AS_INF_EXT *aiep;
 char buf1[60];
 const char *report;
 NUM maxp, file = -1, aspfile = -1, hsfile = -1, midfile = -1;
 int sign, cusp, k, global = -1, table = -1, ruler;
 static char fmt8[] = { "<%s:>\x80" };

 if ( mode & ASTEROID )
	maxp = CHIRON;
 else if ( mode & VERT_EAST )
    maxp = EAST_POINT;
 else
    maxp = PART_FORTU;

 if ( mode & TEXT ) {
    if ( mode & ASPECTS )
       aspfile = open_asp_files( ASPECT_FILE );
    file = open_files( PLANET_FILE );
    if ( mode & HOUSES )
       hsfile = open_files( HOUSE_FILE );
	if ( mode & (BASE_MID|HALF_MID|FULL_MID) )
	   midfile = open_asp_files( MIDPOINT_SIGN );
    }
 if ( mode & NOBIRTHTIM && ( dpk->rdp->rect_system == SOLARCHART ||
		 dpk->rdp->rect_system == FLATCHART ) )
    house_mode = EQUAL;
 table = get_tables( REPORT_NAME, &global );
 if ( table == -1 ) {
    er_close1:
    close_files( file );
    close_files_asp( aspfile );
    close_files( hsfile );
    close_files( midfile );
    return;
    }
 if ( dpk->chart_code & RELOC_CH )
    k = CHH_RELOC;
 else
    k = CHH_TITLE;
 if ( ( report = get_report( table, global, k ) ) == NULL ) {
    goto er_close1;
    }
 else
    output_title( stream, data->name, comment, "", (char *)report );
 if ( output_birth_head_group( stream, mode, data, table, house_mode,
						comment, dpk, global ) ) {
    goto er_close1;
    }
 rppl = get_report( table, global, CH__PLANET );
 rphs = get_report( table, global, CH__PLAN_HOUSE );
 rpah = get_report( table, global, CH__ASPECT_HD );
 rpas = get_report( table, global, CH__ASPECT );
 rpmsc = get_report( table, global, CH__MISC );
 if ( rppl == NULL || rphs == NULL || rpas == NULL || rpah == NULL || rpmsc == NULL ) {
    er_close2:
    close_tables( table, global );
    close_files( file );
    close_files_asp( aspfile );
    close_files( hsfile );
    close_files( midfile );
    return;
    }
 fputs("\n\n", stream );
 if ( ferror( stream ) ) {
    goto er_close2;
    }
 for ( i = 0, aiep = aie_ptr, inf = inf_ptr; i <= maxp; ++i, ++inf, ++aiep ) {
	 if ( !inf->calced )
		 continue;
	 if ( x_kb_check() )
	   break;
     print_entry( stream, inf, i, mode, file, aspfile, hsfile, midfile, aiep,
	       data->name, comment, table, global );
     if ( ferror( stream ) ) {
        goto er_close2;
        }
     }
 if ( mode & TEXT ) {
    close_files( file );
    close_files_asp( aspfile );
    close_files( hsfile );
    close_files( midfile );
    }
 if ( x_kb_check() )
    return;
 if ( mode & HOUSES ) {
    if ( mode & TEXT ) {
       if ( ( hsfile = open_files( SIGN_HOUSE_FILE ) ) == -1 )
	  mode &= ( 0xffff ^ TEXT );
       }
    if ( mode & HOUSES )
       out_do_house_cusps(stream,data->name, comment, "",
					table, global, house_cusps, CH__HOUSE_HEAD );
 if ( ( report = get_report( table, global, CH__HS_SGN_HED ) ) == NULL ) {
    er_close3:
    close_tables( table, global );
    close_files( hsfile );
    return;
    }
 else
    output_title( stream, data->name, comment, "", (char *)report );
 if ( ( report = get_report( table, global, CH__HOUSE_SIGN ) ) == NULL ) {
    goto er_close3;
    }
 if ( ( rphs = strdup( report )) == NULL )
    goto er_close3;
 for ( i = 1; i <= 12; ++i ) {
     if ( x_kb_check() )
	break;
     output_house_sign( stream, i, house_cusps[i], &cusp, &sign,
						     data->name, (char *)rphs );
     ruler = sign_rulers[sign];
	 if ( cusp == -1 ) {
	if ( mode & TEXT )
	   get_transfer_text( sign, i, hsfile, stream );
	   get_transfer_text( ruler, 13, hsfile, stream );
	}
     else {
	if ( mode & TEXT ) {
	   fprintf( stream, (char *)fmt8, sign_str( sign ) );
	   get_transfer_text( sign, i, hsfile, stream );
	   fprintf( stream, (char *)fmt8, sign_str( cusp ) );
	   get_transfer_text( cusp, i, hsfile, stream );
	   get_transfer_text( ruler, 13, hsfile, stream );
	   }
	}
     }
 if ( mode & TEXT )
    close_files( hsfile );
 }
 if ( x_kb_check() ) {
    close_tables( table, global );
    return;
    }
 close_tables( table, global );
 table = get_tables( SUMMARY_NAME, &global );
 if ( table == -1 )
    return;
 if ( mode & SUMMARY )
    print_summary(stream, mode, table, global, data->name, comment );
 close_tables( table, global );
}
