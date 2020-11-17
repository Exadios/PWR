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

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <complex.h>

#include "phy/utils/cexptab.hpp"

//------------------------------------------------------------------------------
int LteCexptabInit(LteCexptab_t *h, uint32_t size)
  {
  uint32_t i;

  h->size = size;
  h->tab = (Cf_t *)malloc(sizeof(Cf_t) * (1 + size));
  if (h->tab)
    {
    for (i = 0; i < size; i++)
      {
      h->tab[i] = cexpf(_Complex_I * 2 * M_PI * (float) i / size);
      }
    return LTE_SUCCESS;
    }
  else
    {
    return LTE_ERROR;
    }
  }

//------------------------------------------------------------------------------
void LteCexptabFree(LteCexptab_t *h)
  {
  if (h->tab)
    {
    free(h->tab);
    }
  bzero(h, sizeof(LteCexptab_t));
  }

//------------------------------------------------------------------------------
void LteCexptabGen(LteCexptab_t *h, Cf_t *x, float freq, uint32_t len)
  {
  uint32_t i;
  uint32_t idx;
  float phase_inc = freq * h->size;
  float phase = 0;

  for (i = 0; i < len; i++)
    {
    while (phase >= (float) h->size)
      {
      phase -= (float) h->size;
      }
    while (phase < 0)
      {
      phase += (float) h->size;
      }
    idx = (uint32_t) phase;
    x[i] = h->tab[idx];
    phase += phase_inc;

    }
  }

//------------------------------------------------------------------------------
void LteCexptabGenDirect(Cf_t *x, float freq, uint32_t len)
  {
  uint32_t i;
  for (i = 0; i < len; i++)
    {
    x[i] = cexpf(_Complex_I * 2 * M_PI * freq * i);
    }
  }

