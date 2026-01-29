#pragma once

#include "vtr_assert.h"
#include "vtr_log.h"
/* Headers from fabric key */
#include "atom_netlist_utils.h"
#include "command_exit_codes.h"
#include "read_blif.h"
#include "read_circuit.h"
#include "read_interchange_netlist.h"
#include "read_xml_arch_file.h"
#include "vtr_path.h"

std::vector<std::string> read_blif_clock_info(const char* arch_fname,
                                              const char* blif_fname);
