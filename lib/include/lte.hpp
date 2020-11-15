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

#include "config.h"
#include "version.h"

#include "phy/utils/bit.h"
#include "phy/utils/ringbuffer.h"
#include "phy/utils/convolution.h"
#include "phy/utils/debug.h"
#include "phy/utils/cexptab.h"
#include "phy/utils/vector.h"

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

#include "phy/sync/pss.h"
#include "phy/sync/sfo.h"
#include "phy/sync/sss.h"
#include "phy/sync/sync.h"
#include "phy/sync/cfo.h"
#include "phy/sync/cp.h"

#ifdef __cplusplus
}
#undef I // Fix complex.h #define I nastiness when using C++
#endif

#endif // __LTE_H__
