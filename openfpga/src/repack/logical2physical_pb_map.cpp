/******************************************************************************
 * Memember functions for data structure PhysicalPb
 ******************************************************************************/
#include "logical2physical_pb_map.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors
 *************************************************/
t_pb_type* Logical2PhysicalPbMap::pb_type(t_pb_type* lgk_pb_type) const {
  if (empty()) {
    return lgk_pb_type;
  }
  auto result = pb_type_map_.find(lgk_pb_type);
  if (result == pb_type_map_.end()) {
    return nullptr;
  }
  return result->second;
}

t_pb_graph_node* Logical2PhysicalPbMap::pb_graph_node(
  t_pb_graph_node* lgk_pb_graph_node) const {
  if (empty()) {
    return lgk_pb_graph_node;
  }
  auto result = pb_graph_node_map_.find(lgk_pb_graph_node);
  if (result == pb_graph_node_map_.end()) {
    return nullptr;
  }
  return result->second;
}

t_pb_graph_pin* Logical2PhysicalPbMap::pb_graph_pin(
  t_pb_graph_pin* lgk_pb_graph_pin) const {
  if (empty()) {
    return lgk_pb_graph_pin;
  }
  auto result = pb_graph_pin_map_.find(lgk_pb_graph_pin);
  if (result == pb_graph_pin_map_.end()) {
    return nullptr;
  }
  return result->second;
}

bool Logical2PhysicalPbMap::build_pb_graph_input_pin_map(
  t_pb_graph_node* lgk_pb_graph_node, t_pb_graph_node* phy_pb_graph_node,
  const bool& verbose) {
  if (lgk_pb_graph_node->num_input_ports !=
      phy_pb_graph_node->num_input_ports) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' has different number of input ports (%d) "
      "than physical pb_graph_node '%s' whose number of input ports is (%d). "
      "The two cannot be considered as equivalent sites for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      lgk_pb_graph_node->num_input_ports,
      phy_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->num_input_ports);
    return false;
  }
  for (int iport = 0; iport < lgk_pb_graph_node->num_input_ports; ++iport) {
    if (lgk_pb_graph_node->num_input_pins[iport] !=
        phy_pb_graph_node->num_input_pins[iport]) {
      VTR_LOG_ERROR(
        "Logical pb_graph_node '%s' has a input port which has a different "
        "number of pins (%d) than physical pb_graph_node '%s' whose number of "
        "pins is (%d). The two cannot be considered as equivalent sites for "
        "repacking\n",
        lgk_pb_graph_node->hierarchical_type_name().c_str(),
        lgk_pb_graph_node->num_input_pins[iport],
        phy_pb_graph_node->hierarchical_type_name().c_str(),
        phy_pb_graph_node->num_input_pins[iport]);
      return false;
    }
    for (int ipin = 0; ipin < lgk_pb_graph_node->num_input_pins[iport];
         ++ipin) {
      if (std::string(lgk_pb_graph_node->input_pins[iport][ipin].port->name) !=
          std::string(phy_pb_graph_node->input_pins[iport][ipin].port->name)) {
        VTR_LOG_ERROR(
          "Logical pb_graph_node '%s' has a input port which has a different "
          "name '%s' than physical pb_graph_node '%s' whose port name is '%s'. "
          "The two cannot be considered as equivalent sites for repacking\n",
          lgk_pb_graph_node->hierarchical_type_name().c_str(),
          lgk_pb_graph_node->input_pins[iport][ipin].port->name,
          phy_pb_graph_node->hierarchical_type_name().c_str(),
          phy_pb_graph_node->input_pins[iport][ipin].port->name);
        return false;
      }
      /* Build the map, sanity checks passed */
      pb_graph_pin_map_[&lgk_pb_graph_node->input_pins[iport][ipin]] =
        &phy_pb_graph_node->input_pins[iport][ipin];
    }
  }
  VTR_LOGV(verbose,
           "Logical pb_graph_node '%s' is equivalent in input ports to "
           "physical pb_graph_node '%s'\n",
           lgk_pb_graph_node->hierarchical_type_name().c_str(),
           phy_pb_graph_node->hierarchical_type_name().c_str());
  return true;
}

bool Logical2PhysicalPbMap::build_pb_graph_output_pin_map(
  t_pb_graph_node* lgk_pb_graph_node, t_pb_graph_node* phy_pb_graph_node,
  const bool& verbose) {
  if (lgk_pb_graph_node->num_output_ports !=
      phy_pb_graph_node->num_output_ports) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' has different number of output ports (%d) "
      "than physical pb_graph_node '%s' whose number of output ports is (%d). "
      "The two cannot be considered as equivalent sites for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      lgk_pb_graph_node->num_output_ports,
      phy_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->num_output_ports);
    return false;
  }
  for (int iport = 0; iport < lgk_pb_graph_node->num_output_ports; ++iport) {
    if (lgk_pb_graph_node->num_output_pins[iport] !=
        phy_pb_graph_node->num_output_pins[iport]) {
      VTR_LOG_ERROR(
        "Logical pb_graph_node '%s' has a output port which has a different "
        "number of pins (%d) than physical pb_graph_node '%s' whose number of "
        "pins is (%d). The two cannot be considered as equivalent sites for "
        "repacking\n",
        lgk_pb_graph_node->hierarchical_type_name().c_str(),
        lgk_pb_graph_node->num_output_pins[iport],
        phy_pb_graph_node->hierarchical_type_name().c_str(),
        phy_pb_graph_node->num_output_pins[iport]);
      return false;
    }
    for (int ipin = 0; ipin < lgk_pb_graph_node->num_output_pins[iport];
         ++ipin) {
      if (std::string(lgk_pb_graph_node->output_pins[iport][ipin].port->name) !=
          std::string(phy_pb_graph_node->output_pins[iport][ipin].port->name)) {
        VTR_LOG_ERROR(
          "Logical pb_graph_node '%s' has a output port which has a different "
          "name '%s' than physical pb_graph_node '%s' whose port name is '%s'. "
          "The two cannot be considered as equivalent sites for repacking\n",
          lgk_pb_graph_node->hierarchical_type_name().c_str(),
          lgk_pb_graph_node->output_pins[iport][ipin].port->name,
          phy_pb_graph_node->hierarchical_type_name().c_str(),
          phy_pb_graph_node->output_pins[iport][ipin].port->name);
        return false;
      }
      /* Build the map, sanity checks passed */
      pb_graph_pin_map_[&lgk_pb_graph_node->output_pins[iport][ipin]] =
        &phy_pb_graph_node->output_pins[iport][ipin];
    }
  }
  VTR_LOGV(verbose,
           "Logical pb_graph_node '%s' is equivalent in output ports to "
           "physical pb_graph_node '%s'\n",
           lgk_pb_graph_node->hierarchical_type_name().c_str(),
           phy_pb_graph_node->hierarchical_type_name().c_str());
  return true;
}

bool Logical2PhysicalPbMap::build_pb_graph_clock_pin_map(
  t_pb_graph_node* lgk_pb_graph_node, t_pb_graph_node* phy_pb_graph_node,
  const bool& verbose) {
  if (lgk_pb_graph_node->num_clock_ports !=
      phy_pb_graph_node->num_clock_ports) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' has different number of clock ports (%d) "
      "than physical pb_graph_node '%s' whose number of clock ports is (%d). "
      "The two cannot be considered as equivalent sites for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      lgk_pb_graph_node->num_clock_ports,
      phy_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->num_clock_ports);
    return false;
  }
  for (int iport = 0; iport < lgk_pb_graph_node->num_clock_ports; ++iport) {
    if (lgk_pb_graph_node->num_clock_pins[iport] !=
        phy_pb_graph_node->num_clock_pins[iport]) {
      VTR_LOG_ERROR(
        "Logical pb_graph_node '%s' has a clock port which has a different "
        "number of pins (%d) than physical pb_graph_node '%s' whose number of "
        "pins is (%d). The two cannot be considered as equivalent sites for "
        "repacking\n",
        lgk_pb_graph_node->hierarchical_type_name().c_str(),
        lgk_pb_graph_node->num_clock_pins[iport],
        phy_pb_graph_node->hierarchical_type_name().c_str(),
        phy_pb_graph_node->num_clock_pins[iport]);
      return false;
    }
    for (int ipin = 0; ipin < lgk_pb_graph_node->num_clock_pins[iport];
         ++ipin) {
      if (std::string(lgk_pb_graph_node->clock_pins[iport][ipin].port->name) !=
          std::string(phy_pb_graph_node->clock_pins[iport][ipin].port->name)) {
        VTR_LOG_ERROR(
          "Logical pb_graph_node '%s' has a clock port which has a different "
          "name '%s' than physical pb_graph_node '%s' whose port name is '%s'. "
          "The two cannot be considered as equivalent sites for repacking\n",
          lgk_pb_graph_node->hierarchical_type_name().c_str(),
          lgk_pb_graph_node->clock_pins[iport][ipin].port->name,
          phy_pb_graph_node->hierarchical_type_name().c_str(),
          phy_pb_graph_node->clock_pins[iport][ipin].port->name);
        return false;
      }
      /* Build the map, sanity checks passed */
      pb_graph_pin_map_[&lgk_pb_graph_node->clock_pins[iport][ipin]] =
        &phy_pb_graph_node->clock_pins[iport][ipin];
    }
  }
  VTR_LOGV(verbose,
           "Logical pb_graph_node '%s' is equivalent in clock ports to "
           "physical pb_graph_node '%s'\n",
           lgk_pb_graph_node->hierarchical_type_name().c_str(),
           phy_pb_graph_node->hierarchical_type_name().c_str());
  return true;
}

bool Logical2PhysicalPbMap::rec_build_pb_map(t_pb_graph_node* lgk_pb_graph_node,
                                             t_pb_graph_node* phy_pb_graph_node,
                                             const bool& verbose) {
  /* Only accept both are root node or neither */
  if (lgk_pb_graph_node->is_root() && !phy_pb_graph_node->is_root()) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' is a root node while physical pb_graph_node "
      "'%s' is not\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->hierarchical_type_name().c_str());
  }
  if (!lgk_pb_graph_node->is_root() && phy_pb_graph_node->is_root()) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' is not a root node while physical "
      "pb_graph_node '%s' is\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->hierarchical_type_name().c_str());
  }
  /* Check if the node has the same name and relative index when these not root
   * nodes  */
  if (!lgk_pb_graph_node->is_root() && !phy_pb_graph_node->is_root()) {
    if (std::string(lgk_pb_graph_node->pb_type->name) !=
        std::string(phy_pb_graph_node->pb_type->name)) {
      VTR_LOG_ERROR(
        "Logical pb_graph_node '%s' is different than physical pb_graph_node "
        "'%s' in term of name. The two cannot be considered as equivalent "
        "sites for repacking\n",
        lgk_pb_graph_node->hierarchical_type_name().c_str(),
        phy_pb_graph_node->hierarchical_type_name().c_str());
      return false;
    }
  }
  if (lgk_pb_graph_node->placement_index !=
      phy_pb_graph_node->placement_index) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' has different relative index (%d) than "
      "physical pb_graph_node '%s' with  index (%d). The two cannot be "
      "considered as equivalent sites for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      lgk_pb_graph_node->placement_index,
      phy_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->placement_index);
    return false;
  }
  /* Check input pins are the same w.r.t. name and width */
  bool status =
    build_pb_graph_input_pin_map(lgk_pb_graph_node, phy_pb_graph_node, verbose);
  if (!status) {
    return false;
  }
  status = build_pb_graph_output_pin_map(lgk_pb_graph_node, phy_pb_graph_node,
                                         verbose);
  if (!status) {
    return false;
  }
  status =
    build_pb_graph_clock_pin_map(lgk_pb_graph_node, phy_pb_graph_node, verbose);
  if (!status) {
    return false;
  }
  /* Must be both primitive or neither */
  if (!lgk_pb_graph_node->is_primitive() && phy_pb_graph_node->is_primitive()) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' is a primitive one while physical "
      "pb_graph_node '%s' is not. The two cannot be considered as equivalent "
      "sites for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->hierarchical_type_name().c_str());
    return false;
  }
  if (lgk_pb_graph_node->is_primitive() && !phy_pb_graph_node->is_primitive()) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' is not a primitive one while physical "
      "pb_graph_node '%s' is. The two cannot be considered as equivalent sites "
      "for repacking\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->hierarchical_type_name().c_str());
    return false;
  }
  /* Stop going recursive for primitives */
  if (lgk_pb_graph_node->is_primitive() && phy_pb_graph_node->is_primitive()) {
    /* Build the map */
    pb_graph_node_map_[lgk_pb_graph_node] = phy_pb_graph_node;
    pb_type_map_[lgk_pb_graph_node->pb_type] = phy_pb_graph_node->pb_type;

    VTR_LOGV(verbose,
             "Logical pb_graph_node '%s' is equivalent to physical "
             "pb_graph_node '%s'\n",
             lgk_pb_graph_node->hierarchical_type_name().c_str(),
             phy_pb_graph_node->hierarchical_type_name().c_str());
    return true;
  }

  /* Go recursively */
  VTR_LOGV(verbose,
           "Go to compare children of logical pb_graph_node '%s' and physical "
           "pb_graph_node '%s' as neither are primitive nodes\n",
           lgk_pb_graph_node->hierarchical_type_name().c_str(),
           phy_pb_graph_node->hierarchical_type_name().c_str());

  if (lgk_pb_graph_node->pb_type->num_modes !=
      phy_pb_graph_node->pb_type->num_modes) {
    VTR_LOG_ERROR(
      "Logical pb_graph_node '%s' contains a different number of modes '%d' "
      "than physical pb_graph_node '%s' "
      "which has %d modes.\n",
      lgk_pb_graph_node->hierarchical_type_name().c_str(),
      lgk_pb_graph_node->pb_type->num_modes,
      phy_pb_graph_node->hierarchical_type_name().c_str(),
      phy_pb_graph_node->pb_type->num_modes);
    return false;
  }

  for (int imode = 0; imode < lgk_pb_graph_node->pb_type->num_modes; ++imode) {
    t_mode* lgk_pb_mode = &lgk_pb_graph_node->pb_type->modes[imode];
    t_mode* phy_pb_mode = &phy_pb_graph_node->pb_type->modes[imode];

    if (lgk_pb_mode->num_pb_type_children !=
        phy_pb_mode->num_pb_type_children) {
      VTR_LOG_ERROR(
        "Logical pb_graph_node '%s' contains a mode '%s' which has a different "
        "number of child pb_graph_nodes (%d) than physical pb_graph_node '%s' "
        "whose mode '%s' has %d child pb_graph_nodes. The two cannot be "
        "considered as equivalent sites for repacking\n",
        lgk_pb_graph_node->hierarchical_type_name().c_str(), lgk_pb_mode->name,
        lgk_pb_mode->num_pb_type_children,
        phy_pb_graph_node->hierarchical_type_name().c_str(), phy_pb_mode->name,
        phy_pb_mode->num_pb_type_children);
      return false;
    }
    for (int ipb = 0; ipb < lgk_pb_mode->num_pb_type_children; ipb++) {
      if (lgk_pb_mode->pb_type_children[ipb].num_pb !=
          phy_pb_mode->pb_type_children[ipb].num_pb) {
        VTR_LOG_ERROR(
          "Logical pb_graph_node '%s' contains a child pb_type '%s' whose "
          "count "
          "(%d) is different than physical pb_graph_node '%s' who has %d child "
          "pb_types in the same name. The two cannot be considered as "
          "equivalent "
          "sites for repacking\n",
          lgk_pb_graph_node->hierarchical_type_name().c_str(),
          lgk_pb_mode->pb_type_children[ipb].name,
          lgk_pb_mode->pb_type_children[ipb].num_pb,
          phy_pb_graph_node->hierarchical_type_name().c_str(),
          phy_pb_mode->pb_type_children[ipb].name,
          phy_pb_mode->pb_type_children[ipb].num_pb);
        return false;
      }
      for (int jpb = 0; jpb < lgk_pb_mode->pb_type_children[ipb].num_pb;
           jpb++) {
        t_pb_graph_node* lgk_child_pb_graph_node =
          &(lgk_pb_graph_node
              ->child_pb_graph_nodes[lgk_pb_mode->index][ipb][jpb]);
        t_pb_graph_node* phy_child_pb_graph_node =
          &(phy_pb_graph_node
              ->child_pb_graph_nodes[phy_pb_mode->index][ipb][jpb]);
        status = rec_build_pb_map(lgk_child_pb_graph_node,
                                  phy_child_pb_graph_node, verbose);
        if (!status) {
          return false;
        }
      }
    }
  }

  /* Build the map */
  pb_graph_node_map_[lgk_pb_graph_node] = phy_pb_graph_node;
  pb_type_map_[lgk_pb_graph_node->pb_type] = phy_pb_graph_node->pb_type;

  VTR_LOGV(
    verbose,
    "Logical pb_graph_node '%s' is equivalent to physical pb_graph_node '%s'\n",
    lgk_pb_graph_node->hierarchical_type_name().c_str(),
    phy_pb_graph_node->hierarchical_type_name().c_str());
  return true;
}

bool Logical2PhysicalPbMap::init(t_logical_block_type_ptr lgk_lb_type,
                                 t_logical_block_type_ptr phy_lb_type,
                                 const bool& verbose) {
  if (lgk_lb_type == phy_lb_type) {
    VTR_LOGV(verbose,
             "Logical and physical equivalent sites are the same. Skip to "
             "build detailed mapping\n");
    return true;
  }
  bool status = rec_build_pb_map(lgk_lb_type->pb_graph_head,
                                 phy_lb_type->pb_graph_head, verbose);
  if (!status) {
    VTR_LOGV(verbose,
             "Logical pb_graph_node '%s' are not equivalent to physical "
             "pb_graph_node '%s'\n",
             lgk_lb_type->pb_graph_head->hierarchical_type_name().c_str(),
             phy_lb_type->pb_graph_head->hierarchical_type_name().c_str());
    /* Clean all the mapping as failed */
    clear();
  }
  return status;
}

void Logical2PhysicalPbMap::clear() {
  pb_type_map_.clear();
  pb_graph_node_map_.clear();
  pb_graph_pin_map_.clear();
}

bool Logical2PhysicalPbMap::empty() const {
  return pb_type_map_.empty() && pb_graph_node_map_.empty() &&
         pb_graph_pin_map_.empty();
}

} /* end namespace openfpga */
