#ifndef DEVICE_RR_GSB_H
#define DEVICE_RR_GSB_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include "rr_gsb.h"

/********************************************************************
 * Object Device Routing Resource Switch Block 
 * This includes:
 * 1. a collection of RRSwitch blocks, each of which can be used to instance Switch blocks in the top-level netlists
 * 2. a collection of unique mirrors of RRGSBs, which can be used to output Verilog / SPICE modules
 * 3. a colleciton of unique rotatable of RRGSBs, which can be used to output Verilog / SPICE modules
 *    The rotatable RRGSBs are more generic mirrors, which allow SwitchBlocks to be wired by rotating the pins, 
 *    further reduce the number of Verilog/SPICE modules outputted. This will lead to rapid layout generation  
 *******************************************************************/
class DeviceRRGSB {
  public: /* Contructors */
  public: /* Accessors */
    DeviceCoordinator get_gsb_range() const; /* get the max coordinator of the switch block array */
    const RRGSB get_gsb(const DeviceCoordinator& coordinator) const; /* Get a rr switch block in the array with a coordinator */
    const RRGSB get_gsb(size_t x, size_t y) const; /* Get a rr switch block in the array with a coordinator */
    size_t get_num_gsb_unique_module() const; /* get the number of unique mirrors of GSB */
    size_t get_num_sb_unique_submodule(enum e_side side, size_t seg_index) const; /* get the number of unique mirrors of switch blocks */
    size_t get_num_sb_unique_module() const; /* get the number of unique mirrors of switch blocks */
    size_t get_num_cb_unique_module(t_rr_type cb_type) const; /* get the number of unique mirrors of CBs */
    size_t get_sb_unique_submodule_id(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const;
    const RRGSB get_sb_unique_submodule(size_t index, enum e_side side, size_t seg_id) const; /* Get a rr switch block which a unique mirror */ 
    const RRGSB get_sb_unique_submodule(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const; /* Get a rr switch block which a unique mirror */ 
    const RRGSB get_sb_unique_module(size_t index) const; /* Get a rr switch block which a unique mirror */ 
    const RRGSB get_sb_unique_module(const DeviceCoordinator& coordinator) const; /* Get a rr switch block which a unique mirror */ 
    const RRGSB& get_cb_unique_module(t_rr_type cb_type, size_t index) const; /* Get a rr switch block which a unique mirror */ 
    const RRGSB& get_cb_unique_module(t_rr_type cb_type, const DeviceCoordinator& coordinator) const;
    size_t get_max_num_sides() const; /* Get the maximum number of sides across the switch blocks */
    size_t get_num_segments() const; /* Get the size of segment_ids */
    size_t get_segment_id(size_t index) const; /* Get a segment id */
    bool is_two_sb_share_same_submodules(DeviceCoordinator& src, DeviceCoordinator& des) const;
  public: /* Mutators */ 
    void set_sb_num_reserved_conf_bits(DeviceCoordinator& coordinator, size_t num_reserved_conf_bits); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_sb_conf_bits_lsb(DeviceCoordinator& coordinator, size_t conf_bits_lsb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_sb_conf_bits_msb(DeviceCoordinator& coordinator, size_t conf_bits_msb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_cb_num_reserved_conf_bits(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t num_reserved_conf_bits); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_cb_conf_bits_lsb(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t conf_bits_lsb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_cb_conf_bits_msb(DeviceCoordinator& coordinator, t_rr_type cb_type, size_t conf_bits_msb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void reserve(DeviceCoordinator& coordinator); /* Pre-allocate the rr_switch_block array that the device requires */ 
    void reserve_sb_unique_submodule_id(DeviceCoordinator& coordinator); /* Pre-allocate the rr_sb_unique_module_id matrix that the device requires */ 
    void resize_upon_need(const DeviceCoordinator& coordinator); /* Resize the rr_switch_block array if needed */ 
    void add_rr_gsb(const DeviceCoordinator& coordinator, const RRGSB& rr_gsb); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
    void build_unique_module(); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
    void clear(); /* clean the content */
  private: /* Internal cleaners */
    void clear_gsb(); /* clean the content */
    void clear_cb_unique_module(t_rr_type cb_type); /* clean the content */
    void clear_cb_unique_module_id(t_rr_type cb_type); /* clean the content */
    void clear_sb_unique_module(); /* clean the content */
    void clear_sb_unique_module_id(); /* clean the content */
    void clear_sb_unique_submodule(); /* clean the content */
    void clear_sb_unique_submodule_id(); /* clean the content */
    void clear_gsb_unique_module(); /* clean the content */
    void clear_gsb_unique_module_id(); /* clean the content */
    void clear_segment_ids();
  private: /* Validators */
    bool validate_coordinator(const DeviceCoordinator& coordinator) const; /* Validate if the (x,y) is the range of this device */
    bool validate_coordinator_edge(DeviceCoordinator& coordinator) const; /* Validate if the (x,y) is the range of this device but takes into consideration the fact that edges are 1 off */
    bool validate_side(enum e_side side) const; /* validate if side is in the range of unique_side_module_ */
    bool validate_sb_unique_module_index(size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
    bool validate_cb_unique_module_index(t_rr_type cb_type, size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
    bool validate_sb_unique_submodule_index(size_t index, enum e_side side, size_t seg_index) const; /* Validate if the index in the range of unique_module vector */
    bool validate_segment_index(size_t index) const;
    bool validate_cb_type(t_rr_type cb_type) const;
  private: /* Internal builders */
    void build_segment_ids(); /* build a map of segment_ids */
    void add_gsb_unique_module(const DeviceCoordinator& coordinator);
    void add_sb_unique_side_submodule(DeviceCoordinator& coordinator, const RRGSB& rr_sb, enum e_side side);
    void add_sb_unique_side_segment_submodule(DeviceCoordinator& coordinator, const RRGSB& rr_sb, enum e_side side, size_t seg_id);
    void add_cb_unique_module(t_rr_type cb_type, const DeviceCoordinator& coordinator);
    void set_cb_unique_module_id(t_rr_type, const DeviceCoordinator& coordinator, size_t id);
    void build_sb_unique_submodule(); /* Add a switch block to the array, which will automatically identify and update the lists of unique side module */
    void build_sb_unique_module(); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
    void build_cb_unique_module(t_rr_type cb_type); /* Add a switch block to the array, which will automatically identify and update the lists of unique side module */
    void build_gsb_unique_module(); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
  private: /* Internal Data */
    std::vector< std::vector<RRGSB> > rr_gsb_;

    std::vector< std::vector<size_t> > gsb_unique_module_id_; /* A map from rr_gsb to its unique mirror */
    std::vector<DeviceCoordinator> gsb_unique_module_; 

    std::vector< std::vector<size_t> > sb_unique_module_id_; /* A map from rr_gsb to its unique mirror */
    std::vector<DeviceCoordinator> sb_unique_module_; 

    std::vector< std::vector< std::vector< std::vector<size_t> > > > sb_unique_submodule_id_; /* A map from rr_switch_block to its unique_side_module [0..x][0..y][0..num_sides][num_seg-1]*/
    std::vector< std::vector <std::vector<DeviceCoordinator> > > sb_unique_submodule_; /* For each side of switch block, we identify a list of unique modules based on its connection. This is a matrix [0..num_sides-1][0..num_seg-1][0..num_module], num_sides will the max number of sides of all the rr_switch_blocks */

    std::vector< std::vector<size_t> > cbx_unique_module_id_; /* A map from rr_gsb to its unique mirror */
    std::vector<DeviceCoordinator> cbx_unique_module_; /* For each side of connection block, we identify a list of unique modules based on its connection. This is a matrix [0..num_module] */

    std::vector< std::vector<size_t> > cby_unique_module_id_; /* A map from rr_gsb to its unique mirror */
    std::vector<DeviceCoordinator> cby_unique_module_; /* For each side of connection block, we identify a list of unique modules based on its connection. This is a matrix [0..num_module] */

    std::vector<size_t> segment_ids_;
};

} /* End namespace openfpga*/

#endif
