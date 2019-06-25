#include <cassert>
#include <string.h>
#include <algorithm>
#include <sstream>

#include "rr_blocks_naming.h"

#include "rr_blocks.h"


/* Member Functions of Class RRChan */
/* Constructors */

/* Copy Constructor */
RRChan::RRChan(const RRChan& rr_chan) {
  this->set(rr_chan);
  return;
}

/* default constructor */
RRChan::RRChan() {
  type_ = NUM_RR_TYPES;
  nodes_.resize(0);
  node_segments_.resize(0);
}

/* Accessors */
t_rr_type RRChan::get_type() const {
  return type_;
}

/* get the number of tracks in this channel */
size_t RRChan::get_chan_width() const { 
  return nodes_.size();
}

/* get the track_id of a node */
int RRChan::get_node_track_id(t_rr_node* node) const {
  /* if the given node is NULL, we return an invalid id */
  if (NULL == node) {
    return -1; /* FIXME: use a strong id!!! */
  }
  /* check each member and return if we find a match in content */
  for (size_t inode = 0; inode < nodes_.size(); ++inode) {
    if (node == nodes_[inode]) {
      return inode; 
    } 
  }
  return -1;
}

/* get the rr_node with the track_id */
t_rr_node* RRChan::get_node(size_t track_num) const {
  if ( false == valid_node_id(track_num) ) {
    return NULL;
  }
  return nodes_[track_num];
} 

/* get the segment id of a node */
int RRChan::get_node_segment(t_rr_node* node) const {
  int node_id = get_node_track_id(node);
  if ( false == valid_node_id(node_id)) {
    return -1;
  }
  return get_node_segment(node_id); 
}

/* get the segment id of a node */
int RRChan::get_node_segment(size_t track_num) const {
  if ( false == valid_node_id(track_num)) {
    return -1;
  }
  return node_segments_[track_num]; 
}

/* evaluate if two RRChan is mirror to each other */
bool RRChan::is_mirror(const RRChan& cand) const {
  /* If any following element does not match, it is not mirror */
  /* 1. type  */
  if (this->get_type() != cand.get_type()) {
    return false;
  }
  /* 2. track_width  */
  if (this->get_chan_width() != cand.get_chan_width()) {
    return false;
  }
  /* 3. for each node */
  for (size_t inode = 0; inode < this->get_chan_width(); ++inode) {
    /* 3.1 check node type */
    if (this->get_node(inode)->type != cand.get_node(inode)->type) {
      return false;
    }
    /* 3.2 check node directionality */
    if (this->get_node(inode)->direction != cand.get_node(inode)->direction) {
      return false;
    }
    /* 3.3 check node segment */
    if (this->get_node_segment(inode) != cand.get_node_segment(inode)) {
      return false;
    }
  }
 
  return true;
}

/* Get a list of segments used in this routing channel */
std::vector<size_t> RRChan::get_segment_ids() const { 
  std::vector<size_t> seg_list;

  /* make sure a clean start */
  seg_list.clear();

  /* Traverse node_segments */
  for (size_t inode = 0; inode < get_chan_width(); ++inode) {
    std::vector<size_t>::iterator it;
    /* Try to find the node_segment id in the list */
    it = find(seg_list.begin(), seg_list.end(), node_segments_[inode]);
    if ( it == seg_list.end() ) {
      /* Not found, add it to the list */
      seg_list.push_back(node_segments_[inode]);
    }
  }

  return seg_list;
}

/* Get a list of nodes whose segment_id is specified  */
std::vector<size_t> RRChan::get_node_ids_by_segment_ids(size_t seg_id) const {
  std::vector<size_t> node_list;

  /* make sure a clean start */
  node_list.clear();

  /* Traverse node_segments */
  for (size_t inode = 0; inode < get_chan_width(); ++inode) {
    /* Try to find the node_segment id in the list */
    if ( seg_id == node_segments_[inode] ) {
      node_list.push_back(inode);
    }
  }

  return node_list;
} 

/* Mutators */
void RRChan::set(const RRChan& rr_chan) {
  /* Ensure a clean start */
  this->clear();
  /* Assign type of this routing channel */
  this->type_ = rr_chan.get_type();
  /* Copy node and node_segments */
  this->nodes_.resize(rr_chan.get_chan_width());
  this->node_segments_.resize(rr_chan.get_chan_width());
  for (size_t inode = 0; inode < rr_chan.get_chan_width(); ++inode) { 
    this->nodes_[inode] = rr_chan.get_node(inode);
    this->node_segments_[inode] = rr_chan.get_node_segment(inode);
  }
  return;
}

/* modify type */
void RRChan::set_type(t_rr_type type) {
  assert(valid_type(type));
  type_ = type;
  return;
} 

/* Reserve node list */
void RRChan::reserve_node(size_t node_size) {
  nodes_.reserve(node_size); /* reserve to the maximum */
  node_segments_.reserve(node_size); /* reserve to the maximum */
}

/* add a node to the array */
void RRChan::add_node(t_rr_node* node, size_t node_segment) {
  /* fill the dedicated element in the vector */
  nodes_.push_back(node);
  node_segments_.push_back(node_segment);

  assert(valid_node_type(node));

  return;
}

/* rotate the nodes and node_segments with a given offset */
void RRChan::rotate(size_t offset) {
  std::rotate(nodes_.begin(), nodes_.begin() + offset, nodes_.end());
  std::rotate(node_segments_.begin(), node_segments_.begin() + offset, node_segments_.end());
  return;
} 

/* rotate all the channel nodes by a given offset:
 * Routing Channel nodes are divided into different groups using segment ids
 * each group is rotated separatedly
 */
void RRChan::rotate(size_t rotate_begin, size_t rotate_end, size_t offset) {
  std::rotate(nodes_.begin() + rotate_begin, nodes_.begin() + rotate_begin + offset, nodes_.begin() + rotate_end);
  std::rotate(node_segments_.begin() + rotate_begin, node_segments_.begin() + rotate_begin + offset, node_segments_.begin() + rotate_end);
  return;
} 

/* rotate all the channel nodes by a given offset:
 * Routing Channel nodes are divided into different groups using segment ids
 * each group should be rotated separatedly
 */
void RRChan::rotate_by_node_direction(enum e_direction node_direction, size_t offset) {
  /* skip if there are no nodes */
  if (0 == get_chan_width()) {
    return;
  }

  /* get a list of segment_ids existing in the routing channel */
  std::vector<size_t> seg_ids = get_segment_ids();

  for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) {
    /* Get the channel nodes of a given direction */
    std::vector<t_rr_node*> nodes;
    std::vector<size_t> node_segments;
    for (size_t inode = 0; inode < get_chan_width(); ++inode) {
      if ( (node_direction == get_node(inode)->direction) 
        && (seg_ids[iseg] == (size_t)get_node_segment(inode)) ) {
        nodes.push_back(get_node(inode));
        node_segments.push_back(get_node_segment(inode));
      }
    }
  
    size_t adapt_offset = offset % nodes.size();
    assert(adapt_offset < nodes.size());
  
    /* Rotate the chan_nodes */
    std::rotate(nodes.begin(), nodes.begin() + adapt_offset, nodes.end());
    std::rotate(node_segments.begin(), node_segments.begin() + adapt_offset, node_segments.end());
  
    /* back-annotate to to the original chan nodes*/
    for (size_t inode = 0; inode < get_chan_width(); ++inode) {
      if ( (node_direction == get_node(inode)->direction) 
        && (seg_ids[iseg] == (size_t)get_node_segment(inode)) ) {
        nodes_[inode] = nodes.front();
        node_segments_[inode] = node_segments.front();
        /* pop up temp vectors */
        nodes.erase(nodes.begin()); 
        node_segments.erase(node_segments.begin()); 
      }
    }
  
    /* Make sure temp vectors are all poped out */
    assert ( 0 == nodes.size());
    assert ( 0 == node_segments.size());
  }

  return;
} 

/* rotate all the channel nodes by a given offset:
 * Routing Channel nodes are divided into different groups using segment ids
 * each group is rotated separatedly
 */
void RRChan::counter_rotate_by_node_direction(enum e_direction node_direction, size_t offset) {
  /* skip if there are no nodes */
  if (0 == get_chan_width()) {
    return;
  }

  /* get a list of segment_ids existing in the routing channel */
  std::vector<size_t> seg_ids = get_segment_ids();

  for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) {
    /* Get the channel nodes of a given direction */
    std::vector<t_rr_node*> nodes;
    std::vector<size_t> node_segments;
    for (size_t inode = 0; inode < get_chan_width(); ++inode) {
      if ( (node_direction == get_node(inode)->direction) 
        && (seg_ids[iseg] == (size_t)get_node_segment(inode)) ) {
        nodes.push_back(get_node(inode));
        node_segments.push_back(get_node_segment(inode));
      }
    }

    size_t adapt_offset = offset % nodes.size();
    assert(adapt_offset < nodes.size());

    /* Rotate the chan_nodes */
    std::rotate(nodes.begin(), nodes.begin() + nodes.size() - adapt_offset, nodes.end());
    std::rotate(node_segments.begin(), node_segments.begin() + node_segments.size() - adapt_offset, node_segments.end());

    /* back-annotate to to the original chan nodes*/
    for (size_t inode = 0; inode < get_chan_width(); ++inode) {
      if ( (node_direction == get_node(inode)->direction) 
        && (seg_ids[iseg] == (size_t)get_node_segment(inode)) ) {
        nodes_[inode] = nodes.front();
        node_segments_[inode] = node_segments.front();
        /* pop up temp vectors */
        nodes.erase(nodes.begin()); 
        node_segments.erase(node_segments.begin()); 
      }
    }

    /* Make sure temp vectors are all poped out */
    assert ( 0 == nodes.size());
    assert ( 0 == node_segments.size());
  }

  return;
} 


/* Mirror the node direction of routing track nodes on a side */
void RRChan::mirror_node_direction() {
  for (size_t inode = 0; inode < get_chan_width(); ++inode) {
    if (INC_DIRECTION == get_node(inode)->direction) {
      nodes_[inode]->direction = DEC_DIRECTION;
    } else {
      assert (DEC_DIRECTION == get_node(inode)->direction);
      nodes_[inode]->direction = INC_DIRECTION;
    }
  }
  return;
} 


/* Clear content */
void RRChan::clear() {
  nodes_.clear();
  node_segments_.clear();

  return;
}

/* Internal functions */

/* for type, only valid type is CHANX and CHANY */
bool RRChan::valid_type(t_rr_type type) const {
  if ((CHANX == type) || (CHANY == type)) {
    return true;
  }
  return false;
}  

/* Check each node, see if the node type is consistent with the type */
bool RRChan::valid_node_type(t_rr_node* node) const {
  valid_type(node->type);
  if (NUM_RR_TYPES == type_) {
    return true;
  }
  valid_type(type_);
  if (type_ != node->type) {
    return false;
  }
  return true;
}

/* check if the node id is valid */
bool RRChan::valid_node_id(size_t node_id) const {
  if (node_id < nodes_.size()) {
    return true;
  }

  return false;
}

/* Member Functions of Class DeviceRRChan */
/* accessors */
RRChan DeviceRRChan::get_module(t_rr_type chan_type, size_t module_id) const {
  assert(valid_module_id(chan_type, module_id));

  if (CHANX == chan_type) {
    return chanx_modules_[module_id]; 
  }
  assert (CHANY == chan_type);
  return chany_modules_[module_id]; 
}

RRChan DeviceRRChan::get_module_with_coordinator(t_rr_type chan_type, size_t x, size_t y) const {
  assert(valid_coordinator(chan_type, x, y));
  assert(valid_module_id(chan_type, get_module_id(chan_type, x, y)));
  return get_module(chan_type, get_module_id(chan_type, x, y));
}

/* Get the number of RRChan modules in either X-channel or Y-channel */
size_t DeviceRRChan::get_num_modules(t_rr_type chan_type) const {
  assert(valid_chan_type(chan_type));

  if (CHANX == chan_type) {
    return chanx_modules_.size(); 
  }
  assert (CHANY == chan_type);
  return chany_modules_.size(); 
}

size_t DeviceRRChan::get_module_id(t_rr_type chan_type, size_t x, size_t y) const {
  assert(valid_coordinator(chan_type, x, y));

  if (CHANX == chan_type) {
    return chanx_module_ids_[x][y]; 
  }
  assert (CHANY == chan_type);
  return chany_module_ids_[x][y]; 
}

void DeviceRRChan::init_module_ids(size_t device_width, size_t device_height) {
  init_chan_module_ids(CHANX, device_width, device_height);
  init_chan_module_ids(CHANY, device_width, device_height);

  return;
}

void DeviceRRChan::init_chan_module_ids(t_rr_type chan_type, size_t device_width, size_t device_height) {
  assert(valid_chan_type(chan_type));

  if (CHANX == chan_type) {
    chanx_module_ids_.resize(device_width);
    for (size_t x = 0; x < chanx_module_ids_.size(); ++x) {
      chanx_module_ids_[x].resize(device_height);
    }
  } else if (CHANY == chan_type) {
    chany_module_ids_.resize(device_width);
    for (size_t x = 0; x < chany_module_ids_.size(); ++x) {
      chany_module_ids_[x].resize(device_height);
    }
  }
  return;
}

void DeviceRRChan::add_one_chan_module(t_rr_type chan_type, size_t x, size_t y, RRChan& rr_chan) {
  assert(valid_coordinator(chan_type, x, y));

  if (CHANX == chan_type) {
    /* Find if the module is unique */
    for (size_t i = 0; i < chanx_modules_.size(); ++i) {
      if ( true == chanx_modules_[i].is_mirror(rr_chan)) {
        /* Find a mirror in the list, assign ids and return */ 
        chanx_module_ids_[x][y] = i;
        return;
      }
    }
    /* Reach here, it means this is a unique module */
    /* add to the module list */
    chanx_modules_.push_back(rr_chan);
    chanx_module_ids_[x][y] = chanx_modules_.size() - 1;
  } else if (CHANY == chan_type) {
    /* Find if the module is unique */
    for (size_t i = 0; i < chany_modules_.size(); ++i) {
      if ( true == chany_modules_[i].is_mirror(rr_chan)) {
        /* Find a mirror in the list, assign ids and return */ 
        chany_module_ids_[x][y] = i;
        return;
      }
    }
    /* Reach here, it means this is a unique module */
    /* add to the module list */
    chany_modules_.push_back(rr_chan);
    chany_module_ids_[x][y] = chany_modules_.size() - 1;
  }

  return;
}

void DeviceRRChan::clear() {
  clear_chan(CHANX);
  clear_chan(CHANY);
}

void DeviceRRChan::clear_chan(t_rr_type chan_type) {
  assert(valid_chan_type(chan_type));

  if (CHANX == chan_type) {
    chanx_modules_.clear();
  } else if (CHANY == chan_type) {
    chany_modules_.clear();
  }

  return;
}

/* for type, only valid type is CHANX and CHANY */
bool DeviceRRChan::valid_chan_type(t_rr_type chan_type) const {
  if ((CHANX == chan_type) || (CHANY == chan_type)) {
    return true;
  }
  return false;
}  

/* check if the coordinator is in range */
bool DeviceRRChan::valid_coordinator(t_rr_type chan_type, size_t x, size_t y) const {
  assert(valid_chan_type(chan_type));

  if (CHANX == chan_type) {
    if (x > chanx_module_ids_.size() - 1 ) {
      return false;
    }
    if (y > chanx_module_ids_[x].size() - 1) {
      return false;
    }
  } else if (CHANY == chan_type) {
    if (x > chany_module_ids_.size() - 1) {
      return false;
    }
    if (y > chany_module_ids_[x].size() - 1) {
      return false;
    }
  }

  return true;
}

/* check if the node id is valid */
bool DeviceRRChan::valid_module_id(t_rr_type chan_type, size_t module_id) const {
  assert(valid_chan_type(chan_type));

  if (CHANX == chan_type) {
    if (module_id < chanx_modules_.size()) {
      return true;
    }
  } else if (CHANY == chan_type) {
    if (module_id < chany_modules_.size()) {
      return true;
    }
  }

  return false;
}

/* Member Functions of Class RRGSB*/
/* Constructor for an empty object */
RRGSB::RRGSB() {
  /* Set a clean start! */
  coordinator_.set(0, 0);
  chan_node_direction_.clear();
  ipin_node_.clear();
  ipin_node_grid_side_.clear();
  opin_node_.clear();
  opin_node_grid_side_.clear();

  sb_conf_port_.reset();
  cbx_conf_port_.reset();
  cby_conf_port_.reset();
  
  return;
}

/* Copy constructor */
RRGSB::RRGSB(const RRGSB& src) {
  /* Copy coordinator */
  this->set(src);
  return;
}

/* Accessors */

/* Get the number of sides of this SB */
size_t RRGSB::get_num_sides() const {
  assert (validate_num_sides());
  return chan_node_direction_.size();
}

/* Get the number of routing tracks on a side */
size_t RRGSB::get_chan_width(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return chan_node_[side_manager.to_size_t()].get_chan_width(); 
}

/* Get the maximum number of routing tracks on all sides */
size_t RRGSB::get_max_chan_width() const {
  size_t max_chan_width = 0;
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);
    max_chan_width = std::max(max_chan_width, get_chan_width(side_manager.get_side()));
  }
  return max_chan_width;
} 

/* Get the number of routing tracks of a X/Y-direction CB */
size_t RRGSB::get_cb_chan_width(t_rr_type cb_type) const {
  return get_chan_width(get_cb_chan_side(cb_type)); 
}

/* Get the sides of ipin_nodes belong to the cb */
std::vector<enum e_side> RRGSB::get_cb_ipin_sides(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  
  std::vector<enum e_side> ipin_sides;

  /* Make sure a clean start */
  ipin_sides.clear();

  switch(cb_type) {
  case CHANX:
    ipin_sides.push_back(TOP);
    ipin_sides.push_back(BOTTOM);
    break;
  case CHANY:
    ipin_sides.push_back(RIGHT);
    ipin_sides.push_back(LEFT);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }

  return ipin_sides;
}

/* Get the direction of a rr_node at a given side and track_id */
enum PORTS RRGSB::get_chan_node_direction(enum e_side side, size_t track_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_track_id(side, track_id) );
  
  return chan_node_direction_[side_manager.to_size_t()][track_id]; 
}

/* get a RRChan at a given side and track_id */
RRChan RRGSB::get_chan(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  return chan_node_[side_manager.to_size_t()]; 
} 

/* Get a list of segments used in this routing channel */
std::vector<size_t> RRGSB::get_chan_segment_ids(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  return get_chan(side).get_segment_ids(); 
}

/* Get a list of rr_nodes whose sed_id is specified */
std::vector<size_t> RRGSB::get_chan_node_ids_by_segment_ids(enum e_side side, size_t seg_id) const {
  return get_chan(side).get_node_ids_by_segment_ids(seg_id);
} 

/* get a rr_node at a given side and track_id */
t_rr_node* RRGSB::get_chan_node(enum e_side side, size_t track_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_track_id(side, track_id) );
  
  return chan_node_[side_manager.to_size_t()].get_node(track_id); 
} 

/* get the segment id of a channel rr_node */
size_t RRGSB::get_chan_node_segment(enum e_side side, size_t track_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_track_id(side, track_id) );
  
  return chan_node_[side_manager.to_size_t()].get_node_segment(track_id); 
} 


/* Get the number of IPIN rr_nodes on a side */
size_t RRGSB::get_num_ipin_nodes(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return ipin_node_[side_manager.to_size_t()].size(); 
} 

/* get a opin_node at a given side and track_id */
t_rr_node* RRGSB::get_ipin_node(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_ipin_node_id(side, node_id) );
  
  return ipin_node_[side_manager.to_size_t()][node_id]; 
} 

/* get the grid_side of a opin_node at a given side and track_id */
enum e_side RRGSB::get_ipin_node_grid_side(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_ipin_node_id(side, node_id) );
  
  return ipin_node_grid_side_[side_manager.to_size_t()][node_id]; 
}

/* get the grid side of a opin_rr_node */
enum e_side RRGSB::get_ipin_node_grid_side(t_rr_node* ipin_node) const {
  enum e_side side;
  int index;

  /* Find the side and index */
  get_node_side_and_index(ipin_node, OUT_PORT, &side, &index);
  assert(-1 != index);
  assert(validate_side(side));
  return get_ipin_node_grid_side(side, index);
} 


/* Get the number of OPIN rr_nodes on a side */
size_t RRGSB::get_num_opin_nodes(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return opin_node_[side_manager.to_size_t()].size(); 
} 

/* get a opin_node at a given side and track_id */
t_rr_node* RRGSB::get_opin_node(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_opin_node_id(side, node_id) );
  
  return opin_node_[side_manager.to_size_t()][node_id]; 
} 

/* get the grid_side of a opin_node at a given side and track_id */
enum e_side RRGSB::get_opin_node_grid_side(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_opin_node_id(side, node_id) );
  
  return opin_node_grid_side_[side_manager.to_size_t()][node_id]; 
}

/* get the grid side of a opin_rr_node */
enum e_side RRGSB::get_opin_node_grid_side(t_rr_node* opin_node) const {
  enum e_side side;
  int index;

  /* Find the side and index */
  get_node_side_and_index(opin_node, IN_PORT, &side, &index);
  assert(-1 != index);
  assert(validate_side(side));
  return get_opin_node_grid_side(side, index);
} 

/* Get the node index of a routing track of a connection block, return -1 if not found */
int RRGSB::get_cb_chan_node_index(t_rr_type cb_type, t_rr_node* node) const {
  enum e_side chan_side = get_cb_chan_side(cb_type);
  return get_chan_node_index(chan_side, node);
}

/* Get the node index in the array, return -1 if not found */
int RRGSB::get_chan_node_index(enum e_side node_side, t_rr_node* node) const {
  assert (validate_side(node_side));
  return get_chan(node_side).get_node_track_id(node); 
}

/* Get the node index in the array, return -1 if not found */
int RRGSB::get_node_index(t_rr_node* node, 
                          enum e_side node_side, 
                          enum PORTS node_direction) const {
  size_t cnt;
  int ret; 
  Side side_manager(node_side);

  cnt = 0;
  ret = -1;

  /* Depending on the type of rr_node, we search different arrays */
  switch (node->type) {
  case CHANX:
  case CHANY:
    for (size_t inode = 0; inode < get_chan_width(node_side); ++inode){
      if ((node == chan_node_[side_manager.to_size_t()].get_node(inode))
        /* Check if direction meets specification */
        &&(node_direction == chan_node_direction_[side_manager.to_size_t()][inode])) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case IPIN:
    for (size_t inode = 0; inode < get_num_ipin_nodes(node_side); ++inode) {
      if (node == ipin_node_[side_manager.to_size_t()][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  case OPIN:
    for (size_t inode = 0; inode < get_num_opin_nodes(node_side); ++inode) {
      if (node == opin_node_[side_manager.to_size_t()][inode]) {
        cnt++;
        ret = inode;
      }
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid cur_rr_node type! Should be [CHANX|CHANY|IPIN|OPIN]\n", __FILE__, __LINE__);
    exit(1);
  }

  assert((0 == cnt)||(1 == cnt));

  return ret; /* Return an invalid value: nonthing is found*/
}

/* Check if the node exist in the opposite side of this Switch Block */
bool RRGSB::is_sb_node_exist_opposite_side(t_rr_node* node, 
                                           enum e_side node_side) const {
  Side side_manager(node_side);
  int index;

  assert((CHANX == node->type) || (CHANY == node->type));

  /* See if we can find the same src_rr_node in the opposite chan_side 
   * if there is one, it means a shorted wire across the SB 
   */
  index = get_node_index(node, side_manager.get_opposite(), IN_PORT);

  if (-1 != index) {
    return true;
  }

  return false;
}

/* Get the side of a node in this SB */
void RRGSB::get_node_side_and_index(t_rr_node* node, 
                                    enum PORTS node_direction,
                                    enum e_side* node_side, 
                                    int* node_index) const {
  size_t side;
  Side side_manager;
  
  /* Count the number of existence of cur_rr_node in cur_sb_info
   * It could happen that same cur_rr_node appears on different sides of a SB
   * For example, a routing track go vertically across the SB.
   * Then its corresponding rr_node appears on both TOP and BOTTOM sides of this SB. 
   * We need to ensure that the found rr_node has the same direction as user want.
   * By specifying the direction of rr_node, There should be only one rr_node can satisfy!
   */
  for (side = 0; side < get_num_sides(); ++side) {
    side_manager.set_side(side);
    (*node_index) = get_node_index(node, side_manager.get_side(), node_direction);
    if (-1 != (*node_index)) {
      break;
    }
  }

  if (side == get_num_sides()) {
    /* we find nothing, return NUM_SIDES, and a OPEN node (-1) */
    (*node_side) = NUM_SIDES;
    assert(-1 == (*node_index));
    return;
  }

  (*node_side) = side_manager.get_side();
  assert(-1 != (*node_index));

  return;
} 

/* Get Switch Block configuration port information */
size_t RRGSB::get_sb_num_reserved_conf_bits() const {
  return sb_conf_port_.get_reserved_port_width();
}

size_t RRGSB::get_sb_reserved_conf_bits_lsb() const {
  return sb_conf_port_.get_reserved_port_lsb();
}

size_t RRGSB::get_sb_reserved_conf_bits_msb() const {
  return sb_conf_port_.get_reserved_port_msb();
}
    
size_t RRGSB::get_sb_num_conf_bits() const {
  return sb_conf_port_.get_regular_port_width();
}

size_t RRGSB::get_sb_conf_bits_lsb() const {
  return sb_conf_port_.get_regular_port_lsb();
}

size_t RRGSB::get_sb_conf_bits_msb() const {
  return sb_conf_port_.get_regular_port_msb();
}

/* Get X-direction Connection Block configuration port information */
size_t RRGSB::get_cb_num_reserved_conf_bits(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_reserved_port_width();
  case CHANY:
    return cby_conf_port_.get_reserved_port_width();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

size_t RRGSB::get_cb_reserved_conf_bits_lsb(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_reserved_port_lsb();
  case CHANY:
    return cby_conf_port_.get_reserved_port_lsb();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

size_t RRGSB::get_cb_reserved_conf_bits_msb(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_reserved_port_msb();
  case CHANY:
    return cby_conf_port_.get_reserved_port_msb();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}
    
size_t RRGSB::get_cb_num_conf_bits(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_regular_port_width();
  case CHANY:
    return cby_conf_port_.get_regular_port_width();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

size_t RRGSB::get_cb_conf_bits_lsb(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_regular_port_lsb();
  case CHANY:
    return cby_conf_port_.get_regular_port_lsb();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

size_t RRGSB::get_cb_conf_bits_msb(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.get_regular_port_msb();
  case CHANY:
    return cby_conf_port_.get_regular_port_msb();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

/* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
bool RRGSB::is_sb_node_imply_short_connection(t_rr_node* src_node) const {

  assert((CHANX == src_node->type) || (CHANY == src_node->type));
  
  for (size_t inode = 0; inode < size_t(src_node->num_drive_rr_nodes); ++inode) {
    enum e_side side;
    int index; 
    get_node_side_and_index(src_node->drive_rr_nodes[inode], IN_PORT, &side, &index);
    /* We need to be sure that drive_rr_node is part of the SB */
    if (((-1 == index) || (NUM_SIDES == side)) 
       && ((CHANX == src_node->drive_rr_nodes[inode]->type) || (CHANY == src_node->drive_rr_nodes[inode]->type))) {
      return true;
    }
  }

  return false;
}

/* check if the candidate SB satisfy the basic requirements on being a mirror of the current one */
/* Idenify mirror Switch blocks 
 * Check each two switch blocks: 
 * Number of channel/opin/ipin rr_nodes are same 
 * If all above are satisfied, the two switch blocks may be mirrors !
 */
bool RRGSB::is_sb_mirrorable(const RRGSB& cand) const {
  /* check the numbers of sides */
  if (get_num_sides() != cand.get_num_sides()) {
    return false;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);

    /* Ensure we have the same channel width on this side */
    if (get_chan_width(side_manager.get_side()) != cand.get_chan_width(side_manager.get_side())) {
      return false;
    }

    if ( ((size_t(-1) == get_track_id_first_short_connection(side_manager.get_side())) 
       && (size_t(-1) != cand.get_track_id_first_short_connection(side_manager.get_side())))
    || ((size_t(-1) != get_track_id_first_short_connection(side_manager.get_side()) )
       && ( size_t(-1) == cand.get_track_id_first_short_connection(side_manager.get_side()))) ) {
      return false;
    }
  } 

  /* check the numbers of opin_rr_nodes */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);

    if (get_num_opin_nodes(side_manager.get_side()) != cand.get_num_opin_nodes(side_manager.get_side())) {
      return false;
    }
  }

  /* Make sure the number of conf bits are the same */
  if ( ( get_sb_num_conf_bits() != cand.get_sb_num_conf_bits() ) 
    || ( get_sb_num_reserved_conf_bits() != cand.get_sb_num_reserved_conf_bits() ) ) { 
    return false;
  }

  return true;
}

/* check if the candidate CB is a mirror of the current one */
bool RRGSB::is_cb_mirror(const RRGSB& cand, t_rr_type cb_type) const { 
  /* Check if channel width is the same */
  if ( get_cb_chan_width(cb_type) != cand.get_cb_chan_width(cb_type) ) {
    return false;
  }

  enum e_side chan_side = get_cb_chan_side(cb_type);

  /* check the numbers/directionality of channel rr_nodes */
  if ( false == get_chan(chan_side).is_mirror(cand.get_chan(chan_side)) ) {
     return false;
  }

  /* check the equivalence of ipins */
  std::vector<enum e_side> ipin_side = get_cb_ipin_sides(cb_type);
  for (size_t side = 0; side < ipin_side.size(); ++side) {
    /* Ensure we have the same number of IPINs on this side */
    if ( get_num_ipin_nodes(ipin_side[side]) != cand.get_num_ipin_nodes(ipin_side[side]) ) {
      return false;
    }
    for (size_t inode = 0; inode < get_num_ipin_nodes(ipin_side[side]); ++inode) {
      if (false == is_cb_node_mirror(cand, cb_type, ipin_side[side], inode)) {
        return false;
      }
    }
  }

  /* Make sure the number of conf bits are the same */
  if ( ( get_cb_num_conf_bits(cb_type) != cand.get_cb_num_conf_bits(cb_type) ) 
    || ( get_cb_num_reserved_conf_bits(cb_type) != cand.get_cb_num_reserved_conf_bits(cb_type) ) ) { 
    return false;
  }

  return true;
}

/* check if the CB exist in this GSB */
bool RRGSB::is_cb_exist(t_rr_type cb_type) const {
  /* if channel width is zero, there is no CB */
  if ( 0 == get_cb_chan_width(cb_type)) {
    return false;
  }
  return true;
} 

/* Determine an initial offset in rotating the candidate Switch Block to find a mirror matching 
 * We try to find the offset in track_id where the two Switch Blocks have their first short connections 
 */
size_t RRGSB::get_hint_rotate_offset(const RRGSB& cand) const {
  size_t offset_hint = size_t(-1);

  assert (get_num_sides() == cand.get_num_sides());

  /* check the numbers/directionality of channel rr_nodes */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);

    /* Ensure we have the same channel width on this side */
    assert (get_chan_width(side_manager.get_side()) == cand.get_chan_width(side_manager.get_side()));

    /* Find the track id of the first short connection */
    size_t src_offset = get_track_id_first_short_connection(side_manager.get_side()); 
    size_t des_offset = cand.get_track_id_first_short_connection(side_manager.get_side()); 
    if ( size_t(-1) == src_offset || size_t(-1) == des_offset ) { 
      return 0; /* default we give zero */
    }
    size_t temp_hint = abs( (int)(src_offset - des_offset));
    offset_hint = std::min(temp_hint, offset_hint);
  }
  return offset_hint;
} 

/* check if all the routing segments of a side of candidate SB is a mirror of the current one */
bool RRGSB::is_sb_side_segment_mirror(const RRGSB& cand, enum e_side side, size_t seg_id) const {
  /* Create a side manager */
  Side side_manager(side);

  /* Make sure both Switch blocks has this side!!! */
  assert ( side_manager.to_size_t() < get_num_sides() ); 
  assert ( side_manager.to_size_t() < cand.get_num_sides() ); 


  /* check the numbers/directionality of channel rr_nodes */
  /* Ensure we have the same channel width on this side */
  if (get_chan_width(side) != cand.get_chan_width(side)) {
    return false;
  }
  for (size_t itrack = 0; itrack < get_chan_width(side); ++itrack) {
    /* Bypass unrelated segments */
    if (seg_id != get_chan_node_segment(side, itrack)) {
      continue;
    }
    /* Check the directionality of each node */
    if (get_chan_node_direction(side, itrack) != cand.get_chan_node_direction(side, itrack)) {
      return false;
    }
    /* Check the track_id of each node
     * ptc is not necessary, we care the connectivity!
    if (get_chan_node(side_manager.get_side(), itrack)->ptc_num != cand.get_chan_node(side_manager.get_side(), itrack)->ptc_num) {
      eturn false;
    }
    */
    /* For OUT_PORT rr_node, we need to check fan-in */
    if (OUT_PORT != get_chan_node_direction(side, itrack)) {
      continue; /* skip IN_PORT */
    }

    if (false == is_sb_node_mirror(cand, side, itrack)) {
      return false;
    } 
  }

  /* check the numbers of opin_rr_nodes */
  if (get_num_opin_nodes(side) != cand.get_num_opin_nodes(side)) {
    return false;
  }

  /* check the numbers of ipin_rr_nodes */
  if (get_num_ipin_nodes(side) != cand.get_num_ipin_nodes(side)) {
    return false;
  }

  return true;
} 

/* check if a side of candidate SB is a mirror of the current one 
 * Check the specified side of two switch blocks: 
 * 1. Number of channel/opin/ipin rr_nodes are same 
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes, 
 * 4. check if their parent type_descriptors same, 
 * 5. check if pin class id and pin id are same 
 * If all above are satisfied, the side of the two switch blocks are mirrors!
 */
bool RRGSB::is_sb_side_mirror(const RRGSB& cand, enum e_side side) const {

  /* get a list of segments */
  std::vector<size_t> seg_ids = get_chan(side).get_segment_ids();

  for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) {
    if (false == is_sb_side_segment_mirror(cand, side, seg_ids[iseg])) {
      return false;
    }
  }

  return true;
}


/* check if the candidate SB is a mirror of the current one */
/* Idenify mirror Switch blocks 
 * Check each two switch blocks: 
 * 1. Number of channel/opin/ipin rr_nodes are same 
 * For channel rr_nodes
 * 2. check if their track_ids (ptc_num) are same
 * 3. Check if the switches (ids) are same
 * For opin/ipin rr_nodes, 
 * 4. check if their parent type_descriptors same, 
 * 5. check if pin class id and pin id are same 
 * If all above are satisfied, the two switch blocks are mirrors!
 */
bool RRGSB::is_sb_mirror(const RRGSB& cand) const {
  /* check the numbers of sides */
  if (get_num_sides() != cand.get_num_sides()) {
    return false;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);
    if (false == is_sb_side_mirror(cand, side_manager.get_side())) {
      return false;
    } 
  }

  /* Make sure the number of conf bits are the same */
  if ( ( get_sb_num_conf_bits() != cand.get_sb_num_conf_bits() ) 
    || ( get_sb_num_reserved_conf_bits() != cand.get_sb_num_reserved_conf_bits() ) ) { 
    return false;
  }

  return true;
}

/* Public Accessors: Cooridinator conversion */

/* get the x coordinator of this GSB */
size_t RRGSB::get_x() const {
  return coordinator_.get_x();
} 

/* get the y coordinator of this GSB */
size_t RRGSB::get_y() const { 
  return coordinator_.get_y();
}


/* get the x coordinator of this switch block */
size_t RRGSB::get_sb_x() const {
  return coordinator_.get_x();
} 

/* get the y coordinator of this switch block */
size_t RRGSB::get_sb_y() const { 
  return coordinator_.get_y();
}

/* Get the number of sides of this SB */
DeviceCoordinator RRGSB::get_sb_coordinator() const {
  return coordinator_;
} 

/* get the x coordinator of this X/Y-direction block */
size_t RRGSB::get_cb_x(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return get_side_block_coordinator(LEFT).get_x();
  case CHANY:
    return get_side_block_coordinator(TOP).get_x();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
} 

/* get the y coordinator of this X/Y-direction block */
size_t RRGSB::get_cb_y(t_rr_type cb_type) const { 
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return get_side_block_coordinator(LEFT).get_y();
  case CHANY:
    return get_side_block_coordinator(TOP).get_y();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

/* Get the coordinator of the X/Y-direction CB */
DeviceCoordinator RRGSB::get_cb_coordinator(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return get_side_block_coordinator(LEFT);
  case CHANY:
    return get_side_block_coordinator(TOP);
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
} 

enum e_side RRGSB::get_cb_chan_side(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return LEFT;
  case CHANY:
    return TOP;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

/* Get the side of routing channel in the GSB according to the side of IPIN */
enum e_side RRGSB::get_cb_chan_side(enum e_side ipin_side) const {
  switch(ipin_side) {
  case TOP:
    return LEFT;
  case RIGHT:
    return TOP;
  case BOTTOM:
    return LEFT;
  case LEFT:
    return TOP;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of ipin_side!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

DeviceCoordinator RRGSB::get_side_block_coordinator(enum e_side side) const {
  Side side_manager(side); 
  assert(side_manager.validate());
  DeviceCoordinator ret(get_sb_x(), get_sb_y());

  switch (side_manager.get_side()) {
  case TOP:
    /* (0 == side) */
    /* 1. Channel Y [x][y+1] inputs */
    ret.set_y(ret.get_y() + 1);
    break;
  case RIGHT:
    /* 1 == side */
    /* 2. Channel X [x+1][y] inputs */
    ret.set_x(ret.get_x() + 1);
    break;
  case BOTTOM:
    /* 2 == side */
    /* 3. Channel Y [x][y] inputs */
    break;
  case LEFT:
    /* 3 == side */
    /* 4. Channel X [x][y] inputs */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File: %s [LINE%d]) Invalid side!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return ret;
}

DeviceCoordinator RRGSB::get_grid_coordinator() const {
  DeviceCoordinator ret(get_sb_x(), get_sb_y());
  ret.set_y(ret.get_y() + 1);

  return ret;
}

/* Public Accessors Verilog writer */
const char* RRGSB::gen_cb_verilog_routing_track_name(t_rr_type cb_type,
                                                     size_t track_id) const {
  std::string cb_name(convert_chan_type_to_string(cb_type));
  std::string x_str = std::to_string(get_cb_x(cb_type));
  std::string y_str = std::to_string(get_cb_y(cb_type));
  std::string track_id_str = std::to_string(track_id);

  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( cb_name.length() + 1 
                               + x_str.length() + 2
                               + y_str.length() + 9 
                               + track_id_str.length() + 1
                               + 1));
  sprintf (ret, "%s_%s__%s__midout_%s_",
           cb_name.c_str(), x_str.c_str(), y_str.c_str(), track_id_str.c_str());

  return ret;
}

const char* RRGSB::gen_sb_verilog_module_name() const {
  std::string x_str = std::to_string(get_sb_x());
  std::string y_str = std::to_string(get_sb_y());

  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( 2 + 1 
                               + x_str.length() + 2
                               + y_str.length() + 1 
                               + 1));
  sprintf (ret, "sb_%s__%s_",
           x_str.c_str(), y_str.c_str());

  return ret;
}

const char* RRGSB::gen_gsb_verilog_module_name() const {
  std::string x_str = std::to_string(get_sb_x());
  std::string y_str = std::to_string(get_sb_y());

  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( 2 + 1 
                               + x_str.length() + 2
                               + y_str.length() + 1 
                               + 1));
  sprintf (ret, "gsb_%s__%s_",
           x_str.c_str(), y_str.c_str());

  return ret;
}

const char* RRGSB::gen_sb_verilog_instance_name() const {
  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( strlen(gen_sb_verilog_module_name()) + 3 
                               + 1));
  sprintf (ret, "%s_0_",
           gen_sb_verilog_module_name());

  return ret;
}

const char* RRGSB::gen_gsb_verilog_instance_name() const {
  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( strlen(gen_gsb_verilog_module_name()) + 3 
                               + 1));
  sprintf (ret, "%s_0_",
           gen_gsb_verilog_module_name());

  return ret;
}

/* Public Accessors Verilog writer */
const char* RRGSB::gen_sb_verilog_side_module_name(enum e_side side, size_t seg_id) const {
  Side side_manager(side);
  
  std::string prefix_str(gen_sb_verilog_module_name());
  std::string seg_id_str(std::to_string(seg_id));
  std::string side_str(side_manager.to_string());

  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( prefix_str.length() + 1 
                               + side_str.length() + 5
                               + seg_id_str.length() + 1
                               + 1));
  sprintf (ret, "%s_%s_seg_%s_",
           prefix_str.c_str(), side_str.c_str(), seg_id_str.c_str());

  return ret;
}

const char* RRGSB::gen_sb_verilog_side_instance_name(enum e_side side, size_t seg_id) const {
  std::string prefix_str = gen_sb_verilog_side_module_name(side, seg_id);
  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( prefix_str.length() + 3 
                               + 1));
  sprintf (ret, "%s_0_",
           prefix_str.c_str());

  return ret;
}

/* Public Accessors Verilog writer */
const char* RRGSB::gen_cb_verilog_module_name(t_rr_type cb_type) const {
  /* check */
  assert (validate_cb_type(cb_type));

  std::string prefix_str = convert_cb_type_to_string(cb_type);
  std::string x_str = std::to_string(get_cb_x(cb_type));
  std::string y_str = std::to_string(get_cb_y(cb_type));

  char* ret = (char*)my_malloc(sizeof(char)* 
                               ( prefix_str.length() + 1 
                               + x_str.length() + 2 
                               + y_str.length() + 1 
                               + 1));
  sprintf (ret, "%s_%s__%s_",
           prefix_str.c_str(), x_str.c_str(), y_str.c_str());

  return ret;
}

const char* RRGSB::gen_cb_verilog_instance_name(t_rr_type cb_type) const {
  /* check */
  assert (validate_cb_type(cb_type));

  std::string prefix_str = gen_cb_verilog_module_name(cb_type);
  char* ret = (char*)my_malloc(sizeof(char)* 
                               (prefix_str.length() + 3 
                               + 1));
  sprintf (ret, "%s_0_",
           prefix_str.c_str());

  return ret;
}

/* Public mutators */

/* get a copy from a source */
void RRGSB::set(const RRGSB& src) { 
  /* Copy coordinator */
  this->set_coordinator(src.get_sb_coordinator().get_x(), src.get_sb_coordinator().get_y());

  /* Initialize sides */ 
  this->init_num_sides(src.get_num_sides());

  /* Copy vectors */
  for (size_t side = 0; side < src.get_num_sides(); ++side) {
    Side side_manager(side);
    /* Copy chan_nodes */
    /* skip if there is no channel width */
    if ( 0 < src.get_chan_width(side_manager.get_side()) ) {
      this->chan_node_[side_manager.get_side()].set(src.get_chan(side_manager.get_side()));
      /* Copy chan_node_direction_*/
      this->chan_node_direction_[side_manager.get_side()].clear();
      for (size_t inode = 0; inode < src.get_chan_width(side_manager.get_side()); ++inode) {
        this->chan_node_direction_[side_manager.get_side()].push_back(src.get_chan_node_direction(side_manager.get_side(), inode));
      }
    }

    /* Copy opin_node and opin_node_grid_side_ */
    this->opin_node_[side_manager.get_side()].clear();
    this->opin_node_grid_side_[side_manager.get_side()].clear();
    for (size_t inode = 0; inode < src.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      this->opin_node_[side_manager.get_side()].push_back(src.get_opin_node(side_manager.get_side(), inode));
      this->opin_node_grid_side_[side_manager.get_side()].push_back(src.get_opin_node_grid_side(side_manager.get_side(), inode));
    }

    /* Copy ipin_node and ipin_node_grid_side_ */
    this->ipin_node_[side_manager.get_side()].clear();
    this->ipin_node_grid_side_[side_manager.get_side()].clear();
    for (size_t inode = 0; inode < src.get_num_ipin_nodes(side_manager.get_side()); ++inode) {
      this->ipin_node_[side_manager.get_side()].push_back(src.get_ipin_node(side_manager.get_side(), inode));
      this->ipin_node_grid_side_[side_manager.get_side()].push_back(src.get_ipin_node_grid_side(side_manager.get_side(), inode));
    }
  }

  /* Copy conf_bits 
   */
  this->set_sb_num_reserved_conf_bits(src.get_sb_num_reserved_conf_bits());
  this->set_sb_conf_bits_lsb(src.get_sb_conf_bits_lsb());
  this->set_sb_conf_bits_msb(src.get_sb_conf_bits_msb());

  this->set_cb_num_reserved_conf_bits(CHANX, src.get_cb_num_reserved_conf_bits(CHANX));
  this->set_cb_conf_bits_lsb(CHANX, src.get_cb_conf_bits_lsb(CHANX));
  this->set_cb_conf_bits_msb(CHANX, src.get_cb_conf_bits_msb(CHANX));

  this->set_cb_num_reserved_conf_bits(CHANY, src.get_cb_num_reserved_conf_bits(CHANY));
  this->set_cb_conf_bits_lsb(CHANY, src.get_cb_conf_bits_lsb(CHANY));
  this->set_cb_conf_bits_msb(CHANY, src.get_cb_conf_bits_msb(CHANY));
  
  return;
}

/* Set the coordinator (x,y) for the switch block */
void RRGSB::set_coordinator(size_t x, size_t y) {
  coordinator_.set(x, y);
  return;
}

/* Allocate the vectors with the given number of sides */
void RRGSB::init_num_sides(size_t num_sides) {
  /* Initialize the vectors */
  chan_node_.resize(num_sides);
  chan_node_direction_.resize(num_sides);
  ipin_node_.resize(num_sides);
  ipin_node_grid_side_.resize(num_sides);
  opin_node_.resize(num_sides);
  opin_node_grid_side_.resize(num_sides);
  return;
}

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRGSB::add_chan_node(enum e_side node_side, RRChan& rr_chan, std::vector<enum PORTS> rr_chan_dir) {
  Side side_manager(node_side);
  /* Validate: 1. side is valid, the type of node is valid */
  assert(validate_side(node_side));

  /* fill the dedicated element in the vector */
  chan_node_[side_manager.to_size_t()].set(rr_chan);
  chan_node_direction_[side_manager.to_size_t()].resize(rr_chan_dir.size());
  for (size_t inode = 0; inode < rr_chan_dir.size(); ++inode) {
    chan_node_direction_[side_manager.to_size_t()][inode] = rr_chan_dir[inode];
  }

  return;
} 

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRGSB::add_ipin_node(t_rr_node* node, const enum e_side node_side, const enum e_side grid_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  /* push pack the dedicated element in the vector */
  ipin_node_[side_manager.to_size_t()].push_back(node);
  ipin_node_grid_side_[side_manager.to_size_t()].push_back(grid_side);

  return;
}

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRGSB::add_opin_node(t_rr_node* node, const enum e_side node_side, const enum e_side grid_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  /* push pack the dedicated element in the vector */
  opin_node_[side_manager.to_size_t()].push_back(node);
  opin_node_grid_side_[side_manager.to_size_t()].push_back(grid_side);

  return;
} 

void RRGSB::set_sb_num_reserved_conf_bits(size_t num_reserved_conf_bits) {
  return sb_conf_port_.set_reserved_port(num_reserved_conf_bits);
}

void RRGSB::set_sb_conf_bits_lsb(size_t conf_bits_lsb) {
  return sb_conf_port_.set_regular_port_lsb(conf_bits_lsb);
}

void RRGSB::set_sb_conf_bits_msb(size_t conf_bits_msb) {
  return sb_conf_port_.set_regular_port_msb(conf_bits_msb);
}

void RRGSB::set_cb_num_reserved_conf_bits(t_rr_type cb_type, size_t num_reserved_conf_bits) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.set_reserved_port(num_reserved_conf_bits);
  case CHANY:
    return cby_conf_port_.set_reserved_port(num_reserved_conf_bits);
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

void RRGSB::set_cb_conf_bits_lsb(t_rr_type cb_type, size_t conf_bits_lsb) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.set_regular_port_lsb(conf_bits_lsb);
  case CHANY:
    return cby_conf_port_.set_regular_port_lsb(conf_bits_lsb);
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

void RRGSB::set_cb_conf_bits_msb(t_rr_type cb_type, size_t conf_bits_msb) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_conf_port_.set_regular_port_msb(conf_bits_msb);
  case CHANY:
    return cby_conf_port_.set_regular_port_msb(conf_bits_msb);
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

/* rotate the channel nodes with the same direction on one side by a given offset */
void RRGSB::rotate_side_chan_node_by_direction(enum e_side side, enum e_direction chan_dir, size_t offset) {
  Side side_manager(side);
  assert(validate_side(side));

  /* Partition the chan nodes on this side, depending on its length */
  /* skip this side if there is no nodes */
  if (0 == get_chan_width(side)) {
    return;
  }
  
  /* Rotate the chan_nodes */
  chan_node_[side_manager.to_size_t()].rotate_by_node_direction(chan_dir, offset);

  return;
} 

/* rotate the channel nodes with the same direction on one side by a given offset */
void RRGSB::counter_rotate_side_chan_node_by_direction(enum e_side side, enum e_direction chan_dir, size_t offset) {
  Side side_manager(side);
  assert(validate_side(side));

  /* Partition the chan nodes on this side, depending on its length */
  /* skip this side if there is no nodes */
  if (0 == get_chan_width(side)) {
    return;
  }
  
  /* Rotate the chan_nodes */
  chan_node_[side_manager.to_size_t()].counter_rotate_by_node_direction(chan_dir, offset);

  return;

} 


/* rotate all the channel nodes by a given offset */
void RRGSB::rotate_side_chan_node(enum e_side side, size_t offset) {
  Side side_manager(side);
  /* Partition the chan nodes on this side, depending on its length */
  /* skip this side if there is no nodes */
  if (0 == get_chan_width(side)) {
    return;
  }
  size_t adapt_offset = offset % get_chan_width(side);
  assert(adapt_offset < get_chan_width(side));
  /* Find a group split, rotate */
  chan_node_[side_manager.to_size_t()].rotate(adapt_offset);
  std::rotate(chan_node_direction_[side_manager.to_size_t()].begin(), 
              chan_node_direction_[side_manager.to_size_t()].begin() + adapt_offset, 
              chan_node_direction_[side_manager.to_size_t()].end());
  return;
} 


/* rotate all the channel nodes by a given offset */
void RRGSB::rotate_chan_node(size_t offset) {
  /* Rotate chan nodes on each side */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);
    rotate_side_chan_node(side_manager.get_side(), offset);
  }

  return;
} 

/* rotate all the channel nodes by a given offset:
 * Routing Channel nodes are divided into different groups using segment ids
 * each group is rotated separatedly
 */
void RRGSB::rotate_chan_node_in_group(size_t offset) {
  /* Rotate chan nodes on each side */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);
    size_t rotate_begin = 0;
    size_t rotate_end = 0;
    /* Partition the chan nodes on this side, depending on its length */
    /* skip this side if there is no nodes */
    if (0 == get_chan_width(side_manager.get_side())) {
      continue;
    }
    for (size_t inode = 0; inode < get_chan_width(side_manager.get_side()) - 1; ++inode) {
      if ( (get_chan_node_segment(side_manager.get_side(), inode) != get_chan_node_segment(side_manager.get_side(), inode + 1)) 
        || ( inode == get_chan_width(side_manager.get_side()) - 2) ) {
        /* Record the upper bound  */
        if ( inode == get_chan_width(side_manager.get_side()) - 2)  {
          rotate_end = get_chan_width(side_manager.get_side()) - 1;
        } else {
          rotate_end = inode;
        }
        /* Make sure offset is in range */
        /* skip this side if there is no nodes */
        if (0 >= rotate_end - rotate_begin) {
          /* Update the lower bound  */
          rotate_begin = inode + 1;
          continue;
        }
        assert(offset < rotate_end - rotate_begin + 1);
        /* Find a group split, rotate */
        chan_node_[side].rotate(rotate_begin, rotate_end, offset);
        std::rotate(chan_node_direction_[side].begin() + rotate_begin, 
                    chan_node_direction_[side].begin() + rotate_begin + offset, 
                    chan_node_direction_[side].begin() + rotate_end);
        /* Update the lower bound  */
        rotate_begin = inode + 1;
      }
    }

  }

  return;
} 

/* rotate one side of the opin nodes by a given offset 
 * OPIN nodes are divided into different groups depending on their grid 
 * each group is rotated separatedly
 */
void RRGSB::rotate_side_opin_node_in_group(enum e_side side, size_t offset) {
  /* Rotate opin nodes on each side */
  Side side_manager(side);
  size_t rotate_begin = 0;
  size_t rotate_end = 0;
  /* skip this side if there is no nodes */
  if (0 == get_num_opin_nodes(side)) {
    return;
  }
  /* Partition the opin nodes on this side, depending on grids */
  for (size_t inode = 0; inode < get_num_opin_nodes(side) - 1; ++inode) {
    if ( ( (opin_node_[side_manager.to_size_t()][inode]->xlow  != opin_node_[side_manager.to_size_t()][inode + 1]->xlow) 
        || (opin_node_[side_manager.to_size_t()][inode]->ylow  != opin_node_[side_manager.to_size_t()][inode + 1]->ylow) 
        || (opin_node_[side_manager.to_size_t()][inode]->xhigh != opin_node_[side_manager.to_size_t()][inode + 1]->xhigh) 
        || (opin_node_[side_manager.to_size_t()][inode]->yhigh != opin_node_[side_manager.to_size_t()][inode + 1]->yhigh)
        || (opin_node_grid_side_[side_manager.to_size_t()][inode] != opin_node_grid_side_[side_manager.to_size_t()][inode + 1]))
        || ( inode == get_num_opin_nodes(side) - 2) ) {
      /* Record the upper bound  */
      if ( inode == get_num_opin_nodes(side) - 2)  {
        rotate_end = get_num_opin_nodes(side) - 1;
      } else {
        rotate_end = inode;
      }
      /* skip this side if there is no nodes */
      if (0 >= rotate_end - rotate_begin) {
        /* Update the lower bound  */
        rotate_begin = inode + 1;
        continue;
      }
      size_t adapt_offset = offset % (rotate_end - rotate_begin + 1);
      /* Make sure offset is in range */
      assert (adapt_offset < rotate_end - rotate_begin + 1);
      /* Find a group split, rotate */
      std::rotate(opin_node_[side_manager.to_size_t()].begin() + rotate_begin, 
                  opin_node_[side_manager.to_size_t()].begin() + rotate_begin + adapt_offset, 
                  opin_node_[side_manager.to_size_t()].begin() + rotate_end);
      std::rotate(opin_node_grid_side_[side_manager.to_size_t()].begin() + rotate_begin, 
                  opin_node_grid_side_[side_manager.to_size_t()].begin() + rotate_begin + adapt_offset, 
                  opin_node_grid_side_[side_manager.to_size_t()].begin() + rotate_end);
      /* Update the lower bound  */
      rotate_begin = inode + 1;
    }
  }

  return;
} 


/* rotate all the opin nodes by a given offset 
 * OPIN nodes are divided into different groups depending on their grid 
 * each group is rotated separatedly
 */
void RRGSB::rotate_opin_node_in_group(size_t offset) {
  /* Rotate opin nodes on each side */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);
    rotate_side_opin_node_in_group(side_manager.get_side(), offset);
  }

  return;
} 

/* rotate all the channel and opin nodes by a given offset */
void RRGSB::rotate(size_t offset) {
  rotate_chan_node(offset);
  rotate_opin_node_in_group(offset);
  return;
} 

/* rotate one side of the channel and opin nodes by a given offset */
void RRGSB::rotate_side(enum e_side side, size_t offset) {
  rotate_side_chan_node(side, offset);
  rotate_side_opin_node_in_group(side, offset);
  return;
} 

/* Mirror the node direction and port direction of routing track nodes on a side */
void RRGSB::mirror_side_chan_node_direction(enum e_side side) {
  assert(validate_side(side));
  Side side_manager(side);

  chan_node_[side_manager.to_size_t()].mirror_node_direction();
  return;
} 

/* swap the chan rr_nodes on two sides */
void RRGSB::swap_chan_node(enum e_side src_side, enum e_side des_side) {
  Side src_side_manager(src_side);
  Side des_side_manager(des_side);
  std::swap(chan_node_[src_side_manager.to_size_t()], 
            chan_node_[des_side_manager.to_size_t()]);
  std::swap(chan_node_direction_[src_side_manager.to_size_t()], 
            chan_node_direction_[des_side_manager.to_size_t()]);
  return;
} 

/* swap the OPIN rr_nodes on two sides */
void RRGSB::swap_opin_node(enum e_side src_side, enum e_side des_side) {
  Side src_side_manager(src_side);
  Side des_side_manager(des_side);
  std::swap(opin_node_[src_side_manager.to_size_t()], 
            opin_node_[des_side_manager.to_size_t()]);
  std::swap(opin_node_grid_side_[src_side_manager.to_size_t()], 
            opin_node_grid_side_[des_side_manager.to_size_t()]);
  return;
} 

/* swap the IPIN rr_nodes on two sides */
void RRGSB::swap_ipin_node(enum e_side src_side, enum e_side des_side) {
  Side src_side_manager(src_side);
  Side des_side_manager(des_side);
  std::swap(ipin_node_[src_side_manager.to_size_t()], 
            ipin_node_[des_side_manager.to_size_t()]);
  std::swap(ipin_node_grid_side_[src_side_manager.to_size_t()], 
            ipin_node_grid_side_[des_side_manager.to_size_t()]);
  return;
} 

/* Reverse the vector of the OPIN rr_nodes on a side */
void RRGSB::reverse_opin_node(enum e_side side) {
  Side side_manager(side);
  std::reverse(opin_node_[side_manager.to_size_t()].begin(), 
               opin_node_[side_manager.to_size_t()].end());
  std::reverse(opin_node_grid_side_[side_manager.to_size_t()].begin(), 
               opin_node_grid_side_[side_manager.to_size_t()].end());
  return;
} 

/* Reverse the vector of the OPIN rr_nodes on a side */
void RRGSB::reverse_ipin_node(enum e_side side) {
  Side side_manager(side);
  std::reverse(ipin_node_[side_manager.to_size_t()].begin(), 
               ipin_node_[side_manager.to_size_t()].end());
  std::reverse(ipin_node_grid_side_[side_manager.to_size_t()].begin(), 
               ipin_node_grid_side_[side_manager.to_size_t()].end());
  return;
} 

/* Reset the RRGSB to pristine state */
void RRGSB::clear() {
  /* Clean all the vectors */
  assert(validate_num_sides());
  /* Clear the inner vector of each matrix */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    chan_node_direction_[side].clear();
    chan_node_[side].clear();
    ipin_node_[side].clear();
    ipin_node_grid_side_[side].clear();
    opin_node_[side].clear();
    opin_node_grid_side_[side].clear();
  }  
  chan_node_direction_.clear();
  chan_node_.clear();
  ipin_node_.clear();
  ipin_node_grid_side_.clear();
  opin_node_.clear();
  opin_node_grid_side_.clear();

  /* Just to make the lsb and msb invalidate */
  sb_conf_port_.reset();
  cbx_conf_port_.reset();
  cby_conf_port_.reset();

  return;
}

/* Clean the chan_width of a side */
void RRGSB::clear_chan_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  chan_node_[side_manager.to_size_t()].clear();
  chan_node_direction_[side_manager.to_size_t()].clear();
  return;
} 

/* Clean the number of IPINs of a side */
void RRGSB::clear_ipin_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  ipin_node_[side_manager.to_size_t()].clear();
  ipin_node_grid_side_[side_manager.to_size_t()].clear();
  return;
} 

/* Clean the number of OPINs of a side */
void RRGSB::clear_opin_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  opin_node_[side_manager.to_size_t()].clear();
  opin_node_grid_side_[side_manager.to_size_t()].clear();
  return;
}

/* Clean chan/opin/ipin nodes at one side */
void RRGSB::clear_one_side(enum e_side node_side) {
  clear_chan_nodes(node_side);
  clear_ipin_nodes(node_side);
  clear_opin_nodes(node_side);

  return;
} 


/* Internal functions for validation */

/* check if two rr_nodes have a similar set of drive_rr_nodes 
 * for each drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. OPIN or IPIN: should have the same side and index
 * 3. each drive_rr_switch should be the same 
 */
bool RRGSB::is_sb_node_mirror(const RRGSB& cand, 
                              enum e_side node_side, 
                              size_t track_id) const {
  /* Ensure rr_nodes are either the output of short-connection or multiplexer  */
  t_rr_node* node = this->get_chan_node(node_side, track_id);
  t_rr_node* cand_node = cand.get_chan_node(node_side, track_id);
  bool is_short_conkt = this->is_sb_node_imply_short_connection(node);

  if (is_short_conkt != cand.is_sb_node_imply_short_connection(cand_node)) {
    return false;
  }
  /* Find the driving rr_node in this sb */
  if (true == is_short_conkt) {
    /* Ensure we have the same track id for the driving nodes */
    if ( this->is_sb_node_exist_opposite_side(node, node_side)
      != cand.is_sb_node_exist_opposite_side(cand_node, node_side)) {
      return false;
    }
  } else { /* check driving rr_nodes */
    if ( node->num_drive_rr_nodes != cand_node->num_drive_rr_nodes ) {
      return false;
    }
    for (size_t inode = 0; inode < size_t(node->num_drive_rr_nodes); ++inode) {
      /* node type should be the same  */
      if ( node->drive_rr_nodes[inode]->type
        != cand_node->drive_rr_nodes[inode]->type) {
        return false;
      }
      /* switch type should be the same  */
      if ( node->drive_switches[inode]
        != cand_node->drive_switches[inode]) {
        return false;
      }
      int src_node_id, des_node_id;
      enum e_side src_node_side, des_node_side; 
      this->get_node_side_and_index(node->drive_rr_nodes[inode], OUT_PORT, &src_node_side, &src_node_id);
       cand.get_node_side_and_index(cand_node->drive_rr_nodes[inode], OUT_PORT, &des_node_side, &des_node_id);
      if (src_node_id != des_node_id) {
        return false;
      } 
      if (src_node_side != des_node_side) {
        return false;
      } 
    }
  }

  return true;
} 

/* check if two ipin_nodes have a similar set of drive_rr_nodes 
 * for each drive_rr_node:
 * 1. CHANX or CHANY: should have the same side and index
 * 2. each drive_rr_switch should be the same 
 */
bool RRGSB::is_cb_node_mirror(const RRGSB& cand, t_rr_type cb_type,
                              enum e_side node_side, 
                              size_t node_id) const {
  /* Ensure rr_nodes are either the output of short-connection or multiplexer  */
  t_rr_node* node = this->get_ipin_node(node_side, node_id);
  t_rr_node* cand_node = cand.get_ipin_node(node_side, node_id);

  if ( node->num_drive_rr_nodes != cand_node->num_drive_rr_nodes ) {
    return false;
  }
  for (size_t inode = 0; inode < size_t(node->num_drive_rr_nodes); ++inode) {
    /* node type should be the same  */
    if ( node->drive_rr_nodes[inode]->type
      != cand_node->drive_rr_nodes[inode]->type) {
      return false;
    }
    /* switch type should be the same  */
    if ( node->drive_switches[inode]
      != cand_node->drive_switches[inode]) {
      return false;
    }

    int src_node_id, des_node_id;
    enum e_side src_node_side, des_node_side; 
    enum e_side chan_side = get_cb_chan_side(cb_type);
    switch (node->drive_rr_nodes[inode]->type) {
    case CHANX:
    case CHANY:
      /* if the drive rr_nodes are routing tracks, find index  */
      src_node_id = this->get_chan_node_index(chan_side, node->drive_rr_nodes[inode]);
      des_node_id =  cand.get_chan_node_index(chan_side, cand_node->drive_rr_nodes[inode]);
      break;
    case OPIN:
      this->get_node_side_and_index(node->drive_rr_nodes[inode], OUT_PORT, &src_node_side, &src_node_id);
       cand.get_node_side_and_index(cand_node->drive_rr_nodes[inode], OUT_PORT, &des_node_side, &des_node_id);
      if (src_node_side != des_node_side) {
        return false;
      } 
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                "(File:%s, [LINE%d])Invalid type of drive_rr_nodes for ipin_node!\n", 
                __FILE__, __LINE__);
      exit(1);
    }
    if (src_node_id != des_node_id) {
      return false;
    } 
  }

  return true;
} 

size_t RRGSB::get_track_id_first_short_connection(enum e_side node_side) const {
  assert(validate_side(node_side));

  /* Walk through chan_nodes and find the first short connection */
  for (size_t inode = 0; inode < get_chan_width(node_side); ++inode) {
    if (true == is_sb_node_imply_short_connection(get_chan_node(node_side, inode))) {
      return inode; 
    }
  }

  return size_t(-1);
}

/* Validate if the number of sides are consistent among internal data arrays ! */
bool RRGSB::validate_num_sides() const {
  size_t num_sides = chan_node_direction_.size();

  if ( num_sides != chan_node_.size() ) {
    return false;
  }

  if ( num_sides != ipin_node_.size() ) {
    return false;
  }

  if ( num_sides != ipin_node_grid_side_.size() ) {
    return false;
  }

  if ( num_sides != opin_node_.size() ) {
    return false;
  }

  if ( num_sides != opin_node_grid_side_.size() ) {
    return false;
  }

  return true; 
}

/* Check if the side valid in the context: does the switch block have the side? */
bool RRGSB::validate_side(enum e_side side) const {
  Side side_manager(side);
  if ( side_manager.to_size_t() < get_num_sides() ) {
    return true;
  }
  return false;
}

/* Check the track_id is valid for chan_node_ and chan_node_direction_ */
bool RRGSB::validate_track_id(enum e_side side, size_t track_id) const {
  Side side_manager(side);

  if (false == validate_side(side)) {
    return false;
  } 
  if ( ( track_id < chan_node_[side_manager.to_size_t()].get_chan_width()) 
    && ( track_id < chan_node_direction_[side_manager.to_size_t()].size()) ) {
    return true;
  }

  return false;
}

/* Check the opin_node_id is valid for opin_node_ and opin_node_grid_side_ */
bool RRGSB::validate_opin_node_id(enum e_side side, size_t node_id) const {
  Side side_manager(side);

  if (false == validate_side(side)) {
    return false;
  } 
  if ( ( node_id < opin_node_[side_manager.to_size_t()].size())
     &&( node_id < opin_node_grid_side_[side_manager.to_size_t()].size()) ) {
    return true;
  }

  return false;
}

/* Check the ipin_node_id is valid for opin_node_ and opin_node_grid_side_ */
bool RRGSB::validate_ipin_node_id(enum e_side side, size_t node_id) const {
  Side side_manager(side);

  if (false == validate_side(side)) {
    return false;
  } 
  if ( ( node_id < ipin_node_[side_manager.to_size_t()].size())
     &&( node_id < ipin_node_grid_side_[side_manager.to_size_t()].size()) ) {
    return true;
  }

  return false;
}

bool RRGSB::validate_cb_type(t_rr_type cb_type) const {
  if ( (CHANX == cb_type) || (CHANY == cb_type) ) {
    return true;
  }
  return false;
}

/* Member Functions of Class RRChan */
/* Accessors */

/* get the max coordinator of the switch block array */
DeviceCoordinator DeviceRRGSB::get_gsb_range() const {
  size_t max_y = 0;
  /* Get the largest size of sub-arrays */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    max_y = std::max(max_y, rr_gsb_[x].size());
  }
  
  DeviceCoordinator coordinator(rr_gsb_.size(), max_y);
  return coordinator;
} 

/* Get a rr switch block in the array with a coordinator */
const RRGSB DeviceRRGSB::get_gsb(DeviceCoordinator& coordinator) const {
  assert(validate_coordinator(coordinator));
  return rr_gsb_[coordinator.get_x()][coordinator.get_y()];
} 

/* Get a rr switch block in the array with a coordinator */
const RRGSB DeviceRRGSB::get_gsb(size_t x, size_t y) const { 
  DeviceCoordinator coordinator(x, y);  
  return get_gsb(coordinator);
}

/* get the number of unique side modules of switch blocks */
size_t DeviceRRGSB::get_num_sb_unique_submodule(enum e_side side, size_t seg_index) const {
  Side side_manager(side);
  assert(validate_side(side));
  assert(validate_segment_index(seg_index));
  return sb_unique_submodule_[side_manager.to_size_t()][seg_index].size();
} 

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRGSB::get_num_cb_unique_module(t_rr_type cb_type) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_unique_module_.size();
  case CHANY:
    return cby_unique_module_.size();
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
} 

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRGSB::get_num_sb_unique_module() const {
  return sb_unique_module_.size();
} 


/* Get the submodule id of a SB */ 
size_t DeviceRRGSB::get_sb_unique_submodule_id(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const {
  assert (validate_coordinator(coordinator));

  Side side_manager(side);
  assert (validate_side(side));
  assert (validate_segment_index(seg_id));
  
  size_t x = coordinator.get_x();
  size_t y = coordinator.get_y();
  return sb_unique_submodule_id_[x][y][side][seg_id];
}

/* Get a rr switch block which is a unique module of a side of SB */ 
const RRGSB DeviceRRGSB::get_sb_unique_submodule(size_t index, enum e_side side, size_t seg_id) const {
  assert (validate_sb_unique_submodule_index(index, side, seg_id));

  Side side_manager(side);
  assert (validate_side(side));
   
  size_t x = sb_unique_submodule_[side_manager.to_size_t()][seg_id][index].get_x();
  size_t y = sb_unique_submodule_[side_manager.to_size_t()][seg_id][index].get_y();
  
  return rr_gsb_[x][y];
}

/* Get a rr switch block which is a unique module of a side of SB */ 
const RRGSB DeviceRRGSB::get_sb_unique_submodule(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const {
  assert (validate_coordinator(coordinator));

  Side side_manager(side);
  assert (validate_side(side));
  
  size_t module_id = get_sb_unique_submodule_id(coordinator, side, seg_id);
  
  return get_sb_unique_submodule(module_id, side, seg_id);
}


/* Get a rr switch block which a unique mirror */ 
const RRGSB DeviceRRGSB::get_sb_unique_module(size_t index) const {
  assert (validate_sb_unique_module_index(index));
  
  return rr_gsb_[sb_unique_module_[index].get_x()][sb_unique_module_[index].get_y()];
}

/* Get a rr switch block which a unique mirror */ 
const RRGSB DeviceRRGSB::get_cb_unique_module(t_rr_type cb_type, size_t index) const {
  assert (validate_cb_unique_module_index(cb_type, index));
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX: 
   return rr_gsb_[cbx_unique_module_[index].get_x()][cbx_unique_module_[index].get_y()];
  case CHANY:
   return rr_gsb_[cby_unique_module_[index].get_x()][cby_unique_module_[index].get_y()];
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }  
}

/* Give a coordinator of a rr switch block, and return its unique mirror */ 
const RRGSB DeviceRRGSB::get_cb_unique_module(t_rr_type cb_type, DeviceCoordinator& coordinator) const {
  assert (validate_cb_type(cb_type));
  assert(validate_coordinator(coordinator));
  size_t cb_unique_module_id;

  switch(cb_type) {
  case CHANX:
    cb_unique_module_id = cbx_unique_module_id_[coordinator.get_x()][coordinator.get_y()];  
    break;
  case CHANY:
    cb_unique_module_id = cby_unique_module_id_[coordinator.get_x()][coordinator.get_y()];  
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }  

  return get_cb_unique_module(cb_type, cb_unique_module_id);
} 

/* Give a coordinator of a rr switch block, and return its unique mirror */ 
const RRGSB DeviceRRGSB::get_sb_unique_module(DeviceCoordinator& coordinator) const {
  assert(validate_coordinator(coordinator));
  size_t sb_unique_module_id = sb_unique_module_id_[coordinator.get_x()][coordinator.get_y()];  
  return get_sb_unique_module(sb_unique_module_id);
} 

/* Get the maximum number of sides across the switch blocks */
size_t DeviceRRGSB::get_max_num_sides() const {
  size_t max_num_sides = 0;
  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      max_num_sides = std::max(max_num_sides, rr_gsb_[ix][iy].get_num_sides());
    }
  }
  return max_num_sides;
}

/* Get the size of segment_ids */
size_t DeviceRRGSB::get_num_segments() const { 
  return segment_ids_.size();
}

/* Get a segment id */
size_t DeviceRRGSB::get_segment_id(size_t index) const {
  assert(validate_segment_index(index));
  return segment_ids_[index];
}

/* Evaluate if the Switch Blocks of two GSBs share exactly the same submodule */
bool DeviceRRGSB::is_two_sb_share_same_submodules(DeviceCoordinator& src, DeviceCoordinator& des) const {

  /* check the numbers of sides */
  if (get_gsb(src).get_num_sides() != get_gsb(des).get_num_sides()) {
    return false;
  }

  /* check the numbers/directionality of channel rr_nodes */
  for (size_t side = 0; side < get_gsb(src).get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t iseg = 0; iseg < get_num_segments(); ++iseg) {
      if ( get_sb_unique_submodule_id(src, side_manager.get_side(), iseg) 
        != get_sb_unique_submodule_id(des, side_manager.get_side(), iseg)) {
        return false;
      }
    }
  }
  return true;
}

/* Public Mutators */

/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_cb_num_reserved_conf_bits(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t num_reserved_conf_bits) {
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_cb_num_reserved_conf_bits(cb_type, num_reserved_conf_bits);
  return;
} 

/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_cb_conf_bits_lsb(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t conf_bits_lsb) { 
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_cb_conf_bits_lsb(cb_type, conf_bits_lsb);
  return;
}

/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_cb_conf_bits_msb(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t conf_bits_msb) { 
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_cb_conf_bits_msb(cb_type, conf_bits_msb);
  return;
}


/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_sb_num_reserved_conf_bits(DeviceCoordinator& coordinator, size_t num_reserved_conf_bits) {
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_sb_num_reserved_conf_bits(num_reserved_conf_bits);
  return;
} 

/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_sb_conf_bits_lsb(DeviceCoordinator& coordinator, size_t conf_bits_lsb) { 
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_sb_conf_bits_lsb(conf_bits_lsb);
  return;
}

/* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
void DeviceRRGSB::set_sb_conf_bits_msb(DeviceCoordinator& coordinator, size_t conf_bits_msb) { 
  assert(validate_coordinator(coordinator));
  rr_gsb_[coordinator.get_x()][coordinator.get_y()].set_sb_conf_bits_msb(conf_bits_msb);
  return;
}

/* Pre-allocate the rr_switch_block array that the device requires */ 
void DeviceRRGSB::reserve(DeviceCoordinator& coordinator) { 
  rr_gsb_.resize(coordinator.get_x());

  sb_unique_submodule_id_.resize(coordinator.get_x());
  sb_unique_module_id_.resize(coordinator.get_x());

  cbx_unique_module_id_.resize(coordinator.get_x());
  cby_unique_module_id_.resize(coordinator.get_x());

  for (size_t x = 0; x < coordinator.get_x(); ++x) {
    rr_gsb_[x].resize(coordinator.get_y()); 

    sb_unique_submodule_id_[x].resize(coordinator.get_y());
    sb_unique_module_id_[x].resize(coordinator.get_y()); 

    cbx_unique_module_id_[x].resize(coordinator.get_y());
    cby_unique_module_id_[x].resize(coordinator.get_y()); 
  }

  return;
}

/* Pre-allocate the rr_sb_unique_module_id matrix that the device requires */ 
void DeviceRRGSB::reserve_sb_unique_submodule_id(DeviceCoordinator& coordinator) { 
  const RRGSB& rr_sb = get_gsb(coordinator);
  sb_unique_submodule_id_[coordinator.get_x()][coordinator.get_y()].resize(rr_sb.get_num_sides());

  for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    sb_unique_submodule_id_[coordinator.get_x()][coordinator.get_y()][side_manager.to_size_t()].resize(segment_ids_.size());
  }

  return;
}

/* Resize rr_switch_block array is needed*/
void DeviceRRGSB::resize_upon_need(DeviceCoordinator& coordinator) { 
  if (coordinator.get_x() + 1 > rr_gsb_.size()) {
    rr_gsb_.resize(coordinator.get_x() + 1);

    sb_unique_submodule_id_.resize(coordinator.get_x() + 1);
    sb_unique_module_id_.resize(coordinator.get_x() + 1);

    cbx_unique_module_id_.resize(coordinator.get_x() + 1);
    cby_unique_module_id_.resize(coordinator.get_x() + 1);
  }

  if (coordinator.get_y() + 1 > rr_gsb_[coordinator.get_x()].size()) {
    rr_gsb_[coordinator.get_x()].resize(coordinator.get_y() + 1);
    sb_unique_submodule_id_[coordinator.get_x()].resize(coordinator.get_y() + 1);
    sb_unique_module_id_[coordinator.get_x()].resize(coordinator.get_y() + 1);

    cbx_unique_module_id_[coordinator.get_x()].resize(coordinator.get_y() + 1);
    cby_unique_module_id_[coordinator.get_x()].resize(coordinator.get_y() + 1);
  }

  return;
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::add_rr_gsb(DeviceCoordinator& coordinator, 
                             const RRGSB& rr_gsb) {
  /* Resize upon needs*/
  resize_upon_need(coordinator);

  /* Add the switch block into array */
  rr_gsb_[coordinator.get_x()][coordinator.get_y()] = rr_gsb; 

  return;
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::build_cb_unique_module(t_rr_type cb_type) {
  /* Make sure a clean start */
  clear_cb_unique_module(cb_type);

  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      bool is_unique_module = true;
      DeviceCoordinator gsb_coordinator(ix, iy);

      /* Bypass non-exist CB */
      if ( false == rr_gsb_[ix][iy].is_cb_exist(cb_type) ) {
        continue;
      }

      /* Traverse the unique_mirror list and check it is an mirror of another */
      for (size_t id = 0; id < get_num_cb_unique_module(cb_type); ++id) {
        if (true == rr_gsb_[ix][iy].is_cb_mirror(get_cb_unique_module(cb_type, id), cb_type)) {
          /* This is a mirror, raise the flag and we finish */
          is_unique_module = false;
          /* Record the id of unique mirror */
          set_cb_unique_module_id(cb_type, gsb_coordinator, id); 
          break;
        }
      }
      /* Add to list if this is a unique mirror*/
      if (true == is_unique_module) {
        add_cb_unique_module(cb_type, gsb_coordinator);
        /* Record the id of unique mirror */
        set_cb_unique_module_id(cb_type, gsb_coordinator, get_num_cb_unique_module(cb_type) - 1); 
      }
    }
  } 
  return;
}


/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::build_sb_unique_module() {
  /* Make sure a clean start */
  clear_sb_unique_module();

  /* Build the unique submodule */
  build_sb_unique_submodule();

  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      bool is_unique_module = true;
      DeviceCoordinator sb_coordinator(ix, iy);

      /* Traverse the unique_mirror list and check it is an mirror of another */
      for (size_t id = 0; id < get_num_sb_unique_module(); ++id) {
        /* Check if the two modules have the same submodules,
         * if so, these two modules are the same, indicating the sb is not unique.
         * else the sb is unique 
         */
        if (true == is_two_sb_share_same_submodules(sb_unique_module_[id], sb_coordinator)) {
          /* This is a mirror, raise the flag and we finish */
          is_unique_module = false;
          /* Record the id of unique mirror */
          sb_unique_module_id_[ix][iy] = id; 
          break;
        }
      }
      /* Add to list if this is a unique mirror*/
      if (true == is_unique_module) {
        sb_unique_module_.push_back(sb_coordinator);
        /* Record the id of unique mirror */
        sb_unique_module_id_[ix][iy] = sb_unique_module_.size() - 1; 
      }
    }
  } 
  return;
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::build_sb_unique_submodule() {
  /* Make sure a clean start */
  clear_sb_unique_submodule();

  /* Allocate the unique_side_module_ */
  sb_unique_submodule_.resize(get_max_num_sides());
  for (size_t side = 0; side < sb_unique_submodule_.size(); ++side) {
    sb_unique_submodule_[side].resize(segment_ids_.size());
  }

  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      DeviceCoordinator coordinator(ix, iy);
      const RRGSB& rr_sb = rr_gsb_[ix][iy]; 

      /* reserve the rr_sb_unique_module_id */
      reserve_sb_unique_submodule_id(coordinator);

      for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
        Side side_manager(side);
        /* Try to add it to the list */
        add_sb_unique_side_submodule(coordinator, rr_sb, side_manager.get_side());
      }
    }
  } 
  return;
}

void DeviceRRGSB::add_sb_unique_side_segment_submodule(DeviceCoordinator& coordinator, 
                                                       const RRGSB& rr_sb, 
                                                       enum e_side side, 
                                                       size_t seg_id) {
  bool is_unique_side_module = true;
  Side side_manager(side);

  /* add rotatable mirror support */
  for (size_t id = 0; id < get_num_sb_unique_submodule(side, seg_id); ++id) {
    /* Skip if these may never match as a mirror (violation in basic requirements */
    if (true == get_gsb(sb_unique_submodule_[side_manager.to_size_t()][seg_id][id]).is_sb_side_segment_mirror(rr_sb, side, segment_ids_[seg_id])) {
      /* This is a mirror, raise the flag and we finish */
      is_unique_side_module = false;
      /* Record the id of unique mirror */
      sb_unique_submodule_id_[coordinator.get_x()][coordinator.get_y()][side_manager.to_size_t()][seg_id] = id; 
      break;
    }
  }

  /* Add to list if this is a unique mirror*/
  if (true == is_unique_side_module) {
    sb_unique_submodule_[side_manager.to_size_t()][seg_id].push_back(coordinator);
    /* Record the id of unique mirror */
    sb_unique_submodule_id_[coordinator.get_x()][coordinator.get_y()][side_manager.to_size_t()][seg_id] = sb_unique_submodule_[side_manager.to_size_t()][seg_id].size() - 1; 
    /* 
    printf("Detect a rotatable mirror: SB[%lu][%lu]\n", coordinator.get_x(), coordinator.get_y());
     */
  }

  return;
}

void DeviceRRGSB::build_unique_module() {
  build_segment_ids();

  build_sb_unique_module();

  build_cb_unique_module(CHANX);
  build_cb_unique_module(CHANY);

  return;
}

/* Add a unique side module to the list:
 * Check if the connections and nodes on the specified side of the rr_sb 
 * If it is similar to any module[side][i] in the list, we build a link from the rr_sb to the unique_module
 * Otherwise, we add the module to the unique_module list 
 */
void DeviceRRGSB::add_sb_unique_side_submodule(DeviceCoordinator& coordinator, 
                                               const RRGSB& rr_sb, 
                                               enum e_side side) {
  Side side_manager(side);

  for (size_t iseg = 0; iseg < segment_ids_.size(); ++iseg) {
    add_sb_unique_side_segment_submodule(coordinator, rr_sb, side, iseg);
  }

  return;
}

void DeviceRRGSB::add_cb_unique_module(t_rr_type cb_type, const DeviceCoordinator& coordinator) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_.push_back(coordinator); 
    return;
  case CHANY:
    cby_unique_module_.push_back(coordinator); 
    return;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

void DeviceRRGSB::set_cb_unique_module_id(t_rr_type cb_type, const DeviceCoordinator& coordinator, size_t id) {
  assert (validate_cb_type(cb_type));
  size_t x = coordinator.get_x();
  size_t y = coordinator.get_y();
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_id_[x][y] = id; 
    return;
  case CHANY:
    cby_unique_module_id_[x][y] = id; 
    return;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

/* build a map of segment_ids */
void DeviceRRGSB::build_segment_ids() {

  /* Make sure a clean start */
  clear_segment_ids();

  /* go through each rr_sb, each side and find the segment_ids */
  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      RRGSB* rr_sb = &(rr_gsb_[ix][iy]);
      for (size_t side = 0 ; side < rr_sb->get_num_sides(); ++side) {
        Side side_manager(side);
        /* get a list of segment_ids in this side */
        std::vector<size_t> cur_seg_ids = rr_sb->get_chan(side_manager.get_side()).get_segment_ids();
        /* add to the segment_id_ if exist */
        for (size_t iseg = 0; iseg < cur_seg_ids.size(); ++iseg) {
          std::vector<size_t>::iterator it = std::find(segment_ids_.begin(), segment_ids_.end(), cur_seg_ids[iseg]);
          /* find if it exists in the list  */
          if (it != segment_ids_.end()) {
            /* exist: continue */
            continue;
          }
          /* does not exist, push into the vector */
          segment_ids_.push_back(cur_seg_ids[iseg]);
        }
      }
    }
  }
  return;
}  

/* clean the content */
void DeviceRRGSB::clear() { 
  clear_gsb();

  /* clean unique module lists */
  clear_cb_unique_module(CHANX);
  clear_cb_unique_module_id(CHANX);

  clear_cb_unique_module(CHANY);
  clear_cb_unique_module_id(CHANY);

  clear_sb_unique_module();
  clear_sb_unique_module_id();

  clear_sb_unique_submodule();
  clear_sb_unique_submodule_id();

  return;
}

void DeviceRRGSB::clear_gsb() {
  /* clean gsb array */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    rr_gsb_[x].clear(); 
  }
  rr_gsb_.clear();
  return;
}

void DeviceRRGSB::clear_sb_unique_module_id() {
  /* clean rr_switch_block array */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    sb_unique_module_id_[x].clear(); 
  }
  return;
}

void DeviceRRGSB::clear_cb_unique_module_id(t_rr_type cb_type) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    for (size_t x = 0; x < rr_gsb_.size(); ++x) {
      cbx_unique_module_id_[x].clear(); 
    }
    return;
  case CHANY:
    for (size_t x = 0; x < rr_gsb_.size(); ++x) {
      cby_unique_module_id_[x].clear(); 
    }
    return;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}

void DeviceRRGSB::clear_sb_unique_submodule_id() {
  /* clean rr_sb_unique_side_module_id */
  for (size_t x = 0; x < sb_unique_submodule_id_.size(); ++x) {
    for (size_t y = 0; y < sb_unique_submodule_id_[x].size(); ++y) {
      for (size_t side = 0; side < sb_unique_submodule_.size(); ++side) {
        sb_unique_submodule_id_[x][y][side].clear();
      }
      sb_unique_submodule_id_[x][y].clear();
    }
    sb_unique_submodule_id_[x].clear();
  }
  sb_unique_submodule_id_.clear();
  return;
}

/* clean the content related to unique_mirrors */
void DeviceRRGSB::clear_sb_unique_submodule() {
  /* clean unique_side_module_ */
  for (size_t side = 0; side < sb_unique_submodule_.size(); ++side) {
    for (size_t iseg = 0; iseg < segment_ids_.size(); ++iseg) {
      sb_unique_submodule_[side][iseg].clear();
    }
    sb_unique_submodule_[side].clear();
  }

  return;
} 

/* clean the content related to unique_mirrors */
void DeviceRRGSB::clear_sb_unique_module() {
  /* clean unique mirror */
  sb_unique_module_.clear();

  return;
} 

void DeviceRRGSB::clear_cb_unique_module(t_rr_type cb_type) {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_.clear(); 
    return;
  case CHANY:
    cby_unique_module_.clear(); 
    return;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  }
}


/* clean the content related to segment_ids */
void DeviceRRGSB::clear_segment_ids() {
  /* clean segment_ids_ */
  segment_ids_.clear();

  return;
} 


/* Validate if the (x,y) is the range of this device */
bool DeviceRRGSB::validate_coordinator(DeviceCoordinator& coordinator) const {
  if (coordinator.get_x() >= rr_gsb_.capacity()) {
    return false;
  }
  if (coordinator.get_y() >= rr_gsb_[coordinator.get_x()].capacity()) {
    return false;
  }
  return true;
} 

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRGSB::validate_side(enum e_side side) const {
  Side side_manager(side);
  if (side_manager.to_size_t() >= sb_unique_submodule_.size()) {
    return false;
  }
  return true;
} 

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRGSB::validate_sb_unique_module_index(size_t index) const { 
  if (index >= sb_unique_module_.size()) {
    return false;
  }
  return true;
}

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRGSB::validate_sb_unique_submodule_index(size_t index, enum e_side side, size_t seg_index) const {
  assert( validate_side(side));
  assert( validate_segment_index(seg_index));
  Side side_manager(side);

  if (index >= sb_unique_submodule_[side_manager.get_side()][seg_index].size()) {
    return false;
  }
  return true;
} 

bool DeviceRRGSB::validate_cb_unique_module_index(t_rr_type cb_type, size_t index) const {
  assert (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    if (index >= cbx_unique_module_.size()) {
      return false;
    }
    return true;
  case CHANY:
    if (index >= cbx_unique_module_.size()) {
      return false;
    }
    return true;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(File:%s, [LINE%d])Invalid type of connection block!\n", 
              __FILE__, __LINE__);
    exit(1);
  } 
}

bool DeviceRRGSB::validate_segment_index(size_t index) const {
  if (index >= segment_ids_.size()) {
    return false;
  }
  return true;
}

bool DeviceRRGSB::validate_cb_type(t_rr_type cb_type) const {
  if ( (CHANX == cb_type) || (CHANY == cb_type) ) {
    return true;
  }
  return false;
}

