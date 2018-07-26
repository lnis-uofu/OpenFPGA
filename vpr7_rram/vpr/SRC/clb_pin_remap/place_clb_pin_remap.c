#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "net_delay.h"
#include "path_delay.h"

/* CLB_PIN_REMAP headers */
#include "clb_pin_remap_util.h"
#include "post_place_timing.h"
#include "place_clb_pin_remap.h"
/* END */

/* main function */
/* Try to remap CLB IPINs that are logic equivalent to optimize their routing delay.
 * We should not remap any direct interconnect(carry_chain)! 
 */
void try_clb_pin_remap_after_placement(t_det_routing_arch det_routing_arch,
                                       t_segment_inf* segment_inf,
                                       t_timing_inf timing_inf,
                                       int num_directs,
                                       t_direct_inf* directs) {
  float** net_delay = NULL;
  float** expected_net_delay = NULL;
  t_slack* slacks = NULL;
  t_chunk net_delay_ch = {NULL, 0, NULL};
  t_chunk expected_net_delay_ch = {NULL, 0, NULL};
  int num_pin_reqr_remap = 0;
  int num_unrouted_blk_pins = 0;
  int num_pin_remapped = 0;
  float critical_path_delay = 0.;
 
  /* Do much more accurate timing-analysis */
  /* Alloc */
  slacks = alloc_and_load_timing_graph(timing_inf);
  net_delay = alloc_net_delay(&net_delay_ch, clb_net, num_nets);
  /* estimate and load net delays according to the PIN locations*/
  load_post_place_net_delay(net_delay, num_blocks, block, num_nets, clb_net, 
                            det_routing_arch, segment_inf, 
                            timing_inf, num_directs, directs);
  /* Timing analysis*/
  load_timing_graph_net_delays(net_delay);
  do_timing_analysis(slacks, FALSE, FALSE, TRUE);
  /* Print critical path delay. */
  critical_path_delay = get_critical_path_delay();
  vpr_printf(TIO_MESSAGE_INFO, "\n");
  vpr_printf(TIO_MESSAGE_INFO, "Post-Placement estimated critical path delay: %g ns\n", critical_path_delay);
  /* Pre-Remap optimizatioin timing analysis finish*/

  /* Now we do the remap */
  mark_blk_pins_nets_sink_index(num_nets, clb_net, num_blocks, block);
  /* Generate suitable pin location list for each mapped CLB IPIN*/
  num_pin_reqr_remap = generate_nets_sinks_prefer_sides(num_nets, clb_net, num_blocks, block);
  vpr_printf(TIO_MESSAGE_INFO, "Detect %d CLB IPINs that need to be remapped.\n", num_pin_reqr_remap);
  num_unrouted_blk_pins = set_unroute_blk_pins_prefer_sides(num_blocks, block);
  vpr_printf(TIO_MESSAGE_INFO, "Detect %d CLB IPINs are open.\n", num_unrouted_blk_pins);
  /* Load expected net_delay */
  expected_net_delay = alloc_net_delay(&expected_net_delay_ch, clb_net, num_nets);
  /* Estimate and load expected net delay when all CLB IPINs are in prefer sides */
  load_expected_remapped_net_delay(expected_net_delay, num_blocks, block, num_nets, clb_net, 
                                   det_routing_arch, segment_inf, 
                                   timing_inf, num_directs, directs);
  /* Try to satify all the pins preferred side! */
  num_pin_remapped = sat_blks_pins_prefer_side(num_nets, clb_net, num_blocks, block, 
                                               net_delay, expected_net_delay, slacks);
  vpr_printf(TIO_MESSAGE_INFO, "Remap %d CLB IPINs.\n", num_pin_remapped);

  /* Post-Remap optimization timing analysis */
  /* estimate and load net delays according to the PIN locations*/
  load_post_place_net_delay(net_delay, num_blocks, block, num_nets, clb_net, 
                            det_routing_arch, segment_inf, 
                            timing_inf, num_directs, directs);
  /* Timing analysis*/
  load_timing_graph_net_delays(net_delay);
  /* Timing analysis*/
  load_timing_graph_net_delays(net_delay);
  do_timing_analysis(slacks, FALSE, FALSE, TRUE);
  /* Print critical path delay. */
  critical_path_delay = get_critical_path_delay();
  vpr_printf(TIO_MESSAGE_INFO, "\n");
  vpr_printf(TIO_MESSAGE_INFO, "After CLB_PIN_REMAP estimated critical path delay: %g ns\n", critical_path_delay);

  /*TODO: free slack, net_delay and expected_net_delay?*/
  free_net_delay(net_delay, &net_delay_ch);
  free_net_delay(expected_net_delay, &expected_net_delay_ch);
  free_timing_graph(slacks);
   
  return;
}

/* For each net sink, we assign its prefer sides */
int generate_nets_sinks_prefer_sides(int n_nets, t_net* nets,
                                     int n_blks, t_block* blk) {
  int inet, isink, iblk;
  int src_blk_index;
  int src_num_pins;
  enum e_side* src_pin_side = NULL;
  int* src_pin_index = NULL;
  int src_pin_class;
  int des_blk_index;
  int des_num_pins;
  enum e_side* des_pin_side = NULL;
  int* des_pin_index = NULL;
  int des_pin_class;

  int num_pin_reqr_remap = 0;
  int temp = 0;

  for (iblk = 0; iblk < n_blks; iblk++) {
    /* Malloc pin_prefer_side for each block */
    assert(NULL != blk[iblk].type);
    blk[iblk].pin_prefer_side = (int**)my_malloc(sizeof(int*)*(blk[iblk].type->num_pins));
  }
 
  for (inet = 0; inet < n_nets; inet++) {
    /* Alloc prefer_side */ 
    nets[inet].prefer_side = (int**)my_malloc(sizeof(int*)*(nets[inet].num_sinks + 1));
    for (isink = 0; isink < (nets[inet].num_sinks + 1); isink++) {
      /* Initialize */
      nets[inet].prefer_side[isink] = (int*)my_calloc(4, sizeof(int)); /* 4 sides: TOP, LEFT, BOTTOM, RIGHT */
    }
    /* Find source block location, side */
    src_blk_index = nets[inet].node_block[0];
    find_blk_net_type_pins(n_blks, blk, inet, src_blk_index,
                           &src_num_pins, &src_pin_side, &src_pin_index);
    /* Should be only 1 pins mapped! */
    assert(1 == src_num_pins);
    nets[inet].prefer_side[0][src_pin_side[0]] = 1;
    src_pin_class = blk[src_blk_index].type->pin_class[src_pin_index[0]];
    assert(DRIVER == blk[src_blk_index].type->class_inf[src_pin_class].type);
    /* Assign block pin_prefer_side */
    blk[src_blk_index].pin_prefer_side[nets[inet].node_block_pin[0]] = nets[inet].prefer_side[0];
    /* For each sink, find block location, side */
    for (isink = 1; isink < (nets[inet].num_sinks + 1); isink++) {
      des_blk_index = nets[inet].node_block[isink];
      /* Find destination block location, side */
      find_blk_net_type_pins(n_blks, blk, inet, des_blk_index,
                             &des_num_pins, &des_pin_side, &des_pin_index);
      assert(1 == des_num_pins);
      des_pin_class = blk[des_blk_index].type->pin_class[des_pin_index[0]];
      assert(RECEIVER == blk[des_blk_index].type->class_inf[des_pin_class].type);
      /* Decide prefered sides of one sink pin */
      set_blk_net_one_sink_prefer_side(nets[inet].prefer_side[isink], src_pin_side[0], 
                                       /* Already allocated, in subroutine, array is writable*/
                                       blk[src_blk_index],
                                       blk[des_blk_index]);
      temp = pin_side_count(nets[inet].prefer_side[isink]);
      assert((!(0 > temp))&&(temp < 4));
      /* Assign block pin_prefer_side */
      blk[des_blk_index].pin_prefer_side[nets[inet].node_block_pin[isink]] = nets[inet].prefer_side[isink];
      /* Statistics the pins should be remapped */
      if (0 == nets[inet].prefer_side[isink][des_pin_side[0]]) {
        num_pin_reqr_remap++;
      }
    }
  } 

  /* Free */
  my_free(src_pin_index);
  my_free(src_pin_side);
  my_free(des_pin_index);
  my_free(des_pin_side);
 
  return num_pin_reqr_remap;
}

int set_unroute_blk_pins_prefer_sides(int n_blk, t_block* blk) {
  int iblk, ipin, side;
  int num_unroute_blk_pins = 0;

  /* For OPEN block pins, we should complete the prefer_side */
  for (iblk = 0; iblk < n_blk; iblk++) { 
    assert(NULL != blk[iblk].type);
    /* By pass I/O type */
    if (IO_TYPE == blk[iblk].type) {
      continue;
    }
    /* Special for global pins or non-routed pins, prefer_side should be all-ones */
    for (ipin = 0; ipin < blk[iblk].type->num_pins; ipin++) {
      if (OPEN == blk[iblk].nets[ipin]) {
        /* malloc a pin_prefer_side */
        blk[iblk].pin_prefer_side[ipin] = (int*)my_malloc(sizeof(int)*4);
        for (side = 0; side < 4; side++) {
          blk[iblk].pin_prefer_side[ipin][side] = 1;
        }
        num_unroute_blk_pins++;
      }
    }
  }
 
  return num_unroute_blk_pins; 
}

void set_blk_net_one_sink_prefer_side(int* prefer_side, /* [0..3] array, should be allocated before */
                                      enum e_side src_pin_side, 
                                      t_block src_blk, 
                                      t_block des_blk) {
  switch (src_pin_side) {
  case TOP:
    set_src_top_side_net_one_sink_prefer_side(prefer_side, src_blk, des_blk);
    break;
  case RIGHT:
    set_src_right_side_net_one_sink_prefer_side(prefer_side, src_blk, des_blk);
    break;
  case BOTTOM:
    set_src_bottom_side_net_one_sink_prefer_side(prefer_side, src_blk, des_blk);
    break;
  case LEFT:
    set_src_left_side_net_one_sink_prefer_side(prefer_side, src_blk, des_blk);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid src_pin_side!\n", __FILE__, __LINE__);
    exit(1); 
  }

  return;
} 

/* Find prefered side for one sink pin when the source pin is on TOP side
 * Refer to this graph, src block locates the center
 *   --------     --------     --------
 *   |      |     |      |     |      |
 *   | Des0 |<--  | Des1 |  -->| Des2 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |    /|\     |     /|\
 *      |------------|-------------|
 *     \|/     |     |      |     \|/
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des3 |<-|  | SRC  |  |->| Des4 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *             |            |        
 *      -------------|--------------
 *     \|/     |    \|/     |     \|/
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des5 |<--->| Des6 |<--->| Des7 |
 *   |      |     |      |     |      |
 *   --------     --------     --------
 */
void set_src_top_side_net_one_sink_prefer_side(int* prefer_side, 
                                               t_block src_blk, t_block des_blk) {
  /* TODO: for none IO_TYPE, there should be no self connections*/
  assert(!((des_blk.x == src_blk.x)&&(des_blk.y == src_blk.y)&&(des_blk.z == src_blk.z)));
  /* Identify which position the des_blk, based on src_blk */
  if ((des_blk.x < src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 0 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 1 */
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 2 */
    prefer_side[LEFT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 3 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 4 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 5 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 6 */
    prefer_side[TOP] = 1;
    //prefer_side[RIGHT] = 1;
    //prefer_side[LEFT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 7 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  }

  return;
}

/* Find prefered side for one sink pin when the source pin is on RIGHT side
 * Refer to this graph, src block locates the center
 *   --------     --------     --------
 *   |      |     |      |     |      |
 *   | Des0 |<--  | Des1 |<--->| Des2 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |    /|\     |     /|\
 *      |--------------------------|
 *     \|/     |            |        
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des3 |<-|  | SRC  |--|->| Des4 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |            |        
 *      |--------------------------|
 *     \|/     |    \|/     |     \|/
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des5 |<--  | Des6 |<--->| Des7 |
 *   |      |     |      |     |      |
 *   --------     --------     --------
 */
void set_src_right_side_net_one_sink_prefer_side(int* prefer_side, 
                                                 t_block src_blk, t_block des_blk) {
  /* TODO: for none IO_TYPE, there should be no self connections*/
  assert(!((des_blk.x == src_blk.x)&&(des_blk.y == src_blk.y)&&(des_blk.z == src_blk.z)));
  /* Identify which position the des_blk, based on src_blk */
  if ((des_blk.x < src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 0 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 1 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 2 */
    prefer_side[LEFT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 3 */
    //prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
    //prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 4 */
    prefer_side[LEFT] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 5 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 6 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 7 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  }

  return;
}

/* Find prefered side for one sink pin when the source pin is on BOTTOM side
 * Refer to this graph, src block locates the center
 *   --------     --------     --------
 *   |      |     |      |     |      |
 *   | Des0 |<--  | Des1 |<--->| Des2 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |    /|\     |     /|\
 *      |--------------------------|
 *             |            |        
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des3 |<-|  | SRC  |  |->| Des4 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |     |      |     /|\
 *      |------------|-------------|
 *     \|/     |    \|/     |     \|/
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des5 |<--  | Des6 |  -->| Des7 |
 *   |      |     |      |     |      |
 *   --------     --------     --------
 */
void set_src_bottom_side_net_one_sink_prefer_side(int* prefer_side, 
                                                  t_block src_blk, t_block des_blk) {
  /* TODO: for none IO_TYPE, there should be no self connections*/
  assert(!((des_blk.x == src_blk.x)&&(des_blk.y == src_blk.y)&&(des_blk.z == src_blk.z)));
  /* Identify which position the des_blk, based on src_blk */
  if ((des_blk.x < src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 0 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 1 */
    //prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
    //prefer_side[LEFT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 2 */
    prefer_side[LEFT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 3 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 4 */
    prefer_side[LEFT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 5 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 6 */
    prefer_side[TOP] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 7 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  }

  return;
}

/* Find prefered side for one sink pin when the source pin is on LEFT side
 * Refer to this graph, src block locates the center
 *   --------     --------     --------
 *   |      |     |      |     |      |
 *   | Des0 |<--->| Des1 |  -->| Des2 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *     /|\     |    /|\     |     /|\
 *      |--------------------------|
 *             |            |     \|/ 
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des3 |<-|--| SRC  |  |->| Des4 |
 *   |      |  |  |      |  |  |      |
 *   --------  |  --------  |  --------
 *             |            |     /|\
 *      |--------------------------|
 *     \|/     |    \|/     |     \|/
 *   --------  |  --------  |  --------
 *   |      |  |  |      |  |  |      |
 *   | Des5 |<--->| Des6 |  -->| Des7 |
 *   |      |     |      |     |      |
 *   --------     --------     --------
 */
void set_src_left_side_net_one_sink_prefer_side(int* prefer_side, 
                                                t_block src_blk, t_block des_blk) {
  /* TODO: for none IO_TYPE, there should be no self connections*/
  assert(!((des_blk.x == src_blk.x)&&(des_blk.y == src_blk.y)&&(des_blk.z == src_blk.z)));
  /* Identify which position the des_blk, based on src_blk */
  if ((des_blk.x < src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 0 */
    prefer_side[RIGHT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 1 */
    prefer_side[BOTTOM] = 1;
    prefer_side[LEFT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y > src_blk.y)) {
    /* Des 2 */
    prefer_side[LEFT] = 1;
    prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 3 */
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y == src_blk.y)) {
    /* Des 4 */
    //prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
    //prefer_side[BOTTOM] = 1;
  } else if ((des_blk.x < src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 5 */
    prefer_side[TOP] = 1;
    prefer_side[RIGHT] = 1;
  } else if ((des_blk.x == src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 6 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  } else if ((des_blk.x > src_blk.x)&&(des_blk.y < src_blk.y)) {
    /* Des 7 */
    prefer_side[TOP] = 1;
    prefer_side[LEFT] = 1;
  }

  return;
}

/* Try to satify the prefer side of all the pins of all the blocks */
int sat_blks_pins_prefer_side(int n_nets, t_net* nets, 
                              int n_blks, t_block* blk,
                              float** net_delay, float** expected_net_delay, t_slack* slack) {
  int num_pin_remapped = 0;
  int iblk;

  /* For each block, try to satify the prefer side of each pin */
  for (iblk = 0; iblk < n_blks; iblk++) {
    /* Bypass IO_TYPE */
    if (IO_TYPE == blk[iblk].type) {
      continue;
    }
    num_pin_remapped += sat_one_blk_pins_prefer_side(n_nets, nets, &(blk[iblk]),
                                                     net_delay, expected_net_delay, slack);
  }

  return num_pin_remapped;
}
  
/* For one block, try to satify the prefer side of each pin */
int sat_one_blk_pins_prefer_side(int n_nets, t_net* nets, 
                                 t_block* target_blk,
                                 float** net_delay, float** expected_net_delay, t_slack* slack) {
  int num_pin_remapped = 0;
  int ipin, num_pins;
  int iclass;
  int* is_pin_conflict = NULL;
  int* cur_pin_side = NULL;

  assert(NULL != target_blk);
  assert(NULL != target_blk->type);
  num_pins = target_blk->type->num_pins;

  /* Bypass IO_TYPE */
  if (IO_TYPE == target_blk->type) {
    return 0;
  }

  /* Malloc */
  cur_pin_side = (int*)my_malloc(sizeof(int*)*num_pins);
  /* Find current pin sides*/
  for (ipin = 0; ipin < num_pins; ipin++) {
    /* Make sure each pin appear at only one side!*/
    cur_pin_side[ipin] = find_blk_net_pin_side((*target_blk), target_blk->z, ipin);
  }
  /* Check the number of conflicts: 
   * pin_side not in prefer side
   */
  /* Malloc and Initialize */
  is_pin_conflict = (int*)my_calloc(num_pins, sizeof(int));
  for (ipin = 0; ipin < num_pins; ipin++) {
    if (0 == target_blk->pin_prefer_side[ipin][cur_pin_side[ipin]]) {
      is_pin_conflict[ipin] = 1;
    }
  } 
  for (iclass = 0; iclass < target_blk->type->num_class; iclass++) {
    /* We care RECEIVER class only */
    if (RECEIVER != target_blk->type->class_inf[iclass].type) {
      continue;
    }
    num_pin_remapped += try_sat_one_blk_pin_class_prefer_side(target_blk, n_nets, nets, 
                                                              iclass, is_pin_conflict, cur_pin_side,
                                                              net_delay, expected_net_delay, slack);
  }

  /* Free */
  my_free(cur_pin_side);
  my_free(is_pin_conflict);

  return num_pin_remapped;
}

int try_sat_one_blk_pin_class_prefer_side(t_block* target_blk,
                                          int n_nets, t_net* nets,
                                          int class_index, 
                                          int* is_pin_conflict, int* cur_pin_side,
                                          float** net_delay, float** expected_net_delay, t_slack* slack) {
  int num_pin_remapped = 0;  
  int ipin, class_num_pins, blk_pin_index;
  int net_index, net_sink_index;
  int num_conflict = 0;
  int sorted_list_len = 0;
  int* sorted_conflict_pin_list = NULL;
  float* esti_delay_gain = NULL;
 
  assert(NULL != target_blk);
  assert(NULL != target_blk->type);

  /* We care RECEIVER class only */
  if (RECEIVER != target_blk->type->class_inf[class_index].type) {
    return num_pin_remapped;
  }
  
  class_num_pins = target_blk->type->class_inf[class_index].num_pins;
  /* If there is only one pin in this class, there is no need to remap */
  assert(0 < class_num_pins);
  if (2 > class_num_pins) {
    return num_pin_remapped;
  }
  /* Count the number of conflict in this class */
  num_conflict = count_blk_one_class_num_conflict(target_blk, class_index, is_pin_conflict);
  /* There is no conflict, directly return */
  if (0 == num_conflict) {
    return num_pin_remapped;
  }
  
  /* Now we try to solve all the conflicts */
  /* See the estimated delay gain */ 
  esti_delay_gain = (float*)my_malloc(sizeof(float)*class_num_pins);
  for (ipin = 0; ipin < class_num_pins; ipin++) { 
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    /* unroute net, bypass not conflict pins */
    if (0 == is_pin_conflict[blk_pin_index]) {
      esti_delay_gain[ipin] = -1.;
      continue;
    }
    /* Estimate delay gain on conflicted pins */
    net_index = target_blk->nets[blk_pin_index];
    net_sink_index = target_blk->nets_sink_index[blk_pin_index];
    assert(OPEN != net_index);
    assert(OPEN != net_sink_index);
    esti_delay_gain[ipin] = expected_net_delay[net_index][net_sink_index]/net_delay[net_index][net_sink_index] - 1;
    assert(!(esti_delay_gain[ipin] > 0)); /* Expected_net_delay should be smaller! */
  }
   
  /* Sort the conflict pins in the class by lowest slack */ 
  sorted_conflict_pin_list = sort_one_class_conflict_pins_by_low_slack(target_blk, class_index, 
                                                                       is_pin_conflict, &sorted_list_len, slack);
  assert(sorted_list_len == num_conflict);

  /* Try to solve each conflict pin */
  num_pin_remapped = 0;
  for (ipin = 0; ipin < num_conflict; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[sorted_conflict_pin_list[ipin]];
    if (1 == is_pin_conflict[blk_pin_index]) {
      num_pin_remapped += try_remap_blk_class_one_conflict_pin(target_blk, class_index, blk_pin_index,
                                                               n_nets, nets, cur_pin_side, is_pin_conflict,
                                                               esti_delay_gain, slack);
    }
  }
  
  /* Free */
  my_free(sorted_conflict_pin_list);
  my_free(esti_delay_gain);
  
  return num_pin_remapped;
}

/* Successful remap, return 1. Fail, return 0.
 * 1. Try to swap with a conflicted pin in the same class
 * 2. Try to swap with an unrouted pin in the same class
 * 3. Try to swap with a unconflicted pin in the same class
 */
int try_remap_blk_class_one_conflict_pin(t_block* target_blk, int class_index, int pin_index,
                                         int n_nets, t_net* nets,
                                         int* cur_pin_side, int* is_pin_conflict,
                                         float* esti_delay_gain, t_slack* slack) {
  int* prefer_sides = NULL;
  int num_pins;
  int ipin, blk_pin_index, side_swap;

  assert(NULL != target_blk);
  assert(NULL != target_blk->type);
  assert((!(0 > pin_index))&&(pin_index < target_blk->type->num_pins));
  assert((!(0 > class_index))&&(class_index < target_blk->type->num_class));
  /* ONLY support INPUT PINs */
  assert(RECEIVER == target_blk->type->class_inf[class_index].type);

  prefer_sides = target_blk->pin_prefer_side[pin_index];
  num_pins = target_blk->type->class_inf[class_index].num_pins;  

  if (!(1 == is_type_pin_in_class(target_blk->type, class_index, pin_index))) {
  printf("num_pin_in_class = %d\n", is_type_pin_in_class(target_blk->type, class_index, pin_index));
  assert(1 == is_type_pin_in_class(target_blk->type, class_index, pin_index));
  }
  assert(1 == is_pin_conflict[pin_index]);

  /* Try to swap with conflicted pins */ 
  for (ipin = 0; ipin < num_pins; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    assert((!(0 >cur_pin_side[blk_pin_index]))&&(cur_pin_side[blk_pin_index] < 4));
    /* conflicted_pin match prefer_side */
    if ((pin_index != blk_pin_index)&&(1 == is_pin_conflict[blk_pin_index])
     &&(1 == is_swap2pins_match_prefer_side(cur_pin_side[pin_index], prefer_sides,
                                            cur_pin_side[blk_pin_index], target_blk->pin_prefer_side[blk_pin_index]))) {
      /* Swap 2 CLB IPIN */
      swap_blk_same_class_2pins(target_blk, n_nets, nets, pin_index, blk_pin_index);
      /* Print DEBUG info */
      vpr_printf(TIO_MESSAGE_INFO, 
                 "SwapCase1: PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d])\n",
                 target_blk->x, target_blk->y, target_blk->z, pin_index, 
                 nets[target_blk->nets[pin_index]].name, target_blk->nets_sink_index[pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, 
                 "       with PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d]).\n",
                 target_blk->x, target_blk->y, target_blk->z, blk_pin_index, 
                 nets[target_blk->nets[blk_pin_index]].name, target_blk->nets_sink_index[blk_pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, "ipin=%d\n", ipin);
      /* Update is_conflict list */
      is_pin_conflict[blk_pin_index] = 0;
      is_pin_conflict[pin_index] = 0;
      /* Update cur_pin_side list */
      side_swap = cur_pin_side[blk_pin_index];
      cur_pin_side[blk_pin_index] = cur_pin_side[pin_index];
      cur_pin_side[pin_index] = side_swap;
      /* Finish, we can return */
      return 2;
    }
  } 

  /* Find all unrounted pins, try to swap */
  for (ipin = 0; ipin < num_pins; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    assert((!(0 >cur_pin_side[blk_pin_index]))&&(cur_pin_side[blk_pin_index] < 4));
    /* conflicted_pin match prefer_side */
    if ((pin_index != blk_pin_index)&&(0 == is_pin_conflict[blk_pin_index])&&(OPEN == target_blk->nets[blk_pin_index])
     &&(1 == is_swap2pins_match_prefer_side(cur_pin_side[pin_index], prefer_sides,
                                            cur_pin_side[blk_pin_index], target_blk->pin_prefer_side[blk_pin_index]))) {
      /* Swap 2 CLB IPIN */
      swap_blk_same_class_2pins(target_blk, n_nets, nets, pin_index, blk_pin_index);
      /* Print DEBUG info */
      vpr_printf(TIO_MESSAGE_INFO, 
                 "SwapCase2: PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d])\n",
                 target_blk->x, target_blk->y, target_blk->z, pin_index, 
                 nets[target_blk->nets[pin_index]].name, target_blk->nets_sink_index[pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, 
                 "       with PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d]).\n",
                 target_blk->x, target_blk->y, target_blk->z, blk_pin_index, 
                 nets[target_blk->nets[blk_pin_index]].name, target_blk->nets_sink_index[blk_pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, "ipin=%d\n", ipin);
      /* Update is_conflict list */
      is_pin_conflict[blk_pin_index] = 0;
      is_pin_conflict[pin_index] = 0;
      /* Update cur_pin_side list */
      side_swap = cur_pin_side[blk_pin_index];
      cur_pin_side[blk_pin_index] = cur_pin_side[pin_index];
      cur_pin_side[pin_index] = side_swap;
      /* Finish, we can return */
      return 1;
    }
  } 

  /* Find unconflicted routed pins, try to swap */
  for (ipin = 0; ipin < num_pins; ipin++) {
    blk_pin_index = target_blk->type->class_inf[class_index].pinlist[ipin];
    assert((!(0 >cur_pin_side[blk_pin_index]))&&(cur_pin_side[blk_pin_index] < 4));
    /* conflicted_pin match prefer_side */
    if ((pin_index != blk_pin_index)&&(0 == is_pin_conflict[blk_pin_index])&&(OPEN != target_blk->nets[blk_pin_index])
     /* TODO: if pin_index slack overwhelms blk_pin_index, shall we swap??? */
     &&(1 == 0)
     &&(1 == is_swap2pins_match_prefer_side(cur_pin_side[pin_index], prefer_sides,
                                            cur_pin_side[blk_pin_index], target_blk->pin_prefer_side[blk_pin_index]))) {
      /* Swap 2 CLB IPIN */
      swap_blk_same_class_2pins(target_blk, n_nets, nets, pin_index, blk_pin_index);
      /* Print DEBUG info */
      vpr_printf(TIO_MESSAGE_INFO, 
                 "SwapCase3: PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d])\n",
                 target_blk->x, target_blk->y, target_blk->z, pin_index, 
                 nets[target_blk->nets[pin_index]].name, target_blk->nets_sink_index[pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, 
                 "       with PIN(block x=%d, y=%d, z=%d, index=%d, net[%s].sink[%d]).\n",
                 target_blk->x, target_blk->y, target_blk->z, blk_pin_index, 
                 nets[target_blk->nets[blk_pin_index]].name, target_blk->nets_sink_index[blk_pin_index]);
      vpr_printf(TIO_MESSAGE_INFO, "ipin=%d\n", ipin);
      /* Update is_conflict list */
      is_pin_conflict[blk_pin_index] = 0;
      is_pin_conflict[pin_index] = 0;
      /* Update cur_pin_side list */
      side_swap = cur_pin_side[blk_pin_index];
      cur_pin_side[blk_pin_index] = cur_pin_side[pin_index];
      cur_pin_side[pin_index] = side_swap;
      /* Finish, we can return */
      return 2;
    }
  } 

  return 0;
}

