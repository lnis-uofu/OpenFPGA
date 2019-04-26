#include <assert.h>
#include <string.h>
/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "vpr_types.h"
#include "globals.h"
#include "read_xml_arch_file.h"
#include "ReadOptions.h"
#include "read_blif.h"
#include "SetupVPR.h"
#include "pb_type_graph.h"
#include "ReadOptions.h"
/* mrFPGA: Xifan TANG*/
#include "mrfpga_api.h"
#include "mrfpga_globals.h"
/* END */

/* Xifan Tang: include for supporting Direct Parsing */
#include "vpr_utils.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

#include "shell_file_postfix.h"

char* shell_vpr_get_circuit_name(char* blif_name) {
  int offset;
  char* circuit_name = my_strdup(blif_name);

  /*if the user entered the circuit name with the .blif extension, remove it now*/
  offset = strlen(circuit_name) - 5;
  if (offset > 0 && !strcmp(circuit_name + offset, BLIF_FILE_POSTFIX)) {
    circuit_name[offset] = '\0';
  }
  vpr_printf(TIO_MESSAGE_INFO, "Circuit name: %s%s\n", 
             circuit_name, BLIF_FILE_POSTFIX);
  vpr_printf(TIO_MESSAGE_INFO, "\n");

  return circuit_name;
}

char* shell_vpr_setup_gen_one_file_name(char* circuit_name,
                                        char* cur_out_file_prefix,
                                        char* postfix) {
  int len;
  char* file_name = NULL;

  len = strlen(circuit_name) + 1; /* circuit_name.net/0*/
  if (NULL != cur_out_file_prefix) {
    len += strlen(cur_out_file_prefix);
  }
  if (NULL != postfix) {
    len += strlen(postfix);
  }
  file_name = (char*) my_calloc(len, sizeof(char));
  if (NULL == cur_out_file_prefix ) {
    if (NULL == postfix) {
      sprintf(file_name, "%s", 
              circuit_name);
    } else {
      sprintf(file_name, "%s%s", 
              circuit_name, postfix);
    }
  } else {
    if (NULL == postfix) {
      sprintf(file_name, "%s%s", 
              cur_out_file_prefix,
              circuit_name);
    } else {
      sprintf(file_name, "%s%s%s", 
              cur_out_file_prefix,
              circuit_name, postfix);
    }
  }

  return file_name;
}

void shell_vpr_setup_default_file_names(t_vpr_setup* vpr_setup, 
                                        t_opt_info* opts) {
  char* circuit_name = shell_vpr_get_circuit_name(get_opt_val(opts, "blif_file"));
  char* cur_out_file_prefix = get_opt_val(opts, "out_file_prefix");
  char* cur_default_output_name = NULL;

  vpr_setup->FileNameOpts.ArchFile = get_opt_val(opts, "arch_file");
  vpr_setup->FileNameOpts.CircuitName = circuit_name;
  vpr_setup->FileNameOpts.out_file_prefix = cur_out_file_prefix; 
  vpr_setup->FileNameOpts.BlifFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                       cur_out_file_prefix,
                                                                       BLIF_FILE_POSTFIX); 
  vpr_setup->FileNameOpts.NetFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                       cur_out_file_prefix,
                                                                       NET_FILE_POSTFIX); 
  vpr_setup->FileNameOpts.PlaceFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                         cur_out_file_prefix,
                                                                         PLACE_FILE_POSTFIX); 
  vpr_setup->FileNameOpts.RouteFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                         cur_out_file_prefix,
                                                                         ROUTE_FILE_POSTFIX); 

  vpr_setup->FileNameOpts.ActFile = get_opt_val(opts, "activity_file"); 
  if (NULL == vpr_setup->FileNameOpts.ActFile) {
    vpr_setup->FileNameOpts.ActFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                       cur_out_file_prefix, 
                                                                       ACTIVITY_FILE_POSTFIX); 
  }

  vpr_setup->FileNameOpts.PowerFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                         cur_out_file_prefix,
                                                                         POWER_FILE_POSTFIX); 

  vpr_setup->FileNameOpts.CmosTechFile = shell_vpr_setup_gen_one_file_name(vpr_setup->FileNameOpts.ArchFile, 
                                                                           cur_out_file_prefix,
                                                                           CMOS_TECH_FILE_POSTFIX); 

  vpr_setup->FileNameOpts.SDCFile = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                                      cur_out_file_prefix,
                                                                      SDC_FILE_POSTFIX); 

  cur_default_output_name = shell_vpr_setup_gen_one_file_name(circuit_name, 
                                                              cur_out_file_prefix,
                                                              NULL); 
   
  alloc_and_load_output_file_names(cur_default_output_name);
  
  return;
}

/* Setup the read_arch options, allocate data structures */
void shell_setup_read_arch_opts(t_shell_env* env, t_opt_info* opts) {
  /* Set read_xml_spice flag */
  env->arch.read_xml_spice = is_opt_set(opts, "read_xml_fpga_x2p", FALSE);
  if (TRUE == env->arch.read_xml_spice) {
    vpr_printf(TIO_MESSAGE_INFO, "Enable read_xml_fpga_x2p...\n");
    env->arch.spice = (t_spice*) my_calloc(1, sizeof(t_spice));
  }

  /* Set read_xml_power flag */
  if (TRUE == is_opt_set(opts, "read_xml_versa_power", FALSE)) {
    env->arch.power = (t_power_arch*) my_calloc (1, sizeof(t_power_arch)); 
    env->arch.clocks = (t_clock_arch*) my_calloc (1, sizeof(t_clock_arch)); 
    g_clock_arch = env->arch.clocks;
  } else {
    env->arch.power = NULL;
    env->arch.clocks = NULL;
    g_clock_arch = NULL;
  }

  return;
}

void shell_setup_vpr_arch(t_shell_env* env, t_opt_info* opts) {
  /* Setup ReadArch Options */
  shell_setup_read_arch_opts(env, opts);

  vpr_printf(TIO_MESSAGE_INFO, "Parsing XML file(%s)...\n",
             env->vpr_setup.FileNameOpts.ArchFile);
  XmlReadArch(env->vpr_setup.FileNameOpts.ArchFile, 
              env->vpr_setup.TimingEnabled, 
              &(env->arch), &type_descriptors,
              &num_types);

  vpr_printf(TIO_MESSAGE_INFO, "Setup Architecture...\n");
  
  VPRSetupArch(&(env->arch), &(env->vpr_setup.RoutingArch),
               &(env->vpr_setup.Segments), &(env->vpr_setup.swseg_patterns), 
               &(env->vpr_setup.user_models), &(env->vpr_setup.library_models));

  /* Build the complex block graph */
  vpr_printf(TIO_MESSAGE_INFO, "Building complex block graph.\n");
  alloc_and_load_all_pb_graphs(env->vpr_setup.PowerOpts.do_power);

  /* Xifan Tang: Initialize the clb to clb directs */
  alloc_and_init_globals_clb_to_clb_directs(env->arch.num_directs, env->arch.Directs); 

  if (getEchoEnabled() && isEchoFileEnabled(E_ECHO_PB_GRAPH)) {
    echo_pb_graph(getEchoFileName(E_ECHO_PB_GRAPH));
  }

  if (getEchoEnabled() && isEchoFileEnabled(E_ECHO_ARCH)) {
    EchoArch(getEchoFileName(E_ECHO_ARCH), type_descriptors, num_types,
             &(env->arch));
  }

  return;
}

void shell_setup_vpr_timing(t_shell_env* env) {
  /* Don't do anything if they don't want timing */
  if (FALSE == env->vpr_setup.TimingEnabled) {
    memset(&(env->vpr_setup.Timing), 0, sizeof(t_timing_inf));
    env->vpr_setup.Timing.timing_analysis_enabled = FALSE;
    return;
  }

  env->vpr_setup.Timing.C_ipin_cblock = env->arch.C_ipin_cblock;
  env->vpr_setup.Timing.T_ipin_cblock = env->arch.T_ipin_cblock;
  env->vpr_setup.Timing.timing_analysis_enabled = env->vpr_setup.TimingEnabled;

  /* If the user specified an SDC filename on the command line, look for specified_name.sdc, otherwise look for circuit_name.sdc*/
  env->vpr_setup.Timing.SDCFile = env->vpr_setup.FileNameOpts.SDCFile;

  return;
}

void shell_setup_graphics(t_shell_env* env,  
                          t_opt_info* opts) {
  env->vpr_setup.GraphPause = is_opt_set(opts, "auto", FALSE);
#ifdef NO_GRAPHICS
  env->vpr_setup.ShowGraphics = FALSE; /* DEFAULT */
#else /* NO_GRAPHICS */
  env->vpr_setup.ShowGraphics = !(is_opt_set(opts, "nodisp")); /* DEFAULT */
#endif /* NO_GRAPHICS */

  return;
}

void shell_execute_vpr_setup(t_shell_env* env, 
                             t_opt_info* opts) {

  /* Timing option priorities */
  vpr_printf(TIO_MESSAGE_INFO, "Setting up timing_analysis...\n");
  env->vpr_setup.TimingEnabled = is_opt_set(opts, "timing_analysis", TRUE);

  vpr_printf(TIO_MESSAGE_INFO, "Setting up echo file...\n");
  setEchoEnabled(is_opt_set(opts, "echo_file", FALSE));

  vpr_printf(TIO_MESSAGE_INFO, "Setting up post-synthesis netlists...\n");
  SetPostSynthesisOption(is_opt_set(opts, "generate_postsynthesis_netlist", FALSE));

  vpr_printf(TIO_MESSAGE_INFO, "Setting up constant net delay...\n");
  env->vpr_setup.constant_net_delay = get_opt_float_val(opts, "constant_net_delay", 0.);

  /* Set up default file names  
   * TODO: to be called with 'read_blif'
   */
  vpr_printf(TIO_MESSAGE_INFO, "Setting up default file names...\n");
  shell_vpr_setup_default_file_names(&(env->vpr_setup), opts);

  /* TODO: to be called with 'read_arch' */
  vpr_printf(TIO_MESSAGE_INFO, "Reading Architecture...\n");
  shell_setup_vpr_arch(env, opts);

  /* VPR setup timing */
  vpr_printf(TIO_MESSAGE_INFO, "Setting up timing engine...\n");
  shell_setup_vpr_timing(env);
 
  /* init global variables */
  vpr_printf(TIO_MESSAGE_INFO, "Setting up some global variables...\n");
  out_file_prefix = env->vpr_setup.FileNameOpts.out_file_prefix; 
  grid_logic_tile_area = env->arch.grid_logic_tile_area;
  ipin_mux_trans_size = env->arch.ipin_mux_trans_size;
 
  vpr_printf(TIO_MESSAGE_INFO, "Setting up Graphic engine...\n");
  shell_setup_graphics(env, opts);

  /* Read blif file and sweep unused components */
  vpr_printf(TIO_MESSAGE_INFO, "Reading blif...\n");
  if (TRUE == (boolean)(is_opt_set(opts, "read_xml_fpga_x2p", FALSE) || is_opt_set(opts, "read_xml_versa_power", FALSE))) {
    vpr_printf(TIO_MESSAGE_INFO, "Reading activity...\n");
  }
  read_and_process_blif(env->vpr_setup.FileNameOpts.BlifFile,
                        env->vpr_setup.PackerOpts.sweep_hanging_nets_and_inputs,
                        env->vpr_setup.user_models, env->vpr_setup.library_models,
                        /* Xifan TANG: we need activity in spice modeling */
                        (boolean) (is_opt_set(opts, "read_xml_fpga_x2p", FALSE) || is_opt_set(opts, "read_xml_versa_power", FALSE)),  
                        env->vpr_setup.FileNameOpts.ActFile); 
  fflush(stdout);


  return;
}


