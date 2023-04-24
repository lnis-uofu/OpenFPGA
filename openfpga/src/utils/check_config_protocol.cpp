/************************************************************************
 * Check functions for the content of config protocol to avoid conflicts with
 * other data structures
 * These functions are not universal methods for the ConfigProtocol class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/
#include "check_config_protocol.h"

#include "circuit_library_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Check if the programming clock port defined in configuration protocol is a
 *valid global programming clock of a ccff model
 *******************************************************************/
static int check_config_protocol_programming_clock(
  const ConfigProtocol& config_protocol, const CircuitLibrary& circuit_lib) {
  int num_err = 0;
  /* Programming clock is only available for CCFF */
  if (config_protocol.type() != CONFIG_MEM_SCAN_CHAIN) {
    return num_err;
  }
  /* Must find a CCFF model */
  std::vector<CircuitModelId> ccff_models =
    circuit_lib.models_by_type(CIRCUIT_MODEL_CCFF);
  if (ccff_models.empty()) {
    VTR_LOG_ERROR(
      "Expect at least one CCFF model to be defined in circuit library!\n");
    num_err++;
  }
  /* Try to match the programming clock port name with the CCFF port name */
  for (BasicPort prog_clk_port : config_protocol.prog_clock_pins()) {
    bool port_match = false;
    for (CircuitModelId ccff_model : ccff_models) {
      CircuitPortId circuit_port =
        circuit_lib.model_port(ccff_model, prog_clk_port.get_name());
      if (circuit_port) {
        port_match = true;
        /* Ensure this is a programming clock and a global port */
        if (!circuit_lib.port_is_global(circuit_port)) {
          VTR_LOG_ERROR(
            "Expect the programming clock '%s' to be a global port but not!\n",
            prog_clk_port.get_name().c_str());
          num_err++;
        }
        if (circuit_lib.port_type(circuit_port) != CIRCUIT_MODEL_PORT_CLOCK) {
          VTR_LOG_ERROR(
            "Expect the programming clock '%s' to be a clock port but not!\n",
            prog_clk_port.get_name().c_str());
          num_err++;
        }
        if (!circuit_lib.port_is_prog(circuit_port)) {
          VTR_LOG_ERROR(
            "Expect the programming clock '%s' to be a programming port but "
            "not!\n",
            prog_clk_port.get_name().c_str());
          num_err++;
        }
      }
    }
    if (!port_match) {
      VTR_LOG_ERROR(
        "Fail to find a port of any CCFF model that matches the programming "
        "clock definition (Expect port name: '%s')!\n",
        prog_clk_port.get_name().c_str());
      num_err++;
    }
  }

  return num_err;
}

/********************************************************************
 * Check if the configuration protocol is valid without any conflict with
 * circuit library content.
 *******************************************************************/
bool check_config_protocol(const ConfigProtocol& config_protocol,
                           const CircuitLibrary& circuit_lib) {
  int num_err = 0;

  if (0 < config_protocol.validate()) {
    num_err++;
  }

  if (!check_configurable_memory_circuit_model(config_protocol, circuit_lib)) {
    num_err++;
  }

  num_err +=
    check_config_protocol_programming_clock(config_protocol, circuit_lib);

  VTR_LOG("Found %ld errors when checking configuration protocol!\n", num_err);
  return (0 == num_err);
}

} /* end namespace openfpga */
