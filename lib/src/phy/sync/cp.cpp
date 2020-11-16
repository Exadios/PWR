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
#include "lte.hpp"

#include "phy/sync/cp.hpp"
#include "phy/utils/vector.hpp"
#include "phy/utils/debug.hpp"

//------------------------------------------------------------------------------
int LteCpSynchInit(LteCpSynch_t *q, uint32_t symbol_sz) 
  {
  q->symbol_sz = symbol_sz;
  q->max_symbol_sz = symbol_sz;

  q->corr = LteVecMalloc(sizeof(Cf_t) * q->symbol_sz);
  if (!q->corr)
    {
    perror("malloc");
    return LTE_ERROR;
    }
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
void LteCpSynchFree(LteCpSynch_t *q)
  {
  if (q->corr)
    {
    free(q->corr);
    }
  }

//------------------------------------------------------------------------------
int LteCpSynchResize(LteCpSynch_t *q, uint32_t symbol_sz)
{
  if (symbol_sz > q->max_symbol_sz)
    {
    fprintf(stderr, "Error in cp_synch_resize(): symbol_sz must be lower than initialized\n");
    return LTE_ERROR;
    }
  q->symbol_sz = symbol_sz;

  return LTE_SUCCESS;
  }


//------------------------------------------------------------------------------
uint32_t LteCpSynch(LteCpSynch_t *q, const Cf_t *input, uint32_t max_offset, uint32_t nof_symbols, uint32_t cp_len)
  {  
  if (max_offset > q->symbol_sz)
    {    
    max_offset = q->symbol_sz; 
    }
  for (int i = 0; i < max_offset; i++)
    {
    q->corr[i] = 0;
    const Cf_t *inputPtr = input;
    for (int n = 0; n < nof_symbols; n++)
      {
      uint32_t cplen = (n % 7) ? cp_len : cp_len + 1;
      q->corr[i] += LteVecDotProdConjCcc(&inputPtr[i], &inputPtr[i+q->symbol_sz], cplen)/nof_symbols;
      inputPtr += q->symbol_sz + cplen;        
      }    
    }
  uint32_t max_idx = LteVecMaxAbsCi(q->corr, max_offset);
  return max_idx; 
  }

//------------------------------------------------------------------------------
Cf_t LteCpSynchCorrOutput(LteCpSynch_t *q, uint32_t offset) 
  {
  if (offset < q->symbol_sz)
    {
    return q->corr[offset];
    }
  else
    {
    return 0;
    }
  }

