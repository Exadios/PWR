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

#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <string.h>
#include "lte.hpp"

#include "phy/dft/dft.hpp"
#include "phy/vector/vector.hpp"

#define dft_ceil(a, b) ((a - 1) / b + 1)
#define dft_floor(a, b) (a / b)

#define FFTW_WISDOM_FILE ".fftw_wisdom"

#ifdef FFTW_WISDOM_FILE
#define FFTW_TYPE FFTW_MEASURE
#else
#define FFTW_TYPE 0
#endif

static pthread_mutex_t fft_mutex = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------------------------------------------
void LteDftLoad()
  {
#ifdef FFTW_WISDOM_FILE
  fftwf_import_wisdom_from_filename(FFTW_WISDOM_FILE);
#else
  printf("Warning: FFTW Wisdom file not defined\n");
#endif
  }

//------------------------------------------------------------------------------
void LteDftExit()
  {
#ifdef FFTW_WISDOM_FILE
  fftwf_export_wisdom_to_filename(FFTW_WISDOM_FILE);
#endif
  fftwf_cleanup();
  }

//------------------------------------------------------------------------------
int LteDftPlan(LteDftPlan_t *plan,
               const int dft_points,
               LteDftDir_t dir,
               LteDftMode_t mode)
  {
  bzero(plan, sizeof(LteDftPlan_t));
  if(mode == LTE_DFT_COMPLEX)
    {
    return LteDftPlanC(plan,dft_points,dir);
    }
  else
    {
    return LteDftPlanR(plan,dft_points,dir);
    }
  return 0;
  }

//------------------------------------------------------------------------------
int LteDftReplan(LteDftPlan_t *plan, const int new_dft_points)
  {
  if (new_dft_points <= plan->init_size)
    {
    if(plan->mode == LTE_DFT_COMPLEX)
      {
      return LteDftReplanC(plan,new_dft_points);
      }
    else
      {
      return LteDftReplanR(plan,new_dft_points);
      }
    }
  else
    {
    fprintf(stderr,
            "DFT: Error calling replan: new_dft_points (%d) must be lower or equal "
            "dft_size passed initially (%d)\n", new_dft_points, plan->init_size);
    return -1;
    }
  }

//------------------------------------------------------------------------------
static void Allocate(LteDftPlan_t *plan, int size_in, int size_out, int len)
  {
  plan->in  = fftwf_malloc(size_in * len);
  plan->out = fftwf_malloc(size_out * len);
  }

//------------------------------------------------------------------------------
int LteDftReplanGuruC(LteDftPlan_t *plan,
                      const int new_dft_points,
                      Cf_t *in_buffer,
                      Cf_t *out_buffer,
                      int istride,
                      int ostride,
                      int how_many,
                      int idist,
                      int odist)
  {
  int sign = (plan->forward) ? FFTW_FORWARD : FFTW_BACKWARD;

  const fftwf_iodim iodim = {new_dft_points, istride, ostride};
  const fftwf_iodim howmany_dims = {how_many, idist, odist};

  pthread_mutex_lock(&fft_mutex);

  /* Destroy current plan */
  fftwf_destroy_plan((fftwf_plan)plan->p);

  plan->p = fftwf_plan_guru_dft(1,
                                &iodim,
                                1,
                                &howmany_dims,
                                (float (*)[2])in_buffer,
                                (float (*)[2])out_buffer,
                                sign,
                                FFTW_TYPE);

  pthread_mutex_unlock(&fft_mutex);

  if (!plan->p)
    {
    return -1;
    }
  plan->size = new_dft_points;
  plan->init_size = plan->size;

  return 0;
  }

//------------------------------------------------------------------------------
int LteDftReplanC(LteDftPlan_t *plan, const int new_dft_points)
  {
  int sign = (plan->dir == LTE_DFT_FORWARD) ? FFTW_FORWARD : FFTW_BACKWARD;

  pthread_mutex_lock(&fft_mutex);
  if (plan->p)
    {
    fftwf_destroy_plan((fftwf_plan)plan->p);
    plan->p = NULL;
    }
  plan->p = fftwf_plan_dft_1d(new_dft_points,
                              (float (*)[2])plan->in,
                              (float (*)[2])plan->out,
                              sign,
                              FFTW_TYPE);
  pthread_mutex_unlock(&fft_mutex);

  if (!plan->p)
    {
    return -1;
    }
  plan->size = new_dft_points;
  return 0;
  }

//------------------------------------------------------------------------------
int LteDftPlanGuruC(LteDftPlan_t *plan,
                    const int dft_points,
                    LteDftDir_t dir,
                    Cf_t *in_buffer,
                    Cf_t *out_buffer,
                    int istride,
                    int ostride,
                    int how_many,
                    int idist,
                    int odist)
  {
  int sign = (dir == LTE_DFT_FORWARD) ? FFTW_FORWARD : FFTW_BACKWARD;

  const fftwf_iodim iodim = {dft_points, istride, ostride};
  const fftwf_iodim howmany_dims = {how_many, idist, odist};

  pthread_mutex_lock(&fft_mutex);

  plan->p = fftwf_plan_guru_dft(1,
                                &iodim,
                                1,
                                &howmany_dims,
                                (float (*)[2])in_buffer,
                                (float (*)[2])out_buffer,
                                sign,
                                FFTW_TYPE);
  if (!plan->p)
    {
    return -1;
    }
  pthread_mutex_unlock(&fft_mutex);

  plan->size = dft_points;
  plan->init_size = plan->size;
  plan->mode = LTE_DFT_COMPLEX;
  plan->dir = dir;
  plan->forward = (dir==LTE_DFT_FORWARD)?true:false;
  plan->mirror = false;
  plan->db = false;
  plan->norm = false;
  plan->dc = false;
  plan->is_guru = true;

  return 0;
  }

//------------------------------------------------------------------------------
int LteDftPlanC(LteDftPlan_t *plan, const int dft_points, LteDftDir_t dir)
  {
  Allocate(plan,sizeof(fftwf_complex),sizeof(fftwf_complex), dft_points);

  pthread_mutex_lock(&fft_mutex);

  int sign = (dir == LTE_DFT_FORWARD) ? FFTW_FORWARD : FFTW_BACKWARD;
//  fprintf(stderr, "***** plan dft 1d\n");
  plan->p = fftwf_plan_dft_1d(dft_points,
                              (float (*)[2])plan->in, 
                              (float (*)[2])plan->out,
                              sign,
                              FFTW_TYPE);

  pthread_mutex_unlock(&fft_mutex);

  if (!plan->p)
    {
    return -1;
    }
  plan->size = dft_points;
  plan->init_size = plan->size;
  plan->mode = LTE_DFT_COMPLEX;
  plan->dir = dir;
  plan->forward = (dir==LTE_DFT_FORWARD)?true:false;
  plan->mirror = false;
  plan->db = false;
  plan->norm = false;
  plan->dc = false;
  plan->is_guru = false;

  return 0;
  }

//------------------------------------------------------------------------------
int LteDftReplanR(LteDftPlan_t *plan, const int new_dft_points)
  {
  int sign = (plan->dir == LTE_DFT_FORWARD) ? FFTW_R2HC : FFTW_HC2R;

  pthread_mutex_lock(&fft_mutex);
  if (plan->p)
    {
    fftwf_destroy_plan((fftwf_plan)plan->p);
    plan->p = NULL;
    }
  plan->p = fftwf_plan_r2r_1d(new_dft_points,
                              (float *)plan->in,
                              (float *)plan->out,
                              (fftwf_r2r_kind)sign,
                              FFTW_TYPE);
  pthread_mutex_unlock(&fft_mutex);

  if (!plan->p)
    {
    return -1;
    }
  plan->size = new_dft_points;
  return 0;
  }

//------------------------------------------------------------------------------
int LteDftPlanR(LteDftPlan_t *plan, const int dft_points, LteDftDir_t dir)
  {
  Allocate(plan,sizeof(float),sizeof(float), dft_points);
  int sign = (dir == LTE_DFT_FORWARD) ? FFTW_R2HC : FFTW_HC2R;

  pthread_mutex_lock(&fft_mutex);
  plan->p = fftwf_plan_r2r_1d(dft_points,
                              (float *)plan->in,
                              (float *)plan->out,
                              (fftwf_r2r_kind)sign,
                              FFTW_TYPE);
  pthread_mutex_unlock(&fft_mutex);

  if (!plan->p)
    {
    return -1;
    }
  plan->size = dft_points;
  plan->init_size = plan->size;
  plan->mode = LTE_REAL;
  plan->dir = dir;
  plan->forward = (dir==LTE_DFT_FORWARD)?true:false;
  plan->mirror = false;
  plan->db = false;
  plan->norm = false;
  plan->dc = false;

  return 0;
  }

//------------------------------------------------------------------------------
void LteDftPlanSetMirror(LteDftPlan_t *plan, bool val)
  {
  plan->mirror = val;
  }

//------------------------------------------------------------------------------
void LteDftPlanSetDb(LteDftPlan_t *plan, bool val)
  {
  plan->db = val;
  }

//------------------------------------------------------------------------------
void LteDftPlanSetNorm(LteDftPlan_t *plan, bool val)
  {
  plan->norm = val;
  }

//------------------------------------------------------------------------------
void LteDftPlanSetDc(LteDftPlan_t *plan, bool val)
  {
  plan->dc = val;
  }

//------------------------------------------------------------------------------
static void CopyPre(uint8_t *dst, uint8_t *src, int size_d, int len,
                    bool forward, bool mirror, bool dc)
  {
  int offset = dc ? 1 : 0;
  if(mirror && !forward)
    {
    int hlen = dft_floor(len,2);
    memset(dst, 0, size_d * offset);
    memcpy(&dst[size_d * offset],
           &src[size_d * hlen],
           size_d * (len - hlen - offset));
    memcpy(&dst[(len - hlen) * size_d], src, size_d * hlen);
    }
  else
    {
    memcpy(dst, src, size_d * len);
    }
  }

//------------------------------------------------------------------------------
static void CopyPost(uint8_t *dst, uint8_t *src, int size_d, int len,
                     bool forward, bool mirror, bool dc)
  {
  int offset = dc ? 1 : 0;
  if(mirror && forward)
    {
    int hlen = dft_ceil(len, 2);
    memcpy(dst, &src[size_d * hlen], size_d * (len - hlen));
    memcpy(&dst[(len - hlen) * size_d],
           &src[size_d * offset],
           size_d * (hlen - offset));
    }
  else
    {
    memcpy(dst, src, size_d * len);
    }
  }

//------------------------------------------------------------------------------
void LteDftRun(LteDftPlan_t *plan, const void *in, void *out)
  {
  if(plan->mode == LTE_DFT_COMPLEX)
    {
    LteDftRunC(plan, (Cf_t *)in, (Cf_t *)out);
    }
  else
    {
    LteDftRunR(plan, (float *)in, (float *)out);
    }
  }

//------------------------------------------------------------------------------
void LteDftRunCZerocopy(LteDftPlan_t *plan, const Cf_t *in, Cf_t *out)
  {
  fftwf_execute_dft((fftwf_plan)plan->p, (float (*)[2])in, (float (*)[2])out);
  }

//------------------------------------------------------------------------------
void LteDftRunC(LteDftPlan_t *plan, const Cf_t *in, Cf_t *out)
  {
  float norm;
  int i;
  fftwf_complex *f_out = (float (*)[2])plan->out;

  CopyPre((uint8_t*)plan->in,
          (uint8_t*)in,
          sizeof(Cf_t),
          plan->size,
          plan->forward,
          plan->mirror,
          plan->dc);
  fftwf_execute((fftwf_plan)plan->p);
  if (plan->norm)
    {
    norm = 1.0 / sqrtf(plan->size);
    LteVecScProdCfc((const Cf_t *)f_out, norm, (Cf_t *)f_out, plan->size);
    }
  if (plan->db)
    {
    for (i = 0; i < plan->size; i++)
      {
      f_out[i][0] = 10 * log10(f_out[i][0]);
      }
    }
  CopyPost((uint8_t*)out,
           (uint8_t*)plan->out,
           sizeof(Cf_t),
           plan->size,
           plan->forward,
           plan->mirror,
           plan->dc);
  }

//------------------------------------------------------------------------------
void LteDftRunGuruC(LteDftPlan_t *plan)
  {
  if (plan->is_guru == true)
    {
    fftwf_execute((fftwf_plan)plan->p);
    }
  else
    {
    fprintf(stderr, "LteDftRunGuruC: the selected plan is not guru!\n");
    }
  }

//------------------------------------------------------------------------------
void LteDftRunR(LteDftPlan_t *plan, const float *in, float *out)
  {
  float norm;
  int i;
  int len = plan->size;
  float *f_out = (float *)plan->out;

  memcpy(plan->in, in, sizeof(float) * plan->size);
  fftwf_execute((fftwf_plan)plan->p);
  if (plan->norm)
    {
    norm = 1.0 / plan->size;
    LteVecScProdFff(f_out, norm, f_out, plan->size);
    }
  if (plan->db)
    {
    for (i = 0; i < len; i++)
      {
      f_out[i] = 10 * log10(f_out[i]);
      }
    }
  memcpy(out, plan->out, sizeof(float) * plan->size);
  }

//------------------------------------------------------------------------------
void LteDftPlanFree(LteDftPlan_t *plan)
  {
  if (!plan) return;
  if (!plan->size) return;

  pthread_mutex_lock(&fft_mutex);
  if (!plan->is_guru)
    {
    if (plan->in) fftwf_free(plan->in);
    if (plan->out) fftwf_free(plan->out);
    }
  if (plan->p)
    fftwf_destroy_plan((fftwf_plan)plan->p);
  pthread_mutex_unlock(&fft_mutex);
  bzero(plan, sizeof(LteDftPlan_t));
  }

