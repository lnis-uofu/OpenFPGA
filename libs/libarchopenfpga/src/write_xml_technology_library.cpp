/********************************************************************
 * This file includes functions that outputs a technology library to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_technology_library.h"

/********************************************************************
 * A writer to output a device model in a technology library to XML format
 *******************************************************************/
static 
void write_xml_device_model(std::fstream& fp,
                            const char* fname,
                            const TechnologyLibrary& tech_lib,
                            const TechnologyModelId& device_model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<device_model";

  /* Write up name and type of the device model */
  write_xml_attribute(fp, "name", tech_lib.model_name(device_model).c_str());
  write_xml_attribute(fp, "type", TECH_LIB_MODEL_TYPE_STRING[tech_lib.model_type(device_model)]);

  fp << ">" << "\n";

  /* Write library settings */
  fp << "\t\t\t\t" << "<lib";

  write_xml_attribute(fp, "type", TECH_LIB_TYPE_STRING[tech_lib.model_lib_type(device_model)]);

  if (TECH_LIB_INDUSTRY == tech_lib.model_lib_type(device_model)) {
    write_xml_attribute(fp, "corner", tech_lib.model_corner(device_model).c_str());
  }

  write_xml_attribute(fp, "ref", tech_lib.model_ref(device_model).c_str());
  write_xml_attribute(fp, "path", tech_lib.model_lib_path(device_model).c_str());

  fp << ">" << "\n";

  /* Write design parameters. This is ONLY applicable to transistors */
  if (TECH_LIB_MODEL_TRANSISTOR == tech_lib.model_type(device_model)) {
    fp << "\t\t\t\t" << "<design";
    write_xml_attribute(fp, "vdd", std::to_string(tech_lib.model_vdd(device_model)).c_str());
    write_xml_attribute(fp, "pn_ratio", std::to_string(tech_lib.model_pn_ratio(device_model)).c_str());
    fp << ">" << "\n";
  }

  /* Write PMOS and NMOS. This is ONLY applicable to transistors */
  if (TECH_LIB_MODEL_TRANSISTOR == tech_lib.model_type(device_model)) {
    fp << "\t\t\t\t" << "<pmos";
    write_xml_attribute(fp, "name", tech_lib.transistor_model_name(device_model, TECH_LIB_TRANSISTOR_PMOS).c_str());
    write_xml_attribute(fp, "chan_length", tech_lib.transistor_model_chan_length(device_model, TECH_LIB_TRANSISTOR_PMOS));
    write_xml_attribute(fp, "min_width", tech_lib.transistor_model_min_width(device_model, TECH_LIB_TRANSISTOR_PMOS));
    write_xml_attribute(fp, "max_width", tech_lib.transistor_model_max_width(device_model, TECH_LIB_TRANSISTOR_PMOS));
    if (TechnologyVariationId::INVALID() != tech_lib.transistor_model_variation(device_model, TECH_LIB_TRANSISTOR_PMOS)) {
      write_xml_attribute(fp, "variation", tech_lib.variation_name(tech_lib.transistor_model_variation(device_model, TECH_LIB_TRANSISTOR_PMOS)).c_str());
    }
    fp << "/>" << "\n";

    fp << "\t\t\t\t" << "<nmos";
    write_xml_attribute(fp, "name", tech_lib.transistor_model_name(device_model, TECH_LIB_TRANSISTOR_NMOS).c_str());
    write_xml_attribute(fp, "chan_length", tech_lib.transistor_model_chan_length(device_model, TECH_LIB_TRANSISTOR_NMOS));
    write_xml_attribute(fp, "min_width", tech_lib.transistor_model_min_width(device_model, TECH_LIB_TRANSISTOR_NMOS));
    if (TechnologyVariationId::INVALID() != tech_lib.transistor_model_variation(device_model, TECH_LIB_TRANSISTOR_NMOS)) {
      write_xml_attribute(fp, "variation", tech_lib.variation_name(tech_lib.transistor_model_variation(device_model, TECH_LIB_TRANSISTOR_NMOS)).c_str());
    }
    fp << "/>" << "\n";
  }

  /* Write RRAM device parameters. This is ONLY applicable to RRAM */
  if (TECH_LIB_MODEL_RRAM == tech_lib.model_type(device_model)) {
    fp << "\t\t\t\t" << "<rram";

    write_xml_attribute(fp, "rlrs", tech_lib.rram_rlrs(device_model));
    write_xml_attribute(fp, "rhrs", tech_lib.rram_rhrs(device_model));

    if (TechnologyVariationId::INVALID() != tech_lib.rram_variation(device_model)) {
      write_xml_attribute(fp, "variation", tech_lib.variation_name(tech_lib.rram_variation(device_model)).c_str());
    }
    fp << "/>" << "\n";
  }

  /* Finished XML dumping for this device model */
  fp << "\t\t\t" << "</device_model>" << "\n";
}

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static 
void write_xml_device_variation(std::fstream& fp,
                                const char* fname,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyVariationId& device_variation) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<variation";

  /* Write up name of the device variation */
  write_xml_attribute(fp, "name", tech_lib.variation_name(device_variation).c_str());
  write_xml_attribute(fp, "abs_deviation", tech_lib.variation_abs_value(device_variation));
  write_xml_attribute(fp, "num_sigma", std::to_string(tech_lib.variation_num_sigma(device_variation)).c_str());

  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output a technology library to XML format
 * Note: 
 * This function should be run after that the following methods of 
 * TechnologyLibrary are executed 
 * 1. link_models_to_variations();
 *******************************************************************/
void write_xml_technology_library(std::fstream& fp,
                                  const char* fname,
                                  const TechnologyLibrary& tech_lib) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node for technology_library, 
   * we apply a tab becuase technology library is a subnode 
   * under the root node <openfpga_arch>
   */
  fp << "\t" << "<technology_library>" << "\n";

  /* Write device library node */ 
  fp << "\t\t" << "<device_library>" << "\n";
  
  /* Write device model one by one */ 
  for (const TechnologyModelId& device_model : tech_lib.models()) {
    write_xml_device_model(fp, fname, tech_lib, device_model);
  }

  /* Finish writing device library node */ 
  fp << "\t\t" << "</device_library>" << "\n";

  /* Write variation library node */ 
  fp << "\t\t" << "<variation_library>" << "\n";

  /* Write variation model one by one */ 
  for (const TechnologyVariationId& variation : tech_lib.variations()) {
    write_xml_device_variation(fp, fname, tech_lib, variation);
  }

  /* Finish writing variation library node */ 
  fp << "\t\t" << "</variation_library>" << "\n";

  /* Write the root node for circuit_library */
  fp << "\t" << "</technology_library>" << "\n";
}
