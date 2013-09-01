#include "lngdbl.h"
/* Structure for carrying julian date type to all routines
   necessary. Reduces redundant calculations of julian centuries,
   jul_date squared etc.  */


typedef struct JULIAN {
	LONG_DOUBLE jd_base;     /* Raw Total Julian Date from date 0 */
	LONG_DOUBLE jd_1900;     /* Julian Day number from epoch 1900.0 */
	LONG_DOUBLE jd_cent;     /* Julian Centuries since 1900.0 */
	LONG_DOUBLE jd_cent_sq;  /* Julian Centuries Squared since 1900.0 */
	LONG_DOUBLE jdb_cent;     /* Julian Centuries */
	LONG_DOUBLE jdb_cent_sq;  /* Julian Centuries Squared */
	LONG_DOUBLE jd_1989;     /* Julian Day number from epoch 10/1/1989 */
	LONG_DOUBLE jd_cent1989;     /* Julian Centuries since 10/1/1989  */
	LONG_DOUBLE jd_cent_sq1989;  /* Julian Centuries Squared since 10/1/1989  */
	double jd_4000bc;
    } JULIAN;

