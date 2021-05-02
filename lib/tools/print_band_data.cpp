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
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <signal.h>

#include <unistd.h>

#include <iostream>

#include "srslte/srslte.h"

#include "srslte/phy/rf/rf_utils.h"
#include "srslte/common/crash_handler.h"

/* This matches the original definition in phy/common/phy_common.c" */
struct lte_band
  {
  uint32_t band;
  float fd_low_mhz;
  uint32_t dl_earfcn_offset;
  uint32_t ul_earfcn_offset;
  float duplex_mhz; 
  enum band_geographical_area area;
  };

extern struct lte_band lte_bands[SRSLTE_NOF_LTE_BANDS];

int main()
  {
  int i;

  for (i = 0; i < SRSLTE_NOF_LTE_BANDS; i++)
    {
    std::cout << lte_bands[i].band << ", " 
              << lte_bands[i].fd_low_mhz << ", "
              << lte_bands[i].dl_earfcn_offset << ", "
              << lte_bands[i].ul_earfcn_offset << ", "
              << lte_bands[i].duplex_mhz << ", ";
    switch (lte_bands[i].area)
      {
      case SRSLTE_BAND_GEO_AREA_ALL:
        std::cout << "ALL";
        break;
      case SRSLTE_BAND_GEO_AREA_NAR:
        std::cout << "NAR";
        break;
      case SRSLTE_BAND_GEO_AREA_APAC:
        std::cout << "APAC";
        break;
      case SRSLTE_BAND_GEO_AREA_EMEA:
        std::cout << "EMEA";
        break;
      case SRSLTE_BAND_GEO_AREA_JAPAN:
        std::cout << "JAPAN";
        break;
      case SRSLTE_BAND_GEO_AREA_NA:
        std::cout << "NA";
        break;
      case SRSLTE_BAND_GEO_AREA_CALA:
        std::cout << "CALA";
        break;
      default:
        std::cout << "UNKNOWN";
        break;
      }
    std::cout << std::endl;
    }

  return 0;
  }
