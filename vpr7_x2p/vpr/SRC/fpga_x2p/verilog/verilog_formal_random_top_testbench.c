/***********************************/
/*  Dump Synthesizable Veriolog    */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
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
#include "route_common.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_bitstream.h"

/* Include verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_pbtypes.h"
#include "verilog_decoder.h"
#include "verilog_top_netlist_utils.h"
#include "verilog_top_testbench.h"

/* Local variables */
static char* formal_random_top_tb_postfix = "_top_formal_verification_random_tb";
static char* gfpga_postfix = "_gfpga";
static char* bench_postfix = "_bench";
static char* flag_postfix = "_flag";
static char* def_clk_name = "clk";
static char* clock_input_name = NULL;

/* Local Subroutines declaration */

/******** Subroutines ***********/
static 
void dump_verilog_top_random_testbench_ports(FILE* fp,
                                           t_sram_orgz_info* cur_sram_orgz_info,
                                           char* circuit_name,
                                           t_syn_verilog_opts fpga_verilog_opts){
  int iblock, iopad_idx;
  boolean bench_as_clk = FALSE;
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
 
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  fprintf(fp, "`include \"%s\"\n", fpga_verilog_opts.reference_verilog_benchmark_file);

  fprintf(fp, "module %s%s;\n", circuit_name, formal_random_top_tb_postfix);
// Instantiate register for inputs stimulis
  fprintf(fp, "//----- Shared inputs\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_INPAD == logical_block[iblock].type) { 
        fprintf(fp, "  reg %s;\n", logical_block[iblock].name);
		if(logical_block[iblock].is_clock)
			bench_as_clk = TRUE;
      }
    }
  }
  if(FALSE == bench_as_clk)
	fprintf(fp, "  reg %s;\n", def_clk_name);
// Instantiate wire for gfpga output
  fprintf(fp, "\n//----- GFPGA outputs\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type) { 
        fprintf(fp, "  wire %s%s;\n", logical_block[iblock].name, gfpga_postfix);
      }
    }
  }
// Instantiate wire for benchmark output
  fprintf(fp, "\n`ifdef %s\n//----- Benchmark outputs\n", autochecked_simulation_flag);
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type) { 
        fprintf(fp, "  wire %s%s;\n", logical_block[iblock].name, bench_postfix);
      }
    }
  }
// Instantiate register for output comparison
  fprintf(fp, "\n//----- Output flags\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type) { 
        fprintf(fp, "  reg %s%s;\n", logical_block[iblock].name, flag_postfix);
      }
    }
  } fprintf(fp, "`endif\n");

  return;
}

static
void dump_verilog_top_random_testbench_call_benchmark(FILE* fp, 
                                                    char* reference_verilog_top_name){
  int iblock, iopad_idx;

  fprintf(fp, "`ifdef %s\n", autochecked_simulation_flag);
  fprintf(fp, "// Reference Benchmark instanciation\n");
  fprintf(fp, "  %s ref_U0(\n", reference_verilog_top_name);

  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(iblock > 0){
        fprintf(fp, ",\n");
      }
      if(VPACK_INPAD == logical_block[iblock].type){
        fprintf(fp, "        %s", logical_block[iblock].name);
      } else if(VPACK_OUTPAD == logical_block[iblock].type){
        fprintf(fp, "        %s%s", 
                logical_block[iblock].name,
                bench_postfix);
      }
    }
  }
  fprintf(fp, " );\n");
  fprintf(fp, "// End Benchmark instanciation\n`endif\n\n");

  return;
}

static 
int get_simulation_time(int num_prog_clock_cycles,
                          float prog_clock_period,
                          int num_op_clock_cycles,
                          float op_clock_period) {
  int total_time_period = 0;

  /* Take into account the prog_reset and reset cycles */
  total_time_period = ((num_prog_clock_cycles + 2) * prog_clock_period + (2 * num_op_clock_cycles * op_clock_period)) * 1000000000; // * 1000000000 is to change the unit to ns rather than second

  return total_time_period; 
}

static
void dump_verilog_timeout_and_vcd(FILE * fp,
									char* circuit_name,
									t_spice verilog,
									t_sram_orgz_info* cur_sram_orgz_info){
	int simulation_time;

	simulation_time = get_simulation_time(get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info),
										  1./verilog.spice_params.stimulate_params.prog_clock_freq,
                                          verilog.spice_params.meas_params.sim_num_clock_cycle,
                                          1./verilog.spice_params.stimulate_params.op_clock_freq);
										  
	fprintf(fp, "  // Begin Icarus requirement\n");
	fprintf(fp, "`ifdef %s\n", icarus_simulator_flag);
	fprintf(fp, "  initial begin\n");
	fprintf(fp, "    $dumpfile(%s_autochecked.vcd);\n", circuit_name);
	fprintf(fp, "    $dumpvars(1, %s%s);\n", circuit_name,
											modelsim_autocheck_testbench_module_postfix);
	fprintf(fp, "  end\n\n");
	fprintf(fp, "  initial begin\n");
	fprintf(fp, "    $display(\"Simulation start\");\n");
	fprintf(fp, "    #%i // Can be changed by the user for his need\n", simulation_time);
	fprintf(fp, "    $display(\"Simulation End: Time's up\");\n");
	fprintf(fp, "  end\n");
	fprintf(fp, "`endif\n\n");
	return;
}

static
void dump_verilog_top_random_testbench_check(FILE* fp){
  int iblock, iopad_idx;
  fprintf(fp, "  // Begin checking\n");
  fprintf(fp, "  always@(negedge %s) begin\n", clock_input_name);
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type){
        fprintf(fp, "    %s%s <= %s%s ^ %s%s ;\n", logical_block[iblock].name, 
                									flag_postfix,
                									logical_block[iblock].name,  
               										gfpga_postfix,
                									logical_block[iblock].name, 
                									bench_postfix);
      }
    }
  } 
  fprintf(fp, "  end\n\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_OUTPAD == logical_block[iblock].type){
        fprintf(fp, "  always@(posedge %s%s) begin\n", logical_block[iblock].name,
                    									flag_postfix);
        fprintf(fp, "      if(%s%s) begin\n", logical_block[iblock].name,
                    							flag_postfix);
        fprintf(fp, "        $display(\"Mismatch on %s%s\");\n", logical_block[iblock].name,
                    											gfpga_postfix);
        fprintf(fp, "        $finish;\n");
        fprintf(fp, "      end\n");
        fprintf(fp, "  end\n");
      }
    }
  }
  return;
}

static
void dump_verilog_random_testbench_call_top_module(FILE* fp,
                                                	char* circuit_name) {
  int iblock, iopad_idx;

  fprintf(fp, "// GFPGA instanciation\n");
  fprintf(fp, "  %s%s DUT(\n", circuit_name, formal_verification_top_postfix);

  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(iblock > 0){
        fprintf(fp, ",\n");
      }
      if(VPACK_INPAD == logical_block[iblock].type){
        fprintf(fp, "        %s", logical_block[iblock].name);
      } else if(VPACK_OUTPAD == logical_block[iblock].type){
        fprintf(fp, "        %s%s", 
                logical_block[iblock].name,
                gfpga_postfix);
      }
    }
  }
  fprintf(fp, " );\n");
  fprintf(fp, "// End GFPGA instanciation\n\n");
}

static
void dump_verilog_top_random_stimuli(FILE* fp,
                                     t_spice verilog){
  int iblock, iopad_idx;
  char* reset_input_name = NULL;

  fprintf(fp, "//----- Initialization\n");
  fprintf(fp, "  initial begin\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_INPAD == logical_block[iblock].type) { 
        fprintf(fp, "    %s <= 1'b0;\n", logical_block[iblock].name);
		if(logical_block[iblock].is_clock) 
			clock_input_name = logical_block[iblock].name;
	/*	if(logical_block[iblock].is_reset) 
			reset_input_name = logical_block[iblock].name; */
      }
    }
  }
  if(NULL == clock_input_name){
    clock_input_name = def_clk_name;
    fprintf(fp, "    %s <= 1'b0;\n", def_clk_name);
  }
  fprintf(fp, "    while(1) begin\n");
  fprintf(fp, "      #%.1f\n", ((0.5/verilog.spice_params.stimulate_params.op_clock_freq)/verilog_sim_timescale));
  fprintf(fp, "      %s <= !%s;\n", clock_input_name, clock_input_name);
  fprintf(fp, "    end\n");
  fprintf(fp, "  end\n\n");
/*  fprintf(fp, "//----- Reset Stimulis\n");			// Not ready yet to determine if input is reset
  fprintf(fp, "  initial begin\n");
  fprintf(fp, "    #%.3f\n",(rand() % 10) + 0.001);
  fprintf(fp, "    %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    #%.3f\n",(rand() % 10) + 0.001);
  fprintf(fp, "    %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    while(1) begin\n");
  fprintf(fp, "      #%.3f\n", (rand() % 15) + 0.5);
  fprintf(fp, "      %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "      #%.3f\n", (rand() % 10000) + 200);
  fprintf(fp, "      %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    end\n");
  fprintf(fp, "  end\n\n");  */
  fprintf(fp, "//----- Input Stimulis\n");
  fprintf(fp, "  always@(negedge %s) begin\n", clock_input_name);
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)
           ||(VPACK_OUTPAD == logical_block[iblock].type));
      if(VPACK_INPAD == logical_block[iblock].type) { 
		//if((logical_block[iblock].is_clock || logical_block[iblock].is_reset ) == 0 )
		if(logical_block[iblock].is_clock  == 0 )
          fprintf(fp, "    %s <= $random;\n", logical_block[iblock].name);
      }
    }
  }
  fprintf(fp, "  end\n\n");
  return;
}

void dump_verilog_random_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                          char* circuit_name,
                                          char* top_netlist_name,
                                          char* verilog_dir_path,
                                          int num_clock,
                                          t_syn_verilog_opts fpga_verilog_opts,
                                          t_spice verilog) {
  FILE* fp = NULL;
  char* title = my_strcat("FPGA Verilog Testbench for Formal Top-level netlist of Design: ", circuit_name);
  
  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create top Verilog testbench %s!",
               __FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, 
             "Writing Random Testbench for FPGA Top-level Verilog netlist for  %s...\n", 
             circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir_path);
  verilog_include_simulation_defines_file(fp, verilog_dir_path);

  /* Start of testbench */
  dump_verilog_top_random_testbench_ports(fp, cur_sram_orgz_info, circuit_name, fpga_verilog_opts);

  /* Call defined top-level module */
  dump_verilog_random_testbench_call_top_module(fp, circuit_name);

  /* Call defined benchmark */
  dump_verilog_top_random_testbench_call_benchmark(fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_top_random_stimuli(fp, verilog);

  /* Add output autocheck */
  fprintf(fp, "`ifdef %s\n", autochecked_simulation_flag);
  dump_verilog_top_random_testbench_check(fp);
  fprintf(fp, "`endif\n\n");

  /* Add Icarus requirement */
  dump_verilog_timeout_and_vcd(fp, circuit_name , verilog, cur_sram_orgz_info);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

