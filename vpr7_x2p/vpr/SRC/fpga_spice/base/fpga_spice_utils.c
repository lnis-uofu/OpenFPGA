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
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"

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
    path = my_strdup("./");
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



/* Tasks: 
 * 1. Search the spice_model_name of input and output buffer and link to the spice_model
 * 2. Copy the information from input/output buffer spice model to higher level spice_models 
 */
void config_spice_model_input_output_buffers_pass_gate(int num_spice_models, 
                                                       t_spice_model* spice_model) {
  int i;
  t_spice_model* buf_spice_model = NULL;
  t_spice_model* pgl_spice_model = NULL;

  for (i = 0; i < num_spice_models; i++) {
    /* By pass inverters and buffers  */
    if (SPICE_MODEL_INVBUF == spice_model[i].type) {
      continue;
    }

    /* Check if this spice model has input buffers */
    if (1 == spice_model[i].input_buffer->exist) {
      buf_spice_model = find_name_matched_spice_model(spice_model[i].input_buffer->spice_model_name,
                                                      num_spice_models, spice_model);
      /* We should find a buffer spice_model*/
      if (NULL == buf_spice_model) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find inv/buffer spice_model to the input buffer of spice_model(name=%s)!\n",
                   __FILE__, __LINE__, spice_model[i].name);
        exit(1);
      }
      /* Copy the information from found spice model to current spice model*/
      memcpy(spice_model[i].input_buffer, buf_spice_model->design_tech_info.buffer_info, sizeof(t_spice_model_buffer));
      /* Recover the spice_model_name and exist */
      spice_model[i].input_buffer->exist = 1;
      spice_model[i].input_buffer->spice_model_name = my_strdup(buf_spice_model->name);
      spice_model[i].input_buffer->spice_model = buf_spice_model;
    }

    /* Check if this spice model has output buffers */
    if (1 == spice_model[i].output_buffer->exist) {
      buf_spice_model = find_name_matched_spice_model(spice_model[i].output_buffer->spice_model_name,
                                                      num_spice_models, spice_model);
      /* We should find a buffer spice_model*/
      if (NULL == buf_spice_model) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find inv/buffer spice_model to the output buffer of spice_model(name=%s)!\n",
                   __FILE__, __LINE__, spice_model[i].name);
        exit(1);
      }
      /* Copy the information from found spice model to current spice model*/
      memcpy(spice_model[i].output_buffer, buf_spice_model->design_tech_info.buffer_info, sizeof(t_spice_model_buffer));
      /* Recover the spice_model_name and exist */
      spice_model[i].output_buffer->exist = 1;
      spice_model[i].output_buffer->spice_model_name = my_strdup(buf_spice_model->name);
      spice_model[i].output_buffer->spice_model = buf_spice_model;
    }
 
    /* If this spice_model is a LUT, check the lut_input_buffer */
    if (SPICE_MODEL_LUT == spice_model[i].type) {
      assert(1 == spice_model[i].lut_input_buffer->exist);

      buf_spice_model = find_name_matched_spice_model(spice_model[i].lut_input_buffer->spice_model_name,
                                                      num_spice_models, spice_model);

      /* We should find a buffer spice_model*/
      if (NULL == buf_spice_model) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find inv/buffer spice_model to the lut_input buffer of spice_model(name=%s)!\n",
                   __FILE__, __LINE__, spice_model[i].name);
        exit(1);
      }
      /* Copy the information from found spice model to current spice model*/
      memcpy(spice_model[i].lut_input_buffer, buf_spice_model->design_tech_info.buffer_info, sizeof(t_spice_model_buffer));
      /* Recover the spice_model_name and exist */
      spice_model[i].lut_input_buffer->exist = 1;
      spice_model[i].lut_input_buffer->spice_model_name = my_strdup(buf_spice_model->name);
      spice_model[i].lut_input_buffer->spice_model = buf_spice_model;
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
      memcpy(spice_model[i].pass_gate_logic, pgl_spice_model->design_tech_info.pass_gate_info, sizeof(t_spice_model_pass_gate_logic));
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
t_spice_model_port** find_spice_model_ports(t_spice_model* spice_model,
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
char* fpga_spice_create_one_subckt_filename(char* file_name_prefix,
                                            int subckt_x, int subckt_y,
                                            char* file_name_postfix) {
  char* fname = NULL;

  fname = (char*) my_malloc(sizeof(char) * (strlen(file_name_prefix)
                            + strlen(my_itoa(subckt_x)) + strlen(my_itoa(subckt_y))
                            + strlen(file_name_postfix) + 1));

  sprintf(fname, "%s%d_%d%s", 
          file_name_prefix, subckt_x, subckt_y, file_name_postfix);

  return fname;
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
    case SPICE_MODEL_PORT_INPUT: /* TODO: support is_non_clock_global*/ 
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
  switch (mux_spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_sram_bits = 1;
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_sram_bits = num_input_per_level;
    if ((2 == num_sram_bits)&&(2 == mux_size)) { 
      num_sram_bits = 1;
    }
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
    num_special_basis = pow((double)(num_input_per_unit), (double)(num_level-1)) - num_basis_last_level;
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
                                   int path_id) {
  int* ret = (int*)my_malloc(sizeof(int)*fan_in);
  int i;

  /* Check */
  assert( (!(0 > path_id)) && (path_id < fan_in) );
  
  for (i = 0; i < fan_in; i++) {
    ret[i] = 0;
  }
  ret[path_id] = 1;
  /* ret[fan_in - 1 - path_id] = 1; */
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

void decode_cmos_mux_sram_bits(t_spice_model* mux_spice_model,
                               int mux_size, int path_id, 
                               int* bit_len, int** conf_bits, int* mux_level) {
  /* Check */
  assert(NULL != mux_level);
  assert(NULL != bit_len);
  assert(NULL != conf_bits);
  assert((-1 < path_id)&&(path_id < mux_size));
  assert(SPICE_MODEL_MUX == mux_spice_model->type);
  assert(SPICE_MODEL_DESIGN_CMOS == mux_spice_model->design_tech);
  
  /* Initialization */
  (*bit_len) = 0;
  (*conf_bits) = NULL;

  /* Special for MUX-2: whatever structure it is, it has always one-level and one configuration bit */
  if (2 == mux_size) {
    (*bit_len) = 1;
    (*mux_level) = 1;
    (*conf_bits) = decode_tree_mux_sram_bits(mux_size, (*mux_level), path_id); 
    return;
  }
  /* Other general cases */ 
  switch (mux_spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    (*mux_level) = determine_tree_mux_level(mux_size);
    (*bit_len) = (*mux_level);
    (*conf_bits) = decode_tree_mux_sram_bits(mux_size, (*mux_level), path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    (*mux_level) = 1;
    (*bit_len) = mux_size;
    (*conf_bits) = decode_onelevel_mux_sram_bits(mux_size, (*mux_level), path_id); 
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    (*mux_level) = mux_spice_model->design_tech_info.mux_num_level;
    (*bit_len) = determine_num_input_basis_multilevel_mux(mux_size, (*mux_level)) * (*mux_level);
    (*conf_bits) = decode_multilevel_mux_sram_bits(mux_size, (*mux_level), path_id); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for mux_spice_model (%s)!\n",
               __FILE__, __LINE__, mux_spice_model->name);
    exit(1);
  } 
  return;
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

char* convert_chan_type_to_string(t_rr_type chan_type) {
  switch(chan_type) {
  case CHANX:
    return "chanx";
    break;
  case CHANY:
    return "chany";
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
}

char* convert_chan_rr_node_direction_to_string(enum PORTS chan_rr_node_direction) {
  switch(chan_rr_node_direction) {
  case IN_PORT:
    return "in";
    break;
  case OUT_PORT:
    return "out";
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of port!\n", __FILE__, __LINE__);
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
    /* Return the logic block we may find */
    switch (pb_spice_model->type) {
    case SPICE_MODEL_LUT :
      /* Special for LUT... They have sub modes!!!*/
      assert(NULL != cur_pb->child_pbs);
      return cur_pb->child_pbs[0][0].logical_block; 
    case SPICE_MODEL_FF:
      assert(pb_spice_model == logical_block[cur_pb->logical_block].mapped_spice_model);
      return cur_pb->logical_block;
    case SPICE_MODEL_HARDLOGIC:
      if (NULL != cur_pb->child_pbs) {
        return cur_pb->child_pbs[0][0].logical_block; 
      } else {
        assert(pb_spice_model == logical_block[cur_pb->logical_block].mapped_spice_model);
        return cur_pb->logical_block;
      } 
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid spice model type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
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
  if (target_switch_inf.structure != target_switch_inf.spice_model->design_tech_info.structure) {
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

/* Find the physical mode index */
int find_pb_type_physical_mode_index(t_pb_type cur_pb_type) {
  int phy_mode_index = 0;
  int imode = 0;
  int num_phy_mode = 0;

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
    if (1 == cur_pb_type.modes[imode].define_physical_mode) {
      phy_mode_index = imode;
      num_phy_mode++;
    }
  } 
  assert(1 == num_phy_mode); 

  return phy_mode_index;
}

void mark_grid_type_pb_graph_node_pins_temp_net_num(int x, int y) {
  int iport, ipin, type_pin_index, class_id, pin_global_rr_node_id; 
  t_type_ptr type = NULL;
  t_pb_graph_node* top_pb_graph_node = NULL;
  int mode_index, ipb, jpb;

  /* Assert */
  assert((!(x < 0))&&(x < (nx + 2)));  
  assert((!(y < 0))&&(y < (ny + 2)));  

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

  /* Run again to handle feedback loop */
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


  return; 
}

/* Assign the temp_net_num by considering the first incoming edge that belongs to the correct operating mode */
void assign_pb_graph_node_pin_temp_net_num_by_mode_index(t_pb_graph_pin* cur_pb_graph_pin,
                                                         int mode_index) {
  int iedge;

  /* IMPORTANT: I assume by default the index of selected edge is 0 
   * Make sure this input edge comes from the default mode     
   */
  for (iedge = 0; iedge < cur_pb_graph_pin->num_input_edges; iedge++) {
    if (mode_index != cur_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode_index) {
      continue;
    }
    cur_pb_graph_pin->temp_net_num = cur_pb_graph_pin->input_edges[iedge]->input_pins[0]->temp_net_num;
    break;
  }

  return;  
}

void mark_pb_graph_node_input_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index) {
  int iport, ipin;

  assert(NULL != cur_pb_graph_node);

  /* Input ports */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      cur_pb_graph_node->input_pins[iport][ipin].temp_net_num = OPEN;
      /* IMPORTANT: I assume by default the index of selected edge is 0 
       * Make sure this input edge comes from the default mode     
       */
       assign_pb_graph_node_pin_temp_net_num_by_mode_index(&(cur_pb_graph_node->input_pins[iport][ipin]), mode_index);
    }
  }

  return;
}

void mark_pb_graph_node_clock_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index) {
  int iport, ipin;

  assert(NULL != cur_pb_graph_node);

  /* Clock ports */
  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
      cur_pb_graph_node->clock_pins[iport][ipin].temp_net_num = OPEN;
      assign_pb_graph_node_pin_temp_net_num_by_mode_index(&(cur_pb_graph_node->clock_pins[iport][ipin]), mode_index);
    }
  }

  return;
}

void mark_pb_graph_node_output_pins_temp_net_num(t_pb_graph_node* cur_pb_graph_node,
                                                int mode_index) {
  int iport, ipin;

  assert(NULL != cur_pb_graph_node);

  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      cur_pb_graph_node->output_pins[iport][ipin].temp_net_num = OPEN;
      /* IMPORTANT: I assume by default the index of selected edge is 0 
       * Make sure this input edge comes from the default mode     
       */
      assign_pb_graph_node_pin_temp_net_num_by_mode_index(&(cur_pb_graph_node->output_pins[iport][ipin]), mode_index);
    }
  }

  return;
}

/* Mark temp_net_num in current pb_graph_node from the parent pb_graph_node */
void rec_mark_pb_graph_node_temp_net_num(t_pb_graph_node* cur_pb_graph_node) {
  int mode_index, ipb, jpb;

  assert(NULL != cur_pb_graph_node);

  /* Find the default mode */
  mode_index = find_pb_type_idle_mode_index(*(cur_pb_graph_node->pb_type));

  mark_pb_graph_node_input_pins_temp_net_num(cur_pb_graph_node, mode_index);

  mark_pb_graph_node_clock_pins_temp_net_num(cur_pb_graph_node, mode_index);

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

  /* IMPORTANT: update the temp_net of Output ports after recursion is done!
   * the outputs of sub pb_graph_node should be updated first 
   */
  mark_pb_graph_node_output_pins_temp_net_num(cur_pb_graph_node, mode_index);

  /* Do this again to handle feedback loops ! */
  mark_pb_graph_node_input_pins_temp_net_num(cur_pb_graph_node, mode_index);

  mark_pb_graph_node_clock_pins_temp_net_num(cur_pb_graph_node, mode_index);

  return;
}

void load_one_pb_graph_pin_temp_net_num_from_pb(t_pb* cur_pb,
                                                t_pb_graph_pin* cur_pb_graph_pin) {
  int node_index;
  t_rr_node* pb_rr_nodes = NULL;

  assert(NULL != cur_pb);
  assert(NULL != cur_pb->pb_graph_node);

  /* Get the selected edge of current pin*/
  pb_rr_nodes = cur_pb->rr_graph;
  node_index = cur_pb_graph_pin->pin_count_in_cluster;
  cur_pb_graph_pin->temp_net_num = pb_rr_nodes[node_index].vpack_net_num;

  return;
}

/* According to the vpack_net_num in cur_pb
 * assign it to the corresponding pb_graph_pins 
 */
void load_pb_graph_node_temp_net_num_from_pb(t_pb* cur_pb) {
  int iport, ipin;

  assert(NULL != cur_pb);
  assert(NULL != cur_pb->pb_graph_node);

  /* Input ports */
  for (iport = 0; iport < cur_pb->pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_input_pins[iport]; ipin++) {
      load_one_pb_graph_pin_temp_net_num_from_pb(cur_pb,
                                                 &(cur_pb->pb_graph_node->input_pins[iport][ipin]));
    }
  }

  /* Clock ports */
  for (iport = 0; iport < cur_pb->pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_clock_pins[iport]; ipin++) {
      load_one_pb_graph_pin_temp_net_num_from_pb(cur_pb,
                                                 &(cur_pb->pb_graph_node->clock_pins[iport][ipin]));
    }
  }

  /* Output ports */
  for (iport = 0; iport < cur_pb->pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb->pb_graph_node->num_output_pins[iport]; ipin++) {
      load_one_pb_graph_pin_temp_net_num_from_pb(cur_pb,
                                                 &(cur_pb->pb_graph_node->output_pins[iport][ipin]));
    }
  }
 
  return;
}

/* Recursively traverse the hierachy of a pb, 
 * store parasitic nets in the temp_net_num of the assoicated pb_graph_node 
 */
void rec_mark_one_pb_unused_pb_graph_node_temp_net_num(t_pb* cur_pb) {
  int ipb, jpb;
  int mode_index;

  /* Check */
  assert(NULL != cur_pb);

  if (NULL != cur_pb->pb_graph_node->pb_type->spice_model) {
    return;
  }
  /* Go recursively ... */
  mode_index = cur_pb->mode;
  if (!(0 < cur_pb->pb_graph_node->pb_type->num_modes)) {
    return;
  }
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        rec_mark_one_pb_unused_pb_graph_node_temp_net_num(&(cur_pb->child_pbs[ipb][jpb]));
      } else {
        /* Print idle graph_node muxes */
        load_pb_graph_node_temp_net_num_from_pb(cur_pb);
        /* We should update the net_num  */
        rec_mark_pb_graph_node_temp_net_num(cur_pb->child_pbs[ipb][jpb].pb_graph_node);
      }
    }
  }
  
  return;
}

void update_pb_vpack_net_num_from_temp_net_num(t_pb* cur_pb, 
                                               t_pb_graph_pin* cur_pb_graph_pin) {
  int node_index;
  t_rr_node* pb_rr_nodes = NULL;

  assert(NULL != cur_pb);
  assert(NULL != cur_pb->pb_graph_node);

  /* Get the selected edge of current pin*/
  pb_rr_nodes = cur_pb->rr_graph;
  node_index = cur_pb_graph_pin->pin_count_in_cluster;
  
  /* Avoid mistakenly modification */
  if (OPEN != pb_rr_nodes[node_index].vpack_net_num) {
    return;
  }
  /* Only modify when original vpack_net_num is open!!! */
  pb_rr_nodes[node_index].vpack_net_num = cur_pb_graph_pin->temp_net_num;

  return;
} 

void update_pb_graph_node_temp_net_num_to_pb(t_pb_graph_node* cur_pb_graph_node,
                                             t_pb* cur_pb) {
  int iport, ipin;
  t_rr_node* pb_rr_nodes = NULL;

  assert(NULL != cur_pb->pb_graph_node);
  assert(NULL != cur_pb);

  pb_rr_nodes = cur_pb->rr_graph;

  /* Input ports */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      update_pb_vpack_net_num_from_temp_net_num(cur_pb,
                                                &(cur_pb_graph_node->input_pins[iport][ipin]));
    }
  }

  /* Clock ports */
  for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
      update_pb_vpack_net_num_from_temp_net_num(cur_pb,
                                                &(cur_pb_graph_node->clock_pins[iport][ipin]));
    }
  }

  /* Output ports */
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      update_pb_vpack_net_num_from_temp_net_num(cur_pb,
                                                &(cur_pb_graph_node->output_pins[iport][ipin]));
    }
  }

  return;
}

void rec_load_unused_pb_graph_node_temp_net_num_to_pb(t_pb* cur_pb) {
  int ipb, jpb;
  int mode_index;

  /* Check */
  assert(NULL != cur_pb);

  if (NULL != cur_pb->pb_graph_node->pb_type->spice_model) {
    return;
  }
  /* Go recursively ... */
  mode_index = cur_pb->mode;
  if (!(0 < cur_pb->pb_graph_node->pb_type->num_modes)) {
    return;
  }
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        rec_load_unused_pb_graph_node_temp_net_num_to_pb(&(cur_pb->child_pbs[ipb][jpb]));
      } else {
        update_pb_graph_node_temp_net_num_to_pb(cur_pb->child_pbs[ipb][jpb].pb_graph_node, 
                                                cur_pb);
      }
    }
  }
  
  return;
}

void mark_one_pb_parasitic_nets(t_pb* cur_pb) {

  /* By go recursively, parasitic net num are stored in the temp_net_num in pb_graph_node */
  rec_mark_one_pb_unused_pb_graph_node_temp_net_num(cur_pb);

  /* Load the temp_net_num to vpack_net_num in the current pb! */
  rec_load_unused_pb_graph_node_temp_net_num_to_pb(cur_pb);

  return;
}

void init_rr_nodes_vpack_net_num_changed(int LL_num_rr_nodes,
                                         t_rr_node* LL_rr_node) {
  int inode;

  for (inode = 0; inode < LL_num_rr_nodes; inode++) {
    LL_rr_node[inode].vpack_net_num_changed = FALSE;
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

/* Count the number of configuration bits of a spice model */
int count_num_sram_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        int mux_size) {
  int num_sram_bits = 0;
  int iport;
  int lut_size;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    /* Determine size of LUT*/
    input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
    output_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
    sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port);
    lut_size = input_ports[0]->size;
    num_sram_bits = (int)pow(2.,(double)(lut_size));
    assert(num_sram_bits == sram_ports[0]->size);
    assert(1 == output_ports[0]->size);
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
    break;
  case SPICE_MODEL_MUX:
    assert((2 == mux_size)||(2 < mux_size));
    /* Number of configuration bits depends on the MUX structure */
    switch (cur_spice_model->design_tech_info.structure) {
    case SPICE_MODEL_STRUCTURE_TREE:
      num_sram_bits = determine_tree_mux_level(mux_size);
      break;
    case SPICE_MODEL_STRUCTURE_ONELEVEL:
      num_sram_bits = mux_size;
      break;
    case SPICE_MODEL_STRUCTURE_MULTILEVEL:
      num_sram_bits = cur_spice_model->design_tech_info.mux_num_level
                      * determine_num_input_basis_multilevel_mux(mux_size, 
                        cur_spice_model->design_tech_info.mux_num_level);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
                 __FILE__, __LINE__, cur_spice_model->name);
      exit(1);
    }
    /* For 2:1 MUX, whatever structure, there is only one level */
    if (2 == mux_size) {
      num_sram_bits = 1;
    }
    /* Also the number of configuration bits depends on the technology*/
    switch (cur_spice_model->design_tech) {
    case SPICE_MODEL_DESIGN_RRAM:
      /* 4T1R MUX requires more configuration bits */
      if (SPICE_MODEL_STRUCTURE_TREE == cur_spice_model->design_tech_info.structure) {
      /* For tree-structure: we need 3 times more config. bits */
        num_sram_bits = 3 * num_sram_bits;
      } else if (SPICE_MODEL_STRUCTURE_MULTILEVEL == cur_spice_model->design_tech_info.structure) {
      /* For multi-level structure: we need 1 more config. bits for each level */
        num_sram_bits += cur_spice_model->design_tech_info.mux_num_level;
      } else {
        num_sram_bits = (num_sram_bits + 1);
      }
      /* For 2:1 MUX, whatever structure, there is only one level */
      if (2 == mux_size) {
        num_sram_bits = 3;
      } 
      break;
    case SPICE_MODEL_DESIGN_CMOS:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
                 __FILE__, __LINE__, cur_spice_model->name); 
      exit(1);
    }
    break;
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_SCFF:
  case SPICE_MODEL_VDD:
  case SPICE_MODEL_GND:
  case SPICE_MODEL_IOPAD:
    /* Other block, we just count the number SRAM ports defined by user */
    num_sram_bits = 0;
    sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
    /* TODO: could be more smart! 
     * Support Non-volatile RRAM-based SRAM */
    if (0 < num_sram_port) {
      assert(NULL != sram_ports);
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
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return num_sram_bits;
}

/* For a multiplexer, determine its reserved configuration bits */
int count_num_reserved_conf_bits_one_lut_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type) {
  int num_reserved_conf_bits = 0;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Check */
  assert(SPICE_MODEL_LUT == cur_spice_model->type);

  /* Determine size of LUT*/
  sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_sram_port);
  /* TODO: could be more smart! Use mapped spice_model of SRAM ports!  
   * Support Non-volatile RRAM-based SRAM */
  switch (sram_ports[0]->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_RRAM:
  /* Non-volatile SRAM requires 2 BLs and 2 WLs for each 1 memory bit, 
   * In memory bank, by intensively share the Bit/Word Lines,
   * we only need 1 additional BL and WL for each memory bit.
   * Number of memory bits is still same as CMOS SRAM
   */
    num_reserved_conf_bits = 
      count_num_reserved_conf_bits_one_rram_sram_spice_model(sram_ports[0]->spice_model,
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

  /* Free */
  my_free(sram_ports);

  return num_reserved_conf_bits;
}

/* For a multiplexer, determine its reserved configuration bits */
int count_num_reserved_conf_bits_one_mux_spice_model(t_spice_model* cur_spice_model,
                                                     enum e_sram_orgz cur_sram_orgz_type,
                                                     int mux_size) {
  int num_reserved_conf_bits = 0;

  /* Check */
  assert(SPICE_MODEL_MUX == cur_spice_model->type);
  assert((2 == mux_size)||(2 < mux_size));

  /* Number of configuration bits depends on the MUX structure */
  switch (cur_spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    num_reserved_conf_bits = 2;
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    num_reserved_conf_bits = mux_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_reserved_conf_bits = cur_spice_model->design_tech_info.mux_num_level * 
                             determine_num_input_basis_multilevel_mux(mux_size, 
                             cur_spice_model->design_tech_info.mux_num_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name);
    exit(1);
  }
  /* For 2:1 MUX, whatever structure, there is only one level */
  if (2 == mux_size) {
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
      if (2 == mux_size) {
        num_reserved_conf_bits = 2;
      } 
      break;
    case SPICE_SRAM_SCAN_CHAIN:
    case SPICE_SRAM_STANDALONE:
      /* 4T1R MUX requires more configuration bits */
      if (SPICE_MODEL_STRUCTURE_TREE == cur_spice_model->design_tech_info.structure) {
      /* For tree-structure: we need 3 times more config. bits */
        num_reserved_conf_bits = 0;
      } else if (SPICE_MODEL_STRUCTURE_MULTILEVEL == cur_spice_model->design_tech_info.structure) {
      /* For multi-level structure: we need 1 more config. bits for each level */
        num_reserved_conf_bits = 0;
      } else {
        num_reserved_conf_bits = 0;
      }
      /* For 2:1 MUX, whatever structure, there is only one level */
      if (2 == mux_size) {
        num_reserved_conf_bits = 0;
      } 
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
  case SPICE_MODEL_SCFF:
  case SPICE_MODEL_VDD:
  case SPICE_MODEL_GND:
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



/* Count the number of configuration bits of a spice model */
int count_num_conf_bits_one_spice_model(t_spice_model* cur_spice_model,
                                        enum e_sram_orgz cur_sram_orgz_type,
                                        int mux_size) {
  int num_conf_bits = 0;
  int iport;
  int lut_size;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  assert(NULL != cur_spice_model);

  /* Only LUT and MUX requires configuration bits*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    /* Determine size of LUT*/
    input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
    output_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
    sram_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port);
    lut_size = input_ports[0]->size;
    num_conf_bits = (int)pow(2.,(double)(lut_size));
    assert(num_conf_bits == sram_ports[0]->size);
    assert(1 == output_ports[0]->size);
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
    break;
  case SPICE_MODEL_MUX:
    assert((2 == mux_size)||(2 < mux_size));
    /* Number of configuration bits depends on the MUX structure */
    switch (cur_spice_model->design_tech_info.structure) {
    case SPICE_MODEL_STRUCTURE_TREE:
      num_conf_bits = determine_tree_mux_level(mux_size);
      break;
    case SPICE_MODEL_STRUCTURE_ONELEVEL:
      num_conf_bits = mux_size;
      break;
    case SPICE_MODEL_STRUCTURE_MULTILEVEL:
      num_conf_bits = cur_spice_model->design_tech_info.mux_num_level
                      * determine_num_input_basis_multilevel_mux(mux_size, 
                        cur_spice_model->design_tech_info.mux_num_level);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
                 __FILE__, __LINE__, cur_spice_model->name);
      exit(1);
    }
    /* For 2:1 MUX, whatever structure, there is only one level */
    if (2 == mux_size) {
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
        num_conf_bits = cur_spice_model->design_tech_info.mux_num_level;
        /* For 2:1 MUX, whatever structure, there is only one level */
        if (2 == mux_size) {
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
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
                 __FILE__, __LINE__, cur_spice_model->name); 
      exit(1);
    }
    break;
  case SPICE_MODEL_WIRE:
  case SPICE_MODEL_FF:
  case SPICE_MODEL_SRAM:
  case SPICE_MODEL_HARDLOGIC:
  case SPICE_MODEL_SCFF:
  case SPICE_MODEL_VDD:
  case SPICE_MODEL_GND:
  case SPICE_MODEL_IOPAD:
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
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Free */
  my_free(input_ports);
  my_free(output_ports);
  my_free(sram_ports);

  return num_conf_bits;
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


int count_num_conf_bit_one_interc(t_interconnect* cur_interc,
                                  enum e_sram_orgz cur_sram_orgz_type) {
  int fan_in = 0;
  enum e_interconnect spice_interc_type = DIRECT_INTERC;

  int num_conf_bits = 0;

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  if (NULL == cur_interc) { 
    return num_conf_bits;
  } else {
    fan_in = cur_interc->fan_in;
    if (0 == fan_in) {
      return num_conf_bits;
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
    num_conf_bits = count_num_conf_bits_one_spice_model(cur_interc->spice_model, 
                                                        cur_sram_orgz_type, fan_in);
    /* FOR COMPLETE_INTERC: we should consider fan_out number ! */
    num_conf_bits = num_conf_bits * cur_interc->fan_out;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  return num_conf_bits;
}

/* Count the number of configuration bits of interconnection inside a pb_type in its default mode */
int count_num_conf_bits_pb_type_mode_interc(t_mode* cur_pb_type_mode,
                                            enum e_sram_orgz cur_sram_orgz_type) {
  int num_conf_bits = 0;
  int jinterc = 0;

  for (jinterc = 0; jinterc < cur_pb_type_mode->num_interconnect; jinterc++) {
    num_conf_bits += count_num_conf_bit_one_interc(&(cur_pb_type_mode->interconnect[jinterc]),
                                                   cur_sram_orgz_type); 
  }
  
  return num_conf_bits;
} 

/* Count the number of configuration bits of interconnection inside a pb_type in its default mode */
int count_num_reserved_conf_bits_pb_type_mode_interc(t_mode* cur_pb_type_mode,
                                                     enum e_sram_orgz cur_sram_orgz_type) {
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;
  int jinterc = 0;

  for (jinterc = 0; jinterc < cur_pb_type_mode->num_interconnect; jinterc++) {
    /* num of reserved configuration bits is determined by the largest one */
    temp_num_reserved_conf_bits = 
                     count_num_reserved_conf_bit_one_interc(&(cur_pb_type_mode->interconnect[jinterc]),
                                                            cur_sram_orgz_type); 
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
  }
  
  return num_reserved_conf_bits;
}

/* Count the number of configuration bits of a grid (type_descriptor) in default mode */
int rec_count_num_conf_bits_pb_type_default_mode(t_pb_type* cur_pb_type,
                                                 t_sram_orgz_info* cur_sram_orgz_info) {
  int mode_index, ipb, jpb;
  int sum_num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  cur_pb_type->default_mode_num_conf_bits = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        rec_count_num_conf_bits_pb_type_default_mode(&(cur_pb_type->modes[mode_index].pb_type_children[ipb]), cur_sram_orgz_info);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    sum_num_conf_bits = count_num_conf_bits_one_spice_model(cur_pb_type->spice_model, cur_sram_orgz_info->type, 0);
    cur_pb_type->default_mode_num_conf_bits = sum_num_conf_bits;
    /* calculate the number of reserved configuration bits */
    cur_pb_type->default_mode_num_reserved_conf_bits = 
         count_num_reserved_conf_bits_one_spice_model(cur_pb_type->spice_model,
                                                      cur_sram_orgz_info->type, 0);
  } else { /* Count the sum of configuration bits of all the children pb_types */
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Count in the number of configuration bits of on child pb_type */
        sum_num_conf_bits += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_conf_bits;
        temp_num_reserved_conf_bits = 
                            cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_reserved_conf_bits;
        /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
        if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
          num_reserved_conf_bits = temp_num_reserved_conf_bits;
        }
      }
    }
    /* Count the number of configuration bits of interconnection */
    sum_num_conf_bits += count_num_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]), cur_sram_orgz_info->type); 
    /* Count the number of reserved_configuration bits of interconnection */
    temp_num_reserved_conf_bits = 
                count_num_reserved_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]),
                                                                 cur_sram_orgz_info->type); 
    /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
    /* Update the info in pb_type */
    cur_pb_type->default_mode_num_reserved_conf_bits = num_reserved_conf_bits;
    cur_pb_type->default_mode_num_conf_bits = sum_num_conf_bits;
  }

  return sum_num_conf_bits;
}

/* Count the number of configuration bits of a grid (type_descriptor) in default mode */
int rec_count_num_conf_bits_pb_type_physical_mode(t_pb_type* cur_pb_type,
                                                  t_sram_orgz_info* cur_sram_orgz_info) {
  int mode_index, ipb, jpb;
  int sum_num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  cur_pb_type->physical_mode_num_conf_bits = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        rec_count_num_conf_bits_pb_type_physical_mode(&(cur_pb_type->modes[mode_index].pb_type_children[ipb]), cur_sram_orgz_info);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    sum_num_conf_bits = count_num_conf_bits_one_spice_model(cur_pb_type->spice_model, cur_sram_orgz_info->type, 0);
    cur_pb_type->physical_mode_num_conf_bits = sum_num_conf_bits;
    /* calculate the number of reserved configuration bits */
    cur_pb_type->physical_mode_num_reserved_conf_bits = 
         count_num_reserved_conf_bits_one_spice_model(cur_pb_type->spice_model,
                                                      cur_sram_orgz_info->type, 0);
  } else { /* Count the sum of configuration bits of all the children pb_types */
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Count in the number of configuration bits of on child pb_type */
        sum_num_conf_bits += cur_pb_type->modes[mode_index].pb_type_children[ipb].physical_mode_num_conf_bits;
        temp_num_reserved_conf_bits = cur_pb_type->modes[mode_index].pb_type_children[ipb].physical_mode_num_reserved_conf_bits;

        /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
        if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
          num_reserved_conf_bits = temp_num_reserved_conf_bits;
        }
      }
    }
    /* Count the number of configuration bits of interconnection */
    sum_num_conf_bits += count_num_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]), 
                                                                 cur_sram_orgz_info->type); 
    /* Count the number of reserved_configuration bits of interconnection */
    temp_num_reserved_conf_bits = 
                count_num_reserved_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]),
                                                                 cur_sram_orgz_info->type); 
    /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
    /* Update the info in pb_type */
    cur_pb_type->physical_mode_num_reserved_conf_bits = num_reserved_conf_bits;
    cur_pb_type->physical_mode_num_conf_bits = sum_num_conf_bits;
  }

  return sum_num_conf_bits;
}


/* Count the number of configuration bits of a pb_graph_node */
int rec_count_num_conf_bits_pb(t_pb* cur_pb, 
                               t_sram_orgz_info* cur_sram_orgz_info) {
  int mode_index, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;
  t_pb_graph_node* cur_pb_graph_node = NULL;
  int sum_num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_graph_node = cur_pb->pb_graph_node;
  cur_pb_type = cur_pb_graph_node->pb_type;
  mode_index = cur_pb->mode; 

  cur_pb->num_conf_bits = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* recursive for the child_pbs*/
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          rec_count_num_conf_bits_pb(&(cur_pb->child_pbs[ipb][jpb]), cur_sram_orgz_info);
        } else {
          /* Check if this pb has no children, no children mean idle*/
          rec_count_num_conf_bits_pb_type_default_mode(cur_pb->child_pbs[ipb][jpb].pb_graph_node->pb_type,
                                                       cur_sram_orgz_info);
        }
      } 
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    sum_num_conf_bits += count_num_conf_bits_one_spice_model(cur_pb_type->spice_model, 
                                                             cur_sram_orgz_info->type, 0);
    cur_pb->num_conf_bits = sum_num_conf_bits;
    /* calculate the number of reserved configuration bits */
    cur_pb->num_reserved_conf_bits = 
         count_num_reserved_conf_bits_one_spice_model(cur_pb_type->spice_model,
                                                      cur_sram_orgz_info->type, 0);
 
  } else {
    /* Definition ends*/
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* we should make sure this placement index == child_pb_type[jpb]*/
        assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          /* Count in the number of configuration bits of on child pb */
          sum_num_conf_bits += cur_pb->child_pbs[ipb][jpb].num_conf_bits;
          temp_num_reserved_conf_bits = cur_pb->child_pbs[ipb][jpb].num_reserved_conf_bits;
        } else {
          /* Count in the number of configuration bits of on child pb_type */
          sum_num_conf_bits += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_conf_bits;
          temp_num_reserved_conf_bits = 
                            cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_reserved_conf_bits;
        }
        /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
        if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
          num_reserved_conf_bits = temp_num_reserved_conf_bits;
        }
      }
    }
    /* Count the number of configuration bits of interconnection */
    sum_num_conf_bits += count_num_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]),
                                                                 cur_sram_orgz_info->type); 
    /* Count the number of reserved_configuration bits of interconnection */
    temp_num_reserved_conf_bits = 
                count_num_reserved_conf_bits_pb_type_mode_interc(&(cur_pb_type->modes[mode_index]),
                                                                 cur_sram_orgz_info->type); 
    /* number of reserved conf. bits is deteremined by the largest number of reserved conf. bits !*/
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
    /* Update the info in pb_type */
    cur_pb->num_reserved_conf_bits = num_reserved_conf_bits;
    cur_pb->num_conf_bits = sum_num_conf_bits;
  }

  return sum_num_conf_bits;
}

/* Initialize the number of configuraion bits for one grid */
void init_one_grid_num_conf_bits(int ix, int iy, 
                                 t_sram_orgz_info* cur_sram_orgz_info) {
  t_block* mapped_block = NULL;
  int iz;
  int cur_block_index = 0;
  int capacity; 

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 
 
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    /* Empty grid, directly return */
    return; 
  }
  capacity= grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    rec_count_num_conf_bits_pb_type_physical_mode(grid[ix][iy].type->pb_type, cur_sram_orgz_info);
    /* Comments: Grid [x][y]*/
    if (NULL == mapped_block) {
      /* Print a consider a idle pb_type ...*/
      rec_count_num_conf_bits_pb_type_default_mode(grid[ix][iy].type->pb_type, cur_sram_orgz_info);
    } else {
      if (iz == mapped_block->z) {
        // assert(mapped_block == &(block[grid[ix][iy].blocks[cur_block_index]]));
        cur_block_index++;
      }
      rec_count_num_conf_bits_pb(mapped_block->pb, cur_sram_orgz_info);
    }
  } 

  assert(cur_block_index == grid[ix][iy].usage);

  return;
}

/* Initialize the number of configuraion bits for all grids */
void init_grids_num_conf_bits(t_sram_orgz_info* cur_sram_orgz_info) {
  int ix, iy; 

  /* Core grid */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of configuration bits of Core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      init_one_grid_num_conf_bits(ix, iy, cur_sram_orgz_info);
    }
  }
  
  /* Consider the IO pads */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of configuration bits of I/O grids...\n");
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_conf_bits(ix, iy, cur_sram_orgz_info);
  }
  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_conf_bits(ix, iy, cur_sram_orgz_info);
  }
  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_conf_bits(ix, iy, cur_sram_orgz_info);
  }
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_conf_bits(ix, iy, cur_sram_orgz_info);
  }
 
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

/* Count the number of inpad and outpad of a grid (type_descriptor) in default mode */
void rec_count_num_iopads_pb_type_physical_mode(t_pb_type* cur_pb_type) {
  int mode_index, ipb, jpb;
  int sum_num_iopads = 0;

  cur_pb_type->physical_mode_num_iopads = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        rec_count_num_iopads_pb_type_physical_mode(&(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_IOPAD == cur_pb_type->spice_model->type) {
      sum_num_iopads = 1;
      cur_pb_type->physical_mode_num_iopads = sum_num_iopads;
    }
  } else { /* Count the sum of configuration bits of all the children pb_types */
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Count in the number of configuration bits of on child pb_type */
        sum_num_iopads += cur_pb_type->modes[mode_index].pb_type_children[ipb].physical_mode_num_iopads;
      }
    }
    /* Count the number of configuration bits of interconnection */
    /* Update the info in pb_type */
    cur_pb_type->physical_mode_num_iopads = sum_num_iopads;
  }

  return;
}

/* Count the number of inpad and outpad of a grid (type_descriptor) in default mode */
void rec_count_num_iopads_pb_type_default_mode(t_pb_type* cur_pb_type) {
  int mode_index, ipb, jpb;
  int sum_num_iopads = 0;

  cur_pb_type->default_mode_num_iopads = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        rec_count_num_iopads_pb_type_default_mode(&(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_IOPAD == cur_pb_type->spice_model->type) {
      sum_num_iopads = 1;
      cur_pb_type->default_mode_num_iopads = sum_num_iopads;
    }
  } else { /* Count the sum of configuration bits of all the children pb_types */
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Count in the number of configuration bits of on child pb_type */
        sum_num_iopads += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_iopads;
      }
    }
    /* Count the number of configuration bits of interconnection */
    /* Update the info in pb_type */
    cur_pb_type->default_mode_num_iopads = sum_num_iopads;
  }

  return;
}

/* Count the number of configuration bits of a pb_graph_node */
void rec_count_num_iopads_pb(t_pb* cur_pb) {
  int mode_index, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;
  t_pb_graph_node* cur_pb_graph_node = NULL;

  int sum_num_iopads = 0;

  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_graph_node = cur_pb->pb_graph_node;
  cur_pb_type = cur_pb_graph_node->pb_type;
  mode_index = cur_pb->mode; 

  cur_pb->num_iopads = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* recursive for the child_pbs*/
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          rec_count_num_iopads_pb(&(cur_pb->child_pbs[ipb][jpb]));
        } else {
          /* Check if this pb has no children, no children mean idle*/
          rec_count_num_iopads_pb_type_default_mode(cur_pb->child_pbs[ipb][jpb].pb_graph_node->pb_type);
        }
      } 
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_IOPAD == cur_pb_type->spice_model->type) {
      sum_num_iopads = 1;
      cur_pb->num_iopads = sum_num_iopads;
    }
  } else {
    /* Definition ends*/
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* we should make sure this placement index == child_pb_type[jpb]*/
        assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          /* Count in the number of configuration bits of on child pb */
          sum_num_iopads += cur_pb->child_pbs[ipb][jpb].num_iopads;
        } else {
          /* Count in the number of configuration bits of on child pb_type */
          sum_num_iopads += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_iopads;
        }
      }
    }
    /* Count the number of configuration bits of interconnection */
    /* Update the info in pb_type */
    cur_pb->num_iopads = sum_num_iopads;
  }

  return;
}

/* Initialize the number of configuraion bits for one grid */
void init_one_grid_num_iopads(int ix, int iy) {
  t_block* mapped_block = NULL;
  int iz;
  int cur_block_index = 0;
  int capacity; 

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 
 
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    /* Empty grid, directly return */
    return; 
  }
  capacity= grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    rec_count_num_iopads_pb_type_physical_mode(grid[ix][iy].type->pb_type);
    /* Comments: Grid [x][y]*/
    if (NULL == mapped_block) {
      /* Print a consider a idle pb_type ...*/
      rec_count_num_iopads_pb_type_default_mode(grid[ix][iy].type->pb_type);
    } else {
      if (iz == mapped_block->z) {
        // assert(mapped_block == &(block[grid[ix][iy].blocks[cur_block_index]]));
        cur_block_index++;
      }
      rec_count_num_iopads_pb(mapped_block->pb);
    }
  } 

  assert(cur_block_index == grid[ix][iy].usage);

  return;
}

/* Initialize the number of configuraion bits for all grids */
void init_grids_num_iopads() {
  int ix, iy; 

  /* Core grid */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of I/O pads of Core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      init_one_grid_num_iopads(ix, iy);
    }
  }
  
  /* Consider the IO pads */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of I/O pads of I/O grids...\n");
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_iopads(ix, iy);
  }
  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_iopads(ix, iy);
  }
  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_iopads(ix, iy);
  }
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_iopads(ix, iy);
  }
 
  return;
}

/* Count the number of mode configuration bits of a grid (type_descriptor) in default mode */
void rec_count_num_mode_bits_pb_type_default_mode(t_pb_type* cur_pb_type) {
  int mode_index, ipb, jpb;
  int sum_num_mode_bits = 0;

  cur_pb_type->default_mode_num_mode_bits = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        rec_count_num_mode_bits_pb_type_default_mode(&(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_IOPAD == cur_pb_type->spice_model->type) {
      sum_num_mode_bits = 1;
      cur_pb_type->default_mode_num_mode_bits = sum_num_mode_bits;
    }
  } else { /* Count the sum of configuration bits of all the children pb_types */
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Count in the number of configuration bits of on child pb_type */
        sum_num_mode_bits += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_mode_bits;
      }
    }
    /* Count the number of configuration bits of interconnection */
    /* Update the info in pb_type */
    cur_pb_type->default_mode_num_mode_bits = sum_num_mode_bits;
  }

  return;
}

/* Count the number of configuration bits of a pb_graph_node */
void rec_count_num_mode_bits_pb(t_pb* cur_pb) {
  int mode_index, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;
  t_pb_graph_node* cur_pb_graph_node = NULL;

  int sum_num_mode_bits = 0;

  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_graph_node = cur_pb->pb_graph_node;
  cur_pb_type = cur_pb_graph_node->pb_type;
  mode_index = cur_pb->mode; 

  cur_pb->num_mode_bits = 0;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* recursive for the child_pbs*/
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) { 
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          rec_count_num_mode_bits_pb(&(cur_pb->child_pbs[ipb][jpb]));
        } else {
          /* Check if this pb has no children, no children mean idle*/
          rec_count_num_mode_bits_pb_type_default_mode(cur_pb->child_pbs[ipb][jpb].pb_graph_node->pb_type);
        }
      } 
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_IOPAD == cur_pb_type->spice_model->type) {
      sum_num_mode_bits = 1;
      cur_pb->num_mode_bits = sum_num_mode_bits;
    }
  } else {
    /* Definition ends*/
    /* Quote all child pb_types */
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      /* Each child may exist multiple times in the hierarchy*/
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* we should make sure this placement index == child_pb_type[jpb]*/
        assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          /* Count in the number of configuration bits of on child pb */
          sum_num_mode_bits += cur_pb->child_pbs[ipb][jpb].num_mode_bits;
        } else {
          /* Count in the number of configuration bits of on child pb_type */
          sum_num_mode_bits += cur_pb_type->modes[mode_index].pb_type_children[ipb].default_mode_num_mode_bits;
        }
      }
    }
    /* Count the number of configuration bits of interconnection */
    /* Update the info in pb_type */
    cur_pb->num_mode_bits = sum_num_mode_bits;
  }

  return;
}

/* Initialize the number of configuraion bits for one grid */
void init_one_grid_num_mode_bits(int ix, int iy) {
  t_block* mapped_block = NULL;
  int iz;
  int cur_block_index = 0;
  int capacity; 

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 
 
  if ((NULL == grid[ix][iy].type)||(0 != grid[ix][iy].offset)) {
    /* Empty grid, directly return */
    return; 
  }
  capacity= grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    /* Comments: Grid [x][y]*/
    if (NULL == mapped_block) {
      /* Print a consider a idle pb_type ...*/
      rec_count_num_mode_bits_pb_type_default_mode(grid[ix][iy].type->pb_type);
    } else {
      if (iz == mapped_block->z) {
        // assert(mapped_block == &(block[grid[ix][iy].blocks[cur_block_index]]));
        cur_block_index++;
      }
      rec_count_num_mode_bits_pb(mapped_block->pb);
    }
  } 

  assert(cur_block_index == grid[ix][iy].usage);

  return;
}

/* Initialize the number of configuraion bits for all grids */
void init_grids_num_mode_bits() {
  int ix, iy; 

  /* Core grid */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of mode configuraiton bits of Core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      init_one_grid_num_mode_bits(ix, iy);
    }
  }
  
  /* Consider the IO pads */
  vpr_printf(TIO_MESSAGE_INFO, "INFO: Initializing number of mode configuration bits of I/O grids...\n");
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_mode_bits(ix, iy);
  }
  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_mode_bits(ix, iy);
  }
  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_mode_bits(ix, iy);
  }
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    init_one_grid_num_mode_bits(ix, iy);
  }
 
  return;
}


/* Check if this SPICE model defined as SRAM 
 * contain necessary ports for its functionality 
 */
void check_sram_spice_model_ports(t_spice_model* cur_spice_model,
                                  boolean include_bl_wl) {
  int num_input_ports;
  t_spice_model_port** input_ports = NULL;
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

  /* Check if we has 1 input other than global ports */
  input_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, TRUE);
  num_global_ports = 0;
  for (iport = 0; iport < num_input_ports; iport++) {
    if (TRUE == input_ports[iport]->is_global) {
      num_global_ports++;
    }
  }
  if (1 != (num_input_ports - num_global_ports)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have only 1 non-global input port!\n",
               __FILE__, __LINE__);
    num_err++;
    if (1 != input_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have an input port with size 1!\n",
                 __FILE__, __LINE__);
      num_err++;
    }
  }
  /* Check if we has 1 output with size 2 */
  output_ports = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_ports, TRUE);
  num_global_ports = 0;
  for (iport = 0; iport < num_output_ports; iport++) {
    if (TRUE == output_ports[iport]->is_global) {
      num_global_ports++;
    }
  }
  if (1 != (num_output_ports - num_global_ports)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have only 1 non-global output port!\n",
               __FILE__, __LINE__);
    num_err++;
    if (2 != output_ports[0]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SRAM SPICE MODEL should have a output port with size 2!\n",
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
  my_free(input_ports);
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
    if (1 != num_output_ports) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL should have only 1 output port!\n",
               __FILE__, __LINE__);
      num_err++;
      if (2 != output_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) SCFF SPICE MODEL: the output port with size 2!\n",
                   __FILE__, __LINE__);
        num_err++;
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

/* add configuration bits of a MUX to linked-list
 * when SRAM organization type is scan-chain */
void  
add_mux_scff_conf_bits_to_llist(int mux_size,
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
  t_conf_bit** sram_bit = NULL;
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
  my_free(sram_bit);
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
    add_mux_scff_conf_bits_to_llist(mux_size, cur_sram_orgz_info, 
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

/* Add SCFF configutration bits to a linked list*/
void  
add_sram_scff_conf_bits_to_llist(t_sram_orgz_info* cur_sram_orgz_info, 
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
  int ibit, cur_bl, cur_wl, cur_mem_bit;
  t_spice_model* cur_sram_spice_model = NULL;
  t_conf_bit* bl_bit = NULL;
  t_conf_bit* wl_bit = NULL;
  int bit_cnt = 0;

  /* Assert*/
  assert(NULL != cur_sram_orgz_info);

  /* Get current counter of sram_spice_model */
  cur_mem_bit = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
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
    add_sram_scff_conf_bits_to_llist(cur_sram_orgz_info, 
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

/* Decode mode bits "01..." to a SRAM bits array */
int* decode_mode_bits(char* mode_bits, int* num_sram_bits) {
  int* sram_bits = NULL;
  int i;
  
  assert(NULL != mode_bits);
  (*num_sram_bits) = strlen(mode_bits);
 
  sram_bits = (int*)my_calloc((*num_sram_bits), sizeof(int));

  for (i = 0; i < (*num_sram_bits); i++) {
    switch(mode_bits[i]) {
    case '1':
      sram_bits[i] = 1;
      break;
    case '0':
      sram_bits[i] = 0;
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid mode_bits(%s)!\n",
                 __FILE__, __LINE__, mode_bits);
      exit(1);
    }
  }

  return sram_bits;
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

char* complete_truth_table_line(int lut_size,
                                char* input_truth_table_line) {
  char* ret = NULL;
  int num_token = 0;
  char** tokens = NULL;
  int cover_len = 0;
  int j;

  /* Due to the size of truth table may be less than the lut size.
   * i.e. in LUT-6 architecture, there exists LUT1-6 in technology-mapped netlists
   * So, in truth table line, there may be 10- 1
   * In this case, we should complete it by --10- 1
   */ 
  /*Malloc the completed truth table, lut_size + space + truth_val + '\0'*/
  ret = (char*)my_malloc(sizeof(char)*lut_size + 3);
  /* Split one line of truth table line*/
  tokens = my_strtok(input_truth_table_line, " ", &num_token); 
  /* Check, only 2 tokens*/
  /* Sometimes, the truth table is ' 0' or ' 1', which corresponds to a constant */
  if (1 == num_token) {
    /* restore the token[0]*/
    tokens = (char**)realloc(tokens, 2*sizeof(char*));
    tokens[1] = tokens[0];
    tokens[0] = my_strdup("-");
    num_token = 2;
  }

  /* In Most cases, there should be 2 tokens. */
  assert(2 == num_token);
  if ((0 != strcmp(tokens[1], "1"))&&(0 != strcmp(tokens[1], "0"))) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Last token of truth table line should be [0|1]!\n",
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Complete the truth table line*/
  cover_len = strlen(tokens[0]); 
  assert((cover_len < lut_size)||(cover_len == lut_size));

  /* Copy the original truth table line */ 
  for (j = 0; j < cover_len; j++) {
    ret[j] = tokens[0][j];
  }
  /* Add the number of '-' we should add in the back !!! */
  for (j = cover_len; j < lut_size; j++) {
    ret[j] = '-';
  }

  /* Copy the original truth table line */ 
  sprintf(ret + lut_size, " %s", tokens[1]);

  /* Free */
  for (j = 0; j < num_token; j++) {
    my_free(tokens[j]);
  }

  return ret;
}

/* For each lut_bit_lines, we should recover the truth table,
 * and then set the sram bits to "1" if the truth table defines so.
 * Start_point: the position we start decode recursively
 */
void configure_lut_sram_bits_per_line_rec(int** sram_bits, 
                                          int lut_size,
                                          char* truth_table_line,
                                          int start_point) {
  int i;
  int num_sram_bit = (int)pow(2., (double)(lut_size));
  char* temp_line = my_strdup(truth_table_line);
  int do_config = 1;
  int sram_id = 0;

  /* Check the length of sram bits and truth table line */
  //assert((sizeof(int)*num_sram_bit) == sizeof(*sram_bits)); /*TODO: fix this assert*/
  assert((unsigned)(lut_size + 1 + 1)== strlen(truth_table_line)); /* lut_size + space + '1' */
  /* End of truth_table_line should be "space" and "1" */ 
  assert((0 == strcmp(" 1", truth_table_line + lut_size))||(0 == strcmp(" 0", truth_table_line + lut_size)));
  /* Make sure before start point there is no '-' */
  for (i = 0; i < start_point; i++) {
    assert('-' != truth_table_line[i]);
  }

  /* Configure sram bits recursively */
  for (i = start_point; i < lut_size; i++) {
    if ('-' == truth_table_line[i]) {
      do_config = 0;
      /* if we find a dont_care, we don't do configure now but recursively*/
      /* '0' branch */
      temp_line[i] = '0'; 
      configure_lut_sram_bits_per_line_rec(sram_bits, lut_size, temp_line, start_point + 1);
      /* '1' branch */
      temp_line[i] = '1'; 
      configure_lut_sram_bits_per_line_rec(sram_bits, lut_size, temp_line, start_point + 1);
      break; 
    }
  }

  /* do_config*/
  if (do_config) {
    for (i = 0; i < lut_size; i++) {
      /* Should be either '0' or '1' */
      switch (truth_table_line[i]) {
      case '0':
        /* We assume the 1-lut pass sram1 when input = 0 */
        sram_id += (int)pow(2., (double)(i));
        break;
      case '1':
        /* We assume the 1-lut pass sram0 when input = 1 */
        break;
      case '-':
        assert('-' != truth_table_line[i]); /* Make sure there is no dont_care */
      default :
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid truth_table bit(%c), should be [0|1|'-]!\n",
                   __FILE__, __LINE__, truth_table_line[i]); 
        exit(1);
      }
    }
    /* Set the sram bit to '1'*/
    assert((-1 < sram_id) && (sram_id < num_sram_bit));
    if (0 == strcmp(" 1", truth_table_line + lut_size)) {
      (*sram_bits)[sram_id] = 1; /* on set*/
    } else if (0 == strcmp(" 0", truth_table_line + lut_size)) {
      (*sram_bits)[sram_id] = 0; /* off set */
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid truth_table_line ending(=%s)!\n",
                 __FILE__, __LINE__, truth_table_line + lut_size);
      exit(1);
    }
  }
  
  /* Free */
  my_free(temp_line);

  return; 
}

/* Determine if the truth table of a LUT is a on-set or a off-set */
int determine_lut_truth_table_on_set(int truth_table_len,
                                     char** truth_table) {
  int on_set = 0;
  int off_set = 0;
  int i, tt_line_len;

  for (i = 0; i < truth_table_len; i++) {
    tt_line_len = strlen(truth_table[i]);
    switch (truth_table[i][tt_line_len - 1]) {
    case '1':
      on_set = 1;
      break;
    case '0':
      off_set = 1;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid truth_table_line ending(=%c)!\n",
                 __FILE__, __LINE__, truth_table[i][tt_line_len - 1]);
      exit(1);
    }
  }

  /* Prefer on_set if both are true */
  if (1 == (on_set + off_set)) {
    on_set = 1; off_set = 0;
  }

  return on_set;
}


int* generate_lut_sram_bits(int truth_table_len,
                            char** truth_table,
                            int lut_size,
                            int default_sram_bit_value) {
  int num_sram = (int)pow(2.,(double)(lut_size));
  int* ret = (int*)my_malloc(sizeof(int)*num_sram); 
  char** completed_truth_table = (char**)my_malloc(sizeof(char*)*truth_table_len);
  int on_set = 0;
  int off_set = 0;
  int i;

  /* if No truth_table, do default*/
  if (0 == truth_table_len) {
    switch (default_sram_bit_value) {
    case 0:
      off_set = 0;
      on_set = 1;
      break;
    case 1:
      off_set = 1;
      on_set = 0;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid default_signal_init_value(=%d)!\n",
                 __FILE__, __LINE__, default_sram_bit_value);
      exit(1);
    }
  } else {
    on_set = determine_lut_truth_table_on_set(truth_table_len, truth_table);
    off_set = 1 - on_set;
  }

  /* Read in truth table lines, decode one by one */
  for (i = 0; i < truth_table_len; i++) {
    /* Complete the truth table line by line*/
    //printf("truth_table[%d] = %s\n", i, truth_table[i]);
    completed_truth_table[i] = complete_truth_table_line(lut_size, truth_table[i]);
    //printf("Completed_truth_table[%d] = %s\n", i, completed_truth_table[i]);
  }
  //printf("on_set=%d off_set=%d", on_set, off_set);
  assert(1 == (on_set + off_set));

  if (1 == on_set) {
    /* Initial all sram bits to 0*/
    for (i = 0 ; i < num_sram; i++) {
      ret[i] = 0;
    }
  } else if (1 == off_set) {
    /* Initial all sram bits to 1*/
    for (i = 0 ; i < num_sram; i++) {
      ret[i] = 1;
    }
  }

  for (i = 0; i < truth_table_len; i++) {
    /* Update the truth table, sram_bits */
    configure_lut_sram_bits_per_line_rec(&ret, lut_size, completed_truth_table[i], 0);
  }

  /* Free */
  for (i = 0; i < truth_table_len; i++) {
    my_free(completed_truth_table[i]);
  }

  return ret;
}

char** assign_lut_truth_table(t_logical_block* mapped_logical_block,
                              int* truth_table_length) {
  char** truth_table = NULL;
  t_linked_vptr* head = NULL;
  int cur = 0;

  if (NULL == mapped_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid mapped_logical_block!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Count the lines of truth table*/
  head = mapped_logical_block->truth_table;
  while(head) {
    (*truth_table_length)++;
    head = head->next;
  }
  /* Allocate truth_tables */
  truth_table = (char**)my_malloc(sizeof(char*)*(*truth_table_length));
  /* Fill truth_tables*/
  cur = 0;
  head = mapped_logical_block->truth_table;
  while(head) {
    truth_table[cur] = my_strdup((char*)head->data_vptr);
    head = head->next;
    cur++;
  }
  assert(cur == (*truth_table_length));

  return truth_table;
}

/* Get the vpack_net_num of all the input pins of a LUT physical pb */
void get_lut_logical_block_input_pin_vpack_net_num(t_logical_block* lut_logical_block,
                                                   int* num_lut_pin, int** lut_pin_net) {
  int ipin;

  /* Check */ 
  assert (NULL == lut_logical_block->model->inputs[0].next);
  (*num_lut_pin) = lut_logical_block->model->inputs[0].size;  
 
  /* Allocate */
  (*lut_pin_net) = (int*) my_malloc ((*num_lut_pin) * sizeof(int)); 
  /* Fill the array */
  for (ipin = 0; ipin < (*num_lut_pin); ipin++) {
    (*lut_pin_net)[ipin] = lut_logical_block->input_nets[0][ipin];
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

int get_pb_graph_node_wired_lut_logical_block_index(t_pb_graph_node* cur_pb_graph_node,
                                                    t_rr_node* op_pb_rr_graph) {
  int iport, ipin;
  int wired_lut_lb_index = OPEN;
  int num_used_lut_input_pins = 0;
  int num_used_lut_output_pins = 0;
  int temp_rr_node_index;
  int lut_output_vpack_net_num = OPEN;

  num_used_lut_output_pins = 0;
  /* Find the used output pin of this LUT and rr_node in the graph */
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      temp_rr_node_index = cur_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
      if (OPEN != op_pb_rr_graph[temp_rr_node_index].vpack_net_num) { /* TODO: Shit... I do not why the vpack_net_num is not synchronized to the net_num !!! */
        num_used_lut_output_pins++;
        lut_output_vpack_net_num = op_pb_rr_graph[temp_rr_node_index].vpack_net_num;
      }
    }
  }
  /* Make sure we only have 1 used output pin */
  /* vpr_printf(TIO_MESSAGE_INFO, "Wired LUT num_used_lut_output_pins is %d\n", num_used_lut_output_pins); */
  assert ((1 == num_used_lut_output_pins)
        && (OPEN != lut_output_vpack_net_num)); 

  num_used_lut_input_pins = 0;
  /* Find the used input pin of this LUT and rr_node in the graph */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      temp_rr_node_index = cur_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
      if (lut_output_vpack_net_num == op_pb_rr_graph[temp_rr_node_index].vpack_net_num) {
        num_used_lut_input_pins++;
      }
    }
  }
  /* Make sure we only have 1 used input pin */
  assert (1 == num_used_lut_input_pins); 

  /* vpr_printf(TIO_MESSAGE_INFO, "Wired LUT output vpack_net_num is %d\n", lut_output_vpack_net_num); */
 
  /* Find the used output*/ 

  /* The logical block is the driver for this vpack_net( node_block[0] )*/
  wired_lut_lb_index = vpack_net[lut_output_vpack_net_num].node_block[0];
  assert (OPEN != wired_lut_lb_index);

  return wired_lut_lb_index;
}



/* Adapt the truth from the actual connection from the input nets of a LUT,
 */
char** assign_post_routing_wired_lut_truth_table(t_logical_block* wired_lut_logical_block,
                                                 int lut_size, int* lut_pin_vpack_net_num,
                                                 int* truth_table_length) {
  int inet, iport;
  char** tt = (char**) my_malloc(sizeof(char*));
  int num_lut_output_ports;
  int* num_lut_output_pins;
  int** lut_output_vpack_net_num;

  /* The output of this mapped block is the wires routed through this LUT */
  /* Find the vpack_net_num of the output of the lut_logical_block */
  get_logical_block_output_vpack_net_num(wired_lut_logical_block, 
                                         &num_lut_output_ports, 
                                         &num_lut_output_pins, 
                                         &lut_output_vpack_net_num);

  /* Check */
  assert ( 1 == num_lut_output_ports);
  assert ( 1 == num_lut_output_pins[0]);
  assert ( OPEN != lut_output_vpack_net_num[0][0]);

  /* truth_table_length will be always 1*/
  (*truth_table_length) = 1;

  /* Malloc */
  tt[0] = (char*)my_malloc((lut_size + 3) * sizeof(char));
  /* Fill the truth table !!! */
  for (inet = 0; inet < lut_size; inet++) {
    /* Find the vpack_num in the lut_input_pin, we fix it to be 1 */
    if (lut_output_vpack_net_num[0][0] == lut_pin_vpack_net_num[inet]) {
      tt[0][inet] = '1'; 
    } else {
    /* Otherwise it should be don't care */
      tt[0][inet] = '-'; 
    }
  }
  memcpy(tt[0] + lut_size, " 1", 3);

  /* Free */
  my_free(num_lut_output_pins);
  for (iport = 0; iport < num_lut_output_ports; iport++) {
    my_free(lut_output_vpack_net_num);
  }
 
  return tt;
}


/* Provide the truth table of a mapped logical block 
 * 1. Reorgainze the truth table to be consistent with the mapped nets of a LUT
 * 2. Allocate the truth table in a clean char array and return
 */
char** assign_post_routing_lut_truth_table(t_logical_block* mapped_logical_block,
                                           int lut_size, int* lut_pin_vpack_net_num,
                                           int* truth_table_length) {
  char** truth_table = NULL;
  t_linked_vptr* head = NULL;
  int cur = 0;
  int inet, jnet;
  int* lut_to_lb_net_mapping = NULL;
  int num_lb_pin = 0;
  int* lb_pin_vpack_net_num = NULL;
  int lb_truth_table_size = 0;

  if (NULL == mapped_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid mapped_logical_block!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Allocate */
  lut_to_lb_net_mapping = (int*) my_malloc (sizeof(int) * lut_size);
  /* Find nets mapped to a logical block */
  get_lut_logical_block_input_pin_vpack_net_num(mapped_logical_block,
                                                &num_lb_pin, &lb_pin_vpack_net_num);
  /* Create a pin-to-pin net_num mapping */
  for (inet = 0; inet < lut_size; inet++) {
    lut_to_lb_net_mapping[inet] = OPEN;
    /* Bypass open nets */
    if (OPEN  == lut_pin_vpack_net_num[inet]) {
      continue;
    }
    assert (OPEN  != lut_pin_vpack_net_num[inet]);
    /* Find the position (offset) of each vpack_net_num in lb_pins */
    for (jnet = 0; jnet < num_lb_pin; jnet++) {
      if (lut_pin_vpack_net_num[inet] == lb_pin_vpack_net_num[jnet]) {
        lut_to_lb_net_mapping[inet] = jnet; 
        break;
      }  
    } 
    /* Not neccesary to find a one, some luts just share part of their pins  */ 
  } 

  /* Initialization */
  (*truth_table_length) = 0;
  /* Count the lines of truth table*/
  head = mapped_logical_block->truth_table;
  while(head) {
    (*truth_table_length)++;
    head = head->next;
  }
  /* Allocate truth_tables */
  truth_table = (char**)my_malloc(sizeof(char*)*(*truth_table_length));
  /* Fill truth_tables*/
  cur = 0;
  head = mapped_logical_block->truth_table;
  while(head) {
    /* Handle the truth table pin remapping */
    truth_table[cur] = (char*) my_malloc((lut_size + 3) * sizeof(char));
    /* Initialize */
    lb_truth_table_size = strlen((char*)(head->data_vptr));
    memcpy(truth_table[cur] + lut_size, (char*)(head->data_vptr) + lb_truth_table_size - 2, 3);
    /* Add */
    for (inet = 0; inet < lut_size; inet++) {
      /* Open net implies a don't care, or some nets are not in the list  */
      if ((OPEN  == lut_pin_vpack_net_num[inet]) 
        || (OPEN == lut_to_lb_net_mapping[inet])) {
        truth_table[cur][inet] = '-';
        continue;
      }
      /* Find the desired truth table bit */
      truth_table[cur][inet] = ((char*)(head->data_vptr))[lut_to_lb_net_mapping[inet]];
    }
    
    head = head->next;
    cur++;
  }
  assert(cur == (*truth_table_length));

  return truth_table;
}


/* Get initial value of a Latch/FF output*/
int get_ff_output_init_val(t_logical_block* ff_logical_block) {
  assert((0 == ff_logical_block->init_val)||(1 == ff_logical_block->init_val));  

  return ff_logical_block->init_val;
}

/* Get initial value of a mapped  LUT output*/
int get_lut_output_init_val(t_logical_block* lut_logical_block) {
  int i;
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int truth_table_length = 0;
  char** truth_table = NULL;
  int lut_size = 0;
  int input_net_index = OPEN;
  int* input_init_val = NULL;
  int init_path_id = 0;
  int output_init_val = 0;

  t_spice_model* lut_spice_model = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == lut_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid LUT logical block!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get SPICE model */
  assert((NULL != lut_logical_block->pb)
        && ( NULL != lut_logical_block->pb->pb_graph_node)
        && ( NULL != lut_logical_block->pb->pb_graph_node->pb_type));
  lut_spice_model = lut_logical_block->pb->pb_graph_node->pb_type->parent_mode->parent_pb_type->spice_model;

  assert(SPICE_MODEL_LUT == lut_spice_model->type);

  sram_ports = find_spice_model_ports(lut_spice_model, SPICE_MODEL_PORT_SRAM, 
                                      &num_sram_port, TRUE);
  assert(1 == num_sram_port);

  /* Get the truth table */
  truth_table = assign_lut_truth_table(lut_logical_block, &truth_table_length); 
  lut_size = lut_logical_block->used_input_pins;
  assert(!(0 > lut_size));
  /* Special for LUT_size = 0 */
  if (0 == lut_size) {
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 
                                       1, sram_ports[0]->default_val);
    /* This is constant generator, SRAM bits should be the same */
    output_init_val = sram_bits[0];
    for (i = 0; i < (int)pow(2.,(double)lut_size); i++) { 
      assert(sram_bits[i] == output_init_val);
    } 
  } else { 
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table,
                                       lut_size, sram_ports[0]->default_val);

    assert(1 == lut_logical_block->pb->pb_graph_node->num_input_ports);
    assert(1 == lut_logical_block->pb->pb_graph_node->num_output_ports);
    /* Get the initial path id */
    input_init_val = (int*)my_malloc(sizeof(int)*lut_size);
    for (i = 0; i < lut_size; i++) {
      input_net_index = lut_logical_block->input_nets[0][i]; 
      input_init_val[i] = vpack_net[input_net_index].spice_net_info->init_val;
    } 

    init_path_id = determine_lut_path_id(lut_size, input_init_val);
    /* Check */  
    assert((!(0 > init_path_id))&&(init_path_id < (int)pow(2.,(double)lut_size)));
    output_init_val = sram_bits[init_path_id]; 
  }
   
  /*Free*/
  for (i = 0; i < truth_table_length; i++) {
    free(truth_table[i]);
  }
  free(truth_table);
  my_free(sram_bits);

  return output_init_val;
}

/* Deteremine the initial value of an output of a logical block 
 * The logical block could be a LUT, a memory block or a multiplier 
 */
int get_logical_block_output_init_val(t_logical_block* cur_logical_block) {
  int output_init_val = 0;
  t_spice_model* cur_spice_model = NULL;

  /* Get the spice_model of current logical_block */
  assert((NULL != cur_logical_block->pb)
        && ( NULL != cur_logical_block->pb->pb_graph_node)
        && ( NULL != cur_logical_block->pb->pb_graph_node->pb_type));
  cur_spice_model = cur_logical_block->pb->pb_graph_node->pb_type->parent_mode->parent_pb_type->spice_model;

  /* Switch to specific cases*/
  switch (cur_spice_model->type) {
  case SPICE_MODEL_LUT:
    /* Determine the initial value from LUT inputs */
    output_init_val = get_lut_output_init_val(cur_logical_block);
    break;
  case SPICE_MODEL_HARDLOGIC:
    /* We have no information, give a default 0 now... 
     * TODO: find a smarter way!
     */
    output_init_val = get_ff_output_init_val(cur_logical_block);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SPICE MODEL (name=%s) in determining the initial output value of logical block(name=%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name, cur_logical_block->name);
    exit(1); 
  }
  
  return output_init_val;
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
                         enum e_sram_orgz cur_sram_orgz_type,
                         int grid_nx, int grid_ny) {
  int i;
  t_llist* temp = NULL;

  assert(NULL != cur_sram_orgz_info);

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
  for (i = 0; i < grid_nx; i++) {
    my_free(cur_sram_orgz_info->grid_reserved_conf_bits[i]);
  }
  my_free(cur_sram_orgz_info->grid_reserved_conf_bits);

  for (i = 0; i < grid_nx; i++) {
    my_free(cur_sram_orgz_info->grid_conf_bits_lsb[i]);
  }
  my_free(cur_sram_orgz_info->grid_conf_bits_lsb);

  for (i = 0; i < grid_nx; i++) {
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
  spice_mux_arch->structure = spice_model->design_tech_info.structure;
  spice_mux_arch->num_input = mux_size;
  /* For different structure */
  switch (spice_model->design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    spice_mux_arch->num_level = determine_tree_mux_level(spice_mux_arch->num_input);
    spice_mux_arch->num_input_basis = 2;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = tree_mux_last_level_input_num(spice_mux_arch->num_level, mux_size);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    spice_mux_arch->num_level = 1;
    spice_mux_arch->num_input_basis = mux_size;
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = mux_size;
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    /* Handle speical case: input size is 2 */
    if (2 == mux_size) { 
      spice_mux_arch->num_level = 1;
    } else {
      spice_mux_arch->num_level = spice_model->design_tech_info.mux_num_level;
    }
    spice_mux_arch->num_input_basis = determine_num_input_basis_multilevel_mux(mux_size, spice_mux_arch->num_level);
    /* Determine the level and index of per MUX inputs*/
    spice_mux_arch->num_input_last_level = multilevel_mux_last_level_input_num(spice_mux_arch->num_level, spice_mux_arch->num_input_basis, mux_size);
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
    cur = spice_mux_arch->num_input_last_level/spice_mux_arch->num_input_basis;
    /* Start from the input ports that are not occupied by the last level
     * last level has (cur) outputs
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
    cur = i+1;
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
      if (0 == node->fan_in) {
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
      if (0 == node->fan_in) {
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
  if (NULL != cur_pb_type->spice_model_name) {
    /* What annoys me is VPR create a sub pb_type for each lut which suppose to be a leaf node
     * This may bring software convience but ruins SPICE modeling
     */
    assert(NULL != cur_pb_type->spice_model);
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

/* Find the interconnection type of pb_graph_pin edges*/
enum e_interconnect find_pb_graph_pin_in_edges_interc_type(t_pb_graph_pin pb_graph_pin) {
  enum e_interconnect interc_type;
  int def_interc_type = 0;
  int iedge;

  for (iedge = 0; iedge < pb_graph_pin.num_input_edges; iedge++) {
    /* Make sure all edges are legal: 1 input_pin, 1 output_pin*/
    check_pb_graph_edge(*(pb_graph_pin.input_edges[iedge]));
    /* Make sure all the edges interconnect type is the same*/
    if (0 == def_interc_type) {
      interc_type = pb_graph_pin.input_edges[iedge]->interconnect->type;
    } else if (interc_type != pb_graph_pin.input_edges[iedge]->interconnect->type) {
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d])Interconnection type are not same for port(%s),pin(%d).\n",
                 __FILE__, __LINE__, pb_graph_pin.port->name,pb_graph_pin.pin_number);
      exit(1);
    }
  }

  return interc_type;  
}

/* Find the interconnection type of pb_graph_pin edges*/
t_spice_model* find_pb_graph_pin_in_edges_interc_spice_model(t_pb_graph_pin pb_graph_pin) {
  t_spice_model* interc_spice_model;
  int def_interc_model = 0;
  int iedge;

  for (iedge = 0; iedge < pb_graph_pin.num_input_edges; iedge++) {
    /* Make sure all edges are legal: 1 input_pin, 1 output_pin*/
    check_pb_graph_edge(*(pb_graph_pin.input_edges[iedge]));
    /* Make sure all the edges interconnect type is the same*/
    if (0 == def_interc_model) {
      interc_spice_model= pb_graph_pin.input_edges[iedge]->interconnect->spice_model;
    } else if (interc_spice_model != pb_graph_pin.input_edges[iedge]->interconnect->spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d])Interconnection spice_model are not same for port(%s),pin(%d).\n",
                 __FILE__, __LINE__, pb_graph_pin.port->name,pb_graph_pin.pin_number);
      exit(1);
    }
  }

  return interc_spice_model;  
}

int find_path_id_between_pb_rr_nodes(t_rr_node* local_rr_graph,
                                     int src_node,
                                     int des_node) {
  int path_id = -1;
  int prev_edge = -1;
  int path_count = 0;
  int iedge;
  t_interconnect* cur_interc = NULL;

  /* Check */
  assert(NULL != local_rr_graph);
  assert((0 == src_node)||(0 < src_node));
  assert((0 == des_node)||(0 < des_node));

  prev_edge = local_rr_graph[des_node].prev_edge;
  check_pb_graph_edge(*(local_rr_graph[src_node].pb_graph_pin->output_edges[prev_edge]));
  assert(local_rr_graph[src_node].pb_graph_pin->output_edges[prev_edge]->output_pins[0] == local_rr_graph[des_node].pb_graph_pin);
 
  cur_interc = local_rr_graph[src_node].pb_graph_pin->output_edges[prev_edge]->interconnect;
  /* Search des_node input edges */ 
  for (iedge = 0; iedge < local_rr_graph[des_node].pb_graph_pin->num_input_edges; iedge++) {
    if (local_rr_graph[des_node].pb_graph_pin->input_edges[iedge]->input_pins[0] 
        == local_rr_graph[src_node].pb_graph_pin) {
      /* Strict check */
      assert(local_rr_graph[src_node].pb_graph_pin->output_edges[prev_edge]
              == local_rr_graph[des_node].pb_graph_pin->input_edges[iedge]);
      path_id = path_count;
      break;
    }
    if (cur_interc == local_rr_graph[des_node].pb_graph_pin->input_edges[iedge]->interconnect) {
      path_count++;
    }
  }

  return path_id; 
}

/* Return a child_pb if it is mapped.*/
t_pb* get_child_pb_for_phy_pb_graph_node(t_pb* cur_pb, int ipb, int jpb) {
  t_pb* child_pb = NULL;
  
  /* TODO: more check ? */

  if (NULL == cur_pb) {
    return NULL;
  }

  if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
    child_pb = &(cur_pb->child_pbs[ipb][jpb]);
  }
  
  return child_pb;
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

/* Return the child_pb of a LUT pb
 * Because the mapping information is stored in the child_pb!!!
 */
t_pb* get_lut_child_pb(t_pb* cur_lut_pb,
                       int mode_index) {

  assert(SPICE_MODEL_LUT == cur_lut_pb->pb_graph_node->pb_type->spice_model->type);

  assert(1 == cur_lut_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children);
  assert(1 == cur_lut_pb->pb_graph_node->pb_type->num_pb);

  return (&(cur_lut_pb->child_pbs[0][0])); 
}

/* Return the child_pb of a hardlogic  pb
 * Because the mapping information is stored in the child_pb!!!
 */
t_pb* get_hardlogic_child_pb(t_pb* cur_hardlogic_pb,
                             int mode_index) {

  assert(SPICE_MODEL_HARDLOGIC == cur_hardlogic_pb->pb_graph_node->pb_type->spice_model->type);

  assert(1 == cur_hardlogic_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children);
  assert(1 == cur_hardlogic_pb->pb_graph_node->pb_type->num_pb);

  return (&(cur_hardlogic_pb->child_pbs[0][0])); 
}


int get_grid_pin_height(int grid_x, int grid_y, int pin_index) {
  int pin_height;
  t_type_ptr grid_type = NULL;

  /* Get type */
  grid_type = grid[grid_x][grid_y].type;

  /* Return if this is an empty type */
  if ((NULL == grid_type)
     ||(EMPTY_TYPE == grid_type)) {
    pin_height = 0;
    return pin_height;
  }

  /* Check if the pin index is in the range */
  assert ( ((0 == pin_index) || (0 < pin_index))
          &&(pin_index < grid_type->num_pins) );

  /* Find the pin_height */
  pin_height = grid_type->pin_height[pin_index];
  
  return pin_height;
}

int get_grid_pin_side(int grid_x, int grid_y, int pin_index) {
  int pin_height, side, pin_side;
  t_type_ptr grid_type = NULL;

  /* Get type */
  grid_type = grid[grid_x][grid_y].type;

  /* Return if this is an empty type */
  if ((NULL == grid_type)
     ||(EMPTY_TYPE == grid_type)) {
    return -1;
  }

  /* Check if the pin index is in the range */
  assert ( ((0 == pin_index) || (0 < pin_index))
          &&(pin_index < grid_type->num_pins) );

  /* Find the pin_height */
  pin_height = get_grid_pin_height(grid_x, grid_y, pin_index);

  pin_side = -1;
  for (side = 0; side < 4; side++) {
    /* Bypass corner cases */
    /* Pin can only locate on BOTTOM side, when grid is on TOP border */
    if ((ny == grid_y)&&(2 != side)) {
      continue;
    }
    /* Pin can only locate on LEFT side, when grid is on RIGHT border */
    if ((nx == grid_x)&&(3 != side)) {
      continue;
    }
    /* Pin can only locate on the TOP side, when grid is on BOTTOM border */
    if ((0 == grid_y)&&(0 != side)) {
      continue;
    }
    /* Pin can only locate on the RIGHT side, when grid is on LEFT border */
    if ((0 == grid_x)&&(1 != side)) {
      continue;
    }
    if (1 == grid_type->pinloc[pin_height][side][pin_index]) {
      if (-1 != pin_side) { 
        vpr_printf(TIO_MESSAGE_ERROR, "(%s, [LINE%d]) Duplicated pin(index:%d) on two sides: %s and %s of type (name=%s)!\n",
                   __FILE__, __LINE__, 
                   pin_index, 
                   convert_side_index_to_string(pin_side),
                   convert_side_index_to_string(side),
                   grid_type->name);
        exit(1);
      }
      pin_side = side;
    }
  }
  
  return pin_side;
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
       ||(1 <  grid[cb_x][cb_y].type->height)) {
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

/* Get the vpack_net_num of all the input pins of a LUT physical pb */
void get_mapped_lut_pb_input_pin_vpack_net_num(t_pb_graph_node* lut_pb_graph_node,
                                               t_rr_node* pb_rr_graph,
                                               int* num_lut_pin, int** lut_pin_net) {
 
  int ipin, inode;

  /* Check */ 
  assert (1 == lut_pb_graph_node->num_input_ports);
  (*num_lut_pin) = lut_pb_graph_node->num_input_pins[0];  
 
  /* Allocate */
  (*lut_pin_net) = (int*) my_malloc ((*num_lut_pin) * sizeof(int)); 
  /* Fill the array */
  for (ipin = 0; ipin < (*num_lut_pin); ipin++) {
    inode = lut_pb_graph_node->input_pins[0][ipin].pin_count_in_cluster;
    (*lut_pin_net)[ipin] = pb_rr_graph[inode].vpack_net_num;
  }

  return;
}

/* Recursively find all the global ports in the spice_model / sub spice_model 
 */
void rec_stats_spice_model_global_ports(t_spice_model* cur_spice_model,
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

/* Identify if this child_pb is actually used for wiring!!! */
boolean is_pb_used_for_wiring(t_pb_graph_node* cur_pb_graph_node,
                              t_pb_type* cur_pb_type,
                              t_rr_node* pb_rr_graph) {
  boolean is_used = FALSE;
  int node_index;
  int port_index = 0;
  int iport, ipin;

  for (iport = 0; iport < cur_pb_type->num_ports && !is_used; iport++) {
    if (OUT_PORT == cur_pb_type->ports[iport].type) {
      for (ipin = 0; ipin < cur_pb_type->ports[iport].num_pins; ipin++) {
        node_index = cur_pb_graph_node->output_pins[port_index][ipin].pin_count_in_cluster;
        if ((OPEN != pb_rr_graph[node_index].net_num) 
          || (OPEN != pb_rr_graph[node_index].vpack_net_num)) {
          return TRUE;
        }
      }
      port_index++;
    }
  }

  return is_used; 
} 


/* Identify if this is an unallocated pb that is used as a wired LUT */
boolean is_pb_wired_lut(t_pb_graph_node* cur_pb_graph_node,
                        t_pb_type* cur_pb_type,
                        t_rr_node* pb_rr_graph) {
  boolean is_used = FALSE;
  
  is_used = is_pb_used_for_wiring(cur_pb_graph_node,
                                  cur_pb_type,
                                  pb_rr_graph);
  /* Return TRUE if this block is not used and it is a LUT ! */
  if ((TRUE == is_used) 
     && (LUT_CLASS == cur_pb_type->class_type)) {
    return TRUE;
  }

  return FALSE;
} 

