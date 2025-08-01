/******************************************************************************
 * Memember functions for data structure RRGSBWriterOption
 ******************************************************************************/
#include "rr_gsb_writer_option.h"

#include <array>
#include <map>

#include "openfpga_tokenizer.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
RRGSBWriterOption::RRGSBWriterOption() {
  output_directory_.clear();
  unique_module_only_ = false;
  exclude_content_ = {false, false, false, false};
  include_gsb_names_.clear();
  verbose_output_ = false;
  num_parse_errors_ = 0;
}

/**************************************************
 * Public Accessors
 *************************************************/
std::string RRGSBWriterOption::output_directory() const {
  return output_directory_;
}

bool RRGSBWriterOption::unique_module_only() const {
  return unique_module_only_;
}

bool RRGSBWriterOption::include_rr_info() const { return !exclude_content_[0]; }

bool RRGSBWriterOption::include_cb_content(const e_rr_type& cb_type) const {
  if (cb_type == e_rr_type::CHANX) {
    return !exclude_content_[1];
  }
  VTR_ASSERT(cb_type == e_rr_type::CHANY);
  return !exclude_content_[2];
}

bool RRGSBWriterOption::include_sb_content() const {
  return !exclude_content_[3];
}

std::vector<std::string> RRGSBWriterOption::include_gsb_names() const {
  return include_gsb_names_;
}

bool RRGSBWriterOption::verbose_output() const { return verbose_output_; }

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void RRGSBWriterOption::set_output_directory(const std::string& output_dir) {
  output_directory_ = output_dir;
}

void RRGSBWriterOption::set_unique_module_only(const bool& enabled) {
  unique_module_only_ = enabled;
}

void RRGSBWriterOption::set_exclude_rr_info(const bool& enabled) {
  exclude_content_[0] = enabled;
}

void RRGSBWriterOption::set_exclude_content(const std::string& content) {
  num_parse_errors_ = 0;
  /* Split the content using a tokenizer */
  StringToken tokenizer(content);
  std::vector<std::string> tokens = tokenizer.split(',');
  /* Parse each token */
  std::map<std::string, int> token2index = {{"sb", 3}, {"cbx", 1}, {"cby", 2}};
  for (std::string token : tokens) {
    auto result = token2index.find(token);
    if (result == token2index.end()) {
      /* Cannot find a valid keyword, error out */
      std::string keyword_list;
      for (auto pair : token2index) {
        keyword_list += pair.first + "|";
      }
      keyword_list.pop_back();
      std::string err_msg = std::string("Invalid content '") + token +
                            std::string("' to skip, expect [ ") + keyword_list +
                            std::string(" ]");
      VTR_LOG_ERROR(err_msg.c_str());
      num_parse_errors_++;
      continue;
    }
    /* Now we should have a valid keyword, assign to designated flag */
    exclude_content_[result->second] = true;
  }
}

void RRGSBWriterOption::set_include_gsb_names(const std::string& content) {
  /* Split the content using a tokenizer */
  StringToken tokenizer(content);
  include_gsb_names_ = tokenizer.split(',');
}

void RRGSBWriterOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

bool RRGSBWriterOption::valid() const {
  if (output_directory_.empty()) {
    return false;
  }
  if (num_parse_errors_) {
    return false;
  }
  return true;
}

} /* end namespace openfpga */
