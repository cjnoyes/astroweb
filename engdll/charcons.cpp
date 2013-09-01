#include "stdafx.h"
#include <stdio.h>
#include "charcons.h"

char _ERROR1[] = "ERROR!";
char _ERROR2[] = "ERROR!!";
char error1[] = "Error!";
char error2[] = "Error!!";
char warn1[]  = "Warning!";
char warn2[]  = "Warning!!";
char *oa[]    = { "Append", "Open", NULL };
char fil_dev_prompt[] = "Enter File Name or Device.";
char *housmod[] = { "Equal House", "Koch", "Placidus",
		    "M House", "Campanus",
		    "Regiomantanus", "Morinus", NULL };
char norml[] = "Direct", retrg[] = "Retrograde", still[] = "Still";
char *motstr[] = { norml, retrg, still };
char *apstr[] = { "AM", "PM", NULL };
char *rectstr[]    =  { "Solar Chart Method", "Flat Chart Method",
	"Sunrise Birth-Time", "Trial & Error", "Use Known Angle" };
char siderealstr[] = "Sidereal";
char tropicstr[]   = "Tropical";
char *timezones[] = { "Eastern Atlantic", "Atlantic Standard", "Eastern Standard",
   "Central Standard", "Mountain Standard", "Pacific Standard", "Hawaii",
   "Greenwich Mean", "Western European", "Eastern Europe", "Eurasia",
   "China", "Japan", "Indochina", "New Zealand", "E. Australia",
   "S. Australia", "W. Australia", "India", "Samoa", "Int'l Date Line",
   "Other", NULL };

char *speccodes[] = { "Standard Time", "War Time", "Daylight Savings",
	       "Double British Summer", "Summer Time", "Winter Time",
	       "Other", NULL };

char PLANET_FILE_name[] = "PLANETS";
char HOUSE_FILE_name[]       = "HOUSES";
char ASPECT_FILE_name[]      = "ASPECTS";
char SUMMARY_FILE_name[]     = "SUMMARY";
char COMPAT_HOUSES_name[]    = "COMPAHSE";
char COMPAT_SUMMARY_name[]   = "COMPASUM";
char TRANSITS_HOUSES_name[]  = "TRANSHSE";
char TRANSITS_ASPECTS_name[] = "TRANSASP";
char PROGRESS_ASPECTS_name[] = "PROGRASP";
char SIGN_HOUSE_FILE_name[]  = "SIGHOUSE";
char COMPAT_ASPECTS_name[]   = "COMPTASP";
char MIDPOINT_SIGN_name[]    = "MIDPTSIN";
char LOCATION_INDEX_name[]   = "NLOCATIN.NDX";
char LOCATION_DATA_name[]    = "NLOCATIN.DAT";
char BIRTHBAS_INDEX_name[]   = "NBIRTHBS.NDX";
char BIRTHBAS_DATA_name[]    = "NBIRTHBS.DAT";
char LOCATION_POPUP_name[]   = "NLOCATIN.LST";
char BIRTHBAS_POPUP_name[]   = "NBIRTHBS.LST";
char LOC_POPUP_INDEX_name[]  = "LPOPLIST.NDX";
char asfilelistname[] = "NASTFIL.LST";

char types[32][15] = { "", "Natal", "Compat", "Transits", "Progressed",
    "Numeric", "Sun Return", "Moon Return", "Relocation", "Composite",
    "Fixed Star", "Arabic Parts", "Heliocentric", "Problem", "Compos Transit",
    "", "Natal-Graph", "Compat-Graph", "Trans-Graph", "Prog-Graph",
    "Numer-Graph", "", "", "Reloc-Graph", "Compos-Graph", "", "", "", "",
    "CompTran-Gra" };

char long_types[][35] = { "", "Birth Output File", "Compatability Output File",
    "Transits Output File", "Progressed Birth Output File",
    "Numeric Birth Output File", "Solar Return File", "Moon Return File",
    "Relocation File",  "Composite Output File", "Fixed Star Output File",
    "Arabic Parts Output File", "Heliocentric Birth Output File",
    "Astrological Problem Output File", "Composite Transits File", "",
    "Birth Graphics File", "Compatability Graphics File",
    "Transits Graphics File", "Progressed Birth Graphics File",
    "Numeric Birth Graphics File", "", "", "Reloction Graphics File",
    "Composite Graphics File", "", "", "", "",
    "Composite Transits Graphics File" };
