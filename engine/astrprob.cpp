#include "stdafx.h"
#include <io.h>
#include "astro.h"
#include "useri.h"
#include "paths.hpp"
#include "dataext.h"
#include "flags.h"
#include "aspect.h"
#include "astrprob.h"
#include "compute.fns"
#include "errors.fns"

/* 09/10/92  Modified for C++ Windows */

#define REPORT_NAME	"PROBLEM"

#define N_ASTR_PROB 358
#define N_RESULT    74
#define RES_LEN     35

#ifndef NDEBUG
int error_state;
int est[6];
#endif

/* Note 99 as planet or arg3 indicates ignored arg

   This stuff is now in MKASTRPR.C --> written to PROBLEM.DAT

ASTR_PROBLEM far problems[N_ASTR_PROB+2] = {
   NEPTUNE,     SIGN,      ARIES,        99,        OR,
   SUN,         SIGN,      TAURUS,       99,        OR,
   MOON,        HOUSE,     1,            99,        5,
   JUPITER,     SIGN,      CANCER,       99,        OR,
   ASCEND,      SIGN,      SCORPIO,      99,        6,
   SUN,         ASPECT,    NEPTUNE,      CONJUNCT,  AND,
   SUN,         PLAHSE,    NEPTUNE,      11,        6,
   MOON,        SIGNHSE,   SCORPIO,      2,         6,
   MARS,        HOUSE,     1,            99,        OR,
   JUPITER,     SIGNHSE,   LIBRA,        1,         7,
   MOON,        PLAHSE,    MERCURY,      1,        AND,
   JUPITER,     HOUSE,     1,            99,        7,
   JUPITER,     SIGNHSE,   SAGITARIUS,   3,         7,
   99,          HSECSP,    9,            LIBRA,     OR,
   VENUS,       SIGNHSE,   LIBRA,        1,         OR,
   MOON,        SIGNHSE,   AQUARIUS,     12,        8,
   MOON,        SIGN,      LEO,          99,        OR,
   NEPTUNE,     SIGN,      ARIES,        99,        OR,
   SUN,         PLAHSE,    NEPTUNE,      12,        OR,
   SUN,         SIGNHSE,   TAURUS,       12,        9,
   JUPITER,     SIGNHSE,   LIBRA,        2,         10,
   SUN,         SIGNHSE,   ARIES,        10,        AND,
   MERCURY,     SIGNHSE,   ARIES,        10,        10,
   MOON,        HOUSE,     3,            99,        OR,
   MOON,        ASPECT,    SATURN,       CONJUNCT,  OR,
   VENUS,       HOUSE,     10,           99,        10,
   NEPTUNE,     SIGN,      ARIES,        99,        11,
   SUN,         PLAHSE,    MARS,         3,         AND,
   SUN,         PLASIGN,   MARS,         SCORPIO,   11,
   MARS,        ASPECT,    ASCEND,       CONJUNCT,  11,
   SATURN,      SIGN,      SAGITARIUS,   99,        12,
   SUN,         PLAHSE,    MERCURY,      9,         AND,
   SUN,         PLASIGN,   MERCURY,      GEMINI,    12,
   SUN,         HOUSE,     10,           99,        12,
   MOON,        SIGNHSE,   TAURUS,       9,         OR,
   MOON,        SIGN,      LEO,          99,        OR,
   MARS,        SIGN,      CAPRICORN,    99,        OR,
   MOON,        PLAHSE,    SATURN,       5,         OR,
   SUN,         HOUSE,     10,           99,        13,
   MERCURY,     SIGN,      SAGITARIUS,   99,        OR,
   MARS,        SIGNHSE,   ARIES,        10,        14,
   VENUS,       PLAHSE,    SATURN,       1,         AND,
   URANUS,      HOUSE,     1,            99,        14,
   SUN,         SIGNHSE,   TAURUS,       11,        14,
   MOON,        SIGN,      TAURUS,       99,        OR,
   MARS,        SIGN,      CAPRICORN,    99,        OR,
   LEO,         HOUSE,     1,            99,        15,
   JUPITER,     ASPECT,    SATURN,       CONJUNCT,  AND,
   JUPITER,     PLAHSE,    SATURN,       1,         15,
   JUPITER,     PLASIGN,   MERCURY,      SAGITARIUS,AND,
   VENUS,       SIGN,      SAGITARIUS,   99,        16,
   SUN,         PLAHSE,    MERCURY,      10,        AND,
   SUN,         PLASIGN,   MERCURY,      ARIES,     16,
   MARS,        SIGNHSE,   SCORPIO,      12,        OR,
   VENUS,       HOUSE,     10,           99,        OR,
   MARS,        SIGNHSE,   GEMINI,       7,         16,
   ASCEND,      SIGN,      SAGITARIUS,   99,        OR,
   VENUS,       SIGN,      VIRGO,        99,        OR,
   SATURN,      PLAHSE,    URANUS,       1,         OR,
   MOON,        SIGNHSE,   LEO,          10,        17,
   VENUS,       SIGNHSE,   SCORPIO,      12,        AND,
   SATURN,      SIGNHSE,   SCORPIO,      12,        AND,
   URANUS,      SIGNHSE,   SCORPIO,      12,        18,
   MERCURY,     SIGNHSE,   ARIES,        4,         OR,
   MOON,        SIGNHSE,   CAPRICORN,    8,         OR,
   MARS,        HOUSE,     4,            99,        18,
   SUN,         PLASIGN,   VENUS,        LEO,       19,
   SUN,         SIGNHSE,   LIBRA,        3,         AND,
   URANUS,      SIGNHSE,   LIBRA,        3,         19,
   SUN,         HOUSE,     10,           99,        OR,
   MOON,        ASPECT,    VENUS,        TRINE,     19,
   MOON,        PLASIGN,   SATURN,       GEMINI,    AND,
   MARS,        SIGN,      GEMINI,       99,        20,
   URANUS,      HOUSE,     3,            99,        OR,
   MERCURY,     HOUSE,     10,           99,        20,
   MOON,        SIGN,      PISCES,       99,        OR,
   99,          HSECSP,    12,           SAGITARIUS, OR,
   SATURN,      ASPECT,    NEPTUNE,      SQUARED,   21,
   99,          HSECSP,    9,            ARIES,     OR,
   JUPITER,     SIGN,      AQUARIUS,     99,        OR,
   99,          HSECSP,    7,            PISCES,    22,
   URANUS,      HOUSE,     10,           99,        AND,
   URANUS,      ASPECT,    SUN,          OPPOSITION, AND,
   URANUS,      ASPECT,    MOON,         OPPOSITION, AND,
   URANUS,      ASPECT,    MARS,         OPPOSITION, 22,
   SUN,         SIGNHSE,   CAPRICORN,    2,         AND,
   MERCURY,     SIGNHSE,   CAPRICORN,    2,         23,
   MERCURY,     PLAHSE,    VENUS,        10,        23,
   MOON,        SIGNHSE,   VIRGO,        5,         AND,
   SATURN,      SIGNHSE,   VIRGO,        5,         23,
   MARS,        PLASIGN,   URANUS,       CANCER,    OR,
   JUPITER,     SIGNHSE,   VIRGO,        4,         OR,
   MARS,        SIGN,      CANCER,       99,        24,
   NEPTUNE,     SIGNHSE,   GEMINI,       10,        OR,
   URANUS,      SIGN,      VIRGO,        99,        25,
   SUN,         PLAHSE,    MERCURY,      3,         AND,
   URANUS,      HOUSE,     3,            99,        25,
   URANUS,      HOUSE,     8,            99,        25,
   GEMINI,      PLASIGN,   NEPTUNE,      GEMINI,    26,
   SUN,         PLAHSE,    MERCURY,      12,        AND,
   VENUS,       HOUSE,     12,           99,        26,
   NEPTUNE,     HOUSE,     7,            99,        AND,
   MERCURY,     ASPECT,    NEPTUNE,      CONJUNCT,  AND,
   MERCURY,     HOUSE,     7,            99,        26,
   SATURN,      SIGN,      VIRGO,        99,        27,
   VENUS,       SIGNHSE,   SCORPIO,      1,         AND,
   SATURN,      SIGNHSE,   SCORPIO,      1,         AND,
   URANUS,      SIGNHSE,   SCORPIO,      1,         27,
   MOON,        SIGN,      SAGITARIUS,   99,        27,
   NEPTUNE,     SIGN,      ARIES,        99,        OR,
   ASCEND,      SIGN,      SCORPIO,      99,        28,
   SUN,         PLAHSE,    NEPTUNE,      11,        AND,
   SUN,         ASPECT,    NEPTUNE,      CONJUNCT,  28,
   SUN,         ASPECT,    MED_COELI,    CONJUNCT,  OR,
   MERCURY,     ASPECT,    MED_COELI,    CONJUNCT,  28,
   SUN,         PLASIGN,   VENUS,        VIRGO,     AND,
   MERCURY,     SIGN,      VIRGO,        99,        29,
   JUPITER,     SIGNHSE,   PISCES,       11,        29,
   VENUS,       SIGNHSE,   SCORPIO,      12,        AND,
   SATURN,      SIGNHSE,   SCORPIO,      12,        AND,
   URANUS,      SIGNHSE,   SCORPIO,      12,        29,
   SUN,         PLASIGN,   MARS,         VIRGO,     AND,
   MERCURY,     SIGN,      VIRGO,        99,        30,
   MARS,        SIGN,      CAPRICORN,    99,        OR,
   URANUS,      HOUSE,     3,            99,        OR,
   MERCURY,     HOUSE,     10,           99,        30,
   MOON,        SIGN,      SAGITARIUS,   99,        31,
   URANUS,      SIGNHSE,   AQUARIUS,     8,         AND,
   JUPITER,     SIGNHSE,   AQUARIUS,     8,         31,
   JUPITER,     PLAHSE,    SATURN,       9,         OR,
   MOON,        SIGN,      SCORPIO,      99,        31,
   JUPITER,     SIGNHSE,   PISCES,       11,        32,
   JUPITER,     SIGNHSE,   CANCER,       12,        32,
   NEPTUNE,     HOUSE,     10,           99,        32,
   JUPITER,     HOUSE,     1,            99,        AND,
   JUPITER,     ASPECT,    SATURN,       CONJUNCT,  AND,
   SATURN,      ASPECT,    URANUS,       SQUARED,   AND,
   JUPITER,     ASPECT,    URANUS,       SQUARED,   32,
   SUN,         PLASIGN,   JUPITER,      ARIES,     33,
   JUPITER,     SIGN,      AQUARIUS,     99,        33,
   PLUTO,       SIGNHSE,   CAPRICORN,    9,         33,
   VENUS,       SIGNHSE,   GEMINI,       1,         AND,
   MERCURY,     SIGNHSE,   GEMINI,       1,         34,
   URANUS,      PLASIGN,   MARS,         CANCER,    OR,
   URANUS,      HOUSE,     3,            99,        OR,
   MOON,        SIGNHSE,   CANCER,       3,         34,
   JUPITER,     SIGN,      ARIES,        99,        OR,
   URANUS,      SIGN,      CANCER,       99,        35,
   VENUS,       PLAHSE,    SATURN,       2,         AND,
   JUPITER,     HOUSE,     2,            99,        35,
   SATURN,      SIGN,      SCORPIO,      99,        35,
   PLUTO,       SIGN,      LIBRA,        99,        OR,
   MARS,        SIGN,      GEMINI,       99,        36,
   SUN,         PLAHSE,    MOON,         3,         AND,
   MARS,        HOUSE,     3,            99,        36,
   MARS,        SIGN,      VIRGO,        99,        36,
   MARS,        SIGN,      SCORPIO,      99,        OR,
   MARS,        SIGN,      ARIES,        99,        OR,
   NEPTUNE,     SIGN,      GEMINI,       99,        OR,
   URANUS,      SIGNHSE,   AQUARIUS,     11,        37,
   MOON,        SIGN,      ARIES,        99,        OR,
   NEPTUNE,     HOUSE,     3,            99,        OR,
   MOON,        SIGN,      CANCER,       99,        38,
   MERCURY,     SIGNHSE,   AQUARIUS,     10,        OR,
   SUN,         PLASIGN,   MARS,         CANCER,    OR,
   MOON,        PLAHSE,    VENUS,        2,         OR,
   JUPITER,     SIGNHSE,   LIBRA,        6,         39,
   MARS,        SIGN,      CAPRICORN,    99,        OR,
   MARS,        HOUSE,     1,            99,        OR,
   SATURN,      SIGNHSE,   SAGITARIUS,   4,         40,
   SUN,         PLASIGN,   MOON,         TAURUS,    AND,
   VENUS,       SIGN,      TAURUS,       99,        41,
   MARS,        PLASIGN,   JUPITER,      LEO,       AND,
   MOON,        SIGN,      LEO,          99,        AND,
   MARS,        PLAHSE,    JUPITER,      2,         AND,
   MOON,        HOUSE,     2,            99,        41,
   SUN,         PLAHSE,    MOON,         2,         AND,
   VENUS,       PLAHSE,    MARS,         2,         41,
   JUPITER,     HOUSE,     11,           99,        41,
   NEPTUNE,     SIGN,      CANCER,       99,        42,
   SUN,         PLASIGN,   MERCURY,      PISCES,    AND,
   SATURN,      SIGN,      PISCES,       99,        42,
   MERCURY,     ASPECT,    MARS,         SQUARED,   AND,
   MERCURY,     HOUSE,     3,            99,        42,
   VENUS,       SIGN,      PISCES,       99,        42,
   SUN,         PLASIGN,   VENUS,        LEO,       OR,
   ASCEND,      SIGN,      SCORPIO,      99,        OR,
   MOON,        SIGNHSE,   CAPRICORN,    3,         OR,
   MOON,        ASPECT,    SATURN,       CONJUNCT,  43,
   MERCURY,     PLASIGN,   SUN,          CANCER,    AND,
   MOON,        SIGN,      CANCER,       99,        44,
   JUPITER,     SIGNHSE,   SAGITARIUS,   4,         44,
   MOON,        ASPECT,    VENUS,        TRINE,     44,
   SUN,         PLASIGN,   MERCURY,      SCORPIO,   AND,
   MOON,        SIGN,      SCORPIO,      99,        45,
   NEPTUNE,     PLASIGN,   MERCURY,      PISCES,    AND,
   VENUS,       PLASIGN,   MOON,         PISCES,    45,
   JUPITER,     SIGN,      VIRGO,        99,        OR,
   MARS,        SIGN,      PISCES,       99,        45,
   MERCURY,     PLASIGN,   JUPITER,      ARIES,     AND,
   PLUTO,       SIGN,      ARIES,        99,        46,
   VENUS,       SIGN,      GEMINI,       99,        46,
   SUN,         PLAHSE,    MERCURY,      3,         AND,
   VENUS,       PLAHSE,    MARS,         3,         AND,
   NEPTUNE,     PLAHSE,    URANUS,       3,         AND,
   SUN,         PLASIGN,   MERCURY,      CAPRICORN, AND,
   VENUS,       PLASIGN,   MARS,         CAPRICORN, AND,
   NEPTUNE,     PLASIGN,   URANUS,       CAPRICORN, 46,
   MARS,        HOUSE,     1,            99,        46,
   VENUS,       SIGN,      GEMINI,       99,        47,
   ASCEND,      SIGN,      SCORPIO,      99,        47,
   URANUS,      SIGNHSE,   CANCER,       3,         47,
   MARS,        ASPECT,    MED_COELI,    CONJUNCT,  47,
   PLUTO,       PLASIGN,   JUPITER,      ARIES,     48,
   SUN,         PLASIGN,   MERCURY,      CAPRICORN, AND,
   VENUS,       SIGN,      CAPRICORN,    99,        48,
   SUN,         HOUSE,     1,            99,        OR,
   SUN,         HOUSE,     2,            99,        48,
   MERCURY,     PLASIGN,   MOON,         GEMINI,    AND,
   VENUS,       SIGN,      GEMINI,       99,        49,
   VENUS,       SIGN,      LIBRA,        99,        49,
   MOON,        HOUSE,     3,            99,        49,
   MOON,        SIGN,      LEO,          99,        49,
   SATURN,      SIGN,      LIBRA,        99,        50,
   MERCURY,     PLASIGN,   VENUS,        PISCES,    AND,
   SATURN,      SIGN,      PISCES,       99,        50,
   NEPTUNE,     SIGNHSE,   AQUARIUS,     3,         50,
   MOON,        SIGN,      AQUARIUS,     99,        50,
   SUN,         PLASIGN,   VENUS,        TAURUS,    51,
   JUPITER,     PLASIGN,   MARS,         LEO,       AND,
   URANUS,      SIGN,      LEO,          99,        51,
   SUN,         HOUSE,     1,            99,        OR,
   MARS,        HOUSE,     1,            99,        51,
   JUPITER,     SIGN,      CANCER,       99,        52,
   SUN,         PLAHSE,    VENUS,        1,         52,
   JUPITER,     PLASIGN,   MARS,         CAPRICORN, AND,
   SATURN,      SIGN,      CAPRICORN,    99,        52,
   MARS,        SIGNHSE,   CANCER,       11,        53,
   SUN,         PLASIGN,   MERCURY,      PISCES,    AND,
   PLUTO,       SIGNHSE,   PISCES,       12,        AND,
   SUN,         PLAHSE,    MERCURY,      12,        53,
   MARS,        ASPECT,    SATURN,       SQUARED,   53,
   MOON,        PLASIGN,   MARS,         CANCER,    AND,
   SATURN,      SIGN,      CANCER,       99,        54,
   SUN,         PLASIGN,   MERCURY,      VIRGO,     54,
   SUN,         PLAHSE,    NEPTUNE,      1,         AND,
   MERCURY,     HOUSE,     1,            99,        54,
   URANUS,      SIGN,      GEMINI,       99,        54,
   VENUS,       SIGN,      ARIES,        99,        55,
   MOON,        SIGNHSE,   CAPRICORN,    6,         55,
   VENUS,       PLAHSE,    JUPITER,      12,        55,
   NEPTUNE,     HOUSE,     12,           99,        55,
   JUPITER,     PLASIGN,   SATURN,       SAGITARIUS,56,
   VENUS,       PLASIGN,   PLUTO,        PISCES,    AND,
   MOON,        SIGN,      PISCES,       99,        56,
   VENUS,       SIGNHSE,   ARIES,        12,        56,
   MOON,        HOUSE,     2,            99,        56,
   SUN,         PLASIGN,   MERCURY,      CANCER,    AND,
   PLUTO,       SIGN,      CANCER,       99,        57,
   SUN,         PLASIGN,   MERCURY,      LEO,       AND,
   VENUS,       SIGN,      LEO,          99,        57,
   PLUTO,       SIGNHSE,   CANCER,       4,         57,
   JUPITER,     SIGN,      GEMINI,       99,        57,
   NEPTUNE,     SIGN,      VIRGO,        99,        58,
   JUPITER,     SIGN,      SAGITARIUS,   99,        58,
   JUPITER,     SIGNHSE,   SAGITARIUS,    2,         58,
   JUPITER,     SIGN,      AQUARIUS,     99,        58,
   SUN,         PLASIGN,   MERCURY,      TAURUS,    AND,
   SATURN,      PLASIGN,   URANUS,       TAURUS,    59,
   MOON,        PLASIGN,   PLUTO,        LEO,       59,
   SATURN,      SIGNHSE,   SAGITARIUS,   2,         59,
   JUPITER,     HOUSE,     11,           99,        59,
   ASCEND,      SIGN,      CANCER,       99,        60,
   MOON,        SIGNHSE,   PISCES,        2,        60,
   MOON,        HOUSE,     4,             99,       60,
   SUN,         ASPECT,    MOON,          SQUARED,  60,
   JUPITER,     SIGN,      GEMINI,        99,       61,
   MOON,        PLASIGN,   VENUS,         LIBRA,    61,
   URANUS,      SIGNHSE,   PISCES,        3,        61,
   MOON,        SIGN,      AQUARIUS,      99,       61,
   URANUS,      SIGN,      VIRGO,         99,       62,
   JUPITER,     SIGN,      AQUARIUS,      99,       62,
   SUN,         PLAHSE,    NEPTUNE,       9,        62,
   SUN,         ASPECT,    MERCURY,       CONJUNCT, AND,
   SUN,         PLASIGN,   MERCURY,       SCORPIO,  62,
   SUN,         SIGNHSE,   ARIES,         2,        63,
   SATURN,      SIGN,      LEO,           99,       63,
   JUPITER,     HOUSE,     9,             99,       63,
   MERCURY,     SIGN,      SCORPIO,       99,       63,
   SUN,         PLASIGN,   MERCURY,       ARIES,    AND,
   VENUS,       PLASIGN,   MARS,          ARIES,    64,
   ASCEND,      SIGN,      SCORPIO,       99,       64,
   URANUS,      SIGNHSE,   AQUARIUS,      8,        64,
   MARS,        ASPECT,    NEPTUNE,       OPPOSITION, 64,
   VENUS,       SIGN,      PISCES,        99,       65,
   MARS,        PLAHSE,    MERCURY,       1,        AND,
   JUPITER,     PLAHSE,    NEPTUNE,       1,        65,
   MOON,        PLASIGN,   MERCURY,       TAURUS,   65,
   VENUS,       PLASIGN,   URANUS,        CAPRICORN,AND,
   NEPTUNE,     SIGN,      CAPRICORN,     99,       66,
   SATURN,      SIGN,      PISCES,        99,       66,
   SUN,         PLAHSE,    JUPITER,       1,        AND,
   VENUS,       HOUSE,     1,             99,       66,
   SATURN,      SIGN,      AQUARIUS,      1,        66,
   MOON,        SIGN,      CANCER,        99,       67,
   URANUS,      PLASIGN,   PLUTO,         ARIES,    67,
   SATURN,      SIGN,      LIBRA,         99,       67,
   JUPITER,     SIGN,      ARIES,         99,       68,
   MOON,        SIGNHSE,   CAPRICORN,     6,        68,
   SUN,         PLAHSE,    VENUS,         3,        AND,
   SUN,         PLASIGN,   VENUS,         LEO,      68,
   SATURN,      HOUSE,     11,            99,       68,
   MOON,        SIGN,      GEMINI,        99,       69,
   JUPITER,     PLASIGN,   MARS,          LIBRA,    69,
   SUN,         PLASIGN,   URANUS,        SAGITARIUS, AND,
   SUN,         PLAHSE,    URANUS,        3,        69,
   SATURN,      SIGN,      CAPRICORN,     99,       69,
   MARS,        PLASIGN,   URANUS,        GEMINI,   AND,
   SATURN,      SIGN,      GEMINI,        99,       70,
   MARS,        HOUSE,     10,            99,       AND,
   MARS,        ASPECT,    NEPTUNE,       CONJUNCT, 70,
   NEPTUNE,     HOUSE,     10,            99,       AND,
   MARS,        ASPECT,    NEPTUNE,       CONJUNCT, 70,
   JUPITER,     PLASIGN,   PLUTO,         CAPRICORN, 71,
   JUPITER,     PLASIGN,   NEPTUNE,       VIRGO,     AND,
   MARS,        SIGN,      VIRGO,         99,        72,
   SUN,         PLAHSE,    MERCURY,       10,        AND,
   JUPITER,     PLAHSE,    SATURN,        10,        72,
   SATURN,      SIGN,      VIRGO,         99,        72,
   SUN,         PLASIGN,   VENUS,         SCORPIO,   AND,
   MERCURY,     SIGN,      SCORPIO,       99,        73,
   MARS,        SIGN,      ARIES,         99,        73,
   SUN,         PLASIGN,   VENUS,         LEO,       AND,
   SUN,         PLAHSE,    VENUS,         9,         73,
   MARS,        SIGN,      CAPRICORN,     99,        73,
   MOON,        PLASIGN,   JUPITER,       TAURUS,    AND,
   PLUTO,       SIGN,      TAURUS,        99,        74,
   SUN,         PLASIGN,   JUPITER,       PISCES,    74,
   SUN,         PLAHSE,    PLUTO,         12,        AND,
   MERCURY,     HOUSE,     12,            99,        74,
   NEPTUNE,     SIGN,      SCORPIO,       99,        74,
   MARS,        ASPECT,    SATURN,        SQUARED,   74,
   SATURN,      SIGN,      LEO,           99,        75,
   URANUS,      SIGN,      VIRGO,         99,        75,
   MOON,        PLAHSE,    SUN,           4,         AND,
   MARS,        PLAHSE,    URANUS,        4,         75,
   MOON,        ASPECT,    SATURN,        CONJUNCT,  75,
   URANUS,      SIGN,      TAURUS,        99,        76,
   MERCURY,     PLASIGN,   MARS,          VIRGO,     76,
   MARS,        HOUSE,     6,             99,        76,
   MOON,        SIGN,      CAPRICORN,     99,        76,
   SUN,         PLASIGN,   MERCURY,       ARIES,     AND,
   SATURN,      SIGN,      ARIES,         99,        77,
   SUN,         PLASIGN,   MERCURY,       CAPRICORN, AND,
   MARS,        SIGN,      CAPRICORN,     99,        77,
   SUN,         PLASIGN,   MERCURY,       LEO,       AND,
   SUN,         PLAHSE,    MERCURY,       9,         77,
   URANUS,      SIGN,      SCORPIO,       99,        77 };

Now in table text PROBRES.TBL

static char far prob_result[N_RESULT][RES_LEN] = {"Abruptness",
 "Alarmist Nature","Anger","Anguish","Arrogance","Argumentativeness",
 "Assertiveness (excessive)","Attention-Seeking","Authoritative Manner",
 "Boisterousness","Boring Manner","Carelessness","Complaining Manner",
 "Compulsiveness","Conceit","Concentration (lack of ability)","Confusion",
 "Controversy","Conventional Manner","Cowardice","Critical Manner",
 "Deception (vulnerabilty towards)","Discontentment","Eccentric Manner",
 "Emotional Nature","Excessive Logic","Exhibitionism","Failure",
 "Fantasy Orientation","Fickleness","Financial Inneptness","Flightiness",
 "Flirtatiousness","Forgetfulness","Frugality","Gloominess","Greed",
 "Gullibility","Harshness","Home Life Problems", "Hostility", "Impatience",
 "Impulsiveness", "Inconsiderateness", "Inconsistency", "Indecision",
 "Independence", "Indescretion", "Indiscrimination", "Inhibitions",
 "Insecurity", "Insensitivity", "Irresponsibility", "Jealosy", "Materialism",
 "Mercenary Nature", "Moodiness", "Nervousness", "Oppinionated Nature",
 "Outspokeness", "Overwhelming Manner", "Passivity", "Pessimism",
 "Possessiveness", "Prejudice", "Procrastination", "Promiscuity",
 "Puritanical Nature","Quarrelsome Manner", "Quiet Manner", "Resentful",
 "Resistance", "Follower of Routine", "Sarcastic" };
*/

static const char *rps[8];

int analyze_problem_item( AS_INF *asp, ASTR_PROBLEM *probp )
{
 AS_INF *pl = asp + probp->planet;
 short aspn;
 extern MINU house_cusps[];

 switch( probp->arg_type ) {
       case HOUSE :
	    if ( pl->house == probp->arg2 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case ASPECT :
	    if ( find_aspect( pl->minutes_total,
		 (asp+probp->arg2)->minutes_total, &aspn, 0 ) == NO_ASPECT )
	       return( 0 );
	    if ( aspn == probp->arg3 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case SIGN :
	    if ( pl->sign == probp->arg2 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case SIGNHSE :
	    if ( pl->sign == probp->arg2 && pl->house == probp->arg3 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case HSECSP :
	    if ( sign( house_cusps[probp->arg2] ) == probp->arg3 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case PLAHSE :
	    if ( pl->house == probp->arg3 &&
		   (asp+probp->arg2)->house == probp->arg3 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       case PLASIGN :
	    if ( pl->sign == probp->arg3 &&
		   (asp+probp->arg2)->sign == probp->arg3 )
	       return( (int ) probp->result );
	    else
	       return( 0 );
	    break;
       default :
	    return( 0 );
	    break;
       }
 return( 0 );
}


int solve_problem( AS_INF *asp, ASTR_PROBLEM *probp, int item,
						  int *next, int *decp )
{
 int r = analyze_problem_item( asp, probp );

 if ( !r && ( probp->result == AND || probp->result == OR ) )
    r = -probp->result;
 else if ( !r ) {
    *decp = item;
    *next = item+1;
    return( 0 );
    }
#ifndef NDEBUG
 if (  r > N_RESULT )
    est[0]++;
#endif
 switch( r ) {
       case AND :
	    *decp = item;
	    ++probp;
	    *next = ++item;
	    return( solve_problem( asp, probp, item, next, decp ) );
	    break;
       case OR :
	    *decp = item;
	    do {
	       ++probp;
	       ++item;
	       } while ( probp->result == OR );
	    *next = item;
#ifndef NDEBUG
	    if ( probp->result < 0 || probp->result > N_RESULT )
	       est[1]++;
#endif
	    return( probp->result );
	    break;
       case -AND :
	    *decp = item;
	    do {
	       ++probp;
	       *next = ++item;
	       } while ( probp->result == AND );
#ifndef NDEBUG
	    if ( probp->result < 0 || probp->result > N_RESULT )
	       est[2]++;
#endif
	    return( 0 );
	    break;
       case -OR :
	    *decp = item;
	    ++probp;
	    *next = ++item;
#ifndef NDEBUG
	    if ( probp->result > N_RESULT )
	       est[3]++;
#endif
	    return( solve_problem( asp, probp, item, next, decp ) );
	    break;
       default :
	    *decp = item;
	    *next = ++item;
#ifndef NDEBUG
	    if ( r < 0 || r > N_RESULT )
	       est[4]++;
#endif
	    return( r );
	    break;
       }
 return( 1 );
}


