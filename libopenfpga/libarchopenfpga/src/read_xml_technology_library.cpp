/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_technology_library.h"

/********************************************************************
 * Convert string to the enumerate of device model type
 *******************************************************************/
static 
e_tech_lib_model_type string_to_device_model_type(const std::string& type_string) {
  if (std::string("transistor") == type_string) {
    return TECH_LIB_MODEL_TRANSISTOR;
  }

  if (std::string("rram") == type_string) {
    return TECH_LIB_MODEL_RRAM;
  }

  return NUM_TECH_LIB_MODEL_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of technology library type
 *******************************************************************/
static 
e_tech_lib_type string_to_tech_lib_type(const std::string& type_string) {
  if (std::string("industry") == type_string) {
    return TECH_LIB_INDUSTRY;
  }

  if (std::string("academia") == type_string) {
    return TECH_LIB_ACADEMIA;
  }

  return NUM_TECH_LIB_TYPES;
}

/********************************************************************
 * Parse XML codes of a <lib> under a device model definiton 
 * to an object of technology library
 *******************************************************************/
static 
void read_xml_device_model_lib_settings(pugi::xml_node& xml_device_model_lib,
                                        const pugiutil::loc_data& loc_data,
                                        TechnologyLibrary& tech_lib,
                                        TechnologyModelId& device_model) {
  /* Parse the type of model library */
  const char* type_attr = get_attribute(xml_device_model_lib, "type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_tech_lib_type device_model_lib_type = string_to_tech_lib_type(std::string(type_attr));

  if (NUM_TECH_LIB_TYPES == device_model_lib_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_device_model_lib),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  tech_lib.set_model_lib_type(device_model, device_model_lib_type);

  /* Parse the type of process corner, this is ONLY applicable to industry library */
  if (TECH_LIB_INDUSTRY == tech_lib.model_lib_type(device_model)) {
    tech_lib.set_model_corner(device_model, get_attribute(xml_device_model_lib, "corner", loc_data).as_string());
  }

  /* Parse the model reference */
  tech_lib.set_model_ref(device_model, get_attribute(xml_device_model_lib, "ref", loc_data).as_string());

  /* Parse the lib path */
  tech_lib.set_model_lib_path(device_model, get_attribute(xml_device_model_lib, "path", loc_data).as_string());
}

/********************************************************************
 * Parse XML codes of design parameters under a device model
 * to an object of technology library
 *******************************************************************/
static 
void read_xml_device_model_design_settings(pugi::xml_node& xml_device_model_design,
                                           const pugiutil::loc_data& loc_data,
                                           TechnologyLibrary& tech_lib,
                                           TechnologyModelId& device_model) {
  /* Parse the vdd to be used in circuit design */
  tech_lib.set_model_vdd(device_model, get_attribute(xml_device_model_design, "vdd", loc_data).as_float(0.));

  /* Parse the width ratio between PMOS and NMOS transistor */
  tech_lib.set_model_pn_ratio(device_model, get_attribute(xml_device_model_design, "pn_ratio", loc_data).as_float(0.));
}

/********************************************************************
 * Parse XML codes of transistor models under a device model
 * to an object of technology library
 *******************************************************************/
static 
void read_xml_device_transistor(pugi::xml_node& xml_device_transistor,
                                const pugiutil::loc_data& loc_data,
                                TechnologyLibrary& tech_lib,
                                TechnologyModelId& device_model,
                                const e_tech_lib_transistor_type& transistor_type) {
  /* Parse the transistor model name */
  tech_lib.set_transistor_model_name(device_model, transistor_type, 
                                     get_attribute(xml_device_transistor, "name", loc_data).as_string());

  /* Parse the transistor channel length */
  tech_lib.set_transistor_model_chan_length(device_model, transistor_type, 
                                            get_attribute(xml_device_transistor, "chan_length", loc_data).as_float(0.));

  /* Parse the transistor minimum width */
  tech_lib.set_transistor_model_min_width(device_model, transistor_type, 
                                          get_attribute(xml_device_transistor, "min_width", loc_data).as_float(0.));

  /* Parse the transistor maximum width, by default we consider the same as minimum width */
  tech_lib.set_transistor_model_max_width(device_model, transistor_type, 
                                          get_attribute(xml_device_transistor, "max_width", loc_data, pugiutil::ReqOpt::OPTIONAL).as_float(tech_lib.transistor_model_min_width(device_model, transistor_type)));

  /* Parse the transistor variation name */
  tech_lib.set_transistor_model_variation_name(device_model, transistor_type, 
                                               get_attribute(xml_device_transistor, "variation", loc_data).as_string());
}

/********************************************************************
 * Parse XML codes of RRAM models under a device model
 * to an object of technology library
 *******************************************************************/
static 
void read_xml_device_rram(pugi::xml_node& xml_device_rram,
                          const pugiutil::loc_data& loc_data,
                          TechnologyLibrary& tech_lib,
                          TechnologyModelId& device_model) {
  /* Parse the LRS and HRS resistance */
  tech_lib.set_rram_rlrs(device_model, get_attribute(xml_device_rram, "rlrs", loc_data).as_float(0.));
  tech_lib.set_rram_rhrs(device_model, get_attribute(xml_device_rram, "rhrs", loc_data).as_float(0.));

  /* Parse the RRAM variation name */
  tech_lib.set_rram_variation_name(device_model, get_attribute(xml_device_rram, "variation", loc_data).as_string());
}

/********************************************************************
 * Parse XML codes of a <device> to an object of technology library
 *******************************************************************/
static 
void read_xml_device_model(pugi::xml_node& xml_device_model,
                           const pugiutil::loc_data& loc_data,
                           TechnologyLibrary& tech_lib) {
  /* Get the name of this device_model node and add a device to the technology library */
  TechnologyModelId device_model = tech_lib.add_model(get_attribute(xml_device_model, "name", loc_data).as_string());

  /* Find the type of device model*/
  const char* type_attr = get_attribute(xml_device_model, "type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_tech_lib_model_type device_model_type = string_to_device_model_type(std::string(type_attr));

  if (NUM_TECH_LIB_MODEL_TYPES == device_model_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_device_model),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  tech_lib.set_model_type(device_model, device_model_type);

  /* Model library -relate attributes */
  auto xml_device_model_lib = get_single_child(xml_device_model, "lib", loc_data); 
  read_xml_device_model_lib_settings(xml_device_model_lib, loc_data, tech_lib, device_model);

  /* Model design -relate attributes, this is ONLY applicable to transistor models */
  if (TECH_LIB_MODEL_TRANSISTOR == tech_lib.model_type(device_model)) {
    auto xml_device_model_design = get_single_child(xml_device_model, "design", loc_data); 
    read_xml_device_model_design_settings(xml_device_model_design, loc_data, tech_lib, device_model);
  }

  /* Transistor -relate attributes, this is ONLY applicable to transistor models */
  if (TECH_LIB_MODEL_TRANSISTOR == tech_lib.model_type(device_model)) {
    auto xml_device_model_pmos = get_single_child(xml_device_model, "pmos", loc_data); 
    read_xml_device_transistor(xml_device_model_pmos, loc_data, tech_lib, device_model, TECH_LIB_TRANSISTOR_PMOS);

    auto xml_device_model_nmos = get_single_child(xml_device_model, "nmos", loc_data); 
    read_xml_device_transistor(xml_device_model_nmos, loc_data, tech_lib, device_model, TECH_LIB_TRANSISTOR_NMOS);
  }

  /* RRAM -relate attributes, this is ONLY applicable to RRAM models */
  if (TECH_LIB_MODEL_RRAM == tech_lib.model_type(device_model)) {
    auto xml_device_rram = get_single_child(xml_device_model, "rram", loc_data); 
    read_xml_device_rram(xml_device_rram, loc_data, tech_lib, device_model);
  }
}

/********************************************************************
 * Parse XML codes of a <variation> to an object of technology library
 *******************************************************************/
static 
void read_xml_device_variation(pugi::xml_node& xml_device_variation,
                               const pugiutil::loc_data& loc_data,
                               TechnologyLibrary& tech_lib) {
  /* Get the name of this variation and add it to the technology library */
  TechnologyVariationId variation = tech_lib.add_variation(get_attribute(xml_device_variation, "name", loc_data).as_string());

  tech_lib.set_variation_abs_value(variation, get_attribute(xml_device_variation, "abs_deviation", loc_data).as_float(0.));
  tech_lib.set_variation_num_sigma(variation, get_attribute(xml_device_variation, "num_sigma", loc_data).as_int(0.));
}

/********************************************************************
 * Parse XML codes of a <device_library> to an object of technology library
 *******************************************************************/
static 
void read_xml_device_lib(pugi::xml_node& xml_device_lib,
                         const pugiutil::loc_data& loc_data,
                         TechnologyLibrary& tech_lib) {
  /* Iterate over the children under this node,
   * each child should be named after <device>
   */
  for (pugi::xml_node xml_device_model : xml_device_lib.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_device_model.name() != std::string("device_model")) {
      bad_tag(xml_device_model, loc_data, xml_device_lib, {"device_model"});
    }
    read_xml_device_model(xml_device_model, loc_data, tech_lib);
  } 
}

/********************************************************************
 * Parse XML codes of a <variation_library> to an object of technology library
 *******************************************************************/
static 
void read_xml_variation_lib(pugi::xml_node& xml_variation_lib,
                            const pugiutil::loc_data& loc_data,
                            TechnologyLibrary& tech_lib) {
  /* Iterate over the children under this node,
   * each child should be named after <variation>
   */
  for (pugi::xml_node xml_device_variation : xml_variation_lib.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_device_variation.name() != std::string("variation")) {
      bad_tag(xml_device_variation, loc_data, xml_variation_lib, {"variation"});
    }
    read_xml_device_variation(xml_device_variation, loc_data, tech_lib);
  } 
}

/********************************************************************
 * Parse XML codes about <technology_library> to an object of technology library
 *******************************************************************/
TechnologyLibrary read_xml_technology_library(pugi::xml_node& Node,
                                              const pugiutil::loc_data& loc_data) {
  TechnologyLibrary tech_lib;

  /* Iterate over the children under this node,
   * There should be only two child nodes
   * 1. device_library
   * 2. variation_library
   */
  size_t num_device_lib = count_children(Node, "device_library", loc_data);
  if (1 != num_device_lib) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(Node),
                   "Expect only 1 <device_library> defined under <technology_library>");
  }
  
  pugi::xml_node xml_device_lib = get_first_child(Node, "device_library", loc_data);
  read_xml_device_lib(xml_device_lib, loc_data, tech_lib);

  size_t num_variation_lib = count_children(Node, "variation_library", loc_data);
  if (1 != num_variation_lib) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(Node),
                   "Expect only 1 <variation_library> defined under <technology_library>");
  }

  pugi::xml_node xml_variation_lib = get_first_child(Node, "variation_library", loc_data);
  read_xml_variation_lib(xml_variation_lib, loc_data, tech_lib);

  return tech_lib;
}
