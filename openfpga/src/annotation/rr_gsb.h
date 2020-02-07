#ifndef RR_GSB_H
#define RR_GSB_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include "rr_chan.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Object Generic Switch Block 
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
 *******************************************************************/
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
    std::vector<size_t> get_chan_segment_ids(enum e_side side) const; /* Get a list of segments used in this routing channel */
    std::vector<size_t> get_chan_node_ids_by_segment_ids(enum e_side side, size_t seg_id) const; /* Get a list of segments used in this routing channel */
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
    bool is_sb_node_passing_wire(const enum e_side node_side, const size_t track_id) const; /* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
    bool is_sb_side_mirror(const RRGSB& cand, enum e_side side) const; /* check if a side of candidate SB is a mirror of the current one */
    bool is_sb_side_segment_mirror(const RRGSB& cand, enum e_side side, size_t seg_id) const; /* check if all the routing segments of a side of candidate SB is a mirror of the current one */
    bool is_sb_mirror(const RRGSB& cand) const; /* check if the candidate SB is a mirror of the current one */
    bool is_sb_mirrorable(const RRGSB& cand) const; /* check if the candidate SB satisfy the basic requirements on being a mirror of the current one */
    bool is_cb_mirror(const RRGSB& cand, t_rr_type cb_type) const; /* check if the candidate SB is a mirror of the current one */
    bool is_cb_exist(t_rr_type cb_type) const; /* check if the candidate SB is a mirror of the current one */
    size_t get_hint_rotate_offset(const RRGSB& cand) const; /* Determine an initial offset in rotating the candidate Switch Block to find a mirror matching*/
  public: /* Cooridinator conversion and output  */
    size_t get_x() const; /* get the x coordinator of this switch block */
    size_t get_y() const; /* get the y coordinator of this switch block */
    size_t get_sb_x() const; /* get the x coordinator of this switch block */
    size_t get_sb_y() const; /* get the y coordinator of this switch block */
    DeviceCoordinator get_sb_coordinator() const; /* Get the coordinator of the SB */
    size_t get_cb_x(t_rr_type cb_type) const; /* get the x coordinator of this X/Y-direction block */
    size_t get_cb_y(t_rr_type cb_type) const; /* get the y coordinator of this X/Y-direction block */
    DeviceCoordinator get_cb_coordinator(t_rr_type cb_type) const; /* Get the coordinator of the X/Y-direction CB */
    enum e_side get_cb_chan_side(t_rr_type cb_type) const; /* get the side of a Connection block */
    enum e_side get_cb_chan_side(enum e_side ipin_side) const; /* get the side of a Connection block */
    DeviceCoordinator get_side_block_coordinator(enum e_side side) const;
    DeviceCoordinator get_grid_coordinator() const;
  public: /* Verilog writer */
    const char* gen_gsb_verilog_module_name() const;
    const char* gen_gsb_verilog_instance_name() const;
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
    void add_ipin_node(t_rr_node* node, const enum e_side node_side, const enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
    void add_opin_node(t_rr_node* node, const enum e_side node_side, const enum e_side grid_side); /* Add a node to the chan_rr_node_ list and also assign its direction in chan_rr_node_direction_ */
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


} /* End namespace openfpga*/

#endif
