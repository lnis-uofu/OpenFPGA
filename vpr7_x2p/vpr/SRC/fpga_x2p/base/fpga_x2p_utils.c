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
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"

enum e_dir_err {
 E_DIR_NOT_EXIST,
 E_EXIST_BUT_NOT_DIR,
 E_DIR_EXIST
};


/***** Local Subroutines *****/
static 
enum e_dir_err try_access_dir(char* dir_path);

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
    vpr_printf(TIO_MESSAGE_WARNING,"Try access File(%s): exists!\n",file_path);
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

static 
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
       vpr_printf(TIO_MESSAGE_WARNING,
                  "Directory(%s) already exists. Will overwrite contents\n",
                  dir_path);
       return 1;
     }
     break;
   default:
     vpr_printf(TIO_MESSAGE_ERROR,"Create directory(%s)...Failed!\n",dir_path);
     exit(1);
     return 0;
   }
}

/* Cat string2 to the end of string1 */
char* my_strcat(const char* str1,
                const char* str2) {
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
    path = my_strdup("./");;
    prog_name = my_strdup(local_copy);
  } else if (len == split_pos) {
    /* In this case the progrom name is NULL... actually the prog_path is a directory*/
    path = my_strdup(local_copy);
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


/* Print SRAM bits, typically in a comment line */
void fprint_commented_sram_bits(FILE* fp,
                                int num_sram_bits, int* sram_bits) {
  int i;
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 
   
  for (i = 0; i < num_sram_bits; i++) { 
    fprintf(fp, "%d", sram_bits[i]);
  }

  return;
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

  if (0 == num_found) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Fail to find a spice model match name: %s !\n",
               __FILE__ ,__LINE__, spice_model_name);
    exit(1);
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

/* Tasks: 
 * 1. Search the inv_spice_model_name of each ports of a spice_model
 * 2. Copy the information from inverter spice model to higher level spice_models 
 */
void config_spice_model_port_inv_spice_model(int num_spice_models, 
                                             t_spice_model* spice_model) {
  int i, iport;
  t_spice_model* inv_spice_model = NULL;

  for (i = 0; i < num_spice_models; i++) {
    /* By pass inverters and buffers  */
    if (SPICE_MODEL_INVBUF == spice_model[i].type) {
      continue;
    }
    for (iport = 0; iport < spice_model[i].num_port; iport++) {
      /* Now we bypass non BL/WL ports */
      if ((SPICE_MODEL_PORT_BL != spice_model[i].ports[iport].type) 
           && (SPICE_MODEL_PORT_BLB != spice_model[i].ports[iport].type) 
           && (SPICE_MODEL_PORT_WL != spice_model[i].ports[iport].type) 
           && (SPICE_MODEL_PORT_WLB != spice_model[i].ports[iport].type)) {
        continue;
      }
      if (NULL == spice_model[i].ports[iport].inv_spice_model_name) {
        inv_spice_model = get_default_spice_model(SPICE_MODEL_INVBUF,
                                                  num_spice_models, spice_model);
      } else {
        inv_spice_model = find_name_matched_spice_model(spice_model[i].ports[iport].inv_spice_model_name,
                                                        num_spice_models, spice_model);
        /* We should find a buffer spice_model*/
        if (NULL == inv_spice_model) {
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find inv_spice_model to the port(name=%s) of spice_model(name=%s)!\n",
                     __FILE__, __LINE__, spice_model[i].ports[iport].prefix, spice_model[i].name);
          exit(1);
        }
      }
      /* Config */
      spice_model[i].ports[iport].inv_spice_model = inv_spice_model;
    }
  }
  return;
}

/* Find a spice model port by given name */
t_spice_model_port* find_spice_model_port_by_name(t_spice_model* cur_spice_model,
                                                  char* port_name) {
  int iport;
  t_spice_model_port* port = NULL;
  int cnt = 0;

  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    if (0 == strcmp(cur_spice_model->ports[iport].prefix, port_name)) {
      port = &(cur_spice_model->ports[iport]);
      cnt++; 
    }
  }

  assert ((0 == cnt) || (1 == cnt));

  return port;
}

void config_one_spice_model_buffer(int num_spice_models, 
                                   t_spice_model* spice_model,
                                   t_spice_model* cur_spice_model,
                                   t_spice_model_buffer* cur_spice_model_buffer) {
  t_spice_model* buf_spice_model = NULL;
  char* location_map = NULL;

  /* Check if this spice model has input buffers */
  if (1 == cur_spice_model_buffer->exist) {
    buf_spice_model = find_name_matched_spice_model(cur_spice_model_buffer->spice_model_name,
                                                    num_spice_models, spice_model);
    /* We should find a buffer spice_model*/
    if (NULL == buf_spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find inv/buffer spice_model to the input buffer of spice_model(name=%s)!\n",
                 __FILE__, __LINE__, cur_spice_model->name);
      exit(1);
    }
    /* Backup location map */
    location_map = cur_spice_model_buffer->location_map; 
    /* Copy the information from found spice model to current spice model*/
    memcpy(cur_spice_model_buffer, buf_spice_model->design_tech_info.buffer_info, sizeof(t_spice_model_buffer));
    /* Recover the spice_model_name and exist */
    cur_spice_model_buffer->exist = 1;
    cur_spice_model_buffer->spice_model_name = my_strdup(buf_spice_model->name);
    cur_spice_model_buffer->spice_model = buf_spice_model;
    cur_spice_model_buffer->location_map = location_map;
  }

  return;
}

/* Tasks: 
 * 1. Search the spice_model_name of input and output buffer and link to the spice_model
 * 2. Copy the information from input/output buffer spice model to higher level spice_models 
 */
void config_spice_model_input_output_buffers_pass_gate(int num_spice_models, 
                                                       t_spice_model* spice_model) {
  int i;
  t_spice_model* pgl_spice_model = NULL;

  for (i = 0; i < num_spice_models; i++) {
    /* By pass inverters and buffers  */
    if (SPICE_MODEL_INVBUF == spice_model[i].type) {
      continue;
    }

    /* Check if this spice model has input buffers */
    config_one_spice_model_buffer(num_spice_models, spice_model,
                                  &(spice_model[i]), spice_model[i].input_buffer);
 
    /* Check if this spice model has output buffers */
    config_one_spice_model_buffer(num_spice_models, spice_model,
                                  &(spice_model[i]), spice_model[i].output_buffer);

    /* If this spice_model is a LUT, check the lut_input_buffer */
    if (SPICE_MODEL_LUT == spice_model[i].type) {
      assert(1 == spice_model[i].lut_input_buffer->exist);
      assert(1 == spice_model[i].lut_input_inverter->exist);

      config_one_spice_model_buffer(num_spice_models, spice_model,
                                    &(spice_model[i]), spice_model[i].lut_input_buffer);

      config_one_spice_model_buffer(num_spice_models, spice_model,
                                    &(spice_model[i]), spice_model[i].lut_input_inverter);

      config_one_spice_model_buffer(num_spice_models, spice_model,
                                    &(spice_model[i]), spice_model[i].lut_intermediate_buffer);
    }

    
    /* Check pass_gate logic only for LUT and MUX */
    if ((SPICE_MODEL_LUT == spice_model[i].type)
       ||(SPICE_MODEL_MUX == spice_model[i].type)) {
      pgl_spice_model = find_name_matched_spice_model(spice_model[i].pass_gate_logic->spice_model_name,
                                                      num_spice_models, spice_model);
      /* We should find a buffer spice_model*/
      if (NULL == pgl_spice_model) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find pass_gate spice_model to the pass_gate_logic of spice_model(name=%s)!\n",
                   __FILE__, __LINE__, spice_model[i].name);
        exit(1);
      }
      /* Copy the information from found spice model to current spice model*/
      /* copy gate info if this is a standard cell */
      if (SPICE_MODEL_GATE == pgl_spice_model->type) {
        assert ( SPICE_MODEL_GATE_MUX2 == pgl_spice_model->design_tech_info.gate_info->type);
        spice_model[i].design_tech_info.gate_info = (t_spice_model_gate*)my_calloc(1, sizeof(t_spice_model_gate));
        memcpy(spice_model[i].design_tech_info.gate_info, pgl_spice_model->design_tech_info.gate_info, sizeof(t_spice_model_gate));
      } else { 
        assert (SPICE_MODEL_PASSGATE == pgl_spice_model->type);
        memcpy(spice_model[i].pass_gate_logic, pgl_spice_model->design_tech_info.pass_gate_info, sizeof(t_spice_model_pass_gate_logic));
      }
      /* Recover the spice_model_name */
      spice_model[i].pass_gate_logic->spice_model_name = my_strdup(pgl_spice_model->name);
      spice_model[i].pass_gate_logic->spice_model = pgl_spice_model;
    }
  }
  
  return;
}

/* Return the SPICE model ports wanted
 * ATTENTION: we use the pointer of spice model here although we don't modify anything of spice_model 
 *            but we have return input ports, whose pointer will be lost if the input is not the pointor of spice_model
 * BECAUSE spice_model will be a local copy if it is not a pointer. And it will be set free when this function
 * finishes. So the return pointers become invalid !
 */
t_spice_model_port** find_spice_model_ports(const t_spice_model* spice_model,
                                            enum e_spice_model_port_type port_type,
                                            int* port_num, boolean ignore_global_port) {
  int iport, cur;
  t_spice_model_port** ret = NULL;

  /* Check codes*/
  assert(NULL != port_num);
  assert(NULL != spice_model);

  /* Count the number of ports that match*/
  (*port_num) = 0;
  for (iport = 0; iport < spice_model->num_port; iport++) {
    /* ignore global port if user specified */
    if ((TRUE == ignore_global_port)
       &&(TRUE == spice_model->ports[iport].is_global)) {
      continue;
    }
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
    /* ignore global port if user specified */
    if ((TRUE == ignore_global_port)
       &&(TRUE == spice_model->ports[iport].is_global)) {
      continue;
    }
    if (port_type == spice_model->ports[iport].type) {
      ret[cur] = &(spice_model->ports[iport]);
      cur++;
    }
  }
  /* Check correctness*/
  assert(cur == (*port_num));
  
  return ret;
}

/* Find the configure done ports */
t_spice_model_port** find_spice_model_config_done_ports(t_spice_model* spice_model,
                                                        enum e_spice_model_port_type port_type,
                                                        int* port_num, boolean ignore_global_port) {
  int iport, cur;
  t_spice_model_port** ret = NULL;

  /* Check codes*/
  assert(NULL != port_num);
  assert(NULL != spice_model);

  /* Count the number of ports that match*/
  (*port_num) = 0;
  for (iport = 0; iport < spice_model->num_port; iport++) {
    /* ignore global port if user specified */
    if ((TRUE == ignore_global_port)
       &&(TRUE == spice_model->ports[iport].is_global)) {
      continue;
    }
    if ((port_type == spice_model->ports[iport].type)
       &&(TRUE == spice_model->ports[iport].is_config_enable)) {
      (*port_num)++;
    }
  }
  
  /* Initial the return pointers*/
  ret = (t_spice_model_port**)my_malloc(sizeof(t_spice_model_port*)*(*port_num));
  memset(ret, 0 , sizeof(t_spice_model_port*)*(*port_num));
  
  /* Fill the return pointers*/
  cur = 0;
  for (iport = 0; iport < spice_model->num_port; iport++) {
    /* ignore global port if user specified */
    if ((TRUE == ignore_global_port)
       &&(TRUE == spice_model->ports[iport].is_global)) {
      continue;
    }
    if ((port_type == spice_model->ports[iport].type)
       &&(TRUE == spice_model->ports[iport].is_config_enable)) {
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

/* Converter an integer to a binary string */
int* my_itobin_int(int in_int, int bin_len) {
  int* ret = (int*) my_calloc (bin_len, sizeof(int));
  int i, temp;

  /* Make sure we do not have any overflow! */
  assert ( (-1 < in_int) && (in_int < pow(2., bin_len)) );
  
  temp = in_int;
  for (i = 0; i < bin_len; i++) {
    if (1 == temp % 2) { 
      ret[i] = 1; 
    }
    temp = temp / 2;
  }
 
  return ret;
}


/* Converter an integer to a binary string */
char* my_itobin(int in_int, int bin_len) {
  char* ret = (char*) my_calloc (bin_len + 1, sizeof(char));
  int i, temp;

  /* Make sure we do not have any overflow! */
  assert ( (-1 < in_int) && (in_int < pow(2., bin_len)) );

  /* Initialize */
  for (i = 0; i < bin_len - 1; i++) {
    ret[i] = '0';
  }
  sprintf(ret + bin_len - 1, "%s", "0");
  
  temp = in_int;
  for (i = 0; i < bin_len; i++) {
    if (1 == temp % 2) { 
      ret[i] = '1'; 
    }
    temp = temp / 2;
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

/* Generate a filename (string) for a grid subckt SPICE netlist, 
 * with given x and y coordinates
 */
char* fpga_spice_create_one_subckt_filename(const char* file_name_prefix,
                                            int subckt_x, int subckt_y,
                                            char* file_name_postfix) {
  char* fname = NULL;
  
  if ( -1 == subckt_y ) {
    fname = (char*) my_malloc(sizeof(char) * (strlen(file_name_prefix)
                              + strlen(my_itoa(subckt_x)) + 1
                              + strlen(file_name_postfix) + 1));

    sprintf(fname, "%s%d_%s", 
            file_name_prefix, subckt_x, file_name_postfix);

  } else {
    fname = (char*) my_malloc(sizeof(char) * (strlen(file_name_prefix)
                              + strlen(my_itoa(subckt_x)) + 1 + strlen(my_itoa(subckt_y))
                              + strlen(file_name_postfix) + 1));

    sprintf(fname, "%s%d_%d%s", 
            file_name_prefix, subckt_x, subckt_y, file_name_postfix);
  }

  return fname;
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
  assert((y < (ny + 1))||(y == (ny + 1)));

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

/**
 * Split a string with strtok
 * Store each token in a char array
 * tokens (char**):
 *  tokens[i] (char*) : pointer to a string split by delims
 */

char** fpga_spice_strtok(char* str, 
                         char* delims, 
                         int* len) {
  char** ret;
  char* result;
  int cnt=0;
  int* lens;
  char* tmp;

  if (NULL == str) {
    printf("Warning: NULL string found in my_strtok!\n");
    return NULL;
  }

  tmp = my_strdup(str);
  result = strtok(tmp,delims);
  /*First scan to determine the size*/
  while(result != NULL) {
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
  while(result != NULL) {
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
  while(result != NULL) {
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

int get_opposite_side(int side){
 
  switch (side) {
  case 0:
    return 2;
  case 1:
    return 3;
  case 2:
    return 0;
  case 3:
    return 1;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index. Should be [0,3].\n",
               __FILE__, __LINE__);
    exit(1);
  }
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

char* convert_process_corner_to_string(enum e_process_corner process_corner) {
  switch(process_corner) {
  case BEST_CORNER:
    return "bc";
  case TYPICAL_CORNER:
    return "tc";
  case WORST_CORNER:
    return "wc";
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid process_corner !\n",
                __FILE__, __LINE__);
    exit(1);
  }
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

t_spice_model* find_iopad_spice_model(int num_spice_model,
                                      t_spice_model* spice_models) {
  t_spice_model* ret = NULL;
  int imodel;
  int num_found = 0;

  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if (SPICE_MODEL_IOPAD == spice_models[imodel].type) {
      ret = &(spice_models[imodel]);
      num_found++;
    }
  }

  assert(1 == num_found);

  return ret;
}

/* Check if the grid coorindate given is in the range */
boolean is_grid_coordinate_in_range(int x_min, 
                                    int x_max,
                                    int grid_x) {
  /* See if x is in the range */
  if ((x_min > grid_x)
     ||(x_max < grid_x)) {
    return FALSE;
  } 

  /* Reach here, means all in the range */
  return TRUE;
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
  case SPICE_MODEL_SRAM:
    ret = "SRAM";
    break;
  case SPICE_MODEL_HARDLOGIC:
    ret = "hard_logic";
    break;
  case SPICE_MODEL_IOPAD:
    ret = "iopad";
    break;
  case SPICE_MODEL_SCFF:
    ret = "Scan-chain Flip-flop";
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
    net_num = pb_rr_graph[pin->rr_node_index_physical_pb].net_num;
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
  net_num = pb_rr_graph[pin->rr_node_index_physical_pb].net_num;

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
  net_num = pb_rr_graph[pin->rr_node_index_physical_pb].net_num;

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
  net_num = pb_rr_graph[pin->rr_node_index_physical_pb].net_num;

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

int recommend_num_sim_clock_cycle(float sim_window_size) {
  float avg_density = 0.;
  float median_density = 0.;
  int recmd_num_sim_clock_cycle = 0;
  int inet, jnet;
  int net_cnt = 0;
  float* density_value = NULL;
  int* sort_index = NULL;
  int* net_to_sort_index_mapping = NULL;

  float weighted_avg_density = 0.;
  float net_weight = 0.;
  int weighted_net_cnt = 0;

  /* get the average density of all the nets */
  for (inet = 0; inet < num_logical_nets; inet++) {
    assert(NULL != vpack_net[inet].spice_net_info);
    if ((FALSE == vpack_net[inet].is_global)
     &&(FALSE == vpack_net[inet].is_const_gen)
     &&(0. != vpack_net[inet].spice_net_info->density)) {
      avg_density += vpack_net[inet].spice_net_info->density; 
      net_cnt++;
      /* Consider the weight of fan-out */
      if (0 == vpack_net[inet].num_sinks) {
        net_weight = 1;
      } else {
        assert( 0 < vpack_net[inet].num_sinks );
        net_weight = vpack_net[inet].num_sinks;
      }
      weighted_avg_density += vpack_net[inet].spice_net_info->density * net_weight;
      weighted_net_cnt += net_weight;
    }
  }
  avg_density = avg_density/net_cnt; 
  weighted_avg_density = weighted_avg_density/weighted_net_cnt; 

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
  median_density = vpack_net[sort_index[(int)(0.5*net_cnt)]].spice_net_info->density;

  /* It may be more reasonable to use median 
   * But, if median density is 0, we use average density
  */
  if ((0. == median_density) && (0. == avg_density)) {
    recmd_num_sim_clock_cycle = 1;
    vpr_printf(TIO_MESSAGE_WARNING, 
               "All the signal density is zero! No. of clock cycles in simulations are set to be %d!",
               recmd_num_sim_clock_cycle);
  } else if (0. == avg_density) {
      recmd_num_sim_clock_cycle = (int)round(1/median_density); 
  } else if (0. == median_density) {
      recmd_num_sim_clock_cycle = (int)round(1/avg_density);
  } else {
    /* add a sim window size to balance the weight of average density and median density
     * In practice, we find that there could be huge difference between avereage and median values 
     * For a reasonable number of simulation clock cycles, we do this window size.
     */
    recmd_num_sim_clock_cycle = (int)round(1 / (sim_window_size * avg_density + (1 - sim_window_size) * median_density ));
  }
  
  assert( 0 < recmd_num_sim_clock_cycle);

  vpr_printf(TIO_MESSAGE_INFO, "Average net density: %.2f\n", avg_density);
  vpr_printf(TIO_MESSAGE_INFO, "Median net density: %.2f\n", median_density);
  vpr_printf(TIO_MESSAGE_INFO, "Average net densityi after weighting: %.2f\n", weighted_avg_density);
  vpr_printf(TIO_MESSAGE_INFO, "Window size set for Simulation: %.2f\n", sim_window_size);
  vpr_printf(TIO_MESSAGE_INFO, "Net density after Window size : %.2f\n", 
                               (sim_window_size * avg_density + (1 - sim_window_size) * median_density));
  vpr_printf(TIO_MESSAGE_INFO, "Recommend no. of clock cycles: %d\n", recmd_num_sim_clock_cycle);

  /* Free */
  my_free(sort_index);
  my_free(density_value);
  my_free(net_to_sort_index_mapping);

  return recmd_num_sim_clock_cycle; 
}

void auto_select_num_sim_clock_cycle(t_spice* spice,
                                     float sim_window_size) {
  int recmd_num_sim_clock_cycle = recommend_num_sim_clock_cycle(sim_window_size);

  /* Auto select number of simulation clock cycles*/
  if (-1 == spice->spice_params.meas_params.sim_num_clock_cycle) {
    vpr_printf(TIO_MESSAGE_INFO, "Auto select the no. of clock cycles in simulation: %d\n", recmd_num_sim_clock_cycle);
    spice->spice_params.meas_params.sim_num_clock_cycle = recmd_num_sim_clock_cycle;
  } else {
    vpr_printf(TIO_MESSAGE_INFO, "No. of clock cycles in simulation is forced to be: %d\n",
               spice->spice_params.meas_params.sim_num_clock_cycle);
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

void free_one_spice_model_grid_index_low_high(t_spice_model* cur_spice_model) {
  int ix;

  for (ix = 0; ix < (nx + 2); ix++) {
    my_free(cur_spice_model->grid_index_high[ix]);
    my_free(cur_spice_model->grid_index_low[ix]);
  }

  my_free(cur_spice_model->grid_index_high);
  my_free(cur_spice_model->grid_index_low);

  return;
}

void free_spice_model_grid_index_low_high(int num_spice_models, 
                                          t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    free_one_spice_model_grid_index_low_high(&(spice_model[i]));
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

void zero_one_spice_model_grid_index_low_high(t_spice_model* cur_spice_model) {
  int ix, iy;
  /* Initialize */
  for (ix = 0; ix < (nx + 2); ix++) { 
    for (iy = 0; iy < (ny + 2); iy++) { 
      cur_spice_model->grid_index_high[ix][iy] = 0;
      cur_spice_model->grid_index_low[ix][iy] = 0;
    }
  }
  return;
}

void zero_spice_model_grid_index_low_high(int num_spice_models, 
                                          t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    zero_one_spice_model_grid_index_low_high(&(spice_model[i]));
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
  if (target_switch_inf.structure != target_switch_inf.spice_model->design_tech_info.mux_info->structure) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure in spice_model(%s) is different from switch_inf[%s]!\n",
               __FILE__, __LINE__, target_switch_inf.spice_model->name, target_switch_inf.name);
    return FALSE;
  }
  return TRUE;
}


void init_rr_nodes_vpack_net_num_changed(int LL_num_rr_nodes,
                                         t_rr_node* LL_rr_node) {
  int inode;

  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    LL_rr_node[inode].vpack_net_num_changed = FALSE;
  }

  return;
}

void init_rr_nodes_is_parasitic_net(int LL_num_rr_nodes,
                                    t_rr_node* LL_rr_node) {
  int inode;

  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    LL_rr_node[inode].is_parasitic_net = FALSE;
  }

  return;
}


/* Check if this net is connected to a PI*/
boolean is_net_pi(t_net* cur_net) {
  int src_blk_idx;

  assert(NULL != cur_net);

  src_blk_idx = cur_net->node_block[0];
  if (VPACK_INPAD == logical_block[src_blk_idx].type) {
    return TRUE;
  }
  return FALSE;
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
  assert(NULL != src_rr_node);
  assert(NULL != des_rr_node);
  /* The src_rr_node should be either CHANX or CHANY */
  assert((CHANX == des_rr_node->type)||(CHANY == des_rr_node->type));
  /* Valid switch_box coordinator */
  assert((!(0 > switch_box_x))&&(!(switch_box_x > (nx + 1)))); 
  assert((!(0 > switch_box_y))&&(!(switch_box_y > (ny + 1)))); 
  /* Valid des_rr_node coordinator */
  assert((!(switch_box_x < (des_rr_node->xlow - 1)))&&(!(switch_box_x > (des_rr_node->xhigh + 1))));
  assert((!(switch_box_y < (des_rr_node->ylow - 1)))&&(!(switch_box_y > (des_rr_node->yhigh + 1))));

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
         &&(!(switch_box_x < (src_rr_node->xlow - 1)))
         &&(!(switch_box_x > (src_rr_node->xhigh + 1)))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!(switch_box_y < src_rr_node->ylow))
         &&(!(switch_box_y > src_rr_node->yhigh))) {
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
         &&(!(switch_box_y < (src_rr_node->ylow - 1)))
         &&(!(switch_box_y > (src_rr_node->yhigh + 1)))) {
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
     *        \ | /  
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
         &&(!(switch_box_x < (src_rr_node->xlow - 1)))
         &&(!(switch_box_x > (src_rr_node->xhigh + 1)))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!((switch_box_y + 1) < src_rr_node->ylow))
         &&(!((switch_box_y + 1) > src_rr_node->yhigh))) {
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
    if (!((DEC_DIRECTION == des_rr_node->direction)&&(CHANX == des_rr_node->type)))
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
         &&(!((switch_box_x + 1) < src_rr_node->xlow))
         &&(!((switch_box_x + 1) > src_rr_node->xhigh))) {
        return 1;
      }
      break;
    case CHANY:
      assert(src_rr_node->xlow == src_rr_node->xhigh);
      if ((switch_box_x == src_rr_node->xlow)
         &&(!(switch_box_y < (src_rr_node->ylow - 1)))
         &&(!(switch_box_y > (src_rr_node->yhigh + 1)))) {
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

/* Reset the counter of each spice_model to be zero */
void zero_spice_models_cnt(int num_spice_models, t_spice_model* spice_model) {
  int imodel = 0; 

  for (imodel = 0; imodel < num_spice_models; imodel++) {
    spice_model[imodel].cnt = 0;
  }
 
  return;
}

void zero_one_spice_model_routing_index_low_high(t_spice_model* cur_spice_model) {
  int ix, iy;
  /* Initialize */
  for (ix = 0; ix < (nx + 1); ix++) { 
    for (iy = 0; iy < (ny + 1); iy++) { 
      cur_spice_model->sb_index_low[ix][iy] = 0;
      cur_spice_model->cbx_index_low[ix][iy] = 0;
      cur_spice_model->cby_index_low[ix][iy] = 0;
      cur_spice_model->sb_index_high[ix][iy] = 0;
      cur_spice_model->cbx_index_high[ix][iy] = 0;
      cur_spice_model->cby_index_high[ix][iy] = 0;
    }
  }
  return;
}

void zero_spice_models_routing_index_low_high(int num_spice_models, 
                                              t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    zero_one_spice_model_routing_index_low_high(&(spice_model[i]));
  }

  return;
}

/* Malloc routing_index_low and routing_index_high for a spice_model */
void alloc_spice_model_routing_index_low_high(t_spice_model* cur_spice_model) {
  int ix;

  /* [cbx|cby|sb]_index_low */
  /* x - direction*/
  cur_spice_model->sb_index_low = (int**)my_malloc(sizeof(int*)*(nx + 1));
  cur_spice_model->cbx_index_low = (int**)my_malloc(sizeof(int*)*(nx + 1));
  cur_spice_model->cby_index_low = (int**)my_malloc(sizeof(int*)*(nx + 1));
  /* y - direction*/
  for (ix = 0; ix < (nx + 1); ix++) { 
    cur_spice_model->sb_index_low[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
    cur_spice_model->cbx_index_low[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
    cur_spice_model->cby_index_low[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
  }

  /* grid_index_high */
  /* x - direction*/
  cur_spice_model->sb_index_high = (int**)my_malloc(sizeof(int*)*(nx + 1));
  cur_spice_model->cbx_index_high = (int**)my_malloc(sizeof(int*)*(nx + 1));
  cur_spice_model->cby_index_high = (int**)my_malloc(sizeof(int*)*(nx + 1));
  /* y - direction*/
  for (ix = 0; ix < (nx + 1); ix++) { 
    cur_spice_model->sb_index_high[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
    cur_spice_model->cbx_index_high[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
    cur_spice_model->cby_index_high[ix] = (int*)my_malloc(sizeof(int)*(ny + 1));
  }

  zero_one_spice_model_routing_index_low_high(cur_spice_model);

  return;
}

void free_one_spice_model_routing_index_low_high(t_spice_model* cur_spice_model) {
  int ix;

  /* index_high */
  for (ix = 0; ix < (nx + 1); ix++) { 
    my_free(cur_spice_model->sb_index_low[ix]);
    my_free(cur_spice_model->cbx_index_low[ix]);
    my_free(cur_spice_model->cby_index_low[ix]);

    my_free(cur_spice_model->sb_index_high[ix]);
    my_free(cur_spice_model->cbx_index_high[ix]);
    my_free(cur_spice_model->cby_index_high[ix]);
  }
  my_free(cur_spice_model->sb_index_low);
  my_free(cur_spice_model->cbx_index_low);
  my_free(cur_spice_model->cby_index_low);

  my_free(cur_spice_model->sb_index_high);
  my_free(cur_spice_model->cbx_index_high);
  my_free(cur_spice_model->cby_index_high);

  return;
}

void free_spice_model_routing_index_low_high(int num_spice_models, 
                                             t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    free_one_spice_model_routing_index_low_high(&(spice_model[i]));
  }

  return;
}

void update_one_spice_model_routing_index_high(int x, int y, t_rr_type chan_type,
                                               t_spice_model* cur_spice_model) {
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1))));
  assert((!(0 > y))&&(!(y > (ny + 1))));
  assert(NULL != cur_spice_model);
  assert(NULL != cur_spice_model->sb_index_high);
  assert(NULL != cur_spice_model->sb_index_high[x]); 
  assert(NULL != cur_spice_model->cbx_index_high);
  assert(NULL != cur_spice_model->cbx_index_high[x]); 
  assert(NULL != cur_spice_model->cby_index_high);
  assert(NULL != cur_spice_model->cby_index_high[x]); 

  /* Assigne the low */ 
  if (CHANX == chan_type) {
    cur_spice_model->cbx_index_high[x][y] = cur_spice_model->cnt;
  } else if (CHANY == chan_type) {
    cur_spice_model->cby_index_high[x][y] = cur_spice_model->cnt;
  } else if (SOURCE == chan_type) {
    cur_spice_model->sb_index_high[x][y] = cur_spice_model->cnt;
  } 

  return;
}

void update_spice_models_routing_index_high(int x, int y, t_rr_type chan_type,
                                            int num_spice_models, 
                                            t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    update_one_spice_model_routing_index_high(x, y, chan_type, &(spice_model[i]));
  }

  return;
}

void update_one_spice_model_routing_index_low(int x, int y, t_rr_type chan_type,
                                             t_spice_model* cur_spice_model) {
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1))));
  assert((!(0 > y))&&(!(y > (ny + 1))));
  assert(NULL != cur_spice_model);
  assert(NULL != cur_spice_model->sb_index_low);
  assert(NULL != cur_spice_model->sb_index_low[x]); 
  assert(NULL != cur_spice_model->cbx_index_low);
  assert(NULL != cur_spice_model->cbx_index_low[x]); 
  assert(NULL != cur_spice_model->cby_index_low);
  assert(NULL != cur_spice_model->cby_index_low[x]); 

  /* Assigne the low */ 
  if (CHANX == chan_type) {
    cur_spice_model->cbx_index_low[x][y] = cur_spice_model->cnt;
  } else if (CHANY == chan_type) {
    cur_spice_model->cby_index_low[x][y] = cur_spice_model->cnt;
  } else if (SOURCE == chan_type) {
    cur_spice_model->sb_index_low[x][y] = cur_spice_model->cnt;
  }

  return;
}

void update_spice_models_routing_index_low(int x, int y, t_rr_type chan_type, 
                                           int num_spice_models, 
                                           t_spice_model* spice_model) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    update_one_spice_model_routing_index_low(x, y, chan_type, &(spice_model[i]));
  }

  return;
}

/* Check if this SPICE model defined as SRAM 
 * contain necessary ports for its functionality 
 */
void check_sram_spice_model_ports(t_spice_model* cur_spice_model,
                                  boolean include_bl_wl) {
  int num_output_ports;
  t_spice_model_port** output_ports = NULL;
  int num_bl_ports;
  t_spice_model_port** bl_ports = NULL;
  int num_wl_ports;
  t_spice_model_port** wl_ports = NULL;

  int iport;
  int num_global_ports = 0;
  int num_err = 0;

  /* Check the type of SPICE model */
  assert(SPICE_MODEL_SRAM == cur_spice_model->type);

  /* Check if we has 1 output with size 2 */
  output_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_ports, TRUE);
  num_global_ports = 0;
  for (iport = 0; iport < num_output_ports; iport++) {
    if (TRUE == output_ports[iport]->is_global) {
      num_global_ports++;
    }
  }
  if ((1 != (num_output_ports - num_global_ports))
     && (2 != (num_output_ports - num_global_ports))) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have only 1 non-global output port!\n",
               __FILE__, __LINE__);
    num_err++;
    if (1 != output_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have a output port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  }
  if (FALSE == include_bl_wl) {
    if (0 == num_err) {
      return;
    } else {
      exit(1);
    }
  }
  /* If bl and wl are required, check their existence */
  bl_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_BL, &num_bl_ports, TRUE);
  if (1 != num_bl_ports) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL with BL and WL should have only 1 BL port!\n",
               __FILE__, __LINE__);
    num_err++;
    exit(1);
    if (1 != bl_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have a BL port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  }

  wl_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_WL, &num_wl_ports, TRUE);
  if (1 != num_wl_ports) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL with WL and WL should have only 1 WL port!\n",
               __FILE__, __LINE__);
    num_err++;
    exit(1);
    if (1 != wl_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have a WL port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  }

  if (0 < num_err) {
    exit(1);
  }

  /* Free */
  my_free(output_ports);
  my_free(bl_ports);
  my_free(wl_ports);

  return;
}

void check_ff_spice_model_ports(t_spice_model* cur_spice_model,
                                boolean is_scff) {
  int iport;
  int num_input_ports;
  t_spice_model_port** input_ports = NULL;
  int num_output_ports;
  t_spice_model_port** output_ports = NULL;
  int num_clock_ports;
  t_spice_model_port** clock_ports = NULL;

  int num_err = 0;

  /* Check the type of SPICE model */
  if (FALSE == is_scff) {
    assert(SPICE_MODEL_FF == cur_spice_model->type);
  } else {
    assert(SPICE_MODEL_SCFF == cur_spice_model->type);
  }
  /* Check if we have D, Set and Reset */
  input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, FALSE);
  if (TRUE == is_scff) {
   if (1 > num_input_ports) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL should at least have an input port!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
    for (iport = 0; iport < num_input_ports; iport++) { 
      if (1 != input_ports[iport]->size) { 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL: each input port with size 1!\n",
                 __FILE__, __LINE__);
        num_err++;
      }
    }
  } else {
    if (3 != num_input_ports) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) FF SPICE MODEL should have only 3 input port!\n",
                 __FILE__, __LINE__);
      num_err++;
    } 
    for (iport = 0; iport < num_input_ports; iport++) { 
      if (1 != input_ports[iport]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) FF SPICE MODEL: each input port with size 1!\n",
                   __FILE__, __LINE__);
        num_err++;
      }
    }
  }
  /* Check if we have clock */
  clock_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_CLOCK, &num_clock_ports, FALSE);
  if (1 > num_clock_ports) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) [FF|SCFF] SPICE MODEL should have at least 1 clock port!\n",
               __FILE__, __LINE__);
    num_err++;
  }
  for (iport = 0; iport < num_clock_ports; iport++) { 
    if (1 != clock_ports[iport]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) [FF|SCFF] SPICE MODEL: 1 clock port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  }
  /* Check if we have output */
  output_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_ports, TRUE);
  if (FALSE == is_scff) {
    if (1 != output_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) FF SPICE MODEL: each output port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  } else {
    if (2 != num_output_ports) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL should have 2 output ports!\n",
               __FILE__, __LINE__);
      num_err++;
      for (iport = 0; iport < num_output_ports; iport++) { 
        if (1 != output_ports[iport]->size) {
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL: the output port (%s) should have a size of 1!\n",
                     __FILE__, __LINE__, output_ports[iport]->prefix);
          num_err++;
        }
      }
    }
  }
  /* Error out if required */
  if (0 < num_err) {
    exit(1);
  }
  
  /* Free */
  my_free(input_ports);
  my_free(output_ports);
  my_free(clock_ports);

  return;
}

/* Free a conf_bit_info */
void free_conf_bit(t_conf_bit* conf_bit) {
  return;
}

void free_conf_bit_info(t_conf_bit_info* conf_bit_info) {
  free_conf_bit(conf_bit_info->sram_bit);
  my_free(conf_bit_info->sram_bit); 

  free_conf_bit(conf_bit_info->bl);
  my_free(conf_bit_info->bl); 

  free_conf_bit(conf_bit_info->wl);
  my_free(conf_bit_info->wl); 

  return;
}

/* Fill the information into a confbit_info */
t_conf_bit_info*  
alloc_one_conf_bit_info(int index,
                        t_conf_bit* sram_val,
                        t_conf_bit* bl_val, t_conf_bit* wl_val,
                        t_spice_model* parent_spice_model) {
  t_conf_bit_info* new_conf_bit_info = (t_conf_bit_info*)my_malloc(sizeof(t_conf_bit_info));

  /* Check if we have a valid conf_bit_info */
  if (NULL == new_conf_bit_info) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to malloc a new conf_bit_info!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Fill the information */
  new_conf_bit_info->index = index;
  new_conf_bit_info->sram_bit = sram_val;
  new_conf_bit_info->bl = bl_val;
  new_conf_bit_info->wl = wl_val;
  new_conf_bit_info->parent_spice_model = parent_spice_model;
  new_conf_bit_info->parent_spice_model_index = parent_spice_model->cnt;

  return new_conf_bit_info; 
}
 
/* Add an element to linked-list */
t_llist* 
add_conf_bit_info_to_llist(t_llist* head, int index, 
                           t_conf_bit* sram_val, t_conf_bit* bl_val, t_conf_bit* wl_val,
                           t_spice_model* parent_spice_model) {
  t_llist* temp = NULL;
  t_conf_bit_info* new_conf_bit_info = NULL;
  
  /* if head is NULL, we create a head */
  if (NULL == head) {
    temp = create_llist(1);
    new_conf_bit_info = alloc_one_conf_bit_info(index, sram_val, bl_val, wl_val, parent_spice_model);
    assert(NULL != new_conf_bit_info);
    temp->dptr = (void*)new_conf_bit_info; 
    assert(NULL == temp->next);
    return temp; 
  } else {
  /* If head is a valid pointer, we add a new element to the tail of this linked-list */
    temp = insert_llist_node_before_head(head);
    new_conf_bit_info = alloc_one_conf_bit_info(index, sram_val, bl_val, wl_val, parent_spice_model);
    assert(NULL != new_conf_bit_info);
    temp->dptr = (void*)new_conf_bit_info; 
    return temp; 
  }
}

/* Find BL and WL ports for a SRAM model.
 * And check if the number of BL/WL satisfy the technology needs
 */
void find_bl_wl_ports_spice_model(t_spice_model* cur_spice_model,
                                  int* num_bl_ports, t_spice_model_port*** bl_ports,
                                  int* num_wl_ports, t_spice_model_port*** wl_ports) {
  int i;

  /* Check */
  assert(NULL != cur_spice_model); 

  /* Find BL ports */
  (*bl_ports) = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_BL, num_bl_ports, TRUE);
  /* Find WL ports */
  (*wl_ports) = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_WL, num_wl_ports, TRUE);

  /* port size of BL/WL should be at least 1 !*/
  assert((*num_bl_ports) == (*num_wl_ports));

  /* Check the size of BL/WL ports */
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
    /* This check may be too tight */
    for (i = 0; i < (*num_bl_ports); i++) {
      assert(0 < (*bl_ports)[i]->size);
    }
    for (i = 0; i < (*num_wl_ports); i++) {
      assert(0 < (*wl_ports)[i]->size);
    }
    break;
  case SPICE_MODEL_DESIGN_CMOS:
    for (i = 0; i < (*num_bl_ports); i++) {
      assert(0 < (*bl_ports)[i]->size);
    }
    for (i = 0; i < (*num_wl_ports); i++) {
      assert(0 < (*wl_ports)[i]->size);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }
  
  return;
}

/* Find BL and WL ports for a SRAM model.
 * And check if the number of BL/WL satisfy the technology needs
 */
void find_blb_wlb_ports_spice_model(t_spice_model* cur_spice_model,
                                    int* num_blb_ports, t_spice_model_port*** blb_ports,
                                    int* num_wlb_ports, t_spice_model_port*** wlb_ports) {
  /* Check */
  assert(NULL != cur_spice_model); 

  /* Find BL ports */
  (*blb_ports) = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_BLB, num_blb_ports, TRUE);
  /* Find WL ports */
  (*wlb_ports) = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_WLB, num_wlb_ports, TRUE);
  
  return;
}


/* Functions to manipulate struct sram_orgz_info */
t_sram_orgz_info* alloc_one_sram_orgz_info() {
  return (t_sram_orgz_info*)my_malloc(sizeof(t_sram_orgz_info));
}

t_mem_bank_info* alloc_one_mem_bank_info() {
  return (t_mem_bank_info*)my_malloc(sizeof(t_mem_bank_info));
}

void free_one_mem_bank_info(t_mem_bank_info* mem_bank_info) {
  return; 
}

t_scff_info* alloc_one_scff_info() {
  return (t_scff_info*)my_malloc(sizeof(t_scff_info));
}

void free_one_scff_info(t_scff_info* scff_info) {
  return;
}

t_standalone_sram_info* alloc_one_standalone_sram_info() {
  return (t_standalone_sram_info*)my_malloc(sizeof(t_standalone_sram_info));
}

void free_one_standalone_sram_info(t_standalone_sram_info* standalone_sram_info) {
  return;
}

void init_mem_bank_info(t_mem_bank_info* cur_mem_bank_info,
                        t_spice_model* cur_mem_model) {
  assert(NULL != cur_mem_bank_info);
  assert(NULL != cur_mem_model);
  cur_mem_bank_info->mem_model = cur_mem_model;
  cur_mem_bank_info->num_mem_bit = 0;
  cur_mem_bank_info->num_bl = 0;
  cur_mem_bank_info->num_wl = 0;
  cur_mem_bank_info->reserved_bl = 0;
  cur_mem_bank_info->reserved_wl = 0;

  return;
}

void update_mem_bank_info_num_mem_bit(t_mem_bank_info* cur_mem_bank_info,
                                      int num_mem_bit) {
  assert(NULL != cur_mem_bank_info);

  cur_mem_bank_info->num_mem_bit = num_mem_bit;
  
  return;
}

void init_scff_info(t_scff_info* cur_scff_info,
                    t_spice_model* cur_mem_model) {
  assert(NULL != cur_scff_info); 
  assert(NULL != cur_mem_model);

  cur_scff_info->mem_model = cur_mem_model;
  cur_scff_info->num_mem_bit = 0; 
  cur_scff_info->num_scff = 0; 
  
  return;
}

void update_scff_info_num_mem_bit(t_scff_info* cur_scff_info,
                                  int num_mem_bit) {
  assert(NULL != cur_scff_info); 

  cur_scff_info->num_mem_bit = num_mem_bit; 

  return;
}

void init_standalone_sram_info(t_standalone_sram_info* cur_standalone_sram_info,
                               t_spice_model* cur_mem_model) {
  assert(NULL != cur_standalone_sram_info); 
  assert(NULL != cur_mem_model);

  cur_standalone_sram_info->mem_model = cur_mem_model;
  cur_standalone_sram_info->num_mem_bit = 0; 
  cur_standalone_sram_info->num_sram = 0; 
  
  return;
}

void update_standalone_sram_info_num_mem_bit(t_standalone_sram_info* cur_standalone_sram_info,
                                             int num_mem_bit) {
  assert(NULL != cur_standalone_sram_info); 

  cur_standalone_sram_info->num_mem_bit = num_mem_bit; 

  return;
}

void init_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                         enum e_sram_orgz cur_sram_orgz_type,
                         t_spice_model* cur_mem_model, 
                         int grid_nx, int grid_ny) {
  int i, num_bl_per_sram, num_wl_per_sram;
  int num_bl_ports;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports;
  t_spice_model_port** wl_port = NULL;

  assert(NULL != cur_sram_orgz_info);

  cur_sram_orgz_info->type = cur_sram_orgz_type;
  cur_sram_orgz_info->conf_bit_head = NULL; /* Configuration bits will be allocated later */

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    cur_sram_orgz_info->mem_bank_info = alloc_one_mem_bank_info();
    init_mem_bank_info(cur_sram_orgz_info->mem_bank_info, cur_mem_model);
    find_bl_wl_ports_spice_model(cur_mem_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    try_update_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, 
                                            num_bl_per_sram, num_wl_per_sram);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    cur_sram_orgz_info->scff_info = alloc_one_scff_info();
    init_scff_info(cur_sram_orgz_info->scff_info, cur_mem_model);
    break;
  case SPICE_SRAM_STANDALONE:
    cur_sram_orgz_info->standalone_sram_info = alloc_one_standalone_sram_info();
    init_standalone_sram_info(cur_sram_orgz_info->standalone_sram_info, cur_mem_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  /* Alloc the configuration bit information per grid */
  cur_sram_orgz_info->grid_nx = grid_nx;
  cur_sram_orgz_info->grid_ny = grid_ny;

  cur_sram_orgz_info->grid_reserved_conf_bits = (int**)my_malloc(grid_nx*sizeof(int*));
  for (i = 0; i < grid_nx; i++) {
    cur_sram_orgz_info->grid_reserved_conf_bits[i] = (int*)my_calloc(grid_ny, sizeof(int));
  }

  cur_sram_orgz_info->grid_conf_bits_lsb = (int**)my_malloc(grid_nx*sizeof(int*));
  for (i = 0; i < grid_nx; i++) {
    cur_sram_orgz_info->grid_conf_bits_lsb[i] = (int*)my_calloc(grid_ny, sizeof(int));
  }

  cur_sram_orgz_info->grid_conf_bits_msb = (int**)my_malloc(grid_nx*sizeof(int*));
  for (i = 0; i < grid_nx; i++) {
    cur_sram_orgz_info->grid_conf_bits_msb[i] = (int*)my_calloc(grid_ny, sizeof(int));
  }
  
  return;
}


void free_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                         enum e_sram_orgz cur_sram_orgz_type) {
  int i;
  t_llist* temp = NULL;

  if (NULL == cur_sram_orgz_info) {
    return;
  }

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    free_one_mem_bank_info(cur_sram_orgz_info->mem_bank_info);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    free_one_scff_info(cur_sram_orgz_info->scff_info);
    break;
  case SPICE_SRAM_STANDALONE:
    free_one_standalone_sram_info(cur_sram_orgz_info->standalone_sram_info);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  /* Free configuration bits linked-list  */ 
  temp = cur_sram_orgz_info->conf_bit_head;
  while(NULL != temp) {
    free_conf_bit_info((t_conf_bit_info*)(temp->dptr));
    /* Set the data pointor to NULL, then we can call linked list free function */
    temp->dptr = NULL;
    /* Go the next */
    temp = temp->next;
  }
  free_llist(cur_sram_orgz_info->conf_bit_head);

  /* Free the configuration bit information per grid */
  for (i = 0; i < cur_sram_orgz_info->grid_nx; i++) {
    my_free(cur_sram_orgz_info->grid_reserved_conf_bits[i]);
  }
  my_free(cur_sram_orgz_info->grid_reserved_conf_bits);

  for (i = 0; i < cur_sram_orgz_info->grid_nx; i++) {
    my_free(cur_sram_orgz_info->grid_conf_bits_lsb[i]);
  }
  my_free(cur_sram_orgz_info->grid_conf_bits_lsb);

  for (i = 0; i < cur_sram_orgz_info->grid_nx; i++) {
    my_free(cur_sram_orgz_info->grid_conf_bits_msb[i]);
  }
  my_free(cur_sram_orgz_info->grid_conf_bits_msb);

  return;
}

void update_mem_bank_info_reserved_blwl(t_mem_bank_info* cur_mem_bank_info,
                                        int updated_reserved_bl, int updated_reserved_wl) {
  assert(NULL != cur_mem_bank_info);

  cur_mem_bank_info->reserved_bl = updated_reserved_bl;  
  cur_mem_bank_info->reserved_wl = updated_reserved_wl;  

  return;
}

void get_mem_bank_info_reserved_blwl(t_mem_bank_info* cur_mem_bank_info,
                                     int* num_reserved_bl, int* num_reserved_wl) {
  assert(NULL != cur_mem_bank_info);

  (*num_reserved_bl) = cur_mem_bank_info->reserved_bl;  
  (*num_reserved_wl) = cur_mem_bank_info->reserved_wl;  

  return;
}

void update_mem_bank_info_num_blwl(t_mem_bank_info* cur_mem_bank_info,
                                   int updated_bl, int updated_wl) {
  assert(NULL != cur_mem_bank_info);

  cur_mem_bank_info->num_bl = updated_bl;  
  cur_mem_bank_info->num_wl = updated_wl;  

  return;
}

/* Initialize the number of normal/reserved BLs and WLs, mem_bits in sram_orgz_info 
 * If the updated_reserved_bl|wl is larger than the existed value,
 * we update the reserved_bl|wl
 */
void try_update_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                             int updated_reserved_bl, int updated_reserved_wl) {
  t_spice_model* mem_model = NULL;
  int cur_bl, cur_wl;

  /* Check */
  assert(updated_reserved_bl == updated_reserved_wl);

  /* get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* CMOS technology does not need to update */
    switch (mem_model->design_tech) {
    case SPICE_MODEL_DESIGN_CMOS:
      break;
    case SPICE_MODEL_DESIGN_RRAM: 
      /* get the current number of reserved bls and wls */
      get_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
      if ((updated_reserved_bl > cur_bl) || (updated_reserved_wl > cur_wl)) {
        update_sram_orgz_info_reserved_blwl(cur_sram_orgz_info,
                                            updated_reserved_bl, updated_reserved_wl);
        update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, 
                                          updated_reserved_bl);
        update_sram_orgz_info_num_blwl(cur_sram_orgz_info,
                                       updated_reserved_bl, updated_reserved_wl);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of design technology!",
                 __FILE__, __LINE__ );
      exit(1); 
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

}

/* Force to update the number of reserved BLs and WLs in sram_orgz_info
 * we always update the reserved_bl|wl
 */
void update_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                         int updated_reserved_bl, int updated_reserved_wl) {
  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    update_mem_bank_info_reserved_blwl(cur_sram_orgz_info->mem_bank_info, 
                                         updated_reserved_bl, updated_reserved_wl);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return;
}

void get_sram_orgz_info_reserved_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                      int* num_reserved_bl, int* num_reserved_wl) {
  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    get_mem_bank_info_reserved_blwl(cur_sram_orgz_info->mem_bank_info, 
                                    num_reserved_bl, num_reserved_wl);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return;
}

void get_sram_orgz_info_num_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                int* cur_bl, int* cur_wl) {
  assert(NULL != cur_bl);
  assert(NULL != cur_wl);
  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    (*cur_bl) = 0;
    (*cur_wl) = 0;
    break;
  case SPICE_SRAM_MEMORY_BANK:
    (*cur_bl) = cur_sram_orgz_info->mem_bank_info->num_bl;
    (*cur_wl) = cur_sram_orgz_info->mem_bank_info->num_wl;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return;
}

int get_sram_orgz_info_num_mem_bit(t_sram_orgz_info* cur_sram_orgz_info) {

  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    return cur_sram_orgz_info->standalone_sram_info->num_mem_bit; 
  case SPICE_SRAM_SCAN_CHAIN:
    return cur_sram_orgz_info->scff_info->num_mem_bit; 
  case SPICE_SRAM_MEMORY_BANK:
    return cur_sram_orgz_info->mem_bank_info->num_mem_bit; 
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return 0;
}

void update_sram_orgz_info_num_mem_bit(t_sram_orgz_info* cur_sram_orgz_info,
                                       int new_num_mem_bit) {

  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    update_standalone_sram_info_num_mem_bit(cur_sram_orgz_info->standalone_sram_info, new_num_mem_bit); 
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    update_scff_info_num_mem_bit(cur_sram_orgz_info->scff_info, new_num_mem_bit); 
    break;
  case SPICE_SRAM_MEMORY_BANK:
    update_mem_bank_info_num_mem_bit(cur_sram_orgz_info->mem_bank_info, new_num_mem_bit); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return;
}

void update_sram_orgz_info_num_blwl(t_sram_orgz_info* cur_sram_orgz_info,
                                    int new_bl, int new_wl) {

  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    update_mem_bank_info_num_blwl(cur_sram_orgz_info->mem_bank_info, new_bl, new_wl); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  
  return;
}

void get_sram_orgz_info_mem_model(t_sram_orgz_info* cur_sram_orgz_info,
                                  t_spice_model** mem_model_ptr) {

  assert(NULL != cur_sram_orgz_info);
  assert(NULL != mem_model_ptr);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    (*mem_model_ptr) = cur_sram_orgz_info->standalone_sram_info->mem_model;
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    (*mem_model_ptr) = cur_sram_orgz_info->scff_info->mem_model;
    break;
  case SPICE_SRAM_MEMORY_BANK:
    (*mem_model_ptr) = cur_sram_orgz_info->mem_bank_info->mem_model;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  assert(NULL != (*mem_model_ptr));
  
  return;
}

void update_sram_orgz_info_mem_model(t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_mem_model) {
  assert(NULL != cur_sram_orgz_info);

  /* According to the type, we allocate structs */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    cur_sram_orgz_info->standalone_sram_info->mem_model = cur_mem_model;
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    cur_sram_orgz_info->scff_info->mem_model = cur_mem_model;
    break;
  case SPICE_SRAM_MEMORY_BANK:
    cur_sram_orgz_info->mem_bank_info->mem_model = cur_mem_model;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  return;
}


/* Copy from a src sram_orgz_info to a des sram_orgz_info 
 * The des_orgz_info must be allocated before!!!
 */
void copy_sram_orgz_info(t_sram_orgz_info* des_sram_orgz_info,
                         t_sram_orgz_info* src_sram_orgz_info) {
  t_spice_model* src_mem_model = NULL;
  int src_num_bl, src_num_wl;
  int ix, iy;

  get_sram_orgz_info_mem_model(src_sram_orgz_info, &src_mem_model);
  
  /* Start copying */
  des_sram_orgz_info->type = src_sram_orgz_info->type;
  update_sram_orgz_info_mem_model(des_sram_orgz_info, src_mem_model);
  update_sram_orgz_info_num_mem_bit(des_sram_orgz_info, 
                                    get_sram_orgz_info_num_mem_bit(src_sram_orgz_info));
  /* According to the type, we create the diff. */
  switch (des_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    get_sram_orgz_info_num_blwl(src_sram_orgz_info, &src_num_bl, &src_num_wl);
    update_sram_orgz_info_num_blwl(des_sram_orgz_info, src_num_bl, src_num_wl);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_STANDALONE:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }
  /* Copy conf bits */
  for  (ix = 0; ix < des_sram_orgz_info->grid_nx; ix++) {
    for  (iy = 0; iy < des_sram_orgz_info->grid_ny; iy++) {
      des_sram_orgz_info->grid_reserved_conf_bits[ix][iy] = src_sram_orgz_info->grid_reserved_conf_bits[ix][iy];
      des_sram_orgz_info->grid_conf_bits_lsb[ix][iy] = src_sram_orgz_info->grid_conf_bits_lsb[ix][iy];
      des_sram_orgz_info->grid_conf_bits_msb[ix][iy] = src_sram_orgz_info->grid_conf_bits_msb[ix][iy];
    }
  }

  return;
}

/* Create a snapshot on the sram_orgz_info, 
 * return the snapshot
 */
t_sram_orgz_info* snapshot_sram_orgz_info(t_sram_orgz_info* src_sram_orgz_info) {
  t_sram_orgz_info* des_sram_orgz_info = NULL;
  t_spice_model* src_mem_model = NULL;
  
  /* allocate the snapshot */
  des_sram_orgz_info = alloc_one_sram_orgz_info();

  /* initialize the snapshot */
  get_sram_orgz_info_mem_model(src_sram_orgz_info, &src_mem_model);
  init_sram_orgz_info(des_sram_orgz_info, src_sram_orgz_info->type, 
                      src_mem_model, src_sram_orgz_info->grid_nx, src_sram_orgz_info->grid_ny);

  /* Start copying */
  copy_sram_orgz_info( des_sram_orgz_info,
                       src_sram_orgz_info);

  return des_sram_orgz_info;
}

/* Compare the two sram_orgz_info and store the difference in the sram_orgz_info to return */
t_sram_orgz_info* diff_sram_orgz_info(t_sram_orgz_info* des_sram_orgz_info, 
                                      t_sram_orgz_info* base_sram_orgz_info) {
  t_sram_orgz_info* diff_sram_orgz_info = NULL;
  t_spice_model* base_mem_model = NULL;
  t_spice_model* des_mem_model = NULL;
  int des_num_wl, base_num_wl;
  int des_num_bl, base_num_bl;
  int ix, iy;

  /* Check: we have the same memory organization type */
  assert ( des_sram_orgz_info->type == base_sram_orgz_info->type );
  get_sram_orgz_info_mem_model(base_sram_orgz_info, &base_mem_model);
  get_sram_orgz_info_mem_model(des_sram_orgz_info, &des_mem_model);
  assert (des_mem_model == base_mem_model);
  assert (des_sram_orgz_info->grid_nx == base_sram_orgz_info->grid_nx);
  assert (des_sram_orgz_info->grid_ny == base_sram_orgz_info->grid_ny);

  /* allocate the diff copy */
  diff_sram_orgz_info = alloc_one_sram_orgz_info();
  init_sram_orgz_info(diff_sram_orgz_info, des_sram_orgz_info->type, 
                      des_mem_model, des_sram_orgz_info->grid_nx, des_sram_orgz_info->grid_ny);

  /* initialize the diff_copy */
  update_sram_orgz_info_num_mem_bit(diff_sram_orgz_info, 
                                    get_sram_orgz_info_num_mem_bit(des_sram_orgz_info) - get_sram_orgz_info_num_mem_bit(base_sram_orgz_info));
  /* According to the type, we create the diff. */
  switch (des_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    get_sram_orgz_info_num_blwl(des_sram_orgz_info, &des_num_bl, &des_num_wl);
    get_sram_orgz_info_num_blwl(base_sram_orgz_info, &base_num_bl, &base_num_wl);
    update_sram_orgz_info_num_blwl(diff_sram_orgz_info, des_num_bl - base_num_bl, des_num_wl - base_num_wl);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_STANDALONE:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
               __FILE__, __LINE__ );
    exit(1); 
  }

  /* Copy conf bits */
  for  (ix = 0; ix < diff_sram_orgz_info->grid_nx; ix++) {
    for  (iy = 0; iy < diff_sram_orgz_info->grid_ny; iy++) {
      diff_sram_orgz_info->grid_reserved_conf_bits[ix][iy] = des_sram_orgz_info->grid_reserved_conf_bits[ix][iy] - base_sram_orgz_info->grid_reserved_conf_bits[ix][iy];
      diff_sram_orgz_info->grid_conf_bits_lsb[ix][iy] = des_sram_orgz_info->grid_conf_bits_lsb[ix][iy] - base_sram_orgz_info->grid_conf_bits_lsb[ix][iy];
      diff_sram_orgz_info->grid_conf_bits_msb[ix][iy] = des_sram_orgz_info->grid_conf_bits_msb[ix][iy] - base_sram_orgz_info->grid_conf_bits_msb[ix][iy];
    }
  }

  return diff_sram_orgz_info;
}


/* Manipulating functions for struct t_reserved_syntax_char */
void init_reserved_syntax_char(t_reserved_syntax_char* cur_reserved_syntax_char,
                               char cur_syntax_char, boolean cur_verilog_reserved, boolean cur_spice_reserved) {
  assert(NULL != cur_reserved_syntax_char);

  cur_reserved_syntax_char->syntax_char = cur_syntax_char;
  cur_reserved_syntax_char->verilog_reserved = cur_verilog_reserved;
  cur_reserved_syntax_char->spice_reserved = cur_spice_reserved;

  return;
}

void check_mem_model_blwl_inverted(t_spice_model* cur_mem_model, 
                                   enum e_spice_model_port_type blwl_port_type,
                                   boolean* blwl_inverted) {
  int num_blwl_ports = 0;
  t_spice_model_port** blwl_port = NULL;

  /* Check */
  assert((SPICE_MODEL_PORT_BL == blwl_port_type)||(SPICE_MODEL_PORT_WL == blwl_port_type));

  /* Find BL and WL ports */
  blwl_port = find_spice_model_ports(cur_mem_model, blwl_port_type, &num_blwl_ports, TRUE);

  /* If we cannot find any return with warnings */
  if (0 == num_blwl_ports) {
    (*blwl_inverted) = FALSE;
    vpr_printf(TIO_MESSAGE_WARNING, "(FILE:%s,[LINE%d])Unable to find any BL/WL port for memory model(%s)!\n",
               __FILE__, __LINE__, cur_mem_model->name);
    return;
  }

  /* Only 1 port should be found */ 
  assert(1 == num_blwl_ports);
  /* And port size should be at least 1 */
  assert(0 < blwl_port[0]->size);

  /* if default value of a BL/WL port is 0, we do not need an inversion. */
  if (0 == blwl_port[0]->default_val) {
    (*blwl_inverted) = FALSE;
  } else {
  /* if default value of a BL/WL port is 1, we need an inversion! */
    assert(1 == blwl_port[0]->default_val); 
    (*blwl_inverted) = TRUE;
  }

  return;
}


/* Check if all the SRAM ports have the correct SPICE MODEL */
void config_spice_models_sram_port_spice_model(int num_spice_model,
                                               t_spice_model* spice_models,
                                               t_spice_model* default_sram_spice_model) {
  int i, iport;

  for (i = 0; i < num_spice_model; i++) {
    for (iport = 0; iport < spice_models[i].num_port; iport++) {
      /* Bypass non SRAM ports */
      if (SPICE_MODEL_PORT_SRAM != spice_models[i].ports[iport].type) {
        continue;
      }
      /* Write for the default SRAM SPICE model! */
      spice_models[i].ports[iport].spice_model = default_sram_spice_model;
      /* Only show warning when we try to override the given spice_model_name ! */ 
      if (NULL == spice_models[i].ports[iport].spice_model_name) { 
        continue;
      }
      /* Give a warning !!! */
      if (0 != strcmp(default_sram_spice_model->name, spice_models[i].ports[iport].spice_model_name)) {
        vpr_printf(TIO_MESSAGE_WARNING, 
                   "(FILE:%s, LINE[%d]) Overwrite SRAM SPICE MODEL of SPICE model port (name:%s, port:%s) to be the correct one (name:%s)!\n",
                   __FILE__ ,__LINE__, 
                   spice_models[i].name,
                   spice_models[i].ports[iport].prefix,
                   default_sram_spice_model->name);
      }
    }
  }

  return;
}

void determine_sb_port_coordinator(t_sb cur_sb_info, int side, 
                                   int* port_x, int* port_y) {
   /* Check */
   assert ((-1 < side) && (side < 4));
   /* Initialize */
   (*port_x) = -1;
   (*port_y) = -1;

  switch (side) {
  case TOP:
    /* (0 == side) */
    /* 1. Channel Y [x][y+1] inputs */
    (*port_x) = cur_sb_info.x;
    (*port_y) = cur_sb_info.y + 1;
    break;
  case RIGHT:
    /* 1 == side */
    /* 2. Channel X [x+1][y] inputs */
    (*port_x) = cur_sb_info.x + 1;
    (*port_y) = cur_sb_info.y;
    break;
  case BOTTOM:
    /* 2 == side */
    /* 3. Channel Y [x][y] inputs */
    (*port_x) = cur_sb_info.x;
    (*port_y) = cur_sb_info.y;
    break;
  case LEFT:
    /* 3 == side */
    /* 4. Channel X [x][y] inputs */
    (*port_x) = cur_sb_info.x;
    (*port_y) = cur_sb_info.y;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid side of sb[%d][%d]!\n",
               __FILE__, __LINE__, cur_sb_info.x, cur_sb_info.y, side);
    exit(1);
  }

  return;
}

void init_spice_models_tb_cnt(int num_spice_models,
                              t_spice_model* spice_model) {
  int imodel;

  for (imodel = 0; imodel < num_spice_models; imodel++) {
    spice_model[imodel].tb_cnt = 0;
  }

  return;
}

void init_spice_models_grid_tb_cnt(int num_spice_models,
                                   t_spice_model* spice_model,
                                   int grid_x, int grid_y) {
  int imodel;

  for (imodel = 0; imodel < num_spice_models; imodel++) {
    spice_model[imodel].tb_cnt = spice_model[imodel].grid_index_low[grid_x][grid_y];
  }

  return;
}

void check_spice_models_grid_tb_cnt(int num_spice_models,
                                    t_spice_model* spice_model,
                                    int grid_x, int grid_y,
                                    enum e_spice_model_type spice_model_type_to_check) {
  int imodel;

  for (imodel = 0; imodel < num_spice_models; imodel++) {
    if (spice_model_type_to_check != spice_model[imodel].type) {
      continue;
    }
    assert(spice_model[imodel].tb_cnt == spice_model[imodel].grid_index_high[grid_x][grid_y]);
  }

  return;
}

boolean check_negative_variation(float avg_val, 
                                 t_spice_mc_variation_params variation_params) {
  boolean exist_neg_val = FALSE;

  /* Assume only support gaussian variation now */
  if (avg_val < 0.) {
    exist_neg_val = TRUE;
  } 

  return exist_neg_val;
}

/* Check if this cby_info exists, it may be covered by a heterogenous block */
boolean is_cb_exist(t_rr_type cb_type,
                    int cb_x, int cb_y) {
  boolean cb_exist = TRUE;

  /* Check */
  assert((!(0 > cb_x))&&(!(cb_x > (nx + 1)))); 
  assert((!(0 > cb_y))&&(!(cb_y > (ny + 1)))); 

  switch (cb_type) {
  case CHANX:
    /* Border case */
    /* Check the grid under this CB */
    if ((NULL == grid[cb_x][cb_y].type) 
       ||(EMPTY_TYPE == grid[cb_x][cb_y].type) 
       ||!(grid[cb_x][cb_y].offset + 1 ==  grid[cb_x][cb_y].type->height)) {
      cb_exist = FALSE;
    }
    break;
  case CHANY:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid CB type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return cb_exist;
}

/* Count the number of IPIN rr_nodes in a CB_info struct */
int count_cb_info_num_ipin_rr_nodes(t_cb cur_cb_info) {
  int side;
  int cnt = 0;

  for (side = 0; side < cur_cb_info.num_sides; side++) {
    cnt += cur_cb_info.num_ipin_rr_nodes[side];
  }

  return cnt; 
}

/* Add a subckt file name to a linked list */
t_llist* add_one_subckt_file_name_to_llist(t_llist* cur_head, 
                                            char* subckt_file_path) {
  t_llist* new_head = NULL;

  if (NULL == cur_head) {
    new_head = create_llist(1);
    new_head->dptr = (void*) my_strdup(subckt_file_path);
  } else {
    new_head = insert_llist_node_before_head(cur_head);
    new_head->dptr = (void*) my_strdup(subckt_file_path);
  }

  return new_head;
}

/* Check if SPICE subckt is already created
 * (if they exist in a given linked-list
 */
boolean check_subckt_file_exist_in_llist(t_llist* subckt_llist_head,
                                         char* subckt_file_name) {
  t_llist* temp = NULL; 

  temp = subckt_llist_head;
  while (temp) {
    if (0 == strcmp(subckt_file_name, (char*)(temp->dptr))) {
      return TRUE;
    }
    temp = temp->next;
  }

  return FALSE;
}

/* Identify if this is a primitive pb_type */
boolean is_primitive_pb_type(t_pb_type* cur_pb_type) {

  if ((NULL != cur_pb_type->spice_model_name) 
     || (NULL != cur_pb_type->physical_pb_type_name)) {
    return TRUE;
  }
  return FALSE;
}

/* Recursively find all the global ports in the spice_model / sub spice_model 
 */
void rec_stats_spice_model_global_ports(const t_spice_model* cur_spice_model,
                                        boolean recursive,
                                        t_llist** spice_model_head) {
  int iport;
  t_llist* temp = NULL;

  /* Check */
  assert(NULL != cur_spice_model);
  if (0 < cur_spice_model->num_port) {
    assert(NULL != cur_spice_model->ports);
  }

  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    /* if this spice model requires customized netlist to be included, we do not go recursively */
    if (TRUE == recursive) { 
      /* GO recursively first, and meanwhile count the number of global ports */
      /* For the port that requires another spice_model, i.e., SRAM
       * We need include any global port in that spice model
       */
      if (NULL != cur_spice_model->ports[iport].spice_model) {
         rec_stats_spice_model_global_ports(cur_spice_model->ports[iport].spice_model, 
                                            recursive, spice_model_head);
      }
    }
    /* By pass non-global ports*/
    if (FALSE == cur_spice_model->ports[iport].is_global) {
      continue;
    }
    /* Now we have a global port, add it to linked list */
    assert (TRUE == cur_spice_model->ports[iport].is_global);
    if (NULL == (*spice_model_head)) {
     (*spice_model_head) = create_llist(1);
     /* Configure the data pointer of linked list */
     (*spice_model_head)->dptr = (void*) (&cur_spice_model->ports[iport]);
     /* Check if this ports exists in the linked list */
    } else if (FALSE == check_dptr_exist_in_llist((*spice_model_head),
                                                  (void*)(&cur_spice_model->ports[iport]))) {
      /* Non-exist in the current linked-list, a new node is required 
       * Go to the tail of the linked-list and add a new node  
       */
      temp = search_llist_tail(*spice_model_head);
      temp = insert_llist_node(temp);
      /* Configure the data pointer of linked list */
      temp->dptr = (void*) (&cur_spice_model->ports[iport]);
    }
  }

  return;
}

/* Create a snapshot on spice_model counter */
int* snapshot_spice_model_counter(int num_spice_models,
                                  t_spice_model* spice_model) {
  int i;
  int* snapshot = (int*) my_calloc(num_spice_models, sizeof(int));

  for (i = 0; i < num_spice_models; i++) {
    snapshot[i] = spice_model[i].cnt;
  }

  return snapshot;
}
                                  

void set_spice_model_counter(int num_spice_models,
                             t_spice_model* spice_model,
                             int* spice_model_counter) {
  int i;

  for (i = 0; i < num_spice_models; i++) {
    spice_model[i].cnt = spice_model_counter[i];
  }

  return;
}

/* Find the vpack_net_num of the outputs of the logical_block */
void get_logical_block_output_vpack_net_num(t_logical_block* cur_logical_block,
                                            int* num_lb_output_ports, int** num_lb_output_pins, 
                                            int*** lb_output_vpack_net_num) {
  int iport, ipin; 
  int num_output_ports = 0;
  int* num_output_pins = NULL;
  t_model_ports* head = NULL;
  int** output_vpack_net_num = NULL;

  assert (NULL != cur_logical_block);

  /* Count how many outputs we have */  
  head = cur_logical_block->model->outputs; 
  while (NULL != head) {
    num_output_ports++; 
    head = head->next;
  }
  /* Allocate */ 
  num_output_pins = (int*) my_calloc(num_output_ports, sizeof(int));
  output_vpack_net_num = (int**) my_calloc(num_output_ports, sizeof(int*));
  /* Fill the array */
  iport = 0;
  head = cur_logical_block->model->outputs; 
  while (NULL != head) {
    num_output_pins[iport] = head->size; 
    output_vpack_net_num[iport] = (int*) my_calloc(num_output_pins[iport], sizeof(int));
    /* Fill the array */
    for (ipin = 0; ipin < num_output_pins[iport]; ipin++) {
      output_vpack_net_num[iport][ipin] = cur_logical_block->output_nets[iport][ipin];
    }
    /* Go to the next */
    head = head->next;
    /* Update counter */
    iport++;
  }
 
  assert (iport == num_output_ports);

  /* Assign return values */
  (*num_lb_output_ports) = num_output_ports;
  (*num_lb_output_pins) = num_output_pins;
  (*lb_output_vpack_net_num) = output_vpack_net_num;
    
  return;
}

int get_lut_logical_block_index_with_output_vpack_net_num(int target_vpack_net_num) {
  int iblk, iport;
  int matched_lb_index = OPEN;
  int matched_count = 0;
  int num_lut_output_ports;
  int* num_lut_output_pins;
  int** lut_output_vpack_net_num;

  for (iblk = 0; iblk < num_logical_blocks; iblk++) {
    /* Bypass the non-LUT logical block */
    if (VPACK_COMB != logical_block[iblk].type) {
      continue;
    }
    if (LUT_CLASS != logical_block[iblk].pb->pb_graph_node->pb_type->class_type) {
      continue;
    }
    /* Reach here it should be a LUT */
    get_logical_block_output_vpack_net_num(&logical_block[iblk],
                                           &num_lut_output_ports, &num_lut_output_pins, 
                                           &lut_output_vpack_net_num);
    /* Check */
    assert ( 1 == num_lut_output_ports);
    assert ( 1 == num_lut_output_pins[0]);
    assert ( OPEN != lut_output_vpack_net_num[0][0]);

    if (target_vpack_net_num == lut_output_vpack_net_num[0][0]) {
      matched_lb_index = iblk;
      matched_count++;
    }

    /* Free */
    my_free(num_lut_output_pins);
    for (iport = 0; iport < num_lut_output_ports; iport++) {
      my_free(lut_output_vpack_net_num);
    }
  } 
  
  assert ((0 == matched_count)
         || (1 == matched_count));
  
  return matched_lb_index;
}

/* Get the operational clock port from the global port linked list */
void get_fpga_x2p_global_op_clock_ports(t_llist* head,
                                        int* num_clock_ports,
                                        t_spice_model_port*** clock_port) {
  t_llist* temp = head;
  t_spice_model_port* cur_port = NULL;
  int cnt = 0;

  /* Get the number of clock ports */
  while (NULL != temp) {
    cur_port = (t_spice_model_port*)(temp->dptr); 
    if ( (SPICE_MODEL_PORT_CLOCK == cur_port->type)
      && (FALSE == cur_port->is_prog)) { 
      cnt++;
    }
    /* Go to the next */
    temp = temp->next;
  }

  /* Initialize the counter */
  (*num_clock_ports) = cnt;

  /* Malloc */
  (*clock_port) = (t_spice_model_port**)my_calloc((*num_clock_ports), sizeof(t_spice_model_port*));

  /* Reset the counter */
  temp = head;
  cnt = 0;
  /* Fill the return array */
  while (NULL != temp) {
    cur_port = (t_spice_model_port*)(temp->dptr); 
    if ( (SPICE_MODEL_PORT_CLOCK == cur_port->type)
      && (FALSE == cur_port->is_prog)) { 
      (*clock_port)[cnt] = cur_port;
      cnt++;
    }
    /* Go to the next */
    temp = temp->next;
  }

  assert (cnt == (*num_clock_ports));
 
  return;
}
                   
/* Get all the clock ports from the global port linked list */
void get_fpga_x2p_global_all_clock_ports(t_llist* head,
                                        int* num_clock_ports,
                                        t_spice_model_port*** clock_port) {
  t_llist* temp = head;
  t_spice_model_port* cur_port = NULL;
  int cnt = 0;

  /* Get the number of clock ports */
  while (NULL != temp) {
    cur_port = (t_spice_model_port*)(temp->dptr); 
    if ( (SPICE_MODEL_PORT_CLOCK == cur_port->type)) { 
      cnt++;
    }
    /* Go to the next */
    temp = temp->next;
  }

  /* Initialize the counter */
  (*num_clock_ports) = cnt;

  /* Malloc */
  (*clock_port) = (t_spice_model_port**)my_calloc((*num_clock_ports), sizeof(t_spice_model_port*));

  /* Reset the counter */
  temp = head;
  cnt = 0;
  /* Fill the return array */
  while (NULL != temp) {
    cur_port = (t_spice_model_port*)(temp->dptr); 
    if ( (SPICE_MODEL_PORT_CLOCK == cur_port->type)) { 
      (*clock_port)[cnt] = cur_port;
      cnt++;
    }
    /* Go to the next */
    temp = temp->next;
  }

  assert (cnt == (*num_clock_ports));
 
  return;
}
                 
/* Returns the number of char occupied by the int */  
int my_strlen_int(int input_int) {
 
  int length_input;
  char* input_str;

  input_str = my_itoa(input_int);
  length_input = strlen(input_str);

  free(input_str);

  return length_input;
}  

boolean my_bool_to_boolean(bool my_bool) {

  if(true == my_bool) {
    return TRUE;
  } else {
   assert (false == my_bool);
   return FALSE;
  }
}
