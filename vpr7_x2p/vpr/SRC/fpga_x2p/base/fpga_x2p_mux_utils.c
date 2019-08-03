/***********************************/
/*      SPICE Modeling for VPR     */
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
#include "route_common.h"

/* Include SPICE support headers*/
#include "quicksort.h"
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"

#include "fpga_x2p_mux_utils.h"

/* Determine the number of SRAM bit for a basis subckt of a multiplexer
 * In general, the number of SRAM bits should be same as the number of inputs per level
 * with one exception: 
 * When multiplexing structure is tree-like, there should be only 1 SRAM bit
 */
int determine_num_sram_bits_mux_basis_subckt(t_spice_model* mux_spice_model,
                                             int mux_size, 
                                             int num_input_per_level,
                                             boolean special_basis) {
  int num_sram_bits; 

  /* General cases */
  switch (mux_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_sram_bits = 1;
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    num_sram_bits = num_input_per_level;
    if (2 == num_input_per_level) { 
      num_sram_bits = 1;
    }
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_sram_bits = num_input_per_level;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
    exit(1);
  }

  /* For special cases: overide the results */
  if (TRUE == special_basis) {
    num_sram_bits = num_input_per_level;
  }

  return num_sram_bits;
}

/* Determine the level of multiplexer
 */
int determine_tree_mux_level(int mux_size) {
  int level = 0;
 
  /* Do log2(mux_size), have a basic number*/ 
  level = (int)(log((double)mux_size)/log(2.));
  /* Fix the error, i.e. mux_size=5, level = 2, we have to complete */
  while (mux_size > pow(2.,(double)level)) {
    level++;
  }

  return level;
}

int determine_num_input_basis_multilevel_mux(int mux_size,
                                             int mux_level) {
  int num_input_per_unit = 2;

  /* Special Case: mux_size = 2 */
  if (2 == mux_size) {
    return mux_size; 
  }
  
  if (1 == mux_level) {
    return mux_size;
  }  

  if (2 == mux_level) {
    num_input_per_unit = (int)sqrt(mux_size);
    while (num_input_per_unit*num_input_per_unit < mux_size) {
      num_input_per_unit++;
    }
    return num_input_per_unit;
  }

  assert(2 < mux_level);


  while(pow((double)num_input_per_unit, (double)mux_level) < mux_size) {
    num_input_per_unit++;
  }

  if (num_input_per_unit < 2) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Number of inputs of each basis should be at least 2!\n",
               __FILE__, __LINE__);
    exit(1); 
  }
  
  return num_input_per_unit;
}

/*Determine the number inputs required at the last level*/
int tree_mux_last_level_input_num(int num_level,
                                  int mux_size) {
  int ret = 0;

  ret = (int)(pow(2., (double)num_level)) - mux_size;

  if (0 < ret) {
    ret = (int)(2.*(mux_size - pow(2., (double)(num_level-1))));
  } else if (0 > ret) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])num_level(%d) is wrong with mux_size(%d)!\n",
              __FILE__, __LINE__, num_level, mux_size);
    exit(1);
  } else {
    ret = mux_size;
  }

  return ret;
}

int multilevel_mux_last_level_input_num(int num_level, int num_input_per_unit,
                                        int mux_size) {
  int ret = 0;
  int num_basis_last_level = (int)(mux_size/num_input_per_unit);
  int num_potential_special_inputs = 0;
  int num_special_basis = 0;
  int num_input_special_basis = 0;
  
  ret = mux_size - num_basis_last_level * num_input_per_unit; 
  assert((0 == ret)||(0 < ret));

  /* Special Case: mux_size = 2 */
  if (2 == mux_size) {
    return mux_size; 
  }

  if (0 < ret) {
    /* Check if we need a special basis at last level,
     * differ : the number of input of the last-2 level will be used 
     */
    num_potential_special_inputs = (num_basis_last_level + ret) -  pow((double)(num_input_per_unit), (double)(num_level-1));
    /* should be smaller than the num_input_per_unit */
    assert((!(0 > num_potential_special_inputs))&&(num_potential_special_inputs < num_input_per_unit));
    /* We need a speical basis */
    num_special_basis = pow((double)(num_input_per_unit), (double)(num_level - 1)) - num_basis_last_level;
    if (ret == num_special_basis) {
      num_input_special_basis = 0;
    } else if (1 == num_special_basis) {
      num_input_special_basis = ret;
    } else {
      assert ( 1 < num_special_basis );
      num_input_special_basis = ret - 1;
    }
    ret = num_input_special_basis + num_basis_last_level * num_input_per_unit;
  } else {
    ret = mux_size;
  }

  return ret;
}

/***************************************************************************************
 * Find the number of inputs for a encoder with a given output size
 *                     Inputs 
 *                   | | | | | 
 *                 +-----------+
 *                /             \
 *               /     Encoder   \
 *              +-----------------+
 *                | | | | | | | |
 *                  Outputs
 *               
 *  The outputs are assumes to be one-hot codes (at most only one '1' exist)
 *  Considering this fact, there are only num_of_outputs + 1 conditions to be encoded.
 *  Therefore, the number of inputs is ceil(log(num_of_outputs+1)/log(2))
 *  We plus 1, which is all-zero condition for outputs
 ***************************************************************************************/
int determine_mux_local_encoder_num_inputs(int num_outputs) {
  return ceil(log(num_outputs + 1) / log(2));
}

/* Decoding a one-level MUX:
 * SPICE/Verilog model declare the sram port sequence as follows:
 * sel0, sel1, ... , selN, 
 * which control the pass-gate logic connected to 
 * in0, in1, ... , inN
 * When decode the SRAM bits, we initialize every bits to zero
 * And then set the sel signal corresponding to the input to be 1.
 * TODO: previously the decoding is not correct, need to check any bug in SPICE part 
 */
int* decode_onelevel_mux_sram_bits(int fan_in,
                                   int mux_level,
                                   int path_id,
                                   boolean use_local_encoder) {
  /* Check */
  assert( (!(0 > path_id)) && (path_id < fan_in) );
  
  /* If we use local encoder, we have a different number of sram bits! */
  int num_sram_bits = fan_in;
  if (TRUE == use_local_encoder) {
    num_sram_bits = determine_mux_local_encoder_num_inputs(fan_in);
  }
  /* Allocate sram_bits array to return */
  int* ret = (int*)my_calloc(num_sram_bits, sizeof(int));

  if (TRUE == use_local_encoder) {
    /* The encoder will convert the path_id to a binary number 
     * For example: when path_id=3, using a 4-input encoder 
     * the sram_bits will be the 4-digit binary number of 3: 0011
     */
    ret = my_itobin_int(path_id, num_sram_bits);
  } else {
    ret[path_id] = 1;
  }
  /* ret[fan_in - 1 - path_id] = 1; */
  return ret; 
}

int* decode_multilevel_mux_sram_bits(int fan_in,
                                     int mux_level,
                                     int path_id,
                                     boolean use_local_encoder) {
  int* ret = NULL;
  int i, j, path_differ, temp;
  int num_last_level_input, active_mux_level, active_path_id, num_input_basis;
  
  /* Check */
  assert((0 == path_id)||(0 < path_id));
  assert(path_id < fan_in);
   
  /* determine the number of input of basis */
  switch (mux_level) {
  case 1:
    /* Special: 1-level should be have special care !!! */
    return decode_onelevel_mux_sram_bits(fan_in, mux_level, path_id, use_local_encoder);
  default:
    assert(1 < mux_level); 
    num_input_basis = determine_num_input_basis_multilevel_mux(fan_in, mux_level);
    break;
  }

  ret = (int*)my_malloc(sizeof(int)*(num_input_basis * mux_level));

  /* Determine last level input */
  num_last_level_input = multilevel_mux_last_level_input_num(mux_level, num_input_basis, fan_in);

  /* Initialize */
  for (i = 0; i < (num_input_basis*mux_level); i++) {
    ret[i] = 0;
  }
  
  /* When last level input number is less than the 2**mux_level,
   * There are some input at the level: (mux_level-1)
   */
  active_mux_level = mux_level; 
  active_path_id = path_id; 
  if (num_last_level_input < fan_in) {
    if (path_id > num_last_level_input - 1) {
      active_mux_level = mux_level - 1; 
      active_path_id = (int)pow((double)num_input_basis,(double)(active_mux_level)) - (fan_in - path_id); 
    }
  } else {
    assert(num_last_level_input == fan_in);
  }

  temp = active_path_id;
  for (i = mux_level - 1; i > (mux_level - active_mux_level - 1); i--) {
    for (j = 0; j < num_input_basis; j++) {
      path_differ = (j + 1) * (int)pow((double)num_input_basis,(double)(i+active_mux_level-mux_level));
      if (temp < path_differ) { 
        /* This is orignal one for SPICE, but not work for VerilogGen
         * I comment it here
         ret[i*num_input_basis + j] = 1; 
         */
        ret[(mux_level - 1 - i)*num_input_basis + j] = 1; 
        /* Reduce the min. start index of this basis */
        temp -= j * (int)pow((double)num_input_basis,(double)(i+active_mux_level-mux_level));
        break; /* Touch the boundry, stop and move onto the next level */
      }
    }
  }

  /* Check */
  assert(0 == temp);

  /* If we do not use a local encoder, these are the sram bits we want */
  if (FALSE == use_local_encoder) {
    return ret;
  }

  /* If we use local encoder, we have a different number of sram bits! */
  int num_bits_per_level = determine_mux_local_encoder_num_inputs(num_input_basis);
  int num_sram_bits = mux_level * num_bits_per_level;
  /* Allocate sram_bits array to return */
  int* encoded_ret = (int*)my_calloc(num_sram_bits, sizeof(int));

  /* Walk through each level and find the path_id and encode it */
  for (int ilvl = 0; ilvl < mux_level; ++ilvl) {
    int start_idx = num_input_basis * ilvl;
    int end_idx = num_input_basis * (ilvl + 1) - 1;
    int encoded_path_id = 0;
    int checker = 0;
    for (int idx = start_idx; idx < end_idx; ++idx) { 
      if ('1' == ret[idx]) {
        checker++;
        encoded_path_id = idx; 
      }
    }
    /* There should be at most one '1' */
    assert( (0 == checker) || (1 == checker));
    /* The encoder will convert the path_id to a binary number 
     * For example: when path_id=3, using a 4-input encoder 
     * the sram_bits will be the 4-digit binary number of 3: 0011
     */
    int* tmp_bits = my_itobin_int(path_id, num_bits_per_level);
    /* Copy tmp_bits to encoded bits */
    for (int idx = 0; idx < num_bits_per_level; ++idx) { 
      encoded_ret[idx + ilvl* num_bits_per_level] = tmp_bits[idx];
    }
    /* Free */
    my_free(tmp_bits);
  }

  /* Free ret */
  my_free(ret);

  return encoded_ret; 
}

/* Decode the configuration to sram_bits
 * A path_id is in the range of [0..fan_in-1]
 *          sram
 *  input0 -----|
 *              |----- output
 *  input1 -----|
 * Here, we assume (fix) the mux2to1 pass input0 when sram = 1 (vdd), and pass input1 when sram = 0(gnd)
 * To generate the sram bits, we can determine the in each level of MUX,
 * the path id is on the upper path(sram = 1) or the lower path (sram = 0), by path_id > 2**mux_level
 */
int* decode_tree_mux_sram_bits(int fan_in,
                               int mux_level,
                               int path_id) {
  int* ret = (int*)my_malloc(sizeof(int)*mux_level);
  int i = 0;
  int path_differ = 0;
  int temp = 0;
  int num_last_level_input = 0;
  int active_mux_level = 0;
  int active_path_id = 0;
  
  /* Check */
  assert((0 == path_id)||(0 < path_id));
  assert(path_id < fan_in);

  /* Determine last level input */
  num_last_level_input = tree_mux_last_level_input_num(mux_level, fan_in);

  /* Initialize */
  for (i = 0; i < mux_level; i++) {
    ret[i] = 0;
  }
  
  /* When last level input number is less than the 2**mux_level,
   * There are some input at the level: (mux_level-1)
   */
  active_mux_level = mux_level; 
  active_path_id = path_id; 
  if (num_last_level_input < fan_in) {
    if (path_id > num_last_level_input) {
      active_mux_level = mux_level - 1; 
      active_path_id = (int)pow(2.,(double)(active_mux_level)) - (fan_in - path_id); 
    }
  } else {
    assert(num_last_level_input == fan_in);
  }

  temp = active_path_id;
  for (i = mux_level - 1; i > (mux_level - active_mux_level - 1); i--) {
    path_differ = (int)pow(2.,(double)(i + active_mux_level - mux_level));
    if (temp < path_differ) { 
      ret[i] = 1; 
    } else {
      temp = temp - path_differ;
      ret[i] = 0;
    }
  }

  /* Check */
  assert(0 == temp);
  
  return ret;
}

int get_mux_default_path_id(t_spice_model* mux_spice_model,
                            int mux_size, int path_id) {
  int default_path_id;

  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  if (TRUE == mux_spice_model->design_tech_info.mux_info->add_const_input) {
    default_path_id = mux_size; /* When there is a constant input, use the last path */
  } else {
    default_path_id = DEFAULT_MUX_PATH_ID; /* When there is no constant input, use the default one */
  }

  return default_path_id; 
}

int get_mux_full_input_size(t_spice_model* mux_spice_model,
                            int mux_size) {
  int full_input_size = mux_size;

  assert ((SPICE_MODEL_MUX == mux_spice_model->type) 
         || (SPICE_MODEL_LUT == mux_spice_model->type));

  if (SPICE_MODEL_LUT == mux_spice_model->type) {
    return full_input_size;
  }

  if (TRUE == mux_spice_model->design_tech_info.mux_info->add_const_input) {
    full_input_size = mux_size + 1;
  }
  
  return full_input_size;
}

void decode_cmos_mux_sram_bits(t_spice_model* mux_spice_model,
                               int mux_size, int path_id, 
                               int* bit_len, int** conf_bits, int* mux_level) {
  int num_mux_input = 0;
  int datapath_id = path_id;

  /* Check */
  assert(NULL != mux_level);
  assert(NULL != bit_len);
  assert(NULL != conf_bits);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);
  assert(SPICE_MODEL_DESIGN_CMOS == mux_spice_model->design_tech);

  /* Handle DEFAULT PATH ID */
  if (DEFAULT_PATH_ID == path_id) {
    datapath_id = get_mux_default_path_id(mux_spice_model, mux_size, path_id);
  } else { 
    assert((DEFAULT_PATH_ID < datapath_id)&&(datapath_id < mux_size));
  }

  /* We have an additional input (last input) connected to a constant */
  num_mux_input = get_mux_full_input_size(mux_spice_model, mux_size);

  /* Initialization */
  (*bit_len) = 0;
  (*conf_bits) = NULL;

  /* Special for MUX-2: whatever structure it is, it has always one-level and one configuration bit */
  if (2 == num_mux_input) {
    (*bit_len) = 1;
    (*mux_level) = 1;
    (*conf_bits) = decode_tree_mux_sram_bits(num_mux_input, (*mux_level), datapath_id); 
    return;
  }
  /* Other general cases */ 
  switch (mux_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    (*mux_level) = determine_tree_mux_level(num_mux_input);
    (*bit_len) = (*mux_level);
    (*conf_bits) = decode_tree_mux_sram_bits(num_mux_input, (*mux_level), datapath_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    (*mux_level) = 1;
    (*bit_len) = num_mux_input;
    (*conf_bits) = decode_onelevel_mux_sram_bits(num_mux_input, (*mux_level), datapath_id, 
                                                 mux_spice_model->design_tech_info.mux_info->local_encoder); 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    (*mux_level) = mux_spice_model->design_tech_info.mux_info->mux_num_level;
    (*bit_len) = determine_num_input_basis_multilevel_mux(num_mux_input, (*mux_level)) * (*mux_level);
    (*conf_bits) = decode_multilevel_mux_sram_bits(num_mux_input, (*mux_level), datapath_id, 
                                                   mux_spice_model->design_tech_info.mux_info->local_encoder); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for mux_spice_model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
    exit(1);
  } 
  return;
}

/** Decode 1-level 4T1R MUX
 */
void decode_one_level_4t1r_mux(int path_id, 
                               int bit_len, int* conf_bits) { 
  int i; 

  /* Check */
  assert(0 < bit_len);
  assert(NULL != conf_bits);
  assert((-1 < path_id)&&((path_id < bit_len/2 - 1)||(path_id == bit_len/2 - 1)));

  /* All the others should be zero */
  for (i = 0; i < bit_len; i++) {
    conf_bits[i] = 0;
  }

  /* Last bit of WL should be 1 */
  conf_bits[bit_len-1] = 1;
  /* determine  which BL should be 1*/
  conf_bits[path_id] = 1;

  return;
}

/** Decode multi-level 4T1R MUX
 */
void decode_multilevel_4t1r_mux(int num_level, int num_input_basis,
                                        int mux_size, int path_id, 
                                        int bit_len, int* conf_bits) { 
  int i, active_basis_path_id;

  /* Check */
  assert(0 < bit_len);
  assert(NULL != conf_bits);
  /* assert((-1 < path_id)&&(path_id < bit_len/2 - 1)); */
  /* Start from first level to the last level */
  active_basis_path_id = path_id;
  for (i = 0; i < num_level; i++) {
    /* Treat each basis as a 1-level 4T1R MUX */
    active_basis_path_id = active_basis_path_id % num_input_basis;
    /* Last bit of WL should be 1 */
    conf_bits[bit_len/2 + (num_input_basis+1)*(i+1) - 1] = 1;
    /* determine  which BL should be 1*/
    conf_bits[(num_input_basis+1)*i + active_basis_path_id] = 1;
  }

  return;
}

/** Decode the configuration bits for a 4T1R-based MUX
 *  Determine the number of configuration bits 
 *  Configuration bits are decoded depending on the MUX structure:
 *  1. 1-level; 2. multi-level (tree-like);
 */
void decode_rram_mux(t_spice_model* mux_spice_model,
                     int mux_size, int path_id,
                     int* bit_len, int** conf_bits, int* mux_level) {
  int num_level, num_input_basis, num_mux_input;
  int datapath_id = path_id;

  /* Check */
  assert(NULL != mux_level);
  assert(NULL != bit_len);
  assert(NULL != conf_bits);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);
  assert(SPICE_MODEL_DESIGN_RRAM == mux_spice_model->design_tech);

  /* Handle DEFAULT PATH ID */
  if (DEFAULT_PATH_ID == datapath_id) {
    datapath_id = get_mux_default_path_id(mux_spice_model, mux_size, path_id);
  } else {
    assert((DEFAULT_PATH_ID < datapath_id)&&(datapath_id < mux_size));
  }

  /* We have an additional input (last input) connected to a constant */
  num_mux_input = get_mux_full_input_size(mux_spice_model, mux_size);
  
  /* Initialization */
  (*mux_level) = 0;
  (*bit_len) = 0;
  (*conf_bits) = NULL;

  (*bit_len) = 2 * count_num_sram_bits_one_spice_model(mux_spice_model, num_mux_input);
  
  /* Switch cases: MUX structure */
  switch (mux_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    /* Number of configuration bits is 2*(input_size+1) */
    num_level = 1;
    break;
  case SPICE_MODEL_STRUCTURE_TREE:
    /* Number of configuration bits is num_level* 2*(basis+1) */
    num_level = determine_tree_mux_level(num_mux_input); 
    num_input_basis = 2;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Number of configuration bits is num_level* 2*(basis+1) */
    num_level = mux_spice_model->design_tech_info.mux_info->mux_num_level; 
    num_input_basis = determine_num_input_basis_multilevel_mux(num_mux_input, num_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid MUX structure!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Malloc configuration bits */
  (*conf_bits) = (int*)my_calloc((*bit_len), sizeof(int));

  /* Decode configuration bits : BL & WL*/
  /* Switch cases: MUX structure */
  switch (mux_spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    decode_one_level_4t1r_mux(datapath_id, (*bit_len), (*conf_bits)); 
    break;
  case SPICE_MODEL_STRUCTURE_TREE:
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    decode_multilevel_4t1r_mux(num_level, num_input_basis, num_mux_input, 
                               datapath_id, (*bit_len), (*conf_bits)); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid MUX structure!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
  
  (*mux_level) = num_level;
  
  return;
}

/* Useful functions for MUX architecture */
void init_spice_mux_arch(t_spice_model* spice_model,
                         t_spice_mux_arch* spice_mux_arch,
                         int mux_size) {
  int cur;
  int i;
  /* Make sure we have a valid pointer*/
  if (NULL == spice_mux_arch) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d])Invalid spice_mux_arch!\n",
               __FILE__, __LINE__);
    exit(1);
  } 

  /* Basic info*/
  spice_mux_arch->structure = spice_model->design_tech_info.mux_info->structure;
  spice_mux_arch->num_data_input = mux_size;
  /* We create an additional input for MUX, which is connected to a constant VDD|GND */
  spice_mux_arch->num_input = get_mux_full_input_size(spice_model, mux_size);

  /* For different structure */
  switch (spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    spice_mux_arch->num_level = determine_tree_mux_level(spice_mux_arch->num_input);
    spice_mux_arch->num_input_basis = 2;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = tree_mux_last_level_input_num(spice_mux_arch->num_level, 
                                                                         spice_mux_arch->num_input);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    spice_mux_arch->num_level = 1;
    spice_mux_arch->num_input_basis = spice_mux_arch->num_input;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = spice_mux_arch->num_input;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Handle speical case: input size is 2 */
    if (2 == spice_mux_arch->num_input) { 
      spice_mux_arch->num_level = 1;
    } else {
      spice_mux_arch->num_level = spice_model->design_tech_info.mux_info->mux_num_level;
    }
    spice_mux_arch->num_input_basis = determine_num_input_basis_multilevel_mux(spice_mux_arch->num_input, 
                                                                               spice_mux_arch->num_level);
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = multilevel_mux_last_level_input_num(spice_mux_arch->num_level, 
                                                                               spice_mux_arch->num_input_basis, 
                                                                               spice_mux_arch->num_input);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model->name);
    exit(1);
  }

  /* Alloc*/
  spice_mux_arch->num_input_per_level = (int*) my_malloc(sizeof(int)*spice_mux_arch->num_level);
  spice_mux_arch->input_level = (int*)my_malloc(sizeof(int)*spice_mux_arch->num_input);
  spice_mux_arch->input_offset = (int*)my_malloc(sizeof(int)*spice_mux_arch->num_input);

  /* Assign inputs info for the last level first */
  for (i = 0; i < spice_mux_arch->num_input_last_level; i++) {
    spice_mux_arch->input_level[i] = spice_mux_arch->num_level;
    spice_mux_arch->input_offset[i] = i; 
  }
  /* For the last second level*/
  if (spice_mux_arch->num_input > spice_mux_arch->num_input_last_level) {
    cur = ceil((double)spice_mux_arch->num_input_last_level / (double)spice_mux_arch->num_input_basis);
    /* Start from the input ports that are not occupied by the last level
     * last level has (cur) outputs
     */
    /*
    printf("size:%d, cur:%d, num_input_last_level=%d, num_input_basis=%d\n",
          spice_mux_arch->num_input, cur,
          spice_mux_arch->num_input_last_level, spice_mux_arch->num_input_basis);
    */
    for (i = spice_mux_arch->num_input_last_level; i < spice_mux_arch->num_input; i++) {
      spice_mux_arch->input_level[i] = spice_mux_arch->num_level - 1;
      spice_mux_arch->input_offset[i] = cur; 
      cur++;
    }
    assert((cur < (int)pow((double)spice_mux_arch->num_input_basis, (double)(spice_mux_arch->num_level-1)))
           ||(cur == (int)pow((double)spice_mux_arch->num_input_basis, (double)(spice_mux_arch->num_level-1))));
  }
  /* Fill the num_input_per_level*/
  for (i = 0; i < spice_mux_arch->num_level; i++) {
    cur = i + 1;
    spice_mux_arch->num_input_per_level[i] = (int)pow((double)spice_mux_arch->num_input_basis, (double)cur);
    if ((cur == spice_mux_arch->num_level)
       &&(spice_mux_arch->num_input_last_level < spice_mux_arch->num_input_per_level[i])) {
      spice_mux_arch->num_input_per_level[i] = spice_mux_arch->num_input_last_level;
    }
  }
  
  return; 
}

/* Determine if we need a speical basis.
 * If we need one, we give the MUX size of this special basis 
 */
int find_spice_mux_arch_special_basis_size(t_spice_mux_arch spice_mux_arch) {
  int im;
  int mux_size = spice_mux_arch.num_input;
  int num_input_basis = spice_mux_arch.num_input_basis;
  int num_input_special_basis = 0;
  int special_basis_start = 0;
   
  /* For different structure */
  switch (spice_mux_arch.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    special_basis_start = mux_size - mux_size % num_input_basis;
    for (im = special_basis_start; im < mux_size; im++) {
      if (spice_mux_arch.num_level == spice_mux_arch.input_level[im]) {
        num_input_special_basis++;
      }
    } 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice_mux_arch!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return num_input_special_basis;
}

/* Search the linked list, if we have the same mux size and spice_model
 * return 1, if not we return 0
 */
t_llist* search_mux_linked_list(t_llist* mux_head,
                                int mux_size,
                                t_spice_model* spice_model) {
  t_llist* temp = mux_head;
  t_spice_mux_model* cur_mux = NULL;
  /* traversal the linked list*/ 
  while(temp) {
    cur_mux = (t_spice_mux_model*)(temp->dptr);
    if ((cur_mux->size == mux_size)
      &&(spice_model == cur_mux->spice_model)) {
      return temp;
    }
    /* next */
    temp = temp->next;
  }

  return NULL;
}


/* Check the linked list if we have a mux stored with same spice model
 * if not, we create a new one.
 */
void check_and_add_mux_to_linked_list(t_llist** muxes_head,
                                      int mux_size,
                                      t_spice_model* spice_model) {
  t_spice_mux_model* cur_mux = NULL;
  t_llist* temp = NULL;

  /* Check code: to avoid mistake, we should check the mux size
   * the mux_size should be at least 2 so that we need a mux
   */
  if (mux_size < 2) {
    printf("Warning:(File:%s,LINE[%d]) ilegal mux size (%d), expect to be at least 2!\n",
           __FILE__, __LINE__, mux_size);
    return;
  }

  /* Search the linked list */
  if (NULL != search_mux_linked_list((*muxes_head),mux_size,spice_model)) {
    /* We find one, there is no need to create a new one*/
    return;
  }
  /*Create a linked list, if head is NULL*/
  if (NULL == (*muxes_head)) {
    (*muxes_head) = create_llist(1); 
    (*muxes_head)->dptr = my_malloc(sizeof(t_spice_mux_model));
    cur_mux = (t_spice_mux_model*)((*muxes_head)->dptr);
  } else { 
    /* We have to create a new elment in linked list*/
    temp = insert_llist_node((*muxes_head)); 
    temp->dptr = my_malloc(sizeof(t_spice_mux_model));
    cur_mux = (t_spice_mux_model*)(temp->dptr);
  }
  /* Fill the new SPICE MUX Model*/
  cur_mux->size = mux_size;
  cur_mux->spice_model = spice_model;
  cur_mux->cnt = 1; /* Initialize the counter*/

  return;
}

/* Free muxes linked list
 */
void free_muxes_llist(t_llist* muxes_head) {
  t_llist* temp = muxes_head;
  while(temp) {
    /* Free the mux_spice_model, remember to set the pointer to NULL */
    free(temp->dptr);
    temp->dptr = NULL;
    /* Move on to the next pointer*/
    temp = temp->next;
  }
  free_llist(muxes_head);
  return;
}

/* For LUTs without SPICE netlist defined, we can create a SPICE netlist
 * In this case, we need a MUX
 */
void stats_lut_spice_mux(t_llist** muxes_head,
                         t_spice_model* spice_model) {
  int lut_mux_size = 0; 
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;

  if (NULL == spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid Spice_model pointer!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert(SPICE_MODEL_LUT == spice_model->type); 

  /* Get input ports */
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  assert(1 == num_input_port);
  lut_mux_size = (int)pow(2.,(double)(input_ports[0]->size));

  /* MUX size = 2^lut_size */
  check_and_add_mux_to_linked_list(muxes_head, lut_mux_size, spice_model);

  return;
}



/* Stats the multiplexer sizes and structure in the global routing architecture*/
void stats_spice_muxes_routing_arch(t_llist** muxes_head,
                                    int num_switch,
                                    t_switch_inf* switches,
                                    t_spice* spice,
                                    t_det_routing_arch* routing_arch) {
  int inode;
  t_rr_node* node;
  t_spice_model* sb_switch_spice_model = NULL;
  t_spice_model* cb_switch_spice_model = NULL;

  /* Current Version: Support Uni-directional routing architecture only*/ 
  if (UNI_DIRECTIONAL != routing_arch->directionality) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Spice Modeling Only support uni-directional routing architecture.\n",__FILE__, __LINE__);
    exit(1);
  }

  /* The routing path is. 
   *  OPIN ----> CHAN ----> ... ----> CHAN ----> IPIN
   *  Each edge is a switch, for IPIN, the switch is a connection block,
   *  for the rest is a switch box
   */
  /* Count the sizes of muliplexers in routing architecture */  
  /* Visit the global variable : num_rr_nodes, rr_node */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    node = &rr_node[inode]; 
    switch (node->type) {
    case IPIN: 
      /* Have to consider the fan_in only, it is a connection box(multiplexer)*/
      assert((node->fan_in > 0)||(0 == node->fan_in));
      if ((0 == node->fan_in)||(1 == node->fan_in)) {
        break; 
      }
      /* Find the spice_model for multiplexers in connection blocks */
      cb_switch_spice_model = switches[node->driver_switch].spice_model;
      /* we should select a spice model for the connection box*/
      assert(NULL != cb_switch_spice_model);
      check_and_add_mux_to_linked_list(muxes_head, node->fan_in,cb_switch_spice_model);
      break;
    case CHANX:
    case CHANY: 
      /* Channels are the same, have to consider the fan_in as well, 
       * it could be a switch box if previous rr_node is a channel
       * or it could be a connection box if previous rr_node is a IPIN or OPIN
       */
      assert((node->fan_in > 0)||(0 == node->fan_in));
      if ((0 == node->fan_in)||(1 == node->fan_in)) {
        break; 
      }
      /* Find the spice_model for multiplexers in switch blocks*/
      sb_switch_spice_model = switches[node->driver_switch].spice_model;
      /* we should select a spice model for the Switch box*/
      assert(NULL != sb_switch_spice_model);
      check_and_add_mux_to_linked_list(muxes_head, node->fan_in,sb_switch_spice_model);
      break;
    case OPIN: 
      /* Actually, in single driver routing architecture, the OPIN, source of a routing path,
       * is directly connected to Switch Box multiplexers
       */
      break;
    default:
      break;
    }
  }

  return;
}

/* Recursively do statistics for the
 * multiplexer spice models inside pb_types
 */
void stats_mux_spice_model_pb_type_rec(t_llist** muxes_head,
                                       t_pb_type* cur_pb_type) {
  
  int imode, ichild, jinterc;
  t_spice_model* interc_spice_model = NULL;
 
  if (NULL == cur_pb_type) {
    vpr_printf(TIO_MESSAGE_WARNING,"(File:%s,LINE[%d])cur_pb_type is null pointor!\n",__FILE__,__LINE__);
    return;
  }

  /* If there is spice_model_name, this is a leaf node!*/
  if ((NULL != cur_pb_type->spice_model_name) 
     || (NULL != cur_pb_type->physical_pb_type_name)) {
    /* What annoys me is VPR create a sub pb_type for each lut which suppose to be a leaf node
     * This may bring software convience but ruins SPICE modeling
     */
    assert(NULL != cur_pb_type->phy_pb_type->spice_model);
    return;
  }
  /* Traversal the hierarchy*/
  for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
    /* Then we have to statisitic the interconnections*/
    for (jinterc = 0; jinterc < cur_pb_type->modes[imode].num_interconnect; jinterc++) {
      /* Check the num_mux and fan_in*/
      assert((0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux)
            ||(0 < cur_pb_type->modes[imode].interconnect[jinterc].num_mux));
      if (0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux) {
        continue;
      }
      interc_spice_model = cur_pb_type->modes[imode].interconnect[jinterc].spice_model;
      assert(NULL != interc_spice_model); 
      check_and_add_mux_to_linked_list(muxes_head,
                                       cur_pb_type->modes[imode].interconnect[jinterc].fan_in,
                                       interc_spice_model);
    }
    for (ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ichild++) {
      stats_mux_spice_model_pb_type_rec(muxes_head,
                                        &cur_pb_type->modes[imode].pb_type_children[ichild]);
    }
  }
  return;
}

/* Statistics the MUX SPICE MODEL with the help of pb_graph
 * Not the most efficient function to finish the job 
 * Abandon it. But remains a good framework that could be re-used in connecting
 * spice components together
 */
void stats_mux_spice_model_pb_node_rec(t_llist** muxes_head,
                                       t_pb_graph_node* cur_pb_node) {
  int imode, ipb, ichild, iport, ipin;
  t_pb_type* cur_pb_type = cur_pb_node->pb_type;
  t_spice_model* interc_spice_model = NULL;
  enum e_interconnect pin_interc_type;
  
  if (NULL == cur_pb_node) {
    vpr_printf(TIO_MESSAGE_WARNING,"(File:%s,LINE[%d])cur_pb_node is null pointor!\n",__FILE__,__LINE__);
    return;
  }

  if (NULL == cur_pb_type) {
    vpr_printf(TIO_MESSAGE_WARNING,"(File:%s,LINE[%d])cur_pb_type is null pointor!\n",__FILE__,__LINE__);
    return;
  }

  /* If there is 0 mode, this is a leaf node!*/
  if (NULL != cur_pb_type->blif_model) {
    assert(0 == cur_pb_type->num_modes);
    assert(NULL == cur_pb_type->modes);
    /* Ensure there is blif_model, and spice_model*/
    assert(NULL != cur_pb_type->model);
    assert(NULL != cur_pb_type->spice_model_name);
    assert(NULL != cur_pb_type->spice_model);
    return;
  }
  /* Traversal the hierarchy*/
  for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
    /* Then we have to statisitic the interconnections*/
    /* See the input ports*/
    for (iport = 0; iport < cur_pb_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_node->num_input_pins[iport]; ipin++) {
        /* Ensure this is an input port */
        assert(IN_PORT == cur_pb_node->input_pins[iport][ipin].port->type);
        /* See the edges, if the interconnetion type infer a MUX, we go next step*/
        pin_interc_type = find_pb_graph_pin_in_edges_interc_type(cur_pb_node->input_pins[iport][ipin]);
        if ((COMPLETE_INTERC != pin_interc_type)&&(MUX_INTERC != pin_interc_type)) {
          continue;
        }
        /* We shoule check the size of inputs, in some case of complete, the input_edge is one...*/
        if ((COMPLETE_INTERC == pin_interc_type)&&(1 == cur_pb_node->input_pins[iport][ipin].num_input_edges)) {
          continue;
        }
        /* Note: i do care the input_edges only! They may infer multiplexers*/
        interc_spice_model = find_pb_graph_pin_in_edges_interc_spice_model(cur_pb_node->input_pins[iport][ipin]);
        check_and_add_mux_to_linked_list(muxes_head,
                                         cur_pb_node->input_pins[iport][ipin].num_input_edges,
                                         interc_spice_model);
      }
    }
    /* See the output ports*/
    for (iport = 0; iport < cur_pb_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_node->num_output_pins[iport]; ipin++) {
        /* Ensure this is an input port */
        assert(OUT_PORT == cur_pb_node->output_pins[iport][ipin].port->type);
        /* See the edges, if the interconnetion type infer a MUX, we go next step*/
        pin_interc_type = find_pb_graph_pin_in_edges_interc_type(cur_pb_node->output_pins[iport][ipin]);
        if ((COMPLETE_INTERC != pin_interc_type)&&(MUX_INTERC != pin_interc_type)) {
          continue;
        }
        /* We shoule check the size of inputs, in some case of complete, the input_edge is one...*/
        if ((COMPLETE_INTERC == pin_interc_type)&&(1 == cur_pb_node->output_pins[iport][ipin].num_input_edges)) {
          continue;
        }
        /* Note: i do care the input_edges only! They may infer multiplexers*/
        interc_spice_model = find_pb_graph_pin_in_edges_interc_spice_model(cur_pb_node->output_pins[iport][ipin]);
        check_and_add_mux_to_linked_list(muxes_head,
                                         cur_pb_node->output_pins[iport][ipin].num_input_edges,
                                         interc_spice_model);
      }
    }
    /* See the clock ports*/
    for (iport = 0; iport < cur_pb_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_node->num_clock_pins[iport]; ipin++) {
        /* Ensure this is an input port */
        assert(IN_PORT == cur_pb_node->clock_pins[iport][ipin].port->type);
        /* See the edges, if the interconnetion type infer a MUX, we go next step*/
        pin_interc_type = find_pb_graph_pin_in_edges_interc_type(cur_pb_node->clock_pins[iport][ipin]);
        if ((COMPLETE_INTERC != pin_interc_type)&&(MUX_INTERC != pin_interc_type)) {
          continue;
        }
        /* We shoule check the size of inputs, in some case of complete, the input_edge is one...*/
        if ((COMPLETE_INTERC == pin_interc_type)&&(1 == cur_pb_node->clock_pins[iport][ipin].num_input_edges)) {
          continue;
        }
        /* Note: i do care the input_edges only! They may infer multiplexers*/
        interc_spice_model = find_pb_graph_pin_in_edges_interc_spice_model(cur_pb_node->clock_pins[iport][ipin]);
        check_and_add_mux_to_linked_list(muxes_head,
                                         cur_pb_node->clock_pins[iport][ipin].num_input_edges,
                                         interc_spice_model);
      }
    }
    for (ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ichild++) {
      /* num_pb is the number of such pb_type in a mode*/
      for (ipb = 0; ipb < cur_pb_type->modes[imode].pb_type_children[ichild].num_pb; ipb++) {
        /* child_pb_grpah_nodes: [0..num_modes-1][0..num_pb_type_in_mode-1][0..num_pb_type-1]*/
        stats_mux_spice_model_pb_node_rec(muxes_head,
                                          &cur_pb_node->child_pb_graph_nodes[imode][ichild][ipb]);
      }
    }
  } 
  return;  
}


/* Statistic for all the multiplexers in FPGA
 * We determine the sizes and its structure (according to spice_model) for each type of multiplexers
 * We search multiplexers in Switch Blocks, Connection blocks and Configurable Logic Blocks
 * In additional to multiplexers, this function also consider crossbars.
 * All the statistics are stored in a linked list, as a return value
 */
t_llist* stats_spice_muxes(int num_switches,
                           t_switch_inf* switches,
                           t_spice* spice,
                           t_det_routing_arch* routing_arch) {
  int itype;
  int imodel;
  /* Linked-list to store the information of Multiplexers*/
  t_llist* muxes_head = NULL; 

  /* Step 1: We should check the multiplexer spice models defined in routing architecture.*/
  stats_spice_muxes_routing_arch(&muxes_head, num_switches, switches, spice, routing_arch);

  /* Statistics after search routing resources */
  /*
  temp = muxes_head;
  while(temp) {
    t_spice_mux_model* spice_mux_model = (t_spice_mux_model*)temp->dptr;
    vpr_printf(TIO_MESSAGE_INFO,"Routing multiplexers: size=%d\n",spice_mux_model->size);
    temp = temp->next;
  }
  */

  /* Step 2: Count the sizes of multiplexers in complex logic blocks */  
  for (itype = 0; itype < num_types; itype++) {
    if (NULL != type_descriptors[itype].pb_type) {
      stats_mux_spice_model_pb_type_rec(&muxes_head,type_descriptors[itype].pb_type);
    }
  }

  /* Step 3: count the size of multiplexer that will be used in LUTs*/
  for (imodel = 0; imodel < spice->num_spice_model; imodel++) {
    /* For those LUTs that netlists are not provided. We create a netlist and thus need a MUX*/
    if ((SPICE_MODEL_LUT == spice->spice_models[imodel].type)
      &&(NULL == spice->spice_models[imodel].model_netlist)) {
      stats_lut_spice_mux(&muxes_head, &(spice->spice_models[imodel])); 
    }
  }

  /* Statistics after search routing resources */
  /*
  temp = muxes_head;
  while(temp) {
    t_spice_mux_model* spice_mux_model = (t_spice_mux_model*)temp->dptr;
    vpr_printf(TIO_MESSAGE_INFO,"Pb_types multiplexers: size=%d\n",spice_mux_model->size);
    temp = temp->next;
  }
  */

  return muxes_head;
}



