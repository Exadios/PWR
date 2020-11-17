/*
 * \section COPYRIGHT
 *
 * Passive Weather Radar (PWR).
 * Copyright (C) 2010-2020 Peter F Bradshaw
 * A detailed list of copyright holders can be found in the file "AUTHORS".
 *
 * \section LICENSE
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *
 *  Description:  Common parameters and lookup functions for PHY
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10
 ******************************************************************************/

#ifndef __LTE_PHY_COMMON__
#define __LTE_PHY_COMMON__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "config.hpp"

#define LTE_NSUBFRAMES_X_FRAME  10
#define LTE_NSLOTS_X_FRAME      (2 * LTE_NSUBFRAMES_X_FRAME)

#define LTE_NSOFT_BITS  250368 // Soft buffer size for Category 1 UE

#define LTE_PC_MAX 23         // Maximum TX power for Category 1 UE (in dBm)

#define LTE_MAX_PORTS     4
#define LTE_MAX_LAYERS    4
#define LTE_MAX_CODEWORDS 2
#define LTE_MAX_TB        LTE_MAX_CODEWORDS

#define LTE_MAX_CODEBLOCKS 32

#define LTE_MAX_CODEBOOKS 4

#define LTE_LTE_CRC24A  0x1864CFB
#define LTE_LTE_CRC24B  0X1800063
#define LTE_LTE_CRC16   0x11021
#define LTE_LTE_CRC8    0x19B

#define LTE_MAX_MBSFN_AREA_IDS 256
#define LTE_PMCH_RV            0

typedef enum {LTE_CP_NORM, LTE_CP_EXT} LteCp_t;
typedef enum {LTE_SF_NORM, LTE_SF_MBSFN} LteSf_t;


#define LTE_CRNTI_START  0x000B
#define LTE_CRNTI_END    0xFFF3
#define LTE_RARNTI_START 0x0001
#define LTE_RARNTI_END   0x000A
#define LTE_SIRNTI       0xFFFF
#define LTE_PRNTI        0xFFFE
#define LTE_MRNTI        0xFFFD

#define LTE_CELL_ID_UNKNOWN         1000

#define LTE_MAX_NSYMB     7

#define LTE_MAX_PRB   110
#define LTE_NRE       12

#define LTE_SYMBOL_SZ_MAX   2048

#define LTE_CP_NORM_NSYMB    7
#define LTE_CP_NORM_SF_NSYMB (2 * LTE_CP_NORM_NSYMB)
#define LTE_CP_NORM_0_LEN    160
#define LTE_CP_NORM_LEN      144

#define LTE_CP_EXT_NSYMB     6
#define LTE_CP_EXT_SF_NSYMB  (2 * LTE_CP_EXT_NSYMB)
#define LTE_CP_EXT_LEN       512
#define LTE_CP_EXT_7_5_LEN   1024

#define LTE_CP_ISNORM(cp) (cp == LTE_CP_NORM)
#define LTE_CP_ISEXT(cp) (cp == LTE_CP_EXT)
#define LTE_CP_NSYMB(cp) (LTE_CP_ISNORM(cp) ? LTE_CP_NORM_NSYMB : LTE_CP_EXT_NSYMB)

#define LTE_CP_LEN(symbol_sz, c)           ((int) ceilf((((float) (c) * (symbol_sz)) / 2048.0f)))
#define LTE_CP_LEN_NORM(symbol, symbol_sz) (((symbol) == 0) ? LTE_CP_LEN((symbol_sz),LTE_CP_NORM_0_LEN) : LTE_CP_LEN((symbol_sz), LTE_CP_NORM_LEN))
#define LTE_CP_LEN_EXT(symbol_sz)          (LTE_CP_LEN((symbol_sz), LTE_CP_EXT_LEN))

#define LTE_SLOT_LEN(symbol_sz)     (symbol_sz * 15 / 2)
#define LTE_SF_LEN(symbol_sz)       (symbol_sz * 15)
#define LTE_SF_LEN_MAX              (LTE_SF_LEN(LTE_SYMBOL_SZ_MAX))

#define LTE_SLOT_LEN_PRB(nof_prb)   (LTE_SLOT_LEN(LteSymbolSz(nof_prb)))
#define LTE_SF_LEN_PRB(nof_prb)     (LTE_SF_LEN(LteSymbolSz(nof_prb)))

#define LTE_SLOT_LEN_RE(nof_prb, cp)        (nof_prb * LTE_NRE * LTE_CP_NSYMB(cp))
#define LTE_SF_LEN_RE(nof_prb, cp)          (2 * LTE_SLOT_LEN_RE(nof_prb, cp))

#define LTE_TA_OFFSET      (10e-6)

#define LTE_LTE_TS         1.0 / (15000.0 * 2048)

#define LTE_SLOT_IDX_CPNORM(symbol_idx, symbol_sz) (symbol_idx==0 ? 0 : (symbol_sz + LTE_CP_LEN(symbol_sz, LTE_CP_NORM_0_LEN) + \
                                                (symbol_idx-1) * (symbol_sz + LTE_CP_LEN(symbol_sz, LTE_CP_NORM_LEN))))
#define LTE_SLOT_IDX_CPEXT(idx, symbol_sz) (idx * (symbol_sz + LTE_CP(symbol_sz, LTE_CP_EXT_LEN)))

#define LTE_RE_IDX(nof_prb, symbol_idx, sample_idx) ((symbol_idx) * (nof_prb) * (LTE_NRE) + sample_idx)

#define LTE_RS_VSHIFT(cell_id) (cell_id % 6)

#define LTE_GUARD_RE(nof_prb) ((LteSymbolSz(nof_prb) - nof_prb * LTE_NRE) / 2)

#define LTE_SYMBOL_HAS_REF(l, cp, nof_ports) ((l == 1 && nof_ports == 4) \
        || l == 0 \
        || l == LTE_CP_NSYMB(cp) - 3)



#define LTE_SYMBOL_HAS_REF_MBSFN(l, s) ((l == 2 && s == 0) || (l == 0 && s == 1) || (l == 4 && s == 1))

#define LTE_NON_MBSFN_REGION_GUARD_LENGTH(non_mbsfn_region,symbol_sz) ((non_mbsfn_region == 1) ? (LTE_CP_LEN_EXT(symbol_sz) - LTE_CP_LEN_NORM(0, symbol_sz)) : (2 * LTE_CP_LEN_EXT(symbol_sz) - LTE_CP_LEN_NORM(0, symbol_sz) - LTE_CP_LEN_NORM(1, symbol_sz)))



#define LTE_NOF_LTE_BANDS 38

#define LTE_DEFAULT_MAX_FRAMES_PBCH      500
#define LTE_DEFAULT_MAX_FRAMES_PSS       10
#define LTE_DEFAULT_NOF_VALID_PSS_FRAMES 10


typedef enum LTE_API
  {
  LTE_PHICH_NORM = 0,
  LTE_PHICH_EXT
  } LtePhichLength_t;

typedef enum LTE_API
  {
  LTE_PHICH_R_1_6 = 0,
  LTE_PHICH_R_1_2,
  LTE_PHICH_R_1,
  LTE_PHICH_R_2

  } LtePhichResources_t;

typedef enum
  {
  LTE_RNTI_USER = 0, /* Cell RNTI */
  LTE_RNTI_SI,       /* System Information RNTI */
  LTE_RNTI_RAR,      /* Random Access RNTI */
  LTE_RNTI_TEMP,     /* Temporary C-RNTI */
  LTE_RNTI_SPS,      /* Semi-Persistent Scheduling C-RNTI */
  LTE_RNTI_PCH,      /* Paging RNTI */
  LTE_RNTI_MBSFN,
  LTE_RNTI_NOF_TYPES
  } LteRntiType_t;

typedef struct LTE_API
  {
  uint32_t nof_prb;
  uint32_t nof_ports;
  uint32_t id;
  float peak;
  LteCp_t cp;
  LtePhichLength_t phich_length;
  LtePhichResources_t phich_resources;
  } LteCell_t;

typedef enum LTE_API
  {
  LTE_MIMO_TYPE_SINGLE_ANTENNA,
  LTE_MIMO_TYPE_TX_DIVERSITY,
  LTE_MIMO_TYPE_SPATIAL_MULTIPLEX,
  LTE_MIMO_TYPE_CDD
  } LteMimoType_t;

typedef enum LTE_API
  {
  LTE_MIMO_DECODER_ZF,
  LTE_MIMO_DECODER_MMSE
  } LteMimoDecoder_t;

typedef enum LTE_API
  {
  LTE_MOD_BPSK = 0,
  LTE_MOD_QPSK,
  LTE_MOD_16QAM,
  LTE_MOD_64QAM,
  LTE_MOD_LAST
  } LteMod_t;

typedef struct LTE_API
  {
  int id;
  float fd;
  } LteEarfcn_t;

enum band_geographical_area
  {
  LTE_BAND_GEO_AREA_ALL,
  LTE_BAND_GEO_AREA_NAR,
  LTE_BAND_GEO_AREA_APAC,
  LTE_BAND_GEO_AREA_EMEA,
  LTE_BAND_GEO_AREA_JAPAN,
  LTE_BAND_GEO_AREA_CALA,
  LTE_BAND_GEO_AREA_NA
  };

LTE_API bool LteCellIsvalid(LteCell_t *cell);

LTE_API void LteCellFprint(FILE *stream,
                           LteCell_t *cell,
                           uint32_t sfn);

/** Returns true if the structure pointed by cell has valid parameters */
LTE_API bool LteCellidIsvalid(uint32_t cell_id);

LTE_API bool LteNofprbIsvalid(uint32_t nof_prb);

LTE_API bool LteSfidxIsvalid(uint32_t sf_idx);

LTE_API bool LtePortidIsvalid(uint32_t port_id);

LTE_API bool LteNId2Isvalid(uint32_t N_id_2);

LTE_API bool LteNId1Isvalid(uint32_t N_id_1);

LTE_API bool LteSymbolSzIsvalid(uint32_t symbol_sz);

LTE_API int LteSymbolSz(uint32_t nof_prb);

LTE_API int LteSymbolSzPower2(uint32_t nof_prb);

LTE_API int LteNofPrb(uint32_t symbol_sz);

LTE_API int LteSamplingFreqHz(uint32_t nof_prb);

LTE_API void LteUseStandardSymbolSize(bool enabled);

LTE_API uint32_t LteReXPrb(uint32_t ns,
                           uint32_t symbol,
                           uint32_t nof_ports,
                           uint32_t nof_symbols);

LTE_API uint32_t LteVoffset(uint32_t symbol_id,
                            uint32_t cell_id,
                            uint32_t nof_ports);

/** Computes sequence-group pattern f_gh according to 5.5.1.3 of 36.211 */
LTE_API int LteGroupHoppingFGh(uint32_t f_gh[LTE_NSLOTS_X_FRAME],
                               uint32_t cell_id);

/** Returns the new time advance as indicated by the random access response
 *  as specified in Section 4.2.3 of 36.213 */
LTE_API uint32_t LteNTaNewRar(uint32_t ta);

/** Returns the new time advance N_ta_new as specified in Section 4.2.3 of
 *  36.213 */
LTE_API uint32_t LteNTaNew(uint32_t N_ta_old,
                           uint32_t ta);

LTE_API float LteCoderate(uint32_t tbs,
                          uint32_t nof_re);

LTE_API char *LteCpString(LteCp_t cp);

LTE_API LteMod_t LteStr2mod (char * mod_str);

LTE_API char *LteModString(LteMod_t mod);

LTE_API uint32_t LteModBitsXSymbol(LteMod_t mod);

LTE_API int LteBandGetBand(uint32_t dl_earfcn);

LTE_API float LteBandFd(uint32_t dl_earfcn);

LTE_API float LteBandFu(uint32_t ul_earfcn);

LTE_API uint32_t LteBandUlEarfcn(uint32_t dl_earfcn);

LTE_API int LteBandGetFdBand(uint32_t band,
                             LteEarfcn_t *earfcn,
                             int earfcn_start,
                             int earfcn_end,
                             uint32_t max_elems);

LTE_API int LteBandGetFdBand_all(uint32_t band,
                                     LteEarfcn_t *earfcn,
                                     uint32_t max_nelems);

LTE_API int LteBandGetFdRegion(enum band_geographical_area region,
                                   LteEarfcn_t *earfcn,
                                   uint32_t max_elems);

LTE_API int LteStr2mimotype(char *mimo_type_str,
                             LteMimoType_t *type);

LTE_API char *LteMimotype2str(LteMimoType_t mimo_type);

/** Returns the interval tti1 - tti2 mod 10240 */
LTE_API uint32_t LteTtiInterval(uint32_t tti1,
                                uint32_t tti2);

#endif // __LTE_PHY_COMMON__

