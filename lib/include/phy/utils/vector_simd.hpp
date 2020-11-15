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

#ifndef __VECTOR_SIMD_H__
#define __VECTOR_SIMD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "config.hpp"

#ifdef LV_HAVE_AVX512
#define LTE_SIMD_BIT_ALIGN 512
#define LTE_IS_ALIGNED(PTR) (((size_t)(PTR) & 0x3F) == 0)
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX
#define LTE_SIMD_BIT_ALIGN 256
#define LTE_IS_ALIGNED(PTR) (((size_t)(PTR) & 0x1F) == 0)
#else /* LV_HAVE_AVX */
#ifdef LV_HAVE_SSE
#define LTE_SIMD_BIT_ALIGN 128
#define LTE_IS_ALIGNED(PTR) (((size_t)(PTR) & 0x0F) == 0)
#else /* LV_HAVE_SSE */
#define LTE_SIMD_BIT_ALIGN 64
#define LTE_IS_ALIGNED(PTR) (1)
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX */
#endif /* LV_HAVE_AVX512 */


/*SIMD Logical operations*/
LTE_API void LteVecXorBbbSimd(const int8_t *x,
                              const int8_t *y,
                              int8_t *z,
                              int len);

/* SIMD Basic vector math */
LTE_API void LteVecSumSssSimd(const int16_t *x,
                              const int16_t *y,
                              int16_t *z,
                              int len);

LTE_API void LteVecSubSssSimd(const int16_t *x,
                              const int16_t *y,
                              int16_t *z,
                              int len);

LTE_API void LteVecSubBbbSimd(const int8_t *x,
                              const int8_t *y,
                              int8_t *z,
                              int len);

LTE_API float LteVecAccFfSimd(const float *x, int len);

LTE_API Cf_t LteVecAccCcSimd(const Cf_t *x, int len);

LTE_API void LteVecAddFffSimd(const float *x,
                              const float *y,
                              float *z,
                              int len);

LTE_API void LteVecSubFffSimd(const float *x,
                              const float *y,
                              float *z,
                              int len);

/* SIMD Vector Scalar Product */
LTE_API void LteVecScProdCfcSimd(const Cf_t *x,
                                 const float h,
                                 Cf_t *y,
                                 const int len);

LTE_API void LteVecScProdFffSimd(const float *x,
                                 const float h,
                                 float *z,
                                 const int len);

LTE_API void LteVecScProdCccSimd(const Cf_t *x,
                                 const Cf_t h,
                                 Cf_t *z,
                                 const int len);

/* SIMD Vector Product */
LTE_API void LteVecProdCccSplitSimd(const float *a_re,
                                    const float *a_im,
                                    const float *b_re,
                                    const float *b_im,
                                    float *r_re,
                                    float *r_im,
                                    const int len);

LTE_API void LteVecProdCccC16Simd(const int16_t *a_re,
                                  const int16_t *a_im,
                                  const int16_t *b_re,
                                  const int16_t *b_im,
                                  int16_t *r_re, 
                                  int16_t *r_im,
                                  const int len);

LTE_API void LteVecProdSssSimd(const int16_t *x,
                               const int16_t *y,
                               int16_t *z,
                               const int len);

LTE_API void LteVecNegSssSimd(const int16_t *x,
                              const int16_t *y,
                              int16_t *z,
                              const int len);

LTE_API void LteVecNegBbbSimd(const int8_t *x,
                              const int8_t *y,
                              int8_t *z,
                              const int len);

LTE_API void LteVecProdCfcSimd(const Cf_t *x,
                               const float *y,
                               Cf_t *z,
                               const int len);

LTE_API void LteVecProdFffSimd(const float *x,
                               const float *y,
                               float *z,
                               const int len);

LTE_API void LteVecProdCccSimd(const Cf_t *x,
                               const Cf_t *y,
                               Cf_t *z,
                               const int len);

LTE_API void LteVecProdConjCccSimd(const Cf_t *x,
                                   const Cf_t *y,
                                   Cf_t *z,
                                   const int len);

/* SIMD Division */
LTE_API void LteVecDivCccSimd(const Cf_t *x,
                              const Cf_t *y,
                              Cf_t *z,
                              const int len);

LTE_API void LteVecDivCfcSimd(const Cf_t *x,
                              const float *y,
                              Cf_t *z,
                              const int len);

LTE_API void LteVecDivFffSimd(const float *x,
                              const float *y,
                              float *z,
                              const int len);

/* SIMD Dot product */
LTE_API Cf_t LteVecDotProdConjCccSimd(const Cf_t *x,
                                      const Cf_t *y,
                                      const int len);

LTE_API Cf_t LteVecDotProdCccSimd(const Cf_t *x,
                                  const Cf_t *y,
                                  const int len);

#ifdef ENABLE_C16
LTE_API c16_t LteVecDotProdCccC16iSimd(const c16_t *x,
                                       const c16_t *y,
                                       const int len);
#endif /* ENABLE_C16 */

LTE_API int LteVecDotProdSssSimd(const int16_t *x,
                                 const int16_t *y,
                                 const int len);

/* SIMD Modulus functions */
LTE_API void LteVecAbsCfSimd(const Cf_t *x, float *z, const int len);

LTE_API void LteVecAbsSquareCfSimd(const Cf_t *x, float *z, const int len);

/* Other Functions */
LTE_API void LteVecLutSssSimd(const short *x,
                              const unsigned short *lut,
                              short *y,
                              const int len);

LTE_API void LteVecLutBbbSimd(const int8_t *x,
                              const unsigned short *lut,
                              int8_t *y,
                              const int len);

LTE_API void LteVecConvertIfSimd(const int16_t *x,
                                 float *z,
                                 const float scale,
                                 const int len);

LTE_API void LteVecConvertFiSimd(const float *x,
                                 int16_t *z,
                                 const float scale,
                                 const int len);

LTE_API void LteVecConvertFbSimd(const float *x,
                                 int8_t *z,
                                 const float scale,
                                 const int len);

LTE_API void LteVecCpSimd(const Cf_t *src, Cf_t *dst, int len);

LTE_API void LteVecInterleaveSimd(const Cf_t *x,
                                  const Cf_t *y,
                                  Cf_t *z,
                                  const int len);

LTE_API void LteVecInterleaveAddSimd(const Cf_t *x,
                                     const Cf_t *y,
                                     Cf_t *z,
                                     const int len);

LTE_API void LteVecApplyCfoSimd(const Cf_t *x, float cfo, Cf_t *z, int len);


/* SIMD Find Max functions */
LTE_API uint32_t LteVecMaxFiSimd(const float *x, const int len);

LTE_API uint32_t LteVecMaxAbsFiSimd(const float *x, const int len);

LTE_API uint32_t LteVecMaxCiSimd(const Cf_t *x, const int len);

#ifdef __cplusplus
  }
#endif

#endif // __VECTOR_SIMD_H__
