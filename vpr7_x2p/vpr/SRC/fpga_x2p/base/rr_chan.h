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

/* Object of a channel in a routing resource graph */
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
  public: /* Accessors */
    RRChan get_module(t_rr_type chan_type, size_t module_id) const;
  public: /* Mutators */
    void init_module_ids(size_t device_height, size_t device_width);
    void add_one_chan_module(t_rr_type chan_type, size_t x, size_t y, RRChan rr_chan); /* Add a new unique module of RRChan*/
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

#endif

