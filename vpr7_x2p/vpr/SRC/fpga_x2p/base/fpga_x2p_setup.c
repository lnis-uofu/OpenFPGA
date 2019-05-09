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

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"
#include "route_common.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_timing_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "verilog_api.h"
#include "fpga_x2p_identify_routing.h"
#include "fpga_x2p_setup.h"

/***** Subroutines Declarations *****/
static 
int map_pb_type_port_to_spice_model_ports(t_pb_type* cur_pb_type,
                                          t_spice_model* cur_spice_model);

static 
void match_pb_types_spice_model_rec(t_pb_type* cur_pb_type,
                                    int num_spice_model,
                                    t_spice_model* spice_models);

static 
void init_and_check_one_sram_inf_orgz(t_sram_inf_orgz* cur_sram_inf_orgz,
                                      int num_spice_model,
                                      t_spice_model* spice_models);

static
void init_and_check_sram_inf(t_arch* arch,
                             t_det_routing_arch* routing_arch);

static 
t_llist* check_and_add_one_global_port_to_llist(t_llist* old_head, 
                                                t_spice_model_port* candidate_port);

static 
void rec_stat_pb_type_keywords(t_pb_type* cur_pb_type,
                               int* num_keyword);

static 
void rec_add_pb_type_keywords_to_list(t_pb_type* cur_pb_type,
                                      int* cur,
                                      char** keywords,
                                      char* prefix);

static 
int check_conflict_syntax_char_in_string(t_llist* LL_reserved_syntax_char_head,
                                         char* str_to_check);

static 
void check_spice_model_name_conflict_syntax_char(t_arch Arch,
                                                 t_llist* LL_reseved_syntax_char_head);

static 
t_llist* init_llist_verilog_and_spice_syntax_char();

static 
boolean is_verilog_and_spice_syntax_conflict_char(t_llist* LL_reserved_syntax_char_head, 
                                                  char ref_char);

static 
int check_and_rename_logical_block_and_net_names(t_llist* LL_reserved_syntax_char_head, 
                                                 char* circuit_name,
                                                 boolean rename_illegal_port,
                                                 int LL_num_logical_blocks, t_logical_block* LL_logical_block,
                                                 int LL_num_clb_nets, t_net* LL_clb_net,
                                                 int LL_num_vpack_nets, t_net* LL_vpack_net);

/***** Subroutines *****/

/* Map (synchronize) pb_type ports to SPICE model ports
 */
static 
int map_pb_type_port_to_spice_model_ports(t_pb_type* cur_pb_type,
                                          t_spice_model* cur_spice_model) {
  int iport;
  t_port* cur_pb_type_port = NULL;

  /* Check */
  assert(NULL != cur_pb_type);

  /* Initialize each port */
  for (iport = 0; iport < cur_pb_type->num_ports; iport++) {
    cur_pb_type->ports[iport].spice_model_port = NULL; 
  } 

  /* Return if SPICE_MODEL is NULL */
  if (NULL == cur_spice_model) {
   return 0;
  }

  /* For each port, find a SPICE model port, which has the same name and port size */
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    cur_pb_type_port = 
       find_pb_type_port_match_spice_model_port(cur_pb_type, 
                                                &(cur_spice_model->ports[iport])); 
    /* Not every spice_model_port can find a mapped pb_type_port.
     * Since a pb_type only includes necessary ports in technology mapping.
     * ports for physical designs may be ignored ! 
     */
    if (NULL != cur_pb_type_port) {
      cur_pb_type_port->spice_model_port = &(cur_spice_model->ports[iport]); 
    }
  }
  /* Although some spice_model_port may not have a corresponding pb_type_port 
   * but each pb_type_port should be mapped to a spice_model_port
   */
  for (iport = 0; iport < cur_pb_type->num_ports; iport++) {
    if (NULL == cur_pb_type->ports[iport].spice_model_port) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d])Pb_type(%s) Port(%s) cannot find a corresponding port in SPICE model(%s)\n",
                 __FILE__, __LINE__, cur_pb_type->name, cur_pb_type->ports[iport].name, 
                 cur_spice_model->name);
      exit(1);
    }
  }

  return cur_pb_type->num_ports;
}

/* Find spice_model_name definition in pb_types
 * Try to match the name with defined spice_models
 */
static 
void match_pb_types_spice_model_rec(t_pb_type* cur_pb_type,
                                    int num_spice_model,
                                    t_spice_model* spice_models) {
  int imode, ipb, jinterc;
  
  if (NULL == cur_pb_type) {
    vpr_printf(TIO_MESSAGE_WARNING,"(File:%s,LINE[%d])cur_pb_type is null pointor!\n",__FILE__,__LINE__);
    return;
  }

  /* If there is a spice_model_name or refer to a physical pb type , this is a leaf node!*/
  if ((NULL != cur_pb_type->spice_model_name) || (NULL != cur_pb_type->physical_pb_type_name))  {
    /* What annoys me is VPR create a sub pb_type for each lut which suppose to be a leaf node
     * This may bring software convience but ruins SPICE modeling
     */
    /* if this is not a physical pb_type, we do not care the spice model name and associated checking */
    if (NULL != cur_pb_type->physical_pb_type_name) {
      vpr_printf(TIO_MESSAGE_INFO, "(File:%s,LINE[%d]) Bypass spice model checking for pb_type(%s)!\n",
                 __FILE__, __LINE__, cur_pb_type->name);
      return;
    }
    /* Let's find a matched spice model!*/
    printf("INFO: matching cur_pb_type=%s with spice_model_name=%s...\n",cur_pb_type->name, cur_pb_type->spice_model_name);
    assert(NULL == cur_pb_type->spice_model);
    cur_pb_type->spice_model = find_name_matched_spice_model(cur_pb_type->spice_model_name, num_spice_model, spice_models);
    if (NULL == cur_pb_type->spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Fail to find a defined SPICE model called %s, in pb_type(%s)!\n",
                 __FILE__, __LINE__, cur_pb_type->spice_model_name, cur_pb_type->name);
      exit(1);
    }
    /* Map pb_type ports to SPICE model ports*/
    map_pb_type_port_to_spice_model_ports(cur_pb_type,cur_pb_type->spice_model);
    return;
  }
  /* Traversal the hierarchy*/
  for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
    /* Task 1: Find the interconnections and match the spice_model */
    for (jinterc = 0; jinterc < cur_pb_type->modes[imode].num_interconnect; jinterc++) {
      assert(NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model);
      /* If the spice_model_name is not defined, we use the default*/
      if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name) {
        switch (cur_pb_type->modes[imode].interconnect[jinterc].type) {
        case DIRECT_INTERC:
          cur_pb_type->modes[imode].interconnect[jinterc].spice_model = 
            get_default_spice_model(SPICE_MODEL_WIRE,num_spice_model,spice_models);
          break;
        case COMPLETE_INTERC:
          /* Special for Completer Interconnection:
           * 1. The input number is 1, this infers a direct interconnection.
           * 2. The input number is larger than 1, this infers multplexers
           * according to interconnect[j].num_mux identify the number of input at this level
           */
          if (0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux) {
            cur_pb_type->modes[imode].interconnect[jinterc].spice_model = 
              get_default_spice_model(SPICE_MODEL_WIRE,num_spice_model,spice_models);
          } else {
            cur_pb_type->modes[imode].interconnect[jinterc].spice_model = 
              get_default_spice_model(SPICE_MODEL_MUX,num_spice_model,spice_models);
            if (NULL != cur_pb_type->modes[imode].interconnect[jinterc].loop_breaker_string) {
              if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model->input_buffer) {
                vpr_printf(TIO_MESSAGE_INFO,"Line[%d] Cannot disable an interconnect without input buffering",
                cur_pb_type->modes[imode].interconnect[jinterc].line_num);
              } 
            }
          } 
          break;
        case MUX_INTERC:
          cur_pb_type->modes[imode].interconnect[jinterc].spice_model = 
            get_default_spice_model(SPICE_MODEL_MUX,num_spice_model,spice_models);
          if (NULL != cur_pb_type->modes[imode].interconnect[jinterc].loop_breaker_string) {
            if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model->input_buffer) {
              vpr_printf(TIO_MESSAGE_INFO,"Line[%d] Cannot disable an interconnect without input buffering",
              cur_pb_type->modes[imode].interconnect[jinterc].line_num);
            } 
          }
          break;
        default:
          break; 
        }        
        vpr_printf(TIO_MESSAGE_INFO,"INFO: Link a SPICE model (%s) for Interconnect (%s)!\n",
                   cur_pb_type->modes[imode].interconnect[jinterc].spice_model->name, cur_pb_type->modes[imode].interconnect[jinterc].name);
      } else {
        cur_pb_type->modes[imode].interconnect[jinterc].spice_model = 
          find_name_matched_spice_model(cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, num_spice_model, spice_models);
        vpr_printf(TIO_MESSAGE_INFO,"INFO: Link a SPICE model (%s) for Interconnect (%s)!\n",
                   cur_pb_type->modes[imode].interconnect[jinterc].spice_model->name, cur_pb_type->modes[imode].interconnect[jinterc].name);
        if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model) {
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d]) Fail to find a defined SPICE model called %s, in pb_type(%s)!\n",
                     __FILE__, __LINE__, cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, cur_pb_type->name);
          exit(1);
        } 
        switch (cur_pb_type->modes[imode].interconnect[jinterc].type) {
        case DIRECT_INTERC:
          if (SPICE_MODEL_WIRE != cur_pb_type->modes[imode].interconnect[jinterc].spice_model->type) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d]) Invalid type of matched SPICE model called %s, in pb_type(%s)! Sould be wire!\n",
                       __FILE__, __LINE__, cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, cur_pb_type->name);
            exit(1);
          }
          break;
        case COMPLETE_INTERC:
          if (0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux) {
            if (SPICE_MODEL_WIRE != cur_pb_type->modes[imode].interconnect[jinterc].spice_model->type) {
              vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d]) Invalid type of matched SPICE model called %s, in pb_type(%s)! Sould be wire!\n",
                         __FILE__, __LINE__, cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, cur_pb_type->name);
              exit(1);
            }
          } else {
            if (SPICE_MODEL_MUX != cur_pb_type->modes[imode].interconnect[jinterc].spice_model->type) {
              vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d]) Invalid type of matched SPICE model called %s, in pb_type(%s)! Sould be MUX!\n",
                         __FILE__, __LINE__, cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, cur_pb_type->name);
              exit(1);
            }
          }
            if (NULL != cur_pb_type->modes[imode].interconnect[jinterc].loop_breaker_string) {
              if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model->input_buffer) {
                vpr_printf(TIO_MESSAGE_INFO,"Line[%d] Cannot disable an interconnect without input buffering",
                cur_pb_type->modes[imode].interconnect[jinterc].line_num);
              } 
            }
          break;
        case MUX_INTERC:
          if (SPICE_MODEL_MUX != cur_pb_type->modes[imode].interconnect[jinterc].spice_model->type) {
            vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d]) Invalid type of matched SPICE model called %s, in pb_type(%s)! Sould be MUX!\n",
                       __FILE__, __LINE__, cur_pb_type->modes[imode].interconnect[jinterc].spice_model_name, cur_pb_type->name);
            exit(1);
          }
            if (NULL != cur_pb_type->modes[imode].interconnect[jinterc].loop_breaker_string) {
              if (NULL == cur_pb_type->modes[imode].interconnect[jinterc].spice_model->input_buffer) {
                vpr_printf(TIO_MESSAGE_INFO,"Line[%d] Cannot disable an interconnect without input buffering",
                cur_pb_type->modes[imode].interconnect[jinterc].line_num);
              } 
            }
          break;
        default:
          break; 
        }        
      }
    }
    /* Task 2: Find the child pb_type, do matching recursively */
    //if (1 == cur_pb_type->modes[imode].define_spice_model) {
    for (ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
      match_pb_types_spice_model_rec(&cur_pb_type->modes[imode].pb_type_children[ipb],
                                     num_spice_model,
                                     spice_models);
    }
    //}
  } 
  return;  
}

static 
void init_and_check_one_sram_inf_orgz(t_sram_inf_orgz* cur_sram_inf_orgz,
                                      int num_spice_model,
                                      t_spice_model* spice_models) {
  /* If cur_sram_inf_orgz is not initialized, do nothing */
  if (NULL == cur_sram_inf_orgz) {
    return;
  } 

  /* For SRAM */
  if (NULL == cur_sram_inf_orgz->spice_model_name) { 
    cur_sram_inf_orgz->spice_model = get_default_spice_model(SPICE_MODEL_SRAM,
                                                             num_spice_model, 
                                                             spice_models);
  } else {
    cur_sram_inf_orgz->spice_model = 
        find_name_matched_spice_model(cur_sram_inf_orgz->spice_model_name,
                                      num_spice_model, 
                                      spice_models); 
  }

  if (NULL == cur_sram_inf_orgz->spice_model) {
    if (NULL == cur_sram_inf_orgz->spice_model_name) { 
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) Cannot find any SRAM spice model!\n",
                 __FILE__ ,__LINE__);
      exit(1);
    } else  {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of SRAM is undefined in SPICE models!\n",
                 __FILE__ ,__LINE__, cur_sram_inf_orgz->spice_model_name);
      exit(1);
    }
  }

  /* Check the type of SRAM_SPICE_MODEL */
  switch (cur_sram_inf_orgz->type) {
  case SPICE_SRAM_STANDALONE:
    vpr_printf(TIO_MESSAGE_INFO, "INFO: Checking if SRAM spice model fit standalone organization...\n");
    if (SPICE_MODEL_SRAM != cur_sram_inf_orgz->spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Standalone SRAM organization requires a SPICE model(type=sram)!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* TODO: check SRAM ports */
    check_sram_spice_model_ports(cur_sram_inf_orgz->spice_model, FALSE);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    vpr_printf(TIO_MESSAGE_INFO, "INFO: Checking if SRAM spice model fit scan-chain organization...\n");
    if (SPICE_MODEL_SCFF != cur_sram_inf_orgz->spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Scan-chain SRAM organization requires a SPICE model(type=sff)!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* TODO: check Scan-chain Flip-flop ports */
    check_ff_spice_model_ports(cur_sram_inf_orgz->spice_model, TRUE);
    /* TODO: RRAM Scan-chain is not supported yet. Now just forbidden this option */
    if (SPICE_MODEL_DESIGN_RRAM == cur_sram_inf_orgz->spice_model->design_tech) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) RRAM-based Scan-chain Flip-flop has not been supported yet!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    vpr_printf(TIO_MESSAGE_INFO, "INFO: Checking if SRAM spice model fit memory-bank organization...\n");
    if (SPICE_MODEL_SRAM != cur_sram_inf_orgz->spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Memory-bank SRAM organization requires a SPICE model(type=sram)!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* TODO: check if this one has bit lines and word lines */
    check_sram_spice_model_ports(cur_sram_inf_orgz->spice_model, TRUE);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}                         

static
void init_and_check_sram_inf(t_arch* arch,
                             t_det_routing_arch* routing_arch) {
  /* We have two branches: 
   * 1. SPICE SRAM organization information 
   * 2. Verilog SRAM organization information 
   */
  init_and_check_one_sram_inf_orgz(arch->sram_inf.spice_sram_inf_orgz, 
                                   arch->spice->num_spice_model,
                                   arch->spice->spice_models);
 
  init_and_check_one_sram_inf_orgz(arch->sram_inf.verilog_sram_inf_orgz, 
                                   arch->spice->num_spice_model,
                                   arch->spice->spice_models);
                         

  return;
}

/* Initialize and check spice models in architecture
 * Tasks:
 * 1. Link the spice model defined in pb_types and routing switches
 * 2. Add default spice model (MUX) if needed
 */
void init_check_arch_spice_models(t_arch* arch,
                                  t_det_routing_arch* routing_arch) {
  int i, iport;

  vpr_printf(TIO_MESSAGE_INFO,"Initializing and checking SPICE models...\n");
  /* Check Spice models first*/
  assert(NULL != arch);
  assert(NULL != arch->spice);
  if ((0 == arch->spice->num_spice_model)||(0 > arch->spice->num_spice_model)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])SPICE models are not defined! Miss this part in architecture file.\n",__FILE__,__LINE__);
    exit(1);
  }
  assert(NULL != arch->spice->spice_models);
  
  /* Find default spice model*/
  /* MUX */
  if (NULL == get_default_spice_model(SPICE_MODEL_MUX,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Fail to find the default MUX SPICE Model! Should define it in architecture file\n",__FILE__,__LINE__);
    exit(1);
  }

  /* Channel Wire */
  if (NULL == get_default_spice_model(SPICE_MODEL_CHAN_WIRE,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Fail to find the default Channel Wire SPICE Model! Should define it in architecture file\n",__FILE__,__LINE__);
    exit(1);
  }

  /* Wire */
  if (NULL == get_default_spice_model(SPICE_MODEL_WIRE,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Fail to find the default Wire SPICE Model! Should define it in architecture file\n",__FILE__,__LINE__);
    exit(1);
  }

  /* Link the input/output buffer spice models to higher level spice models 
   * Configure (fill information) the input/output buffers of high level spice models */
  config_spice_model_input_output_buffers_pass_gate(arch->spice->num_spice_model, 
                                                    arch->spice->spice_models);

  /* Find inversion spice_model for ports */
  config_spice_model_port_inv_spice_model(arch->spice->num_spice_model, 
                                          arch->spice->spice_models);

  /* 1. Link the spice model defined in pb_types and routing switches */
  /* Step A:  Check routing switches, connection blocks*/
  if ((0 == arch->num_cb_switch)||(0 > arch->num_cb_switch)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) Define Switches for Connection Blocks is mandatory in SPICE model support! Miss this part in architecture file.\n",__FILE__,__LINE__);
    exit(1);
  }
  
  for (i = 0; i < arch->num_cb_switch; i++) {
    arch->cb_switches[i].spice_model = 
      find_name_matched_spice_model(arch->cb_switches[i].spice_model_name,
                                    arch->spice->num_spice_model, 
                                    arch->spice->spice_models); 
    if (NULL == arch->cb_switches[i].spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of Switch(%s) is undefined in SPICE models!\n",__FILE__ ,__LINE__, arch->cb_switches[i].spice_model_name, arch->cb_switches[i].name);
      exit(1);
    }
    /* Check the spice model structure is matched with the structure in switch_inf */
    if (FALSE == check_spice_model_structure_match_switch_inf(arch->cb_switches[i])) {
      exit(1);
    }
  } 
 
  /* Step B: Check switch list: Switch Box*/
  if ((0 == arch->num_switches)||(0 > arch->num_switches)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) Define Switches for Switch Boxes is mandatory in SPICE model support! Miss this part in architecture file.\n",__FILE__,__LINE__);
    exit(1);
  }
  
  for (i = 0; i < arch->num_switches; i++) {
    arch->Switches[i].spice_model = 
      find_name_matched_spice_model(arch->Switches[i].spice_model_name,
                                    arch->spice->num_spice_model, 
                                    arch->spice->spice_models); 
    if (NULL == arch->Switches[i].spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of Switch(%s) is undefined in SPICE models!\n",__FILE__ ,__LINE__, arch->Switches[i].spice_model_name, arch->Switches[i].name);
      exit(1);
    }
    /* Check the spice model structure is matched with the structure in switch_inf */
    if (FALSE == check_spice_model_structure_match_switch_inf(arch->Switches[i])) {
      exit(1);
    }
  } 

  /* Update the switches in detailed routing architecture settings*/
  for (i = 0; i < routing_arch->num_switch; i++) {
    if (NULL == switch_inf[i].spice_model_name) { 
      switch_inf[i].spice_model = get_default_spice_model(SPICE_MODEL_MUX,
                                                          arch->spice->num_spice_model, 
                                                          arch->spice->spice_models);
      continue;
    }
    switch_inf[i].spice_model = 
      find_name_matched_spice_model(switch_inf[i].spice_model_name,
                                    arch->spice->num_spice_model, 
                                    arch->spice->spice_models); 
    if (NULL == switch_inf[i].spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of Switch(%s) is undefined in SPICE models!\n",__FILE__ ,__LINE__, switch_inf[i].spice_model_name, switch_inf[i].name);
      exit(1);
    }
  }

  /* Step C: Find SRAM Model*/
  init_and_check_sram_inf(arch, routing_arch);

  /* Step D: Find the segment spice_model*/
  for (i = 0; i < arch->num_segments; i++) {
    if (NULL == arch->Segments[i].spice_model_name) {
      arch->Segments[i].spice_model = 
        get_default_spice_model(SPICE_MODEL_CHAN_WIRE,
                                arch->spice->num_spice_model, 
                                arch->spice->spice_models); 
      continue;
    } else {
      arch->Segments[i].spice_model = 
        find_name_matched_spice_model(arch->Segments[i].spice_model_name,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models); 
    }
    if (NULL == arch->Segments[i].spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR, "(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of Segment(Length:%d) is undefined in SPICE models!\n",
                                    __FILE__ ,__LINE__, 
                                    arch->Segments[i].spice_model_name, 
                                    arch->Segments[i].length);
      exit(1);
    } else if (SPICE_MODEL_CHAN_WIRE != arch->Segments[i].spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(FILE:%s, LINE[%d])Invalid SPICE model(%s) type of Segment(Length:%d)! Should be chan_wire!\n",
                                    __FILE__ , __LINE__, 
                                    arch->Segments[i].spice_model_name, 
                                    arch->Segments[i].length);
      exit(1);
    }
  } 

  /* Step E: Direct connections between CLBs */
  for (i = 0; i < arch->num_directs; i++) {
    if (NULL == arch->Directs[i].spice_model_name) {
      arch->Directs[i].spice_model = 
        get_default_spice_model(SPICE_MODEL_WIRE,
                                arch->spice->num_spice_model, 
                                arch->spice->spice_models); 
      continue;
    } else {
      arch->Directs[i].spice_model = 
        find_name_matched_spice_model(arch->Directs[i].spice_model_name,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models); 
    }
    /* Check SPICE model type */
    if (NULL == arch->Directs[i].spice_model) {
      vpr_printf(TIO_MESSAGE_ERROR, "(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of CLB to CLB Direct Connection (name=%s) is undefined in SPICE models!\n",
                                    __FILE__ ,__LINE__, 
                                    arch->Directs[i].spice_model_name, 
                                    arch->Directs[i].name);
      exit(1);
    } else if (SPICE_MODEL_CHAN_WIRE != arch->Directs[i].spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR, "(FILE:%s, LINE[%d])Invalid SPICE model(%s) type of CLB to CLB Direct Connection (name=%s)! Should be chan_wire!\n",
                                    __FILE__ , __LINE__, 
                                    arch->Directs[i].spice_model_name, 
                                    arch->Directs[i].name);
      exit(1);
    }
    /* Copy it to clb2clb_directs */
    clb2clb_direct[i].spice_model = arch->Directs[i].spice_model; 
  } 

  /* 2. Search Complex Blocks (Pb_Types), Link spice_model according to the spice_model_name*/
  for (i = 0; i < num_types; i++) {
    if (type_descriptors[i].pb_type) {
       match_pb_types_spice_model_rec(type_descriptors[i].pb_type,
                                      arch->spice->num_spice_model,
                                      arch->spice->spice_models);
    }
  }

  /* 3. Initial grid_index_low/high for each spice_model */
  for (i = 0; i < arch->spice->num_spice_model; i++) {
    alloc_spice_model_grid_index_low_high(&(arch->spice->spice_models[i]));
    alloc_spice_model_routing_index_low_high(&(arch->spice->spice_models[i]));
  }
  /* 4. zero the counter of each spice_model */
  zero_spice_models_cnt(arch->spice->num_spice_model, arch->spice->spice_models);
  /* 5. zero all index low high */
  /*
  zero_spice_model_grid_index_low_high(arch->spice->num_spice_model, arch->spice->spice_models);
  zero_spice_models_routing_index_low_high(arch->spice->num_spice_model, arch->spice->spice_models);
  */
 
  /* 6. Check each port of a spice model and create link to another spice model */
  for (i = 0; i < arch->spice->num_spice_model; i++) {
    for (iport = 0; iport < arch->spice->spice_models[i].num_port; iport++) {
      /* Set to NULL pointor first */
      arch->spice->spice_models[i].ports[iport].spice_model = NULL;
      if (NULL != arch->spice->spice_models[i].ports[iport].spice_model_name) {
        arch->spice->spice_models[i].ports[iport].spice_model = 
          find_name_matched_spice_model(arch->spice->spice_models[i].ports[iport].spice_model_name,
                                        arch->spice->num_spice_model, 
                                        arch->spice->spice_models); 
      }
    }
  }

  /* 7. Create timing graph for spice models */
  for (i = 0; i < arch->spice->num_spice_model; i++) {
    /* See if we need a timing graph */
    if (0 == arch->spice->spice_models[i].num_delay_info) {
      continue;
    }
    annotate_spice_model_timing(&(arch->spice->spice_models[i]));
  }

  return;
}

/* Recursively traverse pb_type graph and mark idle mode 
 * Only one idle mode is allowed under each pb_type
 */
static 
void rec_identify_pb_type_idle_mode(t_pb_type* cur_pb_type) {
  int imode, ichild, idle_mode_idx;

  /* Do it only when we have modes */
  if ( 0 < cur_pb_type->num_modes) {
    /* Find idle mode index */
    idle_mode_idx = find_pb_type_idle_mode_index(*cur_pb_type);
    cur_pb_type->modes[idle_mode_idx].define_idle_mode = TRUE;
    return;
  }

  /* Traverse all the modes for identifying idle mode */
  for (imode = 0; cur_pb_type->num_modes; imode++) {
    /* Check each pb_type_child */
    for (ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ichild++) { 
      rec_identify_pb_type_idle_mode(&(cur_pb_type->modes[imode].pb_type_children[ichild]));
    }
  }

  return;
}


/* Recursively traverse pb_type graph and mark idle and physical mode 
 * Only one idle mode and one physical mode is allowed under each pb_type
 * In particular, a physical mode should appear only when its parent is a physical mode.
 */
static 
void rec_identify_pb_type_phy_mode(t_pb_type* cur_pb_type) {
  int imode, ichild, phy_mode_idx;

  /* Only try to find physical mode when parent is a physical mode or this is the top cur_pb_type! */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) {
    if ((NULL == cur_pb_type->parent_mode)
       || (TRUE == cur_pb_type->parent_mode->define_physical_mode)) {
      /* Find physical mode index */
      phy_mode_idx = find_pb_type_physical_mode_index(*cur_pb_type);
      cur_pb_type->modes[phy_mode_idx].define_physical_mode = TRUE;
    } else { 
      /* The parent must not be a physical mode*/
      assert (FALSE == cur_pb_type->parent_mode->define_physical_mode);
      phy_mode_idx = -1;
      /* Traverse all the modes for identifying idle mode */
      for (imode = 0; cur_pb_type->num_modes; imode++) {
        cur_pb_type->modes[imode].define_physical_mode = FALSE;
      }
    }
    return;
  }

  /* Traverse all the modes for identifying idle mode */
  for (imode = 0; cur_pb_type->num_modes; imode++) {
    /* Check each pb_type_child */
    for (ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ichild++) { 
      rec_identify_pb_type_phy_mode(&(cur_pb_type->modes[imode].pb_type_children[ichild]));
    }
  }

  return;
}

/* Identify physical mode of pb_types in each defined complex block */
static 
void init_check_arch_pb_type_idle_and_phy_mode(t_arch* Arch) {
  int itype;

  for (itype = 0; itype < num_types; itype++) {
    if (type_descriptors[itype].pb_type) {
      rec_identify_pb_type_idle_mode(type_descriptors[itype].pb_type);
      rec_identify_pb_type_phy_mode(type_descriptors[itype].pb_type);
    }
  }

  return;
}

/* Statistics reserved names in pb_types to the list*/
static 
void rec_stat_pb_type_keywords(t_pb_type* cur_pb_type,
                               int* num_keyword) {
  int imode, ipb, jpb;

  assert((0 == (*num_keyword))||(0 < (*num_keyword)));
  assert(NULL != num_keyword);
  assert(NULL != cur_pb_type);

  for (ipb = 0; ipb < cur_pb_type->num_pb; ipb++) {
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      /* pb_type_name[num_pb]_mode[mode_name]*/
      (*num_keyword) += 1;
      for (jpb = 0; jpb < cur_pb_type->modes[imode].num_pb_type_children; jpb++) {
        if (NULL == cur_pb_type->modes[imode].pb_type_children[jpb].spice_model) {
          rec_stat_pb_type_keywords(&(cur_pb_type->modes[imode].pb_type_children[jpb]),
                                    num_keyword);
        }
      }
    }
  }

  return;
}

/* Add reserved names in pb_types to the list*/
static 
void rec_add_pb_type_keywords_to_list(t_pb_type* cur_pb_type,
                                      int* cur,
                                      char** keywords,
                                      char* prefix) {
  int imode, ipb, jpb;
  char* formatted_prefix = format_spice_node_prefix(prefix);
  char* pass_on_prefix = NULL;

  assert(NULL != cur);
  assert((0 == (*cur))||(0 < (*cur)));
  assert(NULL != keywords);
  assert(NULL != cur_pb_type);

  /* pb_type_name[num_pb]_mode[mode_name]*/
  // num_keyword += cur_pb_type->num_pb * cur_pb_type->num_modes;
  for (ipb = 0; ipb < cur_pb_type->num_pb; ipb++) {
    for (imode = 0; imode < cur_pb_type->num_modes; imode++) {
      keywords[(*cur)] = (char*)my_malloc(sizeof(char)*
                                         (strlen(formatted_prefix) + strlen(cur_pb_type->name) + 1 + strlen(my_itoa(ipb)) + 7 
                                          + strlen(cur_pb_type->modes[imode].name) + 2));
      sprintf(keywords[(*cur)], "%s%s[%d]_mode[%s]", formatted_prefix, cur_pb_type->name, ipb, cur_pb_type->modes[imode].name);
      pass_on_prefix = my_strdup(keywords[(*cur)]);
      (*cur)++;
      for (jpb = 0; jpb < cur_pb_type->modes[imode].num_pb_type_children; jpb++) {
        if (NULL == cur_pb_type->modes[imode].pb_type_children[jpb].spice_model) {
          rec_add_pb_type_keywords_to_list(&(cur_pb_type->modes[imode].pb_type_children[jpb]),
                                             cur, keywords, pass_on_prefix);
        }
      }
      my_free(pass_on_prefix);
    }
  }

  my_free(formatted_prefix);

  return;
}

/* This function checks conflicts between
 * 1. SPICE model names and reserved sub-circuit names
 */
void check_keywords_conflict(t_arch Arch) {
  int num_keyword = 0;
  char**keywords;
  int conflict = 0;

  int num_keyword_per_grid = 0;
  int cur, iseg, imodel, i, iport;
  int ix, iy, iz;
  t_pb_type* cur_pb_type = NULL;
  char* prefix = NULL;
  t_llist* temp = NULL;
  t_spice_model_port* cur_global_port = NULL;

  /* Generate the list of reserved names */
  num_keyword = 0;
  keywords = NULL;

  /* Reserved names: grid names */
  /* Reserved names: pb_type names */
  for (ix = 0; ix < (nx + 2); ix++) { 
    for (iy = 0; iy < (ny + 2); iy++) { 
      /* by_pass the empty */
      if (EMPTY_TYPE != grid[ix][iy].type) {
        num_keyword += 1; /* plus grid[ix][iy]*/ 
        for (iz = 0; iz < grid[ix][iy].type->capacity; iz++) {
          num_keyword_per_grid = 0;
          /* Per grid, type_descriptor.name[i]*/
          /* Go recursive pb_graph_node, until the leaf which defines a spice_model */
          cur_pb_type = grid[ix][iy].type->pb_type;
          rec_stat_pb_type_keywords(cur_pb_type, &num_keyword_per_grid);
          num_keyword += num_keyword_per_grid; 
        }
      }
    }
  }

  /* Reserved names: switch boxes, connection boxes, channels */
  /* Channels -X */
  num_keyword += (ny+1) * nx;
  /* Channels -Y */
  num_keyword += (nx+1) * ny;
  /* Switch Boxes */
  /* sb[ix][iy]*/
  num_keyword += (nx + 1)*(ny + 1); 
  /* Connection Boxes */
  /* cbx[ix][iy] */
  num_keyword += (ny+1) * nx;
  /* cby[ix][iy] */
  num_keyword += (nx+1) * ny;

  /* internal names: inv, buf, cpt, vpr_nmos, vpr_pmos, wire_segments */
  num_keyword += 5 + Arch.num_segments;

  /* Include keywords of global ports */
  temp = global_ports_head;
  while (NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr);
    num_keyword += cur_global_port->size;
    temp = temp->next;
  }

  /* Malloc */
  keywords = (char**)my_malloc(sizeof(char*)*num_keyword);
  
  /* Add reserved names to the list */
  cur = 0;
  for (i = 0; i < num_keyword; i++) {
    keywords[i] = NULL;
  }
  /* Include keywords of global ports */
  temp = global_ports_head;
  while (NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr);
    for (iport = 0; iport < cur_global_port->size; iport++) { 
      keywords[cur] = (char*)my_malloc(sizeof(char)*
                                      (strlen(cur_global_port->prefix) + 2 + strlen(my_itoa(iport)) + 1));
      sprintf(keywords[cur], "%s[%d]", cur_global_port->prefix, iport);
      cur++;
    }
    temp = temp->next;
  }
  /* internal names: inv, buf, cpt, vpr_nmos, vpr_pmos, wire_segments */
  keywords[cur] = "inv"; cur++;
  keywords[cur] = "buf"; cur++;
  keywords[cur] = "cpt"; cur++;
  keywords[cur] = "vpr_nmos"; cur++;
  keywords[cur] = "vpr_pmos"; cur++;
  for (iseg = 0; iseg < Arch.num_segments; iseg++) {
    keywords[cur] = (char*)my_malloc(sizeof(char)*
                                    (strlen(Arch.Segments[iseg].spice_model->name) + 4 + strlen(my_itoa(iseg)) + 1));
    sprintf(keywords[cur], "%s_seg%d", Arch.Segments[iseg].spice_model->name, iseg);
    cur++;
  }
  /* Reserved names: switch boxes, connection boxes, channels */
  /* Channels -X */
  for (iy = 0; iy < (ny + 1); iy++) { 
    for (ix = 1; ix < (nx + 1); ix++) { 
      /* chanx[ix][iy]*/
      keywords[cur] = (char*)my_malloc(sizeof(char)* (6 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
      sprintf(keywords[cur], "chanx[%d][%d]", ix, iy);
      cur++;
    }
  }
  /* Channels -Y */
  for (ix = 0; ix < (nx + 1); ix++) { 
    for (iy = 1; iy < (ny + 1); iy++) { 
      /* chany[ix][iy]*/
      keywords[cur] = (char*)my_malloc(sizeof(char)* (6 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
      sprintf(keywords[cur], "chany[%d][%d]", ix, iy);
      cur++;
    }
  }
  /* Connection Box */
  /* cbx[ix][iy]*/
  for (iy = 0; iy < (ny + 1); iy++) { 
    for (ix = 1; ix < (nx + 1); ix++) { 
      /* cbx[ix][iy]*/
      keywords[cur] = (char*)my_malloc(sizeof(char)* (4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
      sprintf(keywords[cur], "cbx[%d][%d]", ix, iy);
      cur++;
    }
  }
  /* cby[ix][iy]*/
  for (ix = 0; ix < (nx + 1); ix++) { 
    for (iy = 1; iy < (ny + 1); iy++) { 
      /* cby[ix][iy]*/
      keywords[cur] = (char*)my_malloc(sizeof(char)* (4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
      sprintf(keywords[cur], "cby[%d][%d]", ix, iy);
      cur++;
    }
  }
  /* Switch Boxes */
  for (ix = 0; ix < (nx + 1); ix++) { 
    for (iy = 0; iy < (ny + 1); iy++) { 
      /* sb[ix][iy]*/
      keywords[cur] = (char*)my_malloc(sizeof(char)* (3 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
      sprintf(keywords[cur], "sb[%d][%d]", ix, iy);
      cur++;
    }
  }
  /* Reserved names: grid names */
  /* Reserved names: pb_type names */
  for (ix = 0; ix < (nx + 2); ix++) { 
    for (iy = 0; iy < (ny + 2); iy++) { 
      /* by_pass the empty */
      if (EMPTY_TYPE != grid[ix][iy].type) {
        prefix = (char*)my_malloc(sizeof(char)* (5 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 2));
        sprintf(prefix, "grid[%d][%d]", ix, iy);
        /* plus grid[ix][iy]*/ 
        keywords[cur] = my_strdup(prefix); 
        cur++; 
        for (iz = 0; iz < grid[ix][iy].type->capacity; iz++) {
          /* Per grid, type_descriptor.name[i]*/
          /* Go recursive pb_graph_node, until the leaf which defines a spice_model */
          cur_pb_type = grid[ix][iy].type->pb_type;
          rec_add_pb_type_keywords_to_list(cur_pb_type, &cur, keywords, prefix);
        }
        my_free(prefix);
      }
    }
  }
  /* assert */
  assert(cur == num_keyword);
  
  /* Check the keywords conflicted with defined spice_model names */
  for (imodel = 0; imodel < Arch.spice->num_spice_model; imodel++) {
    for (i = 0; i < num_keyword; i++) {
      if (0 == strcmp(Arch.spice->spice_models[imodel].name, keywords[i])) {
        vpr_printf(TIO_MESSAGE_ERROR, "Keyword Conflicted! Spice Model Name: %s\n", keywords[i]);
        conflict++;
      }
    }
  } 

  assert((0 == conflict)||(0 < conflict));
  if (0 < conflict) {
    vpr_printf(TIO_MESSAGE_ERROR, "Found %d conflicted keywords!\n", conflict);
    exit(1);
  }
  
  return; 
}

/* Need to check if we already a global port with the same name in the list! 
 * This could happen where two spice models share the same global port   
 * If this is a new name in the list, we add this global port.
 * Otherwise, we do nothing 
 */
static 
t_llist* check_and_add_one_global_port_to_llist(t_llist* old_head, 
                                                t_spice_model_port* candidate_port) {
  t_llist* temp = old_head;
  t_llist* new_head = NULL;

  while (NULL != temp) {
    if (0 == strcmp(candidate_port->prefix, 
                    ((t_spice_model_port*)(temp->dptr))->prefix) ) { 
      /* Find a same global port name, we do nothing, return directly */
      return old_head;
    }
    /* Go to the next */
    temp = temp->next;
  }

  new_head = insert_llist_node_before_head(old_head);
  new_head->dptr = (void*)(candidate_port);

  return new_head;
}

/* Create and Initialize the global ports 
 * Search all the ports defined under spice_models
 * if a port is defined to be global, we add its pointer to the linked list
 */
static 
t_llist* init_llist_global_ports(t_spice* spice) {
  int imodel, iport;
  t_llist* head = NULL;

  /* Traverse all the spice models */
  for (imodel = 0; imodel < spice->num_spice_model; imodel++) {
    for (iport = 0; iport < spice->spice_models[imodel].num_port; iport++) {
      if (TRUE == spice->spice_models[imodel].ports[iport].is_global) {
        /* Check each global signal has non conflicted flags : 
         * At most one of the properties: is_config_enable, is_set and is_reset, can be true */
        assert(2 > (spice->spice_models[imodel].ports[iport].is_set
                   + spice->spice_models[imodel].ports[iport].is_reset
                   + spice->spice_models[imodel].ports[iport].is_config_enable));
        /* Add to linked list, the organization will be first-in last-out 
         * First element would be the tail of linked list  
         */
        head = check_and_add_one_global_port_to_llist(head,&(spice->spice_models[imodel].ports[iport]));
      }
    } 
  }

  return head;
}

/* Check how many conflicts of syntax char in a string */
static 
int check_conflict_syntax_char_in_string(t_llist* LL_reserved_syntax_char_head,
                                         char* str_to_check) {
  int num_conflicts = 0;
  int len_str_to_check = strlen(str_to_check);
  int ichar = 0;

  for (ichar = 0; ichar < len_str_to_check; ichar++) {
    if (TRUE == is_verilog_and_spice_syntax_conflict_char(LL_reserved_syntax_char_head, 
                                                          str_to_check[ichar])) {
      /* Print warning */
      vpr_printf(TIO_MESSAGE_ERROR, "String (%s) contains conflicted chars[%c] which is not allowed by Verilog and SPICE!\n",
                                    str_to_check, str_to_check[ichar]);
      num_conflicts++; 
    }
  }

  return num_conflicts;
}

/* Check if each spice_model name contains any syntax char, which is reseved by SPICE or Verilog */
static 
void check_spice_model_name_conflict_syntax_char(t_arch Arch,
                                                 t_llist* LL_reserved_syntax_char_head) {
  int imodel, iport;
  int num_conflicts = 0;

  /* Check spice_model one by one */
  for (imodel = 0; imodel < Arch.spice->num_spice_model; imodel++) {
    /* Check spice_model->name */
    num_conflicts += check_conflict_syntax_char_in_string(LL_reserved_syntax_char_head,
                                                          Arch.spice->spice_models[imodel].name);
    /* Check spice_model->prefix */
    num_conflicts += check_conflict_syntax_char_in_string(LL_reserved_syntax_char_head,
                                                          Arch.spice->spice_models[imodel].prefix);
    /* Check each port name */
    for (iport = 0; iport < Arch.spice->spice_models[imodel].num_port; iport++) {
      num_conflicts += check_conflict_syntax_char_in_string(LL_reserved_syntax_char_head,
                                                            Arch.spice->spice_models[imodel].ports[iport].prefix);
    }
  }

  if (0 < num_conflicts) {
    /* Print warning */
    vpr_printf(TIO_MESSAGE_ERROR, "Fail in syntax char checking, conflicts have been detected!\n");
    exit(1);
  }

  return;
}

/* Initialize a linked-list for syntax char of Verilog and SPICE */
static 
t_llist* init_llist_verilog_and_spice_syntax_char() {
  t_llist* new_head = NULL;
  int num_syntax_chars = 0;
  char* syntax_chars = NULL;
  t_reserved_syntax_char* new_syntax_char = NULL;
  int ichar = 0;
  
  syntax_chars = my_strdup(".,:;\'\"+-<>()[]{}!@#$%^&*~`?/");
  num_syntax_chars = strlen(syntax_chars);

  /* Create a new element */
  for (ichar = 0; ichar < num_syntax_chars; ichar++) { 
    new_syntax_char = (t_reserved_syntax_char*)(my_malloc(sizeof(t_reserved_syntax_char)));
    new_head = insert_llist_node_before_head(new_head);
    new_head->dptr = (void*)new_syntax_char;
    init_reserved_syntax_char(new_syntax_char, syntax_chars[ichar], TRUE, TRUE);
  }

  return new_head;
}

/* Check if a char violates the syntax of Verilog and SPICE */
static 
boolean is_verilog_and_spice_syntax_conflict_char(t_llist* LL_reserved_syntax_char_head, 
                                                  char ref_char) {
  boolean syntax_conflict = FALSE;
  t_llist* temp = LL_reserved_syntax_char_head;
  t_reserved_syntax_char* cur_syntax_char = NULL;

  /* Search the conflict linked list ? */
  while (NULL != temp) {
    cur_syntax_char = (t_reserved_syntax_char*)(temp->dptr);
    if (ref_char == cur_syntax_char->syntax_char) {
      syntax_conflict = TRUE;
      break;
    } 
    /* go to the next */
    temp = temp->next;
  }
 
  return syntax_conflict;
}

/* Check and rename the name of each IO logical block
 * if the current name violates syntax of SPICE or Verilog 
 */
static 
int check_and_rename_logical_block_and_net_names(t_llist* LL_reserved_syntax_char_head, 
                                                 char* circuit_name,
                                                 boolean rename_illegal_port,
                                                 int LL_num_logical_blocks, t_logical_block* LL_logical_block,
                                                 int LL_num_clb_nets, t_net* LL_clb_net,
                                                 int LL_num_vpack_nets, t_net* LL_vpack_net) {
  FILE* fp = NULL;
  int iblock, inet, ichar, name_str_len, num_violations;
  char renamed_char = '_';
  boolean io_renamed = FALSE;
  boolean io_violate_syntax = FALSE;
  char* temp_io_name = NULL;
  char* renaming_report_file_path = NULL;

  vpr_printf(TIO_MESSAGE_INFO, "Check IO pad names, to avoid violate SPICE or Verilog Syntax...\n");

  num_violations = 0;

  /* Check if the path exists*/
  renaming_report_file_path = my_strcat(circuit_name, renaming_report_postfix); 
  fp = fopen(renaming_report_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create renaming report %s!",
               __FILE__, __LINE__, renaming_report_file_path); 
    exit(1);
  } 

  fprintf(fp, "------- Logical block renaming report BEGIN ----------\n");
  for (iblock = 0; iblock < LL_num_logical_blocks; iblock++) {
    /* Bypass non-IO logical blocks */
    /*
    if ((VPACK_INPAD != logical_block[iblock].type)&&(VPACK_OUTPAD != logical_block[iblock].type)) {
      continue;
    }
    */
    /* initialize the flag */
    io_renamed = FALSE;
    io_violate_syntax = FALSE;
    /* Keep a copy of previous name */
    temp_io_name = my_strdup(LL_logical_block[iblock].name);
    /* Check names character by charcter */
    name_str_len = strlen(LL_logical_block[iblock].name); /* exclude the last character: \0, which does require to be checked */
    for (ichar = 0; ichar < name_str_len; ichar++) {
      /* Check syntax senstive list, if violates, rename it to be '_' */
      if (TRUE == is_verilog_and_spice_syntax_conflict_char(LL_reserved_syntax_char_head, LL_logical_block[iblock].name[ichar])) {
        num_violations++;
        io_violate_syntax = TRUE;
        if ( TRUE == rename_illegal_port) {
          LL_logical_block[iblock].name[ichar] = renamed_char;
          io_renamed = TRUE;
        }
      }
    } 
    /* Print a warning if */
    if (TRUE == io_renamed) {
      fprintf(fp, "[RENAMING%d] Logical block (Name: %s) is renamed to %s\n",
                  num_violations,
                  temp_io_name, LL_logical_block[iblock].name);
    } else if (TRUE == io_violate_syntax) {
      fprintf(fp, "[RENAMING%d] Logical block name %s violates syntax rules \n",
                  num_violations,
                  temp_io_name);
    }
    /* Free */
    my_free(temp_io_name);
  } 

  fprintf(fp, "-------Logical block renaming report END ----------\n\n");

  fprintf(fp, "-------CLB_NET renaming report BEGIN ----------\n");
  /* Change the net name in the clb_net and vpack_net info as well !!! */
  for (inet = 0; inet < LL_num_clb_nets; inet++) {
    /* initialize the flag */
    io_renamed = FALSE;
    io_violate_syntax = FALSE;
    /* Keep a copy of previous name */
    temp_io_name = my_strdup(LL_clb_net[inet].name);
    /* Check names character by charcter */
    name_str_len = strlen(LL_clb_net[inet].name); /* exclude the last character: \0, which does require to be checked */
    for (ichar = 0; ichar < name_str_len; ichar++) {
      /* Check syntax senstive list, if violates, rename it to be '_' */
      if (TRUE == is_verilog_and_spice_syntax_conflict_char(LL_reserved_syntax_char_head, LL_clb_net[inet].name[ichar])) {
        num_violations++;
        io_violate_syntax = TRUE;
        if ( TRUE == rename_illegal_port) {
          LL_clb_net[inet].name[ichar] = renamed_char;
          io_renamed = TRUE;
        }
      }
    } 
    /* Print a warning if */
    if (TRUE == io_renamed) {
      fprintf(fp, "[RENAMING%d] clb_net (Name: %s) is renamed to %s\n",
                  num_violations,
                  temp_io_name, LL_clb_net[inet].name);
    } else if (TRUE == io_violate_syntax) {
      fprintf(fp, "[RENAMING%d] clb_net name %s violates syntax rules \n",
                  num_violations,
                  temp_io_name);
    }
    /* Free */
    my_free(temp_io_name);
  } 

  fprintf(fp, "-------CLB_NET renaming report END ----------\n\n");

  fprintf(fp, "-------VPACK_NET renaming report BEGIN ----------\n");

  for (inet = 0; inet < LL_num_vpack_nets; inet++) {
    /* initialize the flag */
    io_renamed = FALSE;
    io_violate_syntax = FALSE;
    /* Keep a copy of previous name */
    temp_io_name = my_strdup(LL_vpack_net[inet].name);
    /* Check names character by charcter */
    name_str_len = strlen(LL_vpack_net[inet].name); /* exclude the last character: \0, which does require to be checked */
    for (ichar = 0; ichar < name_str_len; ichar++) {
      /* Check syntax senstive list, if violates, rename it to be '_' */
      if (TRUE == is_verilog_and_spice_syntax_conflict_char(LL_reserved_syntax_char_head, LL_vpack_net[inet].name[ichar])) {
        num_violations++;
        io_violate_syntax = TRUE;
        if ( TRUE == rename_illegal_port) {
          LL_vpack_net[inet].name[ichar] = renamed_char;
          io_renamed = TRUE;
        }
      }
    } 
    /* Print a warning if */
    if (TRUE == io_renamed) {
      fprintf(fp, "[RENAMING%d] vpack_net (Name: %s) is renamed to %s\n",
                  num_violations,
                  temp_io_name, LL_vpack_net[inet].name);
    } else if (TRUE == io_violate_syntax) {
      fprintf(fp, "[RENAMING%d] vpack_net name %s violates syntax rules \n",
                  num_violations,
                  temp_io_name);
    }
    /* Free */
    my_free(temp_io_name);
  } 

  fprintf(fp, "-------VPACK_NET renaming report END ----------\n");

  if ((0 < num_violations) && ( FALSE == rename_illegal_port )) {
    vpr_printf(TIO_MESSAGE_WARNING, "Detect %d port violate syntax rules while renaming port is disabled\n", num_violations);
  }

  /* close fp */
  fclose(fp);
 
  vpr_printf(TIO_MESSAGE_INFO, "Renaming report is generated in %s\n",
                               renaming_report_file_path); 

  return num_violations;
}


static 
void spice_net_info_add_density_weight(float signal_density_weight) {
  int inet;

  /* a weight of 1. means no change. directly return */
  if ( 1. == signal_density_weight ) {
    return;
  }

  for (inet = 0; inet < num_logical_nets; inet++) {
    assert( NULL != vpack_net[inet].spice_net_info );
    /* By pass PIs since their signal density is usually high */
    if ( TRUE == is_net_pi(&(vpack_net[inet])) ) {
      continue;
    }
    vpack_net[inet].spice_net_info->density *= signal_density_weight;
  }

  for (inet = 0; inet < num_nets; inet++) {
    assert( NULL != clb_net[inet].spice_net_info );
    /* By pass PIs since their signal density is usually high */
    if ( TRUE == is_net_pi(&(vpack_net[clb_to_vpack_net_mapping[inet]])) ) {
      continue;
    }
    clb_net[inet].spice_net_info->density *= signal_density_weight;
  }
}

void fpga_x2p_free(t_arch* Arch) {
  /* Free index low and high */
  free_spice_model_grid_index_low_high(Arch->spice->num_spice_model, Arch->spice->spice_models);
  free_spice_model_routing_index_low_high(Arch->spice->num_spice_model, Arch->spice->spice_models);
}

/* Top-level function of FPGA-SPICE setup */
void fpga_x2p_setup(t_vpr_setup vpr_setup,
                    t_arch* Arch) {
  int num_rename_violation = 0;
  int num_clocks = 0;
  float vpr_crit_path_delay = 0.; 
  float vpr_clock_freq = 0.; 
  float vpr_clock_period = 0.; 


  vpr_printf(TIO_MESSAGE_INFO, "\nFPGA-SPICE Tool suites Initilization begins...\n"); 
  
  /* Initialize Arch SPICE MODELS*/
  init_check_arch_spice_models(Arch, &(vpr_setup.RoutingArch));

  /* Initialize idle mode and physical mode of each pb_type and pb_graph_node */
  init_check_arch_pb_type_idle_and_phy_mode(Arch);

  /* Create and initialize a linked list for global ports */
  global_ports_head = init_llist_global_ports(Arch->spice);
  vpr_printf(TIO_MESSAGE_INFO, "Detect %d global ports...\n", 
             find_length_llist(global_ports_head) );

  /* Build llist for verilog and spice syntax char  */
  vpr_printf(TIO_MESSAGE_INFO, "Initialize reserved Verilog and SPICE syntax chars...\n"); 
  reserved_syntax_char_head = init_llist_verilog_and_spice_syntax_char();

  /* Initialize verilog netlist to be included */
  /* Add keyword checking */
  check_keywords_conflict(*Arch);
  /* TODO: check spice_model names conflict with SPICE or Verilog syntax */
  vpr_printf(TIO_MESSAGE_INFO, "Checking spice_model compatible with syntax chars...\n"); 
  check_spice_model_name_conflict_syntax_char(*Arch, 
                                              reserved_syntax_char_head);


  /* Check and rename io names to avoid violating SPICE or Verilog syntax 
   * Only valid when Verilog generator or SPICE generator is enabled
   */
  num_rename_violation = 
  check_and_rename_logical_block_and_net_names(reserved_syntax_char_head, 
                                               vpr_setup.FileNameOpts.CircuitName,
                                               vpr_setup.FPGA_SPICE_Opts.rename_illegal_port,
                                               num_logical_blocks, logical_block,
                                               num_nets, clb_net,
                                               num_logical_nets, vpack_net);
  /* Violation is not allowed for SPICE and Verilog Generator! */
  if (((0 < num_rename_violation) && (FALSE == vpr_setup.FPGA_SPICE_Opts.rename_illegal_port))
     && ((TRUE == vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice) 
     || (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog))) {
    vpr_printf(TIO_MESSAGE_ERROR, "Port name syntax violations is not allowed for SPICE and Verilog Generators!\n");
    exit(1);
  }

  /* Update global options: 
   * 1. run_parasitic_net_estimation
   * 2. run_testbench_load_extraction 
   */
  run_parasitic_net_estimation = TRUE;
  if (FALSE == vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_parasitic_net_estimation) {
    run_parasitic_net_estimation = FALSE;
  }
  
  run_testbench_load_extraction = TRUE;
  if (FALSE == vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_testbench_load_extraction) {
    run_testbench_load_extraction = FALSE;
    vpr_printf(TIO_MESSAGE_WARNING, "SPICE testbench load extraction is turned off...Accuracy loss may be expected!\n");
  }

  /* Check Activity file is valid */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.read_act_file) {
    if (1 == try_access_file(vpr_setup.FileNameOpts.ActFile)) {
      vpr_printf(TIO_MESSAGE_ERROR,"Activity file (%s) does not exists! Please provide a valid file path!\n",
                 vpr_setup.FileNameOpts.ActFile);
      exit(1);
    } else {
      vpr_printf(TIO_MESSAGE_INFO,"Check Activity file (%s) is a valid file path!\n",
                 vpr_setup.FileNameOpts.ActFile);
    }
  }

  /* Backannotation for post routing information */
  spice_backannotate_vpr_post_route_info(vpr_setup.RoutingArch,
                                         vpr_setup.FPGA_SPICE_Opts.read_act_file,
                                         vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_parasitic_net_estimation);

  /* Try to use mirror SBs/CBs if enabled by user */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy) {
    /* Idenify mirror and rotatable Switch blocks and Connection blocks */
    identify_mirror_switch_blocks();
    identify_mirror_connection_blocks();
    /* Rotatable will be done in the next step 
    identify_rotatable_switch_blocks(); 
    identify_rotatable_connection_blocks(); 
    */ 
  }

  /* Not should be done when read_act_file is disabled */
  if (FALSE == vpr_setup.FPGA_SPICE_Opts.read_act_file) {
    return;
  }

  /* Auto check the density and recommend sim_num_clock_cylce */
  vpr_crit_path_delay = get_critical_path_delay()/1e9;
  assert(vpr_crit_path_delay > 0.);
  /* if we don't have global clock, clock_freqency should be set to 0.*/
  num_clocks = count_netlist_clocks();
  if (0 == num_clocks) {
     /* This could a combinational circuit */
     vpr_clock_freq = 1. / vpr_crit_path_delay; 
  } else { 
    assert(1 == num_clocks);
    vpr_clock_freq = 1. / vpr_crit_path_delay; 
  }
  Arch->spice->spice_params.stimulate_params.num_clocks = num_clocks;
  Arch->spice->spice_params.stimulate_params.vpr_crit_path_delay = vpr_crit_path_delay;
  vpr_clock_period = 1./vpr_clock_freq;
  auto_select_num_sim_clock_cycle(Arch->spice, vpr_setup.FPGA_SPICE_Opts.sim_window_size);

 /* Determine the clock period */
  if (OPEN == Arch->spice->spice_params.stimulate_params.op_clock_freq) {
    /* warning the negative slack ! TODO: move to the general check part??? */
    if (0. > Arch->spice->spice_params.stimulate_params.sim_clock_freq_slack) {
      assert(0. < (1 + Arch->spice->spice_params.stimulate_params.sim_clock_freq_slack));
      vpr_printf(TIO_MESSAGE_WARNING, "Slack for clock frequency(=%g) is less than 0! The simulation may fail!\n",
                 Arch->spice->spice_params.stimulate_params.sim_clock_freq_slack);
    }
    Arch->spice->spice_params.stimulate_params.op_clock_freq = 1./(vpr_clock_period *(1. + Arch->spice->spice_params.stimulate_params.sim_clock_freq_slack));
  } else {
    /* Simulate clock frequency should be larger than 0 !*/
    assert(0. < Arch->spice->spice_params.stimulate_params.op_clock_freq);
  } 
  vpr_printf(TIO_MESSAGE_INFO, "Use Operation Clock freqency %.2f [MHz] in SPICE simulation.\n",
             Arch->spice->spice_params.stimulate_params.op_clock_freq / 1e6);
  vpr_printf(TIO_MESSAGE_INFO, "Use Programming Clock freqency %.2f [MHz] in SPICE simulation.\n",
             Arch->spice->spice_params.stimulate_params.prog_clock_freq / 1e6);

  /* Add weights to spice_net density */ 
  if (!(0 < vpr_setup.FPGA_SPICE_Opts.signal_density_weight)) {
    vpr_printf(TIO_MESSAGE_ERROR, "Signal_density_weight(currently is %.2f) should be a positive number!.\n",
               vpr_setup.FPGA_SPICE_Opts.signal_density_weight); 
    exit(1);
  }
  if (1 != vpr_setup.FPGA_SPICE_Opts.signal_density_weight) {
    vpr_printf(TIO_MESSAGE_INFO, "Add %.2f weight to signal density...\n", 
               vpr_setup.FPGA_SPICE_Opts.signal_density_weight); 
    spice_net_info_add_density_weight(vpr_setup.FPGA_SPICE_Opts.signal_density_weight);
  }

  return;
}
 
