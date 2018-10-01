#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"

/* mrFPGA specfications */
boolean is_isolation = FALSE;
boolean is_stack = FALSE;
boolean is_junction = FALSE;
boolean is_wire_buffer = FALSE;
boolean is_mrFPGA = FALSE;
t_buffer_inf wire_buffer_inf;
t_memristor_inf memristor_inf;
int max_pins_per_side;
t_linked_int* main_best_buffer_list;
short num_normal_switch;
short start_seg_switch;
/* end */

/* bjxiao: show sram and pass transistor usage */
boolean is_show_sram = FALSE, is_show_pass_trans = FALSE;
float Rseg_global, Cseg_global;
float rram_pass_tran_value = 0;
/* end */

