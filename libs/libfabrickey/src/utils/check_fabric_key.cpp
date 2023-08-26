/************************************************************************
 * Check functions for the content of fabric key to avoid conflicts with
 * other data structures
 * These functions are not universal methods for the FabricKey class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/
#include "check_fabric_key.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/** @brief Sanity checks for fabric key alias attribute:
 * - Each alias should NOT be empty
 * - Each alias should be defined only once!
 */
int check_fabric_key_alias(const FabricKey& input_key, const bool& verbose) {
  /* Check each key now */
  size_t num_errors = 0;
  float progress = 0.;
  size_t num_keys_checked = 0;

  std::map<std::string, size_t> alias_count;
  for (FabricKeyId key_id : input_key.keys()) {
    /* Note that this is slow. May consider to build a map first */
    std::string curr_alias = input_key.key_alias(key_id);
    progress = static_cast<float>(num_keys_checked) /
               static_cast<float>(input_key.num_keys()) * 100.0;
    VTR_LOGV(verbose, "[%lu%] Checking key alias '%s'\r", size_t(progress),
             curr_alias.c_str());
    if (curr_alias.empty()) {
      VTR_LOG_ERROR(
        "Empty key alias (id='%lu') found in keys which is invalid!\n",
        size_t(key_id));
      num_errors++;
    }
    auto result = alias_count.find(curr_alias);
    if (result == alias_count.end()) {
      alias_count[curr_alias] = 0;
    } else {
      alias_count[curr_alias] += 1;
    }
    num_keys_checked++;
  }
  for (const auto& kv : alias_count) {
    if (kv.second > 1) {
      std::string key_id_str;
      std::vector<FabricKeyId> found_keys =
        input_key.find_key_by_alias(kv.first);
      for (FabricKeyId found_key_id : found_keys) {
        key_id_str += std::to_string(size_t(found_key_id)) + ",";
      }
      key_id_str.pop_back(); /* Remove last comma */
      VTR_LOG_ERROR(
        "Duplicated key alias '%s' found %lu times in keys (ids: %s), which is "
        "invalid!\n",
        kv.first.c_str(), kv.second, key_id_str.c_str());
      num_errors++;
    }
  }

  return num_errors;
}

/** @brief Sanity checks for fabric key name and value attribute:
 * - Each name should not be empty
 * - Each value should be larger than zero !
 */
int check_fabric_key_names_and_values(const FabricKey& input_key,
                                      const bool& verbose) {
  /* Check each key now */
  size_t num_errors = 0;
  float progress = 0.;
  size_t num_keys_checked = 0;

  std::map<std::string, std::map<size_t, size_t>> key_value_count;
  for (FabricKeyId key_id : input_key.keys()) {
    /* Note that this is slow. May consider to build a map first */
    std::string curr_name = input_key.key_name(key_id);
    size_t curr_value = input_key.key_value(key_id);
    progress = static_cast<float>(num_keys_checked) /
               static_cast<float>(input_key.num_keys()) * 100.0;
    VTR_LOGV(verbose, "[%lu%] Checking key names and values '(%s, %lu)'\r",
             size_t(progress), curr_name.c_str(), curr_value);
    if (curr_name.empty()) {
      VTR_LOG_ERROR(
        "Empty key name (id='%lu') found in keys which is invalid!\n",
        size_t(key_id));
      num_errors++;
    }
    auto result = key_value_count[curr_name].find(curr_value);
    if (result == key_value_count[curr_name].end()) {
      key_value_count[curr_name][curr_value] = 0;
    } else {
      key_value_count[curr_name][curr_value] += 1;
    }
    num_keys_checked++;
  }
  for (const auto& key_name_kv : key_value_count) {
    for (const auto& key_value_kv : key_name_kv.second) {
      if (key_value_kv.second > 1) {
        VTR_LOG_ERROR(
          "Duplicated key name and value pair (%s, %lu) found %lu times in "
          "keys, which is invalid!\n",
          key_name_kv.first.c_str(), key_value_kv.first, key_value_kv.second);
        num_errors++;
      }
    }
  }

  return num_errors;
}

} /* end namespace openfpga */
