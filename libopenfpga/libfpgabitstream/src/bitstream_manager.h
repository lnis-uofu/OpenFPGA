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
#include "vtr_vector.h"

#include "bitstream_manager_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

class BitstreamManager {
  public: /* Types and ranges */
    typedef vtr::vector<ConfigBitId, ConfigBitId>::const_iterator config_bit_iterator;
    typedef vtr::vector<ConfigBlockId, ConfigBlockId>::const_iterator config_block_iterator;

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

    /* Add an input net id to a block */
    void add_input_net_id_to_block(const ConfigBlockId& block, const std::string& input_net_id);

    /* Add an output net id to a block */
    void add_output_net_id_to_block(const ConfigBlockId& block, const std::string& output_net_id);

    /* Add share configuration bits to a configuration bit */
    void add_shared_config_bit_values(const ConfigBitId& bit, const std::vector<bool>& shared_config_bits);

  public:  /* Public Validators */
    bool valid_bit_id(const ConfigBitId& bit_id) const;

    bool valid_block_id(const ConfigBlockId& block_id) const;

    bool valid_block_path_id(const ConfigBlockId& block_id) const;

  private: /* Internal data */
    /* Unique id of a block of bits in the Bitstream */
    vtr::vector<ConfigBlockId, ConfigBlockId> block_ids_; 
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
    vtr::vector<ConfigBlockId, int> block_path_ids_; 

    /* Net ids that are mapped to inputs and outputs of this block
     * 
     * Note: 
     *   -Bitstream manager will NOT check if the id is good for bitstream builders
     *    It just store the results
     */
    vtr::vector<ConfigBlockId, std::vector<std::string>> block_input_net_ids_; 
    vtr::vector<ConfigBlockId, std::vector<std::string>> block_output_net_ids_; 

    /* Unique id of a bit in the Bitstream */
    vtr::vector<ConfigBitId, ConfigBitId> bit_ids_; 
    vtr::vector<ConfigBitId, ConfigBlockId> bit_parent_block_ids_;
    /* value of a bit in the Bitstream */
    vtr::vector<ConfigBitId, bool> bit_values_;
    /* value of a shared configuration bits in the Bitstream */
    vtr::vector<ConfigBitId, std::vector<bool>> shared_config_bit_values_;
};

} /* end namespace openfpga */

#endif
