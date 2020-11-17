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

#include <string.h>
#include <complex.h>
#include <math.h>

#include "phy/vector/vector.hpp"
#include "phy/sync/sss.hpp"

#define MAX_M 3

//------------------------------------------------------------------------------
static void CorrAllZs(Cf_t z[LTE_SSS_N], float s[LTE_SSS_N][LTE_SSS_N-1], float output[LTE_SSS_N])
  {
  uint32_t m;
  Cf_t tmp[LTE_SSS_N]; 
  
  for (m = 0; m < LTE_SSS_N; m++)
    {
    tmp[m] = LteVecDotProdCfc(z, s[m], LTE_SSS_N - 1);    
    }
  LteVecAbsSquareCf(tmp, output, LTE_SSS_N);
  }

//------------------------------------------------------------------------------
static void CorrAllSzPartial(Cf_t z[LTE_SSS_N], float s[LTE_SSS_N][LTE_SSS_N], uint32_t M, float output[LTE_SSS_N])
  {
  uint32_t Nm = LTE_SSS_N/M; 
  Cf_t tmp[LTE_SSS_N];
  float tmp_abs[MAX_M - 1][LTE_SSS_N];
  int j, m; 
  float *ptr; 
  
  for (j = 0; j < M; j++)
    {
    for (m = 0; m < LTE_SSS_N; m++)
      {      
      tmp[m] = LteVecDotProdCfc(&z[j * Nm], &s[m][j * Nm], Nm);        
      }
    if (j == 0) 
      {
      ptr = output; 
      }
    else
      {
      ptr = tmp_abs[j - 1];
      }
    LteVecAbsSquareCf(tmp, ptr, LTE_SSS_N);   
    }
  for (j = 1; j < M; j++)
    {
    LteVecSumFff(tmp_abs[j - 1], output, output, LTE_SSS_N);
    }    
  }

//------------------------------------------------------------------------------
static void ExtractPairSss(LteSss_t *q, const Cf_t *input, Cf_t *ce, Cf_t y[2][LTE_SSS_N])
  {
  Cf_t input_fft[LTE_SYMBOL_SZ_MAX];

  LteDftRunC(&q->dftp_input, input, input_fft);
  
  if (ce)
    {
    LteVecDivCcc(&input_fft[q->fft_size / 2 - LTE_SSS_N],
                       ce,
                       &input_fft[q->fft_size / 2 - LTE_SSS_N],
                       2 * LTE_SSS_N);
    }
  
  for (int i = 0; i < LTE_SSS_N; i++)
    {
    y[0][i] = input_fft[q->fft_size / 2 - LTE_SSS_N + 2 * i];
    y[1][i] = input_fft[q->fft_size / 2-LTE_SSS_N + 2 * i + 1];
    }

  LteVecProdCfc(y[0], q->fc_tables[q->n_id_2].c[0], y[0], LTE_SSS_N);
  LteVecProdCfc(y[1], q->fc_tables[q->n_id_2].c[1], y[1], LTE_SSS_N);

  }    

//------------------------------------------------------------------------------
int LteSssM0m1Diff(LteSss_t *q, const Cf_t *input, uint32_t *m0, float *m0_value,
    uint32_t *m1, float *m1_value) 
  {
  return LteSssM0m1DiffCoh(q, input, NULL, m0, m0_value, m1, m1_value);
  }

//------------------------------------------------------------------------------
/* Differential SSS estimation. 
 * Returns m0 and m1 estimates 
 *
 * Source: "SSS Detection Method for Initial Cell Search in 3GPP LTE FDD/TDD Dual Mode Receiver"
 *       Jung-In Kim, Jung-Su Han, Hee-Jin Roh and Hyung-Jin Choi

 *
 */
int LteSssM0m1DiffCoh(LteSss_t *q, const Cf_t *input, Cf_t ce[2*LTE_SSS_N], uint32_t *m0, float *m0_value,
    uint32_t *m1, float *m1_value) 
  {

  int ret = LTE_ERROR_INVALID_INPUTS; 

  if (q                 != NULL   &&
      input             != NULL   &&
      m0                != NULL   && 
      m1                != NULL)
    {
    
    Cf_t yprod[LTE_SSS_N];
    Cf_t y[2][LTE_SSS_N];

    ExtractPairSss(q, input, ce, y);
    
    LteVecProdConjCcc(&y[0][1], y[0], yprod, LTE_SSS_N - 1);    
    CorrAllZs(yprod, q->fc_tables[q->n_id_2].sd, q->corr_output_m0);
    *m0 = LteVecMaxFi(q->corr_output_m0, LTE_SSS_N);
    if (m0_value)
      {
      *m0_value = q->corr_output_m0[*m0];
      }    
    
    LteVecProdCfc(y[1], q->fc_tables[q->n_id_2].z1[*m0], y[1], LTE_SSS_N);
    LteVecProdConjCcc(&y[1][1], y[1], yprod, LTE_SSS_N - 1);
    CorrAllZs(yprod, q->fc_tables[q->n_id_2].sd, q->corr_output_m1);
    *m1 = LteVecMaxFi(q->corr_output_m1, LTE_SSS_N);
    if (m1_value)
      {
      *m1_value = q->corr_output_m1[*m1];
      }    
    ret = LTE_SUCCESS;
    } 
  return ret; 
  }

//------------------------------------------------------------------------------
/* Partial correlation SSS estimation. 
 * Returns m0 and m1 estimates 
 *
 * Source: "SSS Detection Method for Initial Cell Search in 3GPP LTE FDD/TDD Dual Mode Receiver"
 *       Jung-In Kim, Jung-Su Han, Hee-Jin Roh and Hyung-Jin Choi

 */
int LteSssM0m1Partial(LteSss_t *q, const Cf_t *input, uint32_t M, Cf_t ce[2*LTE_SSS_N], uint32_t *m0, float *m0_value,
    uint32_t *m1, float *m1_value) 
  {

  int ret = LTE_ERROR_INVALID_INPUTS; 

  if (q                 != NULL   &&
      input             != NULL   &&
      m0                != NULL   && 
      m1                != NULL   && 
      M                 <= MAX_M)
    {
    Cf_t y[2][LTE_SSS_N];
    
    ExtractPairSss(q, input, ce, y);
    
    CorrAllSzPartial(y[0], q->fc_tables[q->n_id_2].s, M, q->corr_output_m0);    
    *m0 = LteVecMaxFi(q->corr_output_m0, LTE_SSS_N);
    if (m0_value)
      {
      *m0_value = q->corr_output_m0[*m0];
      }
    LteVecProdCfc(y[1], q->fc_tables[q->n_id_2].z1[*m0], y[1], LTE_SSS_N);
    CorrAllSzPartial(y[1], q->fc_tables[q->n_id_2].s, M, q->corr_output_m1);
    *m1 = LteVecMaxFi(q->corr_output_m1, LTE_SSS_N);
    if (m1_value) 
      {
      *m1_value = q->corr_output_m1[*m1];
      }    
    ret = LTE_SUCCESS;
    } 
  return ret; 
  }

//------------------------------------------------------------------------------
void ConvertTables(LteSssFcTables_t *fc_tables, LteSssTables_t *in)
  {
  uint32_t i, j;

  for (i = 0; i < LTE_SSS_N; i++)
    {
    for (j = 0; j < LTE_SSS_N; j++)
      {
      fc_tables->z1[i][j] = (float) in->z1[i][j];
      }
    }
  for (i = 0; i < LTE_SSS_N; i++)
    {
    for (j = 0; j < LTE_SSS_N; j++)
      {
      fc_tables->s[i][j] = (float) in->s[i][j];
      }
    }
  for (i = 0; i < LTE_SSS_N; i++)
    {
    for (j = 0; j < LTE_SSS_N - 1; j++)
      {
      fc_tables->sd[i][j] = (float) in->s[i][j + 1] * in->s[i][j];
      }
    }
  for (i = 0; i < 2; i++) 
    {
    for (j = 0; j < LTE_SSS_N; j++)
      {
      fc_tables->c[i][j] = (float) in->c[i][j];
      }
    }
  }
