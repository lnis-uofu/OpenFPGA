#ifndef VPR_NETLIST_ANNOTATION_H
#define VPR_NETLIST_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from vpr library */
#include "atom_netlist.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type is a physical pb_type or a operating pb_type
 * 2. what is the circuit model id linked to a physical pb_type
 * 3. what is the physical pb_type for an operating pb_type
 * 4. what is the mode pointer that represents the physical mode for a pb_type
 *******************************************************************/
class VprNetlistAnnotation {
  public:  /* Constructor */
    VprNetlistAnnotation();
  public:  /* Public accessors */
    bool is_block_renamed(const AtomBlockId& block) const;
    std::string block_name(const AtomBlockId& block) const;
    bool is_net_renamed(const AtomNetId& net) const;
    std::string net_name(const AtomNetId& net) const;
  public:  /* Public mutators */
    void rename_block(const AtomBlockId& block, const std::string& name);
    void rename_net(const AtomNetId& net, const std::string& name);
  private: /* Internal data */
    /* Pair a regular pb_type to its physical pb_type */
    std::map<AtomBlockId, std::string> block_names_;
    std::map<AtomNetId, std::string> net_names_;
};

} /* End namespace openfpga*/

#endif
