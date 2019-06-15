/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    chan_node_details.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/11  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains a class to model the details of routing node
 *  in a channel:
 *  1. segment information: length, frequency etc.
 *  2. starting point of segment
 *  3. ending point of segment
 *  4. potentail track_id(ptc_num) of each segment
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CHAN_NODE_DETAILS_H
#define CHAN_NODE_DETAILS_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>
#include "vpr_types.h"

/************************************************************************
 *  ChanNodeDetails records segment length, directionality and starting of routing tracks
 *    +---------------------------------+
 *    | Index | Direction | Start Point |
 *    +---------------------------------+
 *    |   0   | --------> |   Yes       |
 *    +---------------------------------+
 ***********************************************************************/


class ChanNodeDetails {
  public : /* Constructor */
    ChanNodeDetails(const ChanNodeDetails&); /* Duplication */
    ChanNodeDetails(); /* Initilization */
  public: /* Accessors */
    size_t get_chan_width() const;
    size_t get_track_node_id(size_t track_id) const;
    e_direction get_track_direction(size_t track_id) const;
    size_t get_track_segment_length(size_t track_id) const;
    bool is_track_start(size_t track_id) const;
    bool is_track_end(size_t track_id) const;
    std::vector<size_t> get_seg_group(size_t track_id) const;
    std::vector<size_t> get_seg_group_node_id(std::vector<size_t> seg_group) const;
    size_t get_num_starting_tracks() const;
    size_t get_num_ending_tracks() const;
  public: /* Mutators */
    void reserve(size_t chan_width); /* Reserve the capacitcy of vectors */
    void add_track(size_t track_node_id, e_direction track_direction, size_t seg_length, size_t is_start, size_t is_end);
    void set_tracks_start(e_direction track_direction);
    void set_tracks_end(e_direction track_direction);
    void rotate_track_node_id(size_t offset, bool counter_rotate); /* rotate the track_node_id by an offset */
    void clear();
  private: /* validators */
    bool validate_chan_width() const;
    bool validate_track_id(size_t track_id) const;
  private: /* Internal data */ 
    std::vector<size_t> track_node_ids_; /* indices of each track */
    std::vector<e_direction> track_direction_; /* direction of each track */
    std::vector<size_t> seg_length_; /* Length of each segment */
    std::vector<bool>   track_start_; /* flag to identify if this is the starting point of the track */
    std::vector<bool>   track_end_; /* flag to identify if this is the ending point of the track */
};

#endif 

