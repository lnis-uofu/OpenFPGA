/******************************************************************************
 * Memember functions for data structure FlowManager
 ******************************************************************************/
#include "openfpga_flow_manager.h"

#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructor
 *************************************************/
FlowManager::FlowManager() {
  /* Turn off compress_routing as default */
  compress_routing_ = false;
}

/**************************************************
 * Public Accessors
 *************************************************/
bool FlowManager::compress_routing() const { return compress_routing_; }

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void FlowManager::set_compress_routing(const bool& enabled) {
  compress_routing_ = enabled;
}

} /* end namespace openfpga */
