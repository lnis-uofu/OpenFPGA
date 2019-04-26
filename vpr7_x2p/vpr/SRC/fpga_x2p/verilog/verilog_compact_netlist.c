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
#include "route_common.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "spice_mux.h"
#include "fpga_x2p_globals.h"

/* Include Synthesizable Verilog headers */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_primitives.h"
#include "verilog_pbtypes.h"
#include "verilog_top_netlist_utils.h"

/* ONLY for compact Verilog netlists:
 * Generate uniformly the prefix of the module name for each grid  
 */
static 
char* generate_compact_verilog_grid_module_name_prefix(t_type_ptr phy_block_type,
                                                       int border_side) {
  char* subckt_name = my_strdup(grid_verilog_file_name_prefix);

  /* Check */
  if (IO_TYPE == phy_block_type) {
    assert( (-1 < border_side) && (border_side < 4));
  }

  if (IO_TYPE == phy_block_type) {
    subckt_name = my_strcat(subckt_name, convert_side_index_to_string(border_side));
    subckt_name = my_strcat(subckt_name, "_");
  }

  return subckt_name;
}


/* ONLY for compact Verilog netlists:
 * Generate uniformly the module name for each grid  
 */
static 
char* generate_compact_verilog_grid_module_name(t_type_ptr phy_block_type,
                                                int border_side) {
  char* subckt_name = NULL;

  subckt_name = generate_compact_verilog_grid_module_name_prefix(phy_block_type, border_side);

  subckt_name = my_strcat(subckt_name, phy_block_type->name);

  return subckt_name;
}

/* ONLY for compact Verilog netlists:
 * Update the grid_index_low and grid_index_high for each spice_models 
 * Currently, we focus on three spice_models: SRAMs/SCFFs/IOPADs
 */
static 
void compact_verilog_update_one_spice_model_grid_index(t_type_ptr phy_block_type,
                                                       int grid_x, int grid_y, 
                                                       int num_spice_models, 
                                                       t_spice_model* spice_model) {
  int i;
  int stamped_cnt = 0;

  for (i = 0; i < num_spice_models; i++) {
    /* Only LUT and MUX requires configuration bits*/
    switch (spice_model[i].type) {
    case SPICE_MODEL_INVBUF:
    case SPICE_MODEL_PASSGATE:
    case SPICE_MODEL_LUT:
    case SPICE_MODEL_MUX:
    case SPICE_MODEL_WIRE:
    case SPICE_MODEL_CHAN_WIRE:
    case SPICE_MODEL_FF:
    case SPICE_MODEL_HARDLOGIC:
    case SPICE_MODEL_GATE:
      break;
    case SPICE_MODEL_SCFF:
    case SPICE_MODEL_SRAM:
      stamped_cnt = spice_model[i].cnt;
      spice_model[i].grid_index_low[grid_x][grid_y] = stamped_cnt; 
      spice_model[i].grid_index_high[grid_x][grid_y] = stamped_cnt + phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_conf_bits;
      spice_model[i].cnt = spice_model[i].grid_index_high[grid_x][grid_y];
      break;
    case SPICE_MODEL_IOPAD:
      stamped_cnt = spice_model[i].cnt;
      spice_model[i].grid_index_low[grid_x][grid_y] = stamped_cnt; 
      spice_model[i].grid_index_high[grid_x][grid_y] = stamped_cnt + phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_iopads;
      spice_model[i].cnt = spice_model[i].grid_index_high[grid_x][grid_y];
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid spice_model_type!\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  return;
}

/* ONLY for compact Verilog netlists:
 * Update the grid_index_low and grid_index_high in sram_orgz_info 
 */
static 
void compact_verilog_update_sram_orgz_info_grid_index(t_sram_orgz_info* cur_sram_orgz_info,
                                                      t_type_ptr phy_block_type,
                                                      int grid_x, int grid_y) { 
  int cur_num_conf_bits;
  int cur_num_bl, cur_num_wl;

  cur_num_conf_bits = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_num_bl, &cur_num_wl); 

  cur_sram_orgz_info->grid_reserved_conf_bits[grid_x][grid_y] = phy_block_type->pb_type->physical_mode_num_reserved_conf_bits;

  cur_sram_orgz_info->grid_conf_bits_lsb[grid_x][grid_y] = cur_num_conf_bits;

  cur_sram_orgz_info->grid_conf_bits_msb[grid_x][grid_y] = cur_num_conf_bits;

  cur_sram_orgz_info->grid_conf_bits_msb[grid_x][grid_y] += phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_conf_bits;

  cur_num_conf_bits = cur_sram_orgz_info->grid_conf_bits_msb[grid_x][grid_y];
  cur_num_bl = cur_sram_orgz_info->grid_conf_bits_msb[grid_x][grid_y];
  cur_num_wl = cur_sram_orgz_info->grid_conf_bits_msb[grid_x][grid_y];

  /* Update the counter */
  update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_conf_bits);
  update_sram_orgz_info_num_blwl(cur_sram_orgz_info, cur_num_bl, cur_num_wl);

  return;
}

/* ONLY for compact Verilog netlists:
 * Update the grid_index_low and grid_index_high for each spice_models
 * Currently, we focus on three spice_models: SRAMs/SCFFs/IOPADs
 * IMPORTANT: The sequence of for loop should be consistent with 
 * 1. bitstream logic block 
 * 2. verilog pbtypes logic block 
 * 2. spice pbtypes logic block 
 */
static 
void compact_verilog_update_grid_spice_model_and_sram_orgz_info(t_sram_orgz_info* cur_sram_orgz_info,
                                                                int num_spice_models, 
                                                                t_spice_model* spice_model) {
  int ix, iy;


  /* Check the grid*/
  if ((0 == nx)||(0 == ny)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid grid size (nx=%d, ny=%d)!\n", __FILE__, __LINE__, nx, ny);
    return;    
  }

  vpr_printf(TIO_MESSAGE_INFO,"Grid size of FPGA: nx=%d ny=%d\n", nx + 1, ny + 1);
  assert(NULL != grid);
 
  /* Print the core logic block one by one
   * Note ix=0 and ix = nx + 1 are IO pads. They surround the core logic blocks
   */
  vpr_printf(TIO_MESSAGE_INFO,"Generating core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      /* Ensure this is not a io */
      assert(IO_TYPE != grid[ix][iy].type);
      /* Bypass empty type */
      if (EMPTY_TYPE == grid[ix][iy].type) {
        continue;
      }
      /* Bypass non-zero offset grid: heterogeneous block may occupy multiple grids */
      if (0 < grid[ix][iy].offset) {
        continue;
      } 
      /* Update the grid index low and high */ 
      compact_verilog_update_one_spice_model_grid_index(grid[ix][iy].type,
                                                        ix, iy,
                                                        num_spice_models, spice_model);
      /* Update all the sram bits */
      compact_verilog_update_sram_orgz_info_grid_index(cur_sram_orgz_info,
                                                       grid[ix][iy].type,
                                                       ix, iy);
    }
  }

  vpr_printf(TIO_MESSAGE_INFO,"Generating IO grids...\n");
  /* Print the IO pads */
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* Update the grid index low and high */
    compact_verilog_update_one_spice_model_grid_index(grid[ix][iy].type,
                                                      ix, iy,
                                                      num_spice_models, spice_model);
    /* Update all the sram bits */
    compact_verilog_update_sram_orgz_info_grid_index(cur_sram_orgz_info,
                                                     grid[ix][iy].type,
                                                     ix, iy);
  }

  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* Update the grid index low and high */
    compact_verilog_update_one_spice_model_grid_index(grid[ix][iy].type,
                                                      ix, iy,
                                                      num_spice_models, spice_model);
    /* Update all the sram bits */
    compact_verilog_update_sram_orgz_info_grid_index(cur_sram_orgz_info,
                                                     grid[ix][iy].type,
                                                     ix, iy);
  }

  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* Update the grid index low and high */
    compact_verilog_update_one_spice_model_grid_index(grid[ix][iy].type,
                                                      ix, iy,
                                                      num_spice_models, spice_model);
    /* Update all the sram bits */
    compact_verilog_update_sram_orgz_info_grid_index(cur_sram_orgz_info,
                                                     grid[ix][iy].type,
                                                     ix, iy);
  }
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* Update the grid index low and high */
    compact_verilog_update_one_spice_model_grid_index(grid[ix][iy].type,
                                                      ix, iy,
                                                      num_spice_models, spice_model);
    /* Update all the sram bits */
    compact_verilog_update_sram_orgz_info_grid_index(cur_sram_orgz_info,
                                                     grid[ix][iy].type,
                                                     ix, iy);
  }


  /* Free */
   
  return;
}

/* Create a Verilog file and dump a module consisting of a I/O block,
 * The pins appear in the port list will depend on the selected border side
 */
static 
void dump_compact_verilog_one_physical_block(t_sram_orgz_info* cur_sram_orgz_info, 
                                             char* verilog_dir_path,
                                             char* subckt_dir_path,
                                             t_type_ptr phy_block_type,
                                             int border_side,
                                             t_arch* arch,
                                             t_syn_verilog_opts fpga_verilog_opts) {
  int iz;
  int temp_reserved_conf_bits_msb;
  int temp_iopad_lsb, temp_iopad_msb;
  int temp_conf_bits_lsb, temp_conf_bits_msb;
  char* fname = NULL;  
  FILE* fp = NULL;
  char* title = my_strcat("FPGA Verilog Netlist for Design: ", phy_block_type->name);
  char* subckt_name = NULL;
  char* subckt_name_prefix = NULL;
  boolean verilog_module_dumped = FALSE;

  /* Check */
  if (IO_TYPE == phy_block_type) {
    assert( (-1 < border_side) && (border_side < 4));
  }

  /* Give a name to the Verilog netlist */
  fname = my_strcat(format_dir_path(subckt_dir_path), phy_block_type->name);
  /* Give a special name to IO blocks */
  if (IO_TYPE == phy_block_type) {
    fname = my_strcat(fname, "_");
    fname = my_strcat(fname, convert_side_index_to_string(border_side));
  }
  fname = my_strcat(fname, verilog_netlist_file_postfix); 

  /* Create file handler */
  fp = fopen(fname, "w");
  /* Check if the path exists*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog netlist %s!",__FILE__, __LINE__, fname); 
    exit(1);
  } 

  /* Subckt name */
  subckt_name_prefix = generate_compact_verilog_grid_module_name_prefix(phy_block_type, border_side);

  if (IO_TYPE == phy_block_type) {
    vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Verilog Netlist (%s) for logic block %s at %s side ...\n",
               fname, phy_block_type->name, convert_side_index_to_string(border_side));
  } else { 
    vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Verilog Netlist (%s) for logic block %s...\n",
               fname, phy_block_type->name);
  }

  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  /* Dump all the submodules */
  for (iz = 0; iz < phy_block_type->capacity; iz++) {
    /* We only need to output one Verilog module, others are instanced */
    if (TRUE == verilog_module_dumped) {
      continue;
    }
    /* Comments: Grid [x][y]*/
    fprintf(fp, "//----- Submodule of type_descriptor: %s -----\n", phy_block_type->name);
    /* Print a NULL logic block...*/
    dump_verilog_phy_pb_graph_node_rec(cur_sram_orgz_info, fp, subckt_name_prefix, 
                                       phy_block_type->pb_graph_head, iz);
    fprintf(fp, "//----- END -----\n\n");
    /* Switch Flag on dumping verilog module */
    verilog_module_dumped = TRUE;
  }

  /* Subckt name */
  subckt_name = generate_compact_verilog_grid_module_name(phy_block_type, border_side);

  /* Create top module and call all the defined submodule */
  fprintf(fp, "//----- %s, Capactity: %d -----\n", phy_block_type->name, phy_block_type->capacity);
  fprintf(fp, "//----- Top Protocol -----\n");
  /* Definition */
  fprintf(fp, "module %s ( \n", subckt_name);
  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE)) {
    fprintf(fp, ",\n");
  }

  /* Pins */
  if (IO_TYPE == phy_block_type) { 
    /* Generate a fake (x,y) coordinate that can be used for print pin names */
    /* verilog_compact_generate_fake_xy_for_io_border_side(border_side, &ix, &iy); */
    /* Special Care for I/O grid */
    dump_compact_verilog_io_grid_pins(fp, phy_block_type, border_side, TRUE, FALSE);
  } else {
    dump_compact_verilog_grid_pins(fp, phy_block_type, TRUE, FALSE);
  }

  /* I/O PAD */
  dump_verilog_grid_common_port(fp, iopad_verilog_model, gio_inout_prefix, 
                                0, phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_iopads - 1,
                                VERILOG_PORT_INOUT); 

  /* Print configuration ports */
  /* Reserved configuration ports */
  temp_reserved_conf_bits_msb = phy_block_type->pb_type->physical_mode_num_reserved_conf_bits; 
  if (0 < temp_reserved_conf_bits_msb) { 
    fprintf(fp, ",\n");
    dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info,
                                     0, temp_reserved_conf_bits_msb - 1,
                                     VERILOG_PORT_INPUT); 
  }
  /* Normal configuration ports */
  temp_conf_bits_msb = phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_conf_bits; 
  /* Get the number of sram bits in this submodule!!! */
  if (0 < temp_conf_bits_msb) { 
    fprintf(fp, ",\n");
    dump_verilog_sram_ports(fp, cur_sram_orgz_info,
                            0, temp_conf_bits_msb - 1, 
                            VERILOG_PORT_INPUT); 
  }
  /* Dump ports only visible during formal verification*/
  if (0 < temp_conf_bits_msb) { 
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                0, 
                                                temp_conf_bits_msb - 1,
                                                VERILOG_PORT_INPUT);	// Should be modified to be VERILOG_PORT_INPUT
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }

  fprintf(fp, ");\n");

  /* Initialize temporary counter */
  temp_conf_bits_lsb = 0;
  temp_iopad_lsb = 0;

  /* Local wires for memory configurations */
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                              0, temp_conf_bits_msb - 1); 


  /* Quote all the sub blocks*/
  for (iz = 0; iz < phy_block_type->capacity; iz++) {
    /* Local Vdd and Gnd, subckt name*/
    fprintf(fp, "%s ", compact_verilog_get_grid_phy_block_subckt_name(phy_block_type, iz, subckt_name_prefix));
    fprintf(fp, " %s (", gen_verilog_one_phy_block_instance_name(phy_block_type, iz));
    fprintf(fp, "\n");
    /* dump global ports */
    if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
      fprintf(fp, ",\n");
    }
    /* Print all the pins */
    /* Special Care for I/O grid */
    if (IO_TYPE == phy_block_type) { 
      dump_compact_verilog_io_grid_block_subckt_pins(fp, phy_block_type, border_side, iz);
    } else {
      dump_verilog_grid_block_subckt_pins(fp, iz, phy_block_type);
    }

    /* Print configuration ports */
    temp_reserved_conf_bits_msb = phy_block_type->pb_type->physical_mode_num_reserved_conf_bits; 
    temp_conf_bits_msb = temp_conf_bits_lsb + phy_block_type->pb_type->physical_mode_num_conf_bits;
    temp_iopad_msb = temp_iopad_lsb + phy_block_type->pb_type->physical_mode_num_iopads;

    /* Print Input Pad and Output Pad */
    fprintf(fp, "\n//---- IOPAD ----\n");
    dump_verilog_grid_common_port(fp, iopad_verilog_model, gio_inout_prefix,
                                  temp_iopad_lsb,  temp_iopad_msb - 1,
                                  VERILOG_PORT_CONKT); 
    /* Reserved configuration ports */
    if (0 < temp_reserved_conf_bits_msb) { 
      fprintf(fp, ",\n");
      dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info,
                                       0, temp_reserved_conf_bits_msb - 1,
                                       VERILOG_PORT_CONKT); 
    }
    /* Normal configuration ports */
    if (0 < (temp_conf_bits_msb - temp_conf_bits_lsb)) { 
      fprintf(fp, ",\n");
      fprintf(fp, "//---- SRAM ----\n");
      dump_verilog_sram_local_ports(fp, cur_sram_orgz_info,
                                    temp_conf_bits_lsb, temp_conf_bits_msb - 1, 
                                    VERILOG_PORT_CONKT); 
    }

    /* Dump ports only visible during formal verification*/
    if (0 < (temp_conf_bits_msb - temp_conf_bits_lsb)) { 
      fprintf(fp, "\n");
      fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
      fprintf(fp, ",\n");
      dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                  temp_conf_bits_lsb, 
                                                  temp_conf_bits_msb - 1,
                                                  VERILOG_PORT_CONKT);
      fprintf(fp, "\n");
      fprintf(fp, "`endif\n");
    }
    /* Update temp_sram_lsb */
    temp_conf_bits_lsb = temp_conf_bits_msb;
    temp_iopad_lsb = temp_iopad_msb;
    fprintf(fp, ");\n");
  }

  fprintf(fp, "endmodule\n");
  fprintf(fp, "//----- END Top Protocol -----\n");
  fprintf(fp, "//----- END Grid %s, Capactity: %d -----\n\n", phy_block_type->name, phy_block_type->capacity);

  /* Check flags */
  assert( temp_conf_bits_msb == phy_block_type->capacity * phy_block_type->pb_type->physical_mode_num_conf_bits ); 

  /* Close file handler */
  fclose(fp); 

  /* Add fname to the linked list */
  grid_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(grid_verilog_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);
  my_free(subckt_name);
  my_free(subckt_name_prefix);

  return;
}

/** Create logic block modules in a compact way:
 * 1. Only one module for each I/O on each border side (IO_TYPE)
 * 2. Only one module for each CLB (FILL_TYPE)
 * 3. Only one module for each heterogeneous block
 */
void dump_compact_verilog_logic_blocks(t_sram_orgz_info* cur_sram_orgz_info,
                                       char* verilog_dir,
                                       char* subckt_dir,
                                       t_arch* arch,
                                       t_syn_verilog_opts fpga_verilog_opts) {
  int itype, iside, num_sides;
  int* stamped_spice_model_cnt = NULL;
  t_sram_orgz_info* stamped_sram_orgz_info = NULL;

  /* Create a snapshot on spice_model counter */
  stamped_spice_model_cnt = snapshot_spice_model_counter(arch->spice->num_spice_model, 
                                                         arch->spice->spice_models);
  /* Create a snapshot on sram_orgz_info */
  stamped_sram_orgz_info = snapshot_sram_orgz_info(cur_sram_orgz_info);
 
  /* Enumerate the types, dump one Verilog module for each */
  for (itype = 0; itype < num_types; itype++) {
    if (EMPTY_TYPE == &type_descriptors[itype]) {
    /* Bypass empty type or NULL */
      continue;
    } else if (IO_TYPE == &type_descriptors[itype]) {
      num_sides = 4;
    /* Special for I/O block, generate one module for each border side */
      for (iside = 0; iside < num_sides; iside++) {
        dump_compact_verilog_one_physical_block(cur_sram_orgz_info, 
                                                verilog_dir, subckt_dir, 
                                                &type_descriptors[itype], iside, 
                                                arch, fpga_verilog_opts);
      } 
      continue;
    } else if (FILL_TYPE == &type_descriptors[itype]) {
    /* For CLB */
      dump_compact_verilog_one_physical_block(cur_sram_orgz_info,  
                                              verilog_dir, subckt_dir, 
                                              &type_descriptors[itype], -1,
                                              arch, fpga_verilog_opts);
      continue;
    } else {
    /* For heterogenenous blocks */
      dump_compact_verilog_one_physical_block(cur_sram_orgz_info,  
                                              verilog_dir, subckt_dir, 
                                              &type_descriptors[itype], -1,
                                              arch, fpga_verilog_opts);

    }
  }

  /* Output a header file for all the logic blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for grid submodules...\n");
  dump_verilog_subckt_header_file(grid_verilog_subckt_file_path_head,
                                  subckt_dir,
                                  logic_block_verilog_file_name);


  /* Recover spice_model counter */
  set_spice_model_counter(arch->spice->num_spice_model, 
                          arch->spice->spice_models,
                          stamped_spice_model_cnt);

  /* Restore sram_orgz_info to the base */ 
  copy_sram_orgz_info (cur_sram_orgz_info, stamped_sram_orgz_info);

  /* Update the grid_index low and high for spice models 
   * THIS FUNCTION MUST GO AFTER OUTPUTING PHYSICAL LOGIC BLOCKS!!!
   */
  compact_verilog_update_grid_spice_model_and_sram_orgz_info(cur_sram_orgz_info,
                                                             arch->spice->num_spice_model, 
                                                             arch->spice->spice_models);
  /* Free */
  free_sram_orgz_info(stamped_sram_orgz_info, stamped_sram_orgz_info->type);
  my_free (stamped_spice_model_cnt); 

  return;
}

/* Call defined grid 
 * Instance unique submodules (I/O, CLB, Heterogeneous block) for the full grids
 */
static 
void dump_compact_verilog_defined_one_grid(t_sram_orgz_info* cur_sram_orgz_info,
                                           FILE* fp,
                                           int ix, int iy, int border_side) {
  char* subckt_name = NULL;
   
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }
  
  if ((NULL == grid[ix][iy].type)
    || (EMPTY_TYPE == grid[ix][iy].type) 
    ||(0 != grid[ix][iy].offset)) {
    return;
  }

  subckt_name = generate_compact_verilog_grid_module_name(grid[ix][iy].type, border_side);
 
  /* Comment lines */
  fprintf(fp, "//----- BEGIN Call Grid[%d][%d] module -----\n", ix, iy);
  /* Print the Grid module */
  fprintf(fp, "%s  ", subckt_name); /* Call the name of subckt */ 
  fprintf(fp, "%s ", gen_verilog_one_grid_instance_name(ix, iy));
  fprintf(fp, "(");
  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
    fprintf(fp, ",\n");
  }

  if (IO_TYPE == grid[ix][iy].type) {
    dump_verilog_io_grid_pins(fp, ix, iy, TRUE, FALSE, FALSE);
  } else {
    dump_verilog_grid_pins(fp, ix, iy, TRUE, FALSE, FALSE);
  }
 
  /* IO PAD */
  dump_verilog_grid_common_port(fp, iopad_verilog_model, gio_inout_prefix, 
                                iopad_verilog_model->grid_index_low[ix][iy],
                                iopad_verilog_model->grid_index_high[ix][iy] - 1,
                                VERILOG_PORT_CONKT); 

  /* Print configuration ports */
  /* Reserved configuration ports */
  if (0 < cur_sram_orgz_info->grid_reserved_conf_bits[ix][iy]) {
    fprintf(fp, ",\n");
  }
  dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info,
                                   0, 
                                   cur_sram_orgz_info->grid_reserved_conf_bits[ix][iy] - 1,
                                   VERILOG_PORT_CONKT);
  /* Normal configuration ports */
  if (0 < (cur_sram_orgz_info->grid_conf_bits_msb[ix][iy]
           - cur_sram_orgz_info->grid_conf_bits_lsb[ix][iy])) {
    fprintf(fp, ",\n");
    dump_verilog_sram_local_ports(fp, cur_sram_orgz_info,
                                  cur_sram_orgz_info->grid_conf_bits_lsb[ix][iy],
                                  cur_sram_orgz_info->grid_conf_bits_msb[ix][iy] - 1,
                                  VERILOG_PORT_CONKT);
  }

  /* Dump ports only visible during formal verification*/
  if (0 < (cur_sram_orgz_info->grid_conf_bits_msb[ix][iy] - 1
           - cur_sram_orgz_info->grid_conf_bits_lsb[ix][iy])) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                cur_sram_orgz_info->grid_conf_bits_lsb[ix][iy],
                                                cur_sram_orgz_info->grid_conf_bits_msb[ix][iy] - 1,
                                                VERILOG_PORT_CONKT);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }
  fprintf(fp, ");\n");
  /* Comment lines */
  fprintf(fp, "//----- END call Grid[%d][%d] module -----\n\n", ix, iy);

  return;
}

/* Call defined grid 
 * Instance unique submodules (I/O, CLB, Heterogeneous block) for the full grids
 */
void dump_compact_verilog_defined_grids(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Normal Grids */
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grid[ix][iy].type) {
        continue;
      }
      assert(IO_TYPE != grid[ix][iy].type);
      dump_compact_verilog_defined_one_grid(cur_sram_orgz_info, fp, ix, iy, -1);
    }
  } 

  /* IO Grids */
  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_compact_verilog_defined_one_grid(cur_sram_orgz_info, fp, ix, iy, 0);
  } 
  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_compact_verilog_defined_one_grid(cur_sram_orgz_info, fp, ix, iy, 1);
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_compact_verilog_defined_one_grid(cur_sram_orgz_info, fp, ix, iy, 2);
  } 
  /* LEFT side */
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_compact_verilog_defined_one_grid(cur_sram_orgz_info, fp, ix, iy, 3);
  }


  return;
}

/** Print Top-level SPICE netlist in a compact way
 * Instance unique submodules (I/O, CLB, Heterogeneous block) for the full grids
 */
void dump_compact_verilog_top_netlist(t_sram_orgz_info* cur_sram_orgz_info,
                                      char* circuit_name,
                                      char* top_netlist_name,
                                      char* verilog_dir_path,
                                      char* submodule_dir_path,
                                      char* lb_dir_path,
                                      char* rr_dir_path,
                                      int LL_num_rr_nodes,
                                      t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices,
                                      int num_clock,
                                      t_syn_verilog_opts fpga_verilog_opts,
                                      t_spice verilog) {
  FILE* fp = NULL;
  char* formatted_dir_path = NULL;
  char* temp_include_file_path = NULL;
  char* title = my_strcat("FPGA Verilog Netlist for Design: ", circuit_name);

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog netlist %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Top-level Compact Verilog Netlist for %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  /* Include user-defined sub-circuit netlist */
  fprintf(fp, "//----- Include User-defined netlists -----\n");
  init_include_user_defined_verilog_netlists(verilog);
  dump_include_user_defined_verilog_netlists(fp, verilog);
  
  /* Special subckts for Top-level SPICE netlist */
  fprintf(fp, "//------ Include subckt netlists: Basic Primitives -----\n");
  formatted_dir_path = format_dir_path(submodule_dir_path); 
  temp_include_file_path = my_strcat(formatted_dir_path, submodule_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//------ Include subckt netlists: Logic Blocks -----\n");
  formatted_dir_path = format_dir_path(lb_dir_path); 
  temp_include_file_path = my_strcat(formatted_dir_path, logic_block_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) -----\n");
  formatted_dir_path = format_dir_path(rr_dir_path); 
  temp_include_file_path = my_strcat(formatted_dir_path, routing_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);
 
  /* Print all global wires*/
  dump_verilog_top_netlist_ports(cur_sram_orgz_info, fp, num_clock, circuit_name, verilog);

  dump_verilog_top_netlist_internal_wires(cur_sram_orgz_info, fp);

  /* Quote Routing structures: Channels */
  dump_verilog_defined_channels(fp, LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Quote Routing structures: Switch Boxes */
  dump_verilog_defined_switch_boxes(cur_sram_orgz_info, fp); 

  /* Quote Routing structures: Connection Boxes */
  dump_verilog_defined_connection_boxes(cur_sram_orgz_info, fp); 

  /* Quote defined Logic blocks subckts (Grids) */
  dump_compact_verilog_defined_grids(cur_sram_orgz_info, fp);
  
  /* Apply CLB to CLB direct connections */
  dump_verilog_clb2clb_directs(fp, num_clb2clb_directs, clb2clb_direct);

  /* Dump configuration circuits */
  dump_verilog_configuration_circuits(cur_sram_orgz_info, fp);

  /* verilog ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}


