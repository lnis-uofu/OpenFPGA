#ifndef MIF_OPENFPGA_FORMAT_H
#define MIF_OPENFPGA_FORMAT_H

#include <cstddef>
#include <ostream>
#include <string>

#include "mif_storage.h"

/********************************************************************
 * OpenFPGA custom MIF text format (example):
 *   # and // line comments
 *   // X <int>  // Y <int>  (optional tile coordinates)
 *   // ADDR_WIDTH <int>  // DATA_WIDTH <int>
 *   # Address Data
 *   0x2000 0x0000000a
 *   //RAM_ID <int>  (starts a new RAM block section)
 *   0x2e00 0x0000038f
 *******************************************************************/
namespace openfpga {

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os);

/* Parse one raw MIF input line (comments stripped inside). */
int parse_mif_line(const std::string& file_path, size_t line_no,
                   const std::string& raw_line, MifStorage& mif_storage,
                   bool& has_current_segment, MifSegmentId& current_segment_id,
                   size_t& total_words);

/* Validate parse results and drop a trailing empty segment if needed. */
int finalize_openfpga_mif_parse(MifStorage& mif_storage,
                                bool has_current_segment,
                                const MifSegmentId& current_segment_id,
                                size_t total_words,
                                const std::string& file_path);

} /* namespace openfpga */

#endif
