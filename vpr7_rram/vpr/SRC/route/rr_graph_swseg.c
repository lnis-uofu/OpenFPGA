#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "rr_graph_sbox.h"
#include "check_rr_graph.h"
#include "rr_graph_timing_params.h"
#include "rr_graph_indexed_data.h"
#include "vpr_utils.h"
#include "read_xml_arch_file.h"
#include "ReadOptions.h"
#include "rr_graph_swseg.h"

/* Switch Segment Pattern Support
 * Xifan TANG, 
 * EPFL-IC-ISIM-LSI
 * July 2014
 */
enum ret_track_swseg_pattern {
  RET_SWSEG_TRACK_DIR_UNMATCH,
  RET_SWSEG_TRACK_NON_SEG_LEN_PATTERN,
  RET_SWSEG_TRACK_APPLIED
};

/** Basic idea:
 *  Add unbuffered multiplexers(switches) to the length-n segments
 *  For example: 
 *       clb      clb       clb       clb
 *   sb ------sb-------sb--------sb--------sb
 *   BUF      BUF      BUF       BUF       BUF
 *   The above is a length-1 wire. All the multiplexers in thes segments are buffered.
 *   In this work, we adapt the segments to the following story:
 *       clb      clb       clb       clb
 *   sb ------sb-------sb--------sb--------sb
 *   BUF     UNBUF    BUF       UNBUF     BUF
 *
 *   This is called a pattern. The length of the pattern is 2.
 *   Of cource, we can generalize it to length-n wire and define a pattern
 *   Pattern: 0 1 1 1 0, Segment length: 1, Pattern length 5
 *   The results are as follows:
 *       clb      clb       clb       clb
 *   sb ------sb-------sb--------sb--------sb
 *   BUF     UNBUF    UNBUF     UNBUF      BUF
 */

/** We check the routing tracks one by one to apply the pattern.
 *  Start from the source of coordinate systems    
 */


/*  The numbering relation between the channels and clbs is:				*
 *																	        *
 *  |    IO     | chan_   |   CLB     | chan_   |   CLB     |               *
 *  |grid[0][2] | y[0][2] |grid[1][2] | y[1][2] | grid[2][2]|               *
 *  +-----------+         +-----------+         +-----------+               *
 *                                                            } capacity in *
 *   No channel           chan_x[1][1]          chan_x[2][1]  } chan_width  *
 *                                                            } _x[1]       *
 *  +-----------+         +-----------+         +-----------+               *
 *  |           |  chan_  |           |  chan_  |           |               *
 *  |    IO     | y[0][1] |   CLB     | y[1][1] |   CLB     |               *
 *  |grid[0][1] |         |grid[1][1] |         |grid[2][1] |               *
 *  |           |         |           |         |           |               *
 *  +-----------+         +-----------+         +-----------+               *
 *                                                            } capacity in *
 *                        chan_x[1][0]          chan_x[2][0]  } chan_width  * 
 *                                                            } _x[0]       *
 *                        +-----------+         +-----------+               *
 *                 No     |           |	   No   |           |               *
 *               Channel  |    IO     | Channel |    IO     |               *
 *                        |grid[1][0] |         |grid[2][0] |               *
 *                        |           |         |           |               *
 *                        +-----------+         +-----------+               *
 *                                                                          *
 *               {=======}              {=======}                           *
 *              Capacity in            Capacity in                          *
 *            chan_width_y[0]        chan_width_y[1]                        *
 *                                                                          */

/****************************************************************************
 *                 =========chan_x [ix][iy]=========                        *
 *               ||        *---------------*        ||                      *
 *               ||        |               |        ||                      *
 *               ||        |               |        ||                      *
 *             chan_y      |      CLB      |      chan_y                    *
 *           [ix-1][iy]    |    [ix][iy]   |     [ix][iy]                   *
 *               ||        |               |        ||                      *
 *               ||        |               |        ||                      *
 *               ||        *---------------*        ||                      *
 *                 ========chan_x [ix][iy-1]========                        *
 ****************************************************************************
 *                          Directionality                                  *
 *                               /|\                                        *
 *                                |   INC                                   *
 *                                |                                         *
 *                    <---------      ---------->                           *
 *                      DEC       |        INC                              *
 *                                |                                         *
 *                                |   DEC                                   *
 *                               \|/                                        *
 ****************************************************************************
 */
/* We spot a routing track in channel (inx,iny). And start check its segment
 * length. When we got the segment length, then apply the pattern to this routing track.
 * And move on to the next. 
 * So, we would check the channels from [0][0] to [0][ny], [nx][0], [nx][ny] 
 */

/*****Subroutine Declaration*****/
static 
int init_chan_seg_detail_params(INP char* chan_type,
                                INP int curx,
                                INP int cury, 
                                INP int nx, // Width of FPGA
                                INP int ny, // Height of FPGA
                                INP t_seg_details* seg_details_x,
                                INP t_seg_details* seg_details_y,
                               OUTP int* seg_num,
                               OUTP int* chan_num,
                               //OUTP short* direction,
                               OUTP t_seg_details* seg_details,
                               OUTP int* max_len);

static t_swseg_pattern_inf* 
search_swseg_pattern_seg_len(INP int num_swseg_pattern,
                             INP t_swseg_pattern_inf* swseg_patterns,
                             INP int seg_len);

static enum ret_track_swseg_pattern 
    apply_swseg_pattern_chanx_track(INP int track_id,
                                    INP int chanx_y,
                                    INP int start_x,
                                    INP int end_x,
                                    INP int max_x,
                                    INP int max_y,
                                    INP short direction,
                                    INP int num_swseg_pattern,
                                    INP t_swseg_pattern_inf* swseg_patterns,
                                    INP t_ivec*** L_rr_node_indices,
                                    INP t_seg_details* seg_details_x,
                                    INP t_seg_details* seg_details_y,
                                    INP int* num_changed_chanx);

static enum ret_track_swseg_pattern 
    apply_swseg_pattern_chany_track(INP int track_id,
                                    INP int chany_x,
                                    INP int start_y,
                                    INP int end_y,
                                    INP int max_x,
                                    INP int max_y,
                                    INP short direction,
                                    INP int num_swseg_pattern,
                                    INP t_swseg_pattern_inf* swseg_patterns,
                                    INP t_ivec*** L_rr_node_indices,
                                    INP t_seg_details* seg_details_x,
                                    INP t_seg_details* seg_details_y,
                                    INP int* num_changed_chany);

static int swseg_pattern_change_switch_type(int inode,
                                            t_rr_type chan_type,
                                            t_swseg_pattern_inf swseg_pattern,
                                            int* num_unbuf_mux);

/*Update the driver_switch of all rr_nodes*/
void update_rr_nodes_driver_switch(enum e_directionality directionality);

static boolean* rotate_shift_swseg_pattern(int pattern_length,
                                           boolean* pattern,
                                           int rotate_shift_length);

/*Start : The top function*/
int add_rr_graph_switch_segment_pattern(enum e_directionality directionality,
                                        INP int nodes_per_chan,
                                        INP int num_swseg_pattern,
                                        INP t_swseg_pattern_inf* swseg_patterns,
                                        INP t_ivec*** L_rr_node_indices,
                                        INP t_seg_details* seg_details_x,
                                        INP t_seg_details* seg_details_y) {
  int ix, iy, itrack; 
  int num_changed_chanx, num_changed_chany;
  t_seg_details* seg_details;
  
  /*Initialization*/
  seg_details = NULL;
  num_changed_chanx = 0;
  num_changed_chany = 0;

  // Initialization
  update_rr_nodes_driver_switch(directionality);
  /* CHAN_X segments
   * We start from chan_x [1...nx-1][0] to chan_x [1...nx-1][ny-1]. 
   * They are the start points of routing tracks in CHANX
   */ 
  /* Ensure we have channel width > 0*/
  assert(nodes_per_chan > 0);
  seg_details = seg_details_x;
  for (iy = 0; iy < (ny + 1); iy++) {
    /* Check each routing track in a channel*/
    for (itrack = 0; itrack < nodes_per_chan; itrack++) {
      if (INC_DIRECTION == seg_details[itrack].direction) {
        apply_swseg_pattern_chanx_track(itrack, iy, 0, nx, nx, ny, INC_DIRECTION,
                                        num_swseg_pattern, swseg_patterns, L_rr_node_indices, seg_details_x, seg_details_y,
                                        &num_changed_chanx);
      } else if (DEC_DIRECTION == seg_details[itrack].direction) {
        apply_swseg_pattern_chanx_track(itrack, iy, nx, 0, nx, ny, DEC_DIRECTION,
                                        num_swseg_pattern, swseg_patterns, L_rr_node_indices, seg_details_x, seg_details_y,
                                        &num_changed_chanx);
      }
    } // Time to change another channel§
  }
  /* CHAN_Y segments
   * We start from chan_y [0][1...ny-1] to chan_y [nx-1][1...ny-1]. 
   * They are the start points of routing tracks in CHANX
   */ 
  seg_details = seg_details_y;
  for (ix = 0; ix < (nx + 1); ix++) {
    /* Check each routing track in a channel*/
    for (itrack = 0; itrack < nodes_per_chan; itrack++) {
      if (INC_DIRECTION == seg_details[itrack].direction) {
        apply_swseg_pattern_chany_track(itrack, ix, 0, ny, nx, ny, INC_DIRECTION,
                                        num_swseg_pattern, swseg_patterns, L_rr_node_indices, seg_details_x, seg_details_y,
                                        &num_changed_chany);
      } else if (DEC_DIRECTION == seg_details[itrack].direction) {
        apply_swseg_pattern_chany_track(itrack, ix, ny, 0, nx, ny, DEC_DIRECTION,
                                        num_swseg_pattern, swseg_patterns, L_rr_node_indices, seg_details_x, seg_details_y,
                                        &num_changed_chany);
      }
    } // Time to change another channel§
  }

  // Show Statistics
  vpr_printf(TIO_MESSAGE_INFO,"Switch Segment Pattern applied to %d Channel X.\n",num_changed_chanx);
  vpr_printf(TIO_MESSAGE_INFO,"Switch Segment Pattern applied to %d Channel Y.\n",num_changed_chany);

  return 1; // Success
}

// This part copied from function: view_mux_size_distribution
/* Determine seg_num, chan_num, direction, seg_details, max_len
 * according to the side of FPGA
 */
static int init_chan_seg_detail_params(INP char* chan_type,
                                       INP int curx,
                                       INP int cury, 
                                       INP int max_x, // Width of FPGA
                                       INP int max_y, // Height of FPGA
                                       INP t_seg_details* seg_details_x,
                                       INP t_seg_details* seg_details_y,
                                       OUTP int* seg_num,
                                       OUTP int* chan_num,
                                     //OUTP short* direction,
                                       OUTP t_seg_details* seg_details,
                                       OUTP int* max_len) {
  /*Initialization*/
  (*seg_num) = 0;
  (*chan_num) = 0;
  //(*direction) = 0;
  seg_details = NULL;
  (*max_len) = 0;

  if (0 == strcmp(chan_type,"chanx_inc")) {
    (*seg_num) = curx;
    (*chan_num) = cury;
    //(*direction) = INC_DIRECTION;
    seg_details = seg_details_x;
    (*max_len) = max_x;
  } else if (0 == strcmp(chan_type,"chanx_dec")) {
    (*seg_num) = curx;
    (*chan_num) = cury;
    //(*direction) = DEC_DIRECTION;
    seg_details = seg_details_x;
    (*max_len) = max_x;
  } else if (0 == strcmp(chan_type,"chany_inc")) {
    (*seg_num) = cury;
    (*chan_num) = curx;
    //(*direction) = INC_DIRECTION;
    seg_details = seg_details_y;
    (*max_len) = max_y;
  } else if (0 == strcmp(chan_type,"chany_dec")) {
    (*seg_num) = cury;
    (*chan_num) = curx;
    //(*direction) = DEC_DIRECTION;
    seg_details = seg_details_y;
    (*max_len) = max_y;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "Input channel type: %s. Expect [chanx_inc|chanx_dec|chany_inc|chany_dec].\n",chan_type);
    exit(1);
  }
  return 1;
}

/** Select a pattern according to the segment length.
 *  Return NULL if nothing found
 */
static 
t_swseg_pattern_inf* search_swseg_pattern_seg_len(INP int num_swseg_pattern,
                                                  INP t_swseg_pattern_inf* swseg_patterns,
                                                  INP int seg_len) {
  int ipattern;
  t_swseg_pattern_inf* ret = NULL;

  for (ipattern = 0; ipattern < num_swseg_pattern; ipattern++) {
    if (seg_len == swseg_patterns[ipattern].seg_length) {
      ret = &swseg_patterns[ipattern];
    } 
  }
  return ret;
}

/** Apply patterns to all the segments of a track in a chanx
 */
static enum ret_track_swseg_pattern 
    apply_swseg_pattern_chanx_track(INP int track_id,
                                    INP int chanx_y,
                                    INP int start_x,
                                    INP int end_x,
                                    INP int max_x,
                                    INP int max_y,
                                    INP short direction,
                                    INP int num_swseg_pattern,
                                    INP t_swseg_pattern_inf* swseg_patterns,
                                    INP t_ivec*** L_rr_node_indices,
                                    INP t_seg_details* seg_details_x,
                                    INP t_seg_details* seg_details_y,
                                    INP int* num_changed_chanx) {
  int ix, start, end, seg_len;
  int seg_num, chan_num, max_len, inode; 
  t_seg_details* seg_details;
  t_swseg_pattern_inf* select_swseg_pattern;
  int swseg_offset;
  boolean* rotated_pattern;
  
  /*Initialization*/
  seg_len = 0;
  seg_details = seg_details_x;
  seg_num = 0;
  chan_num = 0;
  max_len = 0;
  select_swseg_pattern = NULL;
  swseg_offset = 0;
  rotated_pattern = NULL;

  /* Check directions and ranges*/ 
  assert((INC_DIRECTION == direction)||(DEC_DIRECTION == direction));
  if (INC_DIRECTION == direction) {
    assert((!(start_x < 0))&&(!(end_x > max_x))&&(start_x < end_x));
  } else {
    assert((!(end_x < 0))&&(!(start_x > max_x))&&(start_x > end_x)&&(DEC_DIRECTION == direction));
  }

  // Check if the track direction matches
  if (direction != seg_details[track_id].direction) {
    return RET_SWSEG_TRACK_DIR_UNMATCH; // Direction does not match. Ignore the track
  }
  // Direction match, try to apply the Switch Segment Pattern
  if (INC_DIRECTION == direction) {
    for (ix = start_x; ix < (end_x + 1); ix++) {
      init_chan_seg_detail_params("chanx_inc", ix, chanx_y, max_x, max_y, seg_details_x, seg_details_y, /* INPUT list */
                                  &seg_num, &chan_num, seg_details, &max_len); /* OUTPUT list */
      /* Get the start point and end point of the segment*/
      start = get_seg_start(seg_details, track_id, chan_num, seg_num); 
      end = get_seg_end(seg_details, track_id, start, chan_num, max_len);
      assert((start > 0)||(0 == start));
      assert((end > 0)||(0 == end));
      /* Get the segment length*/
      seg_len = end - start + 1;
      /* Search a pattern*/
      select_swseg_pattern = search_swseg_pattern_seg_len(num_swseg_pattern, swseg_patterns, seg_len); 
      /* Nothing found, continue to the next track*/
      if (NULL == select_swseg_pattern) {
        return RET_SWSEG_TRACK_NON_SEG_LEN_PATTERN; // Do not need to apply any pattern
      }
      /* Rotate it*/
      rotated_pattern = rotate_shift_swseg_pattern(select_swseg_pattern->pattern_length,select_swseg_pattern->patterns,chanx_y); 
      /* Check it the segment starts here*/
      if ((start == seg_num)&&(direction == INC_DIRECTION)) {
        /* Get the corresponding rr_node index of CHANX MUX*/
        inode = get_rr_node_index(seg_num, chan_num, CHANX, track_id, L_rr_node_indices);
        /* Check if we need to change the switch to a unbuffered one*/
        if (TRUE == rotated_pattern[swseg_offset]) {
          swseg_pattern_change_switch_type(inode,CHANX,(*select_swseg_pattern), num_changed_chanx);
        }
        /* Update the swseg_offset*/ 
        swseg_offset++;
        if ((swseg_offset > select_swseg_pattern->pattern_length)
          ||(swseg_offset == select_swseg_pattern->pattern_length)) {
          swseg_offset = 0;
        }
      }
      if ((rotated_pattern != NULL)&&(rotated_pattern != select_swseg_pattern->patterns)) {
        free(rotated_pattern);
      }
    }
  } else if (DEC_DIRECTION == direction) {
    for (ix = start_x; ix > (end_x - 1); ix--) {
      init_chan_seg_detail_params("chanx_dec", ix, chanx_y, max_x, max_y, seg_details_x, seg_details_y, /* INPUT list */
                                  &seg_num, &chan_num, seg_details, &max_len); /* OUTPUT list */
      /* Get the start point and end point of the segment*/
      start = get_seg_start(seg_details, track_id, chan_num, seg_num); 
      end = get_seg_end(seg_details, track_id, start, chan_num, max_len);
      assert((start > 0)||(0 == start));
      assert((end > 0)||(0 == end));
      /* Get the segment length*/
      seg_len = end - start + 1;
      /* Search a pattern*/
      select_swseg_pattern = search_swseg_pattern_seg_len(num_swseg_pattern, swseg_patterns, seg_len); 
      /* Nothing found, continue to the next track*/
      if (NULL == select_swseg_pattern) {
        return RET_SWSEG_TRACK_NON_SEG_LEN_PATTERN; // Do not need to apply any pattern
      }
      /* Rotate it*/
      rotated_pattern = rotate_shift_swseg_pattern(select_swseg_pattern->pattern_length,select_swseg_pattern->patterns,chanx_y); 
      /* Check it the segment starts here*/
      if ((end == seg_num)&&(direction == DEC_DIRECTION)) {
        /* Get the corresponding rr_node index of CHANX MUX*/
        inode = get_rr_node_index(seg_num, chan_num, CHANX, track_id, L_rr_node_indices);
        /* Check if we need to change the switch to a unbuffered one*/
        if (TRUE == rotated_pattern[swseg_offset]) {
          swseg_pattern_change_switch_type(inode,CHANX,(*select_swseg_pattern), num_changed_chanx);
        }
        /* Update the swseg_offset*/ 
        swseg_offset++;
        if ((swseg_offset > select_swseg_pattern->pattern_length)
          ||(swseg_offset == select_swseg_pattern->pattern_length)) {
          swseg_offset = 0;
        }
      }
      if ((rotated_pattern != NULL)&&(rotated_pattern != select_swseg_pattern->patterns)) {
        free(rotated_pattern);
      }
    } 
  }

  return RET_SWSEG_TRACK_APPLIED;
}

/** Apply patterns to all the segments of a track in a chany
 */
static enum ret_track_swseg_pattern 
    apply_swseg_pattern_chany_track(INP int track_id,
                                    INP int chany_x,
                                    INP int start_y,
                                    INP int end_y,
                                    INP int max_x,
                                    INP int max_y,
                                    INP short direction,
                                    INP int num_swseg_pattern,
                                    INP t_swseg_pattern_inf* swseg_patterns,
                                    INP t_ivec*** L_rr_node_indices,
                                    INP t_seg_details* seg_details_x,
                                    INP t_seg_details* seg_details_y,
                                    INP int* num_changed_chany) {
  int iy, start, end, seg_len;
  int seg_num, chan_num, max_len, inode; 
  t_seg_details* seg_details;
  t_swseg_pattern_inf* select_swseg_pattern;
  int swseg_offset;
  boolean* rotated_pattern= NULL;
  
  /*Initialization*/
  seg_len = 0;
  seg_details = seg_details_y;
  seg_num = 0;
  chan_num = 0;
  max_len = 0;
  select_swseg_pattern = NULL;
  swseg_offset = 0;

  /* Check directions and ranges*/ 
  assert((INC_DIRECTION == direction)||(DEC_DIRECTION == direction));
  if (INC_DIRECTION == direction) {
    assert((!(start_y < 0))&&(!(end_y > max_y))&&(start_y < end_y));
  } else {
    assert((!(end_y < 0))&&(!(start_y > max_y))&&(start_y > end_y)&&(DEC_DIRECTION == direction));
  }

  // Check if the track direction matches
  if (direction != seg_details[track_id].direction) {
    return RET_SWSEG_TRACK_DIR_UNMATCH; // Direction does not match. Ignore the track
  }
  // Direction match, try to apply the Switch Segment Pattern
  if (INC_DIRECTION == direction) {
    for (iy = start_y; iy < (end_y + 1); iy++) {
      init_chan_seg_detail_params("chany_inc", chany_x, iy, max_x, max_y, seg_details_x, seg_details_y, /* INPUT list */
                                  &seg_num, &chan_num, seg_details, &max_len); /* OUTPUT list */
      /* Get the start point and end point of the segment*/
      start = get_seg_start(seg_details, track_id, chan_num, seg_num); 
      end = get_seg_end(seg_details, track_id, start, chan_num, max_len);
      assert((start > 0)||(0 == start));
      assert((end > 0)||(0 == end));
      /* Get the segment length*/
      seg_len = end - start + 1;
      /* Search a pattern*/
      select_swseg_pattern = search_swseg_pattern_seg_len(num_swseg_pattern, swseg_patterns, seg_len); 
      /* Nothing found, continue to the next track*/
      if (NULL == select_swseg_pattern) {
        return RET_SWSEG_TRACK_NON_SEG_LEN_PATTERN; // Do not need to apply any pattern
      }
      /* Rotate it*/
      rotated_pattern = rotate_shift_swseg_pattern(select_swseg_pattern->pattern_length,select_swseg_pattern->patterns,chany_x); 
      /* Check it the segment starts here*/
      if ((start == seg_num)&&(direction == INC_DIRECTION)) {
        /* Get the corresponding rr_node index of CHANX MUX*/
        inode = get_rr_node_index(chan_num, seg_num, CHANY, track_id, L_rr_node_indices);
        /* Check if we need to change the switch to a unbuffered one*/
        if (TRUE == rotated_pattern[swseg_offset]) {
          swseg_pattern_change_switch_type(inode,CHANY,(*select_swseg_pattern), num_changed_chany);
        }
        /* Update the swseg_offset*/ 
        swseg_offset++;
        if ((swseg_offset > select_swseg_pattern->pattern_length)
          ||(swseg_offset == select_swseg_pattern->pattern_length)) {
          swseg_offset = 0;
        }
      }
      if ((rotated_pattern != NULL)&&(rotated_pattern != select_swseg_pattern->patterns)) {
        free(rotated_pattern);
      }
    }
  } else if (DEC_DIRECTION == direction) {
    for (iy = start_y; iy > (end_y - 1); iy--) {
      init_chan_seg_detail_params("chany_dec", chany_x, iy, max_x, max_y, seg_details_x, seg_details_y, /* INPUT list */
                                  &seg_num, &chan_num, seg_details, &max_len); /* OUTPUT list */
      /* Get the start point and end point of the segment*/
      start = get_seg_start(seg_details, track_id, chan_num, seg_num); 
      end = get_seg_end(seg_details, track_id, start, chan_num, max_len);
      assert((start > 0)||(0 == start));
      assert((end > 0)||(0 == end));
      /* Get the segment length*/
      seg_len = end - start + 1;
      /* Search a pattern*/
      select_swseg_pattern = search_swseg_pattern_seg_len(num_swseg_pattern, swseg_patterns, seg_len); 
      /* Nothing found, continue to the next track*/
      if (NULL == select_swseg_pattern) {
        return RET_SWSEG_TRACK_NON_SEG_LEN_PATTERN; // Do not need to apply any pattern
      }
      /* Rotate it*/
      rotated_pattern = rotate_shift_swseg_pattern(select_swseg_pattern->pattern_length,select_swseg_pattern->patterns,chany_x); 
      /* Check it the segment starts here*/
      if ((end == seg_num)&&(direction == DEC_DIRECTION)) {
        /* Get the corresponding rr_node index of CHANX MUX*/
        inode = get_rr_node_index(chan_num, seg_num, CHANY, track_id, L_rr_node_indices);
        /* Check if we need to change the switch to a unbuffered one*/
        if (TRUE == rotated_pattern[swseg_offset]) {
          swseg_pattern_change_switch_type(inode,CHANY,(*select_swseg_pattern), num_changed_chany);
        }
        /* Update the swseg_offset*/ 
        swseg_offset++;
        if ((swseg_offset > select_swseg_pattern->pattern_length)
          ||(swseg_offset == select_swseg_pattern->pattern_length)) {
          swseg_offset = 0;
        }
      }
      if ((rotated_pattern != NULL)&&(rotated_pattern != select_swseg_pattern->patterns)) {
        free(rotated_pattern);
      }
    } 
  }

  return RET_SWSEG_TRACK_APPLIED;
}


/* Change the type of switch to a unbuffered one*/
static 
int swseg_pattern_change_switch_type(int cur_node,
                                     t_rr_type chan_type,
                                     t_swseg_pattern_inf swseg_pattern,
                                     int* num_unbuf_mux) {
  int iedge, jedge;
  int inode, to_node;
  
  assert((CHANX == chan_type)||(CHANY == chan_type));
  /*Find the rr_node*/
  assert(rr_node+cur_node);
  
  for (iedge = 0; iedge < rr_node[cur_node].num_edges; iedge++) {
    to_node = rr_node[cur_node].edges[iedge];
    if (chan_type == rr_node[to_node].type) { // Only apply to the matched type
      rr_node[cur_node].switches[iedge] = swseg_pattern.unbuf_switch;
      // Define the driver switch for routing stats
      rr_node[to_node].unbuf_switched = 1;
      rr_node[to_node].driver_switch = rr_node[cur_node].switches[iedge];
      (*num_unbuf_mux)++;
      /* We should change the switches for other rr_nodes that drives this rr_node*/
      for(inode = 0; inode < num_rr_nodes; inode++) {
        for (jedge = 0; jedge < rr_node[inode].num_edges; jedge++) {
          if (to_node == rr_node[inode].edges[jedge]) {
            rr_node[inode].switches[jedge] = rr_node[to_node].driver_switch;
          }
        }
      }
    }
  }
  
  return 1;
}
    
void update_rr_nodes_driver_switch(enum e_directionality directionality) {
  int inode, iedge;
  t_rr_type inode_rr_type;
  int to_node; 
 
  if (UNI_DIRECTIONAL != directionality) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,LINE[%d])Update_rr_nodes_driver_switch is only valid for uni-directional routing architecture.\n",
           __FILE__, __LINE__);
    exit(1);
  }

  /* Initial all the driver_switch to -1*/
  for (inode = 0; inode < num_rr_nodes; inode++) {
    rr_node[inode].driver_switch = -1;
  }

  for (inode = 0; inode < num_rr_nodes; inode++) {
    inode_rr_type = rr_node[inode].type;
    switch (inode_rr_type) {
    // We care only switch boxes
    case CHANX:
    case CHANY:
      for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
        to_node = rr_node[inode].edges[iedge];
        /* if to_node is a Channel, this is a switch box,
         * if to_node is a IPIN, this is a connection box
         */
        if ((CHANX == rr_node[to_node].type)
           ||(CHANY == rr_node[to_node].type)
           ||(IPIN == rr_node[to_node].type)) {
          /* We should consider if driver_switch is the same or not*/
          if (-1 == rr_node[to_node].driver_switch) {
            rr_node[to_node].driver_switch = rr_node[inode].switches[iedge]; 
          } else {
            assert(rr_node[to_node].driver_switch == rr_node[inode].switches[iedge]);
          }
        }
      }
      break;
    case OPIN:
      for (iedge = 0; iedge < rr_node[inode].num_edges; iedge++) {
        to_node = rr_node[inode].edges[iedge];
        /* We care only to_node is a Channel
         * Actually, in single driver routing, the switch is a delayless
         * However, we have to update all switches
         */
        if ((CHANX == rr_node[to_node].type)
           ||(CHANY == rr_node[to_node].type)) {
          /* We should consider if driver_switch is the same or not*/
          if (-1 == rr_node[to_node].driver_switch) {
            rr_node[to_node].driver_switch = rr_node[inode].switches[iedge]; 
          } else {
            assert(rr_node[to_node].driver_switch == rr_node[inode].switches[iedge]);
          }
        }
      }
      break;
    case IPIN:
      /* This is a sink... There is no to_node*/
    default:
      break;
    }
  }
}

/* Rotate the switch segmentpattern
 * Original: 1 0 0 , rotated_shift = 2, return: 0 0 1
 */
static 
boolean* rotate_shift_swseg_pattern(int pattern_length,
                                    boolean* pattern,
                                    int rotate_shift_length) {
  boolean* ret = NULL;
  int shift_length;  

  /* There is no need to do anything if input is NULL*/
  if ((pattern_length == 0)||(pattern_length < 0)) {
    return NULL;
  }
  /* if rotate_shift_length exceeds the pattern lenght, we use mod */
  shift_length = rotate_shift_length % pattern_length;
  /* Direct return the pattern */
  if (0 == shift_length) {
    return pattern;
  }
 
  /*Alloc*/
  ret = (boolean*)my_malloc(pattern_length*sizeof(boolean));
  /* Initialization*/
  memset(ret, 0, pattern_length*sizeof(boolean));

  /*Rotate, rotated pattern: shifted part, orignal part*/
  /* Step 1: fill the shifted part*/
  memcpy(ret, pattern+pattern_length-shift_length, sizeof(boolean)*shift_length);
  /* Step 2 : fill the rest*/
  memcpy(ret+shift_length, pattern, sizeof(boolean)*(pattern_length-shift_length));

  /* Return*/
  return ret; 
}
