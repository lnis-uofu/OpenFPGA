#ifndef MRFPGA_H
#define MRFPGA_H

/* mrFPGA specfications */
extern boolean is_isolation;
extern boolean is_stack;
extern boolean is_junction;
extern boolean is_wire_buffer;
extern boolean is_mrFPGA;
//extern boolean is_accurate; /* Xifan TANG: Abolish */
extern t_buffer_inf wire_buffer_inf;
extern t_memristor_inf memristor_inf;
extern int max_pins_per_side;
extern t_linked_int* main_best_buffer_list;
extern short num_normal_switch;
extern short start_seg_switch;
/* end */

/* bjxiao: show sram and pass transistor usage */
extern boolean is_show_sram, is_show_pass_trans;
//extern enum e_tech_comp tech_comp; /* Xifan TANG : abolish */
extern float Rseg_global, Cseg_global;
extern float rram_pass_tran_value;
/* end */

#endif
