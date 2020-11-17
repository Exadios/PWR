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
#include <stdlib.h>
#include <math.h>
#include "lte.hpp"

#include "phy/utils/cexptab.hpp"
#include "phy/sync/cfo.hpp"
#include "phy/vector/vector.hpp"
//#include "phy/utils/debug.hpp"

/* Set next macro to 1 for using table generated CFO compensation */
#define LTE_CFO_USE_EXP_TABLE 0

//------------------------------------------------------------------------------
int LteCfoInit(LteCfo_t *h, uint32_t nsamples)
  {
#if LTE_CFO_USE_EXP_TABLE
  int ret = LTE_ERROR;
  bzero(h, sizeof(LteCfo_t));

  if (LteCexptabInit(&h->tab, SRSLTE_CFO_CEXPTAB_SIZE))
    {
    goto clean;
    }
  h->cur_cexp = LteVecMalloc(sizeof(Cf_t) * nsamples);
  if (!h->cur_cexp)
    {
    goto clean;
    }
  h->tol = 0;
  h->last_freq = 0;
  h->nsamples = nsamples;
  h->max_samples = nsamples;
  LteCexptabGen(&h->tab, h->cur_cexp, h->last_freq, h->nsamples);

  ret = LTE_SUCCESS;
clean:
  if (ret == LTE_ERROR)
    {
    LteCfoFree(h);
    }
  return ret;
#else /* LTE_CFO_USE_EXP_TABLE */
  h->nsamples = nsamples;
  return LTE_SUCCESS;
#endif /* LTE_CFO_USE_EXP_TABLE */
  }

//------------------------------------------------------------------------------
void LteCfoFree(LteCfo_t *h)
  {
#if LTE_CFO_USE_EXP_TABLE
  LteCexptabFree(&h->tab);
  if (h->cur_cexp) 
    {
    free(h->cur_cexp);
    }
#endif /* LTE_CFO_USE_EXP_TABLE */
  bzero(h, sizeof(LteCfo_t));
  }

//------------------------------------------------------------------------------
void LteCfoSetTol(LteCfo_t *h, float tol)
  {
  h->tol = tol;
  }

//------------------------------------------------------------------------------
int LteCfoResize(LteCfo_t *h, uint32_t samples)
  {
#if LTE_CFO_USE_EXP_TABLE
  if (samples <= h->max_samples)
    {
    LteCexptabGen(&h->tab, h->cur_cexp, h->last_freq, samples);
    h->nsamples = samples;
    } 
  else 
    {
    fprintf(stderr, "Error in cfo_resize(): nof_samples must be lower than initialized\n");
    return LTE_ERROR;
    }
#endif /* LTE_CFO_USE_EXP_TABLE */
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
void LteCfoCorrect(LteCfo_t *h, const Cf_t *input, Cf_t *output, float freq)
  {
#if LTE_CFO_USE_EXP_TABLE
  if (fabs(h->last_freq - freq) > h->tol)
    {
    h->last_freq = freq;
    LteCexptabGen(&h->tab, h->cur_cexp, h->last_freq, h->nsamples);
    DEBUG("CFO generating new table for frequency %.4fe-6\n", freq*1e6);
    }
  LteVecProdCcc(h->cur_cexp, input, output, h->nsamples);
#else /* LTE_CFO_USE_EXP_TABLE */
  LteVecApplyCfo(input, freq, output, h->nsamples);
#endif /* LTE_CFO_USE_EXP_TABLE */
  }
