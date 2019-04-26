#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"
#include "read_opt_types.h"
#include "read_opt.h"

/* External function */
void my_free(void* ptr);
char** fpga_spice_strtok(char* str, 
                         char* delims, 
                         int* len);
int my_strcmp(char* str1, char* str2);


/**
 * Read the integer in option list 
 * Do simple check and convert it from char to 
 * integer
 */
int process_int_arg(INP char* arg) {
  /*Check if the pointor of arg is NULL */
  if (NULL == arg) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "Error: Arg is NULL when processing integer!\n");
    exit(1);
  } 

  return atoi(arg);
}

/**
 * Read the float in option list 
 * Do simple check and convert it from char to 
 * float
 */
float process_float_arg(INP char* arg) {
  /*Check if the pointor of arg is NULL */
  if (NULL == arg) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "Error: Arg is NULL when processing float!\n");
    exit(1);
  }

  return atof(arg);
}

/**
 * Process the argument by comparing 
 * Store the options in struct
 */
boolean process_arg_opt(INP char** argv,
                        INOUTP int* iarg,
                        INP char* curarg,
                        t_opt_info* cur) {
  int itok = 0;
  int num_tokens = 0;
  char** token = NULL;

  while (0 != my_strcmp(LAST_OPT_TAG, cur->tag)) { 
    /* Tokenize the opt_name*/
    token = fpga_spice_strtok(cur->name, ",", &num_tokens);  
    /*Process Match Arguments*/
    for (itok = 0; itok < num_tokens; itok++) { 
      if (0 == my_strcmp(curarg, token[itok])) {
        /* Check the defined flag if yes, return with error! */
        if (OPT_DEF == cur->opt_def) {
          vpr_printf(TIO_MESSAGE_ERROR, 
                     "Intend to redefine the option(%s) with (%s)!\n",
                     cur->name, token[itok]);
          return FALSE;
        }
        cur->opt_def = OPT_DEF;
        /*A value is stored in next argument*/
        if (OPT_WITHVAL == cur->with_val) {
          *(iarg) += 1;
          cur->val = my_strdup((argv[*iarg]));    
          return TRUE;
        } else if (OPT_NONVAL == cur->with_val) {
          /*Do not need next argument, return*/
          return TRUE;
        } else {
          vpr_printf(TIO_MESSAGE_ERROR, 
                     "(FILE:%s, [LINE%d]) Unknown type of Option with_Val! Abort.\n",
                     __FILE__, __LINE__);
          return FALSE;
        }
      }
    }
    cur++;
    /* Free */
    for (itok = 0; itok < num_tokens; itok++) { 
      my_free(token[itok]);
    }
    my_free(token);
  }
 
  return FALSE;
}

char* convert_option_mandatory_to_str(enum opt_manda cur) {
  switch (cur) {
  case OPT_REQ:
    return "Required";
  case OPT_OPT:
    return "Optional";
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(FILE:%s, [LINE%d]) Unknown type of Option Mandatory! Abort.\n",
               __FILE__, __LINE__);
    return NULL;
  }
}

void print_opt_info_help_desk(t_opt_info* cur_opt_info) {
  int max_str_len = -1;
  int offset;
  t_opt_info* cur = cur_opt_info;
  char* str_fixed_len = NULL;
  char* name_tag = "Option Names"; 
  char str_end = '\0'; 

  /* Get the maximum string length of options
   * We can align to the longest string when outputing the help desk
   */
  while (0 != my_strcmp(LAST_OPT_TAG, cur->tag)) { 
    if ( (-1 == max_str_len) 
      || (max_str_len < strlen(cur->name)) ) {
      max_str_len = strlen(cur->name) + 1;
    }
    cur++;
  }
  /* Minimum size is 5 */
  if (max_str_len < strlen(name_tag) + 1) {
    max_str_len = strlen(name_tag) + 1;
  }
  /* Malloc */
  str_fixed_len = (char*)my_calloc(max_str_len, sizeof(char));

  vpr_printf(TIO_MESSAGE_INFO, "Help Desk:\n");
  memset(str_fixed_len, ' ', max_str_len);
  strcpy(str_fixed_len, name_tag);
  str_fixed_len[strlen(name_tag)] = ' ';
  str_fixed_len[max_str_len] = str_end;
  vpr_printf(TIO_MESSAGE_INFO, "%s  Status     Description\n", str_fixed_len);
  cur = cur_opt_info;
  while (0 != my_strcmp(LAST_OPT_TAG, cur->tag)) { 
    memset(str_fixed_len, ' ', max_str_len);
    strcpy(str_fixed_len, cur->name);
    str_fixed_len[strlen(cur->name)] = ' ';
    str_fixed_len[max_str_len] = str_end;
    vpr_printf(TIO_MESSAGE_INFO, "%s  ", str_fixed_len);
    vpr_printf(TIO_MESSAGE_INFO, "%s  ", convert_option_mandatory_to_str(cur->mandatory));
    vpr_printf(TIO_MESSAGE_INFO, "%s", cur->description);
    vpr_printf(TIO_MESSAGE_INFO, "\n");
    cur++;
  }
  vpr_printf(TIO_MESSAGE_INFO, "\n");

  /* Free */
  my_free(str_fixed_len);

  return;
}


boolean read_options(INP int argc,
                     INP char **argv,
                     INOUTP t_opt_info* cur_opt_info) {
  int iarg;
  char* curarg = NULL;
  boolean arg_processed = FALSE;
  t_opt_info* cur = cur_opt_info;
  

  vpr_printf(TIO_MESSAGE_INFO,
             "Processing Options...\n");

  /*Start from argv[1], the 1st argv is programme name*/
  for (iarg = 1; iarg < argc;) {
    curarg = argv[iarg]; 
    /*Process the option start with hyphone*/
    if (0 == strncmp("-", curarg, 1)) {
      arg_processed = process_arg_opt(argv, &iarg, curarg, cur_opt_info); 
      if (FALSE == arg_processed) {
        vpr_printf(TIO_MESSAGE_WARNING,
                   "Warning: Unknown Option(%s) detected!\n",
                   curarg);
        print_opt_info_help_desk(cur_opt_info);
        return FALSE;
      }
      iarg += 1; /*Move on to the next argument*/
    } else {
      iarg++;
    }
  }

  /* Search the command help */ 
  if (TRUE == is_opt_set(cur_opt_info, LAST_OPT_TAG, FALSE)) {
    print_opt_info_help_desk(cur_opt_info);
    return FALSE;
  }

  /* Check if REQUIRED options are processed */
  while (0 != my_strcmp(LAST_OPT_TAG, cur->tag)) { 
    if ( (NULL == cur->val)
      && (OPT_REQ == cur->mandatory)) { 
      vpr_printf(TIO_MESSAGE_WARNING,
                 "Warning: Required Option(%s) is missing!\n",
                 cur->name);
      print_opt_info_help_desk(cur_opt_info);
      return FALSE;
    }
    cur++;
  }

  /* Confirm options */
  /*
  show_opt_list(cur_opt_info);
  */

  return TRUE;
}


/**
 * Show the options in opt_list after process.
 * Only for debug use
 */
int show_opt_list(t_opt_info* cur) {

  vpr_printf(TIO_MESSAGE_INFO, 
             "List Options:\n");
  while (0 != my_strcmp(LAST_OPT_TAG, cur->tag)) { 
    vpr_printf(TIO_MESSAGE_INFO, 
               "Name=%s, Value=%s.\n", 
               cur->name, cur->val);
    cur++;
  }

  return 1;
}

boolean is_opt_set(t_opt_info* opts, char* opt_name, boolean default_val) {
  while (0 != my_strcmp(LAST_OPT_TAG, opts->tag)) {
    if ( 0 != my_strcmp(opts->tag, opt_name)) {
      opts++;
      continue;
    }
    if (OPT_DEF == opts->opt_def) {
      return TRUE;
    } else {
      return default_val;
    }
    opts++;
  }

  return default_val;
}

char* get_opt_val(t_opt_info* opts, char* opt_name) {
  while (0 != my_strcmp(LAST_OPT_TAG, opts->tag)) {
    if ( 0 != my_strcmp(opts->tag, opt_name)) {
      opts++;
      continue;
    }
    if (OPT_WITHVAL != opts->with_val) {
      vpr_printf(TIO_MESSAGE_INFO, 
                 "Try to get the val of an option(%s) which is defined to be without_val!\n", 
                 opt_name);
    }
    if (NULL == opts->val) {
      return NULL;
    } else {
      return my_strdup(opts->val);
    }
    opts++;
  }

  return NULL;
}

int get_opt_int_val(t_opt_info* opts, char* opt_name, int default_val) {
  char* temp = get_opt_val(opts, opt_name);
  int ret = default_val;

  if (NULL != temp) {
    ret = process_int_arg(temp);
  }
  /* Free */
  my_free(temp);

  return ret;
}

float get_opt_float_val(t_opt_info* opts, char* opt_name, float default_val) {
  char* temp = get_opt_val(opts, opt_name);
  float ret = default_val;

  if (NULL != temp) {
    ret = process_float_arg(temp);
  }
  /* Free */
  my_free(temp);

  return ret;
}


