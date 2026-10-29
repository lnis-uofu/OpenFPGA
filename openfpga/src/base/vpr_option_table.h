#ifndef VPR_OPTION_TABLE_H
#define VPR_OPTION_TABLE_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "app_options.h"
#include "read_options.h"

/********************************************************************
 * The application options that are options of VPR, and how they are given
 * to VPR. The table is generated from the sources of VPR by
 * openfpga_flow/scripts/generate_vpr_option_mapping.py: see there why, and
 * run the script again after changing VTR
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* An option of VPR */
struct VprOptionSpec {
  const char* group; /* Prefix of the name of the option, e.g., place */
  const char*
    name; /* Name in the group, e.g., seed. The option is place.seed */
  AppOptionValue::e_type type;
  const char* field; /* The field of t_options, e.g., seed */
  const char* flag; /* The flag of VPR on its command line, e.g., --seed. A name
                     * without dashes is a positional argument */
  bool store_true;  /* A switch without value: it can only be turned on */
  bool multi_value; /* Takes several values */
  bool derived_default; /* VPR derives it from the circuit when it is not given:
                         * the default is empty */
  const char* help;
  /* The values a selection accepts, with the enumeration they stand for */
  const std::map<std::string, int>* choices;
  /* Copy the value of the option from the options read by VPR */
  std::function<void(const t_options&, AppOptionValue&)> get;
};

/* A field of t_options that has no application option, and why */
struct VprFieldNotInTable {
  const char* field;
  const char* flag;
  const char* category;
  const char* reason;
};

/* All the options of VPR that have an application option */
const std::vector<VprOptionSpec>& vpr_option_specs();

/* All the fields of t_options that have no application option */
const std::vector<VprFieldNotInTable>& vpr_fields_not_in_table();

} /* End namespace openfpga */

#endif
