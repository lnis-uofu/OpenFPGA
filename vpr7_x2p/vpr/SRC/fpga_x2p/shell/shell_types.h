#ifndef SHELL_TYPES_H 
#define SHELL_TYPES_H 

#include <vector>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "mux_library.h"
#include "rr_blocks.h"
#include "module_manager.h"
#include "bitstream_manager.h"

typedef struct s_cmd_category t_cmd_category;
typedef struct s_shell_cmd t_shell_cmd;
typedef struct s_shell_env t_shell_env;

enum e_cmd_category {
  BASIC_CMD,
  SETUP_CMD,
  PACK_CMD,
  PLACE_CMD,
  ROUTE_CMD,
  ANALYSIS_CMD,
  PRODUCTION_CMD,
  LAST_CMD_CATEGORY
};

struct s_cmd_category {
  e_cmd_category name;
  char* label; 
};


struct s_shell_cmd {
  char* name;
  e_cmd_category category;
  t_opt_info* opts;
  void (*execute)(t_shell_env*, t_opt_info*);
};

struct s_shell_env {
  ModuleManager module_manager;
  BitstreamManager bitstream_manager;
  std::vector<ConfigBitId> fabric_bitstream;
  MuxLibrary mux_lib;
  std::vector<t_logical_block> logical_blocks;
  vtr::Point<size_t> device_size;
  std::vector<std::vector<t_grid_tile>> grids; 
  std::vector<t_block> blocks;
  DeviceRRGSB device_rr_gsb;
  t_sram_orgz_info sram_orgz_info;
  t_arch arch;
  t_vpr_setup vpr_setup;
  t_shell_cmd* cmd;
  t_cmd_category* cmd_category;
};

#define LAST_CMD_NAME NULL

#endif
