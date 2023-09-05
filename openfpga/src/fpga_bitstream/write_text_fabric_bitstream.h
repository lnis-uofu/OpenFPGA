#ifndef WRITE_TEXT_FABRIC_BITSTREAM_H
#define WRITE_TEXT_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_manager.h"
#include "config_protocol.h"
#include "fabric_bitstream.h"
#include "fabric_global_port_info.h"
#include "memory_bank_shift_register_banks.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_bitstream_to_text_file(
  const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const ConfigProtocol& config_protocol,
  const FabricGlobalPortInfo& global_ports, const std::string& fname,
  const bool& fast_configuration, const bool& keep_dont_care_bits,
  const bool& wl_incremental_order, const bool& include_time_stamp,
  const bool& verbose);

} /* end namespace openfpga */

#endif
