#ifndef TECHNOLOGY_LIBRARY_H
#define TECHNOLOGY_LIBRARY_H

/********************************************************************
 * This file include the declaration of technology library
 *******************************************************************/
#include <string>
#include "technology_library_fwd.h"

/********************************************************************
 * Types for technology library attributes
 * Industrial library: the .lib file which define technology  
 *                     This is ubiquitous in commercial vendors
 *                     For industry library, we allow users to define
 *                     process corners.
 * Academia library: the .pm file which define technology 
 *                   This is mainly used in PTM-like technology library 
 * PTM is the Predictive Technology Model provided by the Arizona
 * State University (ASU). Available at ptm.asu.edu
 *******************************************************************/
enum e_tech_lib_model_type {
  TECH_LIB_MODEL_INDUSTRY,
  TECH_LIB_MODEL_ACADEMIA,
  NUM_TECH_LIB_MODEL_TYPES
};
/* Strings correspond to each technology library type */
constexpr std::array<const char*, NUM_TECH_LIB_MODEL_TYPES> TECH_LIB_MODEL_TYPE_STRING = {{"industry", "academia"}};

/********************************************************************
 * Types of device which may be defined in a technology library
 * 1. transistor 
 * 2. RRAM
 *******************************************************************/
enum e_tech_lib_device_type {
  TECH_LIB_DEVICE_TRANSISTOR, 
  TECH_LIB_DEVICE_RRAM, 
  NUM_TECH_LIB_DEVICE_TYPES
};
/* Strings correspond to transistor type */
constexpr std::array<const char*, NUM_TECH_LIB_DEVICE_TYPES> TECH_LIB_DEVICE_TYPE_STRING = {{"transistor", "rram"}};

/********************************************************************
 * Types of transistors which may be defined in a technology library
 * 1. NMOS transistor 
 * 2. PMOS transistor 
 *******************************************************************/
enum e_tech_lib_trans_type {
  TECH_LIB_TRANS_PMOS, 
  TECH_LIB_TRANS_NMOS, 
  NUM_TECH_LIB_TRANS_TYPES
};
/* Strings correspond to transistor type */
constexpr std::array<const char*, NUM_TECH_LIB_TRANS_TYPES> TECH_LIB_TRANS_TYPE_STRING = {{"pmos", "nmos"}};

/********************************************************************
 * Process corners supported 
 *******************************************************************/
enum e_process_corner {
  TECH_LIB_CORNER_FF,
  TECH_LIB_CORNER_TT,
  TECH_LIB_CORNER_SS,
  NUM_TECH_LIB_CORNER_TYPES
};
/* Strings correspond to process corner type */
constexpr std::array<const char*, NUM_TECH_LIB_CORNER_TYPES> TECH_LIB_CORNER_TYPE_STRING = {{"FF", "TT", "SS"}};

/********************************************************************
 * A data structure to describe technology library
 *******************************************************************/
class TechnologyLibrary {
  private: /* Internal data */
    /* Transistor-related fundamental information */
    vtr::vector<TechnologyDeviceId, TechnologyDeviceId> device_ids_;
    vtr::vector<TechnologyDeviceId, std::string> device_names_;
    vtr::vector<TechnologyDeviceId, e_tech_lib_device_type> device_types_;
    vtr::vector<TechnologyDeviceId, e_tech_lib_model_type> device_model_types_;
    vtr::vector<TechnologyDeviceId, std::string> device_corners_;
    vtr::vector<TechnologyDeviceId, std::string> device_model_refs_;
    vtr::vector<TechnologyDeviceId, std::string> device_lib_paths_;
    vtr::vector<TechnologyDeviceId, float> device_vdds_;
    vtr::vector<TechnologyDeviceId, float> device_pn_ratios_;

    /* Transistor models stored in vtr::Point data structure. pmos->x, nmos->y */
    vtr::vector<TechnologyDeviceId, vtr::Point<std::string>> transistor_model_names_;
    vtr::vector<TechnologyDeviceId, vtr::Point<float>> transistor_model_chan_lengths_;
    vtr::vector<TechnologyDeviceId, vtr::Point<float>> transistor_model_min_widths_;
    vtr::vector<TechnologyDeviceId, vtr::Point<std::string>> transistor_model_variation_names_;
    vtr::vector<TechnologyDeviceId, vtr::Point<TechnologyVariationId>> transistor_model_variation_ids_;

    /* ReRAM-related fundamental information */
    vtr::vector<TechnologyDeviceId, vtr::Point<float>> rram_resistances_;
    vtr::vector<TechnologyDeviceId, float> rram_variation_names_;
    vtr::vector<TechnologyDeviceId, TechnologyVariationId> rram_variation_ids_;
    
    /* Variation-related fundamental information */
    vtr::vector<TechnologyVariationId, TechnologyVariationId> variation_ids_;
    vtr::vector<TechnologyVariationId, std::string> variation_names_;
    vtr::vector<TechnologyVariationId, std::string> variation_abs_values_;
    vtr::vector<TechnologyVariationId, std::string> variation_num_sigmas_;
};

#endif
