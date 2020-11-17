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

#ifndef __PSS_HPP__
#define __PSS_HPP__

/******************************************************************************
 *  File:         pss.hpp
 *
 *  Description:  Primary synchronization signal (PSS) generation and detection.
 *
 *                The LtePss_t object provides functions for fast computation
 *                of the cross correlation between the PSS and received signal
 *                and CFO estimation. Also, the function LtePssTperiodic()
 *                is designed to be called periodically every subframe, taking
 *                care of the correct data alignment with respect to the PSS
 *                sequence.
 *
 *                The object is designed to work with signals sampled at
 *                1.92 MHz centered at the carrier frequency. Thus,
 *                downsampling is required if the signal is sampled at higher
 *                frequencies.
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10 Sec. 6.11.1
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#define LTE_PSS_LEN 62
#define LTE_PSS_RE  (6 * 12)

#include "config.hpp"
#include "phy/common/phy_common.hpp"
#include "phy/utils/convolution.hpp"
#include "phy/utils/filter.hpp"

#define CONVOLUTION_FFT

/* PSS processing options */

#define LTE_PSS_ACCUMULATE_ABS   // If enabled, accumulates the correlation absolute value on consecutive calls to LtePssFindPss

#define LTE_PSS_RETURN_PSR  // If enabled returns peak to side-lobe ratio, otherwise returns absolute peak value


/* Low-level API */
typedef struct LTE_API 
  {

#ifdef CONVOLUTION_FFT
  LteConvFftCc_t conv_fft;
  LteFiltCc_t filter;

#endif
  int decimate;

  uint32_t max_frame_size;
  uint32_t max_fft_size;

  uint32_t frame_size;
  uint32_t N_id_2;
  uint32_t fft_size;
  Cf_t *pss_signal_freq_full[3];

  Cf_t *pss_signal_time[3];
  Cf_t *pss_signal_time_scale[3];

  Cf_t pss_signal_freq[3][LTE_PSS_LEN]; // One sequence for each N_id_2
  Cf_t *tmp_input;
  Cf_t *conv_output;
  float *conv_output_abs;
  float ema_alpha;
  float *conv_output_avg;
  float peak_value;

  bool filter_pss_enable;
  LteDftPlan_t dftp_input;
  LteDftPlan_t idftp_input;
  Cf_t tmp_fft[LTE_SYMBOL_SZ_MAX];
  Cf_t tmp_fft2[LTE_SYMBOL_SZ_MAX];

  Cf_t tmp_ce[LTE_PSS_LEN];

  bool chest_on_filter;

  } LtePss_t;

typedef enum { PSS_TX, PSS_RX } PssDirection_t;

int LtePssInitFft(LtePss_t* q, unsigned int frame_size, unsigned int fft_size);
int LtePssInitFftOffset(LtePss_t* q,
                        unsigned int frame_size,
                        unsigned int fft_size,
                        int cfo_i);
int LtePssInitFftOffsetDecim(LtePss_t* q,
                             unsigned int frame_size,
                             unsigned int fft_size,
                             int cfo_i,
                             int decimate);
/**
 * Initializes the PSS synchronization object.
 *
 * It correlates a signal of frame_size samples with the PSS sequence in the
 * frequency domain. The PSS sequence is transformed using fft_size samples.
 *
 * @param q This object.
 * @param frame_size The FFT frame size.
 * @param fft_size The FFT size.
 * @param offset Offset into the input sequence.
 * @return Either LTE_SUCCESS, LTE_ERROR or LTE_ERROR_INVALID_INPUTS.
 */
int LtePssResize(LtePss_t* q,
                 unsigned int frame_size,
                 unsigned int fft_size,
                 int offset);

/** Initializes the PSS synchronization object with fft_size=128.
 * @param q This object.
 * @param frame_size FFT frame size.
 * @return LTE_SUCCESS if successful, LTE_ERROR otherwise. 
 */
int LtePssInit(LtePss_t* q, unsigned int frame_size);
void LtePssFree(LtePss_t* q);
void LtePssReset(LtePss_t* q);
void LtePssFilterEnable(LtePss_t* q, bool enable);
void LtePssSic(LtePss_t* q, Cf_t* input);
void LtePssFilter(LtePss_t* q, const Cf_t* input, Cf_t* output);
void LtePssGenerate(LtePss_t* q, unsigned int n_id_2);
void LtePssGetSlot(Cf_t* slot,
                   Cf_t* pss_signal,
                   unsigned int n_of_prb,
                   LteCp_t cp);
void LtePssPutSlot(Cf_t* pss_signal,
                   Cf_t* slot,
                   unsigned int n_of_prb,
                   LteCp_t cp);
void LtePssEmaAlpha(LtePss_t* q, double alpha);
int LtePssNId2(LtePss_t* q, unsigned int n_id_2);
int LtePssFindPss(LtePss_t* q, const Cf_t* input, float* corr_peak_value);
int LtePssChest(LtePss_t* q, const Cf_t* input, Cf_t ce[LTE_PSS_LEN]);
float LtePssCfoCompute(LtePss_t* q, const Cf_t* pss_recv);

#endif // __PSS_HPP__
