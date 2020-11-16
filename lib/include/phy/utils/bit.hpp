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

#ifndef __BIT_H__
#define __BIT_H__

#include <stdint.h>
#include <stdio.h>

#include "config.hpp"

 /**
  *  Description:  Bit-level utilities.
  */

typedef struct
  {
  uint32_t nof_bits;
  uint16_t *interleaver;
  uint16_t *byte_idx;
  uint8_t *bit_mask;
  uint8_t n_128;
  }
LteBitInterleaver_t;

LTE_API void LteBitInterleaverInit(LteBitInterleaver_t *q,
                                   uint16_t *interleaver,
                                   uint32_t nof_bits);

LTE_API void LteBitInterleaverFree(LteBitInterleaver_t *q);

LTE_API void LteBitInterleaverRun(LteBitInterleaver_t *q,
                                  uint8_t *input,
                                  uint8_t *output,
                                  uint16_t w_offset);

LTE_API void LteBitInterleave(uint8_t *input, 
                              uint8_t *output, 
                              uint16_t *interleaver, 
                              uint32_t nof_bits); 

/**
 * Copy bits from src to dst, with offsets and length in bits
 *
 * @param[out] dst Output array
 * @param[in] src Input array
 * @param dst_offset Output array write offset in bits
 * @param src_offset Input array read offset in bits
 * @param nof_bits Number of bits to copy
 */
LTE_API void LteBitCopy(uint8_t *dst, 
                        uint32_t dst_offset, 
                        uint8_t *src, 
                        uint32_t src_offset, 
                        uint32_t nof_bits);

LTE_API void LteBitInterleaveI(uint8_t *input,
                               uint8_t *output,
                               uint32_t *interleaver,
                               uint32_t nof_bits);

LTE_API void LteBitInterleaveIWOffset(uint8_t *input,
                                      uint8_t *output,
                                      uint32_t *interleaver,
                                      uint32_t nof_bits,
                                      uint32_t w_offset);

LTE_API void LteBitInterleaveWOffset(uint8_t *input, 
                                     uint8_t *output, 
                                     uint16_t *interleaver, 
                                     uint32_t nof_bits, 
                                     uint32_t w_offset);

LTE_API void LteBitUnpackVector(uint8_t *packed,
                                uint8_t *unpacked,
                                int nof_bits);
                                         
LTE_API void LteBitPackVector(uint8_t *unpacked,
                              uint8_t *packed,
                              int nof_bits);

LTE_API uint32_t LteBitPack(uint8_t **bits, 
                            int nof_bits);

LTE_API uint64_t LteBitPackL(uint8_t **bits, 
                             int nof_bits);

LTE_API void LteBitUnpackL(uint64_t value, 
                           uint8_t **bits, 
                           int nof_bits);

/**
 * Unpacks nof_bits from LSBs of value in MSB order to *bits. Advances pointer past unpacked bits.
 *
 * @param[in] value nof_bits lowest order bits will be unpacked in MSB order
 * @param[in] nof_bits Number of bits to unpack
 * @param[out] bits Points to buffer pointer. The buffer pointer will be advanced by nof_bits
 */
LTE_API void LteBitUnpack(uint32_t value, 
                          uint8_t **bits, 
                          int nof_bits);

LTE_API void LteBitFprint(FILE *stream, 
                          uint8_t *bits, 
                          int nof_bits);

LTE_API uint32_t LteBitDiff(uint8_t *x, 
                            uint8_t *y, 
                            int nbits);

/**
 * Counts the number of ones in a word.
 * @param n The word.
 * @return Number of 'ones' in n.
 */
LTE_API uint32_t LteBitCount(uint32_t n);

#endif // __BIT_H__

