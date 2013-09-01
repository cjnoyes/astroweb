#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include "astro.h"



char *make_date( DATES *dp )
{
  static char buf[20];
	
  sprintf(buf,"%d/%d/%d", dp->month, dp->day, dp->year);
  return buf;
}

char *make_time( TIM *tp, int am_pm )
{
	static char buf[20];
	static char *ampmstr[] = {"AM","PM"};
	
	sprintf(buf,"%d:%d:%d %s",tp->hours,tp->minutes,tp->seconds,ampmstr[am_pm]);
	return buf;
}
