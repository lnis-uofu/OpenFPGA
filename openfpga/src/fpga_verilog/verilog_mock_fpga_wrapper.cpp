/********************************************************************
 * This file includes functions that are used to generate
 * a Verilog module of a pre-configured FPGA fabric
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "bitstream_manager_utils.h"
#include "openfpga_atom_netlist_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "openfpga_reserved_words.h"
#include "verilog_constants.h"
#include "verilog_mock_fpga_wrapper.h"
#include "verilog_testbench_utils.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Local variables used only in this file
 *******************************************************************/
constexpr const char* BENCHMARK_PORT_POSTFIX = "_bench";
constexpr const char* BENCHMARK_INSTANCE_NAME = "MAPPED_DESIGN";

/********************************************************************
 * This function adds stimuli to I/Os of FPGA fabric
 * 1. For mapped I/Os, this function will wire them to the input ports
 *    of the pre-configured FPGA top module
 * 2. For unmapped I/Os, this function will assign a constant value
 *    by default
 *******************************************************************/
static 
void print_verilog_mock_fpga_wrapper_connect_ios(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& top_module, const AtomContext& atom_ctx,
  const PlacementContext& place_ctx, const IoLocationMap& io_location_map,
  const VprNetlistAnnotation& netlist_annotation, const BusGroup& bus_group,
  const std::string& net_name_postfix,
  const std::string& io_input_port_name_postfix,
  const std::string& io_output_port_name_postfix,
  const std::vector<std::string>& clock_port_names,
  const size_t& unused_io_value) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Only mappable i/o ports can be considered */
  std::vector<ModulePortId> module_io_ports;
  for (const ModuleManager::e_module_port_type& module_io_port_type :
       MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id :
         module_manager.module_port_ids_by_type(top_module,
                                                module_io_port_type)) {
      /* Only care mappable I/O */
      if (false ==
          module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }
      module_io_ports.push_back(gpio_port_id);
    }
  }

  /* Keep tracking which I/Os have been used */
  std::map<ModulePortId, std::vector<bool>> io_used;
  for (const ModulePortId& module_io_port_id : module_io_ports) {
    const BasicPort& module_io_port =
      module_manager.module_port(top_module, module_io_port_id);
    io_used[module_io_port_id] =
      std::vector<bool>(module_io_port.get_width(), false);
  }

  /* Type mapping between VPR block and Module port */
  std::map<AtomBlockType, ModuleManager::e_module_port_type>
    atom_block_type_to_module_port_type;
  atom_block_type_to_module_port_type[AtomBlockType::INPAD] =
    ModuleManager::MODULE_GPIN_PORT;
  atom_block_type_to_module_port_type[AtomBlockType::OUTPAD] =
    ModuleManager::MODULE_GPOUT_PORT;

  /* See if this I/O should be wired to a benchmark input/output */
  /* Add signals from blif benchmark and short-wire them to FPGA I/O PADs
   * This brings convenience to checking functionality
   */
  print_verilog_comment(
    fp, std::string("----- Link FPGA I/Os to Benchmark I/Os -----"));

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* If there is a GPIO port, use it directly
     * Otherwise, should find a GPIN for INPAD
     *         or should find a GPOUT for OUTPAD
     */
    std::pair<ModulePortId, size_t> mapped_module_io_info =
      std::make_pair(ModulePortId::INVALID(), -1);
    for (const ModulePortId& module_io_port_id : module_io_ports) {
      const BasicPort& module_io_port =
        module_manager.module_port(top_module, module_io_port_id);

      /* Find the index of the mapped GPIO in top-level FPGA fabric */
      size_t temp_io_index = io_location_map.io_index(
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.x,
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.y,
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.sub_tile,
        module_io_port.get_name());

      /* Bypass invalid index (not mapped to this GPIO port) */
      if (size_t(-1) == temp_io_index) {
        continue;
      }

      /* If the port is an GPIO port, just use it */
      if (ModuleManager::MODULE_GPIO_PORT ==
          module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info =
          std::make_pair(module_io_port_id, temp_io_index);
        break;
      }

      /* If this is an INPAD, we can use an GPIN port (if available) */
      if (atom_block_type_to_module_port_type[atom_ctx.nlist.block_type(
            atom_blk)] ==
          module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info =
          std::make_pair(module_io_port_id, temp_io_index);
        break;
      }
    }

    /* We must find a valid one */
    VTR_ASSERT(true == module_manager.valid_module_port_id(
                         top_module, mapped_module_io_info.first));
    VTR_ASSERT(size_t(-1) != mapped_module_io_info.second);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port =
      module_manager.module_port(top_module, mapped_module_io_info.first);
    size_t io_index = mapped_module_io_info.second;

    /* Set the port pin index */
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_name(module_mapped_io_port.get_name() +
                                   net_name_postfix);
    module_mapped_io_port.set_width(io_index, io_index);

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    /* Note that VPR added a prefix to the name of output blocks
     * We can remove this when specified through input argument
     */
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
    }

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always
     * has a size of 1 In addition, the input and output ports may have
     * different postfix in naming due to verification context! Here, we give
     * full customization on naming
     */
    BasicPort benchmark_io_port;

    /* If this benchmark pin belongs to any bus group, use the bus pin instead
     */
    BusGroupId bus_id = bus_group.find_pin_bus(block_name);
    BusPinId bus_pin_id = bus_group.find_pin(block_name);
    if (bus_id) {
      block_name = bus_group.bus_port(bus_id).get_name();
      VTR_ASSERT_SAFE(bus_pin_id);
      benchmark_io_port.set_width(bus_group.pin_index(bus_pin_id),
                                  bus_group.pin_index(bus_pin_id));
    } else {
      benchmark_io_port.set_width(1);
    }

    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      /* If the port is a clock, do not add a postfix */
      if (clock_port_names.end() != std::find(clock_port_names.begin(),
                                              clock_port_names.end(),
                                              block_name)) {
        benchmark_io_port.set_name(block_name);
      } else {
        benchmark_io_port.set_name(
          std::string(block_name + io_input_port_name_postfix));
      }
      print_verilog_comment(
        fp, std::string("----- Blif Benchmark input " + block_name +
                        " is mapped to FPGA IOPAD " +
                        module_mapped_io_port.get_name() + "[" +
                        std::to_string(io_index) + "] -----"));
      print_verilog_wire_connection(fp, benchmark_io_port,
                                    module_mapped_io_port,
                                    false);
    } else {
      VTR_ASSERT(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
      benchmark_io_port.set_name(
        std::string(block_name + io_output_port_name_postfix));
      print_verilog_comment(
        fp, std::string("----- Blif Benchmark output " + block_name +
                        " is mapped to FPGA IOPAD " +
                        module_mapped_io_port.get_name() + "[" +
                        std::to_string(io_index) + "] -----"));
      print_verilog_wire_connection(fp, module_mapped_io_port, benchmark_io_port, false);
    }

    /* Mark this I/O has been used/wired */
    io_used[mapped_module_io_info.first][io_index] = true;

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* Wire the unused iopads to a constant */
  print_verilog_comment(
    fp, std::string("----- Wire unused FPGA I/Os to constants -----"));
  for (const ModulePortId& module_io_port_id : module_io_ports) {
    for (size_t io_index = 0; io_index < io_used[module_io_port_id].size();
         ++io_index) {
      /* Bypass used iopads */
      if (true == io_used[module_io_port_id][io_index]) {
        continue;
      }

      /* Bypass unused output pads */
      if (ModuleManager::MODULE_GPOUT_PORT !=
          module_manager.port_type(top_module, module_io_port_id)) {
        continue;
      }

      /* Wire to a contant */
      BasicPort module_unused_io_port =
        module_manager.module_port(top_module, module_io_port_id);
      /* Set the port pin index */
      module_unused_io_port.set_name(module_unused_io_port.get_name() +
                                     net_name_postfix);
      module_unused_io_port.set_width(io_index, io_index);

      std::vector<size_t> default_values(module_unused_io_port.get_width(),
                                         unused_io_value);
      print_verilog_wire_constant_values(fp, module_unused_io_port,
                                         default_values);
    }

    /* Add an empty line as a splitter */
    fp << std::endl;
  }
}

/********************************************************************
 * Top-level function to generate a Verilog module of
 * a mock FPGA wrapper which contains an benchmark instance.
 *
 *   Mock FPGA wrapper
 *                        +--------------------------------------------
 *                        |
 *                        |          Benchmark instance
 *                        |          +-------------------------------+
 *                        |          |                               |
 *        fpga_clock----->|--------->|benchmark_clock                |
 *                        |          |                               |
 *        fpga_inputs---->|--------->|benchmark_inputs               |
 *                        |          |                               |
 *        fpga_outputs<---|<---------|benchmark_output               |
 *                        |          |                               |
 *                        |          +-------------------------------+
 *                        |
 *                        +-------------------------------------------
 *
 * Note: we do NOT put this module in the module manager.
 * Because, it is not a standard module, where we just wrap an instance of application HDL (supposed to be implemented on FPGA).
 *******************************************************************/
int print_verilog_mock_fpga_wrapper(
  const ModuleManager &module_manager,
  const BitstreamManager &bitstream_manager,
  const ConfigProtocol &config_protocol, const CircuitLibrary &circuit_lib,
  const FabricGlobalPortInfo &global_ports, const AtomContext &atom_ctx,
  const PlacementContext &place_ctx, const PinConstraints &pin_constraints,
  const BusGroup &bus_group, const IoLocationMap &io_location_map,
  const VprNetlistAnnotation &netlist_annotation,
  const std::string &circuit_name, const std::string &verilog_fname,
  const VerilogTestbenchOption &options) {
  std::string timer_message =
    std::string(
      "Write mock FPGA wrapper in Verilog format for design '") +
    circuit_name + std::string("'");

  int status = CMD_EXEC_SUCCESS;

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title =
    std::string("Verilog netlist for mock FPGA fabric by design: ") +
    circuit_name;
  print_verilog_file_header(fp, title, options.time_stamp());

  print_verilog_default_net_type_declaration(fp, options.default_net_type());

  /* Find the top_module */
  ModuleId top_module =
    module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Print module declaration */
  print_verilog_module_declaration(fp, module_manager, top_module,
                                   default_net_type);

  /* Find clock ports in benchmark */
  std::vector<std::string> benchmark_clock_port_names =
    find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

  /* Instanciate application HDL module */
  print_verilog_testbench_benchmark_instance(
    fp, circuit_name, std::string(BENCHMARK_INSTANCE_NAME),
    std::string(), std::string(), std::string(),
    std::string(BENCHMARK_PORT_POSTFIX), benchmark_clock_port_names, atom_ctx,
    netlist_annotation, pin_constraints, bus_group, options.explicit_port_mapping());

  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_mock_fpga_wrapper_connect_ios(
    fp, module_manager, top_module, atom_ctx, place_ctx, io_location_map,
    netlist_annotation, bus_group,
    std::string(BENCHMARK_PORT_POSTFIX), std::string(),
    std::string(), std::vector<std::string>(),
    (size_t)VERILOG_DEFAULT_SIGNAL_INIT_VALUE);

  /* Testbench ends*/
  print_verilog_module_end(fp, title);

  /* Close the file stream */
  fp.close();

  return status;
}

} /* end namespace openfpga */
