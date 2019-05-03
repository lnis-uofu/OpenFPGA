
#include "util.h"
#include "linkedlist.h"

/* Xifan TANG: Spice support*/
enum e_spice_tech_lib_type {
  SPICE_LIB_INDUSTRY,
  SPICE_LIB_ACADEMIA
};

enum spice_model_delay_type {
  SPICE_MODEL_DELAY_RISE, 
  SPICE_MODEL_DELAY_FALL
};

/*Struct for a SPICE model of a module*/
enum e_spice_model_type {
  SPICE_MODEL_CHAN_WIRE, 
  SPICE_MODEL_WIRE, 
  SPICE_MODEL_MUX, 
  SPICE_MODEL_LUT, 
  SPICE_MODEL_FF, 
  SPICE_MODEL_SRAM, 
  SPICE_MODEL_HARDLOGIC,
  SPICE_MODEL_SCFF,
  SPICE_MODEL_IOPAD, 
  SPICE_MODEL_INVBUF, 
  SPICE_MODEL_PASSGATE, 
  SPICE_MODEL_GATE 
};

enum e_spice_model_design_tech {
  SPICE_MODEL_DESIGN_CMOS, 
  SPICE_MODEL_DESIGN_RRAM
};

enum e_spice_model_structure {
  SPICE_MODEL_STRUCTURE_TREE, 
  SPICE_MODEL_STRUCTURE_ONELEVEL, 
  SPICE_MODEL_STRUCTURE_MULTILEVEL, 
  SPICE_MODEL_STRUCTURE_CROSSBAR 
};

enum e_spice_model_buffer_type {
  SPICE_MODEL_BUF_INV, 
  SPICE_MODEL_BUF_BUF
};

enum e_spice_model_pass_gate_logic_type {
  SPICE_MODEL_PASS_GATE_TRANSMISSION, 
  SPICE_MODEL_PASS_GATE_TRANSISTOR
};

enum e_spice_model_gate_type {
  SPICE_MODEL_GATE_AND, 
  SPICE_MODEL_GATE_OR
};

/* Transistor-level basic informations*/
enum e_spice_trans_type {
  SPICE_TRANS_NMOS, 
  SPICE_TRANS_PMOS, 
  SPICE_TRANS_IO_NMOS, 
  SPICE_TRANS_IO_PMOS
};

enum e_wire_model_type {
  WIRE_MODEL_PIE,
  WIRE_MODEL_T
};

enum e_spice_model_port_type {
  SPICE_MODEL_PORT_INPUT, 
  SPICE_MODEL_PORT_OUTPUT, 
  SPICE_MODEL_PORT_INOUT, 
  SPICE_MODEL_PORT_CLOCK, 
  SPICE_MODEL_PORT_SRAM,
  SPICE_MODEL_PORT_BL,
  SPICE_MODEL_PORT_BLB,
  SPICE_MODEL_PORT_WL,
  SPICE_MODEL_PORT_WLB
};

/* For process corner */
enum e_process_corner {
 BEST_CORNER,
 TYPICAL_CORNER,
 WORST_CORNER
};

/* For SRAM */
enum e_sram_orgz {
  SPICE_SRAM_STANDALONE,
  SPICE_SRAM_SCAN_CHAIN,
  SPICE_SRAM_MEMORY_BANK
};

enum e_spice_accuracy_type {
  SPICE_FRAC, SPICE_ABS
};

enum e_spice_pb_port_type {
  SPICE_PB_PORT_INPUT,
  SPICE_PB_PORT_OUTPUT,
  SPICE_PB_PORT_CLOCK
};

enum e_spice_ff_trigger_type {
  FF_RE, FF_FE
};

enum e_spice_tb_type {
  SPICE_CB_MUX_TB, 
  SPICE_SB_MUX_TB, 
  SPICE_PB_MUX_TB, 
  SPICE_GRID_TB,
  SPICE_SB_TB,
  SPICE_CB_TB,
  SPICE_LUT_TB,
  SPICE_IO_TB,
  SPICE_HARDLOGIC_TB
};

enum e_spice_pin2pin_interc_type {
  INPUT2INPUT_INTERC, 
  OUTPUT2OUTPUT_INTERC
};

/* typedef of structs */
typedef struct s_spice_transistor_type t_spice_transistor_type;
typedef struct s_spice_tech_lib t_spice_tech_lib;
typedef struct s_spice_model_gate t_spice_model_gate;
typedef struct s_spice_model_rram t_spice_model_rram;
typedef struct s_spice_model_mux t_spice_model_mux;
typedef struct s_spice_model_lut t_spice_model_lut;
typedef struct s_spice_model_buffer t_spice_model_buffer;
typedef struct s_spice_model_pass_gate_logic t_spice_model_pass_gate_logic;
typedef struct s_spice_model_port t_spice_model_port;
typedef struct s_spice_model_wire_param t_spice_model_wire_param;
typedef struct s_spice_model_tedge t_spice_model_tedge;
typedef struct s_spice_model_netlist t_spice_model_netlist;
typedef struct s_spice_model_design_tech_info t_spice_model_design_tech_info;
typedef struct s_spice_model_delay_info t_spice_model_delay_info;
typedef struct s_spice_model t_spice_model;
typedef struct s_spice_meas_params t_spice_meas_params;
typedef struct s_spice_stimulate_params t_spice_stimulate_params;
typedef struct s_spice_mc_variation_params t_spice_mc_variation_params;
typedef struct s_spice_mc_params t_spice_mc_params;
typedef struct s_spice_params t_spice_params;
typedef struct s_spice t_spice;
typedef struct s_spice_mux_arch t_spice_mux_arch;
typedef struct s_spice_mux_model t_spice_mux_model;
typedef struct s_sram_inf t_sram_inf;
typedef struct s_sram_inf_orgz t_sram_inf_orgz;
typedef struct s_spice_net_info t_spice_net_info;
typedef struct s_spicetb_info t_spicetb_info;
typedef struct s_conf_bit_info t_conf_bit_info;
typedef struct s_sram_orgz_info t_sram_orgz_info;

/* Struct defintions */
struct s_spice_transistor_type {
  enum e_spice_trans_type type;
  char* model_name;
  float chan_length;
  float min_width;
};

/* Properites for technology library*/
struct s_spice_tech_lib {
  enum e_spice_tech_lib_type type;
  char* transistor_type;
  char* path;
  float nominal_vdd;
  float io_vdd;
  float pn_ratio;
  char* model_ref;
  int num_transistor_type;
  t_spice_transistor_type* transistor_types;
};

struct s_spice_model_buffer {
  int exist;
  char* spice_model_name;
  char* location_map;

  t_spice_model* spice_model;
  enum e_spice_model_buffer_type type;
  float size;
  int tapered_buf; /*Valid only when this is a buffer*/
  int tap_buf_level;
  int f_per_stage;
};

struct s_spice_model_pass_gate_logic {
  enum e_spice_model_pass_gate_logic_type type;
  float nmos_size;
  float pmos_size;
  char* spice_model_name;
  t_spice_model* spice_model;
};

/* Model the pin-to-pin timing edge */
struct s_spice_model_tedge {
  float trise; /* Rise condition: delay */
  float tfall; /* Fall condition: delay */
  t_spice_model_port* from_port;
  int from_port_pin_number;
  t_spice_model_port* to_port;
  int to_port_pin_number;
};

struct s_spice_model_port {
  enum e_spice_model_port_type type;
  int size;
  char* prefix; 
  char* lib_name; 
  char* inv_prefix; 
  /* Mode select port properties */
  boolean mode_select;
  int default_val;
  /* Global port properties */
  boolean is_global;
  boolean is_reset;
  boolean is_set;
  boolean is_config_enable;
  boolean is_prog;
  /* The spice model that this port will be connected to */
  char* spice_model_name;
  t_spice_model* spice_model;
  char* inv_spice_model_name;
  t_spice_model* inv_spice_model;
  /* Tri-state map */
  char* tri_state_map;
  /* For frac_lut only */
  int lut_frac_level;
  int* lut_output_mask;
  /* Timing edeges linked to other t_model_ports */
  int* num_tedges; /* 1-D Array, show number of tedges of each pin */
  t_spice_model_tedge*** tedge; /* 3-D array, considering the each pin in this port, [pin_number][num_edges[iedge]] is an edge pointor */
};

struct s_spice_model_wire_param {
  enum e_wire_model_type type;
  float res_val;
  float cap_val; 
  int level;
};

struct s_spice_model_netlist {
  char* path;
  int included;
};

struct s_spice_model_rram {
  /* Vaild for RRAM technology only, and this is a mux*/
  float ron;
  float roff;
  float wprog_set_nmos;
  float wprog_set_pmos;
  float wprog_reset_nmos;
  float wprog_reset_pmos;
};

struct s_spice_model_mux {
  /* Mux information only */
  enum e_spice_model_structure structure;
  int mux_num_level;
  boolean add_const_input;
  int const_input_val;
  boolean advanced_rram_design;
};

struct s_spice_model_lut {
  /* LUT information */
  boolean frac_lut;
};

struct s_spice_model_gate {
  /* LUT information */
  enum e_spice_model_gate_type type;
};

/* Information about design technology */
struct s_spice_model_design_tech_info {
  /* Valid for SRAM technology */
  t_spice_model_buffer* buffer_info;
  t_spice_model_pass_gate_logic* pass_gate_info;
  t_spice_model_rram* rram_info;
  t_spice_model_mux* mux_info;
  t_spice_model_lut* lut_info;
  t_spice_model_gate* gate_info;
  /* Power gate information */
  boolean power_gated;
};

struct s_spice_model_delay_info {
  enum spice_model_delay_type type;
  char* in_port_name;
  char* out_port_name;
  char* value; 
};

struct s_spice_model {
  enum e_spice_model_type type;
  char* name;
  char* prefix; /* Prefix when it show up in the spice netlist */
  char* model_netlist; /* SPICE netlist provided by user */
  char* verilog_netlist; /* Verilog netlist provided by user */
  t_spice_model_netlist* include_netlist;
  int is_default;
  boolean dump_structural_verilog;
  boolean dump_explicit_port_map;

  /* type */
  enum e_spice_model_design_tech design_tech;
  t_spice_model_design_tech_info design_tech_info;
  /* END*/

  /* buffering information */
  t_spice_model_buffer* lut_input_buffer;
  t_spice_model_buffer* lut_input_inverter;
  t_spice_model_buffer* lut_intermediate_buffer;
  t_spice_model_buffer* input_buffer;
  t_spice_model_buffer* output_buffer;
  t_spice_model_pass_gate_logic* pass_gate_logic;

  /* Ports*/
  int num_port;
  t_spice_model_port* ports;

  /* Delay matrix */
  int num_delay_info;
  t_spice_model_delay_info* delay_info;

  /* Wire Model*/
  t_spice_model_wire_param* wire_param;

  /* Counter for print spice netlist*/
  int cnt;
  int tb_cnt;
  /* Grid index counter */
  int** grid_index_low;
  int** grid_index_high;
  /* CBX index counter */
  int** cbx_index_low;
  int** cbx_index_high;
  /* CBY index counter */
  int** cby_index_low;
  int** cby_index_high;
  /* SB index counter */
  int** sb_index_low;
  int** sb_index_high;
};

struct s_spice_meas_params {
  int auto_select_sim_num_clk_cycle;
  int sim_num_clock_cycle; /* Number of clock cycle in simulation */
  float accuracy;
  enum e_spice_accuracy_type accuracy_type;

  /* Upper/Lower threshold voltage for measuring slew (unit: percentage)*/
  /* Rising edge */
  float slew_upper_thres_pct_rise;
  float slew_lower_thres_pct_rise;
  /* Falling edge */
  float slew_upper_thres_pct_fall;
  float slew_lower_thres_pct_fall;

  /*Input/Output threshold voltage for measuring delay (unit: percentage) */
  /* Rising edge */
  float input_thres_pct_rise;
  float output_thres_pct_rise;
  /* Falling edge */
  float input_thres_pct_fall;
  float output_thres_pct_fall;
};

struct s_spice_stimulate_params {
  /* Clock slew (unit: percentage of clock freqency) */
  float clock_slew_rise_time;  
  float clock_slew_fall_time;  
  enum e_spice_accuracy_type clock_slew_rise_type;
  enum e_spice_accuracy_type clock_slew_fall_type;

  /* Input signal slew (unit: percentage of clock freqency) */
  float input_slew_rise_time;  
  float input_slew_fall_time;  
  enum e_spice_accuracy_type input_slew_rise_type;
  enum e_spice_accuracy_type input_slew_fall_type;
  
  /* clock freqency: could be custimized or following the estimated critical path */
  int num_clocks;
  float vpr_crit_path_delay; /* Reference operation clock frequency */
  float op_clock_freq; /* Operation clock frequency*/
  float prog_clock_freq; /* Programming clock frequency, used during programming phase only */
  /* Simulation Clock frequency slack: In this case, we follow the estimated critical path. 
   * For simulation, usually we use a slack that make sure the circuit can run... */
  float sim_clock_freq_slack;
};

struct s_spice_mc_variation_params {
  /* on/off, abs_variation and num_sigma */
  boolean variation_on;
  float abs_variation;
  int num_sigma;
};

struct s_spice_mc_params {
  boolean mc_sim;
  int num_mc_points;
  /* cmos and rram variation */
  t_spice_mc_variation_params cmos_variation;
  t_spice_mc_variation_params rram_variation;
  t_spice_mc_variation_params wire_variation;
};

struct s_spice_params {
  int sim_temp; /* Simulation Temperature*/
  int post;
  int captab;
  int fast;
  t_spice_meas_params meas_params;
  t_spice_stimulate_params stimulate_params;
  t_spice_mc_params mc_params;
};

struct s_spice {
  /* Parameters */
  t_spice_params spice_params;
  /* Included SPICE netlists */
  int num_include_netlist;
  t_spice_model_netlist* include_netlists; 
  /* Technical Library*/
  t_spice_tech_lib tech_lib;
  int num_spice_model;
  t_spice_model* spice_models;
};

/* Information needed to build a Multiplexer architecture*/
struct s_spice_mux_arch {
  enum e_spice_model_structure structure;
  int num_input; /* All Inputs including those connect to constant generator */
  int num_data_input; /* Inputs for multiplexing datapath signals*/ 
  int num_level;
  int num_input_basis;
  int num_input_last_level;
  int* num_input_per_level; /* [0...num_level] */
  int* input_level;  /* [0...num_input] */
  int* input_offset; /* [0...num_input] */ 
};

/* For Multiplexer size*/
struct s_spice_mux_model {
  int size;
  t_spice_model* spice_model;
  t_spice_mux_arch* spice_mux_arch;
  int cnt; /* Used in mux_testbench only*/
};

struct s_sram_inf_orgz {
  char* spice_model_name; // Xifan TANG: Spice Support
  t_spice_model* spice_model; // Xifan TANG: Spice Support
  enum e_sram_orgz type;
};

struct s_sram_inf {
  float area; //Xifan TANG
  t_sram_inf_orgz* verilog_sram_inf_orgz;
  t_sram_inf_orgz* spice_sram_inf_orgz;
};

/* Xifan TANG: SPICE net information */
struct s_spice_net_info {
  float probability;
  float density;
  /* The following paramters can be calculated by the above properties*/
  int init_val;
  float freq; 
  float pwl;
  float pwh;
  float slew_rise;
  float slew_fall;
};

struct s_spicetb_info {
  char* tb_name;
  int num_sim_clock_cycles;
};

/* A struct containing a syntax_char that is reserved by Verilog or SPICE */
typedef struct s_reserved_syntax_char t_reserved_syntax_char;
struct s_reserved_syntax_char {
  char syntax_char;
  boolean verilog_reserved;
  boolean spice_reserved;
};

/* A struct to contain Address and its value */
typedef struct s_conf_bit t_conf_bit;
struct s_conf_bit {
  int addr; /* Address to write the value */
  int val; /* binary value to be writtent: either 0 or 1 */
};

/* Data structure for storing configurtion bits*/
struct s_conf_bit_info {
  /* index in all the srams/bit lines/word lines */
  int index;
  /* value stored in a SRAM*/
  t_conf_bit* sram_bit;
  /* If bl and wl is required, this is the value to be stored */
  t_conf_bit* bl;
  t_conf_bit* wl;
  /* Which spice model this conf. bit belongs to */
  t_spice_model* parent_spice_model;
  int parent_spice_model_index;
  /* index of this conf_bit in a top-level testbench */
  int index_in_top_tb;
  /* TODO: add location information?
   * i.e. grid location? sb/cb location? 
   */
};

/* Structs including information about SRAM organization:
 * 1. Memory bank
 * 2. Scan-chain FFs
 * 3. Standalone SRAMs */
/* Memory bank information */
typedef struct s_mem_bank_info t_mem_bank_info;
struct s_mem_bank_info {
  t_spice_model* mem_model; /* SPICE model of a memory bit */
  int num_mem_bit; /* Number of memory bits in total */
  int num_bl; /* Number of Bit Lines in total */
  int num_wl; /* Number of Word Lines in total */

  /* Reserved control lines always starts from index 0*/
  int reserved_bl; /* Number of reserved BLs shared by overall RRAM circuits */
  int reserved_wl; /* Number of reserved WLs shared by overall RRAM circuits */
};

/* Scan-chain Flip-flops information */
typedef struct s_scff_info t_scff_info;
struct s_scff_info {
  t_spice_model* mem_model; /* SPICE model of a memory bit */
  int num_mem_bit; /* Number of memory bits in total */
  int num_scff; /* Number of Scan-chain flip-flops */
  /* TODO:  More to be added, SCFF support is naive now */
};

/* Standalone SRAMs information */
typedef struct s_standalone_sram_info t_standalone_sram_info;
struct s_standalone_sram_info {
  t_spice_model* mem_model; /* SPICE model of a memory bit */
  int num_mem_bit; /* Number of memory bits in total */
  int num_sram; /* Number of SRAMs in total */
};

struct s_sram_orgz_info {
  enum e_sram_orgz type;
  t_mem_bank_info* mem_bank_info; /* Only be allocated when orgz type is memory bank */
  t_scff_info* scff_info; /* Only be allocated when orgz type is scan-chain */
  t_standalone_sram_info* standalone_sram_info; /* Only be allocated when orgz type is standalone */
  
  /* Head of configuration bits,
   * which is assigned according to orgz_type */
  t_llist* conf_bit_head; 

  /* Conf bits information per grid */
  int grid_nx; /* grid size */ 
  int grid_ny;
  int** grid_reserved_conf_bits;
  int** grid_conf_bits_lsb;
  int** grid_conf_bits_msb;
};

/* SPICE support end*/
