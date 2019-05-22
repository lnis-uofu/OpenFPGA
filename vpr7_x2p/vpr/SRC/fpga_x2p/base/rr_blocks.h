/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef RR_CHAN_H
#define RR_CHAN_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>

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
    void clear(); /* clear the content */
  private: /* internal functions */
    bool valid_type(t_rr_type type) const;  
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
    size_t get_num_sides() const; /* Get the number of sides of this SB */
    size_t get_chan_width(enum e_side side) const; /* Get the number of routing tracks on a side */
    enum PORTS get_chan_node_direction(enum e_side side, size_t track_id) const; /* Get the direction of a rr_node at a given side and track_id */
    t_rr_node* get_chan_node(enum e_side side, size_t track_id) const; /* get a rr_node at a given side and track_id */
    size_t get_num_ipin_rr_nodes(enum e_side side) const; /* Get the number of IPIN rr_nodes on a side */
    size_t get_num_opin_rr_nodes(enum e_side side) const; /* Get the number of OPIN rr_nodes on a side */
    int get_node_index(t_rr_node* node, enum e_side node_side, enum PORTS node_direction) const; /* Get the node index in the array, return -1 if not found */
    void get_node_side_and_index(t_rr_node* node,  enum PORTS node_direction, enum e_side* node_side, int* node_index) const; /* Given a rr_node, try to find its side and index in the Switch block */
    bool is_node_exist_opposite_side(t_rr_node* node, enum e_side node_side) const; /* Check if the node exist in the opposite side of this Switch Block */
    size_t get_num_reserved_conf_bits() const;
    size_t get_num_conf_bits() const;
    bool is_node_imply_short_connection(t_rr_node* src_node) const; /* Check if the node imply a short connection inside the SB, which happens to long wires across a FPGA fabric */
    bool is_mirror(RRSwitchBlock& cand) const; /* check if the candidate SB is a mirror of the current one */
  public: /* Mutators */
  private: /* internal functions */
    bool is_node_mirror (RRSwitchBlock& cand, enum e_side node_side, size_t track_id) const; 
    bool validate_num_sides() const;
    bool validate_side(enum e_side side) const;
    bool validate_track_id(enum e_side side, size_t track_id) const;
  private: /* Internal Data */
    std::vector< std::vector<enum PORTS> >  chan_rr_node_direction_; 
    std::vector< std::vector<t_rr_node*> >  chan_rr_node_;
    std::vector< std::vector<t_rr_node*> >  ipin_rr_node_;
    std::vector< std::vector<enum e_side> > ipin_rr_node_grid_side_;
    std::vector< std::vector<t_rr_node*> >  opin_rr_node_;
    std::vector< std::vector<enum e_side> > opin_rr_node_grid_side_;
    size_t num_reserved_conf_bits_;
    size_t num_conf_bits_;
};

#endif

