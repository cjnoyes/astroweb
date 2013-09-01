/* codes for mode for operation */

#define SIDEREAL     1
#define BASE_MID     2
#define HALF_MID     4
#define FULL_MID     8
#define MID_ASPECT   16
#define HOUSES       32
#define ASPECTS      64
#define EXT_ASPECTS  128
#define SUMMARY      256
#define MISCELL      512
#define TEXT         1024
#define VERT_EAST    2048
#define NOBIRTHTIM   4096
#define EXT_CALCS    8192
#define ASTEROID     16384

/* codes for various house systems to use */

#define  EQUAL     0
#define  KOCH      1
#define  PLACIDUS  2
#define  M_HOUSE   3
#define  COMPANUS  4
#define  REGIOMONT 5
#define  MORINUS   6
#define  PORPHYRYX  7
#define  TOPOCENTRIC 8
#define  MERIDIAN   9
#define  IMPORT    11

// codes for polar handling ored with house system
#define  POLARKEEP 16
#define  POLARSWAP 32


/* CODES FOR MISC_CODE (AS_INF). MORE THAN ONE VALUE CAN BE 
   ORED TOGETHER. THIS IS ORED TOGETHER WITH THE DECANATES FOR
   THE SUN IN THE SUN SIGN */ 

#define    CRITICAL   4
#define    ANARET_DEG 8
#define    ANARET_PL  16
#define    HYLEGL_PL  32


