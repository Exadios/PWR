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

#include "phy/common/phy_common.hpp"
#include "phy/common/sequence.hpp"

#ifdef FORCE_STANDARD_RATE
static bool use_standard_rates = true;
#else
static bool use_standard_rates = false;
#endif

//------------------------------------------------------------------------------
/* Returns true if the structure pointed by cell has valid parameters
 */
bool LteCellidIsvalid(uint32_t cell_id)
  {
  if (cell_id < 504)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
bool LteNofprbIsvalid(uint32_t nof_prb)
  {
  if (nof_prb >= 6 && nof_prb <= LTE_MAX_PRB)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
bool LteCellIsvalid(LteCell_t *cell)
  {
  return LteCellidIsvalid(cell->id)           &&
         LtePortidIsvalid(cell->nof_ports)    &&
         LteNofprbIsvalid(cell->nof_prb);
  }

//------------------------------------------------------------------------------
void LteCellFprint(FILE *stream, LteCell_t *cell, uint32_t sfn)
  {
  fprintf(stream, " - PCI:             %d\n", cell->id);
  fprintf(stream, " - Nof ports:       %d\n", cell->nof_ports);
  fprintf(stream, " - CP:              %s\n", LteCpString(cell->cp));
  fprintf(stream, " - PRB:             %d\n", cell->nof_prb);
  fprintf(stream, " - PHICH Length:    %s\n",
          cell->phich_length == LTE_PHICH_EXT ? "Extended" : "Normal");
  fprintf(stream, " - PHICH Resources: ");
  switch (cell->phich_resources)
    {
    case LTE_PHICH_R_1_6:
      fprintf(stream, "1/6");
      break;
    case LTE_PHICH_R_1_2:
      fprintf(stream, "1/2");
      break;
    case LTE_PHICH_R_1:
      fprintf(stream, "1");
      break;
    case LTE_PHICH_R_2:
      fprintf(stream, "2");
      break;
    }
  fprintf(stream, "\n");
  fprintf(stream, " - SFN:             %d\n", sfn);

  }

//------------------------------------------------------------------------------
bool LteSfidxIsvalid(uint32_t sf_idx)
  {
  if (sf_idx <= LTE_NSUBFRAMES_X_FRAME)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
bool LtePortidIsvalid(uint32_t port_id)
  {
  if (port_id <= LTE_MAX_PORTS)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
bool LteNId2Isvalid(uint32_t N_id_2)
  {
  if (N_id_2 < 3)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
bool LteNId1Isvalid(uint32_t N_id_1)
  {
  if (N_id_1 < 168)
    {
    return true;
    }
  else
    {
    return false;
    }
  }

//------------------------------------------------------------------------------
LteMod_t lte_str2mod (char * mod_str)
  {
  int i = 0;

  /* Upper case */
  while (mod_str[i] &= (~' '), mod_str[++i]);

  if (!strcmp(mod_str, "QPSK"))
    {
    return LTE_MOD_QPSK;
    }
  else if (!strcmp(mod_str, "16QAM"))
    {
    return LTE_MOD_16QAM;
    }
  else if (!strcmp(mod_str, "64QAM"))
    {
    return LTE_MOD_64QAM;
    }
  else
    {
    return (LteMod_t) LTE_ERROR_INVALID_INPUTS;
    }
  };


//------------------------------------------------------------------------------
char *LteModString(LteMod_t mod)
  {
  switch (mod)
    {
    case LTE_MOD_BPSK:
      return "BPSK";
    case LTE_MOD_QPSK:
      return "QPSK";
    case LTE_MOD_16QAM:
      return "16QAM";
    case LTE_MOD_64QAM:
      return "64QAM";
    default:
      return "N/A";
    }
  }

//------------------------------------------------------------------------------
uint32_t LteModBitsXSymbol(LteMod_t mod)
  {
  switch (mod)
    {
    case LTE_MOD_BPSK:
      return 1;
    case LTE_MOD_QPSK:
      return 2;
    case LTE_MOD_16QAM:
      return 4;
    case LTE_MOD_64QAM:
      return 6;
    default:
      return 0;
    }
  }

//------------------------------------------------------------------------------
char *LteCpString(LteCp_t cp)
  {
  if (cp == LTE_CP_NORM)
    {
    return "Normal  ";
    }
  else
    {
    return "Extended";
    }
  }

//------------------------------------------------------------------------------
/* Returns the new time advance N_ta_new as specified in Section 4.2.3 of 36.213 */
uint32_t LteNTaNew(uint32_t N_ta_old, uint32_t ta)
  {

  ta &= 63;
  int n_ta_new = N_ta_old + ((float) ta - 31) * 16;
  if (n_ta_new < 0)
    {
    return 0;
    }
  else
    {
    if (n_ta_new < 20512)
      {
      return (uint32_t) n_ta_new;
      }
    else
      {
      return 20512;
      }
    }
  }

//------------------------------------------------------------------------------
float LteCoderate(uint32_t tbs, uint32_t nof_re)
  {
  return (float) (tbs + 24) / (nof_re);
  }

//------------------------------------------------------------------------------
/* Returns the new time advance as indicated by the random access response
 * as specified in Section 4.2.3 of 36.213 */
uint32_t LteNTaNewRar(uint32_t ta)
  {
  if (ta > 1282)
    {
    ta = 1282;
    }
  return ta * 16;
  }


//------------------------------------------------------------------------------
void LteUseStandardSymbolSize(bool enabled)
  {
  use_standard_rates = enabled;
  }

//------------------------------------------------------------------------------
int LteSamplingFreqHz(uint32_t nof_prb)
  {
  int n = LteSymbolSz(nof_prb);
  if (n == -1)
    {
    return LTE_ERROR;
    }
  else
    {
    return 15000 * n;
    }
  }

//------------------------------------------------------------------------------
int LteSymbolSzPower2(uint32_t nof_prb)
  {
  if (nof_prb <= 6)
    {
    return 128;
    }
  else if (nof_prb <= 15)
    {
    return 256;
    }
  else if (nof_prb <= 25)
    {
    return 512;
    }
  else if (nof_prb <= 50)
    {
    return 1024;
    }
  else if (nof_prb <= 75)
    {
    return 1536;
    }
  else if (nof_prb <= 110)
    {
    return 2048;
    }
  else
    {
    return -1;
    }
  }

//------------------------------------------------------------------------------
int LteSymbolSz(uint32_t nof_prb)
  {
  if (nof_prb <= 0)
    {
    return LTE_ERROR;
    }
  if (!use_standard_rates)
    {
    if (nof_prb <= 6)
      {
      return 128;
      }
    else if (nof_prb <= 15)
      {
      return 256;
      }
    else if (nof_prb <= 25)
      {
      return 384;
      }
    else if (nof_prb <= 50)
      {
      return 768;
      }
    else if (nof_prb <= 75)
      {
      return 1024;
      }
    else if (nof_prb <= 110)
      {
      return 1536;
      }
    else
      {
      return LTE_ERROR;
      }
    }
  else
    {
    return LteSymbolSzPower2(nof_prb);
    }
  }

//------------------------------------------------------------------------------
int LteNofPrb(uint32_t symbol_sz)
  {
  if (!use_standard_rates)
    {
    switch(symbol_sz)
      {
      case 128:
        return 6;
      case 256:
        return 15;
      case 384:
        return 25;
      case 768:
        return 50;
      case 1024:
        return 75;
      case 1536:
        return 100;
      }
    }
  else
    {
    switch(symbol_sz)
      {
      case 128:
        return 6;
      case 256:
        return 15;
      case 512:
        return 25;
      case 1024:
        return 50;
      case 1536:
        return 75;
      case 2048:
        return 100;
      }
    }
  return LTE_ERROR;
  }

//------------------------------------------------------------------------------
bool LteSymbolSzIsvalid(uint32_t symbol_sz)
  {
  if (!use_standard_rates)
    {
    if (symbol_sz == 128  ||
        symbol_sz == 256  ||
        symbol_sz == 384  ||
        symbol_sz == 768  ||
        symbol_sz == 1024 ||
        symbol_sz == 1536)
      {
      return true;
      }
    else
      {
      return false;
      }
    }
  else
    {
    if (symbol_sz == 128  ||
        symbol_sz == 256  ||
        symbol_sz == 512  ||
        symbol_sz == 1024 ||
        symbol_sz == 1536 ||
        symbol_sz == 2048)
      {
      return true;
      }
    else
      {
      return false;
      }
    }
  }

//------------------------------------------------------------------------------
uint32_t LteVoffset(uint32_t symbol_id, uint32_t cell_id, uint32_t nof_ports)
  {
  if (nof_ports == 1 && symbol_id == 0)
    {
    return (cell_id + 3) % 6;
    }
  else
    {
    return cell_id % 6;
    }
  }


//------------------------------------------------------------------------------
/** Computes sequence-group pattern f_gh according to 5.5.1.3 of 36.211 */
int LteGroupHoppingFGh(uint32_t f_gh[LTE_NSLOTS_X_FRAME], uint32_t cell_id)
  {
  lte_sequence_t seq;
  bzero(&seq, sizeof(lte_sequence_t));

  if (lte_sequence_LTE_pr(&seq, 160, cell_id / 30))
    {
    return LTE_ERROR;
    }

  for (uint32_t ns = 0; ns < LTE_NSLOTS_X_FRAME; ns++)
    {
    f_gh[ns] = 0;
    for (int i = 0; i < 8; i++)
      {
      f_gh[ns] += (((uint32_t) seq.c[8 * ns + i]) << i);
      }
    }

  lte_sequence_free(&seq);
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
/* Returns the number of available RE per PRB */
uint32_t LteReXPrb(uint32_t ns,
                   uint32_t symbol,
                   uint32_t nof_ports,
                   uint32_t nof_symbols)
  {
  if (symbol == 0)
    {
    if (((ns % 2) == 0) || (ns == 1))
      {
      return LTE_NRE - 4;
      }
    else
      {
      if (nof_ports == 1)
        {
        return LTE_NRE - 2;
        }
      else
        {
        return LTE_NRE - 4;
        }
      }
    }
  else if (symbol == 1)
    {
    if (ns == 1)
      {
      return LTE_NRE - 4;
      }
    else if (nof_ports == 4)
      {
      return LTE_NRE - 4;
      }
    else
      {
      return LTE_NRE;
      }
    }
  else if (symbol == nof_symbols - 3)
    {
    if (nof_ports == 1)
      {
      return LTE_NRE - 2;
      }
    else
      {
      return LTE_NRE - 4;
      }
    }
  else
    {
    return LTE_NRE;
    }
  }


//------------------------------------------------------------------------------
struct lte_band
  {
  uint32_t band;
  float fd_low_mhz;
  uint32_t dl_earfcn_offset;
  uint32_t ul_earfcn_offset;
  float duplex_mhz;
  enum band_geographical_area area;
  };

struct lte_band lte_bands[LTE_NOF_LTE_BANDS] =
  {
    {1, 2110, 0, 18000, 190, LTE_BAND_GEO_AREA_ALL},
    {2, 1930, 600, 18600, 80, LTE_BAND_GEO_AREA_NAR},
    {3, 1805, 1200, 19200, 95, LTE_BAND_GEO_AREA_ALL},
    {4, 2110, 1950, 19950, 400, LTE_BAND_GEO_AREA_NAR},
    {5, 869, 2400, 20400, 45, LTE_BAND_GEO_AREA_NAR},
    {6, 875, 2650, 20650, 45, LTE_BAND_GEO_AREA_APAC},
    {7, 2620, 2750, 20750, 120, LTE_BAND_GEO_AREA_EMEA},
    {8, 925, 3450, 21450, 45, LTE_BAND_GEO_AREA_ALL},
    {9, 1844.9, 3800, 21800, 95, LTE_BAND_GEO_AREA_APAC},
    {10, 2110, 4150, 22150, 400, LTE_BAND_GEO_AREA_NAR},
    {11, 1475.9, 4750, 22750, 48, LTE_BAND_GEO_AREA_JAPAN},
    {12, 729, 5010, 23010, 30, LTE_BAND_GEO_AREA_NAR},
    {13, 746, 5180, 23180, -31, LTE_BAND_GEO_AREA_NAR},
    {14, 758, 5280, 23280, -30, LTE_BAND_GEO_AREA_NAR},
    {17, 734, 5730, 23730, 30, LTE_BAND_GEO_AREA_NAR},
    {18, 860, 5850, 23850, 45, LTE_BAND_GEO_AREA_JAPAN},
    {19, 875, 6000, 24000, 45, LTE_BAND_GEO_AREA_JAPAN},
    {20, 791, 6150, 24150, -41, LTE_BAND_GEO_AREA_EMEA},
    {21, 1495.9, 6450, 24450, 48, LTE_BAND_GEO_AREA_JAPAN},
    {22, 3500, 6600, 24600, 100, LTE_BAND_GEO_AREA_NA},
    {23, 2180, 7500, 25500, 180, LTE_BAND_GEO_AREA_NAR},
    {24, 1525, 7700, 25700, -101.5, LTE_BAND_GEO_AREA_NAR},
    {25, 1930, 8040, 26040, 80, LTE_BAND_GEO_AREA_NAR},
    {26, 859, 8690, 26690, 45, LTE_BAND_GEO_AREA_NAR},
    {27, 852, 9040, 27040, 45, LTE_BAND_GEO_AREA_NAR},
    {28, 758, 9210, 27210, 55, LTE_BAND_GEO_AREA_APAC},
    {29, 717, 9660, 0, 0, LTE_BAND_GEO_AREA_NAR},
    {30, 2350, 9770, 27660, 45, LTE_BAND_GEO_AREA_NAR},
    {31, 462.5, 9870, 27760, 10, LTE_BAND_GEO_AREA_CALA},
    {32, 1452, 9920, 0, 0, LTE_BAND_GEO_AREA_EMEA},
    {64, 0,    10359, 27809, 0, LTE_BAND_GEO_AREA_ALL},
    {65, 2110, 65536, 131072, 90, LTE_BAND_GEO_AREA_ALL},
    {66, 2110, 66436, 131972, 90, LTE_BAND_GEO_AREA_NAR},
    {67, 738, 67336, 0, 0, LTE_BAND_GEO_AREA_EMEA},
    {68, 753, 67536, 132672, 30, LTE_BAND_GEO_AREA_EMEA},
    {69, 2570, 67836, 0, 50, LTE_BAND_GEO_AREA_EMEA},
    {70, 1995, 68336, 132972, 25, LTE_BAND_GEO_AREA_NAR},
    {71, 0, 68586, 133122, 0, LTE_BAND_GEO_AREA_NAR} // dummy band to bound band 70 earfcn
  };


//------------------------------------------------------------------------------
int LteStr2mimotype(char *mimo_type_str, LteMimoType_t *type)
  {
  int i = 0;

  /* Low case */
  while (mimo_type_str[i] |= ' ', mimo_type_str[++i]);

  if (!strcmp(mimo_type_str, "single") || !strcmp(mimo_type_str, "port0"))
    {
    *type = LTE_MIMO_TYPE_SINGLE_ANTENNA;
    }
  else if (!strcmp(mimo_type_str, "diversity") || !strcmp(mimo_type_str, "txdiversity"))
    {
    *type = LTE_MIMO_TYPE_TX_DIVERSITY;
    }
  else if (!strcmp(mimo_type_str, "multiplex") || !strcmp(mimo_type_str, "spatialmux"))
    {
    *type = LTE_MIMO_TYPE_SPATIAL_MULTIPLEX;
    }
  else if (!strcmp(mimo_type_str, "cdd"))
    {
    *type = LTE_MIMO_TYPE_CDD;
    }
  else
    {
    return LTE_ERROR;
    }
  return LTE_SUCCESS;
  }

//------------------------------------------------------------------------------
char *LteMimotype2str(LteMimoType_t mimo_type)
  {
  switch (mimo_type)
    {
    case LTE_MIMO_TYPE_SINGLE_ANTENNA:
      return "Single";
    case LTE_MIMO_TYPE_TX_DIVERSITY:
      return "Diversity";
    case LTE_MIMO_TYPE_SPATIAL_MULTIPLEX:
      return "Multiplex";
    case LTE_MIMO_TYPE_CDD:
      return "CDD";
    default:
      return "N/A";
    }
  }

//------------------------------------------------------------------------------
float get_fd(struct lte_band *band, uint32_t dl_earfcn)
  {
  if (dl_earfcn >= band->dl_earfcn_offset)
    {
    return band->fd_low_mhz + 0.1 * (dl_earfcn - band->dl_earfcn_offset);
    }
  else
    {
    return 0.0;
    }
  }

//------------------------------------------------------------------------------
float get_fu(struct lte_band *band, uint32_t ul_earfcn)
  {
  if (ul_earfcn >= band->ul_earfcn_offset)
    {
    return band->fd_low_mhz - band->duplex_mhz + 0.1*(ul_earfcn - band->ul_earfcn_offset);
    }
  else
    {
    return 0.0;
    }
  }

//------------------------------------------------------------------------------
int LteBandGetBand(uint32_t dl_earfcn)
  {
  uint32_t i = LTE_NOF_LTE_BANDS - 1;
  if (dl_earfcn > lte_bands[i].dl_earfcn_offset)
    {
    fprintf(stderr, "Invalid DL_EARFCN=%d\n", dl_earfcn);
    }
  i--;
  while(i > 0 && lte_bands[i].dl_earfcn_offset > dl_earfcn)
    {
    i--;
    }
  return lte_bands[i].band;
  }

//------------------------------------------------------------------------------
float LteBandFd(uint32_t dl_earfcn)
  {
  uint32_t i = LTE_NOF_LTE_BANDS - 1;
  if (dl_earfcn > lte_bands[i].dl_earfcn_offset)
    {
    fprintf(stderr, "Invalid DL_EARFCN=%d\n", dl_earfcn);
    }
  i--;
  while(i > 0 && lte_bands[i].dl_earfcn_offset > dl_earfcn)
    {
    i--;
    }
  return get_fd(&lte_bands[i], dl_earfcn);
  }


//------------------------------------------------------------------------------
float LteBandFu(uint32_t ul_earfcn)
  {
  uint32_t i = LTE_NOF_LTE_BANDS - 1;
  if (ul_earfcn > lte_bands[i].ul_earfcn_offset)
    {
    fprintf(stderr, "Invalid UL_EARFCN=%d\n", ul_earfcn);
    }
  i--;
  while(i > 0 && (lte_bands[i].ul_earfcn_offset > ul_earfcn || lte_bands[i].ul_earfcn_offset == 0))
    {
    i--;
    }
  return get_fu(&lte_bands[i], ul_earfcn);
  }

//------------------------------------------------------------------------------
uint32_t LteBandUlEarfcn(uint32_t dl_earfcn)
  {
  uint32_t i = LTE_NOF_LTE_BANDS - 1;
  if (dl_earfcn > lte_bands[i].dl_earfcn_offset)
    {
    fprintf(stderr, "Invalid DL_EARFCN=%d\n", dl_earfcn);
    }
  i--;
  while(i > 0 && lte_bands[i].dl_earfcn_offset > dl_earfcn)
    {
    i--;
    }
  return lte_bands[i].ul_earfcn_offset + (dl_earfcn - lte_bands[i].dl_earfcn_offset);
  }

//------------------------------------------------------------------------------
int LteBandGetFdBand_all(uint32_t band,
                         lte_earfcn_t *earfcn,
                         uint32_t max_elems)
  {
  return LteBandGetFdBand(band, earfcn, -1, -1, max_elems);
  }

//------------------------------------------------------------------------------
int LteBandGetFdBand(uint32_t band,
                     lte_earfcn_t *earfcn,
                     int start_earfcn,
                     int end_earfcn,
                     uint32_t max_elems)
  {
  uint32_t i, j;
  uint32_t nof_earfcn;
  i = 0;
  while(i < LTE_NOF_LTE_BANDS && lte_bands[i].band != band)
    {
    i++;
    }
  if (i >= LTE_NOF_LTE_BANDS - 1)
    {
    fprintf(stderr, "Error: Invalid band %d\n", band);
    return LTE_ERROR;
    }
  if (end_earfcn == -1)
    {
    end_earfcn = lte_bands[i + 1].dl_earfcn_offset - 1;
    }
  else
    {
    if (end_earfcn > lte_bands[i + 1].dl_earfcn_offset - 1)
      {
      fprintf(stderr,
              "Error: Invalid end earfcn %d. Max is %d\n",
              end_earfcn,
              lte_bands[i + 1].dl_earfcn_offset - 1);
      return LTE_ERROR;
      }
    }
  if (start_earfcn == -1)
    {
    start_earfcn = lte_bands[i].dl_earfcn_offset;
    }
  else
    {
    if (start_earfcn < lte_bands[i].dl_earfcn_offset)
      {
      fprintf(stderr,
              "Error: Invalid start earfcn %d. Min is %d\n",
              start_earfcn,
              lte_bands[i].dl_earfcn_offset);
      return LTE_ERROR;
      }
    }
  nof_earfcn = end_earfcn - start_earfcn;

  if (nof_earfcn > max_elems)
    {
    nof_earfcn = max_elems;
    }
  for (j = 0; j < nof_earfcn; j++)
    {
    earfcn[j].id = j + start_earfcn;
    earfcn[j].fd = get_fd(&lte_bands[i], earfcn[j].id);
    }
  return (int) j;
  }

//------------------------------------------------------------------------------
int LteBandGetFdRegion(enum band_geographical_area region,
                       lte_earfcn_t *earfcn,
                       uint32_t max_elems)
  {
  uint32_t i;
  int n;
  int nof_fd = 0;
  for (i = 0; i < LTE_NOF_LTE_BANDS && max_elems > 0; i++)
    {
    if (lte_bands[i].area == region)
      {
      n = LteBandGetFdBand(i, &earfcn[nof_fd], -1, -1, max_elems);
      if (n != -1)
        {
        nof_fd += n;
        max_elems -= n;
        }
      else
        {
        return LTE_ERROR;
        }
      }
    }
  return nof_fd;
  }


//------------------------------------------------------------------------------
/* Returns the interval tti1 - tti2 mod 10240 */
uint32_t LteTtiInterval(uint32_t tti1, uint32_t tti2)
  {
  if (tti1 >= tti2)
    {
    return tti1 - tti2;
    }
  else
    {
    return 10240 - tti2 + tti1;
    }
  }

