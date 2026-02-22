#pragma once

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map>

#include "physical_types.h"
#include "vpr_device_annotation.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This contains multiple map from the following of a logical equivalent site
 * - pb_type
 * - pb_graph_node
 * - pb_graph_pin
 * to those of its physical equivalent site
 * Note:
 * If the logical equivalent site is known to be the same as the physical
 *equivalent site, please skip the function init().
 *******************************************************************/
class Logical2PhysicalPbMap {
 public: /* Accessors */
  t_pb_type* pb_type(t_pb_type* lgk_pb_type) const;
  t_pb_graph_node* pb_graph_node(t_pb_graph_node* lgk_pb_graph_node) const;
  t_pb_graph_pin* pb_graph_pin(t_pb_graph_pin* lgk_pb_graph_pin) const;

 public: /* Public mutators */
  /* Build the 1:1 map on pb_type, pb_graph_node and pb_graph_pin  between the
   * logical and physical pb_graph.  Return false is failed. This requires two
   * pb_graph has exactly the same hierarchy, names and pins unless the
   * top-level pb_type name could be different */
  bool init(t_logical_block_type_ptr lgk_lb_type,
            t_logical_block_type_ptr phy_lb_type, const bool& verbose);
  void clear();

 public: /* Public validators/invalidators */
  bool empty() const;

 private: /* Private utilities */
  bool rec_build_pb_map(t_pb_graph_node* lgk_pb_graph_node,
                        t_pb_graph_node* phy_pb_graph_node,
                        const bool& verbose);
  bool build_pb_graph_input_pin_map(t_pb_graph_node* lgk_pb_graph_node,
                                    t_pb_graph_node* phy_pb_graph_node,
                                    const bool& verbose);
  bool build_pb_graph_output_pin_map(t_pb_graph_node* lgk_pb_graph_node,
                                     t_pb_graph_node* phy_pb_graph_node,
                                     const bool& verbose);
  bool build_pb_graph_clock_pin_map(t_pb_graph_node* lgk_pb_graph_node,
                                    t_pb_graph_node* phy_pb_graph_node,
                                    const bool& verbose);

 private: /* Internal Data */
  /* logical -> physical  */
  std::map<t_pb_type*, t_pb_type*> pb_type_map_;
  std::map<t_pb_graph_node*, t_pb_graph_node*> pb_graph_node_map_;
  std::map<t_pb_graph_pin*, t_pb_graph_pin*> pb_graph_pin_map_;
};

} /* End namespace openfpga*/
