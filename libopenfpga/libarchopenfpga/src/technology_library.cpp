#include "vtr_assert.h"

#include "technology_library.h"

/************************************************************************
 * Member functions for class TechnologyLibrary
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
TechnologyLibrary::TechnologyLibrary() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
TechnologyLibrary::technology_device_range TechnologyLibrary::devices() const {
  return vtr::make_range(device_ids_.begin(), device_ids_.end());
}

TechnologyLibrary::technology_variation_range TechnologyLibrary::variations() const {
  return vtr::make_range(variation_ids_.begin(), variation_ids_.end());
}

/* Find technology devices in the same type (defined by users) and return a list of ids */
std::vector<TechnologyDeviceId> TechnologyLibrary::devices_by_type(const enum e_tech_lib_device_type& type) const {
  std::vector<TechnologyDeviceId> type_ids;
  for (auto id : devices()) {
    /* Skip unmatched types */
    if (type != device_type(id)) {
      continue;
    }
    /* Matched type, update the vector */
    type_ids.push_back(id);
  }
  return type_ids;
}

/************************************************************************
 * Public Accessors : Basic data query on technology devices
 ***********************************************************************/
/* Access the name of a technology device */
std::string TechnologyLibrary::device_name(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_names_[device_id]; 
}

/* Access the id of a technology device by name */
TechnologyDeviceId TechnologyLibrary::device(const std::string& name) const {
  return device_name2ids_.at(name); 
}

/* Access the type of a technology device */
enum e_tech_lib_device_type TechnologyLibrary::device_type(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_types_[device_id]; 
}

/* Access the model type of a technology device */
enum e_tech_lib_model_type TechnologyLibrary::device_model_type(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_model_types_[device_id]; 
}

/* Access the process corner name of a technology device */
std::string TechnologyLibrary::device_corner(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_corners_[device_id]; 
}

/* Access the model reference name of a technology device */
std::string TechnologyLibrary::device_model_ref(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_model_refs_[device_id]; 
}

/* Access the path of library for a technology device */
std::string TechnologyLibrary::device_lib_path(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  return device_lib_paths_[device_id]; 
}

/* Access the VDD of a technology device
 * Note: This is ONLY applicable to transistor device 
 */
float TechnologyLibrary::device_vdd(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return device_vdds_[device_id]; 
}

/* Access the width ratio between PMOS and NMOS for a technology device
 * Note: This is ONLY applicable to transistor device 
 */
float TechnologyLibrary::device_pn_ratio(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return device_pn_ratios_[device_id]; 
}

/************************************************************************
 * Public Accessors : Basic data query on transistors
 ***********************************************************************/
/* Access the model name of a transistor (either PMOS or NMOS) for a technology device
 * Note: This is ONLY applicable to transistor device 
 */
std::string TechnologyLibrary::transistor_model_name(const TechnologyDeviceId& device_id,
                                                     const e_tech_lib_trans_type& transistor_type) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return transistor_model_names_[device_id][transistor_type]; 
}

/* Access the channel length of a transistor (either PMOS or NMOS) for a technology device
 * Note: This is ONLY applicable to transistor device 
 */
float TechnologyLibrary::transistor_model_chan_length(const TechnologyDeviceId& device_id,
                                                      const e_tech_lib_trans_type& transistor_type) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return transistor_model_chan_lengths_[device_id][transistor_type]; 
}

/* Access the minimum width of a transistor (either PMOS or NMOS) for a technology device
 * Note: This is ONLY applicable to transistor device 
 */
float TechnologyLibrary::transistor_model_min_width(const TechnologyDeviceId& device_id,
                                                    const e_tech_lib_trans_type& transistor_type) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return transistor_model_min_widths_[device_id][transistor_type]; 
}

/* Access the minimum width of a transistor (either PMOS or NMOS) for a technology device
 * Note: This is ONLY applicable to transistor device 
 */
TechnologyVariationId TechnologyLibrary::transistor_model_variation(const TechnologyDeviceId& device_id,
                                                                    const e_tech_lib_trans_type& transistor_type) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_TRANSISTOR == device_type(device_id));
  return transistor_model_variation_ids_[device_id][transistor_type]; 
}

/************************************************************************
 * Public Accessors : Basic data query on RRAM devices
 ***********************************************************************/
/* Access the Low Resistence of a RRAM for a technology device
 * Note: This is ONLY applicable to RRAM device 
 */
float TechnologyLibrary::rram_rlrs(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_RRAM == device_type(device_id));
  return rram_resistances_[device_id].x(); 
}

/* Access the High Resistence of a RRAM for a technology device
 * Note: This is ONLY applicable to RRAM device 
 */
float TechnologyLibrary::rram_rhrs(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_RRAM == device_type(device_id));
  return rram_resistances_[device_id].y(); 
}

/* Access the Variation id of a RRAM for a technology device
 * Note: This is ONLY applicable to RRAM device 
 */
TechnologyVariationId TechnologyLibrary::rram_variation(const TechnologyDeviceId& device_id) const {
  /* validate the device_id */
  VTR_ASSERT(valid_device_id(device_id));
  /* This is only applicable to transistor device */
  VTR_ASSERT(TECH_LIB_DEVICE_RRAM == device_type(device_id));
  return rram_variation_ids_[device_id]; 
}

/************************************************************************
 * Public Accessors : Basic data query on technology variations
 ***********************************************************************/
/* Access the name of a technology variation */
std::string TechnologyLibrary::variation_name(const TechnologyVariationId& variation_id) const {
  /* validate the variation_id */
  VTR_ASSERT(valid_variation_id(variation_id));
  return variation_names_[variation_id]; 
}

/* Access the id of a technology variation by name */
TechnologyVariationId TechnologyLibrary::variation(const std::string& name) const {
  return variation_name2ids_.at(name); 
}

/* Access the abs value of a technology variation */
float TechnologyLibrary::variation_abs_value(const TechnologyVariationId& variation_id) const {
  /* validate the variation_id */
  VTR_ASSERT(valid_variation_id(variation_id));
  return variation_abs_values_[variation_id]; 
}

/* Access the abs value of a technology variation */
size_t TechnologyLibrary::variation_num_sigma(const TechnologyVariationId& variation_id) const {
  /* validate the variation_id */
  VTR_ASSERT(valid_variation_id(variation_id));
  return variation_num_sigmas_[variation_id]; 
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool TechnologyLibrary::valid_device_id(const TechnologyDeviceId& device_id) const {
  return ( size_t(device_id) < device_ids_.size() ) && ( device_id == device_ids_[device_id] ); 
}

bool TechnologyLibrary::valid_variation_id(const TechnologyVariationId& variation_id) const {
  return ( size_t(variation_id) < variation_ids_.size() ) && ( variation_id == variation_ids_[variation_id] ); 
}
