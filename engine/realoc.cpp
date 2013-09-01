#include "stdafx.h"
#include <assert.h>
#include <string.h>
#pragma warning( once : 4135; once : 4136; once : 4100; once : 4309; disable : 4505 )

/* 09/10/92  Modified for C++ Windows */

/* special realloc function which uses new and delete */

void *reallocx( void *p, unsigned bytes )
{
 void *t;
 assert(bytes < 64000);
 t = new char[bytes];
 if ( t == NULL )
    return( NULL );
 memcpy( t, p, bytes );
 delete p;
 return( t );
}
