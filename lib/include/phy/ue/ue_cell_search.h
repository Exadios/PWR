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
 *  File:         ue_cell_search.h
 *
 *  Description:  Wrapper for the ue_sync object.
 *
 *                This object is a wrapper to the ue_sync object. It receives
 *                several synchronized frames and obtains the most common
 *                cell_id
 *                and cp length.
 *
 *                The I/O stream device sampling frequency must be set to
 *                1.92 MHz
 *                (SRSLTE_CS_SAMP_FREQ constant) before calling to
 *                srslte_ue_cellsearch_scan() functions.
 *
 *  Reference:
 *****************************************************************************/

#ifndef __LTE_UE_CELL_SEARCH_H__
#define __LTE_UE_CELL_SEARCH_H__

#include <stdbool.h>

#include "srslte/config.h"
#include "srslte/phy/ue/ue_sync.h"
#include "srslte/phy/ue/ue_mib.h"
#include "srslte/phy/sync/cfo.h"
#include "srslte/phy/ch_estimation/chest_dl.h"
#include "srslte/phy/phch/pbch.h"
#include "srslte/phy/dft/ofdm.h"

#define SRSLTE_CS_NOF_PRB      6
#define SRSLTE_CS_SAMP_FREQ    1920000.0

typedef struct SRSLTE_API
  {
  uint32_t cell_id;
  srslte_cp_t cp;
  float peak;
  float mode;
  float psr;
  float cfo;
  } srslte_ue_cellsearch_result_t;


typedef struct SRSLTE_API
  {
  srslte_ue_sync_t ue_sync;

  cf_t *sf_buffer[SRSLTE_MAX_PORTS];
  uint32_t nof_rx_antennas;

  uint32_t max_frames;
  uint32_t nof_valid_frames;  // number of 5 ms frames to scan

  uint32_t *mode_ntimes;
  uint8_t *mode_counted;

  srslte_ue_cellsearch_result_t *candidates;
  } srslte_ue_cellsearch_t;

SRSLTE_API int srslte_ue_cellsearch_init(srslte_ue_cellsearch_t *q,
    uint32_t max_frames_total,
    int (recv_callback)(void*, void*, uint32_t,srslte_timestamp_t*),
    void *stream_handler);

SRSLTE_API int srslte_ue_cellsearch_init_multi(srslte_ue_cellsearch_t *q,
    uint32_t max_frames_total,
    int (recv_callback)(void*, cf_t*[SRSLTE_MAX_PORTS], uint32_t,srslte_timestamp_t*),
    uint32_t nof_rx_antennas,
    void *stream_handler);

SRSLTE_API void srslte_ue_cellsearch_free(srslte_ue_cellsearch_t *q);

SRSLTE_API int srslte_ue_cellsearch_scan_N_id_2(srslte_ue_cellsearch_t *q,
    uint32_t N_id_2,
    srslte_ue_cellsearch_result_t *found_cell);

SRSLTE_API int srslte_ue_cellsearch_scan(srslte_ue_cellsearch_t * q,
    srslte_ue_cellsearch_result_t found_cells[3],
    uint32_t *max_N_id_2);

SRSLTE_API int srslte_ue_cellsearch_set_nof_valid_frames(srslte_ue_cellsearch_t *q,
    uint32_t nof_frames);






#endif // __LTE_UE_CELL_SEARCH_H__

