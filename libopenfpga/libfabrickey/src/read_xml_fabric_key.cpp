/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of a fabric key to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_fabric_key.h"

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static 
void read_xml_component_key(pugi::xml_node& xml_component_key,
                            const pugiutil::loc_data& loc_data,
                            FabricKey& fabric_key) {

  /* Find the id of component key */
  const size_t& id = get_attribute(xml_component_key, "id", loc_data).as_int();
  const std::string& name = get_attribute(xml_component_key, "name", loc_data).as_string();
  const size_t& value = get_attribute(xml_component_key, "value", loc_data).as_int();
  
  if (false == fabric_key.valid_key_id(FabricKeyId(id))) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_component_key),
                   "Invalid 'id' attribute '%d'\n",
                   id);
  }

  VTR_ASSERT_SAFE(true == fabric_key.valid_key_id(FabricKeyId(id)));

  fabric_key.set_key_name(FabricKeyId(id), name);
  fabric_key.set_key_value(FabricKeyId(id), value);
}

/********************************************************************
 * Parse XML codes about <fabric> to an object of FabricKey
 *******************************************************************/
FabricKey read_xml_fabric_key(const char* key_fname) {

  vtr::ScopedStartFinishTimer timer("Read Fabric Key");

  FabricKey fabric_key;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, key_fname);

    pugi::xml_node xml_root = get_single_child(doc, "fabric_key", loc_data);

    size_t num_keys = std::distance(xml_root.children().begin(), xml_root.children().end());
    fabric_key.reserve_keys(num_keys);
    for (size_t ikey = 0; ikey < num_keys; ++ikey) {
      fabric_key.create_key();
    }

    /* Iterate over the children under this node,
     * each child should be named after circuit_model
     */
    for (pugi::xml_node xml_key : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_key.name() != std::string("key")) {
        bad_tag(xml_key, loc_data, xml_root, {"key"});
      }
      read_xml_component_key(xml_key, loc_data, fabric_key);
    } 
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(key_fname, e.line(),
                   "%s", e.what());
  }

  return fabric_key; 
}

