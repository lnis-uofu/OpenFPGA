/* global parameters for SPICE support*/
extern float max_width_per_trans;

extern char* nmos_subckt_name;
extern char* pmos_subckt_name;
extern char* io_nmos_subckt_name;
extern char* io_pmos_subckt_name;
extern char* cpt_subckt_name;
extern char* rram_veriloga_file_name;
extern char* mux_basis_posfix;
extern char* mux_special_basis_posfix;
extern char* nmos_pmos_spice_file_name;
extern char* basics_spice_file_name;
extern char* muxes_spice_file_name;
extern char* wires_spice_file_name;
extern char* logic_block_spice_file_name;
extern char* luts_spice_file_name;
extern char* routing_spice_file_name;
extern char* meas_header_file_name;
extern char* stimu_header_file_name;
extern char* design_param_header_file_name;

/* Postfix for circuit design parameters */
extern char* design_param_postfix_input_buf_size; 
extern char* design_param_postfix_output_buf_size; 
extern char* design_param_postfix_pass_gate_logic_pmos_size; 
extern char* design_param_postfix_pass_gate_logic_nmos_size; 
extern char* design_param_postfix_wire_param_res_val; 
extern char* design_param_postfix_wire_param_cap_val; 
extern char* design_param_postfix_rram_ron; 
extern char* design_param_postfix_rram_roff; 
extern char* design_param_postfix_rram_wprog_set_pmos; 
extern char* design_param_postfix_rram_wprog_set_nmos; 
extern char* design_param_postfix_rram_wprog_reset_pmos; 
extern char* design_param_postfix_rram_wprog_reset_nmos; 

/* Testbench names */
extern char* spice_top_testbench_postfix;
extern char* spice_grid_testbench_postfix;
extern char* spice_pb_mux_testbench_postfix;
extern char* spice_cb_mux_testbench_postfix;
extern char* spice_sb_mux_testbench_postfix;
extern char* spice_cb_testbench_postfix;
extern char* spice_sb_testbench_postfix;
extern char* spice_lut_testbench_postfix;
extern char* spice_dff_testbench_postfix;
extern char* spice_hardlogic_testbench_postfix;
extern char* spice_io_testbench_postfix;
extern char* bitstream_spice_file_postfix;
/* RUN HSPICE Shell Script Name */
/*
extern char* run_hspice_shell_script_name;
extern char* default_spice_dir_path;
extern char* sim_results_dir_name;
extern char* spice_top_tb_dir_name;
extern char* spice_grid_tb_dir_name;
extern char* spice_pb_mux_tb_dir_name;
extern char* spice_cb_mux_tb_dir_name;
extern char* spice_sb_mux_tb_dir_name;
extern char* spice_lut_tb_dir_name;
extern char* spice_dff_tb_dir_name;
*/

extern t_spice_model* sram_spice_model;
extern enum e_sram_orgz sram_spice_orgz_type;
extern t_sram_orgz_info* sram_spice_orgz_info;

extern t_spice_model* iopad_spice_model;

extern int rram_design_tech;
extern int num_used_grid_mux_tb;
extern int num_used_cb_mux_tb;
extern int num_used_sb_mux_tb;
extern int num_used_grid_tb;
extern int num_used_cb_tb;
extern int num_used_sb_tb;
extern int num_used_lut_tb;
extern int num_used_dff_tb;
extern int num_used_hardlogic_tb;
extern int num_used_io_tb;
extern t_llist* tb_head;
/* Heads of scan-chain */
extern t_llist* scan_chain_heads;

/* Name of global ports used in all netlists */
extern char* spice_tb_global_vdd_port_name;
extern char* spice_tb_global_gnd_port_name;
extern char* spice_tb_global_config_done_port_name;
extern char* spice_tb_global_set_port_name;
extern char* spice_tb_global_reset_port_name;
extern char* spice_tb_global_vdd_localrouting_port_name;
extern char* spice_tb_global_vdd_io_port_name;
extern char* spice_tb_global_vdd_hardlogic_port_name;
extern char* spice_tb_global_vdd_sram_port_name;
extern char* spice_tb_global_vdd_lut_sram_port_name;
extern char* spice_tb_global_vdd_localrouting_sram_port_name;
extern char* spice_tb_global_vdd_io_sram_port_name;
extern char* spice_tb_global_vdd_hardlogic_sram_port_name;
extern char* spice_tb_global_vdd_cb_sram_port_name;
extern char* spice_tb_global_vdd_sb_sram_port_name;
extern char* spice_tb_global_clock_port_name;
extern char* spice_tb_global_vdd_load_port_name;
extern char* spice_tb_global_port_inv_postfix;

extern int spice_sim_multi_thread_num;
