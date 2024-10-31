#ifndef IO_NAME_MAP_H
#define IO_NAME_MAP_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <array>
#include <map>
#include <string>
#include <vector>

#include "openfpga_port.h"

/* Begin namespace openfpga */
namespace openfpga {

/**
 * @brief I/O name map is a data structure to show mapping between the ports
 * of fpga_top and fpga_core, which are the two possible top-level modules that
 * modeling a complete FPGA fabric Using the data structure, developers can find
 * - the corresponding port of fpga_core, with a given port of fpga_top
 * - the corresponding port of fpga_top, with a given port of fpga_core
 */
class IoNameMap {
 public: /* Types */
  enum class e_dummy_port_direction { INPUT = 0, OUTPUT, INOUT, NUM_TYPES };
  enum class e_port_mapping_status {
    NONE = 0,
    PARTIAL,
    FULL,
    OVERLAPPED,
    NUM_TYPES
  };

 public: /* Constructors */
  IoNameMap();

 public: /* Public accessors */
  /** @brief Get all the fpga top ports */
  std::vector<BasicPort> fpga_top_ports() const;
  /** @brief With a given port at fpga_top, find the corresponding I/O at
   * fpga_core. Return an invalid port if not found */
  BasicPort fpga_core_port(const BasicPort& fpga_top_port) const;
  /** @brief With a given port at fpga_core, find the corresponding I/O at
   * fpga_top. Return an invalid port if not found */
  BasicPort fpga_top_port(const BasicPort& fpga_core_port) const;
  /** @brief Identify if the fpga_top port is dummy or not */
  bool fpga_top_port_is_dummy(const BasicPort& fpga_top_port) const;
  /** @brief Get the direction of a dummy port */
  e_dummy_port_direction fpga_top_dummy_port_direction(
    const BasicPort& fpga_top_port) const;
  /** @brief Check if a core port, by considering its port name only, has been
   * fully/partially/no mapped to fpga top. For example, there is a core port
   * 'a[0:3]', while only 'a[0]' is mapped to the fpga top. We can use the port
   * name find it quickly
   */
  e_port_mapping_status fpga_core_port_mapping_status(
    const BasicPort& fpga_core_port, const bool& verbose = false) const;
  /** @brief Identify if there are any naming rules inside */
  bool empty() const;

 public: /* Public mutators */
  /** @brief Create the one-on-one mapping between an port of fpga_top and
   * fpga_core. Return 0 for success, return 1 for fail */
  int set_io_pair(const BasicPort& fpga_top_port,
                  const BasicPort& fpga_core_port);
  /** @brief Add a dummy port at the fpga top, which is not mapped any port at
   * fpga_core */
  int set_dummy_io(const BasicPort& fpga_top_port,
                   const e_dummy_port_direction& direction);

 public: /* Public utility */
  /** @brief Parse the dummy port direction from string to valid type. Parser
   * error can be turned on */
  e_dummy_port_direction str2dummy_port_dir(const std::string& dir_str,
                                            const bool& verbose = false) const;
  /** @brief Output the string representing dummy port direction */
  std::string dummy_port_dir2str(const e_dummy_port_direction& dir,
                                 const bool& verbose = false) const;
  /** @brief Validate the dummy port direction */
  bool valid_dummy_port_direction(
    const e_dummy_port_direction& direction) const;

 private: /* Internal utility */
  /* Convert a port info to string, which can be used to store keys */
  std::string port2str(const BasicPort& port) const;
  /* Convert a string to port, which can be used to echo internal info */
  BasicPort str2port(const std::string& port_str) const;
  /* Generate a string include all the valid directions of the dummy port.
   * Useful for printing debugging messages */
  std::string dummy_port_dir_all2str() const;

 private: /* Internal Data */
  /* fpga_top -> fpga_core io_name_keys. Use the port name to find all the port
   * details. For instance: prog_clk -> ["prog_clk[0:1]", "prog_clk[2:3]"] The
   * keys are then used to spot the fpga core ports in the io_name_map_. For
   * instance: "prog_clk[0:1]" -> pclk[0:1]
   */
  std::map<std::string, std::vector<std::string>> top2core_io_name_keys_;
  std::map<std::string, BasicPort> top2core_io_name_map_;

  std::map<std::string, std::vector<std::string>> core2top_io_name_keys_;
  std::map<std::string, BasicPort> core2top_io_name_map_;

  std::map<std::string, e_dummy_port_direction> dummy_port_direction_;

  /* Constants */
  std::array<const char*, size_t(e_dummy_port_direction::NUM_TYPES)>
    DUMMY_PORT_DIR_STRING_;
};

} /* End namespace openfpga*/

#endif
