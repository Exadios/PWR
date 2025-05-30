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

#ifndef __PHY_HPP__
#define __PHY_HPP__

#include "PhchReceiver.hpp"
#include "PhchWorker.hpp"
#include "radio/radio.h"
#include "srslte.h"

class Phy
  {
public:
  
  /**
   * Ctor.
   */
  Phy();

  /**
   * Initialize the object.
   * @param radio_handler The radio handler.
   * @return If OK then true.
   */
  bool Init(RadioHandler* radio_handler);

  /**
   * Stop the operation of the object and enter a dormant state.
   */
  void Stop();

  /**
   * Test whether initialized.
   * @return If initialized then true.
   */
  bool IsInitialized() const;

  /**
   * Enable or disable AGC.
   * @param enable If it is desired to use AGC then true, otherwise false.
   */
  void EnableAGC(bool enable);

  /**
   * Force the receiver frequency
   * freq The new frequency.
   */
  void ForceFrequency(double f);

  /**
   * Handle a radio overflow. This will result in re synchronization.
   */
  void RadioOverflow();

  /**
   * Give the current pathloss.
   * @return Current pathloss in dB
   */
  double Pathloss() const;

private:
  bool initiated;

  Lte::RadioMulti         radio_handler;
  Lte::thread_pool        workers_pool;
  PhchCommon              workers_common;
  std::vector<PhchWorker> workers;
  PhchReceiver            reciever;

  const static int MAX_WORKERS         = 3;
  const static int DEFAULT_WORKERS     = 2;

  LteCell                 cell;

  int n_of_workers;
  };

#endif  // __PHY_HPP__
