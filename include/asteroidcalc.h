// asteroidcalc.h: interface for the asteroidcalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTEROIDCALC_H__2186D9FF_C7AB_4500_80ED_1B12668D9F3A__INCLUDED_)
#define AFX_ASTEROIDCALC_H__2186D9FF_C7AB_4500_80ED_1B12668D9F3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class asteroidcalc  
{
asteroid_elements elements;
asteroid_data calc_data;
JULIAN *jul;
FILE *stream;
int count;
int index;
LONG_DOUBLE earth_long;
public:
	asteroidcalc(JULIAN *j, LONG_DOUBLE sun);
	BOOL calcnext(short &min, short &ret, double &longt, double &lati);
	virtual ~asteroidcalc() { if (stream != NULL ) fclose( stream ); }

};

#endif // !defined(AFX_ASTEROIDCALC_H__2186D9FF_C7AB_4500_80ED_1B12668D9F3A__INCLUDED_)
