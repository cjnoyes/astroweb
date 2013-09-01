#include "stdafx.h"
#include "astro.h"
#include "useri.h"
#include "aspect.h"
#include "errors.fns"
#include <string.h>
#include "paths.hpp"

int high_aspect()
{
 int i;

 if ( high_asp != -1 )
    return( high_asp );
 for ( i = SEPTILE; i >= 0; i-- ) {
     if ( asp_map[i] ) {
	high_asp = i+1;
	return( i+1 );
	}
     }
 high_asp = i;
 return( 0 );
}

#define ORB_SIZE ((sizeof(ASP) * SEPTILE) + SEPTILE + 6)

int load_orbs( int context )
{
 char name[75];
 FILE *stream;
 static int status = -1;

 strcpy( name, set_path("ASTRO.ORB", DATA_F) );
 if ( ( stream = fopen( name, OPENMODEREAD ) ) == NULL ) {
    do_error( name );
    return( 0 );
    }
 fseek(stream, context * ORB_SIZE, SEEK_SET );
 fread( aspect_def, sizeof(ASP), SEPTILE, stream );
 fread( asp_map, 1, SEPTILE, stream );
 fread( &other_orb, 1, 2, stream );
 fread( &hscsp_orb, 1, 2, stream );
 fread( &inc_hscsp, 1, 2, stream );
 fclose( stream );
 high_asp = -1;
 high_aspect();
 return( 1 );
}

int save_orbs( int context )
{
 char name[75];
 FILE *stream;

 strcpy( name, set_path("ASTRO.ORB", DATA_F) );
 if ( ( stream = fopen( name, "r+b" ) ) == NULL ) {
    do_error( name );
    return( 0 );
    }
 fseek(stream, context * ORB_SIZE, SEEK_SET );
 fwrite( aspect_def, sizeof(ASP), SEPTILE, stream );
 fwrite( asp_map, 1, SEPTILE, stream );
 fwrite( &other_orb, 1, 2, stream );
 fwrite( &hscsp_orb, 1, 2, stream );
 fwrite( &inc_hscsp, 1, 2, stream );
 fclose( stream );
 return( 1 );
}
