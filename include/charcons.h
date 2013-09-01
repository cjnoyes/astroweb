#ifndef _CHARCONS_H___
#define _CHARCONS_H___

#define CHARCONS_SEEN 1

#define ERROR1 "ERROR!"
#define ERROR2 "ERROR!!"

//extern char _ERROR1[];
//extern char _ERROR2[];
extern char error1[];
extern char error2[];
extern char warn1[];
extern char warn2[];

 extern char asfilelistname[];
 extern char long_types[][35];
 extern char types[][15];

extern char *oa[];
extern char fil_dev_prompt[];
extern char *housmod[];
extern char norml[], retrg[], still[];
extern char *motstr[];
#define motion_str( r ) (motstr[r])
extern char *apstr[];
extern char *rectstr[];
extern char siderealstr[];
extern char tropicstr[];
extern char *timezones[];
extern char *speccodes[];
extern char *angles[];




extern char  PLANET_FILE_name[];
extern char  HOUSE_FILE_name[];
extern char  ASPECT_FILE_name[];
extern char  SUMMARY_FILE_name[];
extern char  COMPAT_HOUSES_name[];
extern char  COMPAT_SUMMARY_name[];
extern char  TRANSITS_HOUSES_name[];
extern char  TRANSITS_ASPECTS_name[];
extern char  PROGRESS_ASPECTS_name[];
extern char  SIGN_HOUSE_FILE_name[];
extern char  COMPAT_ASPECTS_name[];
extern char  MIDPOINT_SIGN_name[];
extern char  LOCATION_INDEX_name[];
extern char  LOCATION_DATA_name[];
extern char  BIRTHBAS_INDEX_name[];
extern char  BIRTHBAS_DATA_name[];
extern char  LOCATION_POPUP_name[];
extern char  BIRTHBAS_POPUP_name[];
extern char  LOC_POPUP_INDEX_name[];
extern char ARABIC_SIGN_name[];
extern char ARABIC_HOUSE_name[];
extern char ARABIC_ASPECTS_name[];
extern char FIXEDST_SIGN_name[];
extern char FIXEDST_HOUSE_name[];
extern char FIXEDST_ASPECTS_name[];
extern char SOLAR_PLANET_name[];
extern char SOLAR_HOUSE_name[];
extern char SOLAR_ASPECT_name[];
extern char  CM_PLANET_FILE_name[];
extern char  CM_HOUSE_FILE_name[];
extern char  CM_ASPECT_FILE_name[];
extern char  CM_TRANSITS_HOUSES_name[];
extern char  CM_TRANSITS_ASPECTS_name[];

#endif /* _CHARCONS_H___ */
