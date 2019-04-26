#include <stdio.h>
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
#include "vpr_api.h"
#include "fpga_x2p_api.h"
#include "cmd_vpr_setup.h"
#include "cmd_vpr_pack.h"
#include "cmd_vpr_place_and_route.h"
#include "cmd_vpr_power.h"
#include "cmd_fpga_x2p_setup.h"
#include "cmd_fpga_spice.h"
#include "cmd_fpga_verilog.h"
#include "cmd_fpga_bitstream.h"
#include "cmd_help.h"
#include "cmd_exit.h"
#include "shell_cmds.h"
#include "shell_utils.h"
#include "mini_shell.h"

#include <readline/readline.h>
#include <readline/history.h>


t_shell_env shell_env;

char* vpr_shell_prefix = "VPR7-OpenFPGA> ";


void init_shell_env(t_shell_env* env) {
  memset(&(env->vpr_setup), 0, sizeof(t_vpr_setup));
  memset(&(env->arch), 0, sizeof(t_arch));
  env->cmd = shell_cmd;
  env->cmd_category = cmd_category;
  
  return;
}

/* Start the interactive shell */
void vpr_run_interactive_mode() {

  vpr_printf(TIO_MESSAGE_INFO, "Start interactive mode...\n");

  vpr_print_title();
  
  /* Initialize file handler */
  vpr_init_file_handler();

  /* Initialize shell_env */
  init_shell_env(&shell_env);
 
  while (1) {
    char * line = readline(vpr_shell_prefix);
    process_shell_command(line, &shell_env);
    /* Add to history */
    add_history(line);
    free (line);
  }

  return;
}

void vpr_run_script_mode(char* script_file_name) {
  FILE* fp = NULL;
  char buffer[BUFSIZE];
  char str_end = '\0'; 
  int i;

  vpr_print_title();
  
  /* Initialize file handler */
  vpr_init_file_handler();

  /* Initialize shell_env */
  init_shell_env(&shell_env);

  vpr_printf(TIO_MESSAGE_INFO, "Reading script file %s!\n", script_file_name);
 
  /* Read the file */
  fp = fopen(script_file_name, "r");
  /* Error out if the File handle is empty */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "Fail to open the script file: %s.\n",
				script_file_name);
    exit(1);
  }

  /* Read line by line */
  while (NULL != my_fgets(buffer, BUFSIZE, fp)) {
    if ((0 == strlen(buffer))
      || (0 == my_strcmp(buffer, ""))) {
      continue;
    }
    /* Chomp the \n of buffer */
    for (i = 0; i < strlen(buffer); i++) { 
      if ('\n' == buffer[i]) {
        buffer[i] = str_end;
      }
    }
    /* Treat each valid line as a command */ 
    process_shell_command(buffer, &shell_env);
  }

  /* Close file */
  fclose(fp);

  return;
}

void run_shell(int argc, char ** argv) {

  /* Parse the options and decide which interface to go */
  read_options(argc, argv, shell_opts);

  /* Interface 1: run through -i, --interactive 
   * Or with 0 arguments, we start the interactive shell 
   */
  if ( (1 == argc) 
    || (TRUE == is_opt_set(shell_opts, "i", FALSE)) ) {
    vpr_run_interactive_mode();
  } else if (TRUE == is_opt_set(shell_opts, "f", FALSE)) {
  /* Interface 2: run through -f, --file */
    char* script_file_name = get_opt_val(shell_opts, "f");
    vpr_run_script_mode(script_file_name);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "Invalid command! Launch Help desk:\n");
    print_opt_info_help_desk(shell_opts);
  }

  return;
}


