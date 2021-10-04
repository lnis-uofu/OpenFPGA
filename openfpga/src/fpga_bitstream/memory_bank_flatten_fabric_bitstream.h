#ifndef MEMORY_BANK_FLATTEN_FABRIC_BITSTREAM_H
#define MEMORY_BANK_FLATTEN_FABRIC_BITSTREAM_H

#include <string>
#include <map>
#include <vector>
#include "vtr_vector.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * This files includes data structures that stores a downloadable format of fabric bitstream
 * which is compatible with memory bank configuration protocol using flatten BL/WL buses
 * @note This data structure is mainly used to output bitstream file for compatible protocols
 ******************************************************************************/
class MemoryBankFlattenFabricBitstream {
  public: /* Accessors */
    /* @brief Return the length of bitstream */
    size_t size() const;

    /* @brief Return the BL address size */
    size_t bl_vector_size() const;

    /* @brief Return the WL address size */
    size_t wl_vector_size() const;

    /* @brief Return the BL vectors with a given WL key */
    std::vector<std::string> bl_vector(const std::vector<std::string>& wl_vec) const;

    /* @brief Return all the WL vectors in a downloaded sequence */
    std::vector<std::vector<std::string>> wl_vectors() const;

  public: /* Mutators */
    /* @brief add a pair of BL/WL vectors to the bitstream database */
    void add_blwl_vectors(const std::vector<std::string>& bl_vec,
                          const std::vector<std::string>& wl_vec);
  public:  /* Validators */
  private: /* Internal data */
    /* [(wl_bank0, wl_bank1, ...)] = [(bl_bank0, bl_bank1, ...)]
     * Must use (WL, BL) as pairs in the map!!!
     * This is because BL data may not be unique while WL must be unique
     */
    std::map<std::vector<std::string>, std::vector<std::string>> bitstream_; 
};

} /* end namespace openfpga */

#endif
