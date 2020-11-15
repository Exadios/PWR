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
int LtePssResize(LtePss_t* q,
                 unsigned int frame_size,
                 unsigned int fft_size,
                 int offset);
int LtePssInit(LtePss_t* q, unsigned int frame_size);
void LtePssFree(LtePss_t* q);
void LtePssReset(LtePss_t* q);
void LtePssFilter(LtePss_t* q, bool enable);
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
