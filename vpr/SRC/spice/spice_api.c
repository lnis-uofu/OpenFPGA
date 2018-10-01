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

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_backannotate_utils.h"
#include "spice_subckt.h"
#include "spice_pbtypes.h"
#include "spice_heads.h"
#include "spice_lut.h"
#include "spice_top_netlist.h"
#include "spice_mux_testbench.h"
#include "spice_grid_testbench.h"
#include "spice_lut_testbench.h"
#include "spice_dff_testbench.h"
#include "spice_run_scripts.h"

/* For mrFPGA */
#ifdef MRFPGA_H
#include "mrfpga_globals.h"
#endif

/* RUN HSPICE Shell Script Name */
static char* default_spice_dir_path = "spice_netlists/";
static char* spice_top_tb_dir_name = "top_tb/";
static char* spice_grid_tb_dir_name = "grid_tb/";
static char* spice_pb_mux_tb_dir_name = "pb_mux_tb/";
static char* spice_cb_mux_tb_dir_name = "cb_mux_tb/";
static char* spice_sb_mux_tb_dir_name = "sb_mux_tb/";
static char* spice_lut_tb_dir_name = "lut_tb/";
static char* spice_dff_tb_dir_name = "dff_tb/";
  
/***** Subroutines Declarations *****/
void init_list_include_netlists(t_spice* spice); 

static 
void rec_stat_pb_type_keywords(t_pb_type* cur_pb_type,
                               int* num_keyword);

static 
void rec_add_pb_type_keywords_to_list(t_pb_type* cur_pb_type,
                                      int* cur,
                                      char** keywords,
                                      char* prefix);

void check_keywords_conflict(t_arch Arch);

/***** Subroutines *****/
/* Initialize and check spice models in architecture
 * Tasks:
 * 1. Link the spice model defined in pb_types and routing switches
 * 2. Add default spice model (MUX) if needed
 */
void init_check_arch_spice_models(t_arch* arch,
                                  t_det_routing_arch* routing_arch) {
  int i;

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
    if (FALSE == check_spice_model_structure_match_switch_inf(arch->cb_switches[i])) {
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
  if (NULL == arch->sram_inf.spice_model_name) { 
      arch->sram_inf.spice_model = get_default_spice_model(SPICE_MODEL_SRAM,
                                                           arch->spice->num_spice_model, 
                                                           arch->spice->spice_models);
  } else {
    arch->sram_inf.spice_model = 
        find_name_matched_spice_model(arch->sram_inf.spice_model_name,
                                      arch->spice->num_spice_model, 
                                      arch->spice->spice_models); 
  }
  if (NULL == arch->sram_inf.spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of SRAM is undefined in SPICE models!\n",__FILE__ ,__LINE__, arch->Switches[i].spice_model_name, arch->Switches[i].name);
    exit(1);
  }
  /* Find the sram model and assign the global variable*/
  sram_spice_model = arch->sram_inf.spice_model;

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
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model name(%s) of Segment(Length:%d) is undefined in SPICE models!\n",__FILE__ ,__LINE__, arch->Segments[i].spice_model_name, arch->Segments[i].length);
      exit(1);
    } else if (SPICE_MODEL_CHAN_WIRE != arch->Segments[i].spice_model->type) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d])Invalid SPICE model(%s) type of Segment(Length:%d)! Should be chan_wire!\n",__FILE__ ,__LINE__, arch->Segments[i].spice_model_name, arch->Segments[i].length);
      exit(1);
    }
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
  }

  return;
}

void init_list_include_netlists(t_spice* spice) { 
  int i, j, cur;
  int to_include = 0;
  int num_to_include = 0;

  /* Initialize */
  for (i = 0; i < spice->num_include_netlist; i++) { 
    FreeSpiceModelNetlist(&(spice->include_netlists[i]));
  }
  my_free(spice->include_netlists);
  spice->include_netlists = NULL;
  spice->num_include_netlist = 0;

  /* Generate include netlist list */
  vpr_printf(TIO_MESSAGE_INFO, "Listing Netlist Names to be included...\n");
  for (i = 0; i < spice->num_spice_model; i++) {
    if (NULL != spice->spice_models[i].model_netlist) {
      /* Check if this netlist name has already existed in the list */
      to_include = 1;
      for (j = 0; j < i; j++) {
        if (NULL == spice->spice_models[j].model_netlist) {
          continue;
        }
        if (0 == strcmp(spice->spice_models[j].model_netlist, spice->spice_models[i].model_netlist)) {
          to_include = 0;
          break;
        }
      }
      /* Increamental */
      if (1 == to_include) {
        num_to_include++;
      }
    }
  }

  /* realloc */
  spice->include_netlists = (t_spice_model_netlist*)my_realloc(spice->include_netlists, 
                              sizeof(t_spice_model_netlist)*(num_to_include + spice->num_include_netlist));

  /* Fill the new included netlists */
  cur = spice->num_include_netlist;
  for (i = 0; i < spice->num_spice_model; i++) {
    if (NULL != spice->spice_models[i].model_netlist) {
      /* Check if this netlist name has already existed in the list */
      to_include = 1;
      for (j = 0; j < i; j++) {
        if (NULL == spice->spice_models[j].model_netlist) {
          continue;
        }
        if (0 == strcmp(spice->spice_models[j].model_netlist, spice->spice_models[i].model_netlist)) {
          to_include = 0;
          break;
        }
      }
      /* Increamental */
      if (1 == to_include) {
        spice->include_netlists[cur].path = my_strdup(spice->spice_models[i].model_netlist); 
        spice->include_netlists[cur].included = 0;
        vpr_printf(TIO_MESSAGE_INFO, "[%d] %s\n", cur+1, spice->include_netlists[cur].path);
        cur++;
      }
    }
  }
  /* Check */
  assert(cur == (num_to_include + spice->num_include_netlist));
  /* Update */
  spice->num_include_netlist += num_to_include;
  
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
          my_free(pass_on_prefix);
        }
      }
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
  int cur, iseg, imodel, i;
  int ix, iy, iz;
  t_pb_type* cur_pb_type = NULL;
  char* prefix = NULL;

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

  /* Malloc */
  keywords = (char**)my_malloc(sizeof(char*)*num_keyword);
  
  /* Add reserved names to the list */
  cur = 0;
  for (i = 0; i < num_keyword; i++) {
    keywords[i] = NULL;
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

static 
void free_spice_tb_llist() {
  t_llist* temp = tb_head;

  while (temp) {
    my_free(((t_spicetb_info*)(temp->dptr))->tb_name);
    my_free(temp->dptr);
    temp->dptr = NULL;
    temp = temp->next;
  }
  free_llist(tb_head);

  return;
}

/***** Main Function *****/
void vpr_print_spice_netlists(t_vpr_setup vpr_setup,
                              t_arch Arch,
                              char* circuit_name) {
  clock_t t_start;
  clock_t t_end;
  float run_time_sec;

  int num_clocks = 0;
  float vpr_crit_path_delay = 0.; 
  float vpr_clock_freq = 0.; 

  char* spice_dir_formatted = NULL;
  char* include_dir_path = NULL;
  char* subckt_dir_path = NULL;
  char* top_netlist_path = NULL;
  char* include_dir_name = vpr_setup.SpiceOpts.include_dir;
  char* subckt_dir_name = vpr_setup.SpiceOpts.subckt_dir;
  char* chomped_circuit_name = NULL;
  char* chomped_spice_dir = NULL;
  char* top_testbench_dir_path = NULL;
  char* pb_mux_testbench_dir_path = NULL;
  char* cb_mux_testbench_dir_path = NULL;
  char* sb_mux_testbench_dir_path = NULL;
  char* grid_testbench_dir_path = NULL;
  char* lut_testbench_dir_path = NULL;
  char* dff_testbench_dir_path = NULL;
  char* top_testbench_file = NULL;

  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA SPICE netlists only support uni-directional routing architecture!\n");
    exit(1);
  }
  
  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA SPICE netlists do not support mrFPGA!\n");
    exit(1);
  }
#endif  

  /* Initial Arch SPICE MODELS*/
  init_check_arch_spice_models(&Arch, &vpr_setup.RoutingArch);
  init_list_include_netlists(Arch.spice); 

  /* Add keyword checking */
  check_keywords_conflict(Arch);

  /*Process the circuit name*/
  split_path_prog_name(circuit_name,'/',&chomped_spice_dir ,&chomped_circuit_name);
  
  /* Start Clocking*/
  t_start = clock();

  /* Format the directory path */
  if (NULL != vpr_setup.SpiceOpts.spice_dir) {
    spice_dir_formatted = format_dir_path(vpr_setup.SpiceOpts.spice_dir);
  } else {
    spice_dir_formatted = format_dir_path(my_strcat(format_dir_path(chomped_spice_dir),default_spice_dir_path));
  }

  /*Initial directory organization*/
  /* Process include directory */
  (include_dir_path) = my_strcat(spice_dir_formatted,include_dir_name); 
  /* Process subckt directory */
  (subckt_dir_path) = my_strcat(spice_dir_formatted,subckt_dir_name);

  /* Check the spice folders exists if not we create it.*/
  create_dir_path(spice_dir_formatted);
  create_dir_path(include_dir_path);
  create_dir_path(subckt_dir_path);

  /* determine the VPR clock frequency */
  vpr_crit_path_delay = get_critical_path_delay()/1e9;
  assert(vpr_crit_path_delay > 0.);
  /* if we don't have global clock, clock_freqency should be set to 0.*/
  num_clocks = count_netlist_clocks();
  if (0 == num_clocks) {
     vpr_clock_freq = 0.;
  } else { 
    assert(1 == num_clocks);
    vpr_clock_freq = 1. / vpr_crit_path_delay; 
  }

  /* backannotation */  
  spice_backannotate_vpr_post_route_info(vpr_setup.SpiceOpts.fpga_spice_parasitic_net_estimation_off);

  /* Auto check the density and recommend sim_num_clock_cylce */
  auto_select_num_sim_clock_cycle(Arch.spice);

  /* Generate Header files */
  fprint_spice_headers(include_dir_path, vpr_crit_path_delay, num_clocks, *(Arch.spice));

  /* Generate sub circuits: Inverter, Buffer, Transmission Gate, LUT, DFF, SRAM, MUX*/
  generate_spice_subckts(subckt_dir_path, &Arch ,&vpr_setup.RoutingArch);

  /* Print MUX testbench if needed */
  if (vpr_setup.SpiceOpts.print_spice_pb_mux_testbench) {
    pb_mux_testbench_dir_path = my_strcat(spice_dir_formatted, spice_pb_mux_tb_dir_name);
    create_dir_path(pb_mux_testbench_dir_path);
    fprint_spice_mux_testbench(pb_mux_testbench_dir_path, chomped_circuit_name, 
                               include_dir_path, subckt_dir_path,
                               rr_node_indices, num_clocks, Arch, 
                               SPICE_PB_MUX_TB, 
                               vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  if (vpr_setup.SpiceOpts.print_spice_cb_mux_testbench) {
    cb_mux_testbench_dir_path = my_strcat(spice_dir_formatted, spice_cb_mux_tb_dir_name);
    create_dir_path(cb_mux_testbench_dir_path);
    fprint_spice_mux_testbench(cb_mux_testbench_dir_path, chomped_circuit_name,
                               include_dir_path, subckt_dir_path,
                               rr_node_indices, num_clocks, Arch, SPICE_CB_MUX_TB, 
                               vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  if (vpr_setup.SpiceOpts.print_spice_sb_mux_testbench) {
    sb_mux_testbench_dir_path = my_strcat(spice_dir_formatted, spice_sb_mux_tb_dir_name);
    create_dir_path(sb_mux_testbench_dir_path);
    fprint_spice_mux_testbench(sb_mux_testbench_dir_path, chomped_circuit_name, 
                               include_dir_path, subckt_dir_path,
                               rr_node_indices, num_clocks, Arch, SPICE_SB_MUX_TB, 
                               vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  if (vpr_setup.SpiceOpts.print_spice_lut_testbench) {
    lut_testbench_dir_path = my_strcat(spice_dir_formatted, spice_lut_tb_dir_name); 
    create_dir_path(lut_testbench_dir_path);
    fprint_spice_lut_testbench(lut_testbench_dir_path, chomped_circuit_name, include_dir_path, subckt_dir_path,
                               rr_node_indices, num_clocks, Arch, vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  /* By pass dff testbench file if there is no clock */
  if (vpr_setup.SpiceOpts.print_spice_dff_testbench) {
    dff_testbench_dir_path = my_strcat(spice_dir_formatted, spice_dff_tb_dir_name); 
    create_dir_path(dff_testbench_dir_path);
    fprint_spice_dff_testbench(dff_testbench_dir_path, chomped_circuit_name, include_dir_path, subckt_dir_path,
                               rr_node_indices, num_clocks, Arch, vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  /* Print Grid testbench if needed */
  if (vpr_setup.SpiceOpts.print_spice_grid_testbench) {
    grid_testbench_dir_path = my_strcat(spice_dir_formatted, spice_grid_tb_dir_name);
    create_dir_path(grid_testbench_dir_path);
    fprint_spice_grid_testbench(grid_testbench_dir_path, chomped_circuit_name, 
                                include_dir_path, subckt_dir_path,
                                rr_node_indices, num_clocks, Arch, 
                                vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  /* Print Netlists of the given FPGA*/
  if (vpr_setup.SpiceOpts.print_spice_top_testbench) {
    top_testbench_file = my_strcat(chomped_circuit_name, spice_top_testbench_postfix);
    /* Process top_netlist_path */
    top_testbench_dir_path = my_strcat(spice_dir_formatted, spice_top_tb_dir_name); 
    create_dir_path(top_testbench_dir_path);
    top_netlist_path = my_strcat(top_testbench_dir_path, top_testbench_file); 
    fprint_spice_top_netlist(chomped_circuit_name, top_netlist_path, 
                             include_dir_path, subckt_dir_path, 
                             rr_node_indices, num_clocks, *(Arch.spice), 
                             vpr_setup.SpiceOpts.fpga_spice_leakage_only);
  }

  /* Generate a shell script for running HSPICE simulations */
  fprint_run_hspice_shell_script(*(Arch.spice), spice_dir_formatted, subckt_dir_path);

  /* END Clocking*/
  t_end = clock();

  run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, "SPICE netlists generation took %g seconds\n", run_time_sec);  

  /* Free spice_net_info */
  free_clb_nets_spice_net_info();
  /* TODO: Free tb_llist */
  free_spice_tb_llist(); 
  /* Free */
  my_free(spice_dir_formatted);
  my_free(include_dir_path);
  my_free(subckt_dir_path);

  return;
}
