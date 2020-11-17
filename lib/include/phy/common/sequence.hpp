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

/*******************************************************************************
 *
 *  Description:  Pseudo Random Sequence generation. Sequences are defined by
                  a (length - 31) Gold sequence.
 *
 *  Reference:    3GPP TS 36.211 version 10.0.0 Release 10 Sec. 7.2
 ******************************************************************************/

#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include "config.hpp"
#include "phy/common/phy_common.hpp"

typedef struct LTE_API
  {
  uint8_t *c;
  uint8_t *c_bytes;
  float *c_float;
  short *c_short;
  int8_t *c_char;
  uint32_t cur_len;
  uint32_t max_len;
  } LteSequence_t;

LTE_API int LteSequenceInit(LteSequence_t *q, uint32_t len);

LTE_API void LteSequenceFree(LteSequence_t *q);

LTE_API int LteSequenceLTEPr(LteSequence_t *q,
                             uint32_t len,
                             uint32_t seed);

LTE_API int LteSequenceSetLTEPr(LteSequence_t *q,
                                uint32_t len,
                                uint32_t seed);

LTE_API int LteSequencePbch(LteSequence_t *seq,
                            LteCp_t cp,
                            uint32_t cell_id);

LTE_API int LteSequencePcfich(LteSequence_t *seq,
                              uint32_t nslot,
                              uint32_t cell_id);

LTE_API int LteSequencePhich(LteSequence_t *seq,
                             uint32_t nslot,
                             uint32_t cell_id);

LTE_API int LteSequencePdcch(LteSequence_t *seq,
                             uint32_t nslot,
                             uint32_t cell_id,
                             uint32_t len);

LTE_API int LteSequencePdsch(LteSequence_t *seq,
                             uint16_t rnti,
                             int q,
                             uint32_t nslot,
                             uint32_t cell_id,
                             uint32_t len);

LTE_API int LteSequencePusch(LteSequence_t *seq,
                             uint16_t rnti,
                             uint32_t nslot,
                             uint32_t cell_id,
                             uint32_t len);

LTE_API int LteSequencePucch(LteSequence_t *seq,
                             uint16_t rnti,
                             uint32_t nslot,
                             uint32_t cell_id);

LTE_API int LteSequencePmch(LteSequence_t *seq,
                            uint32_t nslot,
                            uint32_t mbsfn_id,
                            uint32_t len);

#endif // __SEQUENCE_H__
