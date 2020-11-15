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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <stdbool.h>

#include "lte/lte.hpp"

void init_plots();
void do_plots(float *corr, float energy, uint32_t size, cf_t ce[LTE_PSS_LEN]);
void do_plots_sss(float *corr_m0, float *corr_m1);
void destroy_plots();

bool disable_plots = false;
char *input_file_name;
int cell_id = -1;
int nof_frames = -1;
uint32_t fft_size=64;
float threshold = 0.4; 
int n_id_2_sync = -1;
lte_cp_t cp = LTE_CP_NORM;
int file_offset = 0;

//------------------------------------------------------------------------------
void usage(char *prog)
  {
  printf("Usage: %s [nlestodv] -i cell_id -f input_file_name\n", prog);
  printf("\t-n nof_frames [Default %d]\n", nof_frames);
  printf("\t-l n_id_2 to sync [Default use cell_id]\n");
  printf("\t-e Extended CP [Default Normal]\n");
  printf("\t-s symbol_sz [Default %d]\n", fft_size);
  printf("\t-t threshold [Default %.2f]\n", threshold);
  printf("\t-o file read offset [Default %d]\n", file_offset);
  printf("\t-d disable plots [Default enabled]\n");
  printf("\t-v lte_verbose\n");
  }

//------------------------------------------------------------------------------
void parse_args(int argc, char **argv)
  {
  int opt;
  while ((opt = getopt(argc, argv, "nlestdvoif")) != -1)
    {
    switch (opt)
      {
      case 'f':
        input_file_name = argv[optind];
        break;
      case 't':
        threshold = atof(argv[optind]);
        break;
      case 'e':
        cp = LTE_CP_EXT;
        break;
      case 'i':
        cell_id = atoi(argv[optind]);
        break;
      case 'o':
        file_offset = atoi(argv[optind]);
        break;
      case 'l':
        n_id_2_sync = atoi(argv[optind]);
        break;
      case 's':
        fft_size = atoi(argv[optind]);
        break;
      case 'n':
        nof_frames = atoi(argv[optind]);
        break;
      case 'd':
        disable_plots = true;
        break;
      case 'v':
        lte_verbose++;
        break;
      default:
        usage(argv[0]);
        exit(-1);
      }
    }
  if (cell_id < 0)
    {
    usage(argv[0]);
    exit(-1);
    }
  }

float m0_value, m1_value;
//------------------------------------------------------------------------------
int main(int argc, char **argv)
  {
  lte_filesource_t fsrc;
  Cf_t *buffer; 
  int frame_cnt, n;
  lte_pss_t pss;
  lte_cfo_t cfocorr, cfocorr64; 
  lte_sss_t sss;
  int32_t flen; 
  int peak_idx, last_peak;
  float peak_value; 
  float mean_peak; 
  uint32_t nof_det, nof_nodet, nof_nopeak, nof_nopeakdet;
  Cf_t ce[LTE_PSS_LEN]; 
  
  parse_args(argc, argv);

  if (n_id_2_sync == -1)
    {
    n_id_2_sync = cell_id % 3;
    }
  uint32_t n_id_2 = cell_id % 3;
  uint32_t n_id_1 = cell_id / 3;

  if (!disable_plots)
    init_plots();
  
  flen = fft_size * 15 * 5;
  
  buffer = malloc(sizeof(Cf_t) * flen * 2);
  if (!buffer)
    {
    perror("malloc");
    exit(-1);
    }
    
  if (LtePssInitFft(&pss, flen, fft_size))
    {
    fprintf(stderr, "Error initiating PSS\n");
    exit(-1);
    }

  if (LtePssNId2(&pss, n_id_2_sync))
    {
    fprintf(stderr, "Error setting n_id_2=%d\n",n_id_2_sync);
    exit(-1);
  }
  
  LteCfoInit(&cfocorr, flen); 
  LteCfoInit(&cfocorr64, flen); 
 
  if (LteSssInit(&sss, fft_size))
    {
    fprintf(stderr, "Error initializing SSS object\n");
    return LTE_ERROR;
    }

  LteSssSetNId2(&sss, n_id_2);

  printf("Opening file...\n");
  if (lte_filesource_init(&fsrc, input_file_name, LTE_COMPLEX_FLOAT_BIN))
    {
    fprintf(stderr, "Error opening file %s\n", input_file_name);
    exit(-1);
    }
  printf("n_id_2: %d\n", n_id_2);  
  
  printf("Frame length %d samples\n", flen);
  printf("PSS detection threshold: %.2f\n", threshold);
  
  nof_det = nof_nodet = nof_nopeak = nof_nopeakdet = 0;
  frame_cnt = 0;
  last_peak = 0; 
  mean_peak = 0;
  int peak_offset = 0;
  float cfo; 
  float mean_cfo = 0; 
  uint32_t m0, m1; 
  uint32_t sss_error1 = 0, sss_error2 = 0, sss_error3 = 0; 
  uint32_t cp_is_norm = 0; 
  
  lte_sync_t ssync; 
  bzero(&ssync, sizeof(LteSync_t));
  ssync.fft_size = fft_size;
  
  n = lte_filesource_read(&fsrc, buffer, file_offset);
  
  while(frame_cnt < nof_frames || nof_frames == -1)
    {
    n = lte_filesource_read(&fsrc, buffer, flen - peak_offset);
    if (n < 0) {
      fprintf(stderr, "Error reading samples\n");
      exit(-1);
    }
    if (n < flen - peak_offset)
      {
      fprintf(stdout, "End of file (n=%d, flen=%d, peak=%d)\n", n, flen, peak_offset);
      break;
      }
    
    peak_idx = LtePssFindPss(&pss, buffer, &peak_value);
    if (peak_idx < 0)
      {
      fprintf(stderr, "Error finding PSS peak\n");
      exit(-1);
      }
        
    mean_peak = SRSLTE_VEC_CMA(peak_value, mean_peak, frame_cnt);
    
    if (peak_value >= threshold)
      {
      nof_det++;
        
      if (peak_idx >= fft_size)
        {

        // Estimate CFO 
        cfo = LtePssCfoCompute(&pss, &buffer[peak_idx-fft_size]);
        mean_cfo = LTE_VEC_CMA(cfo, mean_cfo, frame_cnt);        

        // Correct CFO
        lte_cfo_correct(&cfocorr, buffer, buffer, -mean_cfo / fft_size);               

        // Estimate channel
        if (lte_pss_chest(&pss, &buffer[peak_idx-fft_size], ce))
          {
          fprintf(stderr, "Error computing channel estimation\n");
          exit(-1);
          }
        
        // Find SSS 
        int sss_idx = peak_idx - 2 * fft_size - (LTE_CP_ISNORM(cp) ? LTE_CP_LEN(fft_size, LTE_CP_NORM_LEN) : LTE_CP_LEN(fft_size, LTE_CP_EXT_LEN));             
        if (sss_idx >= 0 && sss_idx < flen-fft_size) 
          {
          lte_sss_m0m1_partial(&sss, &buffer[sss_idx], 3, NULL, &m0, &m0_value, &m1, &m1_value);
          if (lte_sss_n_id_1(&sss, m0, m1) != n_id_1)
            {
            sss_error2++;            
            }
          INFO("sf_idx = %d\n", LteSssSubframe(m0, m1));
          INFO("Partial n_id_1: %d\n", LteSssNId1(&sss, m0, m1));
          lte_sss_m0m1_diff(&sss, &buffer[sss_idx], &m0, &m0_value, &m1, &m1_value);
          if (lte_sss_n_id_1(&sss, m0, m1) != n_id_1)
            {
            sss_error3++;            
            }
          INFO("Diff n_id_1: %d\n", LteSssNId1(&sss, m0, m1));
          lte_sss_m0m1_partial(&sss, &buffer[sss_idx], 1, NULL, &m0, &m0_value, &m1, &m1_value);
          if (LteSssNId1(&sss, m0, m1) != n_id_1)
            {
            sss_error1++;     
            }
          INFO("Full n_id_1: %d\n", lte_sss_n_id_1(&sss, m0, m1));
          }
        
        // Estimate CP 
        if (peak_idx > 2 * (fft_size + LTE_CP_LEN_EXT(fft_size)))
          {
          lte_cp_t cp = lte_sync_detect_cp(&ssync, buffer, peak_idx);
          if (LTE_CP_ISNORM(cp))
            {
            cp_is_norm++; 
            }          
          }
        }
      else
        {
        INFO("No space for CFO computation. Frame starts at \n");
        }
      
      if(lte_sss_subframe(m0, m1) == 0)
        {
        if (!disable_plots)
          do_plots_sss(sss.corr_output_m0, sss.corr_output_m1);
        }
      }
    else
      {
      nof_nodet++;
      }

    if (frame_cnt > 100)
      {
      if (abs(last_peak-peak_idx) > 4)
        {
        if (peak_value >= threshold)
          {
          nof_nopeakdet++;
          } 
        nof_nopeak++;                  
        } 
      }
    
    frame_cnt++;
   
    printf("[%5d]: Pos: %5d, PSR: %4.1f (~%4.1f) Pdet: %4.2f, "
           "FA: %4.2f, CFO: %+4.1f kHz SSSmiss: %4.2f/%4.2f/%4.2f CPNorm: %.0f%%\r", 
           frame_cnt, 
           peak_idx - flen/10, 
           peak_value, mean_peak,
           (float) nof_det/frame_cnt, 
           (float) nof_nopeakdet/frame_cnt, mean_cfo*15, 
           (float) sss_error1/nof_det,(float) sss_error2/nof_det,(float) sss_error3/nof_det,
           (float) cp_is_norm/nof_det * 100);
    
    if (LTE_VERBOSE_ISINFO())
      {
      printf("\n");
      }

    usleep(10000);
  
    if (!disable_plots)
      do_plots(pss.conv_output_avg, pss.conv_output_avg[peak_idx], pss.fft_size + pss.frame_size - 1, ce);

    last_peak = peak_idx;
    }
  
  lte_pss_free(&pss);
  free(buffer);
  lte_filesource_free(&fsrc);
  if (!disable_plots)
    destroy_plots();

  printf("Ok\n");
  exit(0);
  }

