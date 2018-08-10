
/* Enumeration for the types of measurements*/
enum e_measure_type {
 SPICE_MEASURE_LEAKAGE_POWER, SPICE_MEASURE_DYNAMIC_POWER
};

/* Subroutines declarations */

void fprint_spice_head(FILE* fp,
                       char* usage);

FILE* spice_create_one_subckt_file(char* subckt_dir,
                                   char* subckt_name_prefix,
                                   char* spice_subckt_file_name_prefix,
                                   int grid_x, int grid_y,
                                   char** sp_name);

void spice_print_one_include_subckt_line(FILE* fp, 
                                         char* subckt_dir,
                                         char* subckt_file_name);

int rec_fprint_spice_model_global_ports(FILE* fp, 
                                        t_spice_model* cur_spice_model,
                                        boolean recursive);

void spice_print_subckt_header_file(t_llist* subckt_llist_head,
                                    char* subckt_dir,
                                    char* header_file_name);

int fprint_spice_global_ports(FILE* fp, t_llist* head);

void fprint_spice_generic_testbench_global_ports(FILE* fp, 
                                                 t_sram_orgz_info* cur_sram_orgz_info,
                                                 t_llist* head);

void fprint_spice_sram_one_outport(FILE* fp,
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int cur_sram,
                                   int port_type_index);

void fprint_spice_one_specific_sram_subckt(FILE* fp,
                                          t_sram_orgz_info* cur_sram_orgz_info,
                                          t_spice_model* parent_spice_model,
                                          char* vdd_port_name,
                                          int sram_index);

void fprint_spice_one_sram_subckt(FILE* fp,
                                  t_sram_orgz_info* cur_sram_orgz_info,
                                  t_spice_model* parent_spice_model,
                                  char* vdd_port_name);

void fprint_voltage_pulse_params(FILE* fp,
                                 int init_val,
                                 float density,
                                 float probability);


void init_include_user_defined_netlists(t_spice spice);

void fprint_include_user_defined_netlists(FILE* fp,
                                          t_spice spice);

void fprint_splited_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     t_spice spice);

void fprint_grid_splited_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          t_spice spice);

void fprint_global_vdds_spice_model(FILE* fp, 
                                    enum e_spice_model_type spice_model_type,
                                    t_spice spice);

void fprint_grid_global_vdds_spice_model(FILE* fp, int x, int y, 
                                         enum e_spice_model_type spice_model_type,
                                         t_spice spice);

void fprint_global_pad_ports_spice_model(FILE* fp, 
                                         t_spice spice);

void fprint_spice_global_vdd_switch_boxes(FILE* fp);

void fprint_spice_global_vdd_connection_boxes(FILE* fp);

void fprint_measure_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     enum e_measure_type meas_type,
                                     int num_cycle,
                                     t_spice spice,
                                     boolean leakage_only);

void fprint_measure_grid_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          enum e_measure_type meas_type,
                                          int num_cycle,
                                          t_spice spice,
                                          boolean leakage_only);

void fprint_call_defined_grids(FILE* fp);

void fprint_call_defined_one_channel(FILE* fp,
                                     t_rr_type chan_type,
                                     int x, int y,
                                     int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                     t_ivec*** LL_rr_node_indices);

void fprint_call_defined_channels(FILE* fp,
                                  int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                  t_ivec*** LL_rr_node_indices);

void fprint_call_defined_one_connection_box(FILE* fp,
                                            t_cb cur_cb_info);

void fprint_call_defined_connection_boxes(FILE* fp);

void fprint_call_defined_one_switch_box(FILE* fp,
                                        t_sb cur_sb_info);

void fprint_call_defined_switch_boxes(FILE* fp);

void fprint_spice_toplevel_one_grid_side_pin_with_given_index(FILE* fp, 
                                                              int pin_index, int side,
                                                              int x, int y);

void fprint_spice_clb2clb_directs(FILE* fp, 
                                  int num_directs, 
                                  t_clb_to_clb_directs* direct);

void fprint_one_design_param_w_wo_variation(FILE* fp,
                                            char* param_name,
                                            float avg_val,
                                            t_spice_mc_variation_params variation_params);

void fprint_tech_lib(FILE* fp,
                     t_spice_mc_variation_params cmos_variation_params,
                     t_spice_tech_lib tech_lib);

void fprint_spice_circuit_param(FILE* fp,
                                t_spice_mc_params mc_params,
                                int num_spice_models,
                                t_spice_model* spice_model);

void fprint_spice_netlist_measurement_one_design_param(FILE* fp,
                                                       char* param_name);

void fprint_spice_netlist_generic_measurements(FILE* fp, 
                                               t_spice_mc_params mc_params,
                                               int num_spice_models,
                                               t_spice_model* spice_model);

void fprint_spice_options(FILE* fp,
                          t_spice_params spice_params);

void fprint_spice_include_key_subckts(FILE* fp,
                                      char* subckt_dir_path);

void fprint_spice_include_param_headers(FILE* fp,
                                        char* include_dir_path);

void fprint_spice_netlist_transient_setting(FILE* fp, 
                                            t_spice spice, 
                                            int num_sim_clock_cycles, 
                                            boolean leakage_only);

void fprint_stimulate_dangling_one_grid_pin(FILE* fp,
                                            int x, int y,
                                            int height, int side, int pin_index,
                                            t_ivec*** LL_rr_node_indices);

void fprint_stimulate_dangling_io_grid_pins(FILE* fp,
                                            int x, int y);

void fprint_stimulate_dangling_normal_grid_pins(FILE* fp,
                                                int x, int y);

void fprint_stimulate_dangling_grid_pins(FILE* fp);

void init_logical_block_spice_model_temp_used(t_spice_model* spice_model);

void init_logical_block_spice_model_type_temp_used(int num_spice_models, t_spice_model* spice_model,
                                                   enum e_spice_model_type spice_model_type);

void fprint_global_vdds_logical_block_spice_model(FILE* fp,
                                                  t_spice_model* spice_model);

void fprint_splited_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model);

void fprint_measure_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model,
                                                   enum e_measure_type meas_type,
                                                   int num_clock_cycle,
                                                   boolean leakage_only);

void fprint_spice_testbench_wire_one_global_port_stimuli(FILE* fp, 
                                                         t_spice_model_port* cur_global_port, 
                                                         char* voltage_stimuli_port_name);

void fprint_spice_testbench_global_sram_inport_stimuli(FILE* fp,
                                                       t_sram_orgz_info* cur_sram_orgz_info);

void fprint_spice_testbench_global_vdd_port_stimuli(FILE* fp,
                                                    char* global_vdd_port_name,
                                                    char* voltage_level);

void fprint_spice_testbench_global_ports_stimuli(FILE* fp, 
                                                 t_llist* head);

void fprint_spice_testbench_generic_global_ports_stimuli(FILE* fp,
                                                         int num_clock);

float find_spice_testbench_pb_pin_mux_load_inv_size(t_spice_model* fan_out_spice_model);

float find_spice_testbench_rr_mux_load_inv_size(t_rr_node* load_rr_node,
                                                int switch_index);

void fprint_spice_testbench_pb_graph_pin_inv_loads_rec(FILE* fp, int* testbench_load_cnt, 
                                                       int grid_x, int grid_y,
                                                       t_pb_graph_pin* src_pb_graph_pin, 
                                                       t_pb* src_pb, 
                                                       char* outport_name,
                                                       boolean consider_parent_node,
                                                       t_ivec*** LL_rr_node_indices);

char* fprint_spice_testbench_rr_node_load_version(FILE* fp, int* testbench_load_cnt,
                                                  int num_segments,
                                                  t_segment_inf* segments,
                                                  int load_level,
                                                  t_rr_node cur_rr_node, 
                                                  char* outport_name);

void fprint_spice_testbench_one_cb_mux_loads(FILE* fp, int* testbench_load_cnt,
                                             t_rr_node* src_rr_node,
                                             char* outport_name,
                                             t_ivec*** LL_rr_node_indices);

void fprint_spice_testbench_one_grid_pin_stimulation(FILE* fp, int x, int y, 
                                                    int height, int side, 
                                                    int ipin,
                                                    t_ivec*** LL_rr_node_indices);

void fprint_spice_testbench_one_grid_pin_loads(FILE* fp, int x, int y, 
                                              int height, int side, 
                                              int ipin,
                                              int* testbench_load_cnt,
                                              t_ivec*** LL_rr_node_indices);

t_llist* add_one_spice_tb_info_to_llist(t_llist* cur_head, 
                                        char* tb_file_path, 
                                        int num_sim_clock_cycles);
