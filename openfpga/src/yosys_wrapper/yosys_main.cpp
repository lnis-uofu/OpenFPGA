#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND

#include "yosys_main.h"

#include "command_exit_codes.h"
#include "kernel/yosys.h"
#include "vtr_log.h"

namespace openfpga {

/**
 * Runs a Yosys synthesis script directly via the C++ API.
 * @param script_path The filesystem path to the Yosys script (e.g.,
 * "synth.ys").
 * @return true if successful, false otherwise.
 */
int yosys_script_mode_wrapper(const std::string& script_path) {
  try {
    // 1. Initialize global Yosys state and log systems
    Yosys::yosys_setup();
    Yosys::log_errfile = stderr;
    Yosys::log_streams.push_back(&std::cout);

    // 2. Instantiate a persistent design container
    // This holds the AST, Netlist, and Design modules in memory
    Yosys::RTLIL::Design* design = new Yosys::RTLIL::Design;

    // 3. Execute the script frontend pass directly on your design object.
    // Yosys treats scripts as frontend passes (similar to "read_verilog").
    VTR_LOG("Processing yosys script file: %s\n", script_path.c_str());
    Yosys::run_frontend(script_path, "script", design);

    // 4. Verify design integrity after script completion
    design->check();

    // 5. Clean up allocated design context memory safely
    delete design;

    // 6. Safely tear down global state structures
    Yosys::yosys_shutdown();

    VTR_LOG("Yosys script execution completed successfully.\n");
    return CMD_EXEC_SUCCESS;
  } catch (const std::exception& e) {
    VTR_LOG_ERROR("Error during Yosys execution: %s\n", e.what());
    return CMD_EXEC_FATAL_ERROR;
  } catch (...) {
    VTR_LOG_ERROR("An unhandled Yosys exception occurred.\n");
    return CMD_EXEC_FATAL_ERROR;
  }
}

}  // namespace openfpga

#endif
