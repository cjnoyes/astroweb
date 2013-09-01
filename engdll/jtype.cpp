#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "plans.h"
#include <assert.h>
#include "utl.h"
#include "julian.h"
#include <errno.h>
#include "planpert.fns"
#include "errors.fns"
#include "lngdbl.h"

/* 09/10/92  Modified for C++ Windows */

#define JUPITER 6

#define unwind(x)  ((x) - floor( (x) / TWO_PI ) * TWO_PI)

TM_TYPE *jp = NULL;
T1_TYPE *j1p = NULL;

void remove_j(void)
{
 if ( jp != NULL ) {
    delete jp;
    jp = NULL;
    }
}

void remove_j1(void)
{
 if ( j1p != NULL ) {
    delete j1p;
    j1p = NULL;
    }
}

int make_j( JULIAN *j, int pno )
{
 TM_TYPE *p;

 if ( pno == JUPITER ) {
    p = new TM_TYPE;
    if ( p == NULL ) {
       errno = ENOMEM;
       return( 0 );
       }
    assert( p != NULL );
    p->j1 = j->jd_cent / 5.0 + 0.1;
    p->j2 = unwind( (LONG_DOUBLE) 4.14473 + 5.29691e1 * j->jd_cent );
    p->j3 = unwind( (LONG_DOUBLE)4.641118 + 2.132991e1 * j->jd_cent );
    p->j4 = unwind( (LONG_DOUBLE)4.250177 + 7.478172 * j->jd_cent );
    p->j5 = (5.0 * p->j3) - (2.0 * p->j2);
    p->j6 = (2.0 * p->j2) - (6.0 * p->j3) + (3.0 * p->j4);
    p->j7 = p->j3 - p->j2;
    p->j8 = p->j4 - p->j3;
    jp = p;
    }
 else {
    assert( jp != NULL );
    p = jp;
    p->j8 = unwind( 1.46205 + 3.81337 * j->jd_cent );
    p->j9 = 2.0 * p->j8 - p->j4;
    }
 return( 1 );
}

int make_j1( JULIAN *j, int pno )
{
 double t, mt;
 T1_TYPE *p;

 if ( !make_j( j, pno ) )
    return( 0 );
 if ( pno == JUPITER ) {
    p = new T1_TYPE;
    if ( p == NULL ) {
       errno = ENOMEM;
       remove_j();
       return( 0 );
       }
    t = jp->j3;
    p->cos_j3 = _cos( t );
    p->sin_j3 = _sin( t );
    mt = t + t;
    p->sin_2j3 = _sin( mt );
    p->cos_2j3 = _cos( mt );
    mt += t;
    p->sin_3j3 = _sin( mt );
    p->cos_3j3 = _cos( mt );
    mt += t;
    p->sin_4j3 = _sin( mt );
    p->cos_4j3 = _cos( mt );
    t = jp->j5;
    p->cos_j5 = _cos( t );
    p->sin_j5 = _sin( t );
    mt = t + t;
    p->sin_2j5 = _sin( mt );
    p->cos_2j5 = _cos( mt );
    p->sin_j6 = _sin( jp->j6 );
    t = jp->j7;
    p->cos_j7 = _cos( t );
    p->sin_j7 = _sin( t );
    mt = t + t;
    p->sin_2j7 = _sin( mt );
    p->cos_2j7 = _cos( mt );
    mt += t;
    p->sin_3j7 = _sin( mt );
    p->cos_3j7 = _cos( mt );
    mt += t;
    p->sin_4j7 = _sin( mt );
    p->cos_4j7 = _cos( mt );
    mt += t;
    p->sin_5j7 = _sin( mt );
    p->cos_5j7 = _cos( mt );
    }
 else
    p = j1p;
 t = jp->j8;
 mt = t + t;
 p->sin_2j8 = _sin( mt );
 p->cos_2j8 = _cos( mt );
 mt += t;
 p->sin_3j8 = _sin( mt );
 p->cos_3j8 = _cos( mt );
 if ( pno == JUPITER ) {
    j1p = p;
    return( 1 );
    }
 t = jp->j9;
 p->cos_j9 = _cos( t );
 p->sin_j9 = _sin( t );
 mt = t + t;
 p->sin_2j9 = _sin( mt );
 p->cos_2j9 = _cos( mt );
 return( 1 );
}
