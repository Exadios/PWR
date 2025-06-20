/*
 * \section COPYRIGHT
 *
 * Passive Weather Radar (PWR).
 * Copyright (C) 2010-2025 Peter F Bradshaw
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

#pragma once

#define TX_MODE_CONTINUOUS 1


#include <pthread.h>
#include <string.h>
#include <vector>
#include <semaphore.h>
#include "srslte.h"
#include "interfaces/ue_interfaces.h"
#include "radio/radio.h"
#include "common/log.h"
#include "Metrics.hpp"
#include "srslte/common/gen_mch_tables.h"
//#include "phy_metrics.h"

namespace srsue {

class ChestFeedbackItf
  {
  public:
    virtual void InSync() = 0;
    virtual void OutOfSync() = 0;
    virtual void SetCfo(float cfo) = 0;
  };


typedef enum
  {
  SUBFRAME_TYPE_REGULAR = 0,
  SUBFRAME_TYPE_MBSFN,
  SUBFRAME_TYPE_N_ITEMS,
  } subframe_type_t;
static const char subframe_type_text[SUBFRAME_TYPE_N_ITEMS][20] = 
  {
   "Regular",
   "MBSFN"
  };

/* Subframe config */

typedef struct
  {
  subframe_type_t sf_type;
  uint8_t         mbsfn_area_id;
  uint8_t         non_mbsfn_region_length;
  uint8_t         mbsfn_mcs;
  bool            mbsfn_decode;
  bool            is_mcch;
  } subframe_cfg_t;


/* Subclass that manages variables common to all workers */
class PhchCommon
  {
  public:

    /* Common variables used by all phy workers */
    srsue::phy_interface_rrc::phy_cfg_t *config;
    srsue::phy_args_t                   *args;
    srsue::rrc_interface_phy *rrc;
    srsue::mac_interface_phy *mac;
    srslte_ue_ul_t     ue_ul;

    /* Power control variables */
    float pathloss;
    float cur_pathloss;
    float p0_preamble;
    float cur_radio_power;
    float cur_pusch_power;
    float avg_rsrp;
    float avg_rsrp_cqi;
    float avg_rsrp_dbm;
    float avg_rsrp_sync_dbm;
    float avg_rsrq_db;
    float avg_rssi_dbm;
    float last_radio_rssi;
    float rx_gain_offset;
    float avg_snr_db_cqi;
    float avg_snr_db_sync;
    float avg_ri;

    float avg_noise;
    bool  pcell_meas_enabled;

    uint32_t pcell_report_period;
    bool     pcell_first_measurement;

    // Save last TBS for mcs>28 cases
    int last_dl_tbs[2 * HARQ_DELAY_MS][SRSLTE_MAX_CODEWORDS];
    uint32_t last_dl_tti[2 * HARQ_DELAY_MS];

    int last_ul_tbs[2 * HARQ_DELAY_MS];
    uint32_t last_ul_tti[2 * HARQ_DELAY_MS];
    srslte_mod_t last_ul_mod[2 * HARQ_DELAY_MS];
    uint32_t last_ul_idx[2 * HARQ_DELAY_MS];
    uint8_t last_ri;
    uint8_t last_pmi;

    PhchCommon(uint32_t max_workers);
    ~PhchCommon();
    void Init(srsue::phy_interface_rrc::phy_cfg_t *config,
              srsue::phy_args_t  *args,
              srslte::log *_log,
              srslte::radio *_radio,
              srsue::rrc_interface_phy *rrc,
              srsue::mac_interface_phy *_mac);

    /* For RNTI searches, -1 means now or forever */
    void               set_ul_rnti(srslte_rnti_type_t type, uint16_t rnti_value, int tti_start = -1, int tti_end = -1);
    uint16_t           get_ul_rnti(uint32_t tti);
    srslte_rnti_type_t get_ul_rnti_type();

    void               set_dl_rnti(srslte_rnti_type_t type, uint16_t rnti_value, int tti_start = -1, int tti_end = -1);
    uint16_t           get_dl_rnti(uint32_t tti);
    srslte_rnti_type_t get_dl_rnti_type();

    void set_rar_grant(uint32_t tti, uint8_t grant_payload[SRSLTE_RAR_GRANT_LEN]);
    bool get_pending_rar(uint32_t tti, srslte_dci_rar_grant_t *rar_grant = NULL);

    void reset_pending_ack(uint32_t tti);
    void set_pending_ack(uint32_t tti, uint32_t I_lowest, uint32_t n_dmrs);
    bool get_pending_ack(uint32_t tti);
    bool get_pending_ack(uint32_t tti, uint32_t *I_lowest, uint32_t *n_dmrs);
    bool is_any_pending_ack();

    void worker_end(uint32_t tti, bool tx_enable, cf_t *buffer, uint32_t nof_samples, srslte_timestamp_t tx_time);

    void set_nof_workers(uint32_t nof_workers);
    bool sr_enabled;
    int  sr_last_tx_tti;


    srslte::radio*    get_radio();

    void set_cell(const srslte_cell_t &c);
    uint32_t get_nof_prb();
    void set_dl_metrics(const dl_metrics_t &m);
    void get_dl_metrics(dl_metrics_t &m);
    void set_ul_metrics(const ul_metrics_t &m);
    void get_ul_metrics(ul_metrics_t &m);
    void set_sync_metrics(const sync_metrics_t &m);
    void get_sync_metrics(sync_metrics_t &m);

    void reset_ul();
    void reset();

    // MBSFN helpers
    void build_mch_table();
    void build_mcch_table();
    void set_mcch();
    void get_sf_config(subframe_cfg_t *cfg, uint32_t phy_tti);
    void set_mch_period_stop(uint32_t stop);

  private:

    bool have_mtch_stop;
    pthread_mutex_t mtch_mutex;
    pthread_cond_t  mtch_cvar;



    std::vector<sem_t>    tx_sem;
    uint32_t              nof_workers;
    uint32_t              max_workers;

    bool               is_first_of_burst;
    srslte::radio      *radio_h;
    float              cfo;
    srslte::log       *log_h;


    bool               ul_rnti_active(uint32_t tti);
    bool               dl_rnti_active(uint32_t tti);
    uint16_t           ul_rnti, dl_rnti;
    srslte_rnti_type_t ul_rnti_type, dl_rnti_type;
    int                ul_rnti_start, ul_rnti_end, dl_rnti_start, dl_rnti_end;

    float              time_adv_sec;

    srslte_dci_rar_grant_t rar_grant;
    bool                   rar_grant_pending;
    uint32_t               rar_grant_tti;

    typedef struct
      {
      bool enabled;
      uint32_t I_lowest;
      uint32_t n_dmrs;
      } pending_ack_t;
    pending_ack_t pending_ack[TTIMOD_SZ];

    bool            is_first_tx;

    srslte_cell_t   cell;

    dl_metrics_t    dl_metrics;
    uint32_t        dl_metrics_count;
    bool            dl_metrics_read;
    ul_metrics_t    ul_metrics;
    uint32_t        ul_metrics_count;
    bool            ul_metrics_read;
    sync_metrics_t  sync_metrics;
    uint32_t        sync_metrics_count;
    bool            sync_metrics_read;

    // MBSFN
    bool sib13_configured;
    bool mcch_configured;
    uint32_t mch_period_stop;
    uint8_t mch_table[40];
    uint8_t mcch_table[10];

    bool is_mch_subframe(subframe_cfg_t *cfg, uint32_t phy_tti);
    bool is_mcch_subframe(subframe_cfg_t *cfg, uint32_t phy_tti);
  };

}
