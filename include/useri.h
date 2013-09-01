#ifndef _USERI____H_
#define _USERI____H_

// Edited for Win 32
/* header file for user interface. file "astro.h" must be called first */

/* field widths */
#define  O_NAME_WIDTH            35
#define  O_COMMENT_WIDTH         24
#define  O_COUNTRY_WIDTH         15
#define  O_STATE_WIDTH           15
#define  O_LOCATION_WIDTH        25

/* field widths */
#define  NAME_WIDTH	       50
#define  COMMENT_WIDTH	       50
#define  COUNTRY_WIDTH	       30
#define  STATE_WIDTH	       30
#define  LOCATION_WIDTH        40


/* file op codes */
#define DELETED_FILE           0
#define NATAL_FILE             1
#define COMPATIBILITY_FILE     2
#define TRANSITS_FILE          3
#define PROGRESSED_FILE        4
#define NUMERIC_FILE           5
#define SOLAR_RETURN_FILE      6
#define LUNAR_RETURN_FILE      7
#define RELOCATION_FILE        8
#define COMPOSITE_FILE         9
#define FIXED_FILE             10
#define ARABIC_FILE            11
#define HELIO_FILE             12
#define PROBLEM_FILE           13
#define COMPOTRAN_FILE         14
#define SOLARARC_FILE          15
#define HARMONIC_FILE          16
#define RETURN_W_RELOC         17 // fakes
#define RETURN_W_TRANS         18 // fakes
#define LAST_NON_GRAPH_OLD     16
#define LAST_NON_GRAPH		   25	
#define DELETED_GRAPH_FILE     0
#define NEW_GRA_DIFF           LAST_NON_GRAPH-LAST_NON_GRAPH_OLD;
#define NATAL_GRAPH_FILE       LAST_NON_GRAPH + NATAL_FILE
#define COMPATAB_GRAPH_FILE    LAST_NON_GRAPH + COMPATIBILITY_FILE
#define TRANSITS_GRAPH_FILE    LAST_NON_GRAPH + TRANSITS_FILE
#define PROGRESS_GRAPH_FILE    LAST_NON_GRAPH + PROGRESSED_FILE
#define NUMERIC_GRAPH_FILE     LAST_NON_GRAPH + NUMERIC_FILE
#define RELOC_GRAPH_FILE       LAST_NON_GRAPH + RELOCATION_FILE
#define COMPOS_GRAPH_FILE      LAST_NON_GRAPH + COMPOSITE_FILE
#define COMPTR_GRAPH_FILE      LAST_NON_GRAPH + COMPOTRAN_FILE
#define SOLAR_GRAPH_FILE       LAST_NON_GRAPH + SOLARARC_FILE
#define HARMONIC_GRAPH_FILE    LAST_NON_GRAPH + HARMONIC_FILE
#define LAST_GRAPH 50
#define NATAL_GRID_FILE       LAST_GRAPH + NATAL_FILE
#define COMPATAB_GRID_FILE    LAST_GRAPH + COMPATIBILITY_FILE
#define TRANSITS_GRID_FILE    LAST_GRAPH + TRANSITS_FILE
#define PROGRESS_GRID_FILE    LAST_GRAPH + PROGRESSED_FILE
#define NUMERIC_GRID_FILE     LAST_GRAPH + NUMERIC_FILE
#define RELOC_GRID_FILE       LAST_GRAPH + RELOCATION_FILE
#define COMPOS_GRID_FILE      LAST_GRAPH + COMPOSITE_FILE
#define COMPTR_GRID_FILE      LAST_GRAPH + COMPOTRAN_FILE
#define SOLAR_GRID_FILE       LAST_GRAPH + SOLARARC_FILE
#define HARMONIC_GRID_FILE    LAST_GRAPH + HARMONIC_FILE
#define FIXED_GRID_FILE       LAST_GRAPH + FIXED_FILE
#define ARABIC_GRID_FILE      LAST_GRAPH + ARABIC_FILE
// to do renumber new filelist ver and renumber
// to do need new fileheader version string

/* generalizations of op codes */
#define NON_GRA  -1
#define GRA_F     1
#define ALL_F     0
#define GRIDFX    2

#pragma pack(2)

typedef struct O_LOC_DB {                /* basic location database structure */
	char country[O_COUNTRY_WIDTH+1];
	char state[O_STATE_WIDTH+1];
	char location[O_LOCATION_WIDTH+1];
	DEG_MIN longitude;
	DEG_MIN latitude;
	NUM time_zone;
	NUM special_code;
	HRS hrs_offset;
	MINU min_offset;
	} O_LOC_DB;
					 /* New Format Expanded */
typedef struct LOC_DB { 	       /* basic location database structure */
	char country[COUNTRY_WIDTH+1];
	char state[STATE_WIDTH+1];
	char location[LOCATION_WIDTH+1];
	DEG_MIN longitude;
	DEG_MIN latitude;
	NUM time_zone;
	NUM special_code;
	HRS hrs_offset;
	MINU min_offset;
	char tzcodekey[5];
	} LOC_DB;



					 /* New Format Expanded */
typedef struct BIRTH_DB {	       /* birth database structure */
	char name[NAME_WIDTH+1];
	short  sex;
	char country[COUNTRY_WIDTH+1];
	char state[STATE_WIDTH+1];
	char location[LOCATION_WIDTH+1];
        NUM time_zone;
        NUM special_code;
        short am_pm;
        AS_DATA birth_data;
	char tzcodekey[5];
	} BIRTH_DB;

				       /* Old format */
typedef struct O_BIRTH_DB {              /* birth database structure */
	char name[O_NAME_WIDTH+1];
	char country[O_COUNTRY_WIDTH+1];
	char state[O_STATE_WIDTH+1];
	char location[O_LOCATION_WIDTH+1];
	NUM time_zone;
	NUM special_code;
	short am_pm;
	AS_DATA birth_data;
	char filler[16];     /* for earlier version file compatibility */
	} O_BIRTH_DB;



				       /* Old format */
typedef struct O_FILE_DB {               /* file database structure */
	char name[O_NAME_WIDTH+1];
	char comment[O_COMMENT_WIDTH+1];
	char file_name[14];
	NUM type;
	COUNT file_num;
	} O_FILE_DB;


				       /* Win16 Format Expanded */
typedef struct W16_FILE_DB {	       /* file database structure */
	char name[NAME_WIDTH+1];       /* this has been manually padded */
	char comment[COMMENT_WIDTH+1];
	char file_name[14];
	NUM type;
	COUNT file_num;
	short has_comments;
	short has_cover;
	unsigned char arch_dsk;
	char selected;
	} W16_FILE_DB;


				       /* Win32 Format Expanded file name*/
typedef struct FILE_DB {	       /* file database structure */
	char name[NAME_WIDTH+1];       /* this has been manually padded */
	char comment[COMMENT_WIDTH+1];
	char file_name[255];
	NUM type;
	COUNT file_num;
	short has_comments;
	short has_cover;
	unsigned char arch_dsk;
	char selected;
	} FILE_DB;

 typedef struct XPFILE_DB {	       /* file database structure */
	char name[NAME_WIDTH+1];       /* this has been manually padded */
	char comment[COMMENT_WIDTH+1];
	char file_name[255];
	NUM type;
	COUNT file_num;
	char has_comments;
	char has_cover;
	char arch_dsk;
	char selected;
	short folder;
	} XPFILE_DB;



extern BIRTH_DB birth_rec, comp_rec, trans_rec;
extern LOC_DB reloc_data;

#pragma pack()
#endif /* _USERI____H_  */

