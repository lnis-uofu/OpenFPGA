#ifndef QL_MEMORY_BANK_CONFIG_SETTING_H
#define QL_MEMORY_BANK_CONFIG_SETTING_H

#include <cstdint>
#include <map>
#include <string>

struct QLMemoryBankPBSetting {
  QLMemoryBankPBSetting(uint32_t n = 0) : num_wl(n) {}
  uint32_t num_wl = 0;
};

/********************************************************************
 * A data structure to store QL Memory Bank configuration setting
 *******************************************************************/
class QLMemoryBankConfigSetting {
 public: /* Constructors */
  QLMemoryBankConfigSetting();

 public: /* Public Accessors */
  QLMemoryBankPBSetting pb_setting(const std::string& name) const;

 public: /* Public Mutators */
  void add_pb_setting(const std::string& name, uint32_t num_wl);

 private: /* Internal data */
  std::map<std::string, QLMemoryBankPBSetting> settings_;
};

#endif
