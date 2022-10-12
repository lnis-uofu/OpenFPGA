#ifndef SDC_WRITER_NAMING_H
#define SDC_WRITER_NAMING_H

/* begin namespace openfpga */
namespace openfpga {

constexpr const char* SDC_FILE_NAME_POSTFIX = ".sdc";

constexpr const char* SDC_GLOBAL_PORTS_FILE_NAME = "global_ports.sdc";
constexpr const char* SDC_BENCHMARK_ANALYSIS_FILE_NAME =
  "fpga_top_analysis.sdc";
constexpr const char* SDC_DISABLE_CONFIG_MEM_OUTPUTS_FILE_NAME =
  "disable_configurable_memory_outputs.sdc";
constexpr const char* SDC_DISABLE_MUX_OUTPUTS_FILE_NAME =
  "disable_routing_multiplexer_outputs.sdc";
constexpr const char* SDC_DISABLE_SB_OUTPUTS_FILE_NAME =
  "disable_sb_outputs.sdc";
constexpr const char* SDC_CB_FILE_NAME = "cb.sdc";

constexpr const char* SDC_GRID_HIERARCHY_FILE_NAME = "grid_hierarchy.txt";
constexpr const char* SDC_SB_HIERARCHY_FILE_NAME = "sb_hierarchy.txt";
constexpr const char* SDC_CBX_HIERARCHY_FILE_NAME = "cbx_hierarchy.txt";
constexpr const char* SDC_CBY_HIERARCHY_FILE_NAME = "cby_hierarchy.txt";

constexpr const char* SDC_ANALYSIS_FILE_NAME = "fpga_top_analysis.sdc";

} /* end namespace openfpga */

#endif
