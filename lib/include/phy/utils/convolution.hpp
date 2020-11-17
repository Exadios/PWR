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
 */

/******************************************************************************
 *
 *  Description:  Utility module for fast convolution using FFT.
 *
 *  Reference:
 *****************************************************************************/

#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

#include "config.hpp"
#include "phy/dft/dft.hpp"

typedef struct LTE_API
  {
  Cf_t *input_fft;
  Cf_t *filter_fft;
  Cf_t *output_fft;
  Cf_t *output_fft2;
  uint32_t input_len;
  uint32_t filter_len;
  uint32_t output_len;
  uint32_t max_input_len;
  uint32_t max_filter_len;
  LteDftPlan_t input_plan;
  LteDftPlan_t filter_plan;
  LteDftPlan_t output_plan;
  //Cf_t *pss_signal_time_fft[3]; // One sequence for each N_id_2
  //Cf_t *pss_signal_time[3];

  } LteConvFftCc_t;

LTE_API int lte_conv_fft_cc_init(LteConvFftCc_t *q,
                                 uint32_t input_len,
                                 uint32_t filter_len);

LTE_API int lte_conv_fft_cc_replan(LteConvFftCc_t *q,
                                   uint32_t input_len,
                                   uint32_t filter_len);

LTE_API void lte_conv_fft_cc_free(LteConvFftCc_t *q);

LTE_API uint32_t lte_conv_fft_cc_run(LteConvFftCc_t *q,
                                     const Cf_t *input,
                                     const Cf_t *filter,
                                     Cf_t *output);

LTE_API uint32_t lte_conv_fft_cc_run_opt(LteConvFftCc_t *q,
    const Cf_t *input,
    const Cf_t *filter_freq,
    Cf_t *output);

LTE_API uint32_t lte_conv_cc(const Cf_t *input,
                             const Cf_t *filter,
                             Cf_t *output,
                             uint32_t input_len,
                             uint32_t filter_len);

LTE_API uint32_t lte_conv_same_cf(Cf_t *input,
                                  float *filter,
                                  Cf_t *output,
                                  uint32_t input_len,
                                  uint32_t filter_len);

LTE_API uint32_t lte_conv_same_cc(Cf_t *input,
                                  Cf_t *filter,
                                  Cf_t *output,
                                  uint32_t input_len,
                                  uint32_t filter_len);

#endif // __CONVOLUTION_H__
