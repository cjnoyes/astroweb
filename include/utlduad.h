#pragma once

#define IS_HOME       0x1
#define IS_DETRIMENT  0x2
#define IS_EXALTED    0x4
#define IS_EXACT      0x8     /* used with exalted and fall to indicate */
#define IS_FALL       0x10    /* exact degree */
#define IS_COMBUST    0x20
#define IS_RULER      0x40


typedef struct AS_INF_EXT {
	unsigned flags;
	int duad;
	int duad_ruler;
	int duad_ruler2;
	int navasma;
	int navasma_sign;
	int decan;
	int decan_ruler;
	int decan_ruler2;
	} AS_INF_EXT;


extern AS_INF_EXT birth_ext[], alt_ext[];
