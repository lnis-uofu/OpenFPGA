#ifndef REPACK_OPTION_H
#define REPACK_OPTION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <vector>

#include "repack_design_constraints.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for RRGSB writer
 *******************************************************************/
class RepackOption {
 public: /* Public constructor */
  /* Set default options */
  RepackOption();

 public: /* Public accessors */
  RepackDesignConstraints design_constraints() const;
  /* Identify if a pin should ignore all the global nets */
  bool is_pin_ignore_global_nets(const std::string& pb_type_name,
                                 const BasicPort& pin) const;
  bool net_is_specified_to_be_ignored(std::string cluster_net_name,
                                      std::string pb_type_name,
                                      const BasicPort& pin) const;
  bool verbose_output() const;

 public: /* Public mutators */
  void set_design_constraints(
    const RepackDesignConstraints& design_constraints);
  void set_ignore_global_nets_on_pins(const std::string& content);
  void set_verbose_output(const bool& enabled);

 public: /* Public validators */
  /* Check if the following internal data is valid or not:
   * - no parsing errors
   */
  bool valid() const;

 private: /* Internal Data */
  RepackDesignConstraints design_constraints_;
  /* The pin information on which global nets should be mapped to:
   * [pb_type_name][0..num_ports] For example:
   * - clb.I[0:1], clb.I[5:6] -> ["clb"][BasicPort(I, 0, 1), BasicPort(I, 5, 6)]
   * - clb.I[0:1], clb.I[2:6] -> ["clb"][BasicPort(I, 0, 6)]
   */
  std::map<std::string, std::vector<BasicPort>> ignore_global_nets_on_pins_;

  bool verbose_output_;

  /* A flag to indicate if the data parse is invalid or not */
  int num_parse_errors_;
};

} /* End namespace openfpga*/

#endif
