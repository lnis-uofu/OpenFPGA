/***********************************/
/*  Synthesizable Verilog Dumping  */
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

/***** Subroutines *****/
/* Return the float number of the time unit required by Modelsim */
float convert_modelsim_time_unit_to_float(char* modelsim_time_unit) {
  /* switch cases is made for s, ms, us, ns, ps and fs*/
  if (0 == strcmp("s", modelsim_time_unit)) {
    return 1;
  } else if (0 == strcmp("ms", modelsim_time_unit)) {
    return 1e-3;
  } else if (0 == strcmp("us", modelsim_time_unit)) {
    return 1e-6;
  } else if (0 == strcmp("ns", modelsim_time_unit)) {
    return 1e-9;
  } else if (0 == strcmp("ps", modelsim_time_unit)) {
    return 1e-12;
  } else if (0 == strcmp("fs", modelsim_time_unit)) {
    return 1e-15;
  } else { 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid time unit(=%s) for modelsim!\n",
               __FILE__, __LINE__, modelsim_time_unit);
    exit(1);
    return 0.;
  }
  return 0.;
}

static 
void modelsim_include_user_defined_verilog_netlists(FILE* fp,
                                                    t_spice spice) {
  int i;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    if (0 == spice.include_netlists[i].included) {
      assert(NULL != spice.include_netlists[i].path);
      fprintf(fp, "  %s \\\n", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

/* Estimate the simulation time period to be assigned in Modelsim:
 * Total simulation time = number of programming clock cycles * programming clock period
 *                       + number of operating clock cycles * operating clock period
 */
static 
float get_verilog_modelsim_simulation_time_period(float time_unit,
                                                  int num_prog_clock_cycles,
                                                  float prog_clock_period,
                                                  int num_op_clock_cycles,
                                                  float op_clock_period) {
  float total_time_period = 0.;

  /* Take into account the prog_reset and reset cycles */
  total_time_period = (num_prog_clock_cycles + 2) * prog_clock_period + num_op_clock_cycles * op_clock_period;
  total_time_period = total_time_period / time_unit;

  return total_time_period; 
}

static 
void dump_verilog_modelsim_proc_script(char* modelsim_proc_filename,
                                       char* modelsim_ini_path, 
                                       char* circuit_name) {
  FILE* fp = NULL;
  char* circuit_top_tb_name = NULL;

  circuit_top_tb_name = my_strcat(circuit_name, modelsim_testbench_module_postfix);

  /* Create Modelsim proc file */
  /* Open file and file handler */
  fp = fopen(modelsim_proc_filename, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Modelsim simulation deck auto-generation scripts: %s", 
               __FILE__, __LINE__, modelsim_proc_filename); 
    exit(1);
  }
  
  fprintf(fp, "proc create_project {projectname project_path} {\n");
  fprintf(fp, "  #Switch to the modelsim folder to create the project\n");
  fprintf(fp, "  set libname $projectname\n"); 
  fprintf(fp, "  set initfile %s\n", modelsim_ini_path);
  fprintf(fp, "  project new $project_path/$projectname $projectname $libname $initfile 0\n");
  fprintf(fp, "}\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc create_project_with_close {projectname modelsim_path} {\n");
  fprintf(fp, "  #Get the current project name\n");
  fprintf(fp, "  set project_env [project env]\n");
  fprintf(fp, "  if {$project_env eq \"\"} {\n");
  fprintf(fp, "      #If string empty (no project)\n");
  fprintf(fp, "      create_project $projectname $modelsim_path\n");
  fprintf(fp, "    } else {\n");
  fprintf(fp, "      #If string not empty (a project is loaded so clsoe it first)\n");
  fprintf(fp, "      project close\n");
  fprintf(fp, "      create_project $projectname $modelsim_path\n");
  fprintf(fp, "    }\n");
  fprintf(fp, "  }\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc add_files_project {verilog_files} {\n");
  fprintf(fp, "  #Get the length of the list\n");
  fprintf(fp, "  set listlength [llength $verilog_files]\n");
  fprintf(fp, "  #Add the verilog files one by one\n");
  fprintf(fp, "  for {set x 0} {$x<$listlength} {incr x} {\n");
  fprintf(fp, "    project addfile [lindex $verilog_files $x]\n");
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc add_waves {} {\n");
  fprintf(fp, "  add wave -position insertpoint sim:/%s/*\n", circuit_top_tb_name);
  fprintf(fp, "}\n");


  fprintf(fp, "proc runsim {simtime unit} {\n");
  fprintf(fp, "  run $simtime $unit\n");
  fprintf(fp, "}\n");


  fprintf(fp, "#Top procedure to create enw project\n");
  fprintf(fp, "proc top_create_new_project {projectname verilog_files modelsim_path simtime unit} {\n");
  fprintf(fp, "  #Create the project\n");
  fprintf(fp, "  create_project_with_close $projectname $modelsim_path\n");
  fprintf(fp, "  #Add the verilog files\n");
  fprintf(fp, "  add_files_project $verilog_files\n");
  fprintf(fp, "  #Compile all the files\n");
  fprintf(fp, "  project compileall\n");
  fprintf(fp, "  #Start the simulation\n");  
  fprintf(fp, "  vsim $projectname.%s -voptargs=+acc\n", circuit_top_tb_name);
  fprintf(fp, "  #Add the waves  \n");
  fprintf(fp, "  add_waves\n");
  fprintf(fp, "  #run the simulation\n");
  fprintf(fp, "  runsim $simtime $unit\n");
  fprintf(fp, "  #Fit the window view\n");
  fprintf(fp, "  wave zoom full\n");
  fprintf(fp, "}\n");

  fprintf(fp, "#Top proc to recompile files and re run the simulation\n");
  fprintf(fp, "proc top_rerun_sim {simtime unit} {\n");
  fprintf(fp, "  #Compile updated verilog files\n");
  fprintf(fp, "  project compileoutofdate\n");
  fprintf(fp, "  #restart the simulation\n");
  fprintf(fp, "  restart -force\n");
  fprintf(fp, "  #run the simulation\n");
  fprintf(fp, "  runsim $simtime $unit\n");
  fprintf(fp, "  #Fit the window view\n");
  fprintf(fp, "  wave zoom full\n");
  fprintf(fp, "}\n");

  /* Close File handler */
  fclose(fp);

  /* Free */
  my_free(circuit_top_tb_name);

  return;
}

static 
void dump_verilog_modelsim_proc_auto_script(char* modelsim_proc_filename,
                                  	       char* modelsim_ini_path, 
                                      	   char* circuit_name,
									   	   boolean include_timing,
									 	   boolean init_sim,
									  	   char* modelsim_project_name) {
  FILE* fp = NULL;
  char* circuit_top_tb_name = NULL;

  circuit_top_tb_name = my_strcat(circuit_name, my_strcat(autocheck_testbench_postfix, modelsim_testbench_module_postfix));

  /* Create Modelsim proc file */
  /* Open file and file handler */
  fp = fopen(modelsim_proc_filename, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Modelsim simulation deck auto-generation scripts: %s", 
               __FILE__, __LINE__, modelsim_proc_filename); 
    exit(1);
  }
  
  fprintf(fp, "proc create_project {projectname project_path} {\n");
  fprintf(fp, "  #Switch to the modelsim folder to create the project\n");
  fprintf(fp, "  set libname $projectname\n"); 
  fprintf(fp, "  set initfile %s\n", modelsim_ini_path);
  fprintf(fp, "  project new $project_path/$projectname $projectname $libname $initfile 0\n");
  fprintf(fp, "}\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc create_project_with_close {projectname modelsim_path} {\n");
  fprintf(fp, "  #Get the current project name\n");
  fprintf(fp, "  set project_env [project env]\n");
  fprintf(fp, "  if {$project_env eq \"\"} {\n");
  fprintf(fp, "      #If string empty (no project)\n");
  fprintf(fp, "      create_project $projectname $modelsim_path\n");
  fprintf(fp, "    } else {\n");
  fprintf(fp, "      #If string not empty (a project is loaded so clsoe it first)\n");
  fprintf(fp, "      project close\n");
  fprintf(fp, "      create_project $projectname $modelsim_path\n");
  fprintf(fp, "    }\n");
  fprintf(fp, "  }\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc add_files_project {verilog_files} {\n");
  fprintf(fp, "  #Get the length of the list\n");
  fprintf(fp, "  set listlength [llength $verilog_files]\n");
  fprintf(fp, "  #Add the verilog files one by one\n");
  fprintf(fp, "  for {set x 0} {$x<$listlength} {incr x} {\n");
  fprintf(fp, "    project addfile [lindex $verilog_files $x]\n");
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");

  fprintf(fp, "  \n");

  fprintf(fp, "proc add_waves {} {\n");
  fprintf(fp, "  add wave -position insertpoint sim:/%s/*\n", circuit_top_tb_name);
  fprintf(fp, "}\n");


  fprintf(fp, "proc runsim {simtime unit} {\n");
  fprintf(fp, "  run $simtime $unit\n");
  fprintf(fp, "}\n");


  fprintf(fp, "#Top procedure to create enw project\n");
  fprintf(fp, "proc top_create_new_project {projectname verilog_files modelsim_path simtime unit} {\n");
  fprintf(fp, "  #Create the project\n");
  fprintf(fp, "  create_project_with_close $projectname $modelsim_path\n");
  fprintf(fp, "  #Add the verilog files\n");
  fprintf(fp, "  add_files_project $verilog_files\n");
  fprintf(fp, "  #Compile all the files\n");
//  fprintf(fp, "  project compileall\n");				// removed to allow compilation with define
// Begin of compilation with Define
  fprintf(fp, "  set myFiles [project filenames]\n");
  fprintf(fp, "  foreach x $myFiles {\n");
  fprintf(fp, "    vlog ");
  if(TRUE == include_timing){
    fprintf(fp, "+define+%s ", verilog_timing_preproc_flag);
  }
  if(TRUE == init_sim){
    fprintf(fp, "+define+%s ", verilog_signal_init_preproc_flag);
  }
  fprintf(fp, "$x\n  }\n");
// End of compilation with Define
  fprintf(fp, "  #Start the simulation\n");  
  fprintf(fp, "  vsim $projectname.%s -voptargs=+acc\n", circuit_top_tb_name);
  fprintf(fp, "  #Add the waves  \n");
  fprintf(fp, "  add_waves\n");
  fprintf(fp, "  #run the simulation\n");
  fprintf(fp, "  runsim $simtime $unit\n");
  fprintf(fp, "  #Fit the window view\n");
  fprintf(fp, "  wave zoom full\n");
  fprintf(fp, "}\n");

  fprintf(fp, "#Top proc to recompile files and re run the simulation\n");
  fprintf(fp, "proc top_rerun_sim {simtime unit} {\n");
// Save format
  fprintf(fp, "  #Save actual format\n");
  fprintf(fp, "  set myLoc [pwd]\n");
  fprintf(fp, "  write format wave -window .main_pane.wave.interior.cs.body.pw.wf $myLoc/relaunch.do\n");
// Quit simulation
  fprintf(fp, "  quit -sim\n");
// Recompile file
  fprintf(fp, "  #Compile updated verilog files\n");
  fprintf(fp, "  set myFiles [project filenames]\n");
  fprintf(fp, "  foreach x $myFiles {\n");
  fprintf(fp, "    vlog ");
  if(TRUE == include_timing){
    fprintf(fp, "+define+%s ", verilog_timing_preproc_flag);
  }
  if(TRUE == init_sim){
    fprintf(fp, "+define+%s ", verilog_signal_init_preproc_flag);
  }
  fprintf(fp, "$x\n  }\n");
// Restart the Simulation
  fprintf(fp, "  set projectname %s\n", modelsim_project_name);
  fprintf(fp, "  vsim $projectname.%s -voptargs=+acc -do relaunch.do\n", circuit_top_tb_name);
// Relaunch the Simulation
  fprintf(fp, "  #run the simulation\n");
  fprintf(fp, "  run $simtime $unit\n");
  fprintf(fp, "}\n");

  /* Close File handler */
  fclose(fp);

  /* Free */
  my_free(circuit_top_tb_name);

  return;
}


static 
void dump_verilog_modelsim_top_script(char* modelsim_top_script_filename,
                                      char* modelsim_proc_script_filename,
                                      char* modelsim_project_path,
                                      char* circuit_name,
                                      char* modelsim_project_name,
                                      float sim_time, 
                                      char* sim_time_unit, 
                                      t_spice spice,
                                      char* src_dir_path) {
  FILE* fp = NULL;

  /* Create Modelsim proc file */
  /* Open file and file handler */
  fp = fopen(modelsim_top_script_filename, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Modelsim simulation deck auto-generation scripts: %s", 
               __FILE__, __LINE__, modelsim_top_script_filename); 
    exit(1);
  }

  fprintf(fp, "set projectname %s\n", modelsim_project_name);
  fprintf(fp, "set benchmark %s\n", circuit_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#in ms\n");
  fprintf(fp, "set simtime %.10g\n", sim_time);
  fprintf(fp, "set unit %s\n", 
          sim_time_unit);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Path were both tcl script are located\n");
  fprintf(fp, "set project_path \"%s%s\"\n", modelsim_project_path, default_modelsim_dir_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Path were the verilog files are located\n");
  fprintf(fp, "set script_path \"%s\"\n", modelsim_project_path);
  fprintf(fp, "set verilog_path \"%s\"\n", src_dir_path);
  fprintf(fp, "set verilog_files [list \\\n");
  /* TODO: include verilog files */
  fprintf(fp, "  ${verilog_path}${benchmark}%s \\\n", 
          verilog_top_postfix);
  fprintf(fp, "  ${verilog_path}${benchmark}%s \\\n",
          top_testbench_verilog_file_postfix);
  /* User-defined verilog netlists */
  init_include_user_defined_verilog_netlists(spice);
  modelsim_include_user_defined_verilog_netlists(fp, spice);

  fprintf(fp, "   ${verilog_path}%s%s \\\n",
          default_lb_dir_name, logic_block_verilog_file_name);
  fprintf(fp, "   ${verilog_path}%s%s \\\n",
          default_rr_dir_name, routing_verilog_file_name);
  fprintf(fp, "   ${verilog_path}%s%s ] \n", 
          default_submodule_dir_name, submodule_verilog_file_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Source the tcl script\n");
  fprintf(fp, "source ${script_path}%s\n", modelsim_proc_script_filename);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Execute the top level procedure\n");
  fprintf(fp, "top_create_new_project $projectname $verilog_files $project_path $simtime $unit\n");
  fprintf(fp, "\n");
  
  fprintf(fp, "#Relaunch simulation\n");
  
  /* Close File handler */
  fclose(fp);

  return;
}

static 
void dump_verilog_modelsim_top_auto_script(char* modelsim_top_auto_script_filename,
                                     	   char* modelsim_proc_auto_script_filename,
                                      	   char* modelsim_project_path,
                             	           char* circuit_name,
                                	       char* modelsim_project_name,
                                	       float sim_time, 
                                	       char* sim_time_unit, 
                                	       t_spice spice, 
                                	       char* src_dir_path) {
  FILE* fp = NULL;

  /* Create Modelsim proc file */
  /* Open file and file handler */
  fp = fopen(modelsim_top_auto_script_filename, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Modelsim simulation deck auto-generation scripts: %s", 
               __FILE__, __LINE__, modelsim_top_auto_script_filename); 
    exit(1);
  }

  fprintf(fp, "set projectname %s\n", modelsim_project_name);
  fprintf(fp, "set benchmark %s\n", circuit_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#in ms\n");
  fprintf(fp, "set simtime %.4g\n", sim_time);
  fprintf(fp, "set unit %s\n", 
          sim_time_unit);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Path were both tcl script are located\n");
  fprintf(fp, "set project_path \"%s%s\"\n", modelsim_project_path, default_modelsim_dir_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Path were the verilog files are located\n");
  fprintf(fp, "set script_path \"%s\"\n", modelsim_project_path);
  fprintf(fp, "set verilog_path \"%s\"\n", src_dir_path);
  fprintf(fp, "set verilog_files [list \\\n");
  /* TODO: include verilog files */
  fprintf(fp, "  ${verilog_path}${benchmark}%s \\\n", 
          verilog_top_postfix);
  fprintf(fp, "  ${verilog_path}${benchmark}%s%s \\\n",
          autocheck_testbench_postfix, top_testbench_verilog_file_postfix);

  /* User-defined verilog netlists */
  init_include_user_defined_verilog_netlists(spice);
  modelsim_include_user_defined_verilog_netlists(fp, spice);

  fprintf(fp, "   ${verilog_path}%s%s \\\n",
          default_lb_dir_name, logic_block_verilog_file_name);
  fprintf(fp, "   ${verilog_path}%s%s \\\n",
          default_rr_dir_name, routing_verilog_file_name);
  fprintf(fp, "   ${verilog_path}%s%s ] \n", 
          default_submodule_dir_name, submodule_verilog_file_name);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Source the tcl script\n");
  fprintf(fp, "source ${script_path}%s\n", modelsim_proc_auto_script_filename);
  fprintf(fp, "\n");
  
  fprintf(fp, "#Execute the top level procedure\n");
  fprintf(fp, "top_create_new_project $projectname $verilog_files $project_path $simtime $unit\n");
  fprintf(fp, "\n");
  
  fprintf(fp, "#Relaunch simulation\n");
  
  /* Close File handler */
  fclose(fp);

  return;
}


/***** Top-level function *****/
void dump_verilog_modelsim_autodeck(t_sram_orgz_info* cur_sram_orgz_info, 
                                    t_syn_verilog_opts fpga_verilog_opts,
                                    t_spice spice,
                                    int num_operating_clock_cycles,
                                    char* verilog_dir_formatted,
                                    char* chomped_circuit_name,
                                    char* src_dir_path) {
  char* modelsim_project_name = NULL;
  char* modelsim_proc_script_filename = NULL; 
  char* modelsim_top_script_filename = NULL; 
  char* modelsim_proc_auto_script_filename = NULL; 
  char* modelsim_top_auto_script_filename = NULL; 
  float simulation_time_period = 0.;

  /* Determine the project name for Modelsim */ 
  modelsim_project_name = my_strcat(chomped_circuit_name, modelsim_project_name_postfix);
  modelsim_top_script_filename = my_strcat(verilog_dir_formatted, my_strcat(chomped_circuit_name, modelsim_top_script_name_postfix));
  modelsim_proc_script_filename = my_strcat(verilog_dir_formatted, my_strcat(chomped_circuit_name, modelsim_proc_script_name_postfix));

  modelsim_top_auto_script_filename = my_strcat(verilog_dir_formatted, my_strcat(chomped_circuit_name, my_strcat(autocheck_testbench_postfix, modelsim_top_script_name_postfix)));
  modelsim_proc_auto_script_filename = my_strcat(verilog_dir_formatted, my_strcat(chomped_circuit_name, my_strcat(autocheck_testbench_postfix, modelsim_proc_script_name_postfix)));

  /* Generate files */ 
  vpr_printf(TIO_MESSAGE_INFO, "Writing Modelsim simulation deck auto-generation scripts...\n");

  /* Check */
  /* Modelsim ini path must be valid!*/
  if (NULL == fpga_verilog_opts.modelsim_ini_path) {
    vpr_printf(TIO_MESSAGE_INFO, "(FILE:%s, [LINE%d])Invalid Modelsim ini path!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Dump the Modelsim process function file */
  if (fpga_verilog_opts.print_top_testbench){
    dump_verilog_modelsim_proc_script(modelsim_proc_script_filename, 
                                      fpga_verilog_opts.modelsim_ini_path, chomped_circuit_name);
  }

  if (fpga_verilog_opts.print_autocheck_top_testbench){
  	dump_verilog_modelsim_proc_auto_script(modelsim_proc_auto_script_filename, 
                                           fpga_verilog_opts.modelsim_ini_path, chomped_circuit_name,
								           fpga_verilog_opts.include_timing, fpga_verilog_opts.include_signal_init,
								           modelsim_project_name);
  }


  /* Compute simulation time period */
  simulation_time_period = get_verilog_modelsim_simulation_time_period(convert_modelsim_time_unit_to_float(modelsim_simulation_time_unit),
                                                                       get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info),
                                                                       1./spice.spice_params.stimulate_params.prog_clock_freq,
                                                                       num_operating_clock_cycles,
                                                                       1./spice.spice_params.stimulate_params.op_clock_freq);

  /* Dump the Modelsim top-level script file */
  if (fpga_verilog_opts.print_top_testbench){
    dump_verilog_modelsim_top_script(modelsim_top_script_filename, 
                                     my_strcat(chomped_circuit_name, modelsim_proc_script_name_postfix),
                                     verilog_dir_formatted, 
                                     chomped_circuit_name, modelsim_project_name,
                                     simulation_time_period, modelsim_simulation_time_unit,
                                     spice,
                                     src_dir_path);
  }

  if (fpga_verilog_opts.print_autocheck_top_testbench){
    dump_verilog_modelsim_top_auto_script(modelsim_top_auto_script_filename, 
                                          my_strcat(chomped_circuit_name, my_strcat(autocheck_testbench_postfix, modelsim_proc_script_name_postfix)),
                                          verilog_dir_formatted, 
                                          chomped_circuit_name, modelsim_project_name,
                                          simulation_time_period, modelsim_simulation_time_unit,
                                          spice,
										  src_dir_path);
  }

  /* Free */
  my_free(modelsim_project_name);
  my_free(modelsim_proc_script_filename); 
  my_free(modelsim_top_script_filename); 
  my_free(modelsim_proc_auto_script_filename); 
  my_free(modelsim_top_auto_script_filename); 

  return;
}


