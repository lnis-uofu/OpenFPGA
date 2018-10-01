

/* Xifan TANG: Spice support*/
enum e_spice_tech_lib_type {
  SPICE_LIB_INDUSTRY,SPICE_LIB_ACADEMIA
};

/* Transistor-level basic informations*/
enum e_spice_trans_type {
  SPICE_TRANS_NMOS, SPICE_TRANS_PMOS
};

typedef struct s_spice_transistor_type t_spice_transistor_type;
struct s_spice_transistor_type {
  enum e_spice_trans_type type;
  char* model_name;
  float chan_length;
  float min_width;
};

/* Properites for technology library*/
typedef struct s_spice_tech_lib t_spice_tech_lib;
struct s_spice_tech_lib {
  enum e_spice_tech_lib_type type;
  char* transistor_type;
  char* path;
  float nominal_vdd;
  float pn_ratio;
  char* model_ref;
  int num_transistor_type;
  t_spice_transistor_type* transistor_types;
};

/*Struct for a SPICE model of a module*/
enum e_spice_model_type {
  SPICE_MODEL_CHAN_WIRE, 
  SPICE_MODEL_WIRE, 
  SPICE_MODEL_MUX, 
  SPICE_MODEL_LUT, 
  SPICE_MODEL_FF, 
  SPICE_MODEL_INPAD, 
  SPICE_MODEL_OUTPAD, 
  SPICE_MODEL_SRAM, 
  SPICE_MODEL_HARDLOGIC
};

enum e_spice_model_design_tech {
  SPICE_MODEL_DESIGN_CMOS, 
  SPICE_MODEL_DESIGN_RRAM
};

enum e_spice_model_structure {
  SPICE_MODEL_STRUCTURE_TREE, 
  SPICE_MODEL_STRUCTURE_ONELEVEL, 
  SPICE_MODEL_STRUCTURE_MULTILEVEL 
};

enum e_spice_model_buffer_type {
  SPICE_MODEL_BUF_INV, 
  SPICE_MODEL_BUF_BUF
};

typedef struct s_spice_model_buffer t_spice_model_buffer;
struct s_spice_model_buffer {
  int exist;
  enum e_spice_model_buffer_type type;
  float size;
  int tapered_buf; /*Valid only when this is a buffer*/
  int tap_buf_level;
  int f_per_stage;
};

enum e_spice_model_pass_gate_logic_type {
  SPICE_MODEL_PASS_GATE_TRANSMISSION, SPICE_MODEL_PASS_GATE_TRANSISTOR
};

typedef struct s_spice_model_pass_gate_logic t_spice_model_pass_gate_logic;
struct s_spice_model_pass_gate_logic {
  enum e_spice_model_pass_gate_logic_type type;
  float nmos_size;
  float pmos_size;
};

enum e_spice_model_port_type {
  SPICE_MODEL_PORT_INPUT, 
  SPICE_MODEL_PORT_OUTPUT, 
  SPICE_MODEL_PORT_INOUT, 
  SPICE_MODEL_PORT_CLOCK, 
  SPICE_MODEL_PORT_SRAM
};

typedef struct s_spice_model_port t_spice_model_port;
struct s_spice_model_port {
  enum e_spice_model_port_type type;
  int size;
  char* prefix; 
};

enum e_wire_model_type {
  WIRE_MODEL_PIE,
  WIRE_MODEL_T
};

typedef struct s_spice_model_wire_param t_spice_model_wire_param;
struct s_spice_model_wire_param {
  enum e_wire_model_type type;
  float res_val;
  float cap_val; 
  int level;
};

typedef struct s_spice_model_netlist t_spice_model_netlist;
struct s_spice_model_netlist {
  char* path;
  int included;
};

typedef struct s_spice_model t_spice_model;
struct s_spice_model {
  enum e_spice_model_type type;
  char* name;
  char* prefix; /* Prefix when it show up in the spice netlist */
  char* model_netlist;
  t_spice_model_netlist* include_netlist;
  int is_default;

  enum e_spice_model_design_tech design_tech;
  enum e_spice_model_structure structure;
  int mux_num_level;
  /* Vaild for RRAM technology only, and this is a mux*/
  float ron;
  float roff;
  float prog_trans_size;
  /* END*/
  t_spice_model_buffer* lut_input_buffer;
  t_spice_model_buffer* input_buffer;
  t_spice_model_buffer* output_buffer;
  t_spice_model_pass_gate_logic* pass_gate_logic;
  /* Ports*/
  int num_port;
  t_spice_model_port* ports;

  /* Wire Model*/
  t_spice_model_wire_param* wire_param;

  /* Counter for print spice netlist*/
  int cnt;
  int tb_cnt;
  /* Grid index counter */
  int** grid_index_low;
  int** grid_index_high;
};

enum e_spice_accuracy_type {
  SPICE_FRAC, SPICE_ABS
};

typedef struct s_spice_meas_params t_spice_meas_params;
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

typedef struct s_spice_stimulate_params t_spice_stimulate_params;
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
  float clock_freq;
  /* Simulation Clock frequency slack: In this case, we follow the estimated critical path. 
   * For simulation, usually we use a slack that make sure the circuit can run... */
  float sim_clock_freq_slack;
};

typedef struct s_spice_params t_spice_params;
struct s_spice_params {
  int sim_temp; /* Simulation Temperature*/
  int post;
  int captab;
  int fast;
  t_spice_meas_params meas_params;
  t_spice_stimulate_params stimulate_params;
};

typedef struct s_spice t_spice;
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
typedef struct s_spice_mux_arch t_spice_mux_arch;
struct s_spice_mux_arch {
  int num_input;
  int num_level;
  int num_input_basis;
  int num_input_last_level;
  int* num_input_per_level; /* [0...num_level] */
  int* input_level;  /* [0...num_input] */
  int* input_offset; /* [0...num_input] */ 
};

/* For Multiplexer size*/
typedef struct s_spice_mux_model t_spice_mux_model;
struct s_spice_mux_model {
  int size;
  t_spice_model* spice_model;
  t_spice_mux_arch* spice_mux_arch;
  int cnt; /* Used in mux_testbench only*/
};

/* For SRAM */
typedef struct s_sram_inf t_sram_inf;
struct s_sram_inf {
  float area; //Xifan TANG
  char* spice_model_name; // Xifan TANG: Spice Support
  t_spice_model* spice_model; // Xifan TANG: Spice Support
};

/* Xifan TANG: SPICE net information */
typedef struct s_spice_net_info t_spice_net_info;
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

typedef struct s_spicetb_info t_spicetb_info;
struct s_spicetb_info {
  char* tb_name;
  int num_sim_clock_cycles;
};

/* SPICE support end*/
