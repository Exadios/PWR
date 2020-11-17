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

#ifndef __CFO_H__
#define __CFO_H__

#include <complex.h>

#include "config.hpp"
#include "phy/utils/cexptab.hpp"

#define SRSLTE_CFO_CEXPTAB_SIZE 4096

typedef struct LTE_API
  {
  float last_freq;
  float tol;
  int nsamples;
  int max_samples;
  LteCexptab_t tab;
  Cf_t *cur_cexp;
  }
LteCfo_t;

LTE_API int LteCfoInit(LteCfo_t *h, uint32_t nsamples);

LTE_API void LteCfoFree(LteCfo_t *h);

LTE_API int LteCfoResize(LteCfo_t *h, uint32_t samples);

LTE_API void LteCfoSetTol(LteCfo_t *h, float tol);

LTE_API void LteCfoCorrect(LteCfo_t *h, 
                           const Cf_t *input,
                           Cf_t *output,
                           float freq);

#endif // __CFO_H__
