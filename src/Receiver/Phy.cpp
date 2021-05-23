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

#include <string>
#include <sstream>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>

#include "Phy.hpp"

//------------------------------------------------------------------------------
Phy::Phy()
  : workers_pool(MAX_WORKERS),
    workers(MAX_WORKERS),
    workers_common(MAX_WORKERS)
  {
  ZERO_OBJECT(cell);
  }

//------------------------------------------------------------------------------
void
Phy::Init(RadioMulti* radio_handler)
  {
  this->radio_handler = radio_handler;
  this->initialized   = false;
  return true;
  }

//------------------------------------------------------------------------------
bool
Phy::IsInitiated() const
  {
  return this->initiated;
  }

//------------------------------------------------------------------------------
void
Phy::EnableAGC(bool enable)
  {
  this->receiver.EnableAGC(enable);
  }

//------------------------------------------------------------------------------
void
Phy::Stop()
  {
  this->receiver.Stop();
  this->workers_pool.Stop();
  }

//------------------------------------------------------------------------------
double
Phy::Pathloss() const
  {
  return this->workers_common.CurrentPathloss();
  }


