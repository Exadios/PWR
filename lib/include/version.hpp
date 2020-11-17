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

#ifndef __VERSION_H__
#define __VERSION_H__

// the configured options and settings for LTE
#define LTE_VERSION_MAJOR 18
#define LTE_VERSION_MINOR 12
#define LTE_VERSION_PATCH 0
#define LTE_VERSION_STRING "18.12.0"

#define LTE_VERSION_ENCODE(major, minor, patch) ( \
    ((major) * 10000)                                \
  + ((minor) *   100)                                \
  + ((patch) *     1))

#define LTE_VERSION LTE_VERSION_ENCODE( \
  LTE_VERSION_MAJOR,                       \
  LTE_VERSION_MINOR,                       \
  LTE_VERSION_PATCH)

#define LTE_VERSION_CHECK(major,minor,patch)    \
  (LTE_VERSION >= LTE_VERSION_ENCODE(major,minor,patch))

#include "config.hpp"

LTE_API char* srslte_get_version();
LTE_API int   srslte_get_version_major();
LTE_API int   srslte_get_version_minor();
LTE_API int   srslte_get_version_patch();
LTE_API int   srslte_check_version(int major, int minor, int patch);

#endif // __VERSION_H__
