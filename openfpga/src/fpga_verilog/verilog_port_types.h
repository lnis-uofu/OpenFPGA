#ifndef VERILOG_PORT_TYPES_H
#define VERILOG_PORT_TYPES_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <array>

/* Begin namespace openfpga */
namespace openfpga {

enum e_verilog_default_net_type {
  VERILOG_DEFAULT_NET_TYPE_NONE,
  VERILOG_DEFAULT_NET_TYPE_WIRE,
  NUM_VERILOG_DEFAULT_NET_TYPES,
};
constexpr std::array<const char*, NUM_VERILOG_DEFAULT_NET_TYPES> VERILOG_DEFAULT_NET_TYPE_STRING = {{"none", "wire"}}; //String versions of default net types

enum e_dump_verilog_port_type {
  VERILOG_PORT_INPUT,
  VERILOG_PORT_OUTPUT,
  VERILOG_PORT_INOUT,
  VERILOG_PORT_WIRE,
  VERILOG_PORT_REG,
  VERILOG_PORT_CONKT,
  NUM_VERILOG_PORT_TYPES
};
constexpr std::array<const char*, NUM_VERILOG_PORT_TYPES> VERILOG_PORT_TYPE_STRING = {{"input", "output", "inout", "wire", "reg", ""}}; /* string version of enum e_verilog_port_type */

} /* End namespace openfpga*/

#endif

