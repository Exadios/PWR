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

#ifndef __VECTOR_H__
#define __VECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "config.hpp"


#define LTE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define LTE_MIN(a, b) ((a) < (b) ? (a) : (b))

// Cumulative moving average
#define LTE_VEC_CMA(data, average, n) ((average) +            \
                                       ((data) - (average)) / \
                                       ((n) + 1)) 

// Exponential moving average
#define LTE_VEC_EMA(data, average, alpha) ((alpha) *     \
                                           (data) +      \
                                           (1 - alpha) * \
                                           (average))


/*logical operations */
LTE_API void LteVecXorBbb(int8_t *x,int8_t *y,int8_t *z, const uint32_t len);

/** Return the sum of all the elements */
LTE_API float LteVecAccFf(const float *x, const uint32_t len);
LTE_API Cf_t LteVecAccCc(const Cf_t *x, const uint32_t len);

LTE_API void *LteVecMalloc(uint32_t size);

LTE_API void *LteVecRealloc(void *ptr, uint32_t old_size, uint32_t new_size);

/* print vectors */
LTE_API void LteVecFprintC(FILE *stream, Cf_t *x, const uint32_t len);
LTE_API void LteVecFprintF(FILE *stream, float *x, const uint32_t len);
LTE_API void LteVecFprintB(FILE *stream, uint8_t *x, const uint32_t len);
LTE_API void LteVecFprintBs(FILE *stream, int8_t *x, const uint32_t len);
LTE_API void LteVecFprintByte(FILE *stream, uint8_t *x, const uint32_t len);
LTE_API void LteVecFprintI(FILE *stream, int *x, const uint32_t len);
LTE_API void LteVecFprintS(FILE *stream, short *x, const uint32_t len); 
LTE_API void LteVecFprintHex(FILE *stream, uint8_t *x, const uint32_t len);
LTE_API void LteVecSprintHex(char *str,
                             const uint32_t max_str_len,
                             uint8_t *x,
                             const uint32_t len);

/* Saves/loads a vector to a file */
LTE_API void LteVecSaveFile(char *filename,
                            const void *buffer,
                            const uint32_t len);
LTE_API void LteVecLoadFile(char *filename,
                            void *buffer,
                            const uint32_t len);

/* sum two vectors */
LTE_API void LteVecSumFff(const float *x, const float *y, float *z, const uint32_t len);
LTE_API void LteVecSumCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len);
LTE_API void LteVecSumSss(const int16_t *x, const int16_t *y, int16_t *z, const uint32_t len);

/* substract two vectors z=x-y */
LTE_API void LteVecSubFff(const float *x, const float *y, float *z, const uint32_t len);
LTE_API void LteVecSubCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len);
LTE_API void LteVecSubSss(const int16_t *x, const int16_t *y, int16_t *z, const uint32_t len);
LTE_API void LteVecSubBbb(const int8_t *x, const int8_t *y, int8_t *z, const uint32_t len);

/* scalar product */
LTE_API void LteVecScProdCfc(const Cf_t *x, const float h, Cf_t *z, const uint32_t len);
LTE_API void LteVecScProdiCcc(const Cf_t *x, const Cf_t h, Cf_t *z, const uint32_t len);
LTE_API void LteVecScProdFff(const float *x, const float h, float *z, const uint32_t len);


LTE_API void LteVecConvertFi(const float *x, const float scale, int16_t *z, const uint32_t len);
LTE_API void LteVecConvertIf(const int16_t *x, const float scale, float *z, const uint32_t len);
LTE_API void LteVecConvertFb(const float *x, const float scale, int8_t *z, const uint32_t len);

LTE_API void LteVecLutSss(const short *x, const unsigned short *lut, short *y, const uint32_t len);
LTE_API void LteVecLutBbb(const int8_t *x, const unsigned short *lut, int8_t *y, const uint32_t len);
LTE_API void LteVecLutSis(const short *x, const unsigned int *lut, short *y, const uint32_t len);

/* vector product (element-wise) */
LTE_API void LteVecProdCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len);
LTE_API void LteVecProdCccSplit(const float *x_re, const float *x_im, const float *y_re, const float *y_im, float *z_re, float *z_im, const uint32_t len);

/* vector product (element-wise) */
LTE_API void LteVecProdCfc(const Cf_t *x, const float *y, Cf_t *z, const uint32_t len);

/* conjugate vector product (element-wise) */
LTE_API void LteVecProdConjCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len);

/* real vector product (element-wise) */
LTE_API void LteVecProdFff(const float *x, const float *y, float *z, const uint32_t len);
LTE_API void LteVecProdSss(const int16_t *x, const int16_t *y, int16_t *z, const uint32_t len);

// Negate sign (scrambling)
LTE_API void LteVecNegSss(const int16_t *x, const int16_t *y, int16_t *z, const uint32_t len);
LTE_API void LteVecNegBbb(const int8_t *x, const int8_t *y, int8_t *z, const uint32_t len);

/* Dot-product */
LTE_API Cf_t LteVecDotProdCfc(const Cf_t *x, const float *y, const uint32_t len);
LTE_API Cf_t LteVecDotProdCcc(const Cf_t *x, const Cf_t *y, const uint32_t len);
LTE_API Cf_t LteVecDotProdConjCcc(const Cf_t *x, const Cf_t *y, const uint32_t len);
LTE_API float LteVecDotProdFff(const float *x, const float *y, const uint32_t len);
LTE_API int32_t LteVecDotProdSss(const int16_t *x, const int16_t *y, const uint32_t len); 

/* z=x/y vector division (element-wise) */
LTE_API void LteVecDivCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len);
LTE_API void LteVecDivCfc(const Cf_t *x, const float *y, Cf_t *z, const uint32_t len);
LTE_API void LteVecDivFff(const float *x, const float *y, float *z, const uint32_t len);

/* conjugate */
LTE_API void LteVecConjCc(const Cf_t *x, Cf_t *y, const uint32_t len);

/* average vector power */
LTE_API float LteVecAvgPowerCf(const Cf_t *x, const uint32_t len);

/* Correlation between complex vectors x and y */
LTE_API float LteVecCorrCcc(const Cf_t *x, Cf_t *y, const uint32_t len);

/* return the index of the maximum value in the vector */
LTE_API uint32_t LteVecMaxFi(const float *x, const uint32_t len);
LTE_API uint32_t LteVecMaxAbsFi(const float *x, const uint32_t len);
LTE_API uint32_t LteVecMaxAbsCi(const Cf_t *x, const uint32_t len);

/* quantify vector of floats or int16 and convert to uint8_t */
LTE_API void LteVecQuantFuc(const float *in, uint8_t *out, const float gain, const float offset, const float clip, const uint32_t len);
LTE_API void LteVecQuantFus(float *in, uint16_t *out, float gain, float offset, float clip, uint32_t len);
LTE_API void LteVecQuantSuc(const int16_t *in, uint8_t *out, const float gain, const int16_t offset, const int16_t clip, const uint32_t len); 
LTE_API void LteVecQuantSus(const int16_t *in, uint16_t *out, const float gain, const int16_t offset, const uint32_t len);
/* magnitude of each vector element */
LTE_API void LteVecAbsCf(const Cf_t *x, float *abs, const uint32_t len);
LTE_API void LteVecAbsSquareCf(const Cf_t *x, float *abs_square, const uint32_t len);

/* Copy 256 bit aligned vector */
LTE_API void VecCfCpy(const Cf_t *src, Cf_t *dst, const int len);

LTE_API void LteVecInterleave(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len);

LTE_API void LteVecInterleaveAdd(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len);

LTE_API void LteVecApplyCfo(const Cf_t *x, float cfo, Cf_t *z, int len);


#ifdef __cplusplus
}
#endif

#endif // __VECTOR_H__
