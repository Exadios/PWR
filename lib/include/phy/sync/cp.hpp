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

#ifndef __CP_H__
#define __CP_H__

#include <stdint.h>
#include <complex.h>

#include "config.hpp"

typedef struct
  {
  Cf_t *corr;
  uint32_t symbol_sz;
  uint32_t max_symbol_sz;
  }
LteCpSynch_t;

LTE_API int LteCpSynchInit(LteCpSynch_t *q, uint32_t symbol_sz);

LTE_API void LteCpSynchFree(LteCpSynch_t *q);

LTE_API int LteCpSynchResize(LteCpSynch_t *q, uint32_t symbol_sz);

LTE_API uint32_t LteCpSynch(LteCpSynch_t *q, 
                            const Cf_t *input,
                            uint32_t max_offset, 
                            uint32_t nof_symbols, 
                            uint32_t cp_len);

LTE_API Cf_t LteCpSynchCorrOutput(LteCpSynch_t *q, 
                                            uint32_t offset);

#endif // __CP_H__

