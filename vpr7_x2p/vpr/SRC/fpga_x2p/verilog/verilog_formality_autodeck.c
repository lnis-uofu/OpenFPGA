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
#include <sys/stat.h>
#include <unistd.h>
#define MINI_CASE_SENSITIVE
#include "ini.h"

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "route_common.h"
#include "vpr_utils.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_globals.h"

/* Include Verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_tcl_utils.h"

mINI::INIStructure ini;

static void searching_used_latch(FILE *fp, t_pb * pb, int pb_index, char* chomped_circuit_name, char* inst_name){
  int i, j;
  char WriteBuffer[200];
  char INI_lbl[100];
//	char* tmp = NULL;
  const t_pb_type *pb_type;
  t_mode *mode;
  t_pb_graph_node * node;
//	char* index = NULL;

  pb_type = pb->pb_graph_node->pb_type;
  node = pb->pb_graph_node->physical_pb_graph_node;
  mode = &pb_type->modes[pb->mode];

//	tmp = (char*) my_malloc(sizeof(1 + (strlen(ff_hierarchy) + 1 + strlen(my_strcat(pb_type->name, index)))));
//	tmp = ff_hierarchy;
//	index = my_strcat("_", my_strcat(my_itoa(pb_index), "_"));

  if (pb_type->num_modes > 0) {
    for (i = 0; i < mode->num_pb_type_children; i++) {
      for (j = 0; j < mode->pb_type_children[i].num_pb; j++) {
//				if(strcmp(pb_type->name, mode->name) != 0)
//					tmp = my_strcat(tmp, my_strcat("/", my_strcat(pb_type->name, index)));
        if(pb->child_pbs[i][j].name != NULL)
          searching_used_latch(fp, &pb->child_pbs[i][j], j, chomped_circuit_name, inst_name);
      }
    }
  } else if((pb_type->class_type == LATCH_CLASS) && (pb->name)){
//		tmp = my_strcat(tmp, my_strcat("/", my_strcat(pb_type->physical_pb_type_name, my_strcat(index, "/dff_0_"))));
    fprintf(fp, "set_user_match r:/WORK/%s/%s_reg i:/WORK/%s/%sdff_0 -type cell -noninverted\n", chomped_circuit_name,
                                            pb->name,
                                            inst_name,
                                            gen_verilog_one_pb_graph_node_full_name_in_hierarchy(node) );
    sprintf(WriteBuffer, "%s/%sdff_0 ",
            inst_name, gen_verilog_one_pb_graph_node_full_name_in_hierarchy(node));

    sprintf(INI_lbl, "%s_reg", pb->name);
    ini["REGISTER_MATCH"][INI_lbl] = WriteBuffer;
  }
  //free(tmp); //Looks like is the cause of a double free, once free executated next iteration as no value in tmp
  return;
}

static void clb_iteration(FILE *fp, char* chomped_circuit_name, int h){
  t_pb* pb;
  char* inst_name = NULL;
  const t_pb_type *pb_type;
  t_mode *mode;
  int i, j, x_pos, y_pos;
  char* grid_x = NULL;
  char* grid_y = NULL;

  x_pos = block[h].x;
  y_pos = block[h].y;

  pb = (t_pb*) block[h].pb;

  pb_type = pb->pb_graph_node->pb_type;
  mode = &pb_type->modes[pb->mode];

    grid_x = my_strcat("_", my_strcat(my_itoa(x_pos), "_"));
  grid_y = my_strcat("_", my_strcat(my_itoa(y_pos), "_"));


  if (strcmp(pb_type->name, FILL_TYPE->name) == 0) {
    inst_name = my_strcat(chomped_circuit_name, my_strcat(formal_verification_top_postfix, my_strcat("/", my_strcat(formal_verification_top_module_uut_name, my_strcat("/grid",my_strcat(grid_x, my_strcat(grid_y, "/" )))))));
    if (pb_type->num_modes > 0) {
      for (i = 0; i < mode->num_pb_type_children; i++) {
        inst_name = my_strcat(inst_name, my_strcat("grid_", my_strcat(pb_type->name, my_strcat("_", my_strcat(my_itoa(i), "_")))));
        for (j = 0; j < mode->pb_type_children[i].num_pb; j++) {
          /* If child pb is not used but routing is used, I must print things differently */
          if ((pb->child_pbs[i] != NULL)
              && (pb->child_pbs[i][j].name != NULL)) {
            searching_used_latch(fp, &pb->child_pbs[i][j], j, chomped_circuit_name, inst_name);
          }
        }
      }
    }
  }
  return;
}

static void match_registers(FILE *fp, char* chomped_circuit_name) {
  int h;

  for(h = 0; h < copy_nb_clusters; h++)
    clb_iteration(fp, chomped_circuit_name, h);
/*	for(h = 0; h < copy_nb_clusters; h++){
    free_cb(copy_clb[h].pb);
    free(copy_clb[h].name);
    free(copy_clb[h].nets);
    free(copy_clb[h].pb);
  }*/
//	free(copy_clb);
//	free(block);
  return;
}

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
  int iblock, i, FileCounter=0;
  char* formality_script_file_name = NULL;
  char* benchmark_path = NULL;
  char* original_output_name = NULL;
  char WriteBuffer[200];
  char INI_lbl[20];
/*  int output_length; */
/*  int pos; */
  FILE* fp = NULL;

  if(TRUE == fpga_verilog_opts.print_autocheck_top_testbench)
    benchmark_path = fpga_verilog_opts.reference_verilog_benchmark_file;
  else
    benchmark_path = "Insert verilog benchmark path";

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
  ini["BENCHMARK_INFO"]["benchmark_netlist "] = benchmark_path;

  /* Set reference top */
  fprintf(fp, "set_top r:/WORK/%s\n", chomped_circuit_name);
  ini["BENCHMARK_INFO"]["src_top_module "] = chomped_circuit_name;

  /* Load generated verilog as implemnetation */
  fprintf(fp, "read_verilog -container i -libname WORK -05 { ");
  fprintf(fp, "%s%s%s ",  src_dir_formatted,
              chomped_circuit_name,
              verilog_top_postfix);
  sprintf(WriteBuffer, "%s%s%s", src_dir_formatted, chomped_circuit_name,
          verilog_top_postfix);
  sprintf(INI_lbl, "impl_netlist_%02d",FileCounter++);
  ini["FPGA_INFO"][INI_lbl] = WriteBuffer;

  fprintf(fp, "%s%s%s ",  src_dir_formatted,
              chomped_circuit_name,
              formal_verification_verilog_file_postfix);
  sprintf(WriteBuffer, "%s%s%s\n", src_dir_formatted,
          chomped_circuit_name, formal_verification_verilog_file_postfix);
  sprintf(INI_lbl, "impl_netlist_%02d", FileCounter++);
  ini["FPGA_INFO"][INI_lbl] = WriteBuffer;

  init_include_user_defined_verilog_netlists(spice);
  // formality_include_user_defined_verilog_netlists(fp, spice);

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    if (0 == spice.include_netlists[i].included) {
      assert(NULL != spice.include_netlists[i].path);
      fprintf(fp, "%s ", spice.include_netlists[i].path);
      sprintf(INI_lbl, "impl_netlist_%02d", FileCounter++);
      ini["FPGA_INFO"][INI_lbl] = spice.include_netlists[i].path;
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  }

  fprintf(fp, "%s%s%s ",  src_dir_formatted,
              default_rr_dir_name,
              routing_verilog_file_name);
  sprintf(WriteBuffer, "%s%s%s\n", src_dir_formatted,
          default_rr_dir_name,
          routing_verilog_file_name);
  sprintf(INI_lbl, "impl_netlist_%02d", FileCounter++);
  ini["FPGA_INFO"][INI_lbl] = WriteBuffer;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  fprintf(fp, "%s%s%s ",  src_dir_formatted,
              default_lb_dir_name,
              logic_block_verilog_file_name);
  sprintf(WriteBuffer, "%s%s%s\n", src_dir_formatted,
          default_lb_dir_name,
          logic_block_verilog_file_name);
  sprintf(INI_lbl, "impl_netlist_%02d", FileCounter++);
  ini["FPGA_INFO"][INI_lbl] = WriteBuffer;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  fprintf(fp, "%s%s%s ",  src_dir_formatted,
              default_submodule_dir_name,
              submodule_verilog_file_name);
  sprintf(WriteBuffer, "%s%s%s\n", src_dir_formatted,
          default_submodule_dir_name,
          submodule_verilog_file_name);
  sprintf(INI_lbl, "impl_netlist_%02d", FileCounter++);
  ini["FPGA_INFO"][INI_lbl] = WriteBuffer;
  fprintf(fp, "}\n");

  /* Set implementation top */
  fprintf(fp, "set_top i:/WORK/%s\n", my_strcat(chomped_circuit_name,
                                                formal_verification_top_postfix));
  sprintf(WriteBuffer, "%s", my_strcat(chomped_circuit_name,
                                       formal_verification_top_postfix));
  ini["FPGA_INFO"]["impl_top_module"] = WriteBuffer;

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
                my_strcat(logical_block[iblock].name,
                          formal_verification_top_module_port_postfix));

        sprintf(WriteBuffer, "%s/%s[0]",
                my_strcat(chomped_circuit_name, formal_verification_top_postfix),
                my_strcat(logical_block[iblock].name, formal_verification_top_module_port_postfix));

        sprintf(INI_lbl, "%s", original_output_name);
        ini["PORT_MATCHING"][INI_lbl] = WriteBuffer;
      }
    }
  }
  match_registers(fp, chomped_circuit_name);
  /* Run verification */
  fprintf(fp, "verify\n");
  /* Script END */
  fclose(fp);

  mINI::INIFile file(my_strcat(formality_script_file_name,".ini"));
  file.generate(ini, true);
  return;
}
