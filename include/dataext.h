#ifndef DATAEXT__H__
#define DATAEXT__H__


#pragma pack(2)


typedef struct TRANS_DATA {
    COUNT   num_transits;
    NUM     start_planet;
    NUM     end_planet;
    DATES   interval_date;
    } TRANS_DATA;

#define SECONDARY_PR 0
#define TERTIARY_PR  1
#define MINOR_PR     2

typedef struct PROGR_DATA {
    short     progress_system;
    DATES   offset;
    DATES   acd_date;
    TIM     acd_time;
    } PROGR_DATA;

typedef struct DIREC_DATA {
    short     direct_system;
    DATES     direction;
    short     total_minutes;
    } DIREC_DATA;
#define SAD_YEARDAY 0
#define SAD_YEARDEG 1
#define SAD_NABOD   2

typedef struct RETURN_DATA {
    short     planet;
    DATES   start;
    DATES   end;
    PROGR_DATA prog;
    short     is_prog;
    } RETURN_DATA;


#define SOLARCHART   0
#define FLATCHART    1
#define SUNRISE_CH   2
#define TRIAL_ERR    3
#define KNOWN_ANGL   4

typedef struct RECT_DATA {
    short    birth_time_known;
    short     rect_system;
    TIM     trial;
    short    am_pm;
    short     angle;
    short     sign;
    short     total_minutes;
    DEG_MIN degree;
    } RECT_DATA;

/* DATA_PACKET chart codes */

#define NATAL_CH 1
#define PROGR_CH 2
#define COMPA_CH 4
#define TRANS_CH 8
#define RETUR_CH 16
#define PRRTN_CH 32
#define HARMON_CH 64
#define RELOC_CH 128
#define COMPO_CH 256
#define FIXED_CH 512
#define ARABP_CH 1024
#define HELIO_CH 2048
#define PRGCS_CH 4096
#define CSIDTM_X 8192
#define XCSIDTM_ 16384
#define SOLAR_CH 16384+16384

typedef struct DATA_PACKET {
    int chart_code;
    RECT_DATA *rdp;
    TRANS_DATA *tdp;
    PROGR_DATA *pdp;
    RETURN_DATA *rtdp;
    DIREC_DATA  *ddp;
    struct LOC_DB *relocp;  /* must include useri.h before this file */
    RECT_DATA *crdp;
	int *harmonic;
	int *dial;
    } DATA_PACKET;

 typedef struct DATA_PACKET_EXT {
     int chart_code;
     RECT_DATA *rdp;
     TRANS_DATA *tdp;
     PROGR_DATA *pdp;
     RETURN_DATA *rtdp;
     DIREC_DATA  *ddp;
     struct LOC_DB *relocp;  /* must include useri.h before this file */
     RECT_DATA *crdp;
 	 int *harmonic;
 	 int *dial;
 	 BIRTH_DB *birth, *compat, *trans;
 	 RECT_DATA *birthrc,*compatrc, *transrc;
 	 LOC_DB *reloc;
 	 char *relocpers;
    } DATA_PACKET_EXT;

/*
typedef struct OPTIONS {
        unsigned int flags;
        int house_mode;
    int charts;
    int chart_code;
    } OPTIONS;
*/


#pragma pack()


extern PROGR_DATA progress_data;
extern TRANS_DATA transit_data;
extern DIREC_DATA solar_data;
extern RECT_DATA birth_rectd, comp_rectd, trans_rectd;
extern DATA_PACKET birth_dp, comp_dp, trans_dp, progr_dp, compos_dp,
    reloc_dp, fixed_dp, arabp_dp, comptr_dp, return_dp, solar_dp, harmonic_dp;
extern RETURN_DATA return_data;

#define TEXTF  0x1
#define GRAPH  0x2
#define DATAF  0x4
#define GRIDF  0x8
#define INTRP  0x10

#define DATACMF		0
#define DATAC1F		1
#define DATATAB		2
#define DATABIN		3
#define DATADAO     4
#define DATAODBC    5

#endif /* DATAEXT__H__ */
