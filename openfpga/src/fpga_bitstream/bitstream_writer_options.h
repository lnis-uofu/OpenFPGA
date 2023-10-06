#ifndef BITSTREAM_WRITER_OPTIONS_H
#define BITSTREAM_WRITER_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <array>
#include <string>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for Bitstream Writer
 *******************************************************************/
class BitstreamWriterOption {
 public: /* Private data structures */
  /* A type to define the bitstream file format */
  enum class e_bitfile_type { TEXT, XML, NUM_TYPES };

 public: /* Public constructor */
  /* Set default options */
  BitstreamWriterOption();

 public: /* Public accessors */
  e_bitfile_type output_file_type() const;
  std::string output_file_name() const;
  bool time_stamp() const;
  bool verbose_output() const;

  /* Check if a filter on value is applied */
  bool filter_value() const;
  /* Check if a given value should be skipped */
  bool value_to_skip(const size_t& val) const;
  /* Check if path trimming should be applied or not */
  bool trim_path() const;
  /* Check if path should be outputted in the resulting file */
  bool output_path() const;
  /* Check if value should be outputted in the resulting file */
  bool output_value() const;

  bool fast_configuration() const;
  bool keep_dont_care_bits() const;
  bool wl_decremental_order() const;

 public: /* Public mutators */
  void set_output_file_type(const std::string& val);
  void set_output_file_name(const std::string& output_file);
  void set_time_stamp(const bool& enabled);
  void set_verbose_output(const bool& enabled);
  void set_trim_path(const bool& enabled);
  void set_path_only(const bool& enabled);
  void set_value_only(const bool& enabled);
  void set_fast_configuration(const bool& enabled);
  void set_keep_dont_care_bits(const bool& enabled);
  void set_wl_decremental_order(const bool& enabled);

  void set_filter_value(const std::string& val);

 public: /* Public validator */
  bool validate(bool show_err_msg = false) const;

 public: /* Internal utility */
  /** @brief Parse the file type from string to valid type. Parser
   * error can be turned on */
  e_bitfile_type str2bitfile_type(const std::string& type_str,
                                  const bool& verbose = false) const;

  /** @brief Output the string representing file_type */
  std::string bitfile_type2str(const e_bitfile_type& type,
                               const bool& verbose = false) const;
  /** @brief Validate the file_type */
  bool valid_file_type(const e_bitfile_type& bitfile_type) const;

  /* Generate a string include all the valid style
   * Useful for printing debugging messages */
  std::string bitfile_type_all2str() const;

 private: /* Internal Data */
  /* Universal options */
  e_bitfile_type file_type_;
  std::string output_file_;
  bool time_stamp_;
  bool verbose_output_;

  /* XML-specific options */
  std::string filter_value_;
  bool trim_path_;
  bool path_only_;
  bool value_only_;

  /* Plain-text options */
  bool fast_config_;
  bool keep_dont_care_bits_;
  bool wl_decremental_order_;

  /* Constants */
  std::array<const char*, size_t(e_bitfile_type::NUM_TYPES)>
    BITFILE_TYPE_STRING_;
};

} /* End namespace openfpga*/

#endif
