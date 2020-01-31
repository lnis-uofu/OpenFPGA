/************************************************************************
 * Member functions for class VprNetlistAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_netlist_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprNetlistAnnotation::VprNetlistAnnotation() {
  return;
}

/************************************************************************
 * Public accessors
 ***********************************************************************/
bool VprNetlistAnnotation::is_block_renamed(const AtomBlockId& block) const {
  /* Ensure that the pb_type is in the list */
  std::map<AtomBlockId, std::string>::const_iterator it = block_names_.find(block);
  return it != block_names_.end();
}

std::string VprNetlistAnnotation::block_name(const AtomBlockId& block) const {
  VTR_ASSERT(true == is_block_renamed(block));
  return block_names_.at(block);
}

bool VprNetlistAnnotation::is_net_renamed(const AtomNetId& net) const {
  /* Ensure that the pb_type is in the list */
  std::map<AtomNetId, std::string>::const_iterator it = net_names_.find(net);
  return it != net_names_.end();
}

std::string VprNetlistAnnotation::net_name(const AtomNetId& net) const {
  VTR_ASSERT(true == is_net_renamed(net));
  return net_names_.at(net);
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprNetlistAnnotation::rename_block(const AtomBlockId& block, const std::string& name) {
  /* Warn any override attempt */
  std::map<AtomBlockId, std::string>::const_iterator it = block_names_.find(block);
  if (it != block_names_.end()) {
    VTR_LOG_WARN("Override the block with name '%s' in netlist annotation!\n",
                 name.c_str());
  }

  block_names_[block] = name;
}

void VprNetlistAnnotation::rename_net(const AtomNetId& net, const std::string& name) {
  /* Warn any override attempt */
  std::map<AtomNetId, std::string>::const_iterator it = net_names_.find(net);
  if (it != net_names_.end()) {
    VTR_LOG_WARN("Override the net with name '%s' in netlist annotation!\n",
                 name.c_str());
  }

  net_names_[net] = name;
}

} /* End namespace openfpga*/
