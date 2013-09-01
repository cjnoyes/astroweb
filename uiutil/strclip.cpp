#include "stdafx.h"
#include <string.h>

#pragma warning( once : 4135; once : 4136; once : 4100; once : 4309; disable : 4505 )

char *strclip( char *pStr )
{
 int l = strlen( pStr );
 char *s=0;
 for ( s = (pStr+l)-1; l; l--, s-- ) {
     if ( *s != ' ' )
	    break;
     }
     if ( *(s+1) == ' ' )
       *(s+1) = '\0';
 return( pStr );
}
