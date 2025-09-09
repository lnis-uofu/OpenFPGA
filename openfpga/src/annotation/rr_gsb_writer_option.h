#ifndef RR_GSB_WRITER_OPTION_H
#define RR_GSB_WRITER_OPTION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <vector>

#include "rr_node_types.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for RRGSB writer
 *******************************************************************/
class RRGSBWriterOption {
 public: /* Public constructor */
  /* Set default options */
  RRGSBWriterOption();

 public: /* Public accessors */
  std::string output_directory() const;
  bool unique_module_only() const;
  bool include_rr_info() const;
  bool include_cb_content(const e_rr_type& cb_type) const;
  bool include_sb_content() const;
  std::vector<std::string> include_gsb_names() const;
  bool verbose_output() const;

 public: /* Public mutators */
  void set_output_directory(const std::string& output_dir);
  void set_unique_module_only(const bool& enabled);
  void set_exclude_rr_info(const bool& enabled);
  /* Parse the string which contains the content to be excluded
   * Accepted string format is [sb|cbx|cby]
   * Allow the use ',' as splitter
   * For example: sb,cby
   */
  void set_exclude_content(const std::string& content);
  void set_include_gsb_names(const std::string& gsb_names);
  void set_verbose_output(const bool& enabled);

 public: /* Public validators */
  /* Check if the following internal data is valid or not:
   * - output directory is assigned
   * - no parsing errors
   */
  bool valid() const;

 private: /* Internal Data */
  std::string output_directory_;
  bool unique_module_only_;
  /* Flags to mark what content to be skipped when outputting:
   * 0 : rr_info
   * 1 : cbx
   * 2 : cby
   * 3 : sb
   */
  std::array<bool, 4> exclude_content_;

  std::vector<std::string> include_gsb_names_;
  bool verbose_output_;

  /* A flag to indicate if the data parse is invalid or not */
  int num_parse_errors_;
};

} /* End namespace openfpga*/

#endif
