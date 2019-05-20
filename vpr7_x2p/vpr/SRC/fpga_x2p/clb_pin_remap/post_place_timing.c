#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "net_delay_types.h"
#include "net_delay_local_void.h"
#include "net_delay.h"
#include "path_delay.h"
/* CLB PIN REMAP */
#include "clb_pin_remap_util.h"
#include "post_place_timing.h"

/* Xifan TANG: Copied from place.c */
/* Expected crossing counts for nets with different #'s of pins.  From *
 * ICCAD 94 pp. 690 - 695 (with linear interpolation applied by me).   *
 * Multiplied to bounding box of a net to better estimate wire length  *
 * for higher fanout nets. Each entry is the correction factor for the *
 * fanout index-1                                                      */
static const float local_cross_count[50] = { /* [0..49] */1.0, 1.0, 1.0, 1.0828, 1.1536, 1.2206, 1.2823, 1.3385, 1.3991, 1.4493, 1.4974,
		1.5455, 1.5937, 1.6418, 1.6899, 1.7304, 1.7709, 1.8114, 1.8519, 1.8924,
		1.9288, 1.9652, 2.0015, 2.0379, 2.0743, 2.1061, 2.1379, 2.1698, 2.2016,
		2.2334, 2.2646, 2.2958, 2.3271, 2.3583, 2.3895, 2.4187, 2.4479, 2.4772,
		2.5064, 2.5356, 2.5610, 2.5864, 2.6117, 2.6371, 2.6625, 2.6887, 2.7148,
		2.7410, 2.7671, 2.7933 };

/* Load and estimate post-placement net delays according to the PIN locations */
void load_post_place_net_delay(float** net_delay,
                               int n_blks,
                               t_block* blk,
                               int n_nets, 
                               t_net* nets,
                               t_det_routing_arch det_routing_arch,
                               t_segment_inf* segment_inf,
                               t_timing_inf timing_inf,
                               int num_directs,
                               t_direct_inf* directs) {
  int inet, isink;
  int src_blk_index, /*src_blk_port_index,*/ src_blk_pin_index;
  int des_blk_index, /*des_blk_port_index,*/ des_blk_pin_index;
  float penalty = 0.;

  /* Search each clb net, find the driver (OPIN) and all the receivers (IPINs) */
  for  (inet = 0; inet < n_nets; inet++) {
    /* Global nets, we don't optimize */
    if (TRUE == nets[inet].is_global) {
      load_one_constant_net_delay(net_delay, inet, nets, 0.);   
    } else {
      penalty = get_crossing_penalty(nets[inet].num_sinks);
      /* Spot driver (OPIN) location */ 
      src_blk_index = nets[inet].node_block[0]; 
      //src_blk_port_index = nets[inet].node_block_port[0]; 
      src_blk_pin_index = nets[inet].node_block_pin[0]; 
      /* Check the driver OPIN */
      check_src_blk_pin(n_blks, blk, inet, 
                        src_blk_index, 
                        //src_blk_port_index,
                        src_blk_pin_index);
      /* Search all the sinks and estimate route delay */
      for (isink = 1; isink < (nets[inet].num_sinks + 1); isink++) {
        des_blk_index = nets[inet].node_block[isink];
        //des_blk_port_index = nets[inet].node_block_port[isink];
        des_blk_pin_index = nets[inet].node_block_pin[isink];
        check_des_blk_pin(n_blks, blk, inet, 
                          des_blk_index, 
                          //des_blk_port_index,
                          des_blk_pin_index);
        /* Estimate the net delay */
        net_delay[inet][isink] = penalty *
                                 estimate_post_place_one_net_sink_delay(inet, n_blks, blk, src_blk_index, des_blk_index,
                                                                        det_routing_arch, segment_inf, timing_inf, 
                                                                        num_directs, directs); 
      }
    }
  } 

  return;
}

float estimate_post_place_one_net_sink_delay(int net_index, 
                                             int n_blks,
                                             t_block* blk,
                                             int src_blk_index,
                                             int des_blk_index,
                                             t_det_routing_arch det_routing_arch,
                                             t_segment_inf* segment_inf,
                                             t_timing_inf timing_inf,
                                             int num_directs,
                                             t_direct_inf* directs) {
  float esti_net_delay = -1.;
  int num_src_pins = 0;
  enum e_side* src_pin_side = NULL;
  int* src_pin_index = NULL;

  int num_des_pins = 0;
  enum e_side* des_pin_side;
  int* des_pin_index = NULL;

  int isrc, ides;
  float pin2pin_net_delay = 0.;
  
  /* ONLY APPLICABLE TO UNI_DIRECITONAL ROUTING ARCH. !!!*/
  if (UNI_DIRECTIONAL != det_routing_arch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d])Estimate post-placement net delay is ONLY applicable to uni-directional routint arch!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* Find the driver OPINs location */
  find_blk_net_type_pins(n_blks, blk, net_index, src_blk_index, 
                         &num_src_pins, &src_pin_side, &src_pin_index); 
  /* Find the receiver IPINs location */
  find_blk_net_type_pins(n_blks, blk, net_index, des_blk_index, 
                         &num_des_pins, &des_pin_side, &des_pin_index); 
  /* Search all possible path */
  assert(0 < num_src_pins);
  assert(0 < num_des_pins);
  /* Should be only 1 destination pin */
  assert(1 == num_des_pins);
  for (ides = 0; ides < num_des_pins; ides++) {
    /* Should be only 1 source pin */
    assert(1 == num_src_pins);
    /* Possible paths from sources */
    for (isrc = 0; isrc < num_src_pins; isrc++) {
      pin2pin_net_delay = esti_pin2pin_one_net_delay(blk[src_blk_index], src_pin_side[isrc], src_pin_index[isrc],
                                                     blk[des_blk_index], des_pin_side[ides], des_pin_index[ides],
                                                     det_routing_arch, segment_inf, timing_inf, num_directs, directs);
      /* Consider the worst case src->des pin delay*/
      if ((-1. == esti_net_delay)||(pin2pin_net_delay > esti_net_delay)) {
        esti_net_delay = pin2pin_net_delay;
      }
    }
  }

  return esti_net_delay;                                        
}

float esti_pin2pin_one_net_delay(t_block src_blk,
                                 int src_pin_side,
                                 int src_pin_index,
                                 t_block des_blk,
                                 int des_pin_side,
                                 int des_pin_index,
                                 t_det_routing_arch det_routing_arch,
                                 t_segment_inf* segment_inf,
                                 t_timing_inf timing_inf,
                                 int num_directs,
                                 t_direct_inf* directs) {
  int src_pin_x, src_pin_y;
  int des_pin_x, des_pin_y;
  int delta_x, delta_y;
  float horizen_delay, vertical_delay, path_delay;

  /* ONLY APPLICABLE TO UNI_DIRECITONAL ROUTING ARCH. !!!*/
  if (UNI_DIRECTIONAL != det_routing_arch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d])Estimate post-placement net delay is ONLY applicable to uni-directional routint arch!\n", __FILE__, __LINE__);
    exit(1);
  }
   
  /* Estimate the channel location of src and des pins */
  esti_pin_chan_coordinate(&src_pin_x, &src_pin_y, 
                           src_blk, src_pin_side, src_pin_index);
  esti_pin_chan_coordinate(&des_pin_x, &des_pin_y, 
                           des_blk, des_pin_side, des_pin_index);

  /* TODO: Special for direct connection, delayless switch is used !!!*/

  /* Search a possible path from SRC PIN to DES PIN */ 
  switch (src_pin_side) {
  case TOP:
  case BOTTOM:
    /* 1st step: go horizentally */
    delta_x = abs(src_pin_x - des_pin_x); 
    /* Pass through a number of SB MUX */
    horizen_delay = esti_distance_num_seg_delay(delta_x, det_routing_arch.num_segment, segment_inf, 1);
    /* 2nd step: go vertically */
    delta_y = abs(src_pin_y - des_pin_y); 
    /* Pass through a number of SB MUX */
    vertical_delay = esti_distance_num_seg_delay(delta_y, det_routing_arch.num_segment, segment_inf, 1);
    /* 3rd step: go through a CB MUX */
    path_delay = horizen_delay + vertical_delay 
               + switch_inf[det_routing_arch.wire_to_ipin_switch].R * switch_inf[det_routing_arch.wire_to_ipin_switch].Cout + switch_inf[det_routing_arch.wire_to_ipin_switch].Tdel;
    break;
  case LEFT:
  case RIGHT:
    /* 1st step: go vertically */
    delta_y = abs(src_pin_y - des_pin_y); 
    /* Pass through a number of SB MUX */
    vertical_delay = esti_distance_num_seg_delay(delta_y, det_routing_arch.num_segment, segment_inf, 1);
    /* 2nd step: go horizentally */
    delta_x = abs(src_pin_x - des_pin_x); 
    /* Pass through a number of SB MUX */
    horizen_delay = esti_distance_num_seg_delay(delta_x, det_routing_arch.num_segment, segment_inf, 1);
    /* 3rd step: go through a CB MUX */
    path_delay = horizen_delay + vertical_delay 
               + switch_inf[det_routing_arch.wire_to_ipin_switch].R * switch_inf[det_routing_arch.wire_to_ipin_switch].Cout + switch_inf[det_routing_arch.wire_to_ipin_switch].Tdel;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, LINE[%d])Invalid side!\n", __FILE__, __LINE__);
    exit(1);
  }

  return path_delay;
}
                                 
void load_expected_remapped_net_delay(float** net_delay,                                       
                                      int n_blks, 
                                      t_block* blk,
                                      int n_nets, 
                                      t_net* nets,
                                      t_det_routing_arch det_routing_arch,
                                      t_segment_inf* segment_inf,
                                      t_timing_inf timing_inf,
                                      int num_directs,
                                      t_direct_inf* directs) {
  int inet, isink, i;
  int src_blk_index, src_blk_pin_index, src_blk_pin_side;
  int des_blk_index, des_blk_pin_index, des_blk_pin_side;
  float esti_path_delay = -1.;
  float pin2pin_one_sink_delay = 0.;
  float penalty = 0.;

  /* Search each clb net, find the driver (OPIN) and all the receivers (IPINs) */
  for  (inet = 0; inet < n_nets; inet++) {
    /* Global nets, we don't optimize */
    if (TRUE == nets[inet].is_global) {
      load_one_constant_net_delay(net_delay, inet, nets, 0.);   
    } else {
      penalty =  get_crossing_penalty(nets[inet].num_sinks);
      /* Spot driver (OPIN) location */ 
      src_blk_index = nets[inet].node_block[0];
      src_blk_pin_index = nets[inet].node_block_pin[0];
      assert(NULL != blk[src_blk_index].type);
      src_blk_pin_side = find_blk_net_pin_side(blk[src_blk_index], blk[src_blk_index].type->pin_height[src_blk_pin_index], src_blk_pin_index);
      /* Search all the sinks and estimate route delay */
      for (isink = 1; isink < (nets[inet].num_sinks + 1); isink++) {
        des_blk_index = nets[inet].node_block[isink];
        des_blk_pin_index = nets[inet].node_block_pin[isink];
        assert(NULL != blk[des_blk_index].type);
        des_blk_pin_side = find_blk_net_pin_side(blk[des_blk_index], blk[des_blk_index].type->pin_height[des_blk_pin_index], des_blk_pin_index);
        for (i = 0; i < 4; i++) {
          if (0 == blk[des_blk_index].pin_prefer_side[des_blk_pin_index][i]) {
            continue; /* Bypass non-preferred side */
          }
          pin2pin_one_sink_delay = penalty *
                                   esti_pin2pin_one_net_delay(blk[src_blk_index], src_blk_pin_side, src_blk_pin_index,
                                                              blk[des_blk_index], des_blk_pin_side, des_blk_pin_index,
                                                              det_routing_arch, segment_inf, timing_inf, num_directs, directs);
          if ((-1. == esti_path_delay)||(esti_path_delay < pin2pin_one_sink_delay)) {
            esti_path_delay = pin2pin_one_sink_delay;
          }
        }
        assert(0. < esti_path_delay); 
        net_delay[inet][isink] = esti_path_delay;
      }
    }
  }
 
  return;
}

float esti_distance_num_seg_delay(int distance,
                                  int num_segment,
                                  t_segment_inf* segment_inf,
                                  int allow_long_segment) {
  float esti_delay = 0.;
  float min_out_range_esti_delay = 0.;
  float max_in_range_esti_delay = 0.;
  int iseg, min_out_range_seg, max_in_range_seg;

  /* Find the min-length segment whose length is larger than distance */
  min_out_range_seg = -1;
  for (iseg = 0; iseg < num_segment; iseg++) {
    if (segment_inf[iseg].length > distance) {
      if (-1 == min_out_range_seg) {
        min_out_range_seg = iseg;
      } else if (segment_inf[iseg].length < segment_inf[min_out_range_seg].length) {
        min_out_range_seg = iseg;
      } 
    }
  } 
  /* Find the max-length segment whose length is larger than distance */
  max_in_range_seg = -1;
  for (iseg = 0; iseg < num_segment; iseg++) {
    if ((segment_inf[iseg].length < distance)
       ||(segment_inf[iseg].length == distance)) {
      if (-1 == max_in_range_seg) {
        max_in_range_seg = iseg;
      } else if (segment_inf[iseg].length > segment_inf[max_in_range_seg].length) {
        max_in_range_seg = iseg;
      } 
    }
  } 
  
  /* Estimate the delay */
  if (-1 != max_in_range_seg) {
    max_in_range_esti_delay = esti_one_segment_net_delay(distance, segment_inf[max_in_range_seg]);
  }
  if (-1 != min_out_range_seg) {
    min_out_range_esti_delay = esti_one_segment_net_delay(distance, segment_inf[min_out_range_seg]);
  }

  /* If allow longer segment, we should consider in making decision */
  if (allow_long_segment) {
    if (min_out_range_esti_delay < max_in_range_esti_delay) {
      esti_delay = min_out_range_esti_delay;
    }
  } else {
    esti_delay = max_in_range_esti_delay;
  }
  
  return esti_delay;
}

float esti_one_segment_net_delay(int distance, t_segment_inf segment_inf) {
  int num_sb_mux = 0;
  int switch_index = segment_inf.opin_switch;
  float one_switch_delay = 0.;
  float one_segment_delay = 0.;
  float total_delay = 0.;
  int i;

  /* If segment length >= distance, only 1 SB MUX is required,
   * If segment length < distance, then distance/segment_length SB MUX needed 
   */
  if (segment_inf.length < distance) {
    num_sb_mux = distance/segment_inf.length;
    if (0 != distance%segment_inf.length) {
      num_sb_mux++;
    }
  } else {
    num_sb_mux = 1;
  }
  /* Find the driver switch */
  one_switch_delay = switch_inf[switch_index].R * switch_inf[switch_index].Cout + switch_inf[switch_index].Tdel;
  for (i = 0; i < segment_inf.length; i++) {
    one_segment_delay = segment_inf.Rmetal
                       *((segment_inf.length - i)*(segment_inf.Cmetal + 2*switch_inf[switch_index].Cin) + switch_inf[switch_index].Cin);
  }

  total_delay = num_sb_mux * (one_segment_delay + one_switch_delay);
  
  return total_delay;
}

float get_crossing_penalty(int num_sinks) {
  float crossing; 
  if (((num_sinks + 1) > 50)
      && ((num_sinks + 1) < 85)) {
    crossing = 2.7933 + 0.02616 * (num_sinks + 1) - 50;
  } else if ((num_sinks + 1) >= 85) {
    crossing = 2.7933 + 0.011 * (num_sinks + 1)
             - 0.0000018 * (num_sinks + 1)
             * (num_sinks + 1);
  } else {
    crossing = local_cross_count[(num_sinks + 1) - 1];
  }

  return crossing;
}
