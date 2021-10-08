/*********************************************************************
 * This file includes functions to generate Verilog submodules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 ********************************************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "circuit_library_utils.h"
#include "mux_utils.h"

#include "openfpga_naming.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_shift_register_banks.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Generate Verilog modules for 
 * the shift register banks that are used to control BL/WLs
 ********************************************************************/
void print_verilog_submodule_shift_register_banks(const ModuleManager& module_manager,
                                                  NetlistManager& netlist_manager,
                                                  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                  const std::string& submodule_dir,
                                                  const FabricVerilogOption& options) {

  /* Plug in with the mux subckt */
  std::string verilog_fname(submodule_dir + std::string(SHIFT_REGISTER_BANKS_VERILOG_FILE_NAME));

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fname.c_str(), fp);

  /* Print out debugging information for if the file is not opened/created properly */
  VTR_LOG("Writing Verilog netlist for shift register banks '%s' ...",
          verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Shift register banks used in FPGA"); 

  /* Create the memory circuits for the multiplexer */
  for (const ModuleId& sr_module : blwl_sr_banks.bl_shift_register_bank_unique_modules()) {
    VTR_ASSERT(true == module_manager.valid_module_id(sr_module));
    /* Write the module content in Verilog format */
    write_verilog_module_to_file(fp, module_manager, sr_module, 
                                 options.explicit_port_mapping(),
                                 options.default_net_type());

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  for (const ModuleId& sr_module : blwl_sr_banks.wl_shift_register_bank_unique_modules()) {
    VTR_ASSERT(true == module_manager.valid_module_id(sr_module));
    /* Write the module content in Verilog format */
    write_verilog_module_to_file(fp, module_manager, sr_module, 
                                 options.explicit_port_mapping(),
                                 options.default_net_type());

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* Close the file stream */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(verilog_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
