/*mrFPGA specifications for read_xml_arch: Xifan TANG*/
/* mrFPGA : Reshaped by Xifan TANG*/
typedef struct s_buffer_inf t_buffer_inf;
struct s_buffer_inf { 
  float C;
  float R;
  float Tdel;
}; 

typedef struct s_memristor_inf t_memristor_inf;
struct s_memristor_inf { 
  float C; 
  float R; 
  float Tdel; 
};

enum e_tech_comp { 
  CONV = 0, MONO, STTRAM, PCRAM_Xie, PCRAM_Pierre, NEM 
};
/* end */

typedef struct s_arch_mrfpga t_arch_mrfpga;
struct s_arch_mrfpga {
  boolean is_isolation;
  boolean is_stack;
  boolean is_junction;
  boolean is_wire_buffer;
  boolean is_mrFPGA;
  boolean is_accurate;
  t_buffer_inf wire_buffer_inf;
  t_memristor_inf memristor_inf;
  int max_pins_per_side;
  t_linked_int* main_best_buffer_list;
  short num_normal_switch;
  short start_seg_switch;

  /* show sram and pass transistor uasge*/
  boolean is_show_sram;
  boolean is_show_pass_trans;
  enum e_tech_comp tech_comp;
  float Rseg_global;
  float Cseg_global;
  float rram_pass_tran_value;

  /* timing info to override the opin_to_wire connection block */
  int is_opin_cblock_defined;
  float R_opin_cblock;
  float T_opin_cblock;
};
