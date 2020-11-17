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

/******************************************************************************
 *
 *  Description:  Utility module for generation of complex exponential tables.
 *
 *  Reference:
 *****************************************************************************/

#ifndef __CEXPTAB_H__
#define __CEXPTAB_H__

#include <complex.h>
#include <stdint.h>
#include "config.hpp"

typedef struct LTE_API
  {
  uint32_t size;
  Cf_t *tab;
  } LteCexptab_t;

LTE_API int LteCexptabInit(LteCexptab_t *nco,
                           uint32_t size);

LTE_API void LteCexptabFree(LteCexptab_t *nco);

LTE_API void LteCexptabGen(LteCexptab_t *nco,
                           Cf_t *x,
                           float freq,
                           uint32_t len);

LTE_API void LteCexptabGenDirect(Cf_t *x,
                                 float freq,
                                 uint32_t len);

#endif // __CEXPTAB_H__

