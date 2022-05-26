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
    typedef lazy_id_iterator<FabricBitRegionId> fabric_bit_region_iterator;

    typedef vtr::Range<fabric_bit_iterator> fabric_bit_range;
    typedef vtr::Range<fabric_bit_region_iterator> fabric_bit_region_range;

  public: /* Public constructor */
    FabricBitstream();

  public: /* Public aggregators */
    /* Find all the configuration bits */
    size_t num_bits() const;
    fabric_bit_range bits() const;

    /* Find all the configuration regions */
    size_t num_regions() const;
    fabric_bit_region_range regions() const;
    std::vector<FabricBitId> region_bits(const FabricBitRegionId& region_id) const;

  public:  /* Public Accessors */
    /* Find the configuration bit id in architecture bitstream database */
    ConfigBitId config_bit(const FabricBitId& bit_id) const;

    /* Find the address of bitstream */
    std::vector<char> bit_address(const FabricBitId& bit_id) const;
    std::vector<char> bit_bl_address(const FabricBitId& bit_id) const;
    std::vector<char> bit_wl_address(const FabricBitId& bit_id) const;

    /* Find the data-in of bitstream */
    char bit_din(const FabricBitId& bit_id) const;

    /* Check if address data is accessible or not*/
    bool use_address() const;
    bool use_wl_address() const;

  public:  /* Public Mutators */
    /* Reserve config bits */
    void reserve_bits(const size_t& num_bits);

    /* Add a new configuration bit to the bitstream manager */
    FabricBitId add_bit(const ConfigBitId& config_bit_id);

    void set_bit_address(const FabricBitId& bit_id,
                         const std::vector<char>& address,
                         const bool& tolerant_short_address = false);

    void set_bit_bl_address(const FabricBitId& bit_id,
                            const std::vector<char>& address,
                            const bool& tolerant_short_address = false);

    void set_bit_wl_address(const FabricBitId& bit_id,
                            const std::vector<char>& address,
                            const bool& tolerant_short_address = false);

    void set_bit_din(const FabricBitId& bit_id,
                     const char& din);

    /* Reserve regions */
    void reserve_regions(const size_t& num_regions);

    /* Add a new configuration region */
    FabricBitRegionId add_region();

    void add_bit_to_region(const FabricBitRegionId& region_id,
                           const FabricBitId& bit_id);

    /* Reserve bits by region */
    void reverse_region_bits(const FabricBitRegionId& region_id);

    /* Reverse bit sequence of the fabric bitstream
     * This is required by configuration chain protocol 
     */
    void reverse();

    /* Enable the use of address-related data 
     * When this is enabled, data allocation will be applied to these data
     * and users can access/modify the data
     * Otherwise, it will NOT be allocated and accessible.
     *
     * This function is only applicable before any bits are added
     */
    void set_use_address(const bool& enable);
    void set_address_length(const size_t& length);
    void set_bl_address_length(const size_t& length);

    /* Enable the use of WL-address related data
     * Same priniciple as the set_use_address()  
     */
    void set_use_wl_address(const bool& enable);
    void set_wl_address_length(const size_t& length);

  public:  /* Public Validators */
    bool valid_bit_id(const FabricBitId& bit_id) const;
    bool valid_region_id(const FabricBitRegionId& bit_id) const;

  private: /* Private APIs */
    size_t encode_address_1bits(const std::vector<char>& address) const;
    size_t encode_address_xbits(const std::vector<char>& address) const;
    std::vector<char> decode_address_bits(const size_t& bit1, const size_t& bitx) const;
    std::vector<char> decode_wl_address_bits(const size_t& bit1, const size_t& bitx) const;

  private: /* Internal data */
    /* Unique id of a region in the Bitstream */
    size_t num_regions_; 
    std::unordered_set<FabricBitRegionId> invalid_region_ids_;
    vtr::vector<FabricBitRegionId, std::vector<FabricBitId>> region_bit_ids_; 

    /* Unique id of a bit in the Bitstream */
    size_t num_bits_; 
    std::unordered_set<FabricBitId> invalid_bit_ids_;
    vtr::vector<FabricBitId, ConfigBitId> config_bit_ids_; 

    /* Flags to indicate if the addresses and din should be enabled */
    bool use_address_;
    bool use_wl_address_;

    size_t address_length_;
    size_t wl_address_length_;

    /* Address bits: this is designed for memory decoders
     * Here we store the encoded format of the address, and decoded to binary format which can be loaded
     * to the configuration protocol directly 
     *
     * Encoding strategy is as follows:
     * - An address bit which may contain '0', '1', 'x'. For example
     *     101x1
     * - The string can be encoded into two integer numbers:
     *   - bit-one number: which encodes the '0' and '1' bits into a number. For example,
     *       101x1 -> 10101 -> 21
     *   - bit-x number: which encodes the 'x' bits into a number. For example,
     *       101x1 -> 00010 -> 2
     *
     * TODO: There is a limitation here, when the length of address vector is more than 64,
     * A size_t number overflows (cannot represent any binary number > 64 bit).
     * Such thing can entirely happen even in a medium sized FPGA. 
     * A solution can be use multiple size_t to fit. But clearly, we should not use vector in vector, which causes large memory overhead!
     */
    vtr::vector<FabricBitId, size_t> bit_address_1bits_;
    vtr::vector<FabricBitId, size_t> bit_address_xbits_;
    vtr::vector<FabricBitId, size_t> bit_wl_address_1bits_;
    vtr::vector<FabricBitId, size_t> bit_wl_address_xbits_;

    /* Data input (Din) bits: this is designed for memory decoders */
    vtr::vector<FabricBitId, char> bit_dins_;
};

} /* end namespace openfpga */

#endif
