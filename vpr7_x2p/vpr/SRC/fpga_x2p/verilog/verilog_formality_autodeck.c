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

static 
void formality_include_user_defined_verilog_netlists(FILE* fp,
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
      fprintf(fp, "%s ", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

void write_formality_script (t_syn_verilog_opts fpga_verilog_opts,
                             char* fm_dir_formatted,
                             char* src_dir_formatted,
                             char* chomped_circuit_name, 
                             t_spice spice){
  int iblock;
  char* formality_script_file_name = NULL;
  char* benchmark_path = NULL;
  char* original_output_name = NULL;
/*  int output_length; */
/*  int pos; */
  FILE* fp = NULL;

  if(TRUE == fpga_verilog_opts.print_autocheck_top_testbench){
    benchmark_path = fpga_verilog_opts.reference_verilog_benchmark_file;
  } else {
    benchmark_path = "Insert verilog benchmark path";
  }

  formality_script_file_name = my_strcat(fm_dir_formatted, my_strcat(chomped_circuit_name, formality_script_name_postfix));
  fp = fopen(formality_script_file_name, "w");
    if (NULL == fp) {
      vpr_printf(TIO_MESSAGE_ERROR,
                "(FILE:%s,LINE[%d])Failure in create formality script %s",
                __FILE__, __LINE__, formality_script_file_name); 
      exit(1);
    } 

  /* Load Verilog benchmark as reference */
  fprintf(fp, "read_verilog -container r -libname WORK -05 { %s }\n", benchmark_path);
  /* Set reference top */
  fprintf(fp, "set_top r:/WORK/%s\n", chomped_circuit_name);
  /* Load generated verilog as implemnetation */
  fprintf(fp, "read_verilog -container i -libname WORK -05 { ");
  fprintf(fp, "%s%s%s ",  src_dir_formatted, 
              chomped_circuit_name, 
              verilog_top_postfix);
  fprintf(fp, "%s%s%s ",  src_dir_formatted, 
              chomped_circuit_name, 
              formal_verification_verilog_file_postfix);
  init_include_user_defined_verilog_netlists(spice);
  formality_include_user_defined_verilog_netlists(fp, spice);
  fprintf(fp, "%s%s%s ",  src_dir_formatted, 
              default_rr_dir_name, 
              routing_verilog_file_name);
  fprintf(fp, "%s%s%s ",  src_dir_formatted, 
              default_lb_dir_name, 
              logic_block_verilog_file_name);
  fprintf(fp, "%s%s%s ",  src_dir_formatted, 
              default_submodule_dir_name, 
              submodule_verilog_file_name);
  fprintf(fp, "}\n");
  /* Set implementation top */
  fprintf(fp, "set_top i:/WORK/%s\n", my_strcat(chomped_circuit_name, formal_verification_top_postfix));
  /* Run matching */
  fprintf(fp, "match\n");
  /* Add manual matching for the outputs */
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    original_output_name = NULL;
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type){
        /* output_length = strlen(logical_block[iblock].name); */
        original_output_name = logical_block[iblock].name + 4;
        /* printf("%s", original_output_name); */
        fprintf(fp, "set_user_match r:/WORK/%s/%s i:/WORK/%s/%s[0] -type port -noninverted\n", 
                chomped_circuit_name,
                original_output_name, 
                my_strcat(chomped_circuit_name, formal_verification_top_postfix),
        my_strcat(logical_block[iblock].name, formal_verification_top_module_port_postfix));
      }
    }
  }
  /* Run verification */
  fprintf(fp, "verify\n");
  /* Script END */
  fclose(fp);

  return;
}
