#pragma once

#include <string_view>

#include "pugixml_util.hpp"

namespace openfpga {

pugiutil::loc_data load_xml(pugi::xml_document& doc, std::string_view filename);

}  // namespace openfpga
