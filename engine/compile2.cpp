#include "stdafx.h"
#include "astrow.h"
#include <stdlib.h>
#include <assert.h>
#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "flags.h"
#include "compute.fns"
#include "errors.fns"
#include "charcons.h"
#include <math.h>
#include "utlduad.h"
#include "fixstar.h"
#include "arabpt.h"
#include "setup.hpp"
#include "options.hpp"
#include <memory.h>
#include "compile.hpp"
#include "orbs.h"
#include "io.h"
#include "OptionsReader.h"
#include "files.h"
#include "DataReader.h"

#define debugLogOut(p) 

extern void mem_reclaim( AS_INF *aptr, AS_INF *alt_ptr, TRANS_DATA *td, BITS mode );

static int _break;

/*
void debugLogOut( char *p )
{
 char buf[100];
 FILE *stream;

 stream = fopen( "C:\\debugtest.txt", "at" );
 strcpy( buf, p );
 strcat( buf, "\n");
 fputs( buf, stream );
 fclose( stream );
}
*/

int x_kb_check()
{
 if ( _break )
    return( 1 );
 else
    return( 0 );
}


/* Function to test the value of errno, Basically as a function to
   reduce redundant code */

int ChartCompiler::testErrno()
{
 int e = errno; 
 if ( e && e != ENOENT ) {
	do_error("Compiling");
	return( 1 );
    }
 else
   return( 0 );

}

int ChartCompiler::setup()
{
	FILE *stream = getConfigFile();
	OptionsReader *rd = new OptionsReader(stream);
	int status =rd->readOptions(op);
	delete rd;
	stream = getInputFile();
	DataReader *dr = new DataReader(stream);
	int response = dr->readData();
	delete dr;
	unsigned int mode = op.getFlags();
	if ( mode & VERT_EAST && !(mode & NOBIRTHTIM) )
	   maxpt = EAST_POINT;
    else if ( !(mode & NOBIRTHTIM) )
       maxpt = PART_FORTU;
 else
       maxpt = SOUTH_NODE;
 return 0;
}


int ChartCompiler::runChart()
{
 setup();
 if ( !compile() || _break ) {
    cleanUp(0);
	//fclose(teststream);
    return( 0 );
    }
 writeData();
 cleanUp(1);
 return(1);
}


int ChartCompiler::cleanUp( int ok )
{
 static int runs;

 //load_orbs(NATAL_ORB); 
 mem_reclaim( birth_inf, alt_inf, &transit_data, op.getFlags() );
 return( 1 );
}

int doCompileChart(char *type) {
   ChartCompiler *comp = NULL;
   
   if (strcmpi(type,"natal")==0) {
	   comp = new NatalChartCompiler(Numeric);
   }
   else if (strcmpi(type,"compat")==0){
	   comp = new CompChartCompiler(Compatibility);
   }
   else if (strcmpi(type,"compos")==0){
	   comp = new CompChartCompiler(Compos);
   }
   else if (strcmpi(type,"progr")==0){
	   comp = new ProgChartCompiler(Progress);
   }
   else if (strcmpi(type,"trans")==0){
	   comp = new TransChartCompiler(Transits);
   }
   if (comp != NULL) {
      comp->runChart();
	  delete comp;
   }
   return 1;
}

