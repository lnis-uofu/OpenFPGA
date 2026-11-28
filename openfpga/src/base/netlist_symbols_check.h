#ifndef NETLIST_SYMBOLS_CHECK_H
#define NETLIST_SYMBOLS_CHECK_H

#include <string>

/********************************************************************
 * The check of a file of netlist symbols (docs/plan_netlist_symbols.md,
 * section 2), the type netlist_symbols of a device package: an SVG file in a
 * subset of the format of netlistsvg. The web app compiles the file; this
 * check refuses, when the package is read, what the app would refuse:
 * elements and attributes outside the subset (scripts, styles, images, links,
 * events, colours on shapes), transforms other than translate and scale,
 * roles with no colours, colours without both light and dark values,
 * symbols with no alias, hidden symbols that draw something, and layout
 * options other than the spacings and the direction.
 *
 * That a hidden symbol is one of the default symbols is checked by the app,
 * which has them.
 *******************************************************************/
namespace openfpga {

/* The first problem of the text of a file, with its line, or "" */
std::string check_netlist_symbols(const std::string& text);

} /* end namespace openfpga */

#endif
