// Formality runsim
// Need to declare formality_script_name_postfix = "formality_script.tcl";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"

/* Include FPGA-SPICE utils */
#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_globals.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "verilog_utils.h"

#include "verilog_include_netlists.h"

static 
void include_netlists_include_user_defined_verilog_netlists(FILE* fp,
                                                    		t_spice spice) {
  int i;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid File Handler!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    if (0 == spice.include_netlists[i].included) {
      assert(NULL != spice.include_netlists[i].path);
      fprintf(fp, "`include \"%s\"\n", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

void write_include_netlists (char* src_dir_formatted,
                            char* chomped_circuit_name, 
							t_spice spice){

	char* include_netlists_file_name = NULL;
/*	int output_length; */
/*	int pos; */
	FILE* fp = NULL;

	include_netlists_file_name = my_strcat(src_dir_formatted, my_strcat(chomped_circuit_name, "_include_netlists.v"));
	fp = fopen(include_netlists_file_name, "w");
    if (NULL == fp) {
      vpr_printf(TIO_MESSAGE_ERROR,
                "(FILE:%s,LINE[%d])Failure in create formality script %s",
                __FILE__, __LINE__, include_netlists_file_name); 
      exit(1);
    } 

	/* Print the title */
	dump_verilog_file_header(fp, "Netlists Summary");

	/* Print preprocessing flags */
	verilog_include_defines_preproc_file(fp, src_dir_formatted);
	verilog_include_simulation_defines_file(fp, src_dir_formatted);


	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							chomped_circuit_name, 
							verilog_top_postfix);
	fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							chomped_circuit_name, 
							formal_verification_verilog_file_postfix);
	fprintf(fp, "  `ifdef %s\n", formal_simulation_flag);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							chomped_circuit_name, 
							random_top_testbench_verilog_file_postfix);
	fprintf(fp, "  `endif\n");
	fprintf(fp, "`elsif %s\n", initial_simulation_flag);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							chomped_circuit_name, 
							top_testbench_verilog_file_postfix);
	fprintf(fp, "`elsif %s\n", autochecked_simulation_flag);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							chomped_circuit_name, 
							autocheck_top_testbench_verilog_file_postfix);
	fprintf(fp, "`endif\n");
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							default_rr_dir_name, 
							routing_verilog_file_name);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							default_lb_dir_name, 
							logic_block_verilog_file_name);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							default_submodule_dir_name, 
							submodule_verilog_file_name);
	fprintf(fp, "`include \"%s%s%s\"\n",  src_dir_formatted, 
							default_submodule_dir_name, 
						    config_peripheral_verilog_file_name);
	init_include_user_defined_verilog_netlists(spice);
	include_netlists_include_user_defined_verilog_netlists(fp, spice);
	
	fclose(fp);

	return;
}
