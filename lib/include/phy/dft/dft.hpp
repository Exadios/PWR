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

#ifndef __DFT_H__
#define __DFT_H__

#include <stdbool.h>
#include "config.hpp"

/*******************************************************************************
 *
 *  Description:  Generic DFT module.
 *                Supports one-dimensional complex and real transforms. Options
 *                are set using the dft_plan_set_x functions.
 *
 *                Options (default is false):
 *
 *                mirror - Rearranges negative and positive frequency bins.
 *                         Swaps after transform for FORWARD, swaps before
 *                         transform for BACKWARD.
 *                db     - Provides output in dB (10 * log10(x)).
 *                norm   - Normalizes output (by sqrt(len) for complex, len
 *                         for real).
 *                dc     - Handles insertion and removal of null DC carrier
 *                         internally.
 *
 *  Reference:
 ******************************************************************************/

typedef enum
  {
  LTE_DFT_COMPLEX,
  LTE_REAL
  } LteDftMode_t;

typedef enum
  {
  LTE_DFT_FORWARD,
  LTE_DFT_BACKWARD
  } LteDftDir_t;

typedef struct LTE_API
  {
  int init_size;      // DFT length used in the first initialization
  int size;           // DFT length
  void *in;           // Input buffer
  void *out;          // Output buffer
  void *p;            // DFT plan
  bool is_guru;
  bool forward;       // Forward transform?
  bool mirror;        // Shift negative and positive frequencies?
  bool db;            // Provide output in dB?
  bool norm;          // Normalize output?
  bool dc;            // Handle insertion/removal of null DC carrier internally?
  LteDftDir_t dir;    // Forward/Backward
  LteDftMode_t mode;  // Complex/Real
  } LteDftPlan_t;

LTE_API void LteDftLoad();

LTE_API void LteDftExit();

LTE_API int LteDftPlan(LteDftPlan_t *plan,
                       int dft_points,
                       LteDftDir_t dir,
                       LteDftMode_t type);

LTE_API int LteDftPlanC(LteDftPlan_t *plan,
                        int dft_points,
                        LteDftDir_t dir);

LTE_API int LteDftPlanGuruC(LteDftPlan_t *plan,
                            int dft_points,
                            LteDftDir_t dir,
                            Cf_t *in_buffer,
                            Cf_t *out_buffer,
                            int istride,
                            int ostride,
                            int how_many,
                            int idist,
                            int odist);

LTE_API int LteDftPlanR(LteDftPlan_t *plan,
                        int dft_points,
                        LteDftDir_t dir);

LTE_API int LteDftReplan(LteDftPlan_t *plan,
                         const int new_dft_points);

LTE_API int LteDftReplanGuruC(LteDftPlan_t *plan,
                              const int new_dft_points,
                              Cf_t *in_buffer,
                              Cf_t *out_buffer,
                              int istride,
                              int ostride,
                              int how_many,
                              int idist,
                              int odist);

LTE_API int LteDftReplanC(LteDftPlan_t *plan,
                          int new_dft_points);

LTE_API int LteDftReplanR(LteDftPlan_t *plan,
                          int new_dft_points);


LTE_API void LteDftPlanFree(LteDftPlan_t *plan);

/* Set options */

LTE_API void LteDftPlanSetMirror(LteDftPlan_t *plan,
                                 bool val);

LTE_API void LteDftPlanSetDb(LteDftPlan_t *plan,
                             bool val);

LTE_API void LteDftPlanSetNorm(LteDftPlan_t *plan,
                               bool val);

LTE_API void LteDftPlanSetDc(LteDftPlan_t *plan,
                             bool val);

/* Compute DFT */

LTE_API void LteDftRun(LteDftPlan_t *plan,
                       const void *in,
                       void *out);

LTE_API void LteDftRunCZerocopy(LteDftPlan_t *plan,
                                const Cf_t *in,
                                Cf_t *out);

LTE_API void LteDftRunC(LteDftPlan_t *plan,
                        const Cf_t *in,
                        Cf_t *out);

LTE_API void LteDftRunGuruC(LteDftPlan_t *plan);

LTE_API void LteDftRunR(LteDftPlan_t *plan,
                        const float *in,
                        float *out);

#endif // __DFT_H__

