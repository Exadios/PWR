/*
 * \section COPYRIGHT
 *
 * Passive Weather Radar (PWR).
 * Copyright (C) 2010-2021 Peter F Bradshaw
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
#include <stdio.h>
#include <strings.h>
#include <pthread.h>

#include "phy/common/sequence.hpp"
#include "phy/utils/vector.hpp"
#include "phy/utils/bit.hpp"

#define Nc 1600

#define MAX_SEQ_LEN  (128 * 1024)

#define static_memory

/*
 * Pseudo Random Sequence generation.
 * It follows the 3GPP Release 8 (LTE) 36.211
 * Section 7.2
 */
#ifdef static_memory
static uint8_t x1[Nc + MAX_SEQ_LEN + 31];
static uint8_t x2[Nc + MAX_SEQ_LEN + 31];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//------------------------------------------------------------------------------
int LteSequenceSetLTEPr(LteSequence_t *q, uint32_t len, uint32_t seed)
  {
  int n;

  if (len > q->max_len)
    {
    fprintf(stderr, "Error generating pseudo-random sequence: len %d exceeds maximum len %d\n",
            len, MAX_SEQ_LEN);
    return -1;
    }

  if (len > q->max_len)
    {
    fprintf(stderr, "Error generating pseudo-random sequence: len %d is greater than allocated len %d\n",
            len, q->max_len);
    return -1;
    }
  pthread_mutex_lock(&mutex);

  for (n = 0; n < 31; n++)
    {
    x2[n] = (seed >> n) & 0x1;
    }
  x1[0] = 1;

  for (n = 0; n < Nc + len; n++)
    {
    x1[n + 31] = (x1[n + 3] + x1[n]) & 0x1;
    x2[n + 31] = (x2[n + 3] + x2[n + 2] + x2[n + 1] + x2[n]) & 0x1;
    }

  for (n = 0; n < len; n++)
    {
    q->c[n] = (x1[n + Nc] + x2[n + Nc]) & 0x1;
    }
  pthread_mutex_unlock(&mutex);

  return 0;
  }

#else // static_memory
//------------------------------------------------------------------------------
int LteSequenceSetLTEPr(LteSequence_t *q, uint32_t len, uint32_t seed)
  {
  int n;
  uint32_t *x1, *x2;

  if (len > q->max_len)
    {
    fprintf(stderr, "Error generating pseudo-random sequence: len %d is greater than allocated len %d\n",
            len, q->max_len);
    return -1;
    }

  x1 = calloc(Nc + len + 31, sizeof(uint32_t));
  if (!x1)
    {
    perror("calloc");
    return -1;
    }
  x2 = calloc(Nc + len + 31, sizeof(uint32_t));
  if (!x2)
    {
    free(x1);
    perror("calloc");
    return -1;
    }

  for (n = 0; n < 31; n++)
    {
    x2[n] = (seed >> n) & 0x1;
    }
  x1[0] = 1;

  for (n = 0; n < Nc + len; n++)
    {
    x1[n + 31] = (x1[n + 3] + x1[n]) & 0x1;
    x2[n + 31] = (x2[n + 3] + x2[n + 2] + +x2[n + 1] + x2[n]) & 0x1;
    }

  for (n = 0; n < len; n++)
    {
    q->c[n] = (x1[n + Nc] + x2[n + Nc]) & 0x1;
    }

  free(x1);
  free(x2);

  return 0;
  }

#endif  // static_memory

//------------------------------------------------------------------------------
int LteSequenceLTEPr(LteSequence_t *q, uint32_t len, uint32_t seed)
  {
  if (LteSequenceInit(q, len))
    {
    return LTE_ERROR;
    }
  q->cur_len = len;
  LteSequenceSetLTEPr(q, len, seed);
  lte_bit_pack_vector(q->c, q->c_bytes, len);
  for (int i = 0; i < len; i++)
    {
    q->c_float[i] = (1 - 2 * q->c[i]);
    q->c_short[i] = (int16_t) q->c_float[i];
    q->c_char[i]  = (int8_t) q->c_float[i];;
    }
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
int LteSequenceInit(LteSequence_t *q, uint32_t len)
  {
  if (q->c && len > q->max_len)
    {
    LteSequenceFree(q);
    }
  if (!q->c)
    {
    q->c = lte_vec_malloc(len * sizeof(uint8_t));
    if (!q->c)
      {
      return LTE_ERROR;
      }
    q->c_bytes = lte_vec_malloc(len * sizeof(uint8_t) / 8 + 8);
    if (!q->c_bytes)
      {
      return LTE_ERROR;
      }
    q->c_float = lte_vec_malloc(len * sizeof(float));
    if (!q->c_float)
      {
      return LTE_ERROR;
      }
    q->c_short = lte_vec_malloc(len * sizeof(short));
    if (!q->c_short)
      {
      return LTE_ERROR;
      }
    q->c_char = lte_vec_malloc(len * sizeof(int8_t));
    if (!q->c_char)
      {
      return LTE_ERROR;
      }
    q->max_len = len;
    }
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
void LteSequenceFree(LteSequence_t *q)
  {
  if (q->c)
    {
    free(q->c);
    }
  if (q->c_bytes)
    {
    free(q->c_bytes);
    }
  if (q->c_float)
    {
    free(q->c_float);
    }
  if (q->c_short)
    {
    free(q->c_short);
    }
  if (q->c_char)
    {
    free(q->c_char);
    }
  bzero(q, sizeof(LteSequence_t));
  }

