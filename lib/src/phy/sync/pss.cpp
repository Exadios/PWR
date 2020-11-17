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

#include "phy/sync/pss.hpp"

#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

//------------------------------------------------------------------------------
int LtePssInitNId2(Cf_t *pss_signal_freq,
                   Cf_t *pss_signal_time,
                   uint32_t N_id_2,
                   uint32_t fft_size,
                   int cfo_i)
  {
  LteDftPlan_t plan;
  Cf_t pss_signal_pad[2048];
  int ret = LTE_ERROR_INVALID_INPUTS;
  
  if (srslte_N_id_2_isvalid(N_id_2) && fft_size <= 2048) 
    {
    
    srslte_pss_generate(pss_signal_freq, N_id_2);

    bzero(pss_signal_pad, fft_size * sizeof(Cf_t));
    bzero(pss_signal_time, fft_size * sizeof(Cf_t));
    memcpy(&pss_signal_pad[(fft_size - LTE_PSS_LEN) / 2 + cfo_i],
           pss_signal_freq,
           LTE_PSS_LEN * sizeof(Cf_t));

    /* Convert signal into the time domain */    
    if (srslte_dft_plan(&plan,
                        fft_size,
                        LTE_DFT_BACKWARD,
                        LTE_DFT_COMPLEX))
      {
      return LTE_ERROR;
      }
    
    LteDftPlanSetMirror(&plan, true);
    LteDftPlanSetDc(&plan, true);
    LteDftPlanSetNorm(&plan, true);
    LteDftRunC(&plan, pss_signal_pad, pss_signal_time);

    LteVecConjCc(pss_signal_time, pss_signal_time, fft_size);
    LteVecScProdCfc(pss_signal_time,
                    1.0 / LTE_PSS_LEN,
                    pss_signal_time,
                    fft_size);

    LteDftPlanFree(&plan);

    ret = LTE_SUCCESS;
    }
  return ret;
  }

//------------------------------------------------------------------------------
int
LtePssInit(LtePss_t* q, unsigned int frame_size)
  {
  return LtePssInitFft(q, frame_size, 128);
  }

//------------------------------------------------------------------------------
int
LtePssInitFft(LtePss_t* q, unsigned int frame_size, unsigned int fft_size)
  {
  return LtePssInitFftOffset(q, frame_size, fft_size, 0);
  }

//------------------------------------------------------------------------------
int
LtePssInitFftOffset(LtePss_t* q,
                    unsigned int frame_size,
                    unsigned int fft_size,
                    int offset)
  {
  return LtePssInitFftOffsetDecim(q, frame_size, fft_size, offset, 1);
  }

//------------------------------------------------------------------------------
int LtePssInitFftOffsetDecim(LtePss_t* q,
                             unsigned int max_frame_size,
                             unsigned int max_fft_size,
                             int offset,
                             int decimate)
  {
  int ret = LTE_ERROR_INVALID_INPUTS;
  if (q != NULL)
    {
    ret = LTE_ERROR; 
    uint32_t n_id_2; 
    uint32_t buffer_size;
    bzero(q, sizeof(LtePss_t));
    
    q->N_id_2 = 10;  
    q->ema_alpha = 0.2;

    q->max_fft_size  = max_fft_size;
    q->max_frame_size = max_frame_size;

    q->decimate = decimate;
    uint32_t fft_size = max_fft_size/q->decimate;
    uint32_t frame_size = max_frame_size / q->decimate;
    
    q->fft_size = fft_size;
    q->frame_size = frame_size;

    buffer_size = fft_size + frame_size + 1;

    q->filter_pss_enable = false;
    q->chest_on_filter   = false;

    if(q->decimate > 1)
      {
      int filter_order = 3;
      LteFiltDecimCcInit(&q->filter,q->decimate,filter_order);
      q->filter.filter_output = LteVecMalloc((buffer_size) * sizeof(Cf_t));
      q->filter.downsampled_input = LteVecMalloc((buffer_size + filter_order) * sizeof(Cf_t));
      printf("decimation for the  PSS search is %d \n", q->decimate);
      }
      
    if (LteDftPlan(&q->dftp_input, fft_size, LTE_DFT_FORWARD, LTE_DFT_COMPLEX))
      {
      fprintf(stderr, "Error creating DFT plan \n");
      goto clean_and_exit;
      }
    LteDftPlan_set_mirror(&q->dftp_input, true);
    LteDftPlan_set_dc(&q->dftp_input, true);
    LteDftPlan_set_norm(&q->dftp_input, false);

    if (LteDftPlan(&q->idftp_input, fft_size, LTE_DFT_BACKWARD, LTE_DFT_COMPLEX))
      {
      fprintf(stderr, "Error creating DFT plan \n");
      goto clean_and_exit;
      }
    LteDftPlan_set_mirror(&q->idftp_input, true);
    LteDftPlan_set_dc(&q->idftp_input, true);
    LteDftPlan_set_norm(&q->idftp_input, false);

    bzero(q->tmp_fft2, sizeof(Cf_t)*LTE_SYMBOL_SZ_MAX);

    q->tmp_input = LteVecMalloc((buffer_size + frame_size * (q->decimate - 1)) * sizeof(Cf_t));
    if (!q->tmp_input)
      {
      fprintf(stderr, "Error allocating memory\n");
      goto clean_and_exit;
      }

    bzero(&q->tmp_input[q->frame_size], q->fft_size * sizeof(Cf_t));

    q->conv_output = LteVecMalloc(buffer_size * sizeof(Cf_t));
    if (!q->conv_output)
      {
      fprintf(stderr, "Error allocating memory\n");
      goto clean_and_exit;
      }
    bzero(q->conv_output, sizeof(Cf_t) * buffer_size);
    q->conv_output_avg = LteVecMalloc(buffer_size * sizeof(float));
    if (!q->conv_output_avg)
      {
      fprintf(stderr, "Error allocating memory\n");
      goto clean_and_exit;
      }
    bzero(q->conv_output_avg, sizeof(float) * buffer_size);
#ifdef LTE_PSS_ACCUMULATE_ABS
    q->conv_output_abs = LteVecMalloc(buffer_size * sizeof(float));
    if (!q->conv_output_abs)
      {
      fprintf(stderr, "Error allocating memory\n");
      goto clean_and_exit;
      }
    bzero(q->conv_output_abs, sizeof(float) * buffer_size);
#endif  // LTE_PSS_ACCUMULATE_ABS

    for (n_id_2 = 0; n_id_2 < 3; n_id_2++)
      {
      q->pss_signal_time[n_id_2] = LteVecMalloc(buffer_size * sizeof(Cf_t));
      if (!q->pss_signal_time[n_id_2]) 
        {
        fprintf(stderr, "Error allocating memory\n");
        goto clean_and_exit;
        }
      /* The PSS is translated into the time domain for each n_id_2  */
      if (LtePssInitNId2(q->pss_signal_freq[n_id_2], q->pss_signal_time[n_id_2], n_id_2, fft_size, offset))
        {
        fprintf(stderr, "Error initiating PSS detector for n_id_2=%d fft_size=%d\n", n_id_2, fft_size);
        goto clean_and_exit;
        }
      bzero(&q->pss_signal_time[n_id_2][q->fft_size], q->frame_size * sizeof(Cf_t));
      }

    #ifdef CONVOLUTION_FFT
    if (LteConvFftCcInit(&q->conv_fft, frame_size, fft_size))
      {
      fprintf(stderr, "Error initiating convolution FFT\n");
      goto clean_and_exit;
      }
    for(n_id_2 = 0; n_id_2 < 3; n_id_2++)
      {
      q->pss_signal_freq_full[n_id_2] = LteVecMalloc(buffer_size * sizeof(Cf_t));
      LteDftRunC(&q->conv_fft.filter_plan, q->pss_signal_time[n_id_2], q->pss_signal_freq_full[n_id_2]);
      }

    #endif  // CONVOLUTION_FFT

    LtePssReset(q);

    ret = LTE_SUCCESS;
    }
    
clean_and_exit:
  if (ret == LTE_ERROR)
    {
    LtePssFree(q);
    }
  return ret;
  }
 
//------------------------------------------------------------------------------
int LtePssResize(LtePss_t *q,
                 unsigned int frame_size,
                 unsigned int fft_size,
                 int offset)
  {
  int ret = LTE_ERROR_INVALID_INPUTS;
  if (q != NULL)
    {
    ret = LTE_ERROR;

    if (fft_size > q->max_fft_size || frame_size > q->max_frame_size)
      {
      fprintf(stderr, "Error in pss_config(): fft_size and frame_size must be lower than initialized\n");
      return LTE_ERROR;
      }

    unsigned int n_id_2;
    unsigned int buffer_size;

    q->n_id_2 = 10;
    q->ema_alpha = 0.2;

    fft_size = fft_size / q->decimate;
    frame_size = frame_size / q->decimate;

    q->fft_size = fft_size;
    q->frame_size = frame_size;

    buffer_size = fft_size + frame_size + 1;

    if (LteDftReplan(&q->dftp_input, fft_size))
      {
      fprintf(stderr, "Error creating DFT plan \n");
      return LTE_ERROR;
      }

    if (LteDftReplan(&q->idftp_input, fft_size))
      {
      fprintf(stderr, "Error creating DFT plan \n");
      return SRSLTE_ERROR;
      }

    bzero(q->tmp_fft2, sizeof(Cf_t)*LTE_SYMBOL_SZ_MAX);

    bzero(&q->tmp_input[q->frame_size], q->fft_size * sizeof(cf_t));
    bzero(q->conv_output, sizeof(cf_t) * buffer_size);
    bzero(q->conv_output_avg, sizeof(float) * buffer_size);

#ifdef LTE_PSS_ACCUMULATE_ABS
    bzero(q->conv_output_abs, sizeof(float) * buffer_size);
#endif  // LTE_PSS_ACCUMULATE_ABS

    // Generate PSS sequences for this FFT size
    for (n_id_2 = 0; n_id_2 < 3; n_id_2++)
      {
      if (LtePssInitNId2(q->pss_signal_freq[n_id_2], q->pss_signal_time[n_id_2], n_id_2, fft_size, offset))
        {
        fprintf(stderr, "Error initiating PSS detector for n_id_2=%d fft_size=%d\n", n_id_2, fft_size);
        return LTE_ERROR;
        }
      bzero(&q->pss_signal_time[n_id_2][q->fft_size], q->frame_size * sizeof(cf_t));
    }
#ifdef CONVOLUTION_FFT

    if (LteConvFftCcReplan(&q->conv_fft, frame_size, fft_size))
      {
      fprintf(stderr, "Error initiating convolution FFT\n");
      return LTE_ERROR;
      }
    for(int i = 0; i < 3; i++)
      {
      LteDftRunC(&q->conv_fft.filter_plan, q->pss_signal_time[i], q->pss_signal_freq_full[i]);
      }
#endif  // CONVOLUTION_FFT

    LtePssReset(q);

    ret = LTE_SUCCESS;
    }
  return ret;
  }

//------------------------------------------------------------------------------
void LtePssFree(LtePss_t *q)
  {
  unsigned int i;

  if (q)
    {
    for (i = 0; i < 3; i++)
      {
      if (q->pss_signal_time[i])
        {
        free(q->pss_signal_time[i]);
        }
      if(q->pss_signal_freq_full[i])
        {
        free(q->pss_signal_freq_full[i]);
        }
      }
  #ifdef CONVOLUTION_FFT
    LteConvFftCcFree(&q->conv_fft);
  #endif  // CONVOLUTION_FFT
    if (q->tmp_input)
      {
      free(q->tmp_input);
      }
    if (q->conv_output)
      {
      free(q->conv_output);
      }
    if (q->conv_output_abs)
      {
      free(q->conv_output_abs);
      }
    if (q->conv_output_avg)
      {
      free(q->conv_output_avg);
      }

    LteDftPlanFree(&q->dftp_input);
    LteDftPlanFree(&q->idftp_input);

    if (q->decimate > 1)
      {
      LteFiltDecimCcFree(&q->filter);
      free(q->filter.filter_output);
      free(q->filter.downsampled_input);
      }

    bzero(q, sizeof(LtePss_t));
    }
  }

//------------------------------------------------------------------------------
void LtePssReset(LtePss_t *q)
  {
  unsigned int buffer_size = q->fft_size + q->frame_size + 1;
  bzero(q->conv_output_avg, sizeof(float) * buffer_size);
  }

/**
 * This function calculates the Zadoff-Chu sequence.
 * @param signal Output array.
 */
//------------------------------------------------------------------------------
int LtePssGenerate(Cf_t *signal, unsigned int n_id_2)
  {
  int i;
  float arg;
  const float root_value[] = { 25.0, 29.0, 34.0 };
  int root_idx;

  int sign = -1;

  if (n_id_2 > 2)
    {
    fprintf(stderr, "Invalid n_id_2 %d\n", n_id_2);
    return -1;
    }

  root_idx = n_id_2;

  for (i = 0; i < LTE_PSS_LEN / 2; i++) 
    {
    arg = (float) sign * M_PI * root_value[root_idx]
        * ((float) i * ((float) i + 1.0)) / 63.0;
    __real__ signal[i] = cosf(arg);
    __imag__ signal[i] = sinf(arg);
  }
  for (i = LTE_PSS_LEN / 2; i < LTE_PSS_LEN; i++)
    {
    arg = (float) sign * M_PI * root_value[root_idx]
        * (((float) i + 2.0) * ((float) i + 1.0)) / 63.0;
    __real__ signal[i] = cosf(arg);
    __imag__ signal[i] = sinf(arg);
    }
  return 0;
  }

