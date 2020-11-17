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

#include <float.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "phy/vector/vector.hpp"
#include "phy/vector/vector_simd.hpp"
#include "phy/utils/bit.hpp"



//------------------------------------------------------------------------------
void LteVecXorBbb(int8_t *x,int8_t *y,int8_t *z, const uint32_t len)
  {
  LteVecXorBbbSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// Used in PRACH detector, AGC and chest_dl for noise averaging
float LteVecAccFf(const float *x, const uint32_t len)
  {
  return LteVecAccFfSimd(x, len);
  }

//------------------------------------------------------------------------------
Cf_t LteVecAccCc(const Cf_t *x, const uint32_t len)
  {
  return LteVecAccCcSimd(x, len);
  }

//------------------------------------------------------------------------------
void LteVecSubFff(const float *x, const float *y, float *z, const uint32_t len) 
  {
  LteVecSubFffSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecSubSss(const int16_t *x,
                  const int16_t *y,
                  int16_t *z, 
                  const uint32_t len)
  {
  LteVecSubSssSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecSubBbb(const int8_t *x,
                  const int8_t *y,
                  int8_t *z,
                  const uint32_t len)
  {
  LteVecSubBbbSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// Noise estimation in chest_dl, interpolation
void LteVecSubCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len)
  {
  return LteVecSubFff((const float*) x,(const float*) y,(float*) z, 2 * len);
  }

//------------------------------------------------------------------------------
// Used in PSS/SSS and sum_ccc
void LteVecSumFff(const float *x, const float *y, float *z, const uint32_t len) 
  {
  LteVecAddFffSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecSumSss(const int16_t *x,
                  const int16_t *y,
                  int16_t *z,
                  const uint32_t len)
  {
  LteVecSumSssSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecSumCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len)
  {
  LteVecSumFff((float*) x,(float*) y,(float*) z, 2 * len);
  }

//------------------------------------------------------------------------------
// PSS, PBCH, DEMOD, FFTW, etc.
void LteVecScProdFff(const float *x,
                     const float h,
                     float *z,
                     const uint32_t len)
  {
  LteVecScProdFffSimd(x, h, z, len);
  }

//------------------------------------------------------------------------------
// Used throughout 
void LteVecScProdCfc(const Cf_t *x, const float h, Cf_t *z, const uint32_t len) 
  {
  LteVecScProdCfcSimd(x, h, z, len);
  }

//------------------------------------------------------------------------------
// Chest UL 
void LteVecScProdCcc(const Cf_t *x, const Cf_t h, Cf_t *z, const uint32_t len)
  {
  LteVecScProdCccSimd(x, h, z, len);
  }

//------------------------------------------------------------------------------
// Used in turbo decoder 
void LteVecConvertIf(const int16_t *x,
                     const float scale,
                     float *z,
                     const uint32_t len)
  {
  LteVecConvertIfSimd(x, z, scale, len);
  }

//------------------------------------------------------------------------------
void LteVecConvertFi(const float *x,
                     const float scale,
                     int16_t *z,
                     const uint32_t len)
  {
  LteVecConvertFiSimd(x, z, scale, len);
  }

//------------------------------------------------------------------------------
void LteVecConvertFb(const float *x,
                     const float scale,
                     int8_t *z, 
                     const uint32_t len)
  {
  LteVecConvertFbSimd(x, z, scale, len);
  }

//------------------------------------------------------------------------------
void LteVecLutSss(const short *x,
                  const unsigned short *lut,
                  short *y,
                  const uint32_t len)
  {
  LteVecLutSssSimd(x, lut, y, len);
  }

//------------------------------------------------------------------------------
void LteVecLutBbb(const int8_t *x,
                  const unsigned short *lut,
                  int8_t *y,
                  const uint32_t len)
  {
  LteVecLutBbbSimd(x, lut, y, len);
  }

//------------------------------------------------------------------------------
void LteVecLutSis(const short *x,
                  const unsigned int *lut, 
                  short *y,
                  const uint32_t len)
  {
  for (uint32_t i = 0; i < len; i++)
    {
    y[lut[i]] = x[i];
    }
  }

//------------------------------------------------------------------------------
void *LteVecMalloc(uint32_t size)
  {
  void *ptr;
  if (posix_memalign(&ptr, LTE_SIMD_BIT_ALIGN, size))
    {
    return NULL;
    } 
  else
    {
    return ptr;
    }
  }

//------------------------------------------------------------------------------
void *LteVecRealloc(void *ptr, uint32_t old_size, uint32_t new_size)
  {
#ifndef LV_HAVE_SSE
  return realloc(ptr, new_size);
#else
  void *new_ptr;
  if (posix_memalign(&new_ptr, LTE_SIMD_BIT_ALIGN, new_size))
    {
    return NULL;
    } 
  else
    {
    memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return new_ptr;
    }
#endif
  }

//------------------------------------------------------------------------------
void LteVecFprintC(FILE *stream, Cf_t *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%+2.5f%+2.5fi, ", __real__ x[i], __imag__ x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintF(FILE *stream, float *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%+2.2f, ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintB(FILE *stream, uint8_t *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%d, ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintBs(FILE *stream, int8_t *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%4d, ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintByte(FILE *stream, uint8_t *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%02x ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintI(FILE *stream, int *x, const uint32_t len)
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%d, ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintS(FILE *stream, short *x, const uint32_t len) 
  {
  uint32_t i;
  fprintf(stream, "[");
  for (i = 0; i < len; i++)
    {
    fprintf(stream, "%4d, ", x[i]);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecFprintHex(FILE *stream, uint8_t *x, const uint32_t len)
  {
  uint32_t i, nbytes; 
  uint8_t byte;
  nbytes = len / 8;
  fprintf(stream, "[");
  for (i = 0; i < nbytes; i++)
    {
    byte = (uint8_t) LteBitPack(&x, 8);
    fprintf(stream, "%02x ", byte);
    }
  if (len % 8)
    {
    byte = (uint8_t) LteBitPack(&x, len % 8) << (8 - (len % 8));
    fprintf(stream, "%02x ", byte);
    }
  fprintf(stream, "];\n");
  }

//------------------------------------------------------------------------------
void LteVecSprintHex(char *str,
                     const uint32_t max_str_len,
                     uint8_t *x,
                     const uint32_t len)
  {
  uint32_t i, nbytes; 
  uint8_t byte;
  nbytes = len / 8;
  // check that hex string fits in buffer (every byte takes 3 characters, plus brackets)
  if ((3 * (len / 8 + ((len % 8) ? 1 : 0))) + 2 >= max_str_len)
    {
    fprintf(stderr, "Buffer too small for printing hex string (max_str_len=%d, payload_len=%d).\n", max_str_len, len);
    return;
    }

  int n = 0;
  n += sprintf(&str[n], "[");
  for (i = 0; i < nbytes; i++) 
    {
    byte = (uint8_t) LteBitPack(&x, 8);
    n+=sprintf(&str[n], "%02x ", byte);
    }
  if (len % 8)
    {
    byte = (uint8_t) LteBitPack(&x, len % 8) << (8 - (len % 8));
    n+=sprintf(&str[n], "%02x ", byte);
    }
  n += sprintf(&str[n], "]");
  str[max_str_len - 1] = 0;
  }

//------------------------------------------------------------------------------
void LteVecSaveFile(char *filename, const void *buffer, const uint32_t len)
  {
  FILE *f; 
  f = fopen(filename, "w");
  if (f)
    {
    fwrite(buffer, len, 1, f);
    fclose(f);
    }
  else
    {
    perror("fopen");
    }  
  }

//------------------------------------------------------------------------------
void LteVecLoadFile(char *filename, void *buffer, const uint32_t len)
  {
  FILE *f; 
  f = fopen(filename, "r");
  if (f)
    {
    fread(buffer, len, 1, f);
    fclose(f);
    }
  else
    {
    perror("fopen");
    }  
  }

//------------------------------------------------------------------------------
// Used in PSS
void LteVecConjCc(const Cf_t *x, Cf_t *y, const uint32_t len)
  {
  /* This function is used in initialisation only, then no optimisation is required */
  uint32_t i;
  for (i = 0; i < len; i++)
    {
    y[i] = conjf(x[i]);
    }
  }

//------------------------------------------------------------------------------
// Used in scrambling complex 
void LteVecProdCfc(const Cf_t *x, const float *y, Cf_t *z, const uint32_t len)
  {
  LteVecProdCfcSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// Used in scrambling float
void LteVecProdFff(const float *x,
                   const float *y,
                   float *z,
                   const uint32_t len) 
  {
  LteVecProdFffSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecProdSss(const int16_t *x,
                   const int16_t *y,
                   int16_t *z,
                   const uint32_t len) 
  {
  LteVecProdSssSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// Scrambling
void LteVecNegSss(const int16_t *x,
                  const int16_t *y,
                  int16_t *z,
                  const uint32_t len) 
  {
  LteVecNegSssSimd(x, y, z, len);
  }
//------------------------------------------------------------------------------
void LteVecNegBbb(const int8_t *x,
                  const int8_t *y,
                  int8_t *z,
                  const uint32_t len) 
  {
  LteVecNegBbbSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// CFO and OFDM processing
void LteVecProdCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len)
  {
  LteVecProdCccSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecProdCccSplit(const float *x_re,
                        const float *x_im,
                        const float *y_re,
                        const float *y_im,
                        float *z_re,
                        float *z_im,
                        const uint32_t len)
  {
  LteVecProdCccSplitSimd(x_re, x_im, y_re , y_im, z_re,z_im, len);
  }

//------------------------------------------------------------------------------
// PRACH, CHEST UL, etc. 
void LteVecProdConjCcc(const Cf_t *x,
                       const Cf_t *y,
                       Cf_t *z,
                       const uint32_t len)
  {
  LteVecProdConjCccSimd(x, y, z, len);
  }

//#define DIV_USE_VEC

//------------------------------------------------------------------------------
// Used in SSS 
void LteVecDivCcc(const Cf_t *x, const Cf_t *y, Cf_t *z, const uint32_t len)
  {
  LteVecDivCccSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
/* Complex division by float z=x/y */
void LteVecDivCfc(const Cf_t *x, const float *y, Cf_t *z, const uint32_t len)
  {
  LteVecDivCfcSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecDivFff(const float *x, const float *y, float *z, const uint32_t len) 
  {
  LteVecDivFffSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
// PSS. convolution 
Cf_t LteVecDotProdCcc(const Cf_t *x, const Cf_t *y, const uint32_t len)
  {
  return LteVecDotProdCccSimd(x, y, len);
  }

//------------------------------------------------------------------------------
// Convolution filter and in SSS search 
Cf_t LteVecDotProdCfc(const Cf_t *x, const float *y, const uint32_t len)
  {
  uint32_t i;
  Cf_t res = 0;
  for (i = 0; i < len; i++)
    {
    res += x[i] * y[i];
    }
  return res;
  }

//------------------------------------------------------------------------------
// SYNC 
Cf_t LteVecDotProdConjCcc(const Cf_t *x, const Cf_t *y, const uint32_t len)
  {
  return LteVecDotProdConjCccSimd(x, y, len);
  }

//------------------------------------------------------------------------------
// PHICH 
float LteVecDotProdFff(const float *x, const float *y, const uint32_t len)
  {
  uint32_t i;
  float res = 0;
  for (i = 0; i < len; i++)
    {
    res += x[i] * y[i];
    }
  return res;
  }

//------------------------------------------------------------------------------
int32_t LteVecDotProdSss(const int16_t *x, const int16_t *y, const uint32_t len)
  {
  return LteVecDotProdSssSimd(x, y, len);
  }

//------------------------------------------------------------------------------
float LteVecAvgPowerCf(const Cf_t *x, const uint32_t len) 
  {
  return crealf(LteVecDotProdConjCcc(x,x,len)) / len;
  }

//------------------------------------------------------------------------------
// Correlation assumes zero-mean x and y
float LteVecCorrCcc(const Cf_t *x, Cf_t *y, const uint32_t len)
  {
  float s_x = crealf(LteVecDotProdConjCcc(x, x, len)) / len;
  float s_y = crealf(LteVecDotProdConjCcc(y, y, len)) / len;
  float cov = crealf(LteVecDotProdConjCcc(x, y, len)) / len;
  return cov / (sqrtf(s_x * s_y));
  } 

//------------------------------------------------------------------------------
// PSS (disabled and using abs_square )
void LteVecAbsCf(const Cf_t *x, float *abs, const uint32_t len)
  {
  LteVecAbsCfSimd(x, abs, len);
  }

//------------------------------------------------------------------------------
// PRACH 
void LteVecAbsSquareCf(const Cf_t *x, float *abs_square, const uint32_t len)
  {
  LteVecAbsSquareCfSimd(x, abs_square, len);
  }

//------------------------------------------------------------------------------
uint32_t LteVecMaxFi(const float *x, const uint32_t len)
  {
  return LteVecMaxFiSimd(x, len);
  }

//------------------------------------------------------------------------------
uint32_t LteVecMaxAbsFi(const float *x, const uint32_t len)
  {
  return LteVecMaxAbsFiSimd(x, len);
  }

//------------------------------------------------------------------------------
// CP autocorr
uint32_t LteVecMaxAbsCi(const Cf_t *x, const uint32_t len)
  {
  return LteVecMaxCiSimd(x, len);
  }

//------------------------------------------------------------------------------
void LteVecQuantFus(float *in,
                    uint16_t *out,
                    float gain,
                    float offset,
                    float clip,
                    uint32_t len)
  {
  uint32_t i;
  long tmp;
  
  for (i = 0; i < len; i++)
    {
    tmp = (long) (offset + gain * in[i]);
    if (tmp < 0)
      tmp = 0;
    if (tmp > clip)
      tmp = clip;
    out[i] = (uint16_t) tmp;    
    }
  }

//------------------------------------------------------------------------------
void LteVecQuantFuc(const float *in,
                    uint8_t *out,
                    const float gain,
                    const float offset,
                    const float clip,
                    const uint32_t len)
  {
  uint32_t i;
  int tmp;
  
  for (i = 0; i < len; i++)
    {
    tmp = (int) (offset + gain * in[i]);
    if (tmp < 0)
      tmp = 0;
    if (tmp > clip)
      tmp = clip;
    out[i] = (uint8_t) tmp;    
    }
  }

//------------------------------------------------------------------------------
void LteVecQuantSuc(const int16_t *in,
                    uint8_t *out,
                    const float gain,
                    const int16_t offset,
                    const int16_t clip,
                    const uint32_t len)
  {
  uint32_t i;
  int16_t tmp;
  
  for (i = 0; i < len; i++)
    {
    tmp = (int16_t) (offset + in[i] * gain);
    if (tmp < 0)
      tmp = 0;
    if (tmp > clip)
      tmp = clip;
    out[i] = (uint8_t) tmp;    
    }
  }

//------------------------------------------------------------------------------
void LteVecQuantSus(const int16_t *in,
                    uint16_t *out,
                    const float gain,
                    const int16_t offset,
                    const uint32_t len) 
  {
  uint32_t i;
  int16_t tmp;
  
  for (i = 0; i < len; i++)
    {
    tmp =  (offset + in[i] * gain);
    if (tmp < 0)
      tmp = 0;
    out[i] = (uint16_t) tmp;    
    }
  }

//------------------------------------------------------------------------------
void VecCfCpy(const Cf_t *src, Cf_t *dst, int len)
  {
  LteVecCpSimd(src, dst, len);
  }

//------------------------------------------------------------------------------
void LteVecInterleave(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len)
  {
  LteVecInterleaveSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecInterleaveAdd(const Cf_t *x, const Cf_t *y, Cf_t *z, const int len)
  {
  LteVecInterleaveAddSimd(x, y, z, len);
  }

//------------------------------------------------------------------------------
void LteVecApplyCfo(const Cf_t *x, float cfo, Cf_t *z, int len)
  {
  LteVecApplyCfoSimd(x, cfo, z, len);
  }
