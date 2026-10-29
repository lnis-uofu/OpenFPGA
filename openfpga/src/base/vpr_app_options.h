#ifndef VPR_APP_OPTIONS_H
#define VPR_APP_OPTIONS_H

#include <string>
#include <vector>

#include "app_options.h"
#include "read_options.h"

/********************************************************************
 * Give the application options to VPR.
 *
 * VPR reads its options with read_options(), which infers some defaults
 * from the options that were specified, e.g., the placement algorithm from
 * the use of timing. To get exactly the setup that VPR would build, the
 * options are given to read_options() as a command line, as if they were
 * typed by a user. Only the options whose value is not the default are
 * given, so that VPR infers what it would infer for them.
 *
 * Note that read_options() exits the program when it is given an option it
 * rejects. The application options are checked against their type first,
 * but a text that is valid for the type may still be rejected by VPR, e.g.,
 * a block type that does not exist in a list of pin utilizations
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* The command line of VPR for the application options, with the name of the
 * program first. The positional arguments come first: the architecture
 * (filename.arch_file) and the circuit (filename.circuit_name, or the name of
 * filename.circuit_file without its extension). Return false, with the reason
 * in error, if the architecture or the circuit are not set */
bool vpr_args_from_app_options(const AppOptions& options,
                               std::vector<std::string>& args,
                               std::string& error);

/* Read the options of VPR from a command line, as made by
 * vpr_args_from_app_options(). VPR exits the program on an invalid command
 * line */
t_options read_vpr_options(const std::vector<std::string>& args);

} /* End namespace openfpga */

#endif
