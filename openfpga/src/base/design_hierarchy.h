#ifndef DESIGN_HIERARCHY_H
#define DESIGN_HIERARCHY_H

/********************************************************************
 * The hierarchy of a design, from the JSON that Yosys writes after
 * 'hierarchy -top' (write_json): the tree of the instances of the modules,
 * and what each module is (command report_design_hierarchy)
 *******************************************************************/
#include <cstddef>
#include <string>

#include "nlohmann/json.hpp"

/* begin namespace openfpga */
namespace openfpga {

/* The tree of a design, from the JSON of Yosys (its 'modules'), to give in
 * the answer of report_design_hierarchy:
 * - modules: each module of the tree (name, module: the name of the RTL,
 *   parameters, kind: module, library or unknown, instances in the whole
 *   tree, cells: the cells of Yosys in one instance, src), most instances
 *   first
 * - tree: the top and its instances, breadth first, limit nodes at most; a
 *   node whose children are not all given says how many are not in more
 * - nodes (given), instances (in the whole tree, top included), truncated
 * Throws std::runtime_error when top is not a module of the JSON */
nlohmann::ordered_json hierarchy_of(const nlohmann::json& yosys,
                                    const std::string& top, size_t limit);

/* A name of a module of Yosys as the RTL has it, and its parameters: the
 * derived module $paramod\sub\W=s32'00000000000000000000000000001000 is sub,
 * with W 8 */
nlohmann::ordered_json module_name(const std::string& name);

} /* end namespace openfpga */

#endif
