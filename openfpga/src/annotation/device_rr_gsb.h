#ifndef DEVICE_RR_GSB_H
#define DEVICE_RR_GSB_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
/* Header files from vtrutil library */
#include "vtr_geometry.h"

/* Header files from vpr library */
#include "rr_graph_view.h"
#include "rr_gsb.h"
#include "vpr_device_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * Object Device Routing Resource Switch Block
 * This includes:
 * 1. a collection of RRSwitch blocks, each of which can be used to instance
 *Switch blocks in the top-level netlists
 * 2. a collection of unique mirrors of RRGSBs, which can be used to output
 *Verilog / SPICE modules
 * 3. a colleciton of unique rotatable of RRGSBs, which can be used to output
 *Verilog / SPICE modules The rotatable RRGSBs are more generic mirrors, which
 *allow SwitchBlocks to be wired by rotating the pins, further reduce the number
 *of Verilog/SPICE modules outputted. This will lead to rapid layout generation
 *******************************************************************/
class DeviceRRGSB {
 public: /* Contructors */
  DeviceRRGSB(const VprDeviceAnnotation& device_annotation);

 public: /* Accessors */
  vtr::Point<size_t> get_gsb_range()
    const; /* get the max coordinate of the switch block array */
  const RRGSB& get_gsb(const vtr::Point<size_t>& coordinate)
    const; /* Get a rr switch block in the array with a coordinate */
  const RRGSB& get_gsb(const size_t& x, const size_t& y)
    const; /* Get a rr switch block in the array with a coordinate */
  /* Get a gsb using its connection block coordinate */
  const RRGSB& get_gsb_by_cb_coordinate(
    const vtr::Point<size_t>& coordinate) const;
  size_t get_num_gsb_unique_module()
    const; /* get the number of unique mirrors of GSB */

  size_t get_num_sb_unique_module()
    const; /* get the number of unique mirrors of SB */
  vtr::Point<size_t> get_sb_unique_block_coord(
    size_t id) const; /* get the coordinate of a unique switch block */
  std::vector<vtr::Point<size_t>> get_sb_unique_block_instance_coord(
    const vtr::Point<size_t>& unique_block_coord)
    const; /* get the coordinates of the instances of a unique switch block */

  vtr::Point<size_t> get_cbx_unique_block_coord(size_t id)
    const; /* get the coordinate of a unique connection block of CHANX type */
  std::vector<vtr::Point<size_t>> get_cbx_unique_block_instance_coord(
    const vtr::Point<size_t>& unique_block_coord)
    const; /* get the coordinates of the instances of a unique connection block
              of CHANX type*/
  vtr::Point<size_t> get_cby_unique_block_coord(size_t id)
    const; /* get the coordinate of a unique connection block of CHANY type */
  std::vector<vtr::Point<size_t>> get_cby_unique_block_instance_coord(
    const vtr::Point<size_t>& unique_block_coord)
    const; /* get the coordinates of the instances of a unique connection block
              of CHANY type */

  const RRGSB& get_gsb_unique_module(
    const size_t& index) const; /* Get a rr-gsb which is a unique mirror */
  const RRGSB& get_sb_unique_module(const size_t& index)
    const; /* Get a rr switch block which a unique mirror */
  const RRGSB& get_sb_unique_module(const vtr::Point<size_t>& coordinate)
    const; /* Get a rr switch block which a unique mirror */
  const RRGSB& get_cb_unique_module(const t_rr_type& cb_type,
                                    const size_t& index)
    const; /* Get a rr switch block which a unique mirror */
  const RRGSB& get_cb_unique_module(const t_rr_type& cb_type,
                                    const vtr::Point<size_t>& coordinate) const;
  size_t get_num_cb_unique_module(const t_rr_type& cb_type)
    const; /* get the number of unique mirrors of CBs */
  bool is_gsb_exist(const RRGraphView& rr_graph,
                    const vtr::Point<size_t> coord) const;
  /* Get the index of the unique Switch block module with a given GSB
   * coordinate. Note: Do NOT use sb coordinate!!! */
  size_t get_sb_unique_module_index(const vtr::Point<size_t>& coordinate) const;
  /* Get the index of the unique Connection block module with a given GSB
   * coordinate. Note: Do NOT use sb coordinate!!! */
  size_t get_cb_unique_module_index(const t_rr_type& cb_type,
                                    const vtr::Point<size_t>& coordinate) const;

 public: /* Mutators */
  bool is_compressed() const;
  void build_gsb_unique_module(); /* Add a switch block to the array, which will
                                     automatically identify and update the lists
                                     of unique mirrors and rotatable mirrors */
  void reserve(
    const vtr::Point<size_t>& coordinate); /* Pre-allocate the rr_switch_block
                                              array that the device requires */
  void reserve_unique_modules(); /* Pre-allocate the rr_sb_unique_module_id
                      matrix that the device requires */
  void resize_upon_need(
    const vtr::Point<size_t>&
      coordinate); /* Resize the rr_switch_block array if needed */
  void add_rr_gsb(
    const vtr::Point<size_t>& coordinate,
    const RRGSB& rr_gsb); /* Add a switch block to the array, which will
                             automatically identify and update the lists of
                             unique mirrors and rotatable mirrors */
  RRGSB& get_mutable_gsb(
    const vtr::Point<size_t>&
      coordinate); /* Get a rr switch block in the array with a coordinate */
  RRGSB& get_mutable_gsb(
    const size_t& x,
    const size_t& y); /* Get a rr switch block in the array with a coordinate */
  void build_unique_module(
    const RRGraphView& rr_graph); /* Add a switch block to the array, which will
                                     automatically identify and update the lists
                                     of unique mirrors and rotatable mirrors */
  void clear();                   /* clean the content */
  void preload_unique_cbx_module(
    const vtr::Point<size_t>& block_coordinate,
    const std::vector<vtr::Point<size_t>>&
      instance_coords); /* preload unique CBX blocks and their corresponding
                           instance information. This function will be called
                           when read_unique_blocks command invoked */
  void preload_unique_cby_module(
    const vtr::Point<size_t>& block_coordinate,
    const std::vector<vtr::Point<size_t>>&
      instance_coords); /* preload unique CBY blocks and their corresponding
instance information. This function will be called
when read_unique_blocks command invoked */
  void preload_unique_sb_module(const vtr::Point<size_t>& block_coordinate,
                                const std::vector<vtr::Point<size_t>>&
                                  instance_coords); /* preload unique SB blocks
                   and their corresponding instance information. This function
                   will be called when read_unique_blocks command invoked */
  void clear_unique_modules(); /* clean the content of unique blocks*/

 private:                                                /* Internal cleaners */
  void clear_gsb();                                      /* clean the content */
  void clear_cb_unique_module(const t_rr_type& cb_type); /* clean the content */
  void clear_cb_unique_module_id(
    const t_rr_type& cb_type);       /* clean the content */
  void clear_sb_unique_module();     /* clean the content */
  void clear_sb_unique_module_id();  /* clean the content */
  void clear_gsb_unique_module();    /* clean the content */
  void clear_gsb_unique_module_id(); /* clean the content */
 private:                            /* Validators */
  bool validate_coordinate(const vtr::Point<size_t>& coordinate)
    const; /* Validate if the (x,y) is the range of this device */
  bool validate_side(const e_side& side)
    const; /* validate if side is in the range of unique_side_module_ */
  bool validate_gsb_unique_module_index(const size_t& index)
    const; /* Validate if the index in the range of unique_mirror vector*/
  bool validate_sb_unique_module_index(const size_t& index)
    const; /* Validate if the index in the range of unique_mirror vector*/
  bool validate_cb_unique_module_index(const t_rr_type& cb_type,
                                       const size_t& index)
    const; /* Validate if the index in the range of unique_mirror vector*/
  bool validate_cb_type(const t_rr_type& cb_type) const;

 private: /* Internal builders */
  void add_gsb_unique_module(const vtr::Point<size_t>& coordinate);
  void add_cb_unique_module(const t_rr_type& cb_type,
                            const vtr::Point<size_t>& coordinate);
  void set_cb_unique_module_id(const t_rr_type& cb_type,
                               const vtr::Point<size_t>& coordinate, size_t id);
  void build_sb_unique_module(
    const RRGraphView& rr_graph); /* Add a switch block to the array, which will
                                     automatically identify and update the lists
                                     of unique mirrors and rotatable mirrors */
  void build_cb_unique_module(
    const RRGraphView& rr_graph,
    const t_rr_type&
      cb_type); /* Add a switch block to the array, which will automatically
                   identify and update the lists of unique side module */

 private: /* Internal Data */
  std::vector<std::vector<RRGSB>> rr_gsb_;
  bool is_compressed_ =
    false; /* True if the unique blocks have been preloaded or built */

  std::vector<std::vector<size_t>>
    gsb_unique_module_id_; /* A map from rr_gsb to its unique mirror */
  std::vector<vtr::Point<size_t>> gsb_unique_module_;

  std::vector<std::vector<size_t>>
    sb_unique_module_id_; /* A map from rr_gsb to its unique mirror */
  std::vector<vtr::Point<size_t>> sb_unique_module_;

  std::vector<std::vector<size_t>>
    cbx_unique_module_id_; /* A map from rr_gsb to its unique mirror */
  std::vector<vtr::Point<size_t>>
    cbx_unique_module_; /* For each side of connection block, we identify a list
                           of unique modules based on its connection. This is a
                           matrix [0..num_module] */

  std::vector<std::vector<size_t>>
    cby_unique_module_id_; /* A map from rr_gsb to its unique mirror */
  std::vector<vtr::Point<size_t>>
    cby_unique_module_; /* For each side of connection block, we identify a list
                           of unique modules based on its connection. This is a
                           matrix [0..num_module] */

  /* Cached data */
  const VprDeviceAnnotation& device_annotation_;
};

} /* End namespace openfpga*/

#endif
