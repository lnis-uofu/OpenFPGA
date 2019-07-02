#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

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
#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

/* Include APIs */

#include <readline/readline.h>
#include <readline/history.h>

int my_strcmp(char* str1, char* str2) {
  if ((NULL == str1) && (NULL == str2)) {
    return 0;
  } else if ((NULL == str1) || (NULL == str2)) {
    return 1;
  } else {
    return strcmp(str1, str2);
  }
}

void shell_print_usage(t_shell_env* env) {
  t_cmd_category* cur_category = env->cmd_category;

  /* Output usage by command categories */
  while (LAST_CMD_CATEGORY != cur_category->name) {
    vpr_printf(TIO_MESSAGE_INFO, 
               "%s:\n", cur_category->label);
    t_shell_cmd* cur_cmd = env->cmd;
    int cnt = 0;
    while (0 != my_strcmp(LAST_CMD_NAME, cur_cmd->name)) {
      if (cur_category->name != cur_cmd->category) {
        /* Go to next command*/
        cur_cmd++;
        continue;
      }
      /* Print command name */
      vpr_printf(TIO_MESSAGE_INFO, 
                 "%s",
                 cur_cmd->name);
      cnt++;
      if (4 == cnt) { 
        vpr_printf(TIO_MESSAGE_INFO, "\n");
        cnt = 0;
      } else {
        vpr_printf(TIO_MESSAGE_INFO, "\t");
      }
      /* Go to next command*/
      cur_cmd++;
    }
    vpr_printf(TIO_MESSAGE_INFO, "\n\n");
    /* Go to next */
    cur_category++;
  } 

  return;
}

/* Identify the command to launch */
void process_shell_command(char* line, t_shell_env* env) {
  int itok;
  int num_tokens = 0;
  char** token = NULL;
  t_shell_cmd* cur_cmd = env->cmd;

  /* Tokenize the line */
  token = fpga_spice_strtok(line, " ", &num_tokens);  
  
  /* Search the shell command list and execute */ 
  while (0 != my_strcmp(LAST_CMD_NAME, cur_cmd->name)) {
    if (0 == my_strcmp(cur_cmd->name, token[0])) {
      /* Read options of read_blif */
      if (FALSE == read_options(num_tokens, token, cur_cmd->opts)) {
        return;
      }
      /* Execute setup_vpr engine*/
      cur_cmd->execute(env, cur_cmd->opts);
      /* Return here */
      return;
    }
    /* Go to next command*/
    cur_cmd++;
  } 

  /* Invalid command */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Invalid command!\n");
  shell_print_usage(env); 

  /* Free */
  for (itok = 0; itok < num_tokens; itok++) { 
    my_free(token[itok]);
  }
  my_free(token);

  return;
}


