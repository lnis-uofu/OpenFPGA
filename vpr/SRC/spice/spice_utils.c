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

/* Include SPICE support headers*/
#include "quicksort.h"
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"

enum e_dir_err {
 E_DIR_NOT_EXIST,
 E_EXIST_BUT_NOT_DIR,
 E_DIR_EXIST
};

/***** Subroutines *****/
char* my_gettime() {
  time_t current_time;
  char* c_time_string;
  
  /* Obtain current time as seconds elapsed since the Epoch*/
  current_time = time(NULL);
  
  if (current_time == ((time_t)-1)) {
    vpr_printf(TIO_MESSAGE_ERROR,"Failure to compute the current time.\n");
    exit(1);
  }
  
  /* Convert to local time format*/
  c_time_string = ctime(&current_time);
  if (NULL == c_time_string) {
    vpr_printf(TIO_MESSAGE_ERROR,"Failure to convert the current time.\n");
    exit(1);
  }
  /* Return it*/
  return c_time_string;
}

char* format_dir_path(char* dir_path) {
  int len = strlen(dir_path); /* String length without the last "\0"*/
  int i;
  char* ret = (char*)my_malloc(sizeof(char)*(len+2));
  
  strcpy(ret,dir_path);
  /* Replace all the "\" to "/"*/
  for (i=0; i<len; i++) {
    if (ret[i] == '\\') {
      ret[i] = '/'; /* !!! Should not use "" */
    }
  } 
  /* If the path does not end up with "/" we should complete it*/
  if (ret[len-1] != '/') {
    strcat(ret, "/");
  }
  return ret;
}

int try_access_file(char* file_path) {
  /* F_OK checks existence and also R_OK, W_OK, X_OK,
   * for readable, writable, excutable
   */
  int ret = access(file_path,F_OK); 
  if (0 == ret)  {
    vpr_printf(TIO_MESSAGE_WARNING,"File(%s) exists! Will overwrite it!\n",file_path);
  }
  return ret;
}

void my_remove_file(char* file_path) {
  if (NULL == file_path) {
    return;
  } 
  if (0 != remove(file_path)) {
    vpr_printf(TIO_MESSAGE_WARNING, "Fail to remove file(%s)!\n", file_path);
  }
  return;
}

enum e_dir_err try_access_dir(char* dir_path) {
  struct stat s;
  int err = stat(dir_path, &s);
  if (-1 == err) {
     if (ENOENT == errno) {
       return E_DIR_NOT_EXIST;
     } else {
       perror("stat");
       exit(1);
     }
  } else {
    if (S_ISDIR(s.st_mode)) {
      /* It is a dir*/
      return E_DIR_EXIST;
    } else {
      /* Exists but is no dir*/
      return E_EXIST_BUT_NOT_DIR;
    }
  }
}

int create_dir_path(char* dir_path) {
   int ret; 
   
   /* Check this path does not exist*/

   /* Check the input legal*/
   if (NULL == dir_path) {
     vpr_printf(TIO_MESSAGE_INFO,"dir_path is empty and nothing is created.\n");
     return 0;
   }
   ret = mkdir(dir_path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
   switch (ret) {
   case 0:
     vpr_printf(TIO_MESSAGE_INFO,"Create directory(%s)...successfully.\n",dir_path);
     return 1;
   case -1:
     if (EEXIST == errno) {
       vpr_printf(TIO_MESSAGE_WARNING,"Directory(%s) already exists. Will overwrite SPICE netlists\n",dir_path);
       return 1;
     }
   default:
     vpr_printf(TIO_MESSAGE_ERROR,"Create directory(%s)...Failed!\n",dir_path);
     exit(1);
     return 0;
   }
}

/* Cat string2 to the end of string1 */
char* my_strcat(char* str1,
                char* str2) {
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  char* ret = (char*)my_malloc(sizeof(char) * (len1 + len2 + 1));
  
  strcpy(ret,str1);
  strcat(ret,str2);

  return ret;  
}

/* Split the path and program name*/
int split_path_prog_name(char* prog_path,
                         char  split_token,
                         char** ret_path,
                         char** ret_prog_name) {
  int i;
  int split_pos = -1;
  char* local_copy = my_strdup(prog_path);
  int len = strlen(local_copy);
  char* path = NULL;
  char* prog_name = NULL;
   
  /* Spot the split token*/
  for (i = len; i > -1; i--) {
    if (split_token == local_copy[i]) {
      split_pos = i; 
      break; 
    }
  }

  /* Get the path and prog_name*/
  if (-1 == split_pos) {
    /* In this case, the prog_path actually contains only the program name*/
    path = NULL;
    prog_name = local_copy;
  } else if (len == split_pos) {
    /* In this case the progrom name is NULL... actually the prog_path is a directory*/
    path = local_copy;
    prog_name = NULL;
  } else {
    /* We have to split it!*/
    local_copy[split_pos] = '\0';
    path = my_strdup(local_copy);
    prog_name = my_strdup(local_copy + split_pos + 1);
  } 
  
  /*Copy it to the return*/
  (*ret_path) = my_strdup(path); 
  (*ret_prog_name) = my_strdup(prog_name);

  /* Free useless resources */
  my_free(local_copy);
  my_free(path);
  my_free(prog_name);

  return 1;
}

char* chomp_file_name_postfix(char* file_name) {
  char* ret = NULL;
  char* postfix = NULL;
   
  split_path_prog_name(file_name, '.', &ret, &postfix);

  my_free(postfix);

  return ret;
}

/* With a given spice_model_name, find the spice model and return its pointer
 * If we find nothing, return NULL
 */
t_spice_model* find_name_matched_spice_model(char* spice_model_name,
                                             int num_spice_model,
                                             t_spice_model* spice_models) {
  t_spice_model* ret = NULL;
  int imodel;
  int num_found = 0;

  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if (0 == strcmp(spice_model_name, spice_models[imodel].name)) {
      ret = &(spice_models[imodel]);
      num_found++;
    }
  }

  assert(1 == num_found);

  return ret;
}

/* Get the default spice_model*/
t_spice_model* get_default_spice_model(enum e_spice_model_type default_spice_model_type,
                                       int num_spice_model,
                                       t_spice_model* spice_models) {
  t_spice_model* ret = NULL;
  int i;
  
  for (i = 0; i < num_spice_model; i++) {
    /* Find a MUX and it is set as default*/
    if ((default_spice_model_type == spice_models[i].type)&&(1 == spice_models[i].is_default)) {
      /* Check if we have multiple default*/
      if (NULL != ret) {
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d])Both SPICE model(%s and %s) are set as default!\n",
                   __FILE__, __LINE__, ret->name, spice_models[i].name);
        exit(1);
      } else {
        ret = &(spice_models[i]); 
      }
    }
  }
  
  return ret;
}

/* Return the SPICE model ports wanted
 * ATTENTION: we use the pointer of spice model here although we don't modify anything of spice_model 
 *            but we have return input ports, whose pointer will be lost if the input is not the pointor of spice_model
 * BECAUSE spice_model will be a local copy if it is not a pointer. And it will be set free when this function
 * finishes. So the return pointers become invalid !
 */
t_spice_model_port** find_spice_model_ports(t_spice_model* spice_model,
                                            enum e_spice_model_port_type port_type,
                                            int* port_num) {
  int iport, cur;
  t_spice_model_port** ret = NULL;

  /* Check codes*/
  assert(NULL != port_num);
  assert(NULL != spice_model);

  /* Count the number of ports that match*/
  (*port_num) = 0;
  for (iport = 0; iport < spice_model->num_port; iport++) {
    if (port_type == spice_model->ports[iport].type) {
      (*port_num)++;
    }
  }
  
  /* Initial the return pointers*/
  ret = (t_spice_model_port**)my_malloc(sizeof(t_spice_model_port*)*(*port_num));
  memset(ret, 0 , sizeof(t_spice_model_port*)*(*port_num));
  
  /* Fill the return pointers*/
  cur = 0;
  for (iport = 0; iport < spice_model->num_port; iport++) {
    if (port_type == spice_model->ports[iport].type) {
      ret[cur] = &(spice_model->ports[iport]);
      cur++;
    }
  }
  /* Check correctness*/
  assert(cur == (*port_num));
  
  return ret;
}

/* Find the transistor in the tech lib*/
t_spice_transistor_type* find_mosfet_tech_lib(t_spice_tech_lib tech_lib,
                                              e_spice_trans_type trans_type) {
  /* If we did not find return NULL*/
  t_spice_transistor_type* ret = NULL;
  int i;

  for (i = 0; i < tech_lib.num_transistor_type; i++) {
    if (trans_type == tech_lib.transistor_types[i].type) {
      ret = &(tech_lib.transistor_types[i]); 
      break;
    }
  }
  
  return ret; 
}

/* Convert a integer to a string*/
char* my_itoa(int input) {
  char* ret = NULL;
  int sign = 0;
  int len = 0;
  int temp = input;
  int cur;
  char end_of_str;

  /* Identify input number is positive or negative*/
  if (input < 0) {
    sign = 1; /* sign will be '-'*/
    len = 1;
    temp = 0 - input;
  } else if (0 == input) {
    sign = 0;
    len = 2;
    /* Alloc*/
    ret = (char*)my_malloc(sizeof(char)*len);
    /* Lets get the end_of_str, the char is dependent on OS*/
    sprintf(ret,"%s","0");
    return ret;
  }
  /* Identify the length of string*/
  while(temp > 0) {
    len++;
    temp = temp/10;
  }
  /* Total length of string should include '\0' at the end*/
  len = len + 1;
  /* Alloc*/
  ret = (char*)my_malloc(sizeof(char)*len);

  /*Fill it*/
  temp = input;
  /* Lets get the end_of_str, the char is dependent on OS*/
  sprintf(ret,"%s","-");
  end_of_str = ret[1];
  ret[len-1] = end_of_str;
  cur = len - 2;
  /* Print the number reversely*/
  while(temp > 0) {
    ret[cur] = temp%10 + '0'; /* ASIC II base is '0'*/
    cur--;
    temp = temp/10;
  }
  /* Print the sign*/
  if (1 == sign) {
    assert(0 == cur);
    ret[cur] = '-';
    temp = 0 - input;
  } else {
    assert(-1 == cur);
  }

  return ret;
}

/* With given spice_model_port, find the pb_type port with same name and type*/
t_port* find_pb_type_port_match_spice_model_port(t_pb_type* pb_type,
                                                 t_spice_model_port* spice_model_port) {
  int iport;
  t_port* ret = NULL;

  /* Search ports */
  for (iport = 0; iport < pb_type->num_ports; iport++) {
    /* Match the name and port size*/
    if ((0 == strcmp(pb_type->ports[iport].name, spice_model_port->prefix)) 
      &&(pb_type->ports[iport].num_pins == spice_model_port->size)) {
      /* Match the type*/
      switch (spice_model_port->type) {
      case SPICE_MODEL_PORT_INPUT:
        if ((IN_PORT == pb_type->ports[iport].type)
          &&(0 == pb_type->ports[iport].is_clock)) {
          if (NULL != ret) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])More than 1 pb_type(%s) port match spice_model_port(%s)!\n",
                       __FILE__, __LINE__, pb_type->name, spice_model_port->prefix);
            exit(1);
          }
          ret = &(pb_type->ports[iport]);
        }
        break;
      case SPICE_MODEL_PORT_OUTPUT:
        if (OUT_PORT == pb_type->ports[iport].type) {
          if (NULL != ret) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])More than 1 pb_type(%s) port match spice_model_port(%s)!\n",
                       __FILE__, __LINE__, pb_type->name, spice_model_port->prefix);
            exit(1);
          }
          ret = &(pb_type->ports[iport]);
        }
        break;
      case SPICE_MODEL_PORT_CLOCK:
        if ((IN_PORT == pb_type->ports[iport].type)&&(1 == pb_type->ports[iport].is_clock)) {
          if (NULL != ret) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])More than 1 pb_type(%s) port match spice_model_port(%s)!\n",
                       __FILE__, __LINE__, pb_type->name, spice_model_port->prefix);
            exit(1);
          }
          ret = &(pb_type->ports[iport]);
        }
        break;
      case SPICE_MODEL_PORT_INOUT : 
        if ((INOUT_PORT == pb_type->ports[iport].type)&&(0 == pb_type->ports[iport].is_clock)) {
          if (NULL != ret) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])More than 1 pb_type(%s) port match spice_model_port(%s)!\n",
                       __FILE__, __LINE__, pb_type->name, spice_model_port->prefix);
            exit(1);
          }
          ret = &(pb_type->ports[iport]);
        }
        break;
      case SPICE_MODEL_PORT_SRAM:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid type for spice_model_port(%s)!\n",
                   __FILE__, __LINE__, spice_model_port->prefix);
        exit(1);
      }
    }
  }

  return ret;
}

char* chomp_spice_node_prefix(char* spice_node_prefix) {
  int len = 0;
  char* ret = NULL;

  if (NULL == spice_node_prefix) {
    return NULL;
  }

  len = strlen(spice_node_prefix); /* String length without the last "\0"*/
  ret = (char*)my_malloc(sizeof(char)*(len+2));
  
  /* Don't do anything when input is NULL*/
  if (NULL == spice_node_prefix) {
    my_free(ret);
    return NULL;
  }

  strcpy(ret,spice_node_prefix);
  /* If the path end up with "_" we should remove it*/
  if ('_' == ret[len-1]) {
    ret[len-1] = ret[len];
  }

  return ret;
}

char* format_spice_node_prefix(char* spice_node_prefix) {
  int len = strlen(spice_node_prefix); /* String length without the last "\0"*/
  char* ret = (char*)my_malloc(sizeof(char)*(len+2));
 
  /* Don't do anything when input is NULL*/ 
  if (NULL == spice_node_prefix) {
    my_free(ret);
    return NULL;
  }

  strcpy(ret,spice_node_prefix);
  /* If the path does not end up with "_" we should complete it*/
  if (ret[len-1] != '_') {
    strcat(ret, "_");
  }
  return ret;
}

t_port** find_pb_type_ports_match_spice_model_port_type(t_pb_type* pb_type,
                                                        enum e_spice_model_port_type port_type,
                                                        int* port_num) {
  int iport, cur;
  t_port** ret = NULL;

  /* Check codes*/
  assert(NULL != port_num);
  assert(NULL != pb_type);

  /* Count the number of ports that match*/
  (*port_num) = 0;
  for (iport = 0; iport < pb_type->num_ports; iport++) {
    switch (port_type) {
    case SPICE_MODEL_PORT_INPUT : /* TODO: support is_non_clock_global*/ 
      if ((IN_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        (*port_num)++;
      }
      break;
    case SPICE_MODEL_PORT_OUTPUT: 
      if ((OUT_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        (*port_num)++;
      }
      break;
    case SPICE_MODEL_PORT_INOUT: 
      if ((INOUT_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        (*port_num)++;
      }
      break;
    case SPICE_MODEL_PORT_CLOCK: 
      if ((IN_PORT == pb_type->ports[iport].type)
        &&(1 == pb_type->ports[iport].is_clock)) {
        (*port_num)++;
      }
      break;
    case SPICE_MODEL_PORT_SRAM:
      /* Original VPR don't support this*/
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid type for port!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
  
  /* Initial the return pointers*/
  ret = (t_port**)my_malloc(sizeof(t_port*)*(*port_num));
  memset(ret, 0 , sizeof(t_port*)*(*port_num));
  
  /* Fill the return pointers*/
  cur = 0;

  for (iport = 0; iport < pb_type->num_ports; iport++) {
    switch (port_type) {
    case SPICE_MODEL_PORT_INPUT : /* TODO: support is_non_clock_global*/ 
      if ((IN_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        ret[cur] = &(pb_type->ports[iport]);
        cur++;
      }
      break;
    case SPICE_MODEL_PORT_OUTPUT: 
      if ((OUT_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        ret[cur] = &(pb_type->ports[iport]);
        cur++;
      }
      break;
    case SPICE_MODEL_PORT_INOUT: 
      if ((INOUT_PORT == pb_type->ports[iport].type)
        &&(0 == pb_type->ports[iport].is_clock)) {
        ret[cur] = &(pb_type->ports[iport]);
        cur++;
      }
      break;
    case SPICE_MODEL_PORT_CLOCK: 
      if ((IN_PORT == pb_type->ports[iport].type)
        &&(1 == pb_type->ports[iport].is_clock)) {
        ret[cur] = &(pb_type->ports[iport]);
        cur++;
      }
      break;
    case SPICE_MODEL_PORT_SRAM: 
      /* Original VPR don't support this*/
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid type for port!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
 
  /* Check correctness*/
  assert(cur == (*port_num));
  
  return ret;
}

/* Given the co-ordinators of grid,
 * Find if there is a block mapped into this grid
 */
t_block* search_mapped_block(int x, int y, int z) {
  t_block* ret = NULL;
  int iblk = 0;
  
  /*Valid pointors*/
  assert(NULL != grid);
  assert((0 < x)||(0 == x));
  assert((x < (nx + 1))||(x == (nx + 1)));
  assert((0 < y)||(0 == y));
  assert((x < (ny + 1))||(x == (ny + 1)));

  /* Search all blocks*/
  for (iblk = 0; iblk < num_blocks; iblk++) {
    if ((x == block[iblk].x)&&(y == block[iblk].y)&&(z == block[iblk].z)) {
      /* Matched cordinators*/
      ret = &(block[iblk]);
      /* Check */
      assert(block[iblk].type == grid[x][y].type);
      assert(z < grid[x][y].type->capacity);
      assert(0 < grid[x][y].usage);
    }
  }

  return ret;
}

/* Change the decimal number to binary 
 * and return a array of integer*/
int* my_decimal2binary(int decimal,
                       int* binary_len) {
  int* ret = NULL;
  int i = 0;
  int code = decimal;

  (*binary_len) = 0;
  
  while (0 < code) {
    (*binary_len)++;
    code = code/2;
  }

  i = (*binary_len) - 1;
  while (0 < code) {
    ret[i] = code%2; 
    i--;
    code = code/2;
  }

  return ret;
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
  
  if (1 == mux_level) {
    return mux_size;
  }  

  if (2 == mux_level) {
    num_input_per_unit = (int)sqrt(mux_size);
    if (num_input_per_unit*num_input_per_unit < mux_size) {
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
  int differ = 0;
  int num_input_special_basis = 0;
  
  ret = mux_size - num_basis_last_level * num_input_per_unit; 
  assert((0 == ret)||(0 < ret));

  if (0 < ret) {
    /* Check if we need a special basis at last level,
     * differ : the number of input of the last-2 level will be used 
     */
    differ = (num_basis_last_level + ret) -  pow((double)(num_input_per_unit), (double)(num_level-1));
    /* should be smaller than the num_input_per_unit */
    assert((!(0 > differ))&&(differ < num_input_per_unit));
    /* We need a speical basis */
    num_input_special_basis = differ + 1;
    ret = num_input_special_basis + num_basis_last_level * num_input_per_unit;
  } else {
    ret = mux_size;
  }

  return ret;
}

int determine_lut_path_id(int lut_size,
                          int* lut_inputs) {
  int path_id = 0;
  int i;
  
  for (i = 0; i < lut_size; i++) {
    switch (lut_inputs[i]) {
    case 0:
      path_id += (int)pow(2., (double)(i));
      break;
    case 1:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid sram_bits[%d]!\n", 
                 __FILE__, __LINE__, i);
      exit(1);
    }
  }

  return path_id;
}

int* decode_onelevel_mux_sram_bits(int fan_in,
                                   int mux_level,
                                   int path_id) {
  int* ret = (int*)my_malloc(sizeof(int)*fan_in);
  int i;
  
  for (i = 0; i < fan_in; i++) {
    ret[i] = 0;
  }
  ret[fan_in-1-path_id] = 1;
 
  return ret; 
}

int* decode_multilevel_mux_sram_bits(int fan_in,
                                     int mux_level,
                                     int path_id) {
  int* ret = NULL;
  int i, j, path_differ, temp;
  int num_last_level_input, active_mux_level, active_path_id, num_input_basis;
  
  /* Check */
  assert((0 == path_id)||(0 < path_id));
  assert(path_id < fan_in);
   
  /* TODO: determine the number of input of basis */
  switch (mux_level) {
  case 1:
    /* Special: 1-level should be have special care !!! */
    return decode_onelevel_mux_sram_bits(fan_in, mux_level, path_id);
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
    if (path_id > num_last_level_input) {
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
        ret[i*num_input_basis + j] = 1; 
        /* Reduce the min. start index of this basis */
        temp -= j * (int)pow((double)num_input_basis,(double)(i+active_mux_level-mux_level));
        break; /* Touch the boundry, stop and move onto the next level */
      }
    }
  }

  /* Check */
  assert(0 == temp);
  
  return ret;
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
    path_differ = (int)pow(2.,(double)(i+active_mux_level-mux_level));
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

/**
 * Split a string with strtok
 * Store each token in a char array
 * tokens (char**):
 *  tokens[i] (char*) : pointer to a string split by delims
 */

char** my_strtok(char* str, 
                 char* delims, 
                 int* len)
{
  char** ret;
  char* result;
  int cnt=0;
  int* lens;
  char* tmp;

  if (NULL == str)
  {
    printf("Warning: NULL string found in my_strtok!\n");
    return NULL;
  }

  tmp = my_strdup(str);
  result = strtok(tmp,delims);
  /*First scan to determine the size*/
  while(result != NULL)
  {
    cnt++;
    /* strtok split until its buffer is NULL*/
    result = strtok(NULL,delims); 
  }
  //printf("1st scan cnt=%d\n",cnt);
  /* Allocate memory*/ 
  ret = (char**)my_malloc(cnt*sizeof(char*)); 
  lens = (int*)my_malloc(cnt*sizeof(int));
  /*Second to determine the size of each char*/
  cnt = 0;
  memcpy(tmp,str,strlen(str)+1);
  result = strtok(tmp,delims);  
  while(result != NULL)
  {
    lens[cnt] = strlen(result)+1;
    //printf("lens[%d]=%d .",cnt,lens[cnt]);
    cnt++;
    /* strtok split until its buffer is NULL*/
    result = strtok(NULL,delims); 
  }
  //printf("\n");
  /*Third to allocate and copy each char*/
  cnt = 0;
  memcpy(tmp,str,strlen(str)+1);
  result = strtok(tmp,delims);  
  while(result != NULL)
  {
    //printf("results[%d] = %s ",cnt,result);
    ret[cnt] = my_strdup(result);
    cnt++;
    /* strtok split until its buffer is NULL*/
    result = strtok(NULL,delims); 
  }
  //printf("\n");
  
  (*len) = cnt;

  free(tmp);

  return ret;
}

char* convert_side_index_to_string(int side) {
  switch (side) {
  case 0:
    return "top";
  case 1:
    return "right";
  case 2:
    return "bottom";
  case 3:
    return "left";
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index. Should be [0,3].\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

void fprint_spice_head(FILE* fp,
                       char* usage) {
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 
  fprintf(fp,"*****************************\n");
  fprintf(fp,"*     FPGA SPICE Netlist    *\n");
  fprintf(fp,"* Description: %s *\n",usage);
  fprintf(fp,"*    Author: Xifan TANG     *\n");
  fprintf(fp,"* Organization: EPFL/IC/LSI *\n");
  fprintf(fp,"* Date: %s *\n",my_gettime());
  fprintf(fp,"*****************************\n");
  return;
}

void init_spice_net_info(t_spice_net_info* spice_net_info) {
  if (NULL == spice_net_info) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_net_info!\n", __FILE__, __LINE__);
    exit(1);
  }

  spice_net_info->density = 0.;
  spice_net_info->freq = 0.;

  assert((1 == default_signal_init_value)||(0 == default_signal_init_value));
  spice_net_info->init_val = default_signal_init_value;
  spice_net_info->probability = (float)default_signal_init_value;

  spice_net_info->pwl = 0.;
  spice_net_info->pwh = 0.;
  spice_net_info->slew_rise = 0.;
  
  return;
}

/* Find the spice model for Input Pad
 */
t_spice_model* find_inpad_spice_model(int num_spice_model,
                                      t_spice_model* spice_models) {
  t_spice_model* ret = NULL;
  int imodel;
  int num_found = 0;

  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if (SPICE_MODEL_INPAD == spice_models[imodel].type) {
      ret = &(spice_models[imodel]);
      num_found++;
    }
  }

  assert(1 == num_found);

  return ret;
}


char* generate_string_spice_model_type(enum e_spice_model_type spice_model_type) {
  char* ret = NULL;

  switch (spice_model_type) {
  case SPICE_MODEL_WIRE:
    ret = "wire";
    break;
  case SPICE_MODEL_MUX:
    ret = "Multiplexer";
    break;
  case SPICE_MODEL_LUT:
    ret = "Look-Up Table";
    break;
  case SPICE_MODEL_FF:
    ret = "Flip-flop";
    break;
  case SPICE_MODEL_INPAD:
    ret = "Input PAD";
    break;
  case SPICE_MODEL_OUTPAD:
    ret = "Output PAD";
    break;
  case SPICE_MODEL_SRAM:
    ret = "SRAM";
    break;
  case SPICE_MODEL_HARDLOGIC:
    ret = "hard_logic";
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return ret;
}

/* Deteremine the side of a io grid */
int determine_io_grid_side(int x,
                           int y) {
  /* TOP side IO of FPGA */
  if ((ny + 1) == y) {
    /* Make sure a valid x, y */
    assert((!(0 > x))&&(x < (nx + 1)));
    return BOTTOM; /* Such I/O has only Bottom side pins */
  } else if ((nx + 1) == x) { /* RIGHT side IO of FPGA */
    /* Make sure a valid x, y */
    assert((!(0 > y))&&(y < (ny + 1)));
    return LEFT; /* Such I/O has only Left side pins */
  } else if (0 == y) { /* BOTTOM side IO of FPGA */
    /* Make sure a valid x, y */
    assert((!(0 > x))&&(x < (nx + 1)));
    return TOP; /* Such I/O has only Top side pins */
  } else if (0 == x) { /* LEFT side IO of FPGA */
    /* Make sure a valid x, y */
    assert((!(0 > y))&&(y < (ny + 1)));
    return RIGHT; /* Such I/O has only Right side pins */
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])I/O Grid is in the center part of FPGA! Currently unsupported!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

void find_prev_rr_nodes_with_src(t_rr_node* src_rr_node,
                                 int* num_drive_rr_nodes,
                                 t_rr_node*** drive_rr_nodes,
                                 int** switch_indices) {
  int inode, iedge, next_node;
  int cur_index, switch_index;  

  assert(NULL != src_rr_node);
  assert(NULL != num_drive_rr_nodes);
  assert(NULL != switch_indices); 
  
  (*num_drive_rr_nodes) = 0;
  (*drive_rr_nodes) = NULL;
  (*switch_indices) = NULL;

  switch_index = -1;
  /* Determine num_drive_rr_node */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
      next_node = rr_node[inode].edges[iedge];
      if (src_rr_node == &(rr_node[next_node])) {
        /* Get the spice_model */
        if (-1 == switch_index) {
          switch_index = rr_node[inode].switches[iedge];
        } else { /* Make sure the switches are the same*/
          assert(switch_index == rr_node[inode].switches[iedge]); 
        }
        (*num_drive_rr_nodes)++;
      }
    }
  }
  /* Malloc */
  (*drive_rr_nodes) = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*(*num_drive_rr_nodes));
  (*switch_indices) = (int*)my_malloc(sizeof(int)*(*num_drive_rr_nodes)); 

  /* Find all the rr_nodes that drive current_rr_node*/
  cur_index = 0;
  for (inode = 0; inode < num_rr_nodes; inode++) {
    for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
      next_node = rr_node[inode].edges[iedge];
      if (src_rr_node == &(rr_node[next_node])) {
        /* Update drive_rr_nodes list */
        (*drive_rr_nodes)[cur_index] = &(rr_node[inode]);
        (*switch_indices)[cur_index] = rr_node[inode].switches[iedge];
        cur_index++;
      }
    }
  }
  assert(cur_index == (*num_drive_rr_nodes));

  return;
}


int find_path_id_prev_rr_node(int num_drive_rr_nodes,
                              t_rr_node** drive_rr_nodes,
                              t_rr_node* src_rr_node) {
  int path_id, inode;

  /* Configuration bits for this MUX*/
  path_id = -1;
  for (inode = 0; inode < num_drive_rr_nodes; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[src_rr_node->prev_node])) {
      path_id = inode;
      break;
    }
  }
  assert((-1 != path_id)&&(path_id < src_rr_node->fan_in));

  return path_id;
}

int pb_pin_net_num(t_rr_node* pb_rr_graph, 
                   t_pb_graph_pin* pin) {
  int net_num = OPEN;

  if (NULL == pb_rr_graph) {
    /* Try the temp_net_num in pb_graph_pin */
    net_num = pin->temp_net_num;
  } else {
    net_num = pb_rr_graph[pin->pin_count_in_cluster].net_num;
  }

  return net_num;
}

float pb_pin_density(t_rr_node* pb_rr_graph, 
                     t_pb_graph_pin* pin) {
  float density = 0.;
  int net_num;

  if (NULL == pb_rr_graph) {
    /* Try the temp_net_num in pb_graph_pin */
    net_num = pin->temp_net_num;
    if (OPEN != net_num) {
      density = vpack_net[net_num].spice_net_info->density;
    }
    return density;
  }
  net_num = pb_rr_graph[pin->pin_count_in_cluster].net_num;

  if (OPEN != net_num) {
    density = vpack_net[net_num].spice_net_info->density;
  }

  return density;
}

float pb_pin_probability(t_rr_node* pb_rr_graph, 
                         t_pb_graph_pin* pin) {
  float probability = (float)(default_signal_init_value); 
  int net_num;

  if (NULL == pb_rr_graph) {
    /* Try the temp_net_num in pb_graph_pin */
    net_num = pin->temp_net_num;
    if (OPEN != net_num) {
      probability = vpack_net[net_num].spice_net_info->probability;
    }
    return probability;
  }
  net_num = pb_rr_graph[pin->pin_count_in_cluster].net_num;

  if (OPEN != net_num) {
    probability = vpack_net[net_num].spice_net_info->probability;
  }

  return probability;
}

int pb_pin_init_value(t_rr_node* pb_rr_graph, 
                      t_pb_graph_pin* pin) {
  float init_val = (float)(default_signal_init_value); 
  int net_num;

  if (NULL == pb_rr_graph) {
    /* TODO: we know initialize to vdd could reduce the leakage power od multiplexers!
     *       But I can this as an option !
     */
    /* Try the temp_net_num in pb_graph_pin */
    net_num = pin->temp_net_num;
    if (OPEN != net_num) {
      init_val = vpack_net[net_num].spice_net_info->init_val;
    }
    return init_val;
  }
  net_num = pb_rr_graph[pin->pin_count_in_cluster].net_num;

  if (OPEN != net_num) {
    init_val = vpack_net[net_num].spice_net_info->init_val;
  }

  return init_val;
}

float get_rr_node_net_density(t_rr_node node) {
  /* If we found this net is OPEN, we assume it zero-density */
  if (OPEN == node.vpack_net_num) { 
    return 0.;
  } else {
    return vpack_net[node.vpack_net_num].spice_net_info->density;
  }
}

float get_rr_node_net_probability(t_rr_node node) {
  /* If we found this net is OPEN, we assume it zero-probability */
  if (OPEN == node.vpack_net_num) { 
    /* TODO: we know initialize to vdd could reduce the leakage power od multiplexers!
     *       But I can this as an option !
     */
    return (float)(default_signal_init_value); 
  } else {
    return vpack_net[node.vpack_net_num].spice_net_info->probability;
  }
}

int get_rr_node_net_init_value(t_rr_node node) {
  /* If we found this net is OPEN, we assume it zero-probability */
  if (OPEN == node.vpack_net_num) { 
    /* TODO: we know initialize to vdd could reduce the leakage power od multiplexers!
     *       But I can this as an option !
     */
    return (float)(default_signal_init_value); 
  } else {
    return vpack_net[node.vpack_net_num].spice_net_info->init_val;
  }
}

int find_parent_pb_type_child_index(t_pb_type* parent_pb_type,
                                    int mode_index,
                                    t_pb_type* child_pb_type) {
  int i;

  assert(NULL != parent_pb_type);
  assert(NULL != child_pb_type);
  assert((!(0 > mode_index))&&(mode_index < parent_pb_type->num_modes));

  for (i = 0; i < parent_pb_type->modes[mode_index].num_pb_type_children; i++) {
    if (child_pb_type == &(parent_pb_type->modes[mode_index].pb_type_children[i])) {
      assert(0 == strcmp(child_pb_type->name, parent_pb_type->modes[mode_index].pb_type_children[i].name));
      return i;
    }
  }
  
  return -1;
}

/* Rule in generating a unique name: 
 * name of current pb =  <parent_pb_name_tag>_<cur_pb_graph_node>[index]
 */
void gen_spice_name_tag_pb_rec(t_pb* cur_pb,
                               char* prefix) {
  char* prefix_rec = NULL; 
  int ipb, jpb, mode_index; 

  mode_index = cur_pb->mode;

  /* Free previous name_tag if there is */
  /* my_free(cur_pb->spice_name_tag); */

  /* Generate the name_tag */
  if ((0 < cur_pb->pb_graph_node->pb_type->num_modes)
    &&(NULL == cur_pb->pb_graph_node->pb_type->spice_model_name)) {
    prefix_rec = (char*)my_malloc(sizeof(char)*(strlen(prefix) + 1 + strlen(cur_pb->pb_graph_node->pb_type->name) + 1
                                              + strlen(my_itoa(cur_pb->pb_graph_node->placement_index)) + 7 + strlen(cur_pb->pb_graph_node->pb_type->modes[mode_index].name) + 2 ));
    sprintf(prefix_rec, "%s_%s[%d]_mode[%s]", 
            prefix, cur_pb->pb_graph_node->pb_type->name, cur_pb->pb_graph_node->placement_index, cur_pb->pb_graph_node->pb_type->modes[mode_index].name);
    cur_pb->spice_name_tag = my_strdup(prefix_rec);
  } else {
    assert((0 == cur_pb->pb_graph_node->pb_type->num_modes)
          ||(NULL != cur_pb->pb_graph_node->pb_type->spice_model_name));
    prefix_rec = (char*)my_malloc(sizeof(char)*(strlen(prefix) + 1 + strlen(cur_pb->pb_graph_node->pb_type->name) + 1
                                              + strlen(my_itoa(cur_pb->pb_graph_node->placement_index)) + 2 ));
    sprintf(prefix_rec, "%s_%s[%d]", 
            prefix, cur_pb->pb_graph_node->pb_type->name, cur_pb->pb_graph_node->placement_index);
    cur_pb->spice_name_tag = my_strdup(prefix_rec);
  }

  /* When reach the leaf, we directly return */
  /* Recursive until reach the leaf */
  if ((0 == cur_pb->pb_graph_node->pb_type->num_modes)
     ||(NULL == cur_pb->child_pbs)) {
    return;
  }
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      //if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) { 
        /* Try to simplify the name tag... to avoid exceeding the length of SPICE name (up to 1024 chars) */
        /* gen_spice_name_tag_pb_rec(&(cur_pb->child_pbs[ipb][jpb]),prefix); */
        gen_spice_name_tag_pb_rec(&(cur_pb->child_pbs[ipb][jpb]),prefix_rec); 
      //}
    }
  }
 
  my_free(prefix_rec);
 
  return;
}


/* Generate a unique name tag for each pb, 
 * to identify it in both SPICE netlist and Power Modeling.
 */
void gen_spice_name_tags_all_pbs() {
  int iblk;
  char* prefix = NULL;

  for (iblk = 0; iblk < num_blocks; iblk++) {
    prefix = (char*)my_malloc(sizeof(char)*(5 + strlen(my_itoa(block[iblk].x)) + 2 + strlen(my_itoa(block[iblk].y)) + 2));
    sprintf(prefix, "grid[%d][%d]", block[iblk].x, block[iblk].y);
    gen_spice_name_tag_pb_rec(block[iblk].pb, prefix);
    my_free(prefix);
  }

  return;
}

/* Make sure the edge has only one input pin and output pin*/
void check_pb_graph_edge(t_pb_graph_edge pb_graph_edge) {
  assert(1 == pb_graph_edge.num_input_pins);
  assert(1 == pb_graph_edge.num_output_pins);

  return;
}

/* Check all the edges for a given pb_graph_pin*/
void check_pb_graph_pin_edges(t_pb_graph_pin pb_graph_pin) {
  int iedge;
 
  for (iedge = 0; iedge < pb_graph_pin.num_input_edges; iedge++) {
    check_pb_graph_edge(*(pb_graph_pin.input_edges[iedge]));
  }
  
  for (iedge = 0; iedge < pb_graph_pin.num_output_edges; iedge++) {
    check_pb_graph_edge(*(pb_graph_pin.output_edges[iedge]));
  }
 
  return;
}

int find_pb_mapped_logical_block_rec(t_pb* cur_pb,
                                     t_spice_model* pb_spice_model, 
                                     char* pb_spice_name_tag) {
  int logical_block_index = OPEN;
  int mode_index, ipb, jpb;

  assert(NULL != cur_pb);

  if ((pb_spice_model == cur_pb->pb_graph_node->pb_type->spice_model)
    &&(0 == strcmp(cur_pb->spice_name_tag, pb_spice_name_tag))) {
    /* Special for LUT... They have sub modes!!!*/
    if (SPICE_MODEL_LUT == pb_spice_model->type) {
      mode_index = cur_pb->mode;
      assert(NULL != cur_pb->child_pbs);
      return cur_pb->child_pbs[0][0].logical_block; 
    }
    assert(pb_spice_model == logical_block[cur_pb->logical_block].mapped_spice_model);
    return cur_pb->logical_block;
  }
  
  /* Go recursively ... */
  mode_index = cur_pb->mode;
  if (0 == cur_pb->pb_graph_node->pb_type->num_modes) {
    return logical_block_index;
  }
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        logical_block_index = 
        find_pb_mapped_logical_block_rec(&(cur_pb->child_pbs[ipb][jpb]), pb_spice_model, pb_spice_name_tag);
        if (OPEN != logical_block_index) {
          return logical_block_index;
        }
      }
    }
  }
  
  return logical_block_index;
}

int find_grid_mapped_logical_block(int x, int y,
                                   t_spice_model* pb_spice_model,
                                   char* pb_spice_name_tag) {
  int logical_block_index = OPEN;
  int iblk;

  /* Find the grid usage */
  if (0 == grid[x][y].usage) { 
    return logical_block_index;
  } else {
    assert(0 < grid[x][y].usage);
    /* search each block */
    for (iblk = 0; iblk < grid[x][y].usage; iblk++) {
      /* Get the pb */
      logical_block_index = find_pb_mapped_logical_block_rec(block[grid[x][y].blocks[iblk]].pb,
                                                             pb_spice_model, pb_spice_name_tag);
      if (OPEN != logical_block_index) {
        return logical_block_index;
      }
    }
  }
  
  return logical_block_index;
}

void stats_pb_graph_node_port_pin_numbers(t_pb_graph_node* cur_pb_graph_node,
                                          int* num_inputs,
                                          int* num_outputs,
                                          int* num_clock_pins) {
  int iport;

  assert(NULL != cur_pb_graph_node);

  (*num_inputs) = 0;
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    (*num_inputs) += cur_pb_graph_node->num_input_pins[iport];
  }
  (*num_outputs) = 0;
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    (*num_outputs) += cur_pb_graph_node->num_output_pins[iport];
  }
  (*num_clock_pins) = 0;
  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    (*num_clock_pins) += cur_pb_graph_node->num_clock_pins[iport];
  }

  return;
}

int recommend_num_sim_clock_cycle() {
  float avg_density = 0.;
  float median_density = 0.;
  int recmd_num_sim_clock_cycle = 0;
  int inet, jnet;
  int net_cnt = 0;
  float* density_value = NULL;
  int* sort_index = NULL;
  int* net_to_sort_index_mapping = NULL;

  /* get the average density of all the nets */
  for (inet = 0; inet < num_logical_nets; inet++) {
    assert(NULL != vpack_net[inet].spice_net_info);
    if ((FALSE == vpack_net[inet].is_global)
     &&(FALSE == vpack_net[inet].is_const_gen)
     &&(0. != vpack_net[inet].spice_net_info->density)) {
      avg_density += vpack_net[inet].spice_net_info->density; 
      net_cnt++;
    }
  }
  avg_density = avg_density/net_cnt; 
  /* Fill the array to be sorted */
  density_value = (float*)my_malloc(sizeof(float)*net_cnt);
  sort_index = (int*)my_malloc(sizeof(int)*net_cnt);
  net_to_sort_index_mapping = (int*)my_malloc(sizeof(int)*net_cnt);
  jnet = 0;
  for (inet = 0; inet < num_logical_nets; inet++) {
    assert(NULL != vpack_net[inet].spice_net_info);
    if ((FALSE == vpack_net[inet].is_global)
     &&(FALSE == vpack_net[inet].is_const_gen)
     &&(0. != vpack_net[inet].spice_net_info->density)) {
      sort_index[jnet] = jnet;
      net_to_sort_index_mapping[jnet] = inet;
      density_value[jnet] = vpack_net[inet].spice_net_info->density;
      jnet++;
    }
  }
  assert(jnet == net_cnt);
  /* Sort the density */
  quicksort_float_index(net_cnt, sort_index, density_value);
  /* Get the median */
  median_density = vpack_net[sort_index[(int)net_cnt/2]].spice_net_info->density;
  
  recmd_num_sim_clock_cycle = (int)(1/avg_density); 
  vpr_printf(TIO_MESSAGE_INFO, "Average net density: %.2g\n", avg_density);
  vpr_printf(TIO_MESSAGE_INFO, "Net density median: %.2g\n", median_density);
  vpr_printf(TIO_MESSAGE_INFO, "Recommend no. of clock cycles: %d\n", recmd_num_sim_clock_cycle);

  /* Free */
  my_free(sort_index);
  my_free(density_value);
  my_free(net_to_sort_index_mapping);

  return recmd_num_sim_clock_cycle; 
}

void auto_select_num_sim_clock_cycle(t_spice* spice) {
  int recmd_num_sim_clock_cycle = recommend_num_sim_clock_cycle();

  /* Auto select number of simulation clock cycles*/
  if (-1 == spice->spice_params.meas_params.sim_num_clock_cycle) {
    vpr_printf(TIO_MESSAGE_INFO, "Auto select the no. of clock cycles in simulation: %d\n", recmd_num_sim_clock_cycle);
    spice->spice_params.meas_params.sim_num_clock_cycle = recmd_num_sim_clock_cycle;
  }

  return; 
}

/* Malloc grid_index_low and grid_index_high for a spice_model */
void alloc_spice_model_grid_index_low_high(t_spice_model* cur_spice_model) {
  int ix, iy;

  /* grid_index_low */
  /* x - direction*/
  cur_spice_model->grid_index_low = (int**)my_malloc(sizeof(int*)*(nx + 2));
  /* y - direction*/
  for (ix = 0; ix < (nx + 2); ix++) { 
    cur_spice_model->grid_index_low[ix] = (int*)my_malloc(sizeof(int)*(ny + 2));
  }
  /* Initialize */
  for (ix = 0; ix < (nx + 2); ix++) { 
    for (iy = 0; iy < (ny + 2); iy++) { 
      cur_spice_model->grid_index_low[ix][iy] = 0;
    }
  }
  /* grid_index_high */
  /* x - direction*/
  cur_spice_model->grid_index_high = (int**)my_malloc(sizeof(int*)*(nx + 2));
  /* y - direction*/
  for (ix = 0; ix < (nx + 2); ix++) { 
    cur_spice_model->grid_index_high[ix] = (int*)my_malloc(sizeof(int)*(ny + 2));
  }
  /* Initialize */
  for (ix = 0; ix < (nx + 2); ix++) { 
    for (iy = 0; iy < (ny + 2); iy++) { 
      cur_spice_model->grid_index_high[ix][iy] = 0;
    }
  }

  return;
}

void update_one_spice_model_grid_index_low(int x, int y, 
                                           t_spice_model* cur_spice_model) {
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1))));
  assert((!(0 > y))&&(!(y > (ny + 1))));
  assert(NULL != cur_spice_model);
  assert(NULL != cur_spice_model->grid_index_low);
  assert(NULL != cur_spice_model->grid_index_low[x]); 

  /* Assigne the low */ 
  cur_spice_model->grid_index_low[x][y] = cur_spice_model->cnt;

  return;
}

void update_spice_models_grid_index_low(int x, int y, 
                                        int num_spice_models, 
                                        t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    update_one_spice_model_grid_index_low(x, y, &(spice_model[i]));
  }

  return;
}

void update_one_spice_model_grid_index_high(int x, int y, 
                                            t_spice_model* cur_spice_model) {
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1))));
  assert((!(0 > y))&&(!(y > (ny + 1))));
  assert(NULL != cur_spice_model);
  assert(NULL != cur_spice_model->grid_index_high);
  assert(NULL != cur_spice_model->grid_index_high[x]); 

  /* Assigne the low */ 
  cur_spice_model->grid_index_high[x][y] = cur_spice_model->cnt;

  return;
}

void update_spice_models_grid_index_high(int x, int y, 
                                         int num_spice_models, 
                                         t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    update_one_spice_model_grid_index_high(x, y, &(spice_model[i]));
  }

  return;
}

char* gen_str_spice_model_structure(enum e_spice_model_structure spice_model_structure) {
  switch (spice_model_structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    return "tree-like"; 
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    return "one-level"; 
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    return "multi-level"; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid spice model structure!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/* Check if the spice model structure is the same with the switch_inf structure */
boolean check_spice_model_structure_match_switch_inf(t_switch_inf target_switch_inf) {
  assert(NULL != target_switch_inf.spice_model);
  if (target_switch_inf.structure != target_switch_inf.spice_model->structure) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure in spice_model(%s) is different from switch_inf[%s]!\n",
               __FILE__, __LINE__, target_switch_inf.spice_model->name, target_switch_inf.name);
    return FALSE;
  }
  return TRUE;
}

int find_pb_type_idle_mode_index(t_pb_type cur_pb_type) {
  int idle_mode_index = 0;
  int imode = 0;
  int num_idle_mode = 0;

  /* if we touch the leaf node */
  if (NULL != cur_pb_type.blif_model) {
    return 0;
  }

  if (0 == cur_pb_type.num_modes) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Intend to find the idle mode while cur_pb_type has 0 modes!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Normal Condition: */ 
  for (imode = 0; imode < cur_pb_type.num_modes; imode++) {
    if (1 == cur_pb_type.modes[imode].define_idle_mode) {
      idle_mode_index = imode;
      num_idle_mode++;
    }
  } 
  assert(1 == num_idle_mode); 

  return idle_mode_index;
}


void mark_grid_type_pb_graph_node_pins_temp_net_num(int x, int y) {
  int iport, ipin, type_pin_index, class_id, pin_global_rr_node_id; 
  t_type_ptr type = NULL;
  t_pb_graph_node* top_pb_graph_node = NULL;
  int mode_index, ipb, jpb;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 1)));  
  assert((!(y < 0))&&(y < (ny + 1)));  

  type = grid[x][y].type;

  if (EMPTY_TYPE == type) {
    return; /* Bypass empty grid */
  }

  top_pb_graph_node = type->pb_graph_head;
  /* Input ports */
  for (iport = 0; iport < top_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_input_pins[iport]; ipin++) {
      top_pb_graph_node->input_pins[iport][ipin].temp_net_num = OPEN;
      type_pin_index = top_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
      class_id = type->pin_class[type_pin_index];
      assert(RECEIVER == type->class_inf[class_id].type);
      /* Find the pb net_num and update OPIN net_num */
      pin_global_rr_node_id = get_rr_node_index(x, y, IPIN, type_pin_index, rr_node_indices);
      if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
        top_pb_graph_node->input_pins[iport][ipin].temp_net_num = OPEN;
        continue;
      } 
      top_pb_graph_node->input_pins[iport][ipin].temp_net_num = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
    } 
  }
  /* Output ports */
  for (iport = 0; iport < top_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport]; ipin++) {
      top_pb_graph_node->output_pins[iport][ipin].temp_net_num = OPEN;
      type_pin_index = top_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      class_id = type->pin_class[type_pin_index];
      assert(DRIVER == type->class_inf[class_id].type);
      /* Find the pb net_num and update OPIN net_num */
      pin_global_rr_node_id = get_rr_node_index(x, y, OPIN, type_pin_index, rr_node_indices);
      if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
        top_pb_graph_node->output_pins[iport][ipin].temp_net_num = OPEN;
        continue;
      } 
      top_pb_graph_node->output_pins[iport][ipin].temp_net_num = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
    } 
  }
  /* clock ports */
  for (iport = 0; iport < top_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_clock_pins[iport]; ipin++) {
      top_pb_graph_node->clock_pins[iport][ipin].temp_net_num = OPEN;
      type_pin_index = top_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster;
      class_id = type->pin_class[type_pin_index];
      assert(RECEIVER == type->class_inf[class_id].type);
      /* Find the pb net_num and update OPIN net_num */
      pin_global_rr_node_id = get_rr_node_index(x, y, IPIN, type_pin_index, rr_node_indices);
      if (OPEN == rr_node[pin_global_rr_node_id].net_num) {
        top_pb_graph_node->clock_pins[iport][ipin].temp_net_num = OPEN;
        continue;
      } 
      top_pb_graph_node->clock_pins[iport][ipin].temp_net_num = clb_to_vpack_net_mapping[rr_node[pin_global_rr_node_id].net_num];
    } 
  }

  /* Go recursively ... */
  mode_index = find_pb_type_idle_mode_index(*(top_pb_graph_node->pb_type));
  for (ipb = 0; ipb < top_pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < top_pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Mark pb_graph_node temp_net_num */
      rec_mark_pb_graph_node_temp_net_num(&(top_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]));
    }
  }

  return; 
}

/* Mark temp_net_num in current pb_graph_node from the parent pb_graph_node */
void rec_mark_pb_graph_node_temp_net_num(t_pb_graph_node* cur_pb_graph_node) {
  int iport, ipin, iedge;
  int mode_index, ipb, jpb;

  assert(NULL != cur_pb_graph_node);

  /* Input ports */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      cur_pb_graph_node->input_pins[iport][ipin].temp_net_num = OPEN;
      /* TODO: I assume by default the index of selected edge is 0 */
      cur_pb_graph_node->input_pins[iport][ipin].temp_net_num = cur_pb_graph_node->input_pins[iport][ipin].input_edges[0]->input_pins[0]->temp_net_num;
    }
  }
  /* Clock ports */
  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
      cur_pb_graph_node->clock_pins[iport][ipin].temp_net_num = OPEN;
      /* TODO: I assume by default the index of selected edge is 0 */
      cur_pb_graph_node->clock_pins[iport][ipin].temp_net_num = cur_pb_graph_node->clock_pins[iport][ipin].input_edges[0]->input_pins[0]->temp_net_num;
    }
  }
  /* Output ports */
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      /* TODO: I assume by default the index of selected edge is 0 */
      cur_pb_graph_node->output_pins[iport][ipin].temp_net_num = OPEN;
      for (iedge = 0; iedge < cur_pb_graph_node->output_pins[iport][ipin].num_output_edges; iedge++) {
        if (&(cur_pb_graph_node->output_pins[iport][ipin]) == cur_pb_graph_node->output_pins[iport][ipin].output_edges[iedge]->output_pins[0]->input_edges[0]->input_pins[0]) {
          cur_pb_graph_node->output_pins[iport][ipin].temp_net_num = cur_pb_graph_node->output_pins[iport][ipin].output_edges[0]->output_pins[0]->temp_net_num;
        }
      }
    }
  }

  if (NULL != cur_pb_graph_node->pb_type->spice_model) {
    return;
  }

  /* Go recursively ... */
  mode_index = find_pb_type_idle_mode_index(*(cur_pb_graph_node->pb_type));
  for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Mark pb_graph_node temp_net_num */
      rec_mark_pb_graph_node_temp_net_num(&(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]));
    }
  }

  return;
}

int check_consistency_logical_block_net_num(t_logical_block* lgk_blk, 
                                            int num_inputs, int* input_net_num) {
  int i, iport, ipin, net_eq;
  int consistency = 1;
  int* input_net_num_mapped = (int*)my_calloc(num_inputs, sizeof(int));
  
  for (iport = 0; iport < lgk_blk->pb->pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < lgk_blk->pb->pb_graph_node->num_input_pins[iport]; ipin++) {
      if (OPEN == lgk_blk->input_nets[iport][ipin]) {
        continue; /* bypass unused pins */
      }
      /* Initial net_eq */
      net_eq = 0;
      /* Check if this net can be found in the input net_num */
      for (i = 0; i < num_inputs; i++) {
        if (1 == input_net_num_mapped[i]) {
          continue;
        }
        if (input_net_num[i] == lgk_blk->input_nets[iport][ipin]) {
          net_eq = 1;
          input_net_num_mapped[i] = 1;
          break;
        }
      }
      if (0 == net_eq) {
        consistency = 0;
        break;
      }
    }
    if (0 == consistency) {
      break;
    }
  }

  /* Free */
  my_free(input_net_num_mapped);
   
  return consistency; 
}

/* Determine if this rr_node is driving this switch box (x,y)
 * For more than length-1 wire, the fan-in of a des_rr_node in a switch box
 * contain all the drivers in the switch boxes that it passes through.
 * This function is to identify if the src_rr_node is the driver in this switch box
 */
int rr_node_drive_switch_box(t_rr_node* src_rr_node,
                             t_rr_node* des_rr_node,
                             int switch_box_x,
                             int switch_box_y,
                             int chan_side) {
  
  /* Make sure a valid src_rr_node and des_rr_node */
  if (NULL == src_rr_node) {
  assert(NULL != src_rr_node);
  }
  assert(NULL != des_rr_node);
  /* The src_rr_node should be either CHANX or CHANY */
  assert((CHANX == des_rr_node->type)||(CHANY == des_rr_node->type));
  /* Valid switch_box coordinator */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 
  /* Valid des_rr_node coordinator */
  assert((!(switch_box_x < (des_rr_node->xlow-1)))&&(!(switch_box_x > (des_rr_node->xhigh+1))));
  assert((!(switch_box_y < (des_rr_node->ylow-1)))&&(!(switch_box_y > (des_rr_node->yhigh+1))));

  /* Check the src_rr_node coordinator */
  switch (chan_side) {
  case TOP:
    /* Following cases:
     *               |
     *             / | \
     */
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == des_rr_node->direction)&&(CHANY == des_rr_node->type));
    /* depend on the type of src_rr_node */
    switch (src_rr_node->type) {
    case OPIN:
      if (((switch_box_y + 1) == src_rr_node->ylow)
         &&((switch_box_x == src_rr_node->xlow)||((switch_box_x + 1) == src_rr_node->xlow))) {
        return 1;
      }
      break;
    case CHANX:
      assert(src_rr_node->ylow == src_rr_node->yhigh);
      if ((switch_box_y == src_rr_node->ylow)
         &&(!(switch_box_x < (src_rr_node->xlow-1)))&&(!(switch_box_x > (src_rr_node->xhigh+1)))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!(switch_box_y < src_rr_node->ylow))&&(!(switch_box_y > src_rr_node->yhigh))) {
        return 1;
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid src_rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    break;
  case RIGHT:
    /* Following cases:
     *          \               
     *       ---  ----  
     *          /
     */
    /* The destination rr_node only have one condition!!! */
    assert((INC_DIRECTION == des_rr_node->direction)&&(CHANX == des_rr_node->type));
    /* depend on the type of src_rr_node */
    switch (src_rr_node->type) {
    case OPIN:
      if (((switch_box_x + 1) == src_rr_node->xlow)
         &&((switch_box_y == src_rr_node->ylow)||((switch_box_y + 1) == src_rr_node->ylow))) {
        return 1;
      }
      break;
    case CHANX:
      assert(src_rr_node->ylow == src_rr_node->yhigh);
      if ((switch_box_y == src_rr_node->ylow)
         &&(!(switch_box_x < src_rr_node->xlow))&&(!(switch_box_x > src_rr_node->xhigh))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!(switch_box_y < (src_rr_node->ylow-1)))&&(!(switch_box_y > (src_rr_node->yhigh+1)))) {
        return 1;
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid src_rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    break;
  case BOTTOM:
    /* Following cases:
     *          |               
     *        \   /  
     *          |
     */
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == des_rr_node->direction)&&(CHANY == des_rr_node->type));
    /* depend on the type of src_rr_node */
    switch (src_rr_node->type) {
    case OPIN:
      if ((switch_box_y == src_rr_node->ylow)
         &&((switch_box_x == src_rr_node->xlow)||((switch_box_x + 1) == src_rr_node->xlow))) {
        return 1;
      }
      break;
    case CHANX:
      assert(src_rr_node->ylow == src_rr_node->yhigh);
      if ((switch_box_y == src_rr_node->ylow)
         &&(!(switch_box_x < (src_rr_node->xlow-1)))&&(!(switch_box_x > (src_rr_node->xhigh+1)))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!((switch_box_y+1) < src_rr_node->ylow))&&(!((switch_box_y+1) > src_rr_node->yhigh))) {
        return 1;
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid src_rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    break;
  case LEFT: 
    /* Following cases:
     *           /               
     *       ---  ----  
     *           \
     */
    /* The destination rr_node only have one condition!!! */
    assert((DEC_DIRECTION == des_rr_node->direction)&&(CHANX == des_rr_node->type));
    /* depend on the type of src_rr_node */
    switch (src_rr_node->type) {
    case OPIN:
      if ((switch_box_x == src_rr_node->xlow)
         &&((switch_box_y == src_rr_node->ylow)||((switch_box_y + 1) == src_rr_node->ylow))) {
        return 1;
      }
      break;
    case CHANX:
      assert(src_rr_node->ylow == src_rr_node->yhigh);
      if ((switch_box_y == src_rr_node->ylow)
         &&(!((switch_box_x+1) < src_rr_node->xlow))&&(!((switch_box_x+1) > src_rr_node->xhigh))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!(switch_box_y < (src_rr_node->ylow-1)))&&(!(switch_box_y > (src_rr_node->yhigh+1)))) {
        return 1;
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid src_rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }
  
  return 0;
}

void find_drive_rr_nodes_switch_box(int switch_box_x,
                                    int switch_box_y,
                                    t_rr_node* src_rr_node,
                                    int chan_side,
                                    int return_num_only,
                                    int* num_drive_rr_nodes,
                                    t_rr_node*** drive_rr_nodes,
                                    int* switch_index) {
  int cur_index = 0;
  //int inode, iedge, next_node;
  int inode;  

  /* I decide to kill the codes that search all the edges, the running time is huge... */
  /* Determine the num_drive_rr_nodes */
  (*num_drive_rr_nodes) = 0;
  (*switch_index) = -1;

  for (inode = 0; inode < src_rr_node->num_drive_rr_nodes; inode++) {
    if (1 == rr_node_drive_switch_box(src_rr_node->drive_rr_nodes[inode], src_rr_node, 
                                      switch_box_x, switch_box_y, chan_side)) { 
      /* Get the spice_model */
      if (-1 == (*switch_index)) {
        (*switch_index) = src_rr_node->drive_switches[inode];
      } else { /* Make sure the switches are the same*/
        assert((*switch_index) == src_rr_node->drive_switches[inode]); 
      }
      (*num_drive_rr_nodes)++;
    }
  }
  
  //for (inode = 0; inode < num_rr_nodes; inode++) {
  //  for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
  //    next_node = rr_node[inode].edges[iedge];
  //    /* Make sure the coordinator is matched to this switch box*/
  //    if ((src_rr_node == &(rr_node[next_node]))
  //       &&(1 == rr_node_drive_switch_box(&(rr_node[inode]), src_rr_node, switch_box_x, switch_box_y, chan_side))) { 
  //      /* Get the spice_model */
  //      if (-1 == (*switch_index)) {
  //        (*switch_index) = rr_node[inode].switches[iedge];
  //      } else { /* Make sure the switches are the same*/
  //        assert((*switch_index) == rr_node[inode].switches[iedge]); 
  //      }
  //      (*num_drive_rr_nodes)++;
  //    }
  //  }
  //}

  /* Check and malloc*/
  assert((!(0 > (*num_drive_rr_nodes)))&&(!((*num_drive_rr_nodes) > src_rr_node->fan_in)));
  if (1 == return_num_only) {
    return;
  }
  (*drive_rr_nodes) = NULL;
  if (0 == (*num_drive_rr_nodes)) {
    return;
  }
  (*drive_rr_nodes) = (t_rr_node**)my_malloc(sizeof(t_rr_node*)*(*num_drive_rr_nodes));

  /* Find all the rr_nodes that drive current_rr_node*/
  cur_index = 0;
  (*switch_index) = -1;

  for (inode = 0; inode < src_rr_node->num_drive_rr_nodes; inode++) {
    if (1 == rr_node_drive_switch_box(src_rr_node->drive_rr_nodes[inode], src_rr_node, 
                                      switch_box_x, switch_box_y, chan_side)) { 
      /* Update drive_rr_nodes list */
      (*drive_rr_nodes)[cur_index] = src_rr_node->drive_rr_nodes[inode];
      /* Get the spice_model */
      if (-1 == (*switch_index)) {
        (*switch_index) = src_rr_node->drive_switches[inode];
      } else { /* Make sure the switches are the same*/
        assert((*switch_index) == src_rr_node->drive_switches[inode]); 
      }
      cur_index++;
    }
  }
  //for (inode = 0; inode < num_rr_nodes; inode++) {
  //  for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
  //    next_node = rr_node[inode].edges[iedge];
  //    /* Make sure the coordinator is matched to this switch box*/
  //    if ((src_rr_node == &(rr_node[next_node]))
  //       &&(1 == rr_node_drive_switch_box(&(rr_node[inode]), src_rr_node, switch_box_x, switch_box_y, chan_side))) { 
  //      /* Update drive_rr_nodes list */
  //      (*drive_rr_nodes)[cur_index] = &(rr_node[inode]);
  //      /* Get the spice_model */
  //      if (-1 == (*switch_index)) {
  //        (*switch_index) = rr_node[inode].switches[iedge];
  //      } else { /* Make sure the switches are the same*/
  //        assert((*switch_index) == rr_node[inode].switches[iedge]); 
  //      }
  //      cur_index++;
  //    }
  //  }
  //}
  /* Verification */
  assert(cur_index == (*num_drive_rr_nodes));

  return;
}

int is_sb_interc_between_segments(int switch_box_x, 
                                  int switch_box_y, 
                                  t_rr_node* src_rr_node, 
                                  int chan_side) {
  int inode;
  int cur_sb_num_drive_rr_nodes = 0;

  for (inode = 0; inode < src_rr_node->num_drive_rr_nodes; inode++) {
    if (1 == rr_node_drive_switch_box(src_rr_node->drive_rr_nodes[inode], src_rr_node, 
                                      switch_box_x, switch_box_y, chan_side)) { 
      cur_sb_num_drive_rr_nodes++;
    }
  }
  if (0 == cur_sb_num_drive_rr_nodes) {
    return 1;
  } else {
    return 0;
  }
}
