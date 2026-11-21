#ifndef VPR_READ_OPTIONS_H
#define VPR_READ_OPTIONS_H

#include "read_options.h"

/********************************************************************
 * Read the command line of VPR without ending the process.
 *
 * VPR's read_options() parses with parse_args(), which calls std::exit()
 * after --help, after --version and on any option it rejects. In the shell,
 * and above all in Python, that ends the program (the interpreter) without
 * a message to the caller. These functions do what read_options() does with
 * the parts that VPR exposes (create_arg_parser(), set_conditional_defaults()
 * and verify_args()), but parse with parse_args_throw()
 *******************************************************************/
/* Begin namespace vpr */
namespace vpr {

/* What the command line asks for */
enum class e_vpr_options_request {
  RUN,     /* The options are read: run VPR with them */
  HELP,    /* --help: the help was printed, VPR is not to be run */
  VERSION, /* --version: the version was printed, VPR is not to be run */
};

/* Read the options of VPR from a command line (argv[0] is the name of the
 * program), as read_options() does. Throw a VprError when the command line is
 * rejected, instead of exiting */
e_vpr_options_request read_options_no_exit(int argc, const char** argv,
                                           t_options& options);

/* vpr_init() of VPR, with read_options_no_exit(): set up the logging, read
 * the options and, unless the command line asks for the help or the version,
 * the architecture and the circuit. Return false when VPR is not to be run.
 * Throw a VprError when the command line is rejected */
bool vpr_init_no_exit(int argc, const char** argv, t_options* options,
                      t_vpr_setup* vpr_setup, t_arch* arch);

} /* End namespace vpr */

#endif
