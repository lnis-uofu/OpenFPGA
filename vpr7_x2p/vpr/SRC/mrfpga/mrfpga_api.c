#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* libarchfpga data structures*/
#include "util.h"
#include "physical_types.h"
/* END */

/* VPR data structures*/
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
/* END */

/* mrFPGA: Xifan TANG*/
#include "mrfpga_globals.h"
#include "mrfpga_util.h"
#include "buffer_insertion.h"
#include "cal_capacitance.h"
#include "mrfpga_api.h"

/***** Subroutines *****/
/* Copy the contents in arch_mrfpga to globals*/
void sync_arch_mrfpga_globals(t_arch_mrfpga arch_mrfpga) {
  /* Booleans */
  is_isolation = arch_mrfpga.is_isolation;
  is_stack = arch_mrfpga.is_stack;
  is_junction = arch_mrfpga.is_junction;
  is_wire_buffer = arch_mrfpga.is_wire_buffer;
  is_mrFPGA = arch_mrfpga.is_mrFPGA;

  /* Copy some struct*/
  wire_buffer_inf = arch_mrfpga.wire_buffer_inf;
  memristor_inf = arch_mrfpga.memristor_inf;
  max_pins_per_side = arch_mrfpga.max_pins_per_side;
  main_best_buffer_list = arch_mrfpga.main_best_buffer_list;
  num_normal_switch = arch_mrfpga.num_normal_switch;
  start_seg_switch = arch_mrfpga.start_seg_switch;
  
  /* SRAM and Pass Transistor Usage*/
  is_show_sram = arch_mrfpga.is_show_sram;
  is_show_pass_trans = arch_mrfpga.is_show_pass_trans;
  Rseg_global = arch_mrfpga.Rseg_global;
  Cseg_global = arch_mrfpga.Cseg_global;
  
  return;
}

/* Get the switch type for mrFPGA: Xifan TANG
 * This function is called rr_graph2.c but I move it here 
 * so that it is easy to see mrFPGA modifications
 */
void get_mrfpga_switch_type(boolean is_from_sbox,
                            boolean is_to_sbox,
                            short from_node_switch,
                            short to_node_switch,
                            short switch_types[2]) {
    /* This routine looks at whether the from_node and to_node want a switch,  *
     * and what type of switch is used to connect *to* each type of node       *
     * (from_node_switch and to_node_switch).  It decides what type of switch, *
     * if any, should be used to go from from_node to to_node.  If no switch   *
     * should be inserted (i.e. no connection), it returns OPEN.  Its returned *
     * values are in the switch_types array.  It needs to return an array      *
     * because one topology (a buffer in the forward direction and a pass      *
     * transistor in the backward direction) results in *two* switches.        */

    /* modified by bjxiao to make the pass transistors accepted */
    /* the original version has bugs */
    /* this version is copied from VPR4.3 */
    switch_types[0] = OPEN;  /* No switch */
    switch_types[1] = OPEN;

    if (!is_from_sbox && !is_to_sbox) {  /* No connection wanted in either dir */
        switch_types[0] = OPEN;
    }

    else if (is_from_sbox && !is_to_sbox) {  /* Only forward connection wanted */
        switch_types[0] = to_node_switch;  /* Type of switch to go *to* to_node */
    }

    else if (!is_from_sbox && is_to_sbox) {

        /* Only backward connection desired.  We're deciding whether or not to put *
         * in the forward connection.  Put it in if the backward connection uses   *
         * a bidirectional (pass transistor) switch.  Remember that the backward   *
         * connection uses a from_node_switch type of switch.                      */

        if ( switch_inf[from_node_switch].buffered == FALSE) {
            switch_types[0] = from_node_switch;
        }
    } else {
        /* Both a forward and a backward connection desired.  If the switch types   *
         * desired for the two connection are different, we have to reconcile them. */

        if (from_node_switch == to_node_switch) {
            switch_types[0] = to_node_switch;
        } else {    /* Different switch types.  Reconcile. */
            if (switch_inf[to_node_switch].buffered) {
                switch_types[0] = to_node_switch;
                if ( switch_inf[from_node_switch].buffered == FALSE) {
                    /* Buffer in forward direction, pass transistor in backward.  Put *
                     * in *two* edges.                                                */
                    switch_types[1] = from_node_switch;
                }
            } else {   /* Forward connection is a pass transistor. */
                if (switch_inf[from_node_switch].buffered) {
                    switch_types[0] = to_node_switch;
                } else {  

                    /* Both forward and backward connections use pass transistors. *
                     * use whichever one is larger, since you'll only physically   *
                     * build one switch.                                           */

                    if (switch_inf[to_node_switch].R < 
                            switch_inf[from_node_switch].R) {
                        switch_types[0] = to_node_switch;
                    } else if (switch_inf[from_node_switch].R < 
                            switch_inf[to_node_switch].R) {
                        switch_types[0] = from_node_switch;
                    } else {

                        /* Two pass transistors have the same R, but are have different *
                         * switch indices.  Use the one with lower index (arbitrarily), *
                         * to ensure both switches are of the same type (since you can  *
                         * only physically build one).  I'm being pretty dogmatic here. */

                        if (to_node_switch < from_node_switch) {
                            switch_types[0] = to_node_switch;
                        } else {
                            switch_types[0] = from_node_switch;
                        }
                    }
                }
            }
        }     /* End switch types are different */
    }   /* End both forward and backward connection desired. */
    /* end */
}

