/******************************************************************************
 * Memember functions for data structure BitstreamWriterOption
 ******************************************************************************/
#include "bitstream_writer_options.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
BitstreamWriterOption::BitstreamWriterOption() {
  file_type_ = BitstreamWriterOption::e_bitfile_type::NUM_TYPES;
  BITFILE_TYPE_STRING_ = {"plain_text", "xml"};
  output_file_.clear();
  time_stamp_ = true;
  verbose_output_ = false;

  filter_value_ = "";
  trim_path_ = false;
  path_only_ = false;
  value_only_ = false;

  fast_config_ = false;
  keep_dont_care_bits_ = false;
  wl_decremental_order_ = false;
}

/**************************************************
 * Public Accessors
 *************************************************/
BitstreamWriterOption::e_bitfile_type BitstreamWriterOption::output_file_type()
  const {
  return file_type_;
}

std::string BitstreamWriterOption::output_file_name() const {
  return output_file_;
}

bool BitstreamWriterOption::time_stamp() const { return time_stamp_; }

bool BitstreamWriterOption::verbose_output() const { return verbose_output_; }

bool BitstreamWriterOption::filter_value() const {
  return !filter_value_.empty();
}

bool BitstreamWriterOption::value_to_skip(const size_t& val) const {
  return std::to_string(val) == filter_value_;
}

bool BitstreamWriterOption::trim_path() const { return trim_path_; }
bool BitstreamWriterOption::output_path() const {
  if (!path_only_ && !value_only_) {
    return true;
  }
  return path_only_;
}
bool BitstreamWriterOption::output_value() const {
  if (!path_only_ && !value_only_) {
    return true;
  }
  return value_only_;
}

bool BitstreamWriterOption::fast_configuration() const { return fast_config_; }
bool BitstreamWriterOption::keep_dont_care_bits() const {
  return keep_dont_care_bits_;
}
bool BitstreamWriterOption::wl_decremental_order() const {
  return wl_decremental_order_;
}

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void BitstreamWriterOption::set_output_file_type(const std::string& val) {
  file_type_ = str2bitfile_type(val);
}

void BitstreamWriterOption::set_output_file_name(
  const std::string& output_file) {
  output_file_ = output_file;
}

void BitstreamWriterOption::set_time_stamp(const bool& enabled) {
  time_stamp_ = enabled;
}

void BitstreamWriterOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

void BitstreamWriterOption::set_filter_value(const std::string& val) {
  filter_value_ = val;
}

void BitstreamWriterOption::set_trim_path(const bool& enabled) {
  trim_path_ = enabled;
}

void BitstreamWriterOption::set_path_only(const bool& enabled) {
  path_only_ = enabled;
}

void BitstreamWriterOption::set_value_only(const bool& enabled) {
  value_only_ = enabled;
}

void BitstreamWriterOption::set_fast_configuration(const bool& enabled) {
  fast_config_ = enabled;
}

void BitstreamWriterOption::set_keep_dont_care_bits(const bool& enabled) {
  keep_dont_care_bits_ = enabled;
}

void BitstreamWriterOption::set_wl_decremental_order(const bool& enabled) {
  wl_decremental_order_ = enabled;
}

bool BitstreamWriterOption::validate(bool show_err_msg) const {
  /* Check file type */
  if (!valid_file_type(file_type_)) {
    VTR_LOGV_ERROR(show_err_msg, "Invalid file type!\n");
    return false;
  }
  if (output_file_.empty()) {
    VTR_LOGV_ERROR(show_err_msg, "Empty file name!\n");
    return false;
  }
  if (file_type_ == BitstreamWriterOption::e_bitfile_type::XML) {
    /* All the options in the XML format should be off */
    if (path_only_ && value_only_) {
      VTR_LOGV_ERROR(show_err_msg,
                     "Both path and value are specifed as only inputs! If "
                     "specified, please define one of them\n");
      return false;
    }
    if (!filter_value_.empty() && (filter_value_ != std::to_string(0) &&
                                   filter_value_ != std::to_string(1))) {
      VTR_LOGV_ERROR(show_err_msg,
                     "Invalid value '%s' for filter values. Expect [0|1]!\n",
                     filter_value_.c_str());
      return false;
    }
  }
  return true;
}

BitstreamWriterOption::e_bitfile_type BitstreamWriterOption::str2bitfile_type(
  const std::string& type_str, const bool& verbose) const {
  for (int itype = size_t(BitstreamWriterOption::e_bitfile_type::TEXT);
       itype != size_t(BitstreamWriterOption::e_bitfile_type::NUM_TYPES);
       ++itype) {
    if (type_str == std::string(BITFILE_TYPE_STRING_[itype])) {
      return static_cast<BitstreamWriterOption::e_bitfile_type>(itype);
    }
  }
  VTR_LOGV_ERROR(verbose, "Invalid type for bitstream file! Expect %s\n",
                 bitfile_type_all2str().c_str());
  return BitstreamWriterOption::e_bitfile_type::NUM_TYPES;
}

std::string BitstreamWriterOption::bitfile_type2str(
  const BitstreamWriterOption::e_bitfile_type& type,
  const bool& verbose) const {
  if (!valid_file_type(type)) {
    VTR_LOGV_ERROR(verbose, "Invalid type for bitstream file! Expect %s\n",
                   bitfile_type_all2str().c_str());
    return std::string();
  }
  return std::string(BITFILE_TYPE_STRING_[size_t(type)]);
}

std::string BitstreamWriterOption::bitfile_type_all2str() const {
  std::string full_types = "[";
  for (int itype = size_t(BitstreamWriterOption::e_bitfile_type::TEXT);
       itype != size_t(BitstreamWriterOption::e_bitfile_type::NUM_TYPES);
       ++itype) {
    full_types += std::string(BITFILE_TYPE_STRING_[itype]) + std::string("|");
  }
  full_types.pop_back();
  full_types += "]";
  return full_types;
}

bool BitstreamWriterOption::valid_file_type(
  const BitstreamWriterOption::e_bitfile_type& bitfile_type) const {
  return bitfile_type != BitstreamWriterOption::e_bitfile_type::NUM_TYPES;
}

} /* end namespace openfpga */
