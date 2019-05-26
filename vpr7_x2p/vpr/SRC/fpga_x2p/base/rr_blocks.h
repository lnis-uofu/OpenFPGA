/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef RR_BLOCKS_H
#define RR_BLOCKS_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>

#include "device_coordinator.h"
#include "vpr_types.h"

/* RRChan coordinator class */

/* Object of a routing channel in a routing resource graph 
 * This is a collection of rr_nodes, which may be replaced with RRNodeId in new RRGraph 
 * Each routing channel is categorized in terms of directionality, 
 * being either X-direction or Y-direction
 *   -------------   ------
 *  |             | |      |
 *  |             | |  Y   |
 *  |     CLB     | | Chan |
 *  |             | |      |
 *  |             | |      |
 *   -------------   ------
 *   -------------
 *  |      X      |
 *  |    Channel  |
 *   -------------
 */
class RRChan {
  public: /* Constructors */
    RRChan();
  public: /* Accessors */
    t_rr_type get_type() const;
    size_t get_chan_width() const; /* get the number of tracks in this channel */
    int get_node_track_id(t_rr_node* node) const; /* get the track_id of a node */
    t_rr_node* get_node(size_t track_num) const; /* get the rr_node with the track_id */
    int get_node_segment(t_rr_node* node) const;
    int get_node_segment(size_t track_num) const;
    bool is_mirror(RRChan& cand) const; /* evaluate if two RR_chan is mirror to each other */
  public: /* Mutators */
    void set_type(t_rr_type type); /* modify type */
    void reserve_node(size_t node_size); /* reseve a number of nodes to the array */
    void add_node(t_rr_node* node, size_t node_segment); /* add a node to the array */
    void rotate(size_t rotate_begin, size_t rotate_end, size_t offset); /* rotate the nodes and node_segments with a given offset */
    void rotate(size_t offset); /* rotate the nodes and node_segments with a given offset */
    void clear(); /* clear the content */
  private: /* internal functions */
    bool valid_type(t_rr_type type) const;  
    bool valid_node_type(t_rr_node* node) const;  
    bool valid_node_id(size_t node_id) const;  
  private: /* Internal Data */
    t_rr_type type_; /* channel type: CHANX or CHANY */
    std::vector<t_rr_node*> nodes_; /* rr nodes of each track in the channel */
    std::vector<size_t> node_segments_; /* segment of each track */
};

/* Object including all the RR channels in a device,
 * 1. the RR channels will be in an 2D array
 * 2. Unique Module Name for each channel 
 * 3. Instance name for each channel 
 * 4. Detailed internal structure of each channel
 *    Considering RR channels may share the same structure
 *    To be memory efficient, we build a list of unique structures
 *    and link each RR channel to
 */
class DeviceRRChan {
  public: /* contructor */
  public: /* Accessors */
    RRChan get_module(t_rr_type chan_type, size_t module_id) const;
    RRChan get_module_with_coordinator(t_rr_type chan_type, size_t x, size_t y) const;
    size_t get_num_modules(t_rr_type chan_type) const;
    size_t get_module_id(t_rr_type chan_type, size_t x, size_t y) const;
  public: /* Mutators */
    void init_module_ids(size_t device_height, size_t device_width);
    void add_one_chan_module(t_rr_type chan_type, size_t x, size_t y, RRChan& rr_chan); /* Add a new unique module of RRChan*/
    void clear();
  private: /* internal functions */
    void clear_chan(t_rr_type chan_type);
    void init_chan_module_ids(t_rr_type chan_type, size_t device_width, size_t device_height);
    bool valid_chan_type(t_rr_type chan_type) const;  
    bool valid_coordinator(t_rr_type chan_type, size_t x, size_t y) const;  
    bool valid_module_id(t_rr_type chan_type, size_t module_id) const;  
  private: /* Internal Data */
    std::vector< std::vector<size_t> > chanx_module_ids_; /* Module id in modules_ for each X-direction rr_channel */ 
    std::vector< std::vector<size_t> > chany_module_ids_; /* Module id in modules_ for each Y-direction rr_channel */ 
    std::vector<RRChan> chanx_modules_; /* Detailed internal structure of each unique module */
    std::vector<RRChan> chany_modules_; /* Detailed internal structure of each unique module */
};

/* Object Switch Block 
 * This is a collection of rr_nodes, which may be replaced with RRNodeId in new RRGraph 
 *                        TOP SIDE
 *              ---------------------------------
 *             | OPIN_NODE CHAN_NODES OPIN_NODES |
 *             |                                 |
 *             | OPIN_NODES           OPIN_NODES |
 *             |                                 |
 * LEFT SIDE   |        Switch Block             |  RIGHT SIDE
 *             |                                 |
 *             | CHAN_NODES           CHAN_NODES |
 *             |                                 |
 *             | OPIN_NODES           OPIN_NODES |
 *             |                                 |
 *             | OPIN_NODE CHAN_NODES OPIN_NODES |
 *              ---------------------------------
 *                       BOTTOM SIDE
 * num_sides: number of sides of this switch block
 * chan_rr_node: a collection of rr_nodes as routing tracks locating at each side of the Switch block <0..num_sides-1><0..chan_width-1>
 * chan_rr_node_direction: Indicate if this rr_node is an input or an output of the Switch block <0..num_sides-1><0..chan_width-1>
 * ipin_rr_node: a collection of rr_nodes as IPIN of a GRID locating at each side of the Switch block <0..num_sides-1><0..num_ipin_rr_nodes-1>
 * ipin_rr_node_grid_side: specify the side of the input pins on which side of a GRID  <0..num_sides-1><0..num_ipin_rr_nodes-1>
 * opin_rr_node: a collection of rr_nodes as OPIN of a GRID locating at each side of the Switch block <0..num_sides-1><0..num_opin_rr_nodes-1>
 * opin_rr_node_grid_side: specify the side of the output pins on which side of a GRID  <0..num_sides-1><0..num_opin_rr_nodes-1>
 * num_reserved_conf_bits: number of reserved configuration bits this switch block requires (mainly due to RRAM-based multiplexers)
 * num_conf_bits: number of configuration bits this switch block requires
 */
class RRSwitchBlock {
  public: /* Contructors */
  public: /* Accessors */
    size_t get_x() const; /* get the x coordinator of this switch block */
    size_t get_y() const; /* get the y coordinator of this switch block */
    DeviceCoordinator get_coordinator() const; /* Get the number of sides of this SB */
    size_t get_num_sides() const; /* Get the number of sides of this SB */
    size_t get_chan_width(enum e_side side) const; /* Get the number of routing tracks on a side */
    size_t get_max_chan_width() const; /* Get the maximum number of routing tracks on all sides */
    enum PORTS get_chan_node_direction(enum e_side side, size_t track_id) const; /* Get the direction of a rr_node at a given side and track_id */
    t_rr_node* get_chan_node(enum e_side side, size_t track_id) const; /* get a rr_node at a given side and track_id */
    size_t get_chan_node_segment(enum e_side side, size_t track_id) const; /* get the segment id of a channel rr_node */
    size_t get_num_ipin_nodes(enum e_side side) const; /* Get the number of IPIN rr_nodes on a side */
    size_t get_num_opin_nodes(enum e_side side) const; /* Get the number of OPIN rr_nodes on a side */
    t_rr_node* get_opin_node(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_opin_node_grid_side(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_opin_node_grid_side(t_rr_node* opin_node) const; /* get a rr_node at a given side and track_id */
    int get_node_index(t_rr_node* node, enum e_side node_side, enum PORTS node_direction) const; /* Get the node index in the array, return -1 if not found */
    void get_node_side_and_index(t_rr_node* node,  enum PORTS node_direction, enum e_side* node_side, int* node_index) const; /* Given a rr_node, try to find its side and index in the Switch block */
    bool is_node_exist_opposite_side(t_rr_node* node, enum e_side node_side) const; /* Check if the node exist in the opposite side of this Switch Block */
    size_t get_num_reserved_conf_bits() const;
    size_t get_reserved_conf_bits_lsb() const;
    size_t get_reserved_conf_bits_msb() const;
    size_t get_num_conf_bits() const;
    size_t get_conf_bits_lsb() const;
    size_t get_conf_bits_msb() const;
    bool is_node_imply_short_connection(t_rr_node* src_node) const; /* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
    bool is_mirror(RRSwitchBlock& cand) const; /* check if the candidate SB is a mirror of the current one */
    bool is_mirrorable(RRSwitchBlock& cand) const; /* check if the candidate SB satisfy the basic requirements on being a mirror of the current one */
    size_t get_hint_rotate_offset(RRSwitchBlock& cand) const; /* Determine an initial offset in rotating the candidate Switch Block to find a mirror matching*/
  public: /* Cooridinator conversion */
    DeviceCoordinator get_side_block_coordinator(enum e_side side) const;
  public: /* Verilog writer */
    char* gen_verilog_module_name() const;
    char* gen_verilog_instance_name() const;
  public: /* Mutators */
    void set_coordinator(size_t x, size_t y);
    void init_num_sides(size_t num_sides); /* Allocate the vectors with the given number of sides */
    void add_chan_node(enum e_side node_side, RRChan rr_chan, std::vector<enum PORTS> rr_chan_dir); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void add_ipin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void add_opin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void set_num_reserved_conf_bits(size_t num_reserved_conf_bits);
    void set_conf_bits_lsb(size_t conf_bits_lsb);
    void set_conf_bits_msb(size_t conf_bits_msb);
    void rotate_side_chan_node(enum e_side side, size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_chan_node(size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_chan_node_in_group(size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_side_opin_node_in_group(enum e_side side, size_t offset); /* rotate all the opin nodes by a given offset */
    void rotate_opin_node_in_group(size_t offset); /* rotate all the opin nodes by a given offset */
    void rotate_side(enum e_side side, size_t offset); /* rotate all the channel and opin nodes by a given offset */
    void rotate(size_t offset); /* rotate all the channel and opin nodes by a given offset */
    void clear();
    void clear_chan_nodes(enum e_side node_side); /* Clean the chan_width of a side */
    void clear_ipin_nodes(enum e_side node_side); /* Clean the number of IPINs of a side */
    void clear_opin_nodes(enum e_side node_side); /* Clean the number of OPINs of a side */
    void clear_one_side(enum e_side node_side); /* Clean chan/opin/ipin nodes at one side */
  private: /* Internal Mutators */
  private: /* internal functions */
    bool is_node_mirror (RRSwitchBlock& cand, enum e_side node_side, size_t track_id) const; 
    size_t get_track_id_first_short_connection(enum e_side node_side) const; 
    bool validate_num_sides() const;
    bool validate_side(enum e_side side) const;
    bool validate_track_id(enum e_side side, size_t track_id) const;
    bool validate_opin_node_id(enum e_side side, size_t node_id) const;
    bool validate_num_reserved_conf_bits() const;
    bool validate_num_conf_bits() const;
  private: /* Internal Data */
    DeviceCoordinator coordinator_;
    std::vector<RRChan>  chan_node_;
    std::vector< std::vector<enum PORTS> >  chan_node_direction_; 
    std::vector< std::vector<t_rr_node*> >  ipin_node_;
    std::vector< std::vector<enum e_side> > ipin_node_grid_side_;
    std::vector< std::vector<t_rr_node*> >  opin_node_;
    std::vector< std::vector<enum e_side> > opin_node_grid_side_;
    size_t reserved_conf_bits_lsb_;
    size_t reserved_conf_bits_msb_;
    size_t conf_bits_lsb_; /* Least Significant Bit (LSB) of configuration port*/
    size_t conf_bits_msb_; /* Most  Significant Bit (MSB) of configuration port*/
};

/* Object Device Routing Resource Switch Block 
 * This includes:
 * 1. a collection of RRSwitch blocks, each of which can be used to instance Switch blocks in the top-level netlists
 * 2. a collection of unique mirrors of RRSwitchBlocks, which can be used to output Verilog / SPICE modules
 * 3. a colleciton of unique rotatable of RRSwitchBlocks, which can be used to output Verilog / SPICE modules
 *    The rotatable RRSwitchBlocks are more generic mirrors, which allow SwitchBlocks to be wired by rotating the pins, 
 *    further reduce the number of Verilog/SPICE modules outputted. This will lead to rapid layout generation  
 */
class DeviceRRSwitchBlock {
  public: /* Contructors */
  public: /* Accessors */
    DeviceCoordinator get_switch_block_range() const; /* get the max coordinator of the switch block array */
    RRSwitchBlock get_switch_block(DeviceCoordinator& coordinator) const; /* Get a rr switch block in the array with a coordinator */
    RRSwitchBlock get_switch_block(size_t x, size_t y) const; /* Get a rr switch block in the array with a coordinator */
    size_t get_num_unique_mirror() const; /* get the number of unique mirrors of switch blocks */
    size_t get_num_rotatable_mirror() const; /* get the number of rotatable mirrors of switch blocks */
    RRSwitchBlock get_unique_mirror(size_t index) const; /* Get a rr switch block which a unique mirror */ 
    RRSwitchBlock get_unique_mirror(DeviceCoordinator& coordinator) const; /* Get a rr switch block which a unique mirror */ 
    RRSwitchBlock get_rotatable_mirror(size_t index) const; /* Get a rr switch block which a unique mirror */ 
  public: /* Mutators */
    void set_rr_switch_block_num_reserved_conf_bits(DeviceCoordinator& coordinator, size_t num_reserved_conf_bits); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_rr_switch_block_conf_bits_lsb(DeviceCoordinator& coordinator, size_t conf_bits_lsb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void set_rr_switch_block_conf_bits_msb(DeviceCoordinator& coordinator, size_t conf_bits_msb); /* TODO: TOBE DEPRECATED!!! conf_bits should be initialized when creating a switch block!!! */
    void reserve(DeviceCoordinator& coordinator); /* Pre-allocate the rr_switch_block array that the device requires */ 
    void resize_upon_need(DeviceCoordinator& coordinator); /* Resize the rr_switch_block array if needed */ 
    void add_rr_switch_block(DeviceCoordinator& coordinator, RRSwitchBlock& rr_switch_block); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
    void clear(); /* clean the content */
  private: /* Validators */
    bool validate_coordinator(DeviceCoordinator& coordinator) const; /* Validate if the (x,y) is the range of this device */
    bool validate_unique_mirror_index(size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
    bool validate_rotatable_mirror_index(size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
  private: /* Internal Data */
    std::vector< std::vector<RRSwitchBlock> > rr_switch_block_;
    std::vector< std::vector<size_t> > rr_switch_block_mirror_id_; /* A map from rr_switch_block to its unique mirror */
    std::vector< std::vector<size_t> > rr_switch_block_rotatable_mirror_id_; /* A map from rr_switch_block to its unique mirror */
    std::vector<DeviceCoordinator> unique_mirror_; 
    std::vector<DeviceCoordinator> rotatable_mirror_; 
};

#endif

