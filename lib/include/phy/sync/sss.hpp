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

#ifndef __SSS_H__
#define __SSS_H__

#include <stdint.h>
#include <stdbool.h>

#include "config.hpp"
#include "phy/common/phy_common.hpp"
#include "phy/dft/dft.hpp"


#define LTE_SSS_N      31
#define LTE_SSS_LEN    2 * LTE_SSS_N

typedef struct LTE_API
  {
  int z1[LTE_SSS_N][LTE_SSS_N];
  int c[2][LTE_SSS_N];
  int s[LTE_SSS_N][LTE_SSS_N];
  }
LteSssTables_t;

/* Allocate 32 complex to make it multiple of 32-byte AVX instructions alignment requirement.
 * Should use LteVecMalloc() to make it platform agnostic.
 */
typedef struct LTE_API
  {
  float z1[LTE_SSS_N][LTE_SSS_N];
  float c[2][LTE_SSS_N];
  float s[LTE_SSS_N][LTE_SSS_N];
  float sd[LTE_SSS_N][LTE_SSS_N-1];
  }
LteSssFcTables_t;


/* Low-level API */
typedef struct LTE_API
  {

  LteDftPlan_t dftp_input;
  
  uint32_t fft_size;
  uint32_t max_fft_size;

  float corr_peak_threshold;
  uint32_t symbol_sz;
  uint32_t subframe_sz;
  uint32_t n_id_2;
  
  uint32_t n_id_1_table[30][30];
  LteSssFcTables_t fc_tables[3]; // one for each n_id_2

  float corr_output_m0[LTE_SSS_N];
  float corr_output_m1[LTE_SSS_N];

  }
LteSss_t;


/* Basic functionality */
LTE_API int LteSssInit(LteSss_t *q, uint32_t fft_size);
LTE_API int LteSssResize(LteSss_t *q, uint32_t fft_size); 
LTE_API void LteSssFree(LteSss_t *q);
LTE_API void LteSssGenerate(float *signal0, 
                            float *signal5, 
                            uint32_t cell_id);
LTE_API void LteSssPutSlot(float *sss, 
                           Cf_t *symbol, 
                           uint32_t nof_prb, 
                           LteCp_t cp);
LTE_API int LteSssSetNId2(LteSss_t *q, uint32_t n_id_2);
LTE_API int LteSssM0m1Partial(LteSss_t *q,
                              const Cf_t *input,
                              uint32_t M, 
                              Cf_t ce[2*LTE_SSS_N],
                              uint32_t *m0, 
                              float *m0_value, 
                              uint32_t *m1, 
                              float *m1_value);
LTE_API int LteSssM0m1DiffCoh(LteSss_t *q,
                              const Cf_t *input,
                              Cf_t ce[2*LTE_SSS_N],
                              uint32_t *m0, 
                              float *m0_value, 
                              uint32_t *m1, 
                              float *m1_value);
LTE_API int LteSssM0m1Diff(LteSss_t *q,
                           const Cf_t *input,
                           uint32_t *m0, 
                           float *m0_value, 
                           uint32_t *m1, 
                           float *m1_value);
LTE_API uint32_t LteSssSubframe(uint32_t m0, uint32_t m1);
LTE_API int LteSssNId1(LteSss_t *q, uint32_t m0, uint32_t m1);
LTE_API int LteSssFrame(LteSss_t *q,
                        Cf_t *input, 
                        uint32_t *subframe_idx, 
                        uint32_t *n_id_1);
LTE_API void LteSssSetThreshold(LteSss_t *q, float threshold);
LTE_API void LteSssSetSymbol(LteSss_t *q, uint32_t symbol_sz);
LTE_API void LteSssSetSubframe(LteSss_t *q, uint32_t subframe_sz);

void generate_zsc_tilde(int *z_tilde, int *s_tilde, int *c_tilde);
void generate_m0m1(uint32_t N_id_1, uint32_t *m0, uint32_t *m1);
void generate_N_id_1_table(uint32_t table[30][30]);
void generate_s(int *s, int *s_tilde, uint32_t m0_m1);
void generate_s_all(int s[LTE_SSS_N][LTE_SSS_N], int *s_tilde);
void generate_c(int *c, int *c_tilde, uint32_t N_id_2, bool is_c0);
void generate_z(int *z, int *z_tilde, uint32_t m0_m1);
void generate_z_all(int z[LTE_SSS_N][LTE_SSS_N], int *z_tilde);
void generate_sss_all_tables(LteSssTables_t *tables, uint32_t N_id_2);
void LteSssGenerate(float *signal0, float *signal5, uint32_t cell_id);

#endif //__SSS_H__

