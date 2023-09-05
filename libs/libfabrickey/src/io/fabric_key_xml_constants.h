#ifndef FABRIC_KEY_XML_CONSTANTS_H
#define FABRIC_KEY_XML_CONSTANTS_H

namespace openfpga {  // Begin namespace openfpga

/* Constants required by XML parser */
constexpr const char* XML_FABRIC_KEY_ROOT_NAME = "fabric_key";
constexpr const char* XML_FABRIC_KEY_MODULE_NODE_NAME = "module";
constexpr const char* XML_FABRIC_KEY_MODULE_ATTRIBUTE_NAME_NAME = "name";
constexpr const char* XML_FABRIC_KEY_REGION_NODE_NAME = "region";
constexpr const char* XML_FABRIC_KEY_REGION_ATTRIBUTE_ID_NAME = "id";
constexpr const char* XML_FABRIC_KEY_KEY_NODE_NAME = "key";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_ID_NAME = "id";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_ALIAS_NAME = "alias";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_NAME_NAME = "name";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_VALUE_NAME = "value";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_COLUMN_NAME = "column";
constexpr const char* XML_FABRIC_KEY_KEY_ATTRIBUTE_ROW_NAME = "row";
constexpr const char* XML_FABRIC_KEY_BL_SHIFT_REGISTER_BANKS_NODE_NAME =
  "bl_shift_register_banks";
constexpr const char* XML_FABRIC_KEY_WL_SHIFT_REGISTER_BANKS_NODE_NAME =
  "wl_shift_register_banks";
constexpr const char* XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME =
  "bank";
constexpr const char*
  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_ID_NAME = "id";
constexpr const char*
  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_RANGE_NAME = "range";

}  // End of namespace openfpga

#endif
