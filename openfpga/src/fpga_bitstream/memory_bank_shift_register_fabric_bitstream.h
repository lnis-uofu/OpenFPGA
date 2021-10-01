#ifndef MEMORY_BANK_SHIFT_REGISTER_FABRIC_BITSTREAM_H
#define MEMORY_BANK_SHIFT_REGISTER_FABRIC_BITSTREAM_H

#include <string>
#include <map>
#include <vector>
#include "vtr_vector.h"
#include "memory_bank_shift_register_fabric_bitstream_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * This files includes data structures that stores a downloadable format of fabric bitstream
 * which is compatible with memory bank configuration protocol using shift register to control BL/WLs
 * @note This data structure is mainly used to output bitstream file for compatible protocols
 ******************************************************************************/
class MemoryBankShiftRegisterFabricBitstream {
  public: /* Types */
    typedef vtr::vector<MemoryBankShiftRegisterFabricBitstreamWordId, MemoryBankShiftRegisterFabricBitstreamWordId>::const_iterator word_iterator;
    /* Create range */
    typedef vtr::Range<word_iterator> word_range;

  public: /* Accessors: aggregates */
    word_range words() const;

  public: /* Accessors */
    /* @brief Return the length of bitstream */
    size_t num_words() const;

    /* @brief Return the length of each word. All the word should have a uniform size */
    size_t word_size() const;

    /* @brief Return the width of each BL word, which is the number of heads through which a BL word can be loaded in parallel */
    size_t bl_width() const;

    /* @brief Return the width of each WL word, which is the number of heads through which a WL word can be loaded in parallel */
    size_t wl_width() const;

    /* @brief Return the BL vectors with a given word id*/
    std::vector<std::string> bl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const;

    /* @brief Return the WL vectors in a given word id */
    std::vector<std::string> wl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const;

    /* @brief Return the pair of BL and WL vectors in a given word id */
    std::vector<std::string> blwl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const;

  public: /* Mutators */
    /* @brief Create a new word */
    MemoryBankShiftRegisterFabricBitstreamWordId create_word();

    /* @brief Add BLs to a given word */
    void add_bl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id,
                        const std::string& bl_vec);

    /* @brief Add WLs to a given word */
    void add_wl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id,
                        const std::string& wl_vec);

  public:  /* Validators */
    bool valid_word_id(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const;

  private: /* Internal data */
    /* Organization of the bitstream
     *
     *   ============= Begin of Word 1 ==============
     *     |<--No of -->|<-- No of -->|     
     *     | BL heads   | WL heads    |
     *     010101 .. 101 101110 .. 001  ---- 
     *     ...           ...              ^
     *                                    |
     *                       max. shift register length per word
     *                                    |
     *                                    v
     *     110001 .. 111 100100 .. 110  ---- 
     *   =============   End of Word 1 ==============
     *   ============= Begin of Word 2 ==============
     *     010101 .. 101 101110 .. 001  ---- 
     *     ...           ...              ^
     *                                    |
     *                       max. shift register length per word
     *                                    |
     *                                    v
     *     110001 .. 111 100100 .. 110  ---- 
     *   =============   End of Word 2 ==============
     *    .... more words
     */
    vtr::vector<MemoryBankShiftRegisterFabricBitstreamWordId, MemoryBankShiftRegisterFabricBitstreamWordId> bitstream_word_ids_; 
    vtr::vector<MemoryBankShiftRegisterFabricBitstreamWordId, std::vector<std::string>> bitstream_word_bls_; 
    vtr::vector<MemoryBankShiftRegisterFabricBitstreamWordId, std::vector<std::string>> bitstream_word_wls_; 
};

} /* end namespace openfpga */

#endif
