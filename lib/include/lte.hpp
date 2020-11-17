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

#ifndef __LTE_H__
#define __LTE_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <complex.h>
#include <math.h>

#include "config.hpp"
#include "version.hpp"

#include "phy/utils/bit.hpp"
#if 0
#include "phy/utils/ringbuffer.h"
#include "phy/utils/convolution.h"
#include "phy/utils/debug.h"
#include "phy/utils/cexptab.h"
#include "phy/utils/vector.hpp"

#include "phy/common/timestamp.h"
#include "phy/common/sequence.h"
#include "phy/common/phy_common.h"
#include "phy/common/phy_logger.h"

#include "phy/resampling/interp.h"
#include "phy/resampling/decim.h"
#include "phy/resampling/resample_arb.h"

#include "phy/dft/dft_precoding.h"
#include "phy/dft/ofdm.h"
#include "phy/dft/dft.h"
#endif

#include "phy/sync/pss.hpp"
#include "phy/sync/sfo.hpp"
#include "phy/sync/sss.hpp"
#include "phy/sync/sync.hpp"
#include "phy/sync/cfo.hpp"
#include "phy/sync/cp.hpp"

#ifdef __cplusplus
}
#undef I // Fix complex.h #define I nastiness when using C++
#endif

#endif // __LTE_H__
