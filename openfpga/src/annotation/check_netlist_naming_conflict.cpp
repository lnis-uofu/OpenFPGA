/********************************************************************
 * This file includes functions to detect and correct any naming
 * in the users' BLIF netlist that violates the syntax of OpenFPGA
 * fabric generator, i.e., Verilog generator and SPICE generator
 *******************************************************************/
#include <fstream>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from archopenfpga library */
#include "write_xml_utils.h"

/* Headers from openfpgautil library */
#include "check_netlist_naming_conflict.h"
#include "openfpga_digest.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to check if the name contains any of the
 * sensitive characters in the list
 * Return a string of sensitive characters which are contained
 * in the name
 *******************************************************************/
static std::string name_contain_sensitive_chars(
  const std::string& name, const std::string& sensitive_chars) {
  std::string violation;

  for (const char& sensitive_char : sensitive_chars) {
    /* Return true since we find a characters */
    if (std::string::npos != name.find(sensitive_char)) {
      violation.push_back(sensitive_char);
    }
  }

  return violation;
}

/********************************************************************
 * This function aims to fix up a name that contains any of the
 * sensitive characters in the list
 * Return a string the fixed name
 *******************************************************************/
static std::string fix_name_contain_sensitive_chars(
  const std::string& name, const std::string& sensitive_chars,
  const std::string& fix_chars) {
  std::string fixed_name = name;

  VTR_ASSERT(sensitive_chars.length() == fix_chars.length());

  for (size_t ichar = 0; ichar < sensitive_chars.length(); ++ichar) {
    /* Keep fixing the characters until we cannot find anymore */
    std::string::size_type pos = 0u;
    while (std::string::npos !=
           (pos = fixed_name.find(sensitive_chars[ichar], pos))) {
      fixed_name.replace(pos, 1, std::string(1, fix_chars[ichar]));
      pos += 1;
    }
  }

  return fixed_name;
}

/********************************************************************
 * Detect and report any naming conflict by checking a list of
 * sensitive characters
 * - Iterate over all the blocks and see if any block name contain
 *   any sensitive character
 * - Iterate over all the nets and see if any net name contain
 *   any sensitive character
 *******************************************************************/
size_t detect_netlist_naming_conflict(const AtomNetlist& atom_netlist,
                                      const std::string& sensitive_chars) {
  size_t num_conflicts = 0;

  /* Walk through blocks in the netlist */
  for (const auto& block : atom_netlist.blocks()) {
    const std::string& block_name = atom_netlist.block_name(block);
    const std::string& violation =
      name_contain_sensitive_chars(block_name, sensitive_chars);
    if (false == violation.empty()) {
      VTR_LOG("Block '%s' contains illegal characters '%s'\n",
              block_name.c_str(), violation.c_str());
      num_conflicts++;
    }
  }

  /* Walk through nets in the netlist */
  for (const auto& net : atom_netlist.nets()) {
    const std::string& net_name = atom_netlist.net_name(net);
    const std::string& violation =
      name_contain_sensitive_chars(net_name, sensitive_chars);
    if (false == violation.empty()) {
      VTR_LOG("Net '%s' contains illegal characters '%s'\n", net_name.c_str(),
              violation.c_str());
      num_conflicts++;
    }
  }

  return num_conflicts;
}

/********************************************************************
 * Correct and report any naming conflict by checking a list of
 * sensitive characters
 * - Iterate over all the blocks and correct any block name that contains
 *   any sensitive character
 * - Iterate over all the nets and correct any net name that contains
 *   any sensitive character
 *******************************************************************/
void fix_netlist_naming_conflict(const AtomNetlist& atom_netlist,
                                 const std::string& sensitive_chars,
                                 const std::string& fix_chars,
                                 VprNetlistAnnotation& vpr_netlist_annotation) {
  size_t num_fixes = 0;

  /* Walk through blocks in the netlist */
  for (const auto& block : atom_netlist.blocks()) {
    const std::string& block_name = atom_netlist.block_name(block);
    const std::string& violation =
      name_contain_sensitive_chars(block_name, sensitive_chars);

    if (false == violation.empty()) {
      /* Apply fix-up here */
      vpr_netlist_annotation.rename_block(
        block, fix_name_contain_sensitive_chars(block_name, sensitive_chars,
                                                fix_chars));
      num_fixes++;
    }
  }

  /* Walk through nets in the netlist */
  for (const auto& net : atom_netlist.nets()) {
    const std::string& net_name = atom_netlist.net_name(net);
    const std::string& violation =
      name_contain_sensitive_chars(net_name, sensitive_chars);
    if (false == violation.empty()) {
      /* Apply fix-up here */
      vpr_netlist_annotation.rename_net(
        net,
        fix_name_contain_sensitive_chars(net_name, sensitive_chars, fix_chars));
      num_fixes++;
    }
  }

  if (0 < num_fixes) {
    VTR_LOG("Fixed %ld naming conflicts in the netlist.\n", num_fixes);
  }
}

/********************************************************************
 * Report all the fix-up in the naming of netlist components,
 * i.e., blocks, nets
 *******************************************************************/
void print_netlist_naming_fix_report(
  const std::string& fname, const AtomNetlist& atom_netlist,
  const VprNetlistAnnotation& vpr_netlist_annotation) {
  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname.c_str(), fp);

  fp << "<!-- Netlist naming fix-up report --> "
     << "\n";
  fp << "<netlist>"
     << "\n";

  fp << "\t"
     << "<blocks>"
     << "\n";

  for (const auto& block : atom_netlist.blocks()) {
    const std::string& block_name = atom_netlist.block_name(block);
    if (false == vpr_netlist_annotation.is_block_renamed(block)) {
      continue;
    }
    fp << "\t\t"
       << "<block";
    write_xml_attribute(fp, "previous", block_name.c_str());
    write_xml_attribute(fp, "current",
                        vpr_netlist_annotation.block_name(block).c_str());
    fp << "/>"
       << "\n";
  }

  fp << "\t"
     << "</blocks>"
     << "\n";

  fp << "\t"
     << "<nets>"
     << "\n";

  for (const auto& net : atom_netlist.nets()) {
    const std::string& net_name = atom_netlist.net_name(net);
    if (false == vpr_netlist_annotation.is_net_renamed(net)) {
      continue;
    }
    fp << "\t\t"
       << "<net";
    write_xml_attribute(fp, "previous", net_name.c_str());
    write_xml_attribute(fp, "current",
                        vpr_netlist_annotation.net_name(net).c_str());
    fp << "/>"
       << "\n";
  }

  fp << "\t"
     << "</nets>"
     << "\n";

  fp << "</netlist>"
     << "\n";

  /* Close the file stream */
  fp.close();
}

} /* end namespace openfpga */
