#ifndef OPENFPGA_INTERCONNECT_TYPES_H
#define OPENFPGA_INTERCONNECT_TYPES_H

/* begin namespace openfpga */
namespace openfpga {

enum e_pin2pin_interc_type {
  INPUT2INPUT_INTERC,
  OUTPUT2OUTPUT_INTERC,
  NUM_PIN2PIN_INTERC_TYPES
};

enum e_circuit_pb_port_type {
  CIRCUIT_PB_PORT_INPUT,
  CIRCUIT_PB_PORT_OUTPUT,
  CIRCUIT_PB_PORT_CLOCK,
  NUM_CIRCUIT_PB_PORT_TYPES 
};

} /* end namespace openfpga */

#endif
