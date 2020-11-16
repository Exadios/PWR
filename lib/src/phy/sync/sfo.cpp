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


#include <stdio.h>
#include <stdlib.h>
#include "phy/sync/sfo.hpp"

//------------------------------------------------------------------------------
/* Estimate SFO based on the array of time estimates t0
 * of length len. The parameter period is the time between t0 samples
 */
float LteSfoEstimate(int *t0, int len, float period)
  {
  int i;
  float sfo = 0.0;
  for (i=1; i < len; i++)
    {
    sfo += (t0[i] - t0[i - 1]) / period / len;
    }
  return sfo;
  }

//------------------------------------------------------------------------------
/* Same as LteSfoEstimate but period is non-uniform.
 * Vector t is the sampling time times period for each t0
 */
float LteSfoEstimatePeriod(int *t0, int *t, int len, float period)
  {
  int i;
  float sfo = 0.0;
  for (i=1; i < len; i++)
    {
    if (abs(t0[i] - t0[i - 1]) < 5000)
      {
      sfo += (t0[i] - t0[i - 1]) / (t[i] - t[i - 1]) / period;
      }
    }
  return sfo / (len - 2);
  }
