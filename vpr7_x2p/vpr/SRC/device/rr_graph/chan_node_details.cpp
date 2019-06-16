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
 * Filename:    chan_node_details.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/14  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file contains member functions for class ChanNodeDetails
 ***********************************************************************/
#include <cassert>
#include <algorithm>
#include "chan_node_details.h"

/************************************************************************
 *  Constructors 
 ***********************************************************************/
ChanNodeDetails::ChanNodeDetails(const ChanNodeDetails& src) {
  /* duplicate */
  size_t chan_width = src.get_chan_width();
  this->reserve(chan_width); 
  for (size_t itrack = 0; itrack < chan_width; ++itrack) {
    track_node_ids_.push_back(src.get_track_node_id(itrack));
    track_direction_.push_back(src.get_track_direction(itrack));
    seg_length_.push_back(src.get_track_segment_length(itrack));
    track_start_.push_back(src.is_track_start(itrack));
    track_end_.push_back(src.is_track_end(itrack));
  }
}

ChanNodeDetails::ChanNodeDetails() {
  this->clear();
}

/************************************************************************
 *  Accessors
 ***********************************************************************/
size_t ChanNodeDetails::get_chan_width() const {
  assert(validate_chan_width());
  return track_node_ids_.size();
}

size_t ChanNodeDetails::get_track_node_id(size_t track_id) const {
  assert(validate_track_id(track_id));
  return track_node_ids_[track_id];
}

e_direction ChanNodeDetails::get_track_direction(size_t track_id) const {
  assert(validate_track_id(track_id));
  return track_direction_[track_id];
}

size_t ChanNodeDetails::get_track_segment_length(size_t track_id) const {
  assert(validate_track_id(track_id));
  return seg_length_[track_id];
}

bool   ChanNodeDetails::is_track_start(size_t track_id) const {
  assert(validate_track_id(track_id));
  return track_start_[track_id];
}

bool   ChanNodeDetails::is_track_end(size_t track_id) const {
  assert(validate_track_id(track_id));
  return track_end_[track_id];
}

/* Track_id is the starting point of group (whose is_start should be true) 
 * This function will try to find the track_ids with the same directionality as track_id and seg_length
 * A group size is the number of such nodes between the starting points (include the 1st starting point) 
 */
std::vector<size_t> ChanNodeDetails::get_seg_group(size_t track_id) const {
  assert(validate_chan_width());
  assert(validate_track_id(track_id));
  assert(is_track_start(track_id));
  
  std::vector<size_t> group;
  /* Make sure a clean start */
  group.clear();

  /* track_id is the first element */
  group.push_back(track_id);

  for (size_t itrack = track_id; itrack < get_chan_width(); ++itrack) {
    if ( (get_track_direction(itrack) == get_track_direction(track_id) )
      && (get_track_segment_length(itrack) == get_track_segment_length(track_id)) ) {
      if ( (false == is_track_start(itrack)) 
        || ( (true == is_track_start(itrack)) && (itrack == track_id)) ) {
        group.push_back(itrack);
        continue;
      }
      /* Stop if this another starting point */
      if (true == is_track_start(itrack)) {
        break;
      }
    }
  }  
  return group;
}

/* Get a list of track_ids with the given list of track indices */
std::vector<size_t> ChanNodeDetails::get_seg_group_node_id(std::vector<size_t> seg_group) const {
  std::vector<size_t> group;
  /* Make sure a clean start */
  group.clear();

  for (size_t id = 0; id < seg_group.size(); ++id) {
    assert(validate_track_id(seg_group[id]));
    group.push_back(get_track_node_id(seg_group[id]));
  }

  return group;
}

/* Get the number of tracks that starts in this routing channel */
size_t ChanNodeDetails::get_num_starting_tracks() const {
  size_t counter = 0;
  for (size_t itrack = 0; itrack < get_chan_width(); ++itrack) {
    if (false == is_track_start(itrack)) {
      continue;
    } 
    counter++;
  }  
  return counter;
}

/* Get the number of tracks that ends in this routing channel */
size_t ChanNodeDetails::get_num_ending_tracks() const {
  size_t counter = 0;
  for (size_t itrack = 0; itrack < get_chan_width(); ++itrack) {
    if (false == is_track_end(itrack)) {
      continue;
    } 
    counter++;
  }  
  return counter;
}

/************************************************************************
 *  Mutators
 ***********************************************************************/
/* Reserve the capacitcy of vectors */
void ChanNodeDetails::reserve(size_t chan_width) {
  track_node_ids_.reserve(chan_width);
  track_direction_.reserve(chan_width);
  seg_length_.reserve(chan_width);
  track_start_.reserve(chan_width);
  track_end_.reserve(chan_width);
}

/* Add a track to the channel */
void ChanNodeDetails::add_track(size_t track_node_id, e_direction track_direction, size_t seg_length, size_t is_start, size_t is_end) {
  track_node_ids_.push_back(track_node_id);
  track_direction_.push_back(track_direction);
  seg_length_.push_back(seg_length);
  track_start_.push_back(is_start);
  track_end_.push_back(is_end);
}

/* Update the node_id of a given track */
void ChanNodeDetails::set_track_node_id(size_t track_index, size_t track_node_id) {
  assert(validate_track_id(track_index));
  track_node_ids_[track_index] = track_node_id; 
}

/* Set tracks with a given direction to start */
void ChanNodeDetails::set_tracks_start(e_direction track_direction) {
  for (size_t inode = 0; inode < get_chan_width(); ++inode) {
    /* Bypass non-match tracks */
    if (track_direction != get_track_direction(inode)) {
    }
    track_start_[inode] = true;
  }
}

/* Set tracks with a given direction to end */
void ChanNodeDetails::set_tracks_end(e_direction track_direction) {
  for (size_t inode = 0; inode < get_chan_width(); ++inode) {
    /* Bypass non-match tracks */
    if (track_direction != get_track_direction(inode)) {
    }
    track_end_[inode] = true;
  }
}

/* rotate the track_node_id by an offset */
void ChanNodeDetails::rotate_track_node_id(size_t offset, e_direction track_direction, bool counter_rotate) {
  /* Rotate the node_ids by groups
   * A group begins from a track_start and ends before another track_start  
   */
  assert(validate_chan_width());
  for (size_t itrack = 0; itrack < get_chan_width(); ++itrack) { 
    /* Bypass non-start segment */
    if (false == is_track_start(itrack) ) {
      continue;
    }
    /* Bypass segments do not match track_direction */
    if (track_direction != get_track_direction(itrack) ) {
      continue;
    }
    /* Find the group nodes */
    std::vector<size_t> track_group = get_seg_group(itrack);
    /* Build a vector of the node ids of the tracks */
    std::vector<size_t> track_group_node_id = get_seg_group_node_id(track_group);
    /* Rotate or Counter rotate */
    if (true == counter_rotate) {
      std::rotate(track_group_node_id.begin(), track_group_node_id.end() - offset, track_group_node_id.end());
    } else {
      std::rotate(track_group_node_id.begin(), track_group_node_id.begin() + offset, track_group_node_id.end());
    }
    /* Update the node_ids */
    for (size_t inode = 0; inode < track_group.size(); ++inode) {
      track_node_ids_[track_group[inode]] = track_group_node_id[inode];
    }
  }
  return;
}

void ChanNodeDetails::clear() {
  track_node_ids_.clear();
  track_direction_.clear();
  seg_length_.clear();
  track_start_.clear();
  track_end_.clear();
}

/************************************************************************
 *  Validators 
 ***********************************************************************/
bool ChanNodeDetails::validate_chan_width() const {
  size_t chan_width = track_node_ids_.size();
  if ( (chan_width == track_direction_.size())
     &&(chan_width == seg_length_.size())
     &&(chan_width == track_start_.size())
     &&(chan_width == track_end_.size()) ) {
    return true;
  }
  return false;
}

bool ChanNodeDetails::validate_track_id(size_t track_id) const {
  if ( (track_id < track_node_ids_.size()) 
    && (track_id < track_direction_.size()) 
    && (track_id < seg_length_.size()) 
    && (track_id < track_start_.size()) 
    && (track_id < track_end_.size()) ) {
    return true;
  } 
  return false;
}

