#ifndef FABRIC_KEY_WRITER_H
#define FABRIC_KEY_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string.h>
#include "module_manager.h"
#include "config_protocol.h"
#include "memory_bank_shift_register_banks.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_key_to_xml_file(const ModuleManager& module_manager,
                                 const std::string& fname,
                                 const ConfigProtocol& config_protocol,
                                 const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                 const bool& verbose);

} /* end namespace openfpga */

#endif
