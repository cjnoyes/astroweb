#include "astro.h"
#include "useri.h"
#include "dataext.h"
#include "utlduad.h"
#include "setup.hpp"
#include <memory.h>
#include "compile.hpp"
#include "setup.hpp"
#include "errors.fns"

int m_dial;
int m_harmonic;
PROGR_DATA progress_data;
TRANS_DATA transit_data;
RECT_DATA birth_rectd, comp_rectd, trans_rectd;
RETURN_DATA return_data;
BIRTH_DB birth_rec, comp_rec, trans_rec;
LOC_DB reloc_data;
DIREC_DATA solar_data;
short birth_ok, birth_rect_ok = 1, comp_ok, comp_rect_ok = 1,
    trans_ok, trans_data_ok, prog_ok;
AS_INF birth_inf[32], alt_inf[32];
AS_INF_EXT birth_ext[32], alt_ext[32];
short house_cusps[13], alt_cusps[13];
DATA_PACKET birth_dp = { NATAL_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
comp_dp = { COMPA_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, &comp_rectd, &m_harmonic, &m_dial },
trans_dp = { TRANS_CH, &birth_rectd, &transit_data, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
progr_dp = { PROGR_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
return_dp = { RETUR_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
reloc_dp = { RELOC_CH, &birth_rectd, NULL, &progress_data,  &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
compos_dp = { COMPO_CH|RELOC_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, &comp_rectd, &m_harmonic, &m_dial },
fixed_dp = { FIXED_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
arabp_dp = { ARABP_CH, &birth_rectd, NULL, &progress_data,  &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
comptr_dp = { COMPO_CH|TRANS_CH|RELOC_CH, &birth_rectd, &transit_data, &progress_data, &return_data, &solar_data, &reloc_data, &comp_rectd, &m_harmonic, &m_dial },
solar_dp = { NATAL_CH|SOLAR_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial },
harmonic_dp = { NATAL_CH|HARMON_CH, &birth_rectd, NULL, &progress_data, &return_data, &solar_data, &reloc_data, NULL, &m_harmonic, &m_dial };


