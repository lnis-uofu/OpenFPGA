/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    fpga_x2p_bitstream_utils.c
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/07/02  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains most utilized functions for the bitstream generator 
 ***********************************************************************/

/***********************************/
/* Synthesizable Verilog Dumping   */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "vpr_utils.h"

/* FPGA-SPICE utils */
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_globals.h"

#include "fpga_x2p_bitstream_utils.h"

/* Determine the size of input address of a decoder */
int determine_decoder_size(int num_addr_out) {
  return ceil(log(num_addr_out)/log(2.));
}

static 
int count_num_sram_bits_one_lut_spice_model(t_spice_model* cur_spice_model) {
  int num_sram_bits = 0;
  int iport;
  int lut_size;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  int lut_sram_port = 0;
  int mode_sram_port = 0;

  assert(NULL != cur_spice_model);
  assert(SPICE_MODEL_LUT == cur_spice_model->type);

  /* Check ports */
  input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_input_port);

  /* Determine size of LUT*/
  lut_size = input_ports[0]->size;

  num_sram_bits = 0;
  lut_sram_port = 0;
  mode_sram_port = 0;
  /* Total SRAM bit count = LUT SRAM bit + Mode bit */
  for (iport = 0; iport < num_sram_port; iport++) {
    if (FALSE == sram_ports[iport]->mode_select) {
      num_sram_bits += (int)pow(2.,(double)(lut_size));
      assert(num_sram_bits == sram_ports[iport]->size);
      lut_sram_port++;
    } else { 
      assert (TRUE == sram_ports[iport]->mode_select);
      num_sram_bits += sram_ports[iport]->size;
      mode_sram_port++;
    }
  }
  assert (1 == lut_sram_port);
  assert ((0 == mode_sram_port) || (1 == mode_sram_port));
  
  /* TODO: could be more smart! Use mapped spice_model of SRAM ports!  
   * Support Non-volatile RRAM-based SRAM */
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
  /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
   * Number of memory bits is still same as CMOS SRAM
   */
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  /* Free */
  my_free(input_ports);
  my_free(sram_ports);

  return num_sram_bits;
}

static 
int count_num_sram_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                            int mux_size) {
  int num_sram_bits = 0;
  int num_input_size = mux_size;

  assert(SPICE_MODEL_MUX == cur_spice_model->type);

  assert((2 == mux_size)||(2 < mux_size));

  num_input_size = get_mux_full_input_size (cur_spice_model, mux_size);

  /* Number of configuration bits depends on the MUX structure */
  switch (cur_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_sram_bits = determine_tree_mux_level(num_input_size);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    num_sram_bits = num_input_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_sram_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level
                    * determine_num_input_basis_multilevel_mux(num_input_size, 
                      cur_spice_model->design_tech_info.mux_info->mux_num_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name);
    exit(1);
  }
  /* For 2:1 MUX, whatever structure, there is only one level */
  if (2 == num_input_size) {
    num_sram_bits = 1;
  }
  /* Also the number of configuration bits depends on the technology*/
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
    /* 4T1R MUX requires more configuration bits */
    if (SPICE_MODEL_STRUCTURE_TREE == cur_spice_model->design_tech_info.mux_info->structure) {
    /* For tree-structure: we need 3 times more config. bits */
      num_sram_bits = 3 * num_sram_bits;
    } else if (SPICE_MODEL_STRUCTURE_MULTILEVEL == cur_spice_model->design_tech_info.mux_info->structure) {
    /* For multi-level structure: we need 1 more config. bits for each level */
      num_sram_bits += cur_spice_model->design_tech_info.mux_info->mux_num_level;
    } else {
      num_sram_bits = (num_sram_bits + 1);
    }
    /* For 2:1 MUX, whatever structure, there is only one level */
    if (2 == num_input_size) {
      num_sram_bits = 3;
    } 
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    /* When a local encoder is added, the number of sram bits will be reduced
     * to N * log_2{X}, where N is the number of levels and X is the number of inputs per level
     * Note that: we only apply this to one-level and multi-level multiplexers
     */
    if ( (TRUE == cur_spice_model->design_tech_info.mux_info->local_encoder) 
        && (2 < num_input_size) ) { 
      if (SPICE_MODEL_STRUCTURE_ONELEVEL == cur_spice_model->design_tech_info.mux_info->structure) {
        num_sram_bits = determine_mux_local_encoder_num_inputs(num_sram_bits);
      } else if (SPICE_MODEL_STRUCTURE_MULTILEVEL == cur_spice_model->design_tech_info.mux_info->structure) {
        num_sram_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level * determine_mux_local_encoder_num_inputs(num_sram_bits / cur_spice_model->design_tech_info.mux_info->mux_num_level);
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  /* Free */

  return num_sram_bits;
}

static 
int count_num_sram_bits_one_generic_spice_model(t_spice_model* cur_spice_model) {
  int iport;
  int num_sram_bits = 0;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Other block, we just count the number SRAM ports defined by user */
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  /* TODO: could be more smart! 
   * Support Non-volatile RRAM-based SRAM */
  if (0 < num_sram_port) {
    assert(NULL != sram_ports);
  }
  for (iport = 0; iport < num_sram_port; iport++) {
    assert(NULL != sram_ports[iport]->spice_model);
    num_sram_bits += sram_ports[iport]->size;
    /* TODO: could be more smart! 
    * Support Non-volatile RRAM-based SRAM */
    switch (cur_spice_model->design_tech) {
    case SPICE_MODEL_DESIGN_RRAM:
    /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
     * Number of memory bits is still same as CMOS SRAM
     */
    case SPICE_MODEL_DESIGN_CMOS:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
                 __FILE__, __LINE__, cur_spice_model->name); 
      exit(1);
    }
  }

  /* Free */
  my_free(sram_ports);

  return num_sram_bits;
}

/* Count the number of configuration bits of a spice model */
int count_num_sram_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        int mux_size) {
  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    return count_num_sram_bits_one_lut_spice_model(cur_spice_model);
  case SPICE_MODEL_MUX:
    return count_num_sram_bits_one_mux_spice_model(cur_spice_model, mux_size);
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_CCFF:
  case SPICE_MODEL_IOPAD:
    return count_num_sram_bits_one_generic_spice_model(cur_spice_model);
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return -1;
}

static 
int count_num_mode_bits_one_generic_spice_model(t_spice_model* cur_spice_model) {
  int iport;
  int num_mode_bits = 0;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Other block, we just count the number SRAM ports defined by user */
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  /* TODO: could be more smart! 
   * Support Non-volatile RRAM-based SRAM */
  if (0 < num_sram_port) {
    assert(NULL != sram_ports);
    for (iport = 0; iport < num_sram_port; iport++) {
      assert(NULL != sram_ports[iport]->spice_model);
      if (FALSE == sram_ports[iport]->mode_select) {
        continue;
      }
      num_mode_bits += sram_ports[iport]->size;
      /* TODO: could be more smart! 
       * Support Non-volatile RRAM-based SRAM */
      switch (cur_spice_model->design_tech) {
      case SPICE_MODEL_DESIGN_RRAM:
      /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
       * Number of memory bits is still same as CMOS SRAM
       */
      case SPICE_MODEL_DESIGN_CMOS:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
                   __FILE__, __LINE__, cur_spice_model->name); 
        exit(1);
      }
    }
  }

  /* Free */
  my_free(sram_ports);

  return num_mode_bits;
}


/* Count the number of configuration bits of a spice model */
int count_num_mode_bits_one_spice_model(t_spice_model* cur_spice_model) {
  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
  case SPICE_MODEL_MUX:
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_CCFF:
  case SPICE_MODEL_IOPAD:
    return count_num_mode_bits_one_generic_spice_model(cur_spice_model);
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return -1;
}


/* For a non-volatile SRAM, we determine its number of reserved conf. bits */
int count_num_reserved_conf_bits_one_rram_sram_spice_model(t_spice_model* cur_spice_model,
                                                           enum e_sram_orgz cur_sram_orgz_type) {
  int num_reserved_conf_bits = 0;
  int num_bl_ports, num_wl_ports;
  t_spice_model_port** bl_ports = NULL;
  t_spice_model_port** wl_ports = NULL;

  /* Check */
  assert(SPICE_MODEL_SRAM == cur_spice_model->type);

  switch (cur_sram_orgz_type) {
  case SPICE_SRAM_MEMORY_BANK:
    find_bl_wl_ports_spice_model(cur_spice_model,
                                 &num_bl_ports, &bl_ports,
                                 &num_wl_ports, &wl_ports);
    assert((1 == num_bl_ports)&&(1 == num_wl_ports));
    assert(bl_ports[0]->size == wl_ports[0]->size);
    num_reserved_conf_bits = bl_ports[0]->size - 1; /*TODO: to be more smart: num_bl-1 of SRAM model ?*/
    break;
  case SPICE_SRAM_SCAN_CHAIN:
  case SPICE_SRAM_STANDALONE:
    num_reserved_conf_bits = 0;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Free */
  my_free(bl_ports);
  my_free(wl_ports);

  return num_reserved_conf_bits;
} 


/* For a multiplexer, determine its reserved configuration bits */
int count_num_reserved_conf_bits_one_lut_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type) {
  int num_reserved_conf_bits = 0;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  int iport;

  /* Check */
  assert(SPICE_MODEL_LUT == cur_spice_model->type);

  /* Determine size of LUT*/
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  /* TODO: could be more smart! Use mapped spice_model of SRAM ports!  
   * Support Non-volatile RRAM-based SRAM */
  for (iport = 0; iport < num_sram_port; iport++) {
    switch (sram_ports[iport]->spice_model->design_tech) {
    case SPICE_MODEL_DESIGN_RRAM:
    /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
     * In memory bank, by intensively share the Bit/Word Lines,
     * we only need 1 additional BL and WL for each memory bit.
     * Number of memory bits is still same as CMOS SRAM
     */
      num_reserved_conf_bits = 
        count_num_reserved_conf_bits_one_rram_sram_spice_model(sram_ports[iport]->spice_model,
                                                               cur_sram_orgz_type);
      break;
    case SPICE_MODEL_DESIGN_CMOS:
      num_reserved_conf_bits = 0;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
                 __FILE__, __LINE__, cur_spice_model->name); 
      exit(1);
    }
  }

  /* Free */
  my_free(sram_ports);

  return num_reserved_conf_bits;
}

/* For a multiplexer, determine its reserved configuration bits */
int count_num_reserved_conf_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type,
                                                     int mux_size) {
  int num_reserved_conf_bits = 0;
  int num_input_size = mux_size;

  /* Check */
  assert(SPICE_MODEL_MUX == cur_spice_model->type);
  assert((2 == mux_size)||(2 < mux_size));

  num_input_size = get_mux_full_input_size(cur_spice_model, mux_size);

  /* Number of configuration bits depends on the MUX structure */
  switch (cur_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_reserved_conf_bits = 2;
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    num_reserved_conf_bits = num_input_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_reserved_conf_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level * 
                             determine_num_input_basis_multilevel_mux(num_input_size, 
                             cur_spice_model->design_tech_info.mux_info->mux_num_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name);
    exit(1);
  }
  /* For 2:1 MUX, whatever structure, there is only one level */
  if (2 == num_input_size) {
    num_reserved_conf_bits = 2;
  }
  /* Also the number of configuration bits depends on the technology*/
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
    switch (cur_sram_orgz_type) {
    case SPICE_SRAM_MEMORY_BANK:
     /* In memory bank, by intensively share the Bit/Word Lines,
      * we only need 1 additional BL and WL for each MUX level.
      */
      /* For 2:1 MUX, whatever structure, there is only one level */
      if (2 == num_input_size) {
        num_reserved_conf_bits = 2;
      } 
      break;
    case SPICE_SRAM_SCAN_CHAIN:
    case SPICE_SRAM_STANDALONE:
      num_reserved_conf_bits = 0;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
                 __FILE__, __LINE__); 
      exit(1);
    }
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    num_reserved_conf_bits = 0;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  return num_reserved_conf_bits;
}


int count_num_reserved_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                                 enum e_sram_orgz cur_sram_orgz_type,
                                                 int mux_size) {
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;
  int iport;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    num_reserved_conf_bits = 
      count_num_reserved_conf_bits_one_lut_spice_model(cur_spice_model,
                                                       cur_sram_orgz_type);
    break;
  case SPICE_MODEL_MUX:
    num_reserved_conf_bits = 
      count_num_reserved_conf_bits_one_mux_spice_model(cur_spice_model,
                                                       cur_sram_orgz_type,
                                                       mux_size);
    break;
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_CCFF:
  case SPICE_MODEL_IOPAD:
    /* Other block, we just count the number SRAM ports defined by user */
    num_reserved_conf_bits = 0;
    sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
    /* TODO: could be more smart! 
     * Support Non-volatile RRAM-based SRAM */
    if (0 < num_sram_port) {
      assert(NULL != sram_ports);
      for (iport = 0; iport < num_sram_port; iport++) {
        assert(NULL != sram_ports[iport]->spice_model);
        /* TODO: could be more smart! 
         * Support Non-volatile RRAM-based SRAM */
        switch (sram_ports[iport]->spice_model->design_tech) {
        case SPICE_MODEL_DESIGN_RRAM:
        /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
         * Number of memory bits is still same as CMOS SRAM
         */
          temp_num_reserved_conf_bits = 
            count_num_reserved_conf_bits_one_rram_sram_spice_model(sram_ports[iport]->spice_model,
                                                                   cur_sram_orgz_type);
          if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
            num_reserved_conf_bits = temp_num_reserved_conf_bits;
          }
          break;
        case SPICE_MODEL_DESIGN_CMOS:
          num_reserved_conf_bits = 0;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
                     __FILE__, __LINE__, cur_spice_model->name); 
          exit(1);
        }
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Free */
  my_free(sram_ports);

  return num_reserved_conf_bits;
}

static 
int count_num_conf_bits_one_lut_spice_model(t_spice_model* cur_spice_model,
                                            enum e_sram_orgz cur_sram_orgz_type) {
  int num_conf_bits = 0;
  int iport;
  int lut_size;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  int lut_sram_port = 0;
  int mode_sram_port = 0;

  assert(NULL != cur_spice_model);
  assert(SPICE_MODEL_LUT == cur_spice_model->type);

  /* Determine size of LUT*/
  input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_input_port);
  lut_size = input_ports[0]->size;

  /* Check lut sram bits and mode bits */
  num_conf_bits = 0;
  lut_sram_port = 0;
  mode_sram_port = 0;
  /* Total SRAM bit count = LUT SRAM bit + Mode bit */
  for (iport = 0; iport < num_sram_port; iport++) {
    if (FALSE == sram_ports[iport]->mode_select) {
      num_conf_bits += (int)pow(2.,(double)(lut_size));
      assert(num_conf_bits == sram_ports[iport]->size);
      lut_sram_port++;
    } else { 
      assert (TRUE == sram_ports[iport]->mode_select);
      num_conf_bits += sram_ports[iport]->size;
      mode_sram_port++;
    }
  }
  assert (1 == lut_sram_port);
  assert ((0 == mode_sram_port) || (1 == mode_sram_port));

  /* TODO: could be more smart! Use mapped spice_model of SRAM ports!  
   * Support Non-volatile RRAM-based SRAM */
  switch (sram_ports[0]->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
  /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
   * In memory bank, by intensively share the Bit/Word Lines,
   * we only need 1 additional BL and WL for each memory bit.
   * Number of memory bits is still same as CMOS SRAM
   */
    switch (cur_sram_orgz_type) {
    case SPICE_SRAM_MEMORY_BANK:
      break;
    case SPICE_SRAM_SCAN_CHAIN:
    case SPICE_SRAM_STANDALONE:
      num_conf_bits = 2 * num_conf_bits;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
                 __FILE__, __LINE__); 
      exit(1);
    }
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  /* Free */
  my_free(input_ports);
  my_free(sram_ports);

  return num_conf_bits;
}

static 
int count_num_conf_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                            enum e_sram_orgz cur_sram_orgz_type,
                                            int mux_size) {
  int num_conf_bits = 0;
  int num_input_size = mux_size;

  assert(NULL != cur_spice_model);
  assert(SPICE_MODEL_MUX == cur_spice_model->type);

  assert((2 == mux_size)||(2 < mux_size));

  num_input_size = get_mux_full_input_size(cur_spice_model, mux_size);

  /* Number of configuration bits depends on the MUX structure */
  switch (cur_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_conf_bits = determine_tree_mux_level(num_input_size);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    num_conf_bits = num_input_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_conf_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level
                    * determine_num_input_basis_multilevel_mux(num_input_size, 
                      cur_spice_model->design_tech_info.mux_info->mux_num_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name);
    exit(1);
  }
  /* For 2:1 MUX, whatever structure, there is only one level */
  if (2 == num_input_size) {
    num_conf_bits = 1;
  }
  /* Also the number of configuration bits depends on the technology*/
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
    switch (cur_sram_orgz_type) {
    case SPICE_SRAM_MEMORY_BANK:
     /* In memory bank, by intensively share the Bit/Word Lines,
      * we only need 1 additional BL and WL for each MUX level.
      */
      num_conf_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level;
      /* For 2:1 MUX, whatever structure, there is only one level */
      if (2 == num_input_size) {
        num_conf_bits = 1;
      } 
      break;
    case SPICE_SRAM_SCAN_CHAIN:
    case SPICE_SRAM_STANDALONE:
      /* Currently we keep the same as CMOS MUX */
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
                 __FILE__, __LINE__); 
      exit(1);
    }
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    /* When a local encoder is added, the number of sram bits will be reduced
     * to N * log_2{X}, where N is the number of levels and X is the number of inputs per level
     * Note that: we only apply this to one-level and multi-level multiplexers
     */
    if ( (TRUE == cur_spice_model->design_tech_info.mux_info->local_encoder) 
        && (2 < num_input_size) ) { 
      if (SPICE_MODEL_STRUCTURE_ONELEVEL == cur_spice_model->design_tech_info.mux_info->structure) {
        num_conf_bits = determine_mux_local_encoder_num_inputs(num_conf_bits);
      } else if (SPICE_MODEL_STRUCTURE_MULTILEVEL == cur_spice_model->design_tech_info.mux_info->structure) {
        num_conf_bits = cur_spice_model->design_tech_info.mux_info->mux_num_level * determine_mux_local_encoder_num_inputs(num_conf_bits / cur_spice_model->design_tech_info.mux_info->mux_num_level);
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  /* Free */

  return num_conf_bits;
}

static 
int count_num_conf_bits_one_generic_spice_model(t_spice_model* cur_spice_model,
                                                enum e_sram_orgz cur_sram_orgz_type) {
  int num_conf_bits = 0;
  int iport;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  assert(NULL != cur_spice_model);

  /* Other block, we just count the number SRAM ports defined by user */
  num_conf_bits = 0;
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  /* TODO: could be more smart! 
   * Support Non-volatile RRAM-based SRAM */
  if (0 < num_sram_port) {
    assert(NULL != sram_ports);
    for (iport = 0; iport < num_sram_port; iport++) {
      assert(NULL != sram_ports[iport]->spice_model);
      /* TODO: could be more smart! 
       * Support Non-volatile RRAM-based SRAM */
      switch (sram_ports[iport]->spice_model->design_tech) {
      case SPICE_MODEL_DESIGN_RRAM:
      /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
      * Number of memory bits is still same as CMOS SRAM
       */
        switch (cur_sram_orgz_type) {
        case SPICE_SRAM_MEMORY_BANK:
          num_conf_bits += sram_ports[iport]->size;
          break;
        case SPICE_SRAM_SCAN_CHAIN:
        case SPICE_SRAM_STANDALONE:
          num_conf_bits += sram_ports[iport]->size;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
                     __FILE__, __LINE__); 
          exit(1);
        }
        break;
      case SPICE_MODEL_DESIGN_CMOS:
       /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
        * Number of memory bits is still same as CMOS SRAM
        */
        switch (cur_sram_orgz_type) {
        case SPICE_SRAM_MEMORY_BANK:
          num_conf_bits += sram_ports[iport]->size;
          break;
        case SPICE_SRAM_SCAN_CHAIN:
        case SPICE_SRAM_STANDALONE:
          num_conf_bits += sram_ports[iport]->size;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid type of SRAM organization!\n",
                     __FILE__, __LINE__); 
          exit(1);
        }

        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of LUT(name: %s)\n",
                 __FILE__, __LINE__, cur_spice_model->name); 
        exit(1);
      }
    }
  }

  /* Free */
  my_free(sram_ports);

  return num_conf_bits;
}



/* Count the number of configuration bits of a spice model */
int count_num_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        enum e_sram_orgz cur_sram_orgz_type,
                                        int mux_size) {
  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    return count_num_conf_bits_one_lut_spice_model(cur_spice_model, cur_sram_orgz_type);
  case SPICE_MODEL_MUX:
    return count_num_conf_bits_one_mux_spice_model(cur_spice_model, cur_sram_orgz_type, mux_size);
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_CCFF:
  case SPICE_MODEL_IOPAD:
    return count_num_conf_bits_one_generic_spice_model(cur_spice_model, cur_sram_orgz_type);
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }
  return -1;
}

int count_num_reserved_conf_bit_one_interc(t_interconnect* cur_interc,
                                           enum e_sram_orgz cur_sram_orgz_type) {
  int fan_in = 0;
  enum e_interconnect spice_interc_type = DIRECT_INTERC;

  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  if (NULL == cur_interc) { 
    return num_reserved_conf_bits;
  } else {
    fan_in = cur_interc->fan_in;
    if (0 == fan_in) {
      return num_reserved_conf_bits;
    }
  }
  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  switch (cur_interc->type) {
  case DIRECT_INTERC:
    assert(cur_interc->fan_out == fan_in);
    spice_interc_type = DIRECT_INTERC;
    break;
  case COMPLETE_INTERC:
    if (1 == fan_in) {
      spice_interc_type = DIRECT_INTERC;
    } else {
      assert((2 == fan_in)||(2 < fan_in));
      spice_interc_type = MUX_INTERC;
    }
    break;
  case MUX_INTERC:
    assert((2 == fan_in)||(2 < fan_in));
    spice_interc_type = MUX_INTERC;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  /* This time, (2nd round), count the number of configuration bits, according to interc type*/ 
  switch (spice_interc_type) {
  case DIRECT_INTERC:
    /* Check : 
     * 1. Direct interc has only one fan-in!
     */
    assert((cur_interc->fan_out == fan_in)
         ||((COMPLETE_INTERC == cur_interc->type)&&(1 == fan_in)));
    break;
  case COMPLETE_INTERC:
  case MUX_INTERC:
    /* Check : 
     * MUX should have at least 2 fan_in
     */
    assert((2 == fan_in)||(2 < fan_in));
    assert((1 == cur_interc->fan_out)||(1 < cur_interc->fan_out));
    /* 2. spice_model is a wire */ 
    assert(NULL != cur_interc->spice_model);
    assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
    temp_num_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(cur_interc->spice_model, 
                                                                          cur_sram_orgz_type, fan_in);
    /* FOR COMPLETE_INTERC: we should consider fan_out number ! */
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  return num_reserved_conf_bits;
}

/* add configuration bits of a MUX to linked-list
 * when SRAM organization type is scan-chain */
void  
add_mux_ccff_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model) {
  int ibit, cur_mem_bit;
  t_conf_bit** sram_bit = NULL;
 
  /* Assert*/
  assert(NULL != cur_sram_orgz_info);
  assert(NULL != mux_spice_model);

  cur_mem_bit = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  /* Depend on the design technology of mux_spice_model
   * Fill the conf_bits information  */
  switch (mux_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
  case SPICE_MODEL_DESIGN_RRAM:
    /* Count how many configuration bits need to program
     * Scan-chain needs to know each memory bit whatever it is 0 or 1
     */
    /* Allocate the array */
    sram_bit = (t_conf_bit**)my_malloc(num_mux_sram_bits * sizeof(t_conf_bit*));
    for (ibit = 0; ibit < num_mux_sram_bits; ibit++) {
      sram_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
    }
    /* Fill the array: sram_bit */
    for (ibit = 0; ibit < num_mux_sram_bits; ibit++) {
      sram_bit[ibit]->addr = cur_mem_bit + ibit;
      sram_bit[ibit]->val = mux_sram_bits[ibit]; 
    }
    break; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid design technology!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  /* Fill the linked list */
  for (ibit = 0; ibit < num_mux_sram_bits; ibit++) {
    cur_sram_orgz_info->conf_bit_head =
        add_conf_bit_info_to_llist(cur_sram_orgz_info->conf_bit_head, cur_mem_bit + ibit, 
                                   sram_bit[ibit], NULL, NULL,
                                   mux_spice_model);
  }

  /* Free */
  my_free(sram_bit);

  return;
}

/* add configuration bits of a MUX to linked-list
 * when SRAM organization type is scan-chain */
void  
add_mux_membank_conf_bits_to_llist(int mux_size,
                                   t_sram_orgz_info* cur_sram_orgz_info, 
                                   int num_mux_sram_bits, int* mux_sram_bits,
                                   t_spice_model* mux_spice_model) {
  int ibit, cur_mem_bit, num_conf_bits, cur_bit, cur_bl, cur_wl;
  int ilevel;
  int num_bl_enabled, num_wl_enabled;
  t_conf_bit** wl_bit = NULL;
  t_conf_bit** bl_bit = NULL;
 
  /* Assert*/
  assert(NULL != cur_sram_orgz_info);
  assert(NULL != mux_spice_model);

  cur_mem_bit = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);

  /* Depend on the design technology of mux_spice_model
   * Fill the conf_bits information  */
  switch (mux_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Count how many configuration bits need to program
     * Assume all the SRAMs are zero initially.
     * only Configuration to bit 1 requires a programming operation    
     */
    num_conf_bits = num_mux_sram_bits;
    /* Allocate the array */
    bl_bit = (t_conf_bit**)my_malloc(num_mux_sram_bits * sizeof(t_conf_bit*));
    wl_bit = (t_conf_bit**)my_malloc(num_mux_sram_bits * sizeof(t_conf_bit*));
    for (ibit = 0; ibit < num_mux_sram_bits; ibit++) {
      bl_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
      wl_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
    }
    /* SRAMs are typically organized in an array where BLs and WLs are efficiently shared
     * Actual BL/WL address in the array is hard to predict here,
     * they will be handled in the top_netlist and top_testbench generation 
     */
    for (ibit = 0; ibit < num_mux_sram_bits; ibit++) {
      bl_bit[ibit]->addr = cur_mem_bit + ibit;
      bl_bit[ibit]->val = mux_sram_bits[ibit];
      wl_bit[ibit]->addr = cur_mem_bit + ibit; 
      wl_bit[ibit]->val = 1; /* We always assume WL is the write enable signal of a SRAM */ 
    }
    break; 
  case SPICE_MODEL_DESIGN_RRAM:
    /* Count how many configuration bits need to program
     * only BL and WL are both 1 requires a programming operation    
     * Each level of a MUX requires 1 RRAM to be configured.
     * Therefore, the number of configuration bits should be num_mux_levels
     */
    num_bl_enabled = 0;
    /* Check how many Bit lines are 1 */
    for (ibit = 0; ibit < num_mux_sram_bits/2; ibit++) {
      if (1 == mux_sram_bits[ibit]) {
        num_bl_enabled++;
      }
    }
    num_wl_enabled = 0;
    /* Check how many Word lines are 1 */
    for (ibit = 0; ibit < num_mux_sram_bits/2; ibit++) {
      if (1 == mux_sram_bits[ibit + num_mux_sram_bits/2]) {
        num_wl_enabled++;
      }
    }
    /* The number of enabled Bit and Word lines should be the same */
    assert(num_bl_enabled == num_wl_enabled);
    /* Assign num_conf_bits */
    num_conf_bits = num_bl_enabled;
    /* Allocate the array */
    bl_bit = (t_conf_bit**)my_malloc(num_conf_bits * sizeof(t_conf_bit*));
    wl_bit = (t_conf_bit**)my_malloc(num_conf_bits * sizeof(t_conf_bit*));
    for (ibit = 0; ibit < num_conf_bits; ibit++) {
      bl_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
      wl_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
    }
    /* For one-level RRAM MUX: 
     * There should be only 1 BL and 1 WL whose value is 1
     * First half of mux_sram_bits are BL, the rest are WL
     * For multi-level RRAM MUX:
     * There could be more than 1 BL and 1 WL whose value is 1 
     * We need to divde the mux_sram_bits into small part,
     * each part has only 1 BL and 1 WL whose value is 1
     */
    /* Assign bit lines address and values */
    cur_bit = 0;
    /* We slice the BL part of array mux_sram_bits to N=num_conf_bits parts */
    for (ilevel = 0; ilevel < num_conf_bits; ilevel++) {
      for (ibit = ilevel * num_mux_sram_bits/(2*num_conf_bits); /* Start address of each slice*/ 
           ibit < (ilevel + 1) * num_mux_sram_bits/(2*num_conf_bits); /* End address of each slice*/ 
           ibit++) {
        if (0 == mux_sram_bits[ibit]) {
          continue; /* Skip non-zero bits */
        }
        assert(1 == mux_sram_bits[ibit]);
        if (ibit == (ilevel + 1) * num_mux_sram_bits/(2*num_conf_bits) - 1) { 
          bl_bit[cur_bit]->addr = cur_bl + ilevel;
        /* Last conf_bit should use a new BL/WL */
        } else {
        /* First part of conf_bit should use reserved BL/WL */
          bl_bit[cur_bit]->addr = ibit;
        }
        bl_bit[cur_bit]->val = mux_sram_bits[ibit];
        cur_bit++;
      } 
    }
    assert(num_conf_bits == cur_bit);
    /* Assign Word lines address and values */
    cur_bit = 0;
    for (ilevel = 0; ilevel < num_conf_bits; ilevel++) {
      for (ibit = num_mux_sram_bits/2 + ilevel * num_mux_sram_bits/(2*num_conf_bits); /* Start address of each slice*/ 
           ibit < num_mux_sram_bits/2 + (ilevel + 1) * num_mux_sram_bits/(2*num_conf_bits); /* End address of each slice*/ 
           ibit++) {
        if (0 == mux_sram_bits[ibit]) {
          continue; /* Skip non-zero bits */
        }
        assert(1 == mux_sram_bits[ibit]);
        if (ibit == num_mux_sram_bits/2 + (ilevel + 1) * num_mux_sram_bits/(2*num_conf_bits) - 1) { 
          wl_bit[cur_bit]->addr = cur_wl + ilevel;
        /* Last conf_bit should use a new BL/WL */
        } else {
        /* First part of conf_bit should use reserved BL/WL */
          wl_bit[cur_bit]->addr = ibit;
        }
        wl_bit[cur_bit]->val = mux_sram_bits[ibit];
        cur_bit++;
      } 
    }
    assert(num_conf_bits == cur_bit);
    break; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid design technology!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  /* Fill the linked list */
  for (ibit = 0; ibit < num_conf_bits; ibit++) {
    cur_sram_orgz_info->conf_bit_head =
        add_conf_bit_info_to_llist(cur_sram_orgz_info->conf_bit_head, cur_mem_bit + ibit, 
                                   NULL, bl_bit[ibit], wl_bit[ibit],
                                   mux_spice_model);
  }

  /* Free */
  my_free(bl_bit);
  my_free(wl_bit);

  return;
}

/* Should we return a value ? */
void  
add_mux_conf_bits_to_llist(int mux_size,
                           t_sram_orgz_info* cur_sram_orgz_info, 
                           int num_mux_sram_bits, int* mux_sram_bits,
                           t_spice_model* mux_spice_model) {
  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    add_mux_ccff_conf_bits_to_llist(mux_size, cur_sram_orgz_info, 
                                    num_mux_sram_bits, mux_sram_bits, 
                                    mux_spice_model);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    add_mux_membank_conf_bits_to_llist(mux_size, cur_sram_orgz_info, 
                                      num_mux_sram_bits, mux_sram_bits, 
                                      mux_spice_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Add CCFF configutration bits to a linked list*/
static 
void add_sram_ccff_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, 
                                      int num_sram_bits, int* sram_bits) {
  int ibit, cur_mem_bit;
  t_conf_bit** sram_bit = NULL;
  t_spice_model* cur_sram_spice_model = NULL;
 
  /* Assert*/
  assert(NULL != cur_sram_orgz_info);

  cur_mem_bit = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &cur_sram_spice_model);
  assert(NULL != cur_sram_spice_model);

  /* Count how many configuration bits need to program
   * Scan-chain needs to know each memory bit whatever it is 0 or 1
   */
  /* Allocate the array */
  sram_bit = (t_conf_bit**)my_malloc(num_sram_bits * sizeof(t_conf_bit*));
  for (ibit = 0; ibit < num_sram_bits; ibit++) {
    sram_bit[ibit] = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
  }
  /* Fill the array: sram_bit */
  for (ibit = 0; ibit < num_sram_bits; ibit++) {
    sram_bit[ibit]->addr = cur_mem_bit + ibit;
    sram_bit[ibit]->val = sram_bits[ibit]; 
  }

  /* Fill the linked list */
  for (ibit = 0; ibit < num_sram_bits; ibit++) {
    cur_sram_orgz_info->conf_bit_head =
        add_conf_bit_info_to_llist(cur_sram_orgz_info->conf_bit_head, cur_mem_bit + ibit, 
                                   sram_bit[ibit], NULL, NULL,
                                   cur_sram_spice_model);
  }

  /* Free */
  my_free(sram_bit);

  return;
}


/* Add SRAM configuration bits in memory bank organization to a linked list */
void add_sram_membank_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                                         int num_bls, int num_wls, 
                                         int* bl_conf_bits, int* wl_conf_bits) {
  int ibit, cur_bl, cur_wl;
  t_spice_model* cur_sram_spice_model = NULL;
  t_conf_bit* bl_bit = NULL;
  t_conf_bit* wl_bit = NULL;
  int bit_cnt = 0;

  /* Assert*/
  assert(NULL != cur_sram_orgz_info);

  /* Get current counter of sram_spice_model */
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);

  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &cur_sram_spice_model);
  assert(NULL != cur_sram_spice_model);

  /* Malloc */
  bl_bit = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));
  wl_bit = (t_conf_bit*)my_malloc(sizeof(t_conf_bit));

  /* Depend on the memory technology, we have different configuration bits */
  switch (cur_sram_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS: 
    assert((1 == num_bls)&&(1 == num_wls));
    bl_bit->addr = mem_index;
    wl_bit->addr = mem_index;
    bl_bit->val = bl_conf_bits[0];
    wl_bit->val = wl_conf_bits[0];
    break;
  case SPICE_MODEL_DESIGN_RRAM: 
    /* Fill information */
    bit_cnt = 0; /* Check counter */
    for (ibit = 0; ibit < num_bls; ibit++) {
      /* Bypass zero bit */
      if (0 == bl_conf_bits[ibit]) {
        continue;
      }
      /* Check if this bit is in reserved bls */
      if (ibit == num_bls - 1) { 
        /* Last bit is always independent */
        bl_bit->addr = mem_index;
        bl_bit->val = 1;
      } else {
        /* Other bits are shared */
        bl_bit->addr = ibit;
        bl_bit->val = 1;
      }
      /* Update check counter */
      bit_cnt++;
    }
    /* Check */
    assert(1 == bit_cnt);
  
    bit_cnt = 0; /* Check counter */
    for (ibit = 0; ibit < num_wls; ibit++) {
      /* Bypass zero bit */
      if (0 == wl_conf_bits[ibit]) {
        continue;
      }
      /* Check if this bit is in reserved bls */
      if (ibit == num_wls - 1) { 
        /* Last bit is always independent */
        wl_bit->addr = mem_index;
        wl_bit->val = 1;
      } else {
        /* Other bits are shared */
        wl_bit->addr = ibit;
        wl_bit->val = 1;
      }
      /* Update check counter */
      bit_cnt++;
    }
    /* Check */
    assert(1 == bit_cnt);
    break; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid design technology!",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Fill the linked list */
  cur_sram_orgz_info->conf_bit_head =
      add_conf_bit_info_to_llist(cur_sram_orgz_info->conf_bit_head, mem_index, 
                                 NULL, bl_bit, wl_bit,
                                 cur_sram_spice_model);

  return;
}

/* Add SRAM configuration bits to a linked list */
void  
add_sram_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, int mem_index, 
                            int num_sram_bits, int* sram_bits) {
  int num_bls, num_wls;
  int* bl_conf_bits = NULL;
  int* wl_conf_bits = NULL;

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    add_sram_ccff_conf_bits_to_llist(cur_sram_orgz_info, 
                                     num_sram_bits, sram_bits);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Initialize parameters */
    /* Number of BLs should be same as WLs */
    num_bls = num_sram_bits/2;
    num_wls = num_sram_bits/2;
    /* Convention: first part of Array (sram_bits) is BL configuration bits,
     * second part is WL configuration bits.
     */
    bl_conf_bits = sram_bits;
    wl_conf_bits = sram_bits + num_bls;
    add_sram_membank_conf_bits_to_llist(cur_sram_orgz_info, mem_index, 
                                        num_bls, num_wls, 
                                        bl_conf_bits, wl_conf_bits);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}


/* Decode BL and WL bits for a SRAM
 * SRAM could be
 * 1. NV SRAM
 * or
 * 2. SRAM 
 */
void decode_memory_bank_sram(t_spice_model* cur_sram_spice_model, int sram_bit,
                             int bl_len, int wl_len, int bl_offset, int wl_offset,
                             int* bl_conf_bits, int* wl_conf_bits) {
  int i;

  /* Check */
  assert(NULL != cur_sram_spice_model);
  assert(NULL != bl_conf_bits);
  assert(NULL != wl_conf_bits);
  assert((1 == sram_bit)||(0 == sram_bit));

  /* All the others should be zero */
  for (i = 0; i < bl_len; i++) {
    bl_conf_bits[i] = 0;
  }
  for (i = 0; i < wl_len; i++) {
    wl_conf_bits[i] = 0;
  }
  
  /* Depending on the design technology of SRAM */
  switch (cur_sram_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* CMOS SRAM */
    /* Make sure there is only 1 BL and 1 WL */
    assert((1 == bl_len)&&(1 == wl_len));
    /* We always assume that WL is a write-enable signal
     * While BL contains what data will be written into SRAM 
     */
    bl_conf_bits[0] = sram_bit;
    wl_conf_bits[0] = 1;
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* NV SRAM (RRAM-based) */
    /* We need at least 2 BLs and 2 WLs but no more than 3, See schematic in manual */
    /* Whatever the number of BLs and WLs, (RRAM0)
     * when sram bit is 1, last bit of BL should be enabled
     * while first bit of WL should be enabled at the same time
     * when sram bit is 0, last bit of WL should be enabled
     * while first bit of BL should be enabled at the same time
     */
    assert((1 < bl_len)&&(bl_len < 4)); 
    assert((1 < wl_len)&&(wl_len < 4)); 
    assert((-1 < bl_offset)&&(bl_offset < bl_len));
    assert((-1 < wl_offset)&&(wl_offset < wl_len));
    /* In addition, we will may need two programing cycles.
     * The first cycle is dedicated to programming RRAM0
     * The second cycle is dedicated to programming RRAM1
     */
    if (1 == sram_bit) {
      bl_conf_bits[bl_len-1] = 1;
      wl_conf_bits[0 + wl_offset] = 1;
    } else {
      assert(0 == sram_bit);
      bl_conf_bits[0 + bl_offset] = 1;
      wl_conf_bits[wl_len-1] = 1;
    } 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for SRAM!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Decode one SRAM bit for memory-bank-style configuration circuit, and add it to linked list */
void 
decode_and_add_sram_membank_conf_bit_to_llist(t_sram_orgz_info* cur_sram_orgz_info,
                                              int mem_index,
                                              int num_bl_per_sram, int num_wl_per_sram, 
                                              int cur_sram_bit) {
  int j;
  int* conf_bits_per_sram = NULL;
  t_spice_model* mem_model = NULL;

  /* Check */
  assert( SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type );

  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Malloc/Calloc */
  conf_bits_per_sram = (int*)my_calloc(num_bl_per_sram + num_wl_per_sram, sizeof(int));

  /* Depend on the memory technology, we have different configuration bits */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS: 
    /* Check */
    assert((1 == num_bl_per_sram) && (1 == num_wl_per_sram));
    /* For CMOS SRAM */
    decode_memory_bank_sram(mem_model, cur_sram_bit, 
                                    num_bl_per_sram, num_wl_per_sram, 0, 0, 
                                    conf_bits_per_sram, conf_bits_per_sram + num_bl_per_sram);
    /* Use memory model here! Design technology of memory model determines the decoding strategy, instead of LUT model*/
    add_sram_conf_bits_to_llist(cur_sram_orgz_info, mem_index, 
                                num_bl_per_sram + num_wl_per_sram, conf_bits_per_sram); 
    break; 
  case SPICE_MODEL_DESIGN_RRAM: 
    /* Decode the SRAM bits to BL/WL bits.
     * first half part is BL, the other half part is WL 
     */
    /* Store the configuraion bit to linked-list */
    assert(num_bl_per_sram == num_wl_per_sram);
    /* When the number of BL/WL is more than 1, we need multiple programming cycles to configure a SRAM */
    /* ONLY valid for NV SRAM !!!*/
    for (j = 0; j < num_bl_per_sram - 1; j++) { 
      if (0 == j) {
        /* Store the configuraion bit to linked-list */
        decode_memory_bank_sram(mem_model, cur_sram_bit, 
                                        num_bl_per_sram, num_wl_per_sram, j, j, 
                                        conf_bits_per_sram, conf_bits_per_sram + num_bl_per_sram);
       } else {
        /* Store the configuraion bit to linked-list */
        decode_memory_bank_sram(mem_model, 1 - cur_sram_bit, 
                                        num_bl_per_sram, num_wl_per_sram, j, j, 
                                        conf_bits_per_sram, conf_bits_per_sram + num_bl_per_sram);
      }
      /* Use memory model here! Design technology of memory model determines the decoding strategy, instead of LUT model*/
      add_sram_conf_bits_to_llist(cur_sram_orgz_info, mem_index, 
                                  num_bl_per_sram + num_wl_per_sram, conf_bits_per_sram); 
    }
    break; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid design technology!",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Free */
  my_free(conf_bits_per_sram);
 
  return;
}

void determine_blwl_decoder_size(INP t_sram_orgz_info* cur_sram_orgz_info,
                                 OUTP int* num_array_bl, OUTP int* num_array_wl,
                                 OUTP int* bl_decoder_size, OUTP int* wl_decoder_size) {
  t_spice_model* mem_model = NULL;
  int num_reserved_bl, num_reserved_wl;

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type);

  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, num_array_bl, num_array_wl);
  get_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, &num_reserved_bl, &num_reserved_wl);

  /* Sizes of decodes depend on the Memory technology */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model); 
  switch (mem_model->design_tech) {
  /* CMOS SRAM*/
  case SPICE_MODEL_DESIGN_CMOS:
   /* SRAMs can efficiently share BLs and WLs, 
    * Actual number of BLs and WLs will be sqrt(num_bls) and sqrt(num_wls) 
    */
    assert(0 == num_reserved_bl);
    assert(0 == num_reserved_wl);
    (*num_array_bl) = ceil(sqrt(*num_array_bl));
    (*num_array_wl) = ceil(sqrt(*num_array_wl));
    (*bl_decoder_size) = determine_decoder_size(*num_array_bl);
    (*wl_decoder_size) = determine_decoder_size(*num_array_wl);
    break;
  /* RRAM */
  case SPICE_MODEL_DESIGN_RRAM:
    /* Currently we do not have more efficient way to share the BLs and WLs as CMOS SRAMs */
    (*bl_decoder_size) = determine_decoder_size(*num_array_bl);
    (*wl_decoder_size) = determine_decoder_size(*num_array_wl);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology [CMOS|RRAM] for memory technology!\n",
               __FILE__, __LINE__);
    exit(1);
 }

  return;
}

void init_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                       int num_switch,
                                       t_switch_inf* switches,
                                       t_spice* spice,
                                       t_det_routing_arch* routing_arch) {

  /* We have linked list whichs stores spice model information of multiplexer*/
  t_llist* muxes_head = NULL; 
  t_llist* temp = NULL;
  t_spice_mux_model* cur_spice_mux_model = NULL;
  int max_routing_mux_size = -1;

  /* Alloc the muxes*/
  muxes_head = stats_spice_muxes(num_switch, switches, spice, routing_arch);

  temp = muxes_head;
  while(temp) {
    assert(NULL != temp->dptr);
    cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);
    /* Exclude LUT MUX from this statistics */
    if ((SPICE_MODEL_MUX == cur_spice_mux_model->spice_model->type)
       &&((-1 == max_routing_mux_size)||(max_routing_mux_size < cur_spice_mux_model->size))) {
      max_routing_mux_size = cur_spice_mux_model->size;
    }
    /* Move on to the next*/
    temp = temp->next;
  }

  try_update_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, 
                                          max_routing_mux_size, max_routing_mux_size);

  vpr_printf(TIO_MESSAGE_INFO,"Detected %d reserved BLs and% d reserved WLs...\n", 
             max_routing_mux_size, max_routing_mux_size);

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);
  
  return;
}

void add_one_conf_bit_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info) {
  int cur_num_sram = 0;
  int cur_bl, cur_wl;

  /* Get current index of SRAM module */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl); 
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + 1,
                                   cur_wl + 1);
    break;
  case SPICE_SRAM_STANDALONE:
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

void add_sram_conf_bits_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info, 
                                          t_spice_model* cur_spice_model) {
  int i;
  int num_sram; 

  /* Synchronize the internal counters of sram_orgz_info with generated bitstreams*/
  num_sram = count_num_sram_bits_one_spice_model(cur_spice_model, -1);
  for (i = 0; i < num_sram; i++) {
    add_one_conf_bit_to_sram_orgz_info(cur_sram_orgz_info); /* use the mem_model in cur_sram_orgz_info */
  }

  return;
}

void add_mux_conf_bits_to_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                                         t_spice_model* mux_spice_model, int mux_size) {
  int i;
  int num_mux_sram_bits, num_mux_conf_bits;
  int cur_num_sram, cur_bl, cur_wl;

  /* cur_num_sram = sram_verilog_model->cnt; */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);

  /* Get the number of configuration bits required by this MUX */
  num_mux_sram_bits = count_num_sram_bits_one_spice_model(mux_spice_model, mux_size);

  num_mux_conf_bits = count_num_conf_bits_one_spice_model(mux_spice_model, 
                                                          cur_sram_orgz_info->type,
                                                          mux_size);
 
  /* Synchronize sram_orgz_info by incrementing its internal counters */
  switch (mux_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* SRAM-based MUX required dumping SRAMs! */
    for (i = 0; i < num_mux_sram_bits; i++) {
      add_one_conf_bit_to_sram_orgz_info(cur_sram_orgz_info); /* use the mem_model in sram_verilog_orgz_info */
    }
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM-based MUX does not need any SRAM dumping
     * But we have to get the number of configuration bits required by this MUX 
     * and update the number of memory bits 
     */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + num_mux_conf_bits, 
                                   cur_wl + num_mux_conf_bits);
    break;  
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
  }

  return;
}

/************************************************************************
 * End of file : fpga_x2p_bitstream_utils.c
 ***********************************************************************/
