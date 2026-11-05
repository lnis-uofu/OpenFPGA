#include "vpr_read_options.h"

#include "argparse.hpp"
#include "argparse_error.hpp"
#include "vpr_api.h"
#include "vpr_error.h"

/* Begin namespace vpr */
namespace vpr {

e_vpr_options_request read_options_no_exit(int argc, const char** argv,
                                           t_options& options) {
  options = t_options();
  /* The parser writes into options: options must outlive it */
  argparse::ArgumentParser parser = create_arg_parser(argv[0], options);

  try {
    parser.parse_args_throw(argc, argv);
  } catch (const argparse::ArgParseHelp&) {
    parser.print_help();
    return e_vpr_options_request::HELP;
  } catch (const argparse::ArgParseVersion&) {
    parser.print_version();
    return e_vpr_options_request::VERSION;
  } catch (const argparse::ArgParseError& error) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER,
                    "Invalid command line of VPR: %s (see 'vpr --help')",
                    error.what());
  }

  set_conditional_defaults(options);
  verify_args(options);
  return e_vpr_options_request::RUN;
}

bool vpr_init_no_exit(int argc, const char** argv, t_options* options,
                      t_vpr_setup* vpr_setup, t_arch* arch) {
  vpr_initialize_logging();
  vpr_print_title();

  if (read_options_no_exit(argc, argv, *options) !=
      e_vpr_options_request::RUN) {
    return false;
  }

  /* As vpr_init(): the command line in the log, to reproduce the run */
  vpr_print_args(argc, argv);
  vpr_init_with_options(options, vpr_setup, arch);
  return true;
}

} /* End namespace vpr */
