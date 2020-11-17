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

#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#include <config.hpp>
#include "phy/vector/vector_simd.hpp"
#include "phy/vector/simd.hpp"


//------------------------------------------------------------------------------
void LteVecXorBbbSimd(const int8_t *x,
                      const int8_t *y,
                      int8_t *z,
                      const int len)
  {
  int i = 0;
#if LTE_SIMD_B_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_B_SIZE + 1; i += LTE_SIMD_B_SIZE)
      {
      SimdB_t a = LteSimdBLoad(&x[i]);
      SimdB_t b = LteSimdBLoad(&y[i]);

      SimdB_t r = LteSimdBXor(a, b);

      LteSimdBStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_B_SIZE + 1; i += LTE_SIMD_B_SIZE)
      {
      SimdB_t a = LteSimdBLoadu(&x[i]);
      SimdB_t b = LteSimdBLoadu(&y[i]);

      SimdB_t r = LteSimdBXor(a, b);

      LteSimdBStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_B_SIZE */

  for(; i < len; i++)
    {
    z[i] = x[i] ^ y[i];
    }
  }

//------------------------------------------------------------------------------
int LteVecDotProdSssSimd(const int16_t *x, const int16_t *y, const int len)
  {
  int i = 0;
  int result = 0;
#if LTE_SIMD_S_SIZE
  SimdS_t simd_dotProdVal = LteSimdSZero();
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoad(&x[i]);
      SimdS_t b = LteSimdSLoad(&y[i]);

      SimdS_t z = LteSimdSMul(a, b);

      simd_dotProdVal = LteSimdSAdd(simd_dotProdVal, z);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoadu(&x[i]);
      SimdS_t b = LteSimdSLoadu(&y[i]);

      SimdS_t z = LteSimdSMul(a, b);

      simd_dotProdVal = LteSimdSAdd(simd_dotProdVal, z);
      }
    }
  __attribute__ ((aligned (LTE_SIMD_S_SIZE*2))) short dotProdVector[LTE_SIMD_S_SIZE];
  LteSimdSStore(dotProdVector, simd_dotProdVal);
  for (int k = 0; k < LTE_SIMD_S_SIZE; k++)
    {
    result += dotProdVector[k];
    }
#endif /* LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    result += (x[i] * y[i]);
    }

  return result;
  }

//------------------------------------------------------------------------------
void LteVecSumSssSimd(const int16_t *x,
                      const int16_t *y,
                      int16_t *z,
                      const int len)
  {
  int i = 0;
#if LTE_SIMD_S_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoad(&x[i]);
      SimdS_t b = LteSimdSLoad(&y[i]);

      SimdS_t r = LteSimdSAdd(a, b);

      LteSimdSStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoadu(&x[i]);
      SimdS_t b = LteSimdSLoadu(&y[i]);

      SimdS_t r = LteSimdSAdd(a, b);

      LteSimdSStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    z[i] = x[i] + y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecSubSssSimd(const int16_t *x,
                      const int16_t *y,
                      int16_t *z,
                      const int len)
  {
  int i = 0;
#if LTE_SIMD_S_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoad(&x[i]);
      SimdS_t b = LteSimdSLoad(&y[i]);

      SimdS_t r = LteSimdSSub(a, b);

      LteSimdSStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoadu(&x[i]);
      SimdS_t b = LteSimdSLoadu(&y[i]);

      SimdS_t r = LteSimdSSub(a, b);

      LteSimdSStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    z[i] = x[i] - y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecSubBbbSimd(const int8_t *x,
                      const int8_t *y,
                      int8_t *z,
                      const int len)
  {
  int i = 0;
#if LTE_SIMD_B_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_B_SIZE + 1; i += LTE_SIMD_B_SIZE)
      {
      SimdB_t a = LteSimdBLoad(&x[i]);
      SimdB_t b = LteSimdBLoad(&y[i]);

      SimdB_t r = LteSimdBSub(a, b);

      LteSimdBStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdB_t a = LteSimdBLoadu(&x[i]);
      SimdB_t b = LteSimdBLoadu(&y[i]);

      SimdB_t r = LteSimdBSub(a, b);

      LteSimdBStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    z[i] = x[i] - y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecProdSssSimd(const int16_t *x,
                       const int16_t *y,
                       int16_t *z,
                       const int len)
  {
  int i = 0;
#if LTE_SIMD_S_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoad(&x[i]);
      SimdS_t b = LteSimdSLoad(&y[i]);

      SimdS_t r = LteSimdSMul(a, b);

      LteSimdSStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoadu(&x[i]);
      SimdS_t b = LteSimdSLoadu(&y[i]);

      SimdS_t r = LteSimdSMul(a, b);

      LteSimdSStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    z[i] = x[i] * y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecNegSssSimd(const int16_t *x,
                      const int16_t *y,
                      int16_t *z,
                      const int len)
  {
  int i = 0;

#ifndef HAVE_NEON
#if LTE_SIMD_S_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoad(&x[i]);
      SimdS_t b = LteSimdSLoad(&y[i]);

      SimdS_t r = LteSimdSNeg(a, b);

      LteSimdSStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdS_t a = LteSimdSLoadu(&x[i]);
      SimdS_t b = LteSimdSLoadu(&y[i]);

      SimdS_t r = LteSimdSNeg(a, b);

      LteSimdSStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */
#endif /* NOT HAVE_NEON*/

  for(; i < len; i++)
    {
    z[i] = y[i]<0?-x[i]:x[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecNegBbbSimd(const int8_t *x,
                      const int8_t *y,
                      int8_t *z,
                      const int len)
  {
  int i = 0;

#ifndef HAVE_NEON
#if LTE_SIMD_B_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_B_SIZE + 1; i += LTE_SIMD_B_SIZE)
      {
      SimdS_t a = LteSimdBLoad(&x[i]);
      SimdS_t b = LteSimdBLoad(&y[i]);

      SimdS_t r = LteSimdBNeg(a, b);

      LteSimdBStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_B_SIZE + 1; i += LTE_SIMD_B_SIZE)
      {
      SimdS_t a = LteSimdBLoadu(&x[i]);
      SimdS_t b = LteSimdBLoadu(&y[i]);

      SimdS_t r = LteSimdBNeg(a, b);

      LteSimdBStoreu(&z[i], r);
      }
    }
#endif /* LTE_SIMD_S_SIZE */
#endif /* NOT HAVE_NEON*/
  for(; i < len; i++)
    {
    z[i] = y[i] < 0 ? - x[i] : x[i];
    }
  }

//------------------------------------------------------------------------------
/* No improvement with AVX */
void LteVecLutSssSimd(const short *x,
                      const unsigned short *lut,
                      short *y,
                      const int len)
  {
  int i = 0;
#ifdef LV_HAVE_SSE
#ifndef DEBUG_MODE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(lut))
    {
    for (; i < len - 7; i += 8)
      {
      __m128i xVal = _mm_load_si128((__m128i *) &x[i]);
      __m128i lutVal = _mm_load_si128((__m128i *) &lut[i]);

      for (int k = 0; k < 8; k++)
        {
        int16_t x = (int16_t) _mm_extract_epi16(xVal, k);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal, k);
        y[l] = (short) x;
        }
      }
    }
  else
    {
    for (; i < len - 7; i += 8)
      {
      __m128i xVal = _mm_loadu_si128((__m128i *) &x[i]);
      __m128i lutVal = _mm_loadu_si128((__m128i *) &lut[i]);

      for (int k = 0; k < 8; k++)
        {
        int16_t x = (int16_t) _mm_extract_epi16(xVal, k);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal, k);
        y[l] = (short) x;
        }
      }
    }
#endif
#endif

  for (; i < len; i++)
    {
    y[lut[i]] = x[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecLutBbbSimd(const int8_t *x,
                      const unsigned short *lut,
                      int8_t *y,
                      const int len)
  {
  int i = 0;
#ifdef LV_HAVE_SSE
#ifndef DEBUG_MODE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(lut))
    {
    for (; i < len - 15; i += 16)
      {
      __m128i xVal = _mm_load_si128((__m128i *) &x[i]);
      __m128i lutVal1 = _mm_load_si128((__m128i *) &lut[i]);
      __m128i lutVal2 = _mm_load_si128((__m128i *) &lut[i+8]);

      for (int k = 0; k < 8; k++)
        {
        int8_t x = (int8_t) _mm_extract_epi8(xVal, k);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal1, k);
        y[l] = (char) x;
        }
      for (int k = 0; k < 8; k++)
        {
        int8_t x = (int8_t) _mm_extract_epi8(xVal, k+8);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal2, k);
        y[l] = (char) x;
        }
      }
    }
  else
    {
    for (; i < len - 15; i += 16)
      {
      __m128i xVal = _mm_loadu_si128((__m128i *) &x[i]);
      __m128i lutVal1 = _mm_loadu_si128((__m128i *) &lut[i]);
      __m128i lutVal2 = _mm_loadu_si128((__m128i *) &lut[i+8]);

      for (int k = 0; k < 8; k++)
        {
        int8_t x = (int8_t) _mm_extract_epi8(xVal, k);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal1, k);
        y[l] = (char) x;
        }
      for (int k = 0; k < 8; k++)
        {
        int8_t x = (int8_t) _mm_extract_epi8(xVal, k+8);
        uint16_t l = (uint16_t) _mm_extract_epi16(lutVal2, k);
        y[l] = (char) x;
        }
      }
    }
#endif
#endif

  for (; i < len; i++)
    {
    y[lut[i]] = x[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecConvertIfSimd(const int16_t *x,
                         float *z,
                         const float scale,
                         const int len)
  {
  int i = 0;
  const float gain = 1.0f / scale;

#ifdef LV_HAVE_SSE
  __m128 s = _mm_set1_ps(gain);
  if (LTE_IS_ALIGNED(z))
    {
    for (; i < len - 3; i += 4)
      {
      __m64 *ptr = (__m64 *) &x[i];
      __m128 fl = _mm_cvtpi16_ps(*ptr);
      __m128 v = _mm_mul_ps(fl, s);

      _mm_store_ps(&z[i], v);
      }
    }
  else
    {
    for (; i < len - 3; i += 4)
      {
      __m64 *ptr = (__m64 *) &x[i];
      __m128 fl = _mm_cvtpi16_ps(*ptr);
      __m128 v = _mm_mul_ps(fl, s);

      _mm_storeu_ps(&z[i], v);
      }
    }
#endif /* LV_HAVE_SSE */

  for (; i < len; i++)
    {
    z[i] = ((float) x[i]) * gain;
    }
  }

//------------------------------------------------------------------------------
void LteVecConvertFiSimd(const float *x,
                         int16_t *z,
                         const float scale,
                         const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE && LTE_SIMD_S_SIZE
  SimdF_t s = LteSimdFSet1(scale);
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&x[i + LTE_SIMD_F_SIZE]);

      SimdF_t sa = LteSimdFMul(a, s);
      SimdF_t sb = LteSimdFMul(b, s);

      SimdS_t i16 = LteSimdConvert2fS(sa, sb);

      LteSimdSStore(&z[i], i16);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_S_SIZE + 1; i += LTE_SIMD_S_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&x[i + LTE_SIMD_F_SIZE]);

      SimdF_t sa = LteSimdFMul(a, s);
      SimdF_t sb = LteSimdFMul(b, s);

      SimdS_t i16 = LteSimdConvert2fS(sa, sb);

      LteSimdSStoreu(&z[i], i16);
      }
    }
#endif /* LTE_SIMD_F_SIZE && LTE_SIMD_S_SIZE */

  for(; i < len; i++)
    {
    z[i] = (int16_t) (x[i] * scale);
    }
  }

#define LTE_IS_ALIGNED_SSE(PTR) (((size_t)(PTR) & 0x0F) == 0)

//------------------------------------------------------------------------------
void LteVecConvertFbSimd(const float *x,
                         int8_t *z,
                         const float scale,
                         const int len)
  {
  int i = 0;

  // Force the use of SSE here instead of AVX since the implementations requires too many permutes across 128-bit boundaries

#ifdef LV_HAVE_SSE
  __m128 s = _mm_set1_ps(scale);
  if (LTE_IS_ALIGNED_SSE(x) && LTE_IS_ALIGNED_SSE(z))
    {
    for (; i < len - 16 + 1; i += 16)
      {
      __m128 a = _mm_load_ps(&x[i]);
      __m128 b = _mm_load_ps(&x[i + 1*4]);
      __m128 c = _mm_load_ps(&x[i + 2*4]);
      __m128 d = _mm_load_ps(&x[i + 3*4]);

      __m128  sa = _mm_mul_ps(a, s);
      __m128  sb = _mm_mul_ps(b, s);
      __m128  sc = _mm_mul_ps(c, s);
      __m128  sd = _mm_mul_ps(d, s);

      __m128i ai = _mm_cvttps_epi32(sa);
      __m128i bi = _mm_cvttps_epi32(sb);
      __m128i ci = _mm_cvttps_epi32(sc);
      __m128i di = _mm_cvttps_epi32(sd);
      __m128i ab = _mm_packs_epi32(ai, bi);
      __m128i cd = _mm_packs_epi32(ci, di);

      __m128i i8 =_mm_packs_epi16(ab, cd);

      _mm_store_si128((__m128i*)&z[i], i8);
      }
    }
  else
    {
    for (; i < len - 16 + 1; i += 16)
      {
      __m128 a = _mm_load_ps(&x[i]);
      __m128 b = _mm_load_ps(&x[i + 1*4]);
      __m128 c = _mm_load_ps(&x[i + 2*4]);
      __m128 d = _mm_load_ps(&x[i + 3*4]);

      __m128  sa = _mm_mul_ps(a, s);
      __m128  sb = _mm_mul_ps(b, s);
      __m128  sc = _mm_mul_ps(c, s);
      __m128  sd = _mm_mul_ps(d, s);

      __m128i ai = _mm_cvttps_epi32(sa);
      __m128i bi = _mm_cvttps_epi32(sb);
      __m128i ci = _mm_cvttps_epi32(sc);
      __m128i di = _mm_cvttps_epi32(sd);
      __m128i ab = _mm_packs_epi32(ai, bi);
      __m128i cd = _mm_packs_epi32(ci, di);

      __m128i i8 =_mm_packs_epi16(ab, cd);

      _mm_storeu_si128((__m128i*)&z[i], i8);
      }
    }
#endif

#ifdef HAVE_NEON
#warning LteVecConvertFbSimd not implemented in neon
#endif /* HAVE_NEON */

  for(; i < len; i++)
    {
    z[i] = (int8_t) (x[i] * scale);
    }
  }

//------------------------------------------------------------------------------
float LteVecAccFfSimd(const float *x, const int len)
  {
  int i = 0;
  float acc_sum = 0.0f;

#if LTE_SIMD_F_SIZE
  SimdF_t simd_sum = LteSimdFZero();

  if (LTE_IS_ALIGNED(x))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);

      simd_sum = LteSimdFAdd(simd_sum, a);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);

      simd_sum = LteSimdFAdd(simd_sum, a);
      }
    }

  __attribute__((aligned(LTE_SIMD_F_SIZE*4))) float sum[LTE_SIMD_F_SIZE];
  LteSimdFStore(sum, simd_sum);
  for (int k = 0; k < LTE_SIMD_F_SIZE; k++)
    {
    acc_sum += sum[k];
    }
#endif

  for (; i<len; i++)
    {
    acc_sum += x[i];
    }

  return acc_sum;
  }

//------------------------------------------------------------------------------
Cf_t LteVecAccCcSimd(const Cf_t *x, const int len)
  {
  int i = 0;
  Cf_t acc_sum = 0.0f;

#if LTE_SIMD_F_SIZE
  SimdF_t simd_sum = LteSimdFZero();

  if (LTE_IS_ALIGNED(x))
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t a = LteSimdFLoad((float *) &x[i]);

      simd_sum = LteSimdFAdd(simd_sum, a);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t a = LteSimdFLoadu((float *) &x[i]);

      simd_sum = LteSimdFAdd(simd_sum, a);
      }
    }

  __attribute__((aligned(64))) Cf_t sum[LTE_SIMD_F_SIZE/2];
  LteSimdFStore((float*)&sum, simd_sum);
  for (int k = 0; k < LTE_SIMD_F_SIZE/2; k++)
    {
    acc_sum += sum[k];
    }
#endif

  for (; i<len; i++)
    {
    acc_sum += x[i];
    }
  return acc_sum;
  }

//------------------------------------------------------------------------------
void LteVecAddFffSimd(const float *x, const float *y, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&y[i]);

      SimdF_t r = LteSimdFAdd(a, b);

      LteSimdFStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&y[i]);

      SimdF_t r = LteSimdFAdd(a, b);

      LteSimdFStoreu(&z[i], r);
      }
    }
#endif

  for (; i<len; i++)
    {
    z[i] = x[i] + y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecSubFffSimd(const float *x, const float *y, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&y[i]);

      SimdF_t r = LteSimdFSub(a, b);

      LteSimdFStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&y[i]);

      SimdF_t r = LteSimdFSub(a, b);

      LteSimdFStoreu(&z[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] - y[i];
    }
  }

//------------------------------------------------------------------------------
Cf_t LteVecDotProdCccSimd(const Cf_t *x, const Cf_t *y, const int len)
  {
  int i = 0;
  Cf_t result = 0;

#if LTE_SIMD_CF_SIZE
  __attribute__((aligned(64))) Cf_t simd_dotProdVector[LTE_SIMD_CF_SIZE];

  SimdCf_t avx_result = LteSimdCfZero();
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t xVal = LteSimdCfiLoad(&x[i]);
      SimdCf_t yVal = LteSimdCfiLoad(&y[i]);

      avx_result = LteSimdCfAdd(LteSimdCfProd(xVal, yVal), avx_result);
      LteSimdCfiStore(simd_dotProdVector, avx_result);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t xVal = LteSimdCfiLoadu(&x[i]);
      SimdCf_t yVal = LteSimdCfiLoadu(&y[i]);

      avx_result = LteSimdCfAdd(LteSimdCfProd(xVal, yVal), avx_result);
      LteSimdCfiStoreu(simd_dotProdVector, avx_result);
      }
    }

  LteSimdCfiStore(simd_dotProdVector, avx_result);
  for (int k = 0; k < LTE_SIMD_CF_SIZE; k++)
    {
    result += simd_dotProdVector[k];
    }
#endif

  for (; i < len; i++)
    {
    result += (x[i] * y[i]);
    }

  return result;
  }

//------------------------------------------------------------------------------
#ifdef ENABLE_C16
c16_t LteVecDotProdCccC16iSimd(const c16_t *x, const c16_t *y, const int len)
  {
  int i = 0;
  c16_t result = 0;

#if LTE_SIMD_C16_SIZE
  SimdC16_t avx_result = LteSimdC16Zero();

  for (; i < len - LTE_SIMD_C16_SIZE + 1; i += LTE_SIMD_C16_SIZE)
    {
    SimdC16_t xVal = LteSimdC16iLoad(&x[i]);
    SimdC16_t yVal = LteSimdC16iLoad(&y[i]);

    avx_result = LteSimdC16Add(LteSimdC16Prod(xVal, yVal), avx_result);
    }

  __attribute__((aligned(256))) c16_t avx_dotProdVector[16] = {0};
  LteSimdC16iStore(avx_dotProdVector, avx_result);
  for (int k = 0; k < 16; k++)
    {
    result += avx_dotProdVector[k];
    }
#endif

  for(; i < len; i++)
    {
    result += (x[i] * y[i]) / (1 << 14);
    }

  return result;
  }
#endif /* ENABLE_C16 */

//------------------------------------------------------------------------------
Cf_t LteVecDotProdConjCccSimd(const Cf_t *x, const Cf_t *y, const int len)
  {
  int i = 0;
  Cf_t result = 0;

#if LTE_SIMD_CF_SIZE
  __attribute__((aligned(256))) Cf_t simd_dotProdVector[LTE_SIMD_CF_SIZE];

  SimdCf_t simd_result = LteSimdCfZero();
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t xVal = LteSimdCfiLoad(&x[i]);
      SimdCf_t yVal = LteSimdCfiLoad(&y[i]);

      simd_result = LteSimdCfAdd(LteSimdCfConjprod(xVal, yVal), simd_result);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t xVal = LteSimdCfiLoadu(&x[i]);
      SimdCf_t yVal = LteSimdCfiLoadu(&y[i]);

      simd_result = LteSimdCfAdd(LteSimdCfConjprod(xVal, yVal), simd_result);
      }
    }

  LteSimdCfiStore(simd_dotProdVector, simd_result);
  for (int k = 0; k < LTE_SIMD_CF_SIZE; k++)
    {
    result += simd_dotProdVector[k];
    }
#endif

  for (; i < len; i++)
    {
    result += x[i] * conjf(y[i]);
    }

  return result;
  }

//------------------------------------------------------------------------------
void LteVecProdCfcSimd(const Cf_t *x, const float *y, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_CF_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdF_t s = LteSimdFLoad(&y[i]);

      SimdCf_t a = LteSimdCfiLoad(&x[i]);
      SimdCf_t r = LteSimdCfMul(a, s);
      LteSimdCfiStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t s = LteSimdFLoadu(&y[i]);

      SimdCf_t a = LteSimdCfiLoadu(&x[i]);
      SimdCf_t r = LteSimdCfMul(a, s);
      LteSimdCfiStoreu(&z[i], r);
      }
    }
#endif

  for (; i<len; i++)
    {
    z[i] = x[i] * y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecProdFffSimd(const float *x, const float *y, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&y[i]);

      SimdF_t r = LteSimdFMul(a, b);

      LteSimdFStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&y[i]);

      SimdF_t r = LteSimdFMul(a, b);

      LteSimdFStoreu(&z[i], r);
      }
    }
#endif  // LTE_SIMD_F_SIZE

  for (; i<len; i++)
    {
    z[i] = x[i] * y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecProdCccSimd(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_CF_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoad(&x[i]);
      SimdCf_t b = LteSimdCfiLoad(&y[i]);

      SimdCf_t r = LteSimdCfProd(a, b);

      LteSimdCfiStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoadu(&x[i]);
      SimdCf_t b = LteSimdCfiLoadu(&y[i]);

      SimdCf_t r = LteSimdCfProd(a, b);

      LteSimdCfiStoreu(&z[i], r);
      }
    }
#endif  // LTE_SIMD_CF_SIZE

  for (; i<len; i++)
    {
    z[i] = x[i] * y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecProdConjCccSimd(const Cf_t *x,
                           const Cf_t *y,
                           Cf_t *z,
                           const int len)
  {
  int i = 0;

#if LTE_SIMD_CF_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoad(&x[i]);
      SimdCf_t b = LteSimdCfiLoad(&y[i]);
      SimdCf_t r = LteSimdCfConjprod(a, b);
      LteSimdCfiStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoadu(&x[i]);
      SimdCf_t b = LteSimdCfiLoadu(&y[i]);
      SimdCf_t r = LteSimdCfConjprod(a, b);
      LteSimdCfiStoreu(&z[i], r);
      }
    }
#endif  // SRSLTE_SIMD_CF_SIZE

  for (; i < len; i++)
    {
    z[i] = x[i] * conjf(y[i]);
    }
  }

//------------------------------------------------------------------------------
void LteVecProdCccSplitSimd(const float *a_re,
                            const float *a_im,
                            const float *b_re,
                            const float *b_im,
                            float *r_re,
                            float *r_im,
                            const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(a_re) &&
      LTE_IS_ALIGNED(a_im) &&
      LTE_IS_ALIGNED(b_re) &&
      LTE_IS_ALIGNED(b_im) &&
      LTE_IS_ALIGNED(r_re) &&
      LTE_IS_ALIGNED(r_im))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfLoad(&a_re[i], &a_im[i]);
      SimdCf_t b = LteSimdCfLoad(&b_re[i], &b_im[i]);

      SimdCf_t r = LteSimdCfProd(a, b);

      LteSimdCfStore(&r_re[i], &r_im[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfLoadu(&a_re[i], &a_im[i]);
      SimdCf_t b = LteSimdCfLoadu(&b_re[i], &b_im[i]);

      SimdCf_t r = LteSimdCfProd(a, b);

      LteSimdCfStoreu(&r_re[i], &r_im[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    r_re[i] = a_re[i]*b_re[i] - a_im[i]*b_im[i];
    r_im[i] = a_re[i]*b_im[i] + a_im[i]*b_re[i];
    }
  }

//------------------------------------------------------------------------------
#ifdef ENABLE_C16
void LteVecProdCccC16Simd(const int16_t *a_re,
                          const int16_t *a_im,
                          const int16_t *b_re,
                          const int16_t *b_im,
                          int16_t *r_re,
                          int16_t *r_im,
                          const int len)
  {
  int i = 0;

#if LTE_SIMD_C16_SIZE
  if (LTE_IS_ALIGNED(a_re) &&
      LTE_IS_ALIGNED(a_im) &&
      LTE_IS_ALIGNED(b_re) &&
      LTE_IS_ALIGNED(b_im) &&
      LTE_IS_ALIGNED(r_re) &&
      LTE_IS_ALIGNED(r_im))
    {
    for (; i < len - LTE_SIMD_C16_SIZE + 1; i += LTE_SIMD_C16_SIZE)
      {
      SimdC16_t a = LteSimdC16Load(&a_re[i], &a_im[i]);
      SimdC16_t b = LteSimdC16Load(&b_re[i], &b_im[i]);

      SimdC16_t r = LteSimdC16Prod(a, b);

      LteSimdC16Store(&r_re[i], &r_im[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_C16_SIZE + 1; i += LTE_SIMD_C16_SIZE)
      {
      SimdC16_t a = LteSimdC16Loadu(&a_re[i], &a_im[i]);
      SimdC16_t b = LteSimdC16Loadu(&b_re[i], &b_im[i]);

      SimdC16_t r = LteSimdC16Prod(a, b);

      LteSimdC16Storeu(&r_re[i], &r_im[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    r_re[i] = a_re[i] * b_re[i] - a_im[i] * b_im[i];
    r_im[i] = a_re[i] * b_im[i] + a_im[i] * b_re[i];
    }
  }
#endif /* ENABLE_C16 */

//------------------------------------------------------------------------------
void LteVecDivCccSimd(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_CF_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoad(&x[i]);
      SimdCf_t b = LteSimdCfiLoad(&y[i]);

      SimdCf_t rcpb = LteSimdCfRcp(b);
      SimdCf_t r = LteSimdCfProd(a, rcpb);

      LteSimdCfiStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoadu(&x[i]);
      SimdCf_t b = LteSimdCfiLoadu(&y[i]);

      SimdCf_t rcpb = LteSimdCfRcp(b);
      SimdCf_t r = LteSimdCfProd(a, rcpb);

      LteSimdCfiStoreu(&z[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] / y[i];
    }
  }


//------------------------------------------------------------------------------
void LteVecDivCfcSimd(const Cf_t *x, const float *y, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_CF_SIZE && LTE_SIMD_CF_SIZE == LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&y[i]);

      SimdF_t rcpb = LteSimdFRcp(b);
      SimdCf_t r = LteSimdCfMul(a, rcpb);

      LteSimdCfiStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&y[i]);

      SimdF_t rcpb = LteSimdFRcp(b);
      SimdCf_t r = LteSimdCfMul(a, rcpb);

      LteSimdCfiStoreu(&z[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] / y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecDivFffSimd(const float *x, const float *y, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdF_t b = LteSimdFLoad(&y[i]);

      SimdF_t rcpb = LteSimdFRcp(b);
      SimdF_t r = LteSimdFMul(a, rcpb);

      LteSimdFStore(&z[i], r);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdF_t b = LteSimdFLoadu(&y[i]);

      SimdF_t rcpb = LteSimdFRcp(b);
      SimdF_t r = LteSimdFMul(a, rcpb);

      LteSimdFStoreu(&z[i], r);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] / y[i];
    }
  }

//------------------------------------------------------------------------------
int  LteVecScProdCccSimd2(const Cf_t *x, const Cf_t h, Cf_t *z, const int len)
  {
  unsigned int i = 0;
  const unsigned int loops = len / 4;
#ifdef HAVE_NEON
  SimdCf_t h_vec;
  h_vec.val[0] = LteSimdFSet1(__real__ h);
  h_vec.val[1] = LteSimdFSet1(__imag__ h);
  for (; i < loops; i++)
    {

    SimdCf_t in =  LteSimdCfiLoad(&x[i*4]);
    SimdCf_t temp =  LteSimdCfProd(in, h_vec);
    LteSimdCfiStore(&z[i*4], temp);
    }

#endif
  i = loops * 4;
  return i;
  }

//------------------------------------------------------------------------------
void LteVecScProdCccSimd(const Cf_t *x, const Cf_t h, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE


#ifdef HAVE_NEON
  i = LteVecScProdCccSimd2(x, h, z, len);
#else
  const SimdF_t hre = LteSimdFSet1(__real__ h);
  const SimdF_t him = LteSimdFSet1(__imag__ h);

  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoad((float *) &x[i]);

      SimdF_t m1 = LteSimdFMul(hre, temp);
      SimdF_t sw = LteSimdFSwap(temp);
      SimdF_t m2 = LteSimdFMul(him, sw);
      SimdF_t r = LteSimdFAddsub(m1, m2);
      LteSimdFStore((float *) &z[i], r);

      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoadu((float *) &x[i]);

      SimdF_t m1 = LteSimdFMul(hre, temp);
      SimdF_t sw = LteSimdFSwap(temp);
      SimdF_t m2 = LteSimdFMul(him, sw);
      SimdF_t r = LteSimdFAddsub(m1, m2);

      LteSimdFStoreu((float *) &z[i], r);
      }
    }
#endif
#endif
  for (; i < len; i++)
    {
    z[i] = x[i] * h;
    }

  }

//------------------------------------------------------------------------------
void LteVecScProdFffSimd(const float *x, const float h, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  const SimdF_t hh = LteSimdFSet1(h);
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t xx = LteSimdFLoad(&x[i]);

      SimdF_t zz = LteSimdFMul(xx, hh);

      LteSimdFStore(&z[i], zz);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t xx = LteSimdFLoadu(&x[i]);

      SimdF_t zz = LteSimdFMul(xx, hh);

      LteSimdFStoreu(&z[i], zz);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] * h;
    }
  }

//------------------------------------------------------------------------------
void LteVecAbsCfSimd(const Cf_t *x, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t x1 = LteSimdFLoad((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoad((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);
      z1 = LteSimdFSqrt(z1);
      LteSimdFStore(&z[i], z1);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t x1 = LteSimdFLoadu((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoadu((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);
      z1 = LteSimdFSqrt(z1);

      LteSimdFStoreu(&z[i], z1);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = sqrtf(__real__(x[i]) * __real__(x[i]) + __imag__(x[i]) * __imag__(x[i]));
    }
  }

//------------------------------------------------------------------------------
void LteVecAbsSquareCfSimd(const Cf_t *x, float *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t x1 = LteSimdFLoad((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoad((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);

      LteSimdFStore(&z[i], z1);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      SimdF_t x1 = LteSimdFLoadu((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoadu((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);

      LteSimdFStoreu(&z[i], z1);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = __real__(x[i]) * __real__(x[i]) + __imag__(x[i]) * __imag__(x[i]);
    }
  }


//------------------------------------------------------------------------------
void LteVecScProdCfcSimd(const Cf_t *x, const float h, Cf_t *z, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  const SimdF_t tap = LteSimdFSet1(h);

  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoad((float *) &x[i]);

      temp = LteSimdFMul(tap, temp);

      LteSimdFStore((float *) &z[i], temp);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoadu((float *) &x[i]);

      temp = LteSimdFMul(tap, temp);

      LteSimdFStoreu((float *) &z[i], temp);
      }
    }
#endif

  for (; i < len; i++)
    {
    z[i] = x[i] * h;
    }
  }

//------------------------------------------------------------------------------
void LteVecCpSimd(const Cf_t *src, Cf_t *dst, const int len)
  {
  int i = 0;

#if LTE_SIMD_F_SIZE
  if (LTE_IS_ALIGNED(src) && LTE_IS_ALIGNED(dst))
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoad((float *) &src[i]);

      LteSimdFStore((float *) &dst[i], temp);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE / 2 + 1; i += LTE_SIMD_F_SIZE / 2)
      {
      SimdF_t temp = LteSimdFLoadu((float *) &src[i]);

      LteSimdFStoreu((float *) &dst[i], temp);
      }
    }
#endif

  for (; i < len; i++)
    {
    dst[i] = src[i];
    }
  }

//------------------------------------------------------------------------------
uint32_t LteVecMaxFiSimd(const float *x, const int len)
  {
  int i = 0;

  float max_value = -INFINITY;
  uint32_t max_index = 0;

#if LTE_SIMD_I_SIZE
  __attribute__ ((aligned (LTE_SIMD_I_SIZE*sizeof(int)))) int indexes_buffer[LTE_SIMD_I_SIZE] = {0};
  __attribute__ ((aligned (LTE_SIMD_I_SIZE*sizeof(float)))) float values_buffer[LTE_SIMD_I_SIZE] = {0};

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++) indexes_buffer[k] = k;
  SimdI_t simd_inc = LteSimdISet1(LTE_SIMD_I_SIZE);
  SimdI_t simd_indexes = LteSimdILoad(indexes_buffer);
  SimdI_t simd_max_indexes = LteSimdISet1(0);

  SimdF_t simd_max_values = LteSimdFSet1(-INFINITY);

  if (LTE_IS_ALIGNED(x))
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t a = LteSimdFLoad(&x[i]);
      SimdSel_t res = LteSimdFMax(a, simd_max_values);
      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) a, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t a = LteSimdFLoadu(&x[i]);
      SimdSel_t res = LteSimdFMax(a, simd_max_values);
      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) a, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }

  LteSimdIStore(indexes_buffer, simd_max_indexes);
  LteSimdFStore(values_buffer, simd_max_values);

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++)
    {
    if (values_buffer[k] > max_value)
      {
      max_value = values_buffer[k];
      max_index = (uint32_t) indexes_buffer[k];
      }
    }
#endif /* LTE_SIMD_I_SIZE */

  for (; i < len; i++)
    {
    if (x[i] > max_value)
      {
      max_value = x[i];
      max_index = (uint32_t)i;
      }
    }

  return max_index;
  }

//------------------------------------------------------------------------------
uint32_t LteVecMaxAbsFiSimd(const float *x, const int len)
  {
  int i = 0;

  float max_value = -INFINITY;
  uint32_t max_index = 0;

#if LTE_SIMD_I_SIZE
  __attribute__ ((aligned (LTE_SIMD_I_SIZE * sizeof(int)))) int indexes_buffer[LTE_SIMD_I_SIZE] = {0};
  __attribute__ ((aligned (LTE_SIMD_I_SIZE * sizeof(float)))) float values_buffer[LTE_SIMD_I_SIZE] = {0};

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++) indexes_buffer[k] = k;
  SimdI_t simd_inc = LteSimdISet1(LTE_SIMD_I_SIZE);
  SimdI_t simd_indexes = LteSimdILoad(indexes_buffer);
  SimdI_t simd_max_indexes = LteSimdISet1(0);

  SimdF_t simd_max_values = LteSimdFSet1(-INFINITY);

  if (LTE_IS_ALIGNED(x))
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t a = LteSimdFAbs(LteSimdFLoad(&x[i]));
      SimdSel_t res = LteSimdFMax(a, simd_max_values);
      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) a, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t a = LteSimdFAbs(LteSimdFLoadu(&x[i]));
      SimdSel_t res = LteSimdFMax(a, simd_max_values);
      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) a, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }

  LteSimdIStore(indexes_buffer, simd_max_indexes);
  LteSimdFStore(values_buffer, simd_max_values);

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++)
    {
    if (values_buffer[k] > max_value)
      {
      max_value = values_buffer[k];
      max_index = (uint32_t) indexes_buffer[k];
      }
    }
#endif /* LTE_SIMD_I_SIZE */

  for (; i < len; i++)
    {
    float a = fabsf(x[i]);
    if (a > max_value)
      {
      max_value = a;
      max_index = (uint32_t)i;
      }
    }

  return max_index;
  }

//------------------------------------------------------------------------------
uint32_t LteVecMaxCiSimd(const Cf_t *x, const int len)
  {
  int i = 0;

  float max_value = -INFINITY;
  uint32_t max_index = 0;

#if LTE_SIMD_I_SIZE
  __attribute__ ((aligned (LTE_SIMD_I_SIZE*sizeof(int)))) int indexes_buffer[LTE_SIMD_I_SIZE] = {0};
  __attribute__ ((aligned (LTE_SIMD_I_SIZE*sizeof(float)))) float values_buffer[LTE_SIMD_I_SIZE] = {0};

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++) indexes_buffer[k] = k;
  SimdI_t simd_inc = LteSimdISet1(LTE_SIMD_I_SIZE);
  SimdI_t simd_indexes = LteSimdILoad(indexes_buffer);
  SimdI_t simd_max_indexes = LteSimdISet1(0);

  SimdF_t simd_max_values = LteSimdFSet1(-INFINITY);

  if (LTE_IS_ALIGNED(x))
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t x1 = LteSimdFLoad((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoad((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);

      SimdSel_t res = LteSimdFMax(z1, simd_max_values);

      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) z1, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }
  else
    {
    for (; i < len - LTE_SIMD_I_SIZE + 1; i += LTE_SIMD_I_SIZE)
      {
      SimdF_t x1 = LteSimdFLoadu((float *) &x[i]);
      SimdF_t x2 = LteSimdFLoadu((float *) &x[i + LTE_SIMD_F_SIZE / 2]);

      SimdF_t mul1 = LteSimdFMul(x1, x1);
      SimdF_t mul2 = LteSimdFMul(x2, x2);

      SimdF_t z1 = LteSimdFHadd(mul1, mul2);

      SimdSel_t res = LteSimdFMax(z1, simd_max_values);

      simd_max_indexes = LteSimdISelect(simd_max_indexes, simd_indexes, res);
      simd_max_values = (SimdF_t) LteSimdISelect((SimdI_t) simd_max_values, (SimdI_t) z1, res);
      simd_indexes = LteSimdIAdd(simd_indexes, simd_inc);
      }
    }

  LteSimdIStore(indexes_buffer, simd_max_indexes);
  LteSimdFStore(values_buffer, simd_max_values);

  for (int k = 0; k < LTE_SIMD_I_SIZE; k++)
    {
    if (values_buffer[k] > max_value)
      {
      max_value = values_buffer[k];
      max_index = (uint32_t) indexes_buffer[k];
      }
    }
#endif /* LTE_SIMD_I_SIZE */

  for (; i < len; i++)
    {
    Cf_t a = x[i];
    float abs2 = __real__ a * __real__ a + __imag__ a * __imag__ a;
    if (abs2 > max_value)
      {
      max_value = abs2;
      max_index = (uint32_t)i;
      }
    }

  return max_index;
  }

//------------------------------------------------------------------------------
void LteVecInterleaveSimd(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len)
  {
  int i = 0, k = 0;

#ifdef LV_HAVE_SSE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - 2 + 1; i += 2)
      {
      __m128i a = _mm_load_si128((__m128i *) &x[i]);
      __m128i b = _mm_load_si128((__m128i *) &y[i]);

      __m128i r1 = _mm_unpacklo_epi64(a, b);
      _mm_store_si128((__m128i *) &z[k], r1);
      k += 2;

      __m128i r2 = _mm_unpackhi_epi64(a, b);
      _mm_store_si128((__m128i *) &z[k], r2);
      k += 2;
      }
    }
  else
    {
    for (; i < len - 2 + 1; i += 2)
      {
      __m128i a = _mm_loadu_si128((__m128i *) &x[i]);
      __m128i b = _mm_loadu_si128((__m128i *) &y[i]);

      __m128i r1 = _mm_unpacklo_epi64(a, b);
      _mm_storeu_si128((__m128i *) &z[k], r1);
      k += 2;

      __m128i r2 = _mm_unpackhi_epi64(a, b);
      _mm_storeu_si128((__m128i *) &z[k], r2);
      k += 2;
      }
    }
#endif /* LV_HAVE_SSE */

  for (; i < len; i++)
    {
    z[k++] = x[i];
    z[k++] = y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecInterleaveAddSimd(const Cf_t *x,
                             const Cf_t *y,
                             Cf_t *z,
                             const int len)
  {
  int i = 0, k = 0;

#ifdef LV_HAVE_SSE
  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(y) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - 2 + 1; i += 2)
      {
      __m128i a = _mm_load_si128((__m128i *) &x[i]);
      __m128i b = _mm_load_si128((__m128i *) &y[i]);

      __m128 r1 = (__m128) _mm_unpacklo_epi64(a, b);
      __m128 z1 = _mm_load_ps((float *) &z[k]);
      r1 = _mm_add_ps((__m128) r1, z1);
      _mm_store_ps((float *) &z[k], r1);
      k += 2;

      __m128 r2 = (__m128) _mm_unpackhi_epi64(a, b);
      __m128 z2 = _mm_load_ps((float *) &z[k]);
      r2 = _mm_add_ps((__m128) r2, z2);
      _mm_store_ps((float *) &z[k], r2);
      k += 2;
      }
    }
  else
    {
    for (; i < len - 2 + 1; i += 2)
      {
      __m128i a = _mm_loadu_si128((__m128i *) &x[i]);
      __m128i b = _mm_loadu_si128((__m128i *) &y[i]);

      __m128 r1 = (__m128) _mm_unpacklo_epi64(a, b);
      __m128 z1 = _mm_loadu_ps((float *) &z[k]);
      r1 = _mm_add_ps((__m128) r1, z1);
      _mm_storeu_ps((float *) &z[k], r1);
      k += 2;

      __m128 r2 = (__m128) _mm_unpackhi_epi64(a, b);
      __m128 z2 = _mm_loadu_ps((float *) &z[k]);
      r2 = _mm_add_ps((__m128) r2, z2);
      _mm_storeu_ps((float *) &z[k], r2);
      k += 2;
      }
    }
#endif /* LV_HAVE_SSE */

  for (; i < len; i++)
    {
    z[k++] += x[i];
    z[k++] += y[i];
    }
  }

//------------------------------------------------------------------------------
void LteVecApplyCfoSimd(const Cf_t *x, float cfo, Cf_t *z, int len)
  {
  const float TWOPI = 2.0f * (float) M_PI;
  int i = 0;

#if LTE_SIMD_CF_SIZE
  __attribute__ ((aligned (LTE_SIMD_BIT_ALIGN / 8))) Cf_t _osc[LTE_SIMD_CF_SIZE];
  __attribute__ ((aligned (LTE_SIMD_BIT_ALIGN / 8))) Cf_t _phase[LTE_SIMD_CF_SIZE];

  if (i < len - LTE_SIMD_CF_SIZE + 1)
    {
    for (int k = 0; k < LTE_SIMD_CF_SIZE; k++)
      {
      _osc[k] = cexpf(_Complex_I * TWOPI * cfo * LTE_SIMD_CF_SIZE);
      _phase[k] = cexpf(_Complex_I * TWOPI * cfo * k);
      }
    }
  SimdCf_t _simd_osc = LteSimdCfiLoad(_osc);
  SimdCf_t _simd_phase = LteSimdCfiLoad(_phase);

  if (LTE_IS_ALIGNED(x) && LTE_IS_ALIGNED(z))
    {
    for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
      {
      SimdCf_t a = LteSimdCfiLoad(&x[i]);

      SimdCf_t r = LteSimdCfProd(a, _simd_phase);

      LteSimdCfiStore(&z[i], r);

      _simd_phase = LteSimdCfProd(_simd_phase, _simd_osc);

      }
    }
  else
    {
    for (; i < len - LTE_SIMD_F_SIZE + 1; i += LTE_SIMD_F_SIZE)
      {
      for (; i < len - LTE_SIMD_CF_SIZE + 1; i += LTE_SIMD_CF_SIZE)
        {
        SimdCf_t a = LteSimdCfiLoadu(&x[i]);

        SimdCf_t r = LteSimdCfProd(a, _simd_phase);
        _simd_phase = LteSimdCfProd(_simd_phase, _simd_osc);

        LteSimdCfiStoreu(&z[i], r);
        }
      }
    }
#endif
  Cf_t osc = cexpf(_Complex_I * TWOPI * cfo);
  Cf_t phase = cexpf(_Complex_I * TWOPI * cfo * i);
  for (; i < len; i++)
    {
    z[i] = x[i] * phase;

    phase *= osc;
    }
  }

