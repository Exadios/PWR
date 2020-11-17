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
 *  Description:  Time and frequency synchronization using the PSS and SSS
 *                signals.
 *
 *                The object is designed to work with signals sampled at
 *                1.92 Mhz centered at the carrier frequency. Thus,
 *                downsampling is required if the signal is sampled at higher
 *                frequencies.
 *
 *                Correlation peak is detected comparing the maximum at the 
 *                output of the correlator with a threshold. The comparison
 *                accepts two modes: absolute value or peak-to-mean ratio,
 *                which are configured with the functions 
 *                sync_pss_det_absolute() and sync_pss_det_peakmean().
 *
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10 Sec. 6.11.1, 6.11.2
 *****************************************************************************/

#ifndef __SYNC_H__
#define __SYNC_H__

#include <stdbool.h>
#include <math.h>

#include "config.hpp"
#include "phy/sync/pss.hpp"
#include "phy/sync/sss.hpp"
#include "phy/sync/cfo.hpp"
#include "phy/sync/cp.hpp"

#define LTE_SYNC_FFT_SZ_MIN    64
#define LTE_SYNC_FFT_SZ_MAX    2048

typedef enum
  {
   SSS_DIFF = 0,
   SSS_PARTIAL_3 = 2,
   SSS_FULL=1
  }
SssAlg_t; 

typedef struct LTE_API
  {
  LtePss_t pss;
  LtePss_t pss_i[2];
  LteSss_t sss;
  LteCpSynch_t cp_synch;
  Cf_t *cfo_i_corr[2];
  int decimate;
  float threshold;
  float peak_value;
  uint32_t N_id_2;
  uint32_t N_id_1;
  uint32_t sf_idx;
  uint32_t fft_size;
  uint32_t frame_size;
  uint32_t max_offset;
  uint32_t nof_symbols;
  uint32_t cp_len;
  float current_cfo_tol;
  SssAlg_t sss_alg; 
  bool detect_cp;
  bool sss_en;
  LteCp_t cp;
  uint32_t m0;
  uint32_t m1;
  float m0_value;
  float m1_value;
  float M_norm_avg; 
  float M_ext_avg; 
  Cf_t  *temp;

  uint32_t max_frame_size;


  // variables for various CFO estimation methods
  bool cfo_cp_enable;
  bool cfo_pss_enable;
  bool cfo_i_enable;

  bool cfo_cp_is_set;
  bool cfo_pss_is_set;
  bool cfo_i_initiated;

  float cfo_cp_mean;
  float cfo_pss;
  float cfo_pss_mean;
  int   cfo_i_value;

  float cfo_ema_alpha;

  uint32_t cfo_cp_nsymbols;

  LteCfo_t cfo_corr_frame;
  LteCfo_t cfo_corr_symbol;

  bool sss_channel_equalize;
  bool pss_filtering_enabled;
  Cf_t sss_filt[LTE_SYMBOL_SZ_MAX];
  Cf_t pss_filt[LTE_SYMBOL_SZ_MAX];

  }
LteSync_t;

typedef enum
  {
  LTE_SYNC_FOUND = 1, 
  LTE_SYNC_FOUND_NOSPACE = 2, 
  LTE_SYNC_NOFOUND = 0, 
  LTE_SYNC_ERROR = -1  
  }
LteSyncFindRet_t; 

LTE_API int LteSyncInit(LteSync_t *q, 
                        uint32_t frame_size, 
                        uint32_t max_offset,
                        uint32_t fft_size);

LTE_API int LteSyncInitDecim(LteSync_t *q, 
                             uint32_t frame_size, 
                             uint32_t max_offset,
                             uint32_t fft_size,
                             int decimate);


LTE_API void LteSyncFree(LteSync_t *q);

LTE_API int LteSyncResize(LteSync_t *q,
                          uint32_t frame_size,
                          uint32_t max_offset,
                          uint32_t fft_size);

LTE_API void LteSyncReset(LteSync_t *q); 

/* Finds a correlation peak in the input signal around position find_offset */
/** Finds the PSS sequence previously defined by a call to LteSyncSetNId2()
 * around the position find_offset in the buffer input.
 *
 * Returns 1 if the correlation peak exceeds the threshold set by
 * LteSyncSetThreshold() or 0 otherwise. Returns a negative number on error
 * (if N_id_2 has not been set).
 *
 * The input signal is not modified. Any CFO correction is done in internal
 * buffers
 *
 * The maximum of the correlation peak is always stored in *peak_position
 *
 * @param q This object.
 * @param input The observed sequence.
 * @param offset The offset into the input sequence.
 * @param peak_position The position of the correlation peak, if found.
 * @return If the correlation peak is greater than the threshold previously
 *         set by LteSyncSetThreshold() then return 1, return 0 otherwise.
 *         If an error is detected then return negative number.
 */

LTE_API LteSyncFindRet_t LteSyncFind(LteSync_t *q, 
                                     const Cf_t *input,
                                     uint32_t find_offset,
                                     uint32_t *peak_position);

/* Estimates the CP length */
LTE_API LteCp_t LteSyncDetectCp(LteSync_t *q, 
                                    const Cf_t *input,
                                    uint32_t peak_pos);

/* Sets the threshold for peak comparison */
LTE_API void LteSyncSetThreshold(LteSync_t *q, float threshold);

/* Gets the subframe idx (0 or 5) */
LTE_API uint32_t LteSyncGetSfIdx(LteSync_t *q);

/* Gets the peak value */
LTE_API float LteSyncGetPeakValue(LteSync_t *q);

/* Choose SSS detection algorithm */
LTE_API void LteSyncSetSss_algorithm(LteSync_t *q, SssAlg_t alg); 

/* Sets PSS exponential averaging alpha weight */
LTE_API void LteSyncSetEmAlpha(LteSync_t *q, float alpha);

/* Sets the N_id_2 to search for.
 * @param N_id_2 The cell id.
 * @return Either LTE_SUCCESS or  LTE_ERROR_INVALID_INPUTS.
 */
LTE_API int LteSyncSetNId2(LteSync_t *q, uint32_t N_id_2);

/* Gets the Physical CellId from the last call to synch_run() */
LTE_API int LteSyncGetCellId(LteSync_t *q);

/* Enables/disables filtering of the central PRBs before PSS CFO estimation or SSS correlation*/
LTE_API void LteSyncSetPssFiltEnable(LteSync_t *q, bool enable);

LTE_API void LteSyncSetSssEq_enable(LteSync_t *q, bool enable);

/* Gets the CFO estimation from the last call to synch_run() */
LTE_API float LteSyncGetCfo(LteSync_t *q);

/* Resets internal CFO state */
LTE_API void LteSyncCfoReset(LteSync_t *q);

/* Copies CFO internal state from another object to avoid long transients */
LTE_API void LteSyncCopyCfo(LteSync_t *q, LteSync_t *src_obj);

/* Enable different CFO estimation stages */
LTE_API void LteSyncSetCfoIEnable(LteSync_t *q, bool enable);
LTE_API void LteSyncSetCfoCpEnable(LteSync_t *q,
                                   bool enable,
                                   uint32_t nof_symbols);

LTE_API void LteSyncSetCfoPssEnable(LteSync_t *q, bool enable);

/* Sets CFO correctors tolerance (in Hz) */
LTE_API void LteSyncSetCfoTol(LteSync_t *q, float tol);

/* Sets the exponential moving average coefficient for CFO averaging */
LTE_API void LteSyncSetCfoEmaAlpha(LteSync_t *q, float alpha);


/* Gets the CP length estimation from the last call to synch_run() */
LTE_API LteCp_t LteSyncGetCp(LteSync_t *q);

/* Sets the CP length estimation (must do it if disabled) */
LTE_API void LteSyncSetCp(LteSync_t *q, LteCp_t cp);

/* Enables/Disables SSS detection  */
LTE_API void LteSyncSssEn(LteSync_t *q, bool enabled);

LTE_API LtePss_t* LteSyncGetCurPssObj(LteSync_t *q);

LTE_API bool LteSyncSssDetected(LteSync_t *q);

/* Enables/Disables CP detection  */
LTE_API void LteSyncCpEn(LteSync_t *q, bool enabled);

#endif // __SYNC_H__

