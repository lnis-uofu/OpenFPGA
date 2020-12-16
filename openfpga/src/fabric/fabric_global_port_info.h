#ifndef FABRIC_GLOBAL_PORT_INFO_H
#define FABRIC_GLOBAL_PORT_INFO_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <vector>
#include <string>

#include "vtr_vector.h"
#include "module_manager_fwd.h"

#include "fabric_global_port_info_fwd.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * A data structure to store necessary information for the global
 * ports that have been defined for the FPGA fabric
 *
 * Currently the global port information is mainly used for testbench generation
 *******************************************************************/
class FabricGlobalPortInfo {
  public: /* Types */
    typedef vtr::vector<FabricGlobalPortId, FabricGlobalPortId>::const_iterator global_port_iterator;
    /* Create range */
    typedef vtr::Range<global_port_iterator> global_port_range;
  public:  /* Constructor */
    FabricGlobalPortInfo();
  public:  /* Public accessors: aggregators */
    global_port_range global_ports() const;
  public:  /* Public accessors */
    ModulePortId global_module_port(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_clock(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_set(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_reset(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_prog(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_config_enable(const FabricGlobalPortId& global_port_id) const;
    bool global_port_is_io(const FabricGlobalPortId& global_port_id) const;
    size_t global_port_default_value(const FabricGlobalPortId& global_port_id) const;
  public: /* Public mutators */
    /* By default, we do not set it as a clock.
     * Users should set it through the set_global_port_is_clock() function
     */
    FabricGlobalPortId create_global_port(const ModulePortId& module_port);
    void set_global_port_is_clock(const FabricGlobalPortId& global_port_id,
                                  const bool& is_clock);
    void set_global_port_is_set(const FabricGlobalPortId& global_port_id,
                                const bool& is_set);
    void set_global_port_is_reset(const FabricGlobalPortId& global_port_id,
                                  const bool& is_reset);
    void set_global_port_is_prog(const FabricGlobalPortId& global_port_id,
                                 const bool& is_prog);
    void set_global_port_is_config_enable(const FabricGlobalPortId& global_port_id,
                                          const bool& is_config_enable);
    void set_global_port_is_io(const FabricGlobalPortId& global_port_id,
                               const bool& is_io);
    void set_global_port_default_value(const FabricGlobalPortId& global_port_id,
                                       const size_t& default_value);
  public: /* Public validator */
    bool valid_global_port_id(const FabricGlobalPortId& global_port_id) const;
  private: /* Internal data */
    /* Global port information for tiles */
    vtr::vector<FabricGlobalPortId, FabricGlobalPortId> global_port_ids_;
    vtr::vector<FabricGlobalPortId, ModulePortId> global_module_ports_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_clock_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_reset_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_set_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_prog_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_config_enable_;
    vtr::vector<FabricGlobalPortId, bool> global_port_is_io_;
    vtr::vector<FabricGlobalPortId, size_t> global_port_default_values_;
};

} /* namespace openfpga ends */

#endif

