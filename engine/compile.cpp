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
#include  "setup.hpp"
#include <math.h>
#include "utlduad.h"
#include "fixstar.h"
#include "Arabpt.h"
#include <memory.h>
#include "compile.hpp"
#include "orbs.h"
#include <string.h>
#include "ChartWriter.h"
#include "files.h"

#define debugLogOut(p) 

extern void mem_reclaim( AS_INF *aptr, AS_INF *alt_ptr, TRANS_DATA *td, BITS mode );

unsigned short fixupbits( unsigned short mode, unsigned short excl, unsigned short incl )
{
 unsigned short andmask;

 andmask = 0xffff ^ excl;
 mode &= andmask;
 mode |= incl;
 return( mode );
}



int NatalChartCompiler::compile()
{
 memcpy(&dp,&birth_dp,sizeof(DATA_PACKET));
 BIRTH_DB *pb=&birth_rec;
 do_calc( birth_inf, &pb->birth_data,
	op.getFlags(), op.getHouseMode(), &dp );
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int NatalChartCompiler::writeData()
{
 FILE *stream = getOutputFile();
 ChartWriter *wrt = new ChartWriter(stream);
 wrt->writeNatal(maxpt);
 delete wrt;
 return( 1 );
}




int CompChartCompiler::compile()
{
 if ( chartType == Compatibility  ) {
	memcpy(&dp,&comp_dp,sizeof(DATA_PACKET));
    do_comp_calc( &birth_rec.birth_data, birth_inf, &comp_rec.birth_data,
	  alt_inf, op.getFlags(), op.getFlags(), op.getHouseMode(), &dp );
 }
 else {
	memcpy(&dp,&compos_dp,sizeof(DATA_PACKET));
    do_composite_calc( &birth_rec.birth_data, birth_inf, &comp_rec.birth_data,
	  alt_inf, op.getFlags(), op.getFlags(), op.getHouseMode(), &dp );
 }
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int CompChartCompiler::writeData()
{
 FILE *stream = getOutputFile();
 ChartWriter *wrt = new ChartWriter(stream);
 if ( chartType == Compatibility  ) {
	 wrt->writeCompat(maxpt);
 }
 else {
	 wrt->writeNatal(maxpt);
 }
 delete wrt;
 return( 1 );
}

int ProgChartCompiler::compile()
{
 memcpy(&dp,&progr_dp,sizeof(DATA_PACKET));
 do_full_progr( &birth_rec.birth_data, birth_inf, alt_inf,
    op.getFlags(), op.getFlags(), op.getHouseMode(), &dp );
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int ProgChartCompiler::writeData()
{
 FILE *stream = getOutputFile();
 ChartWriter *wrt = new ChartWriter(stream);
 wrt->writeProgr(maxpt);
 delete wrt;
 return( 1 );
}


int SolarChartCompiler::compile()
{
 memcpy(&dp,&solar_dp,sizeof(DATA_PACKET));
 do_full_direc( &birth_rec.birth_data, birth_inf, alt_inf,
    op.getFlags(), op.getHouseMode(), &dp );
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int SolarChartCompiler::writeData()
{
 return( 1 );
}

int HarmonicChartCompiler::compile()
{
 memcpy(&dp,&harmonic_dp,sizeof(DATA_PACKET));	
 harmonic_calc_chart( birth_inf, alt_inf, &birth_rec.birth_data,
    op.getFlags(), op.getHouseMode(), &dp );
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int HarmonicChartCompiler::writeData()
{
 return( 1 );
}

int TransChartCompiler::compile()
{
 memcpy(&dp,&trans_dp,sizeof(DATA_PACKET));
 BIRTH_DB *pb=&trans_rec;
 manage_transits( birth_inf, alt_inf, &birth_rec.birth_data,
	   &pb->birth_data, op.getFlags(), op.getFlags(), op.getHouseMode(), &dp);
 if ( testErrno() )
    return( 0 );
 else
    return( 1 );
}

int TransChartCompiler::writeData()
{
 FILE *stream = getOutputFile();
 ChartWriter *wrt = new ChartWriter(stream);
 wrt->writeTrans(maxpt);
 delete wrt;
 return( 1 );
}










