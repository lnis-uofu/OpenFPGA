#ifndef BUILD_MODULE_GRAPH_H
#define BUILD_MODULE_GRAPH_H

#include "vpr_types.h"
#include "mux_library.h"
#include "module_manager.h"

ModuleManager build_device_module_graph(const t_vpr_setup& vpr_setup,
                                        const t_arch& arch,
                                        const MuxLibrary& mux_lib);

#endif
