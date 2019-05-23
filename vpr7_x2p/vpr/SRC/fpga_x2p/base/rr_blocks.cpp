#include <cassert>
#include <string>
#include <algorithm>

#include "rr_blocks.h"


/* Member Functions of Class RRChan */
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
bool RRChan::is_mirror(RRChan& cand) const {
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


/* Mutators */
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
  /* resize the array if needed, node is placed in the sequence of node->ptc_num */
  if (size_t(node->ptc_num + 1) > nodes_.size()) {
    nodes_.resize(node->ptc_num + 1); /* resize to the maximum */
    node_segments_.resize(node->ptc_num + 1); /* resize to the maximum */
  }
  /* fill the dedicated element in the vector */
  nodes_[node->ptc_num] = node;
  node_segments_[node->ptc_num] = node_segment;

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

/* Member Functions of Class RRSwitchBlock*/

/* Accessors */

/* get the x coordinator of this switch block */
size_t RRSwitchBlock::get_x() const {
  return coordinator_.get_x();
} 

/* get the y coordinator of this switch block */
size_t RRSwitchBlock::get_y() const { 
  return coordinator_.get_y();
}

/* Get the number of sides of this SB */
DeviceCoordinator RRSwitchBlock::get_coordinator() const {
  return coordinator_;
} 

/* Get the number of sides of this SB */
size_t RRSwitchBlock::get_num_sides() const {
  assert (validate_num_sides());
  return chan_node_direction_.size();
}

/* Get the number of routing tracks on a side */
size_t RRSwitchBlock::get_chan_width(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return chan_node_[side_manager.to_size_t()].size(); 
}

/* Get the direction of a rr_node at a given side and track_id */
enum PORTS RRSwitchBlock::get_chan_node_direction(enum e_side side, size_t track_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_track_id(side, track_id) );
  
  return chan_node_direction_[side_manager.to_size_t()][track_id]; 
}

/* get a rr_node at a given side and track_id */
t_rr_node* RRSwitchBlock::get_chan_node(enum e_side side, size_t track_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_track_id(side, track_id) );
  
  return chan_node_[side_manager.to_size_t()][track_id]; 
} 

/* Get the number of IPIN rr_nodes on a side */
size_t RRSwitchBlock::get_num_ipin_nodes(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return ipin_node_[side_manager.to_size_t()].size(); 
} 

/* Get the number of OPIN rr_nodes on a side */
size_t RRSwitchBlock::get_num_opin_nodes(enum e_side side) const {
  Side side_manager(side);
  assert(side_manager.validate());
  return opin_node_[side_manager.to_size_t()].size(); 
} 

/* get a opin_node at a given side and track_id */
t_rr_node* RRSwitchBlock::get_opin_node(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_opin_node_id(side, node_id) );
  
  return opin_node_[side_manager.to_size_t()][node_id]; 
} 

/* get the grid_side of a opin_node at a given side and track_id */
enum e_side RRSwitchBlock::get_opin_node_grid_side(enum e_side side, size_t node_id) const {
  Side side_manager(side);
  assert(side_manager.validate());
 
  /* Ensure the side is valid in the context of this switch block */ 
  assert( validate_side(side) );

  /* Ensure the track is valid in the context of this switch block at a specific side */ 
  assert( validate_opin_node_id(side, node_id) );
  
  return opin_node_grid_side_[side_manager.to_size_t()][node_id]; 
}

/* get the grid side of a opin_rr_node */
enum e_side RRSwitchBlock::get_opin_node_grid_side(t_rr_node* opin_node) const {
  enum e_side side;
  int index;

  /* Find the side and index */
  get_node_side_and_index(opin_node, IN_PORT, &side, &index);
  assert(-1 != index);
  assert(validate_side(side));
  return get_opin_node_grid_side(side, index);
} 

/* Get the node index in the array, return -1 if not found */
int RRSwitchBlock::get_node_index(t_rr_node* node, 
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
    for (size_t inode = 0; inode < get_chan_width(node_side); ++inode) {
      if ((node == chan_node_[side_manager.to_size_t()][inode])
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
bool RRSwitchBlock::is_node_exist_opposite_side(t_rr_node* node, 
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
void RRSwitchBlock::get_node_side_and_index(t_rr_node* node, 
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

size_t RRSwitchBlock::get_num_reserved_conf_bits() const {
  assert (validate_num_reserved_conf_bits());
  return reserved_conf_bits_msb_ - reserved_conf_bits_lsb_ + 1;
}

size_t RRSwitchBlock::get_reserved_conf_bits_lsb() const {
  return reserved_conf_bits_lsb_;
}

size_t RRSwitchBlock::get_reserved_conf_bits_msb() const {
  return reserved_conf_bits_msb_;
}
    
size_t RRSwitchBlock::get_num_conf_bits() const {
  assert (validate_num_conf_bits());
  return conf_bits_msb_ - conf_bits_lsb_ + 1;
}

size_t RRSwitchBlock::get_conf_bits_lsb() const {
  return conf_bits_lsb_;
}

size_t RRSwitchBlock::get_conf_bits_msb() const {
  return conf_bits_msb_;
}

/* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
bool RRSwitchBlock::is_node_imply_short_connection(t_rr_node* src_node) const {

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
bool RRSwitchBlock::is_mirror(RRSwitchBlock& cand) const {
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
    for (size_t itrack = 0; itrack < get_chan_width(side_manager.get_side()); ++itrack) {
      /* Check the directionality of each node */
      if (get_chan_node_direction(side_manager.get_side(), itrack) != cand.get_chan_node_direction(side_manager.get_side(), itrack)) {
        return false;
      }
      /* Check the track_id of each node */
      if (get_chan_node(side_manager.get_side(), itrack)->ptc_num != cand.get_chan_node(side_manager.get_side(), itrack)->ptc_num) {
        return false;
      }
      /* For OUT_PORT rr_node, we need to check fan-in */
      if (OUT_PORT != get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue; /* skip IN_PORT */
      }

      if (false == is_node_mirror(cand, side_manager.get_side(), itrack)) {
        return false;
      } 
    }
  } 

  /* check the numbers of opin_rr_nodes */
  for (size_t side = 0; side < get_num_sides(); ++side) {
    Side side_manager(side);

    if (get_num_ipin_nodes(side_manager.get_side()) != cand.get_num_ipin_nodes(side_manager.get_side())) {
      return false;
    }
  }

  /* Make sure the number of conf bits are the same */
  if ( ( get_num_conf_bits() != cand.get_num_conf_bits() ) 
    || ( get_num_reserved_conf_bits() != cand.get_num_reserved_conf_bits() ) ) {
    return false;
  }

  return true;
}

/* Public Accessors: Cooridinator conversion */
DeviceCoordinator RRSwitchBlock::get_side_block_coordinator(enum e_side side) const {
  Side side_manager(side); 
  assert(side_manager.validate());
  DeviceCoordinator ret(get_x(), get_y());

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

/* Public Accessors Verilog writer */
char* RRSwitchBlock::gen_verilog_module_name() const {
  char* ret = NULL;
  std::string x_str = std::to_string(get_x());
  std::string y_str = std::to_string(get_y());
  
  ret = (char*)my_malloc(2 + 1 + x_str.length()
                         + 2 + y_str.length()
                         + 1 + 1); 

  sprintf(ret, "sb_%lu__%lu_", 
          get_x(), get_y());

  return ret;
}

char* RRSwitchBlock::gen_verilog_instance_name() const {
  char* ret = NULL;
  std::string x_str = std::to_string(get_x());
  std::string y_str = std::to_string(get_y());
  
  ret = (char*)my_malloc(2 + 1 + x_str.length()
                         + 2 + y_str.length()
                         + 4 + 1); 

  sprintf(ret, "sb_%lu__%lu__0_", 
          get_x(), get_y());

  return ret;
}


/* Public mutators */

/* Set the coordinator (x,y) for the switch block */
void RRSwitchBlock::set_coordinator(size_t x, size_t y) {
  coordinator_.set(x, y);
  return;
}

/* Allocate the vectors with the given number of sides */
void RRSwitchBlock::init_num_sides(size_t num_sides) {
  /* Initialize the vectors */
  chan_node_direction_.resize(num_sides);
  chan_node_.resize(num_sides);
  ipin_node_.resize(num_sides);
  ipin_node_grid_side_.resize(num_sides);
  opin_node_.resize(num_sides);
  opin_node_grid_side_.resize(num_sides);
  return;
}

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRSwitchBlock::add_chan_node(t_rr_node* node, enum e_side node_side, enum PORTS node_direction) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  /* resize the array if needed, node is placed in the sequence of node->ptc_num */
  if (size_t(node->ptc_num + 1) > chan_node_[side_manager.to_size_t()].size()) {
    chan_node_[side_manager.to_size_t()].resize(node->ptc_num + 1); /* resize to the maximum */
    chan_node_direction_[side_manager.to_size_t()].resize(node->ptc_num + 1); /* resize to the maximum */
  }
  /* fill the dedicated element in the vector */
  chan_node_[side_manager.to_size_t()][node->ptc_num] = node;
  chan_node_direction_[side_manager.to_size_t()][node->ptc_num] = node_direction;

  return;
} 

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRSwitchBlock::add_ipin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  /* push pack the dedicated element in the vector */
  ipin_node_[side_manager.to_size_t()].push_back(node);
  ipin_node_grid_side_[side_manager.to_size_t()].push_back(grid_side);

  return;
}

/* Add a node to the chan_node_ list and also assign its direction in chan_node_direction_ */
void RRSwitchBlock::add_opin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  /* push pack the dedicated element in the vector */
  opin_node_[side_manager.to_size_t()].push_back(node);
  opin_node_grid_side_[side_manager.to_size_t()].push_back(grid_side);

  return;
} 

void RRSwitchBlock::set_num_reserved_conf_bits(size_t num_reserved_conf_bits) {
  reserved_conf_bits_lsb_ = 0;
  reserved_conf_bits_msb_ = num_reserved_conf_bits - 1;
  return;
}

void RRSwitchBlock::set_conf_bits_lsb(size_t conf_bits_lsb) {
  conf_bits_lsb_ = conf_bits_lsb;
  return;
}

void RRSwitchBlock::set_conf_bits_msb(size_t conf_bits_msb) {
  conf_bits_msb_ = conf_bits_msb;
  return;
}

void RRSwitchBlock::clear() {
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
  reserved_conf_bits_lsb_ = 1;
  reserved_conf_bits_msb_ = 0;
  /* Just to make the lsb and msb invalidate */
  set_conf_bits_lsb(1);
  set_conf_bits_msb(0);

  return;
}

/* Clean the chan_width of a side */
void RRSwitchBlock::clear_chan_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  chan_node_[side_manager.to_size_t()].clear();
  chan_node_direction_[side_manager.to_size_t()].clear();
  return;
} 

/* Clean the number of IPINs of a side */
void RRSwitchBlock::clear_ipin_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  ipin_node_[side_manager.to_size_t()].clear();
  ipin_node_grid_side_[side_manager.to_size_t()].clear();
  return;
} 

/* Clean the number of OPINs of a side */
void RRSwitchBlock::clear_opin_nodes(enum e_side node_side) {
  Side side_manager(node_side);
  assert(validate_side(node_side));
  
  opin_node_[side_manager.to_size_t()].clear();
  opin_node_grid_side_[side_manager.to_size_t()].clear();
  return;
}

/* Clean chan/opin/ipin nodes at one side */
void RRSwitchBlock::clear_one_side(enum e_side node_side) {
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
bool RRSwitchBlock::is_node_mirror(RRSwitchBlock& cand, 
                                   enum e_side node_side, 
                                   size_t track_id) const {
  /* Ensure rr_nodes are either the output of short-connection or multiplexer  */
  t_rr_node* node = this->get_chan_node(node_side, track_id);
  t_rr_node* cand_node = cand.get_chan_node(node_side, track_id);
  bool is_short_conkt = this->is_node_imply_short_connection(node);
  if (is_short_conkt != cand.is_node_imply_short_connection(cand_node)) {
    return false;
  }
  /* Find the driving rr_node in this sb */
  if (true == is_short_conkt) {
    /* Ensure we have the same track id for the driving nodes */
    if ( this->is_node_exist_opposite_side(node, node_side)
      != cand.is_node_exist_opposite_side(cand_node, node_side)) {
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

/* Validate if the number of sides are consistent among internal data arrays ! */
bool RRSwitchBlock::validate_num_sides() const {
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
bool RRSwitchBlock::validate_side(enum e_side side) const {
  Side side_manager(side);
  if ( side_manager.to_size_t() < get_num_sides() ) {
    return true;
  }
  return false;
}

/* Check the track_id is valid for chan_node_ and chan_node_direction_ */
bool RRSwitchBlock::validate_track_id(enum e_side side, size_t track_id) const {
  Side side_manager(side);

  if (false == validate_side(side)) {
    return false;
  } 
  if ( ( track_id < chan_node_[side_manager.to_size_t()].size()) 
    && ( track_id < chan_node_direction_[side_manager.to_size_t()].size()) ) {
    return true;
  }

  return false;
}

/* Check the opin_node_id is valid for opin_node_ and opin_node_grid_side_ */
bool RRSwitchBlock::validate_opin_node_id(enum e_side side, size_t node_id) const {
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

/* Validate the number of configuration bits, MSB should be no less than the LSB !!! */
bool RRSwitchBlock::validate_num_conf_bits() const {
  if (conf_bits_msb_ >= conf_bits_lsb_) {
    return true;
  }
  return false;
}

/* Validate the number of configuration bits, MSB should be no less than the LSB !!! */
bool RRSwitchBlock::validate_num_reserved_conf_bits() const {
  if (reserved_conf_bits_msb_ >= reserved_conf_bits_lsb_) {
    return true;
  }
  return false;
}

/* Member Functions of Class RRChan */
/* Accessors */

/* get the max coordinator of the switch block array */
DeviceCoordinator DeviceRRSwitchBlock::get_switch_block_range() const {
  size_t max_y = 0;
  /* Get the largest size of sub-arrays */
  for (size_t x = 0; x < rr_switch_block_.size(); ++x) {
    max_y = std::max(max_y, rr_switch_block_[x].size());
  }
  
  DeviceCoordinator coordinator(rr_switch_block_.size(), max_y);
  return coordinator;
} 

/* Get a rr switch block in the array with a coordinator */
RRSwitchBlock DeviceRRSwitchBlock::get_switch_block(DeviceCoordinator coordinator) const {
  assert(validate_coordinator(coordinator));
  return rr_switch_block_[coordinator.get_x()][coordinator.get_y()];
} 

/* Get a rr switch block in the array with a coordinator */
RRSwitchBlock DeviceRRSwitchBlock::get_switch_block(size_t x, size_t y) const { 
  DeviceCoordinator coordinator(x, y);  
  return get_switch_block(coordinator);
}

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRSwitchBlock::get_num_unique_mirror() const {
  return unique_mirror_.size();
} 

/* get the number of rotatable mirrors of switch blocks */
size_t DeviceRRSwitchBlock::get_num_rotatable_mirror() const {
  return rotatable_mirror_.size();
} 

/* Get a rr switch block which a unique mirror */ 
RRSwitchBlock DeviceRRSwitchBlock::get_unique_mirror(size_t index) const {
  assert (validate_unique_mirror_index(index));
  
  return rr_switch_block_[unique_mirror_[index].get_x()][unique_mirror_[index].get_y()];
}

/* Get a rr switch block which a unique mirror */ 
RRSwitchBlock DeviceRRSwitchBlock::get_rotatable_mirror(size_t index) const {
  assert (validate_rotatable_mirror_index(index));
  
  return rr_switch_block_[rotatable_mirror_[index].get_x()][rotatable_mirror_[index].get_y()];
} 

/* Public Mutators */

/* Pre-allocate the rr_switch_block array that the device requires */ 
void DeviceRRSwitchBlock::reserve(DeviceCoordinator& coordinator) { 
  rr_switch_block_.reserve(coordinator.get_x());
  for (size_t x = 0; x < coordinator.get_x(); ++x) {
    rr_switch_block_[x].reserve(coordinator.get_y()); 
  }
  return;
}

/* Resize rr_switch_block array is needed*/
void DeviceRRSwitchBlock::resize_upon_need(DeviceCoordinator& coordinator) { 
  if (coordinator.get_x() + 1 > rr_switch_block_.capacity()) {
    rr_switch_block_.resize(coordinator.get_x());
  }

  if (coordinator.get_y() + 1 > rr_switch_block_[coordinator.get_x()].capacity()) {
    rr_switch_block_[coordinator.get_x()].resize(coordinator.get_y());
  }
  
  return;
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRSwitchBlock::add_rr_switch_block(DeviceCoordinator& coordinator, 
                                              RRSwitchBlock& rr_switch_block) {
  bool is_unique_mirror = true;

  /* Resize upon needs*/
  resize_upon_need(coordinator);

  /* Add the switch block into array */
  rr_switch_block_[coordinator.get_x()][coordinator.get_y()] = rr_switch_block; 

  /* Traverse the unique_mirror list and check it is an mirror of another */
  for (size_t mirror_id = 0; mirror_id < get_num_unique_mirror(); ++mirror_id) {
    if (true == get_switch_block(unique_mirror_[mirror_id]).is_mirror(rr_switch_block)) {
      /* This is a mirror, raise the flag and we finish */
      is_unique_mirror = false;
      break;
    }
  }
  /* Add to list if this is a unique mirror*/
  if (true == is_unique_mirror) {
    unique_mirror_.push_back(coordinator);
  }

  /* TODO: add rotatable mirror support */

  return;
} 

/* clean the content */
void DeviceRRSwitchBlock::clear() { 
  /* clean rr_switch_block array */
  for (size_t x = 0; x < rr_switch_block_.size(); ++x) {
    rr_switch_block_[x].clear(); 
  }
  rr_switch_block_.clear();

  /* clean unique mirror */
  unique_mirror_.clear();

  /* clean unique mirror */
  rotatable_mirror_.clear();

  return;
}

/* Validate if the (x,y) is the range of this device */
bool DeviceRRSwitchBlock::validate_coordinator(DeviceCoordinator& coordinator) const {
  if (coordinator.get_x() >= rr_switch_block_.capacity()) {
    return false;
  }
  if (coordinator.get_y() >= rr_switch_block_[coordinator.get_x()].capacity()) {
    return false;
  }
  return true;
} 

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRSwitchBlock::validate_unique_mirror_index(size_t index) const { 
  if (index >= unique_mirror_.size()) {
    return false;
  }
  return true;
}

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRSwitchBlock::validate_rotatable_mirror_index(size_t index) const {
  if (index >= rotatable_mirror_.size()) {
    return false;
  }
  return true;
} 
