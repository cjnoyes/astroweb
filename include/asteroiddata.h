

typedef struct asteroid_elements {
  LONG_DOUBLE mean_anomaly[3];
  LONG_DOUBLE eccentricity[3];
  LONG_DOUBLE semi_major;
  LONG_DOUBLE perhelion[3];
  LONG_DOUBLE ascend_node[3];
  LONG_DOUBLE inclination[3];
} asteroid_elements;


typedef struct asteroid_data {
 LONG_DOUBLE mean_anomaly;
 LONG_DOUBLE eccentricity;
 LONG_DOUBLE semi_major;
 LONG_DOUBLE perhelion;
 LONG_DOUBLE ascend_node;
 LONG_DOUBLE inclination;
} asteroid_data;

#define xmul(x)  a = elements.x;\
   calc_data.x = fmodulus((d2r(a[0])+d2r(a[1]) * jul->jd_cent)+(d2r(a[2]) * jul->jd_cent_sq),TWO_PI)
