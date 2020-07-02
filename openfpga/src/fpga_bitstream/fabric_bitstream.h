/******************************************************************************
 * This file introduces a data structure to store fabric-dependent bitstream information 
 *
 * General concept
 * ---------------
 * The idea is to create a unified data structure that stores the sequence of configuration 
 * bit in the architecture bitstream database
 * as well as the information (such as address of each bit) required by a specific 
 * configuration protocol
 *
 * Cross-reference 
 * ---------------
 * By using the link between ArchBitstreamManager and FabricBitstream, 
 * we can build a sequence of configuration bits to fit different configuration protocols.
 *
 *     +----------------------+                 +-------------------+
 *     |                      |   ConfigBitId   |                   |
 *     | ArchBitstreamManager |---------------->|  FabricBitstream  |
 *     |                      |                 |                   |   
 *     +----------------------+                 +-------------------+ 
 *
 * Restrictions:
 * 1. Each block inside BitstreamManager should have only 1 parent block 
 *    and multiple child block
 * 2. Each bit inside BitstreamManager should have only 1 parent block 
 * 
 ******************************************************************************/
#ifndef FABRIC_BITSTREAM_H
#define FABRIC_BITSTREAM_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "vtr_vector.h"

#include "bitstream_manager_fwd.h"
#include "fabric_bitstream_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

class FabricBitstream {
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

  public: /* Types and ranges */
    //Lazy iterator utility forward declaration
    template<class ID>
    class lazy_id_iterator;

    typedef lazy_id_iterator<FabricBitId> fabric_bit_iterator;

    typedef vtr::Range<fabric_bit_iterator> fabric_bit_range;

  public: /* Public constructor */
    FabricBitstream();

  public: /* Public aggregators */
    /* Find all the configuration bits */
    fabric_bit_range bits() const;

  public:  /* Public Accessors */
    /* Find the configuration bit id in architecture bitstream database */
    ConfigBitId config_bit(const FabricBitId& bit_id) const;

    /* Find the address of bitstream */
    std::vector<char> bit_address(const FabricBitId& bit_id) const;
    std::vector<char> bit_bl_address(const FabricBitId& bit_id) const;
    std::vector<char> bit_wl_address(const FabricBitId& bit_id) const;

    /* Find the data-in of bitstream */
    char bit_din(const FabricBitId& bit_id) const;

  public:  /* Public Mutators */
    /* Reserve config bits */
    void reserve(const size_t& num_bits);

    /* Add a new configuration bit to the bitstream manager */
    FabricBitId add_bit(const ConfigBitId& config_bit_id);

    void set_bit_address(const FabricBitId& bit_id,
                         const std::vector<char>& address);

    void set_bit_bl_address(const FabricBitId& bit_id,
                            const std::vector<char>& address);

    void set_bit_wl_address(const FabricBitId& bit_id,
                            const std::vector<char>& address);

    void set_bit_din(const FabricBitId& bit_id,
                     const char& din);

    /* Reverse bit sequence of the fabric bitstream
     * This is required by configuration chain protocol 
     */
    void reverse();

  public:  /* Public Validators */
    char valid_bit_id(const FabricBitId& bit_id) const;

  private: /* Internal data */
    /* Unique id of a bit in the Bitstream */
    size_t num_bits_; 
    std::unordered_set<FabricBitId> invalid_bit_ids_;
    vtr::vector<FabricBitId, ConfigBitId> config_bit_ids_; 

    /* Address bits: this is designed for memory decoders
     * Here we store the binary format of the address, which can be loaded
     * to the configuration protocol directly 
     *
     * We use a 2-element array, as we may have a BL address and a WL address
     */
    vtr::vector<FabricBitId, std::array<std::vector<char>, 2>> bit_addresses_;

    /* Data input (Din) bits: this is designed for memory decoders */
    vtr::vector<FabricBitId, char> bit_dins_;
};

} /* end namespace openfpga */

#endif
