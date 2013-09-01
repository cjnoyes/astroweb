#include "lngdbl.h"

typedef struct ORBIT_ELE {      /* Orbital Data For Each Planet */
	double mean_long[4];    /* Mean Longitude--4 terms--Deg */
	double perhelion[4];    /* Mean Long. of the perhelion--4 terms--Deg */
	double eccentricity[4]; /* eccentricity--4 terms */
	double inclination[4];  /* inclination--4 terms--Deg */
	double ascend_node[4];  /* Long of the Ascending Node--4 terms--Deg */
	double semi_major;      /* Semi Major Axis in A.U. */
	} ORBIT_ELE;

typedef struct PLAN_ELE {       /* Elements of a Planet's Orbit corrected */
	LONG_DOUBLE mean_long;       /* Mean Longitude--PLAN(i,1)  */
	LONG_DOUBLE mean_daily;      /* Mean Daily Motion--PLAN(i,2) */
	LONG_DOUBLE perhelion;       /* Mean Longitude of the perhelion--PLAN(i,3) */
	LONG_DOUBLE eccentricity;    /* eccentricity--PLAN(i,4) */
	LONG_DOUBLE inclination;     /* inclination--PLAN(i,5) */
	LONG_DOUBLE ascend_node;     /* Long of the Ascending Node --PLAN(i,6)*/
	LONG_DOUBLE semi_major;      /* Semi Major Axis in A.U.--PLAN(i,7) */
	LONG_DOUBLE mean_anomaly;    /* The Mean Anomaly --MAP(i) */
	} PLAN_ELE;

typedef struct DELTA {
	LONG_DOUBLE longitude;
	LONG_DOUBLE radius_vector;
	LONG_DOUBLE semi_major;
	LONG_DOUBLE eccentricity;
	LONG_DOUBLE mean_long;
	LONG_DOUBLE mean_anomaly;
	LONG_DOUBLE helio_latitude;
	LONG_DOUBLE perhelion;
	} DELTA;

/* A Collection of temporary variables used by planets Jupiter - Neptune */

typedef struct TM_TYPE {
	LONG_DOUBLE j1, j2, j3;
	LONG_DOUBLE j4, j5, j6;
	LONG_DOUBLE j7, j8, j9;
	} TM_TYPE;

/* A Collection of variables used for Jupiter and saturn. Part of these
   variables are used only for saturn. These are included in this structure
   only because it's neater, easier and faster to create on the fly */

typedef struct T1_TYPE {
	LONG_DOUBLE cos_j3, sin_j3, sin_2j3, cos_2j3, sin_3j3, cos_3j3,
	     sin_4j3, cos_4j3;
	LONG_DOUBLE cos_j5, sin_j5, sin_2j5, cos_2j5;
	LONG_DOUBLE sin_j6;
	LONG_DOUBLE sin_j7, cos_j7, sin_2j7, cos_2j7, sin_3j7;
	LONG_DOUBLE cos_3j7, sin_4j7, cos_4j7, cos_5j7, sin_5j7;
	LONG_DOUBLE sin_2j8, cos_2j8, sin_3j8, cos_3j8;
	LONG_DOUBLE sin_j9, cos_j9, sin_2j9, cos_2j9;
	} T1_TYPE;


