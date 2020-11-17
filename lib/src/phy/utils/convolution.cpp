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
#include <string.h>
#include "lte.hpp"

#include "phy/dft/dft.hpp"
#include "phy/utils/vector.hpp"
#include "phy/utils/convolution.hpp"

//------------------------------------------------------------------------------
int LteConvFftCcInit(LteConvFftCc_t *q, uint32_t input_len, uint32_t filter_len)
  {
  bzero(q, sizeof(LteConvFftCc_t));

  q->input_len = input_len;
  q->filter_len = filter_len;
  q->output_len = input_len + filter_len;
  q->max_filter_len = filter_len;
  q->max_input_len  = input_len;
  q->input_fft  = (Cf_t *)LteVecMalloc(sizeof(Cf_t) * q->output_len);
  q->filter_fft = (Cf_t *)LteVecMalloc(sizeof(Cf_t) * q->output_len);
  q->output_fft = (Cf_t *)LteVecMalloc(sizeof(Cf_t) * q->output_len);

  if (!q->input_fft || !q->filter_fft || !q->output_fft)
    {
    return LTE_ERROR;
    }
  if (LteDftPlan(&q->input_plan,
                 q->output_len,
                 LTE_DFT_FORWARD,
                 LTE_DFT_COMPLEX))
    {
    fprintf(stderr, "Error initiating input plan\n");
    return LTE_ERROR;
    }
  if (LteDftPlan(&q->filter_plan,
                 q->output_len,
                 LTE_DFT_FORWARD,
                 LTE_DFT_COMPLEX))
    {
    fprintf(stderr, "Error initiating filter plan\n");
    return LTE_ERROR;
    }
  if (LteDftPlan(&q->output_plan,
                 q->output_len,
                 LTE_DFT_BACKWARD,
                 LTE_DFT_COMPLEX))
    {
    fprintf(stderr, "Error initiating output plan\n");
    return LTE_ERROR;
    }
  LteDftPlanSetNorm(&q->input_plan, true);
  LteDftPlanSetNorm(&q->filter_plan, true);
  LteDftPlanSetNorm(&q->output_plan, false);

  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
int LteConvFftCcReplan(LteConvFftCc_t *q,
                       uint32_t input_len,
                       uint32_t filter_len)
  {
  if (input_len > q->max_input_len || filter_len > q->max_filter_len)
    {
    fprintf(stderr, "Error in conv_fft_cc_replan(): input_len and filter_len must be lower than initialized\n");
    return -1;
    }

  q->input_len = input_len;
  q->filter_len = filter_len;
  q->output_len = input_len + filter_len;

  if (!q->input_fft || !q->filter_fft || !q->output_fft)
    {
    return LTE_ERROR;
    }
  if (LteDftReplan(&q->input_plan, q->output_len))
    {
    fprintf(stderr, "Error initiating input plan\n");
    return LTE_ERROR;
    }
  if (LteDftReplan(&q->filter_plan, q->output_len))
    {
    fprintf(stderr, "Error initiating filter plan\n");
    return LTE_ERROR;
    }
  if (LteDftReplan(&q->output_plan, q->output_len))
    {
    fprintf(stderr, "Error initiating output plan\n");
    return LTE_ERROR;
    }
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
void LteConvFftCcFree(LteConvFftCc_t *q)
  {
  if (q->input_fft)
    {
    free(q->input_fft);
    }
  if (q->filter_fft)
    {
    free(q->filter_fft);
    }
  if (q->output_fft)
    {
    free(q->output_fft);
    }

  LteDftPlanFree(&q->input_plan);
  LteDftPlanFree(&q->filter_plan);
  LteDftPlanFree(&q->output_plan);

  bzero(q, sizeof(LteConvFftCc_t));

  }

//------------------------------------------------------------------------------
uint32_t LteConvFftCcRunOpt(LteConvFftCc_t *q, const Cf_t *input, const Cf_t *filter_freq, Cf_t *output)
  {
  LteDftRunC(&q->input_plan, input, q->input_fft);
  LteVecProdCcc(q->input_fft, filter_freq, q->output_fft, q->output_len);
  LteDftRunC(&q->output_plan, q->output_fft, output);

  return (q->output_len-1); // divide output length by dec factor

  }

//------------------------------------------------------------------------------
uint32_t LteConvFftCcRun(LteConvFftCc_t *q, const Cf_t *input, const Cf_t *filter, Cf_t *output)
  {

  LteDftRunC(&q->filter_plan, filter, q->filter_fft);

  return LteConvFftCcRunOpt(q, input, q->filter_fft, output);

  }

//------------------------------------------------------------------------------
uint32_t LteConvCc(const Cf_t *input, const Cf_t *filter, Cf_t *output, uint32_t input_len, uint32_t filter_len)
  {
  uint32_t i;
  uint32_t M = filter_len;
  uint32_t N = input_len;

  for (i = 0; i < M; i++)
    {
    output[i]=LteVecDotProdCcc(&input[i],&filter[i],i);
    }
  for (; i < M + N - 1; i++)
    {
    output[i] = LteVecDotProdCcc(&input[i - M], filter, M);
    }
  return M + N - 1;
  }

//------------------------------------------------------------------------------
/* Centered convolution. Returns the same number of input elements. Equivalenti
 * to conv(x, h, 'same') in matlab.
 * y(n) = sum_i x(n + i - M / 2) * h(i) for n = 1..N with N input samples and
 *                                      M filter len
 */
uint32_t LteConvSameCc(Cf_t *input, Cf_t *filter, Cf_t *output, uint32_t input_len, uint32_t filter_len)
  {
  uint32_t i;
  uint32_t M = filter_len;
  uint32_t N = input_len;

  for (i = 0; i < M / 2; i++)
    {
    output[i]=LteVecDotProdCcc(&input[i],&filter[M/2-i],M-M/2+i);
    }
  for (; i < N -M / 2; i++)
    {
    output[i] = LteVecDotProdCcc(&input[i - M / 2], filter, M);
    }
  for (; i < N; i++)
    {
    output[i] = LteVecDotProdCcc(&input[i - M / 2], filter, N - i + M / 2);
    }
  return N;
  }


#define conv_same_extrapolates_extremes

#ifdef conv_same_extrapolates_extremes
//------------------------------------------------------------------------------
uint32_t LteConvSameCf(Cf_t *input, float *filter, Cf_t *output,
                       uint32_t input_len, uint32_t filter_len)
  {
  uint32_t i;
  uint32_t M = filter_len;
  uint32_t N = input_len;
  Cf_t first[filter_len + filter_len / 2];
  Cf_t last[filter_len + filter_len / 2];

  for (i = 0; i < M + M / 2; i++)
    {
    if (i < M / 2)
      {
      first[i] = (2 + M / 2 - i) * input[1] - (1 + M / 2 - i) * input[0];
      }
    else
      {
      first[i] = input[i - M / 2];
      }
    }

  for (i = 0; i < M + M / 2; i++)
    {
    if (i >= M - 1)
      {
      last[i] = (2 + i - M / 2) * input[N - 1] - (1 + i - M / 2) * input[N - 2];
      }
    else
      {
      last[i] = input[N - M + i + 1];
      }
    }

  for (i = 0; i < M / 2; i++)
    {
    output[i]=LteVecDotProdCfc(&first[i], filter, M);
    }

  for (; i < N - M / 2; i++)
    {
    output[i] = LteVecDotProdCfc(&input[i - M / 2], filter, M);
    }
  int j = 0;
  for (; i < N; i++)
    {
    output[i] = LteVecDotProdCfc(&last[j++], filter, M);
    }
  return N;
  }

#else // conv_same_extrapolates_extremes

//------------------------------------------------------------------------------
uint32_t LteConvSameCf(Cf_t *input, float *filter, Cf_t *output,
                       uint32_t input_len, uint32_t filter_len)
  {
  uint32_t i;
  uint32_t M = filter_len;
  uint32_t N = input_len;

  for (i = 0; i < M / 2; i++)
    {
    output[i] = LteVecDotProdCfc(&input[i], &filter[M / 2 - i], M - M / 2 + i);
    }
  for (; i < N - M / 2; i++)
    {
    output[i] = LteVecDotProdCfc(&input[i - M / 2], filter, M);
    }
  for (; i < N; i++)
    {
    output[i] = LteVecDotProdCfc(&input[i - M / 2], filter, N - i + M / 2);
    }
  return N;
  }

#endif // conv_same_extrapolates_extremes
