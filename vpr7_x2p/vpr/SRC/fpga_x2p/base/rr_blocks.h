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
#include "device_port.h"
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
    RRChan(const RRChan&); /* Copy Constructor */
    RRChan();
  public: /* Accessors */
    t_rr_type get_type() const;
    size_t get_chan_width() const; /* get the number of tracks in this channel */
    int get_node_track_id(t_rr_node* node) const; /* get the track_id of a node */
    t_rr_node* get_node(size_t track_num) const; /* get the rr_node with the track_id */
    int get_node_segment(t_rr_node* node) const;
    int get_node_segment(size_t track_num) const;
    bool is_mirror(const RRChan& cand) const; /* evaluate if two RR_chan is mirror to each other */
    std::vector<size_t> get_segment_ids() const; /* Get a list of segments used in this routing channel */
  public: /* Mutators */
    void set(const RRChan&); /* copy */
    void set_type(t_rr_type type); /* modify type */
    void reserve_node(size_t node_size); /* reseve a number of nodes to the array */
    void add_node(t_rr_node* node, size_t node_segment); /* add a node to the array */
    void rotate(size_t rotate_begin, size_t rotate_end, size_t offset); /* rotate the nodes and node_segments with a given offset */
    void rotate_by_node_direction(enum e_direction node_direction, size_t offset);
    void counter_rotate_by_node_direction(enum e_direction node_direction, size_t offset);
    void rotate(size_t offset); /* rotate the nodes and node_segments with a given offset */
    void mirror_node_direction(); /* mirror node direction */
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

/* Object Generic Switch Block 
 * This block contains
 * 1. A switch block
 * 2. A X-direction Connection block locates at the left side of the switch block
 * 2. A Y-direction Connection block locates at the top side of the switch block
 * This is a collection of rr_nodes, which may be replaced with RRNodeId in new RRGraph 
 *
 *                               +---------------------------------+
 *                               |          Y-direction CB         |
 *                               |              [x][y + 1]         |
 *                               +---------------------------------+
 *                  
 *                                          TOP SIDE
 *  +-------------+              +---------------------------------+
 *  |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODES           OPIN_NODES |
 *  | X-direction |              |                                 |
 *  |      CB     |  LEFT SIDE   |        Switch Block             |  RIGHT SIDE
 *  |   [x][y]    |              |              [x][y]             |
 *  |             |              |                                 |
 *  |             |              | CHAN_NODES           CHAN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODES           OPIN_NODES |
 *  |             |              |                                 |
 *  |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *  +-------------+              +---------------------------------+
 *                                             BOTTOM SIDE
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
class RRGSB {
  public: /* Contructors */
    RRGSB(const RRGSB&);/* Copy constructor */
    RRGSB();/* Default constructor */
  public: /* Accessors */
    size_t get_num_sides() const; /* Get the number of sides of this SB */
    size_t get_chan_width(enum e_side side) const; /* Get the number of routing tracks on a side */
    size_t get_cb_chan_width(t_rr_type cb_type) const; /* Get the number of routing tracks of a X/Y-direction CB */
    std::vector<enum e_side> get_cb_ipin_sides(t_rr_type cb_type) const; /* Get the sides of CB ipins in the array */
    size_t get_max_chan_width() const; /* Get the maximum number of routing tracks on all sides */
    enum PORTS get_chan_node_direction(enum e_side side, size_t track_id) const; /* Get the direction of a rr_node at a given side and track_id */
    RRChan get_chan(enum e_side side) const; /* get a rr_node at a given side and track_id */
    t_rr_node* get_chan_node(enum e_side side, size_t track_id) const; /* get a rr_node at a given side and track_id */
    size_t get_chan_node_segment(enum e_side side, size_t track_id) const; /* get the segment id of a channel rr_node */
    size_t get_num_ipin_nodes(enum e_side side) const; /* Get the number of IPIN rr_nodes on a side */
    t_rr_node* get_ipin_node(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_ipin_node_grid_side(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_ipin_node_grid_side(t_rr_node* ipin_node) const; /* get a rr_node at a given side and track_id */
    size_t get_num_opin_nodes(enum e_side side) const; /* Get the number of OPIN rr_nodes on a side */
    t_rr_node* get_opin_node(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_opin_node_grid_side(enum e_side side, size_t node_id) const; /* get a rr_node at a given side and track_id */
    enum e_side get_opin_node_grid_side(t_rr_node* opin_node) const; /* get a rr_node at a given side and track_id */
    int get_cb_chan_node_index(t_rr_type cb_type, t_rr_node* node) const;
    int get_chan_node_index(enum e_side node_side, t_rr_node* node) const;
    int get_node_index(t_rr_node* node, enum e_side node_side, enum PORTS node_direction) const; /* Get the node index in the array, return -1 if not found */
    void get_node_side_and_index(t_rr_node* node,  enum PORTS node_direction, enum e_side* node_side, int* node_index) const; /* Given a rr_node, try to find its side and index in the Switch block */
    bool is_sb_node_exist_opposite_side(t_rr_node* node, enum e_side node_side) const; /* Check if the node exist in the opposite side of this Switch Block */
  public: /* Accessors: get information about configuration ports */
    size_t get_sb_num_reserved_conf_bits() const;
    size_t get_sb_reserved_conf_bits_lsb() const;
    size_t get_sb_reserved_conf_bits_msb() const;
    size_t get_sb_num_conf_bits() const;
    size_t get_sb_conf_bits_lsb() const;
    size_t get_sb_conf_bits_msb() const;
    size_t get_cb_num_reserved_conf_bits(t_rr_type cb_type) const;
    size_t get_cb_reserved_conf_bits_lsb(t_rr_type cb_type) const;
    size_t get_cb_reserved_conf_bits_msb(t_rr_type cb_type) const;
    size_t get_cb_num_conf_bits(t_rr_type cb_type) const;
    size_t get_cb_conf_bits_lsb(t_rr_type cb_type) const;
    size_t get_cb_conf_bits_msb(t_rr_type cb_type) const;
    bool is_sb_node_imply_short_connection(t_rr_node* src_node) const; /* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
    bool is_sb_side_mirror(const RRGSB& cand, enum e_side side) const; /* check if a side of candidate SB is a mirror of the current one */
    bool is_sb_side_segment_mirror(const RRGSB& cand, enum e_side side, size_t seg_id) const; /* check if all the routing segments of a side of candidate SB is a mirror of the current one */
    bool is_sb_mirror(const RRGSB& cand) const; /* check if the candidate SB is a mirror of the current one */
    bool is_sb_mirrorable(const RRGSB& cand) const; /* check if the candidate SB satisfy the basic requirements on being a mirror of the current one */
    bool is_cb_mirror(const RRGSB& cand, t_rr_type cb_type) const; /* check if the candidate SB is a mirror of the current one */
    bool is_cb_exist(t_rr_type cb_type) const; /* check if the candidate SB is a mirror of the current one */
    size_t get_hint_rotate_offset(const RRGSB& cand) const; /* Determine an initial offset in rotating the candidate Switch Block to find a mirror matching*/
  public: /* Cooridinator conversion and output  */
    size_t get_sb_x() const; /* get the x coordinator of this switch block */
    size_t get_sb_y() const; /* get the y coordinator of this switch block */
    DeviceCoordinator get_sb_coordinator() const; /* Get the coordinator of the SB */
    size_t get_cb_x(t_rr_type cb_type) const; /* get the x coordinator of this X/Y-direction block */
    size_t get_cb_y(t_rr_type cb_type) const; /* get the y coordinator of this X/Y-direction block */
    DeviceCoordinator get_cb_coordinator(t_rr_type cb_type) const; /* Get the coordinator of the X/Y-direction CB */
    enum e_side get_cb_chan_side(t_rr_type cb_type) const; /* get the side of a Connection block */
    DeviceCoordinator get_side_block_coordinator(enum e_side side) const;
  public: /* Verilog writer */
    const char* gen_sb_verilog_module_name() const;
    const char* gen_sb_verilog_instance_name() const;
    const char* gen_sb_verilog_side_module_name(enum e_side side, size_t seg_id) const;
    const char* gen_sb_verilog_side_instance_name(enum e_side side, size_t seg_id) const;
    const char* gen_cb_verilog_module_name(t_rr_type cb_type) const;
    const char* gen_cb_verilog_instance_name(t_rr_type cb_type) const;
    const char* gen_cb_verilog_routing_track_name(t_rr_type cb_type, size_t track_id) const;
  public: /* Mutators */
    void set(const RRGSB& src); /* get a copy from a source */
    void set_coordinator(size_t x, size_t y);
    void init_num_sides(size_t num_sides); /* Allocate the vectors with the given number of sides */
    void add_chan_node(enum e_side node_side, RRChan& rr_chan, std::vector<enum PORTS> rr_chan_dir); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void add_ipin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void add_opin_node(t_rr_node* node, enum e_side node_side, enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void set_sb_num_reserved_conf_bits(size_t num_reserved_conf_bits);
    void set_sb_conf_bits_lsb(size_t conf_bits_lsb);
    void set_sb_conf_bits_msb(size_t conf_bits_msb);
    void set_cb_num_reserved_conf_bits(t_rr_type cb_type, size_t num_reserved_conf_bits);
    void set_cb_conf_bits_lsb(t_rr_type cb_type, size_t conf_bits_lsb);
    void set_cb_conf_bits_msb(t_rr_type cb_type, size_t conf_bits_msb);
    void rotate_side_chan_node_by_direction(enum e_side side, enum e_direction chan_dir, size_t offset); /* rotate all the channel nodes by a given offset */
    void counter_rotate_side_chan_node_by_direction(enum e_side side, enum e_direction chan_dir, size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_side_chan_node(enum e_side side, size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_chan_node(size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_chan_node_in_group(size_t offset); /* rotate all the channel nodes by a given offset */
    void rotate_side_opin_node_in_group(enum e_side side, size_t offset); /* rotate all the opin nodes by a given offset */
    void rotate_opin_node_in_group(size_t offset); /* rotate all the opin nodes by a given offset */
    void rotate_side(enum e_side side, size_t offset); /* rotate all the channel and opin nodes by a given offset */
    void rotate(size_t offset); /* rotate all the channel and opin nodes by a given offset */
    void swap_chan_node(enum e_side src_side, enum e_side des_side); /* swap the chan rr_nodes on two sides */
    void swap_opin_node(enum e_side src_side, enum e_side des_side); /* swap the OPIN rr_nodes on two sides */
    void swap_ipin_node(enum e_side src_side, enum e_side des_side); /* swap the IPIN rr_nodes on two sides */
    void reverse_opin_node(enum e_side side); /* reverse the OPIN rr_nodes on two sides */
    void reverse_ipin_node(enum e_side side); /* reverse the IPIN rr_nodes on two sides */
  public: /* Mutators: cleaners */
    void clear();
    void clear_chan_nodes(enum e_side node_side); /* Clean the chan_width of a side */
    void clear_ipin_nodes(enum e_side node_side); /* Clean the number of IPINs of a side */
    void clear_opin_nodes(enum e_side node_side); /* Clean the number of OPINs of a side */
    void clear_one_side(enum e_side node_side); /* Clean chan/opin/ipin nodes at one side */
  private: /* Internal Mutators */
    void mirror_side_chan_node_direction(enum e_side side); /* Mirror the node direction and port direction of routing track nodes on a side */
  private: /* internal functions */
    bool is_sb_node_mirror (const RRGSB& cand, enum e_side node_side, size_t track_id) const; 
    bool is_cb_node_mirror (const RRGSB& cand, t_rr_type cb_type, enum e_side node_side, size_t node_id) const; 
    size_t get_track_id_first_short_connection(enum e_side node_side) const; 
    bool validate_num_sides() const;
    bool validate_side(enum e_side side) const;
    bool validate_track_id(enum e_side side, size_t track_id) const;
    bool validate_opin_node_id(enum e_side side, size_t node_id) const;
    bool validate_ipin_node_id(enum e_side side, size_t node_id) const;
    bool validate_cb_type(t_rr_type cb_type) const;
  private: /* Internal Data */
    /* Coordinator */
    DeviceCoordinator coordinator_;
    /* Routing channel data */
    std::vector<RRChan>  chan_node_;
    std::vector< std::vector<enum PORTS> >  chan_node_direction_; 

    /* Logic Block Inputs data */
    std::vector< std::vector<t_rr_node*> >  ipin_node_;
    std::vector< std::vector<enum e_side> > ipin_node_grid_side_;

    /* Logic Block Outputs data */
    std::vector< std::vector<t_rr_node*> >  opin_node_;
    std::vector< std::vector<enum e_side> > opin_node_grid_side_;
    
    /* Configuration bits */
    ConfPorts  sb_conf_port_;
    ConfPorts cbx_conf_port_;
    ConfPorts cby_conf_port_;
};

/* Object Device Routing Resource Switch Block 
 * This includes:
 * 1. a collection of RRSwitch blocks, each of which can be used to instance Switch blocks in the top-level netlists
 * 2. a collection of unique mirrors of RRGSBs, which can be used to output Verilog / SPICE modules
 * 3. a colleciton of unique rotatable of RRGSBs, which can be used to output Verilog / SPICE modules
 *    The rotatable RRGSBs are more generic mirrors, which allow SwitchBlocks to be wired by rotating the pins, 
 *    further reduce the number of Verilog/SPICE modules outputted. This will lead to rapid layout generation  
 */
class DeviceRRGSB {
  public: /* Contructors */
  public: /* Accessors */
    DeviceCoordinator get_gsb_range() const; /* get the max coordinator of the switch block array */
    RRGSB get_gsb(DeviceCoordinator& coordinator) const; /* Get a rr switch block in the array with a coordinator */
    RRGSB get_gsb(size_t x, size_t y) const; /* Get a rr switch block in the array with a coordinator */
    size_t get_num_sb_unique_submodule(enum e_side side, size_t seg_index) const; /* get the number of unique mirrors of switch blocks */
    size_t get_num_sb_unique_module() const; /* get the number of unique mirrors of switch blocks */
    size_t get_num_cb_unique_module(t_rr_type cb_type) const; /* get the number of unique mirrors of CBs */
    size_t get_sb_unique_submodule_id(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const;
    RRGSB get_sb_unique_submodule(size_t index, enum e_side side, size_t seg_id) const; /* Get a rr switch block which a unique mirror */ 
    RRGSB get_sb_unique_submodule(DeviceCoordinator& coordinator, enum e_side side, size_t seg_id) const; /* Get a rr switch block which a unique mirror */ 
    RRGSB get_sb_unique_module(size_t index) const; /* Get a rr switch block which a unique mirror */ 
    RRGSB get_sb_unique_module(DeviceCoordinator& coordinator) const; /* Get a rr switch block which a unique mirror */ 
    RRGSB get_cb_unique_module(t_rr_type cb_type, size_t index) const; /* Get a rr switch block which a unique mirror */ 
    RRGSB get_cb_unique_module(t_rr_type cb_type, DeviceCoordinator& coordinator) const;
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
    void resize_upon_need(DeviceCoordinator& coordinator); /* Resize the rr_switch_block array if needed */ 
    void add_rr_gsb(DeviceCoordinator& coordinator, RRGSB& rr_gsb); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
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
    void clear_segment_ids();
  private: /* Validators */
    bool validate_coordinator(DeviceCoordinator& coordinator) const; /* Validate if the (x,y) is the range of this device */
    bool validate_side(enum e_side side) const; /* validate if side is in the range of unique_side_module_ */
    bool validate_sb_unique_module_index(size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
    bool validate_cb_unique_module_index(t_rr_type cb_type, size_t index) const; /* Validate if the index in the range of unique_mirror vector*/
    bool validate_sb_unique_submodule_index(size_t index, enum e_side side, size_t seg_index) const; /* Validate if the index in the range of unique_module vector */
    bool validate_segment_index(size_t index) const;
    bool validate_cb_type(t_rr_type cb_type) const;
  private: /* Internal builders */
    void build_segment_ids(); /* build a map of segment_ids */
    void add_sb_unique_side_submodule(DeviceCoordinator& coordinator, RRGSB& rr_sb, enum e_side side);
    void add_sb_unique_side_segment_submodule(DeviceCoordinator& coordinator, RRGSB& rr_sb, enum e_side side, size_t seg_id);
    void add_cb_unique_module(t_rr_type cb_type, const DeviceCoordinator& coordinator);
    void set_cb_unique_module_id(t_rr_type, const DeviceCoordinator& coordinator, size_t id);
    void build_sb_unique_submodule(); /* Add a switch block to the array, which will automatically identify and update the lists of unique side module */
    void build_sb_unique_module(); /* Add a switch block to the array, which will automatically identify and update the lists of unique mirrors and rotatable mirrors */
    void build_cb_unique_module(t_rr_type cb_type); /* Add a switch block to the array, which will automatically identify and update the lists of unique side module */
  private: /* Internal Data */
    std::vector< std::vector<RRGSB> > rr_gsb_;

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

#endif

