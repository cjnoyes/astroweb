#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "paths.hpp"

#pragma warning( once : 4135; once : 4136; once : 4100; once : 4309; disable : 4505 )

char *make_name( char *buf, char *name, int dir )
{
 if ( strrchr( name, ':' ) != NULL || strrchr( name, '\\' ) != NULL )
    strcpy( buf, name );
 else
    strcpy( buf, set_path( name, dir ) );
 return( buf );
}

char *replace_ext( char *buf, char *name, char *ext )
{
 char *dot;

 strcpy( buf, name );
 dot = strchr( buf, '.' );
 if ( dot != NULL )
    strcpy( dot, ext );
 else
    strcat( buf, ext );
 return( buf );
}
