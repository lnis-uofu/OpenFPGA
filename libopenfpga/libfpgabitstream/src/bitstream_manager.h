/******************************************************************************
 * This file introduces a data structure to store bitstream-related information 
 *
 * General concept
 * ---------------
 * The idea is to create a unified data structure that stores all the configuration bits 
 * with proper annotation to which modules in FPGA fabric it belongs to.
 *   1. It can be easily organized in fabric-dependent representation 
 *      (generate a sequence of bitstream which exactly fit the configuration protocol of FPGA fabric)
 *   2. Or it can be easily organized in fabric-independent representation (think about XML file)
 *
 * Cross-reference 
 * ---------------
 * May be used only when you want to bind the bitstream to a specific FPGA fabric!
 * If you do so, please make sure the block name is exactly same as the instance name
 * of a child module in ModuleManager!!!
 * The configurable modules/instances in module manager are arranged
 * in the sequence to fit different configuration protocol. 
 * By using the link between ModuleManager and BitstreamManager, 
 * we can build a sequence of configuration bits to fit different configuration protocols.
 *
 *     +------------------+                                 +-----------------+
 *     |                  |   block_name == instance_name   |                 |
 *     | BitstreamManager |-------------------------------->|  ModuleManager  |
 *     |                  |                                 |                 |
 *     +------------------+                                 +-----------------+ 
 *
 * Restrictions:
 * 1. Each block inside BitstreamManager should have only 1 parent block 
 *    and multiple child block
 * 2. Each bit inside BitstreamManager should have only 1 parent block 
 * 
 ******************************************************************************/
#ifndef BITSTREAM_MANAGER_H
#define BITSTREAM_MANAGER_H

#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "vtr_vector.h"

#include "bitstream_manager_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

class BitstreamManager {
  public: /* Type implementations */
    /*
     * This class (forward delcared above) is a template used to represent a lazily calculated 
     * iterator of the specified ID type. The key assumption made is that the ID space is 
     * contiguous and can be walked by incrementing the underlying ID value. To account for 
     * invalid IDs, it keeps a reference to the invalid ID set and returns ID::INVALID() for
     * ID values in the set.
     *
     * It is used to lazily create an iteration range (e.g. as returned by RRGraph::edges() RRGraph::nodes())
     * just based on the count of allocated elements (i.e. RRGraph::num_nodes_ or RRGraph::num_edges_),
     * and the set of any invalid IDs (i.e. RRGraph::invalid_node_ids_, RRGraph::invalid_edge_ids_).
     */
    template<class ID>
    class lazy_id_iterator : public std::iterator<std::bidirectional_iterator_tag, ID> {
      public:
        //Since we pass ID as a template to std::iterator we need to use an explicit 'typename'
        //to bring the value_type and iterator names into scope
        typedef typename std::iterator<std::bidirectional_iterator_tag, ID>::value_type value_type;
        typedef typename std::iterator<std::bidirectional_iterator_tag, ID>::iterator iterator;

        lazy_id_iterator(value_type init, const std::unordered_set<ID>& invalid_ids)
            : value_(init)
            , invalid_ids_(invalid_ids) {}

        //Advance to the next ID value
        iterator operator++() {
            value_ = ID(size_t(value_) + 1);
            return *this;
        }

        //Advance to the previous ID value
        iterator operator--() {
            value_ = ID(size_t(value_) - 1);
            return *this;
        }

        //Dereference the iterator
        value_type operator*() const { return (invalid_ids_.count(value_)) ? ID::INVALID() : value_; }

        friend bool operator==(const lazy_id_iterator<ID> lhs, const lazy_id_iterator<ID> rhs) { return lhs.value_ == rhs.value_; }
        friend bool operator!=(const lazy_id_iterator<ID> lhs, const lazy_id_iterator<ID> rhs) { return !(lhs == rhs); }

      private:
        value_type value_;
        const std::unordered_set<ID>& invalid_ids_;
    };

  public: /* Public constructor */
    BitstreamManager();

  public: /* Types and ranges */
    //Lazy iterator utility forward declaration
    template<class ID>
    class lazy_id_iterator;

    typedef lazy_id_iterator<ConfigBitId> config_bit_iterator;
    typedef lazy_id_iterator<ConfigBlockId> config_block_iterator;

    typedef vtr::Range<config_bit_iterator> config_bit_range;
    typedef vtr::Range<config_block_iterator> config_block_range;

  public: /* Public aggregators */
    /* Find all the configuration bits */
    config_bit_range bits() const;

    config_block_range blocks() const;

  public:  /* Public Accessors */
    /* Find the value of bitstream */
    bool bit_value(const ConfigBitId& bit_id) const;

    /* Find a name of a block */
    std::string block_name(const ConfigBlockId& block_id) const;

    /* Find the parent of a block */
    ConfigBlockId block_parent(const ConfigBlockId& block_id) const;

    /* Find the children of a block */
    std::vector<ConfigBlockId> block_children(const ConfigBlockId& block_id) const;

    /* Find all the bits that belong to a block */
    std::vector<ConfigBitId> block_bits(const ConfigBlockId& block_id) const;

    /* Find the parent block of a bit */
    ConfigBlockId bit_parent_block(const ConfigBitId& bit_id) const;

    /* Find the index of a configuration bit in its parent block */
    size_t bit_index_in_parent_block(const ConfigBitId& bit_id) const; 

    /* Find the child block in a bitstream manager with a given name */
    ConfigBlockId find_child_block(const ConfigBlockId& block_id, const std::string& child_block_name) const;

    /* Find path id of a block */
    int block_path_id(const ConfigBlockId& block_id) const;

    /* Find input net ids of a block */
    std::vector<std::string> block_input_net_ids(const ConfigBlockId& block_id) const;

    /* Find input net ids of a block */
    std::vector<std::string> block_output_net_ids(const ConfigBlockId& block_id) const;

  public:  /* Public Mutators */
    /* Add a new configuration bit to the bitstream manager */
    ConfigBitId add_bit(const bool& bit_value);

    /* Reserve memory for a number of clocks */
    void reserve_blocks(const size_t& num_blocks);

    /* Reserve memory for a number of bits */
    void reserve_bits(const size_t& num_bits);

    /* Create a new block of configuration bits */
    ConfigBlockId create_block();

    /* Add a new block of configuration bits to the bitstream manager */
    ConfigBlockId add_block(const std::string& block_name);

    /* Set a name for a block */
    void set_block_name(const ConfigBlockId& block_id,
                        const std::string& block_name);

    /* Set a block as a child block of another */
    void add_child_block(const ConfigBlockId& parent_block, const ConfigBlockId& child_block);

    /* Add a configuration bit to a block */
    void add_bit_to_block(const ConfigBlockId& block, const ConfigBitId& bit);

    /* Add a path id to a block */
    void add_path_id_to_block(const ConfigBlockId& block, const int& path_id);
 
    /* Reserve input net ids for a block */
    void reserve_block_input_net_ids(const ConfigBlockId& block, const size_t& num_input_net_ids);

    /* Add an input net id to a block */
    void add_input_net_id_to_block(const ConfigBlockId& block, const std::string& input_net_id);

    /* Reserve output net ids for a block */
    void reserve_block_output_net_ids(const ConfigBlockId& block, const size_t& num_output_net_ids);

    /* Add an output net id to a block */
    void add_output_net_id_to_block(const ConfigBlockId& block, const std::string& output_net_id);

    /* Add share configuration bits to a configuration bit */
    void add_shared_config_bit_values(const ConfigBitId& bit, const std::vector<char>& shared_config_bits);

  public:  /* Public Validators */
    bool valid_bit_id(const ConfigBitId& bit_id) const;

    bool valid_block_id(const ConfigBlockId& block_id) const;

    bool valid_block_path_id(const ConfigBlockId& block_id) const;

  private: /* Internal data */
    /* Unique id of a block of bits in the Bitstream */
    size_t num_blocks_; 
    std::unordered_set<ConfigBlockId> invalid_block_ids_;
    vtr::vector<ConfigBlockId, std::vector<ConfigBitId>> block_bit_ids_; 

    /* Back-annotation for the bits */
    /* Parent block of a bit in the Bitstream 
     * For each bit, the block name can be designed to be same as the instance name in a module
     * to reflect its position in the module tree (ModuleManager)
     * Note that the blocks here all unique, unlike ModuleManager where modules can be instanciated 
     * Therefore, this block graph can be considered as a flattened graph of ModuleGraph
     */
    vtr::vector<ConfigBlockId, std::string> block_names_; 
    vtr::vector<ConfigBlockId, ConfigBlockId> parent_block_ids_; 
    vtr::vector<ConfigBlockId, std::vector<ConfigBlockId>> child_block_ids_; 

    /* The ids of the inputs of routing multiplexer blocks which is propagated to outputs 
     * By default, it will be -2 (which is invalid)
     * A valid id starts from -1 
     * -1 indicates an unused routing multiplexer. 
     * It will be converted to a valid id by bitstream builders)
     * For used routing multiplexers, the path id will be >= 0
     *
     * Note: 
     *   -Bitstream manager will NOT check if the id is good for bitstream builders
     *    It just store the results
     */
    vtr::vector<ConfigBlockId, short> block_path_ids_; 

    /* Net ids that are mapped to inputs and outputs of this block
     * 
     * Note: 
     *   -Bitstream manager will NOT check if the id is good for bitstream builders
     *    It just store the results
     */
    vtr::vector<ConfigBlockId, std::vector<std::string>> block_input_net_ids_; 
    vtr::vector<ConfigBlockId, std::vector<std::string>> block_output_net_ids_; 

    /* Unique id of a bit in the Bitstream */
    size_t num_bits_; 
    std::unordered_set<ConfigBitId> invalid_bit_ids_; 
    vtr::vector<ConfigBitId, ConfigBlockId> bit_parent_block_ids_;
    /* value of a bit in the Bitstream */
    vtr::vector<ConfigBitId, char> bit_values_;
    /* value of a shared configuration bits in the Bitstream */
    vtr::vector<ConfigBitId, std::vector<char>> shared_config_bit_values_;
};

} /* end namespace openfpga */

#endif
