#include "ql_memory_bank_config_setting.h"

#include "openfpga_tokenizer.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/************************************************************************
 * Member functions for class QLMemoryBankConfigSetting
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
QLMemoryBankConfigSetting::QLMemoryBankConfigSetting() {}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
QLMemoryBankPBSetting QLMemoryBankConfigSetting::pb_setting(
  const std::string& name) const {
  if (settings_.find(name) != settings_.end()) {
    return settings_.at(name);
  }
  return QLMemoryBankPBSetting();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void QLMemoryBankConfigSetting::add_pb_setting(const std::string& name,
                                               uint32_t num_wl) {
  VTR_ASSERT(settings_.find(name) == settings_.end());
  settings_[name] = QLMemoryBankPBSetting(num_wl);
}
