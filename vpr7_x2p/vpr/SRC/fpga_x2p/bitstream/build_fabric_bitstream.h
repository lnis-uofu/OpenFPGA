#ifndef BUILD_FABRIC_BITSTREAM_H
#define BUILD_FABRIC_BITSTREAM_H

#include <vector>
#include "bitstream_manager.h"
#include "module_manager.h"

std::vector<ConfigBitId> build_fabric_dependent_bitstream(const BitstreamManager& bitstream_manager,
                                                          const ModuleManager& module_manager);

#endif
