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

#ifndef __PHCHRECEIVER_HPP__
#define __PHCHRECEIVER_HPP__

#include "srslte/srslte.h"
#include "srslte/radio/radio_multi.h"
#include "srslte/interfaces/ue_interfaces.h"
#include "PhchWorker.hpp"
#include "PhchCommon.hpp"

/**
 * PHCH receiver.
 */
class PhchReceiver
  {
public:
  /**
   * Ctor.
   */
  PhchReceiver();

  /**
   * Dtor.
   */
  ~PhchReceiver();

  /**
   * Initialize the object.
   * @param radio_handler The raw radio.
   * @param worker_common
   * @param n_of_antennas Number of receive streams.
   */
  void Init(srslte::radio_multi* radio_handler,
            srsue::PhchCommon*   worker_common,
            int                  n_of_antennas);

  /**
   * Stop the object.
   */
  void Stop();

  /**
   * Overflow. Reset and require.
   */
  void RadioOverflow();

  /**
   * Finds the strongest cell in a EARFCN. Cell searches are performed at
   * 1.92 mHz sampling rate and involves PSS/SSS synchronization.
   * @param cell The return data for any cell found.
   * @return If a cell is found fill out cell and return 1. If no cells
   *         found return 0. If error return -1.
   */
  srsue::phy_interface_rrc::cell_search_ret_t CellSearch(PhyCell_t *cell);

private:
  /**
   * Class to run a cell search.
   */
  class Search
    {
  public:
    
    typedef enum
      {
      CELL_NOT_FOUND,
      CELL_FOUND,
      ERROR,
      TIMEOUT
      } ret_code;

    /**
     * Dtor.
     */
    ~Search();

    /**
     * Initialize the object.
     * @param buffer Output buffer.
     * @param n_of_receive_antennas Number of receive antennas.
     * @param parent Parent receiver.
     */
    void Init(cf_t *buffer[LTE_MAX_PORTS],
              int n_of_receive_antennas,
              PhchReceiver *parent);

    /**
     * Reset the object.
     */
    void Reset();

    /**
     * Control the AGC state.
     * @param enable If true the set AGC active.
     */
    void AGCEnable(bool enable);

  private:
    PhchReceiver *pr;
    Cf_t         *buffer[LTE_MAX_PORTS];
    };

  std::vector<int> earfcn;

  void Reset();
  void RadioError();
  void RunThread();
  void SampleRate();
  bool Frequency();
  bool Cell();
  
  int new_earfcn;
  LteCell_t new_cell;

  /**
   * State machine for the sync thread.
   */
  class SyncState
    {
    typedef enum =
      {
      IDLE = 0,
      CELL_SEARCH,
      SFN_SYNC,
      CAMPING,
      }
    State_t;

    /**
     * Give the current state.
     * @return The current run state.
     */
    State_t RunState();
    
    /**
     * Called to indicate that the current state is exiting.
     * @param exit If true and the current is SFN_SYNC then the next state
     *             will be CAMPING otherwise the next state will be IDLE.
     */
    void StateExit(bool exit = true);

    /**
     * Force the next state to be SFN_SYNC
     */
    void ForceSfnSync();

    /**
     * Is the state currently IDLE?
     * @return If IDLE then true.
     */
    bool IsIdle() const;

    /**
     * Is the state currently CAMPING?
     * @return If CAMPING then true.
     */
    bool IsCamping() const;

    /**
     * Give the string representation of the current state.
     * @return The current state as ASCII.
     */
    const char *ToString() const;

    /**
     * Switch to initial state.
     */
    void SyncState();
    };

  private:
    void GoToState(State_t s);
    void WaitStateChange(State_t prev_state);

    bool state_changing, state_setting;
    };
  };
#endif  // __PHCHRECEIVER_HPP__
