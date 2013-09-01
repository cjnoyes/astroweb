#include "stdafx.h"
#include "astro.h"
#include "flags.h"
#include "useri.h"
#include "dataext.h"

/* 09/10/92  Modified for C++ Windows */

/* function to reclaim memory. Works by reclaiming all the memory
   allocated to midpoints, aspects, transits, and a progressed moon
   series if allocated */

void mem_reclaim( AS_INF *aptr, AS_INF *alt_ptr, TRANS_DATA *td, BITS mode )
{
 COUNT x, y;
 AS_INF **ptr, *pt, *nas, *alt_as;
 MIDPT *mdp;
 NUM maxp = EAST_POINT;
 if ( date_ptr != NULL ) {
    delete date_ptr;
    date_ptr = NULL;
    }
#ifdef DELUXE
 if ( moon_series != NULL ) {
    for ( x = 0, nas = moon_series; x < 16; ++x, ++nas ) {
	if ( !nas->calced )
		continue;
	if ( nas->no_aspects > 0 && mode & ASPECTS && nas->aspectr != NULL )
	   delete nas->aspectr;
        }      
    delete moon_series;
    moon_series = NULL;
    }
 if ( prog_moon_series != NULL ) {
    for ( x = 0, nas = prog_moon_series; x < 16; ++x, ++nas ) {
	    if ( !nas->calced )
		   continue;
		if ( nas->no_aspects > 0 && mode & ASPECTS && nas->aspectr != NULL )
	       delete nas->aspectr;
	}
    delete prog_moon_series;
    prog_moon_series = NULL;
    }
 if ( prog != NULL ) {
    for ( x = 1, nas = prog; x <= maxp; ++x, ++nas ) {
	    if ( !nas->calced )
		   continue;
		if ( nas->no_aspects > 0 && mode & ASPECTS && nas->aspectr != NULL )
	      delete nas->aspectr;
	}
    delete prog;
    prog = NULL;
    }
#endif
 if ( trans_ptr != NULL ) {
    for ( x = 1, ptr = trans_ptr; x < td->num_transits; ++x, ++ptr ) {
	for ( y = td->start_planet, pt = *ptr; y <= td->end_planet;
			     y++, pt++ ) {
	    if ( !pt->calced )
		   continue;
		if ( pt->no_aspects > 0 && mode & ASPECTS && pt->aspectr != NULL )
	       delete pt->aspectr;
            }       
	if ( *ptr != NULL )
	   delete *ptr;
       }
    if ( td->num_transits > 0 && *(trans_ptr+td->num_transits-1) != NULL )
       delete *(trans_ptr+td->num_transits-1);
    delete trans_ptr;
    trans_ptr = NULL;
    }
 for ( x = ASCEND, nas = aptr, alt_as = alt_ptr; x <= maxp;
                             ++x, ++nas, ++alt_as ) {
  #ifdef DELUXE
		if ( nas->calced && nas->no_midpt > 0 && mode & (BASE_MID | FULL_MID) ) {
        for ( y = 1, mdp = nas->midptr; y <= nas->no_midpt; ++y, ++mdp ) {
	       if ( mdp->no_aspect > 0 && mode & MID_ASPECT ) {
	          delete mdp->aspectr;
	          }
         }
	 }
     if ( nas->calced && nas->no_midpt > 0 && mode & (BASE_MID | FULL_MID) ) {
        for ( y = 1, mdp = nas->midptr; y <= nas->no_midpt; ++y, ++mdp ) {
	       if ( mdp->no_alt_aspect > 0 && mode & MID_ASPECT ) {
	          delete mdp->altaspectr;
	          }
        }
	    delete nas->midptr;
        nas->midptr = NULL;
        nas->no_midpt = 0; 
        }
	 if ( alt_as->calced && alt_as->no_midpt > 0 && mode & (BASE_MID | FULL_MID) ) {
	    for ( y = 1, mdp = alt_as->midptr; y <= alt_as->no_midpt; ++y, ++mdp ) {
            if ( mdp->no_aspect > 0 && mode & MID_ASPECT )
	           delete mdp->aspectr;
           }
	 }
	 if ( nas->calced && nas->no_midpt > 0 && mode & (BASE_MID | FULL_MID) ) {
        for ( y = 1, mdp = nas->midptr; y <= nas->no_midpt; ++y, ++mdp ) {
	       if ( mdp->no_alt_aspect > 0 && mode & MID_ASPECT ) {
	          delete mdp->altaspectr;
	          }
        }
	}
	if ( alt_as->midptr != NULL )
	   delete alt_as->midptr;
	alt_as->midptr = NULL;
	alt_as->no_midpt = 0;
   
#endif
	if ( nas->calced && nas->no_aspects > 0 && nas->aspectr != NULL ) {
	   delete nas->aspectr;
	   nas->aspectr = NULL;
       nas->no_aspects = 0;
       }
	if ( alt_as->calced && alt_as->no_aspects > 0 && alt_as->aspectr != NULL ) {
	  delete alt_as->aspectr;
	  alt_as->aspectr = NULL;
	  alt_as->no_aspects = 0;
      }
  }
}
