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
 *
*/

#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "phy/sync/sss.hpp"
#include "phy/dft/dft.hpp"
#include "phy/utils/convolution.hpp"
#include "phy/utils/vector.hpp"

void generate_sss_all_tables(LteSssTables_t *tables, uint32_t N_id_2);
void convert_tables(LteSssFcTables_t *fc_tables, LteSssTables_t *in);
void generate_N_id_1_table(uint32_t table[30][30]);

//------------------------------------------------------------------------------
int LteSssInit(LteSss_t *q, uint32_t fft_size)
  {
  
  if (q != NULL  && fft_size <= 2048)
    {
    uint32_t n_id_2;
    LteSssTables_t sss_tables;

    bzero(q, sizeof(LteSss_t));
    
    if (LteDftPlan(&q->dftp_input, fft_size, LTE_DFT_FORWARD, LTE_DFT_COMPLEX))
      {
      LteSssFree(q);
      return LTE_ERROR;
      }
    LteDftPlan_set_mirror(&q->dftp_input, true);
    LteDftPlan_set_dc(&q->dftp_input, true);

    q->fft_size = fft_size; 
    q->max_fft_size = fft_size;

    generate_N_id_1_table(q->N_id_1_table);
    
    for (n_id_2 = 0;n_id_2 < 3;n_id_2++)
      {
      generate_sss_all_tables(&sss_tables, n_id_2);
      convert_tables(&q->fc_tables[n_id_2], &sss_tables);
      }
    q->n_id_2 = 0;
    return LTE_SUCCESS;
    } 
  return LTE_ERROR_INVALID_INPUTS;
  }

//------------------------------------------------------------------------------
int LteSssResize(LteSss_t *q, uint32_t fft_size)
  {
  if (q != NULL  && fft_size <= 2048)
    {
    if (fft_size > q->max_fft_size)
      {
      fprintf(stderr, "Error in sss_synch_resize(): fft_size must be lower than initialized\n");
      return LTE_ERROR;
      }
    if (LteDftReplan(&q->dftp_input, fft_size))
      {
      LteSssFree(q);
      return LTE_ERROR;
      }
    q->fft_size = fft_size;
    return LTE_SUCCESS;
    }
  return LTE_ERROR_INVALID_INPUTS;
  }

//------------------------------------------------------------------------------
void LteSssFree(LteSss_t *q)
  {
  LteDftPlan_free(&q->dftp_input);
  bzero(q, sizeof(LteSss_t));
  }

//------------------------------------------------------------------------------
/** Sets the n_id_2 to search for */
int LteSssSetNId2(LteSss_t *q, uint32_t n_id_2)
  {
  if (!LteNId2Isvalid(n_id_2))
    {
    fprintf(stderr, "Invalid n_id_2 %d\n", n_id_2);
    return LTE_ERROR;
    }
  else
    {
    q->n_id_2 = n_id_2;
    return LTE_SUCCESS;
    }
  }

//------------------------------------------------------------------------------
/** 36.211 10.3 section 6.11.2.2
 */
void LteSssPutSlot(float *sss, cf_t *slot, uint32_t nof_prb, LteCp_t cp)
  {
  uint32_t i, k;

  k = (LTE_CP_NSYMB(cp) - 2) * nof_prb * LTE_NRE + nof_prb * LTE_NRE / 2 - 31;
  
  if (k > 5)
    {
    memset(&slot[k - 5], 0, 5 * sizeof(cf_t));
    for (i = 0; i < LTE_SSS_LEN; i++)
      {
      __real__ slot[k + i] = sss[i];
      __imag__ slot[k + i] = 0;
      }
    memset(&slot[k + LTE_SSS_LEN], 0, 5 * sizeof(cf_t));
    }
  }

//------------------------------------------------------------------------------
/** Sets the SSS correlation peak detection threshold */
void LteSssSetThreshold(LteSss_t *q, float threshold)
  {
  q->corr_peak_threshold = threshold;
  }

//------------------------------------------------------------------------------
/** Returns the subframe index based on the m0 and m1 values */
uint32_t LteSssSubframe(uint32_t m0, uint32_t m1)
  {
  if (m1 > m0)
    {
    return 0;
    }
  else
    {
    return 5;
    }
  }

//------------------------------------------------------------------------------
/** Returns the N_id_1 value based on the m0 and m1 values */
int LteSssNId1(LteSss_t *q, uint32_t m0, uint32_t m1)
  {
  int N_id_1 = -1; 
  if (m1 > m0)
    {
    if (m0 < 30 && m1 - 1 < 30)
      {
      N_id_1 = q->N_id_1_table[m0][m1 - 1];
      }
    }
  else
    {
    if (m1 < 30 && m0 - 1 < 30)
      {
      N_id_1 = q->N_id_1_table[m1][m0 - 1];
      }
    } 
  return N_id_1;
  }
