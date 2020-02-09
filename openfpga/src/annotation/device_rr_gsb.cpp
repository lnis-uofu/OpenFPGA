/************************************************************************
 * Member functions for class DeviceRRGSB
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "device_rr_gsb.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/

/************************************************************************
 * Public accessors
 ***********************************************************************/
/* get the max coordinate of the switch block array */
vtr::Point<size_t> DeviceRRGSB::get_gsb_range() const {
  size_t max_y = 0;
  /* Get the largest size of sub-arrays */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    max_y = std::max(max_y, rr_gsb_[x].size());
  }
  
  vtr::Point<size_t> coordinate(rr_gsb_.size(), max_y);
  return coordinate;
} 

/* Get a rr switch block in the array with a coordinate */
const RRGSB DeviceRRGSB::get_gsb(const vtr::Point<size_t>& coordinate) const {
  VTR_ASSERT(validate_coordinate(coordinate));
  return rr_gsb_[coordinate.x()][coordinate.y()];
} 

/* Get a rr switch block in the array with a coordinate */
const RRGSB DeviceRRGSB::get_gsb(const size_t& x, const size_t& y) const { 
  vtr::Point<size_t> coordinate(x, y);  
  return get_gsb(coordinate);
}

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRGSB::get_num_cb_unique_module(const t_rr_type& cb_type) const {
  VTR_ASSERT (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return cbx_unique_module_.size();
  case CHANY:
    return cby_unique_module_.size();
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }
} 

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRGSB::get_num_sb_unique_module() const {
  return sb_unique_module_.size();
} 

/* get the number of unique mirrors of switch blocks */
size_t DeviceRRGSB::get_num_gsb_unique_module() const {
  return gsb_unique_module_.size();
} 

/* Get a rr switch block which a unique mirror */ 
const RRGSB DeviceRRGSB::get_sb_unique_module(const size_t& index) const {
  VTR_ASSERT (validate_sb_unique_module_index(index));
  
  return rr_gsb_[sb_unique_module_[index].x()][sb_unique_module_[index].y()];
}

/* Get a rr switch block which a unique mirror */ 
const RRGSB& DeviceRRGSB::get_cb_unique_module(const t_rr_type& cb_type, const size_t& index) const {
  VTR_ASSERT (validate_cb_unique_module_index(cb_type, index));
  VTR_ASSERT (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX: 
   return rr_gsb_[cbx_unique_module_[index].x()][cbx_unique_module_[index].y()];
  case CHANY:
   return rr_gsb_[cby_unique_module_[index].x()][cby_unique_module_[index].y()];
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }  
}

/* Give a coordinate of a rr switch block, and return its unique mirror */ 
const RRGSB& DeviceRRGSB::get_cb_unique_module(const t_rr_type& cb_type, const vtr::Point<size_t>& coordinate) const {
  VTR_ASSERT(validate_cb_type(cb_type));
  VTR_ASSERT(validate_coordinate(coordinate));
  size_t cb_unique_module_id;

  switch(cb_type) {
  case CHANX:
    cb_unique_module_id = cbx_unique_module_id_[coordinate.x()][coordinate.y()];  
    break;
  case CHANY:
    cb_unique_module_id = cby_unique_module_id_[coordinate.x()][coordinate.y()];  
    break;
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }  

  return get_cb_unique_module(cb_type, cb_unique_module_id);
} 

/* Give a coordinate of a rr switch block, and return its unique mirror */ 
const RRGSB DeviceRRGSB::get_sb_unique_module(const vtr::Point<size_t>& coordinate) const {
  VTR_ASSERT(validate_coordinate(coordinate));
  size_t sb_unique_module_id = sb_unique_module_id_[coordinate.x()][coordinate.y()];  
  return get_sb_unique_module(sb_unique_module_id);
} 

/************************************************************************
 * Public mutators
 ***********************************************************************/

/* Pre-allocate the rr_switch_block array that the device requires */ 
void DeviceRRGSB::reserve(const vtr::Point<size_t>& coordinate) { 
  rr_gsb_.resize(coordinate.x());

  gsb_unique_module_id_.resize(coordinate.x());

  sb_unique_module_id_.resize(coordinate.x());

  cbx_unique_module_id_.resize(coordinate.x());
  cby_unique_module_id_.resize(coordinate.x());

  for (size_t x = 0; x < coordinate.x(); ++x) {
    rr_gsb_[x].resize(coordinate.y()); 

    gsb_unique_module_id_[x].resize(coordinate.y()); 

    sb_unique_module_id_[x].resize(coordinate.y()); 

    cbx_unique_module_id_[x].resize(coordinate.y());
    cby_unique_module_id_[x].resize(coordinate.y()); 
  }
}

/* Resize rr_switch_block array is needed*/
void DeviceRRGSB::resize_upon_need(const vtr::Point<size_t>& coordinate) { 
  if (coordinate.x() + 1 > rr_gsb_.size()) {
    rr_gsb_.resize(coordinate.x() + 1);

    sb_unique_module_id_.resize(coordinate.x() + 1);

    cbx_unique_module_id_.resize(coordinate.x() + 1);
    cby_unique_module_id_.resize(coordinate.x() + 1);
  }

  if (coordinate.y() + 1 > rr_gsb_[coordinate.x()].size()) {
    rr_gsb_[coordinate.x()].resize(coordinate.y() + 1);
    sb_unique_module_id_[coordinate.x()].resize(coordinate.y() + 1);

    cbx_unique_module_id_[coordinate.x()].resize(coordinate.y() + 1);
    cby_unique_module_id_[coordinate.x()].resize(coordinate.y() + 1);
  }
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::add_rr_gsb(const vtr::Point<size_t>& coordinate, 
                             const RRGSB& rr_gsb) {
  /* Resize upon needs*/
  resize_upon_need(coordinate);

  /* Add the switch block into array */
  rr_gsb_[coordinate.x()][coordinate.y()] = rr_gsb; 
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::build_cb_unique_module(const RRGraph& rr_graph, const t_rr_type& cb_type) {
  /* Make sure a clean start */
  clear_cb_unique_module(cb_type);

  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      bool is_unique_module = true;
      vtr::Point<size_t> gsb_coordinate(ix, iy);

      /* Bypass non-exist CB */
      if ( false == rr_gsb_[ix][iy].is_cb_exist(cb_type) ) {
        continue;
      }

      /* Traverse the unique_mirror list and check it is an mirror of another */
      for (size_t id = 0; id < get_num_cb_unique_module(cb_type); ++id) {
        const RRGSB& unique_module = get_cb_unique_module(cb_type, id);
        if (true == rr_gsb_[ix][iy].is_cb_mirror(rr_graph, unique_module, cb_type)) {
          /* This is a mirror, raise the flag and we finish */
          is_unique_module = false;
          /* Record the id of unique mirror */
          set_cb_unique_module_id(cb_type, gsb_coordinate, id); 
          break;
        }
      }
      /* Add to list if this is a unique mirror*/
      if (true == is_unique_module) {
        add_cb_unique_module(cb_type, gsb_coordinate);
        /* Record the id of unique mirror */
        set_cb_unique_module_id(cb_type, gsb_coordinate, get_num_cb_unique_module(cb_type) - 1); 
      }
    }
  } 
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
void DeviceRRGSB::build_sb_unique_module(const RRGraph& rr_graph) {
  /* Make sure a clean start */
  clear_sb_unique_module();

  /* Build the unique module */
  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      bool is_unique_module = true;
      vtr::Point<size_t> sb_coordinate(ix, iy);

      /* Traverse the unique_mirror list and check it is an mirror of another */
      for (size_t id = 0; id < get_num_sb_unique_module(); ++id) {
        /* Check if the two modules have the same submodules,
         * if so, these two modules are the same, indicating the sb is not unique.
         * else the sb is unique 
         */
        const RRGSB& unique_module = get_sb_unique_module(id);
        if (true == rr_gsb_[ix][iy].is_sb_mirror(rr_graph, unique_module)) {
          /* This is a mirror, raise the flag and we finish */
          is_unique_module = false;
          /* Record the id of unique mirror */
          sb_unique_module_id_[ix][iy] = id; 
          break;
        }
      }
      /* Add to list if this is a unique mirror*/
      if (true == is_unique_module) {
        sb_unique_module_.push_back(sb_coordinate);
        /* Record the id of unique mirror */
        sb_unique_module_id_[ix][iy] = sb_unique_module_.size() - 1; 
      }
    }
  } 
}

/* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */

/* Find repeatable GSB block in the array */
void DeviceRRGSB::build_gsb_unique_module() {
  /* Make sure a clean start */
  clear_gsb_unique_module();

  for (size_t ix = 0; ix < rr_gsb_.size(); ++ix) {
    for (size_t iy = 0; iy < rr_gsb_[ix].size(); ++iy) {
      bool is_unique_module = true;
      vtr::Point<size_t> gsb_coordinate(ix, iy);

      /* Traverse the unique_mirror list and check it is an mirror of another */
      for (size_t id = 0; id < get_num_gsb_unique_module(); ++id) {
        /* We have alreay built sb and cb unique module list 
         * We just need to check if the unique module id of SBs, CBX and CBY are the same or not 
         */
        const vtr::Point<size_t>& gsb_unique_module_coordinate = gsb_unique_module_[id];
        if ((sb_unique_module_id_[ix][iy] == sb_unique_module_id_[gsb_unique_module_coordinate.x()][gsb_unique_module_coordinate.y()])
         && (cbx_unique_module_id_[ix][iy] == cbx_unique_module_id_[gsb_unique_module_coordinate.x()][gsb_unique_module_coordinate.y()])
         && (cby_unique_module_id_[ix][iy] == cby_unique_module_id_[gsb_unique_module_coordinate.x()][gsb_unique_module_coordinate.y()])) {
          /* This is a mirror, raise the flag and we finish */
          is_unique_module = false;
          /* Record the id of unique mirror */
          gsb_unique_module_id_[ix][iy] = id; 
          break;
        }
      }
      /* Add to list if this is a unique mirror*/
      if (true == is_unique_module) {
        add_gsb_unique_module(gsb_coordinate);
        /* Record the id of unique mirror */
        gsb_unique_module_id_[ix][iy] = get_num_gsb_unique_module() - 1;
      }
    }
  } 
}

void DeviceRRGSB::build_unique_module(const RRGraph& rr_graph) {
  build_sb_unique_module(rr_graph);

  build_cb_unique_module(rr_graph, CHANX);
  build_cb_unique_module(rr_graph, CHANY);

  build_gsb_unique_module();
}

void DeviceRRGSB::add_gsb_unique_module(const vtr::Point<size_t>& coordinate) {
  gsb_unique_module_.push_back(coordinate); 
}

void DeviceRRGSB::add_cb_unique_module(const t_rr_type& cb_type, const vtr::Point<size_t>& coordinate) {
  VTR_ASSERT (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_.push_back(coordinate); 
    return;
  case CHANY:
    cby_unique_module_.push_back(coordinate); 
    return;
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }
}

void DeviceRRGSB::set_cb_unique_module_id(const t_rr_type& cb_type, const vtr::Point<size_t>& coordinate, size_t id) {
  VTR_ASSERT(validate_cb_type(cb_type));
  size_t x = coordinate.x();
  size_t y = coordinate.y();
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_id_[x][y] = id; 
    return;
  case CHANY:
    cby_unique_module_id_[x][y] = id; 
    return;
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }
}

/************************************************************************
 * Public clean-up functions:
 ***********************************************************************/
/* clean the content */
void DeviceRRGSB::clear() { 
  clear_gsb();

  clear_gsb_unique_module();
  clear_gsb_unique_module_id();

  /* clean unique module lists */
  clear_cb_unique_module(CHANX);
  clear_cb_unique_module_id(CHANX);

  clear_cb_unique_module(CHANY);
  clear_cb_unique_module_id(CHANY);

  clear_sb_unique_module();
  clear_sb_unique_module_id();
}

void DeviceRRGSB::clear_gsb() {
  /* clean gsb array */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    rr_gsb_[x].clear(); 
  }
  rr_gsb_.clear();
}

void DeviceRRGSB::clear_gsb_unique_module_id() {
  /* clean rr_switch_block array */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    gsb_unique_module_id_[x].clear(); 
  }
}

void DeviceRRGSB::clear_sb_unique_module_id() {
  /* clean rr_switch_block array */
  for (size_t x = 0; x < rr_gsb_.size(); ++x) {
    sb_unique_module_id_[x].clear(); 
  }
}

void DeviceRRGSB::clear_cb_unique_module_id(const t_rr_type& cb_type) {
  VTR_ASSERT (validate_cb_type(cb_type));
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
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }
}

/* clean the content related to unique_mirrors */
void DeviceRRGSB::clear_gsb_unique_module() {
  /* clean unique mirror */
  gsb_unique_module_.clear();
} 

/* clean the content related to unique_mirrors */
void DeviceRRGSB::clear_sb_unique_module() {
  /* clean unique mirror */
  sb_unique_module_.clear();
} 

void DeviceRRGSB::clear_cb_unique_module(const t_rr_type& cb_type) {
  VTR_ASSERT (validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    cbx_unique_module_.clear(); 
    return;
  case CHANY:
    cby_unique_module_.clear(); 
    return;
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  }
}

/************************************************************************
 * Internal validators
 ***********************************************************************/
/* Validate if the (x,y) is the range of this device */
bool DeviceRRGSB::validate_coordinate(const vtr::Point<size_t>& coordinate) const {
  if (coordinate.x() >= rr_gsb_.capacity()) {
    return false;
  }
  return (coordinate.y() < rr_gsb_[coordinate.x()].capacity());
} 

/* Validate if the index in the range of unique_mirror vector*/
bool DeviceRRGSB::validate_sb_unique_module_index(const size_t& index) const { 
  return (index < sb_unique_module_.size());
}

bool DeviceRRGSB::validate_cb_unique_module_index(const t_rr_type& cb_type, const size_t& index) const {
  VTR_ASSERT(validate_cb_type(cb_type));
  switch(cb_type) {
  case CHANX:
    return (index < cbx_unique_module_.size());
  case CHANY:
    return (index < cby_unique_module_.size());
  default: 
    VTR_LOG_ERROR("Invalid type of connection block!\n");
    exit(1);
  } 

  return false;
}

bool DeviceRRGSB::validate_cb_type(const t_rr_type& cb_type) const {
  return ((CHANX == cb_type) || (CHANY == cb_type));
}

} /* End namespace openfpga*/
