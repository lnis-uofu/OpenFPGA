#ifndef TECHNOLOGY_LIBRARY_H
#define TECHNOLOGY_LIBRARY_H

/********************************************************************
 * This file include the declaration of technology library
 *******************************************************************/
#include <string>
#include <map>
#include <array>

/* Headers from vtrutil library */
#include "vtr_vector.h"
#include "vtr_geometry.h"

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
  public: /* Types */
    typedef vtr::vector<TechnologyDeviceId, TechnologyDeviceId>::const_iterator technology_device_iterator;
    typedef vtr::vector<TechnologyVariationId, TechnologyVariationId>::const_iterator technology_variation_iterator;
    /* Create range */
    typedef vtr::Range<technology_device_iterator> technology_device_range;
    typedef vtr::Range<technology_variation_iterator> technology_variation_range;
  public:  /* Constructors */
    TechnologyLibrary();
  public: /* Accessors: aggregates */
    technology_device_range devices() const;
    technology_variation_range variations() const;
    std::vector<TechnologyDeviceId> devices_by_type(const enum e_tech_lib_device_type& type) const;
  public: /* Public Accessors: Basic data query on devices */
    std::string device_name(const TechnologyDeviceId& device_id) const;
    TechnologyDeviceId device(const std::string& name) const;
    enum e_tech_lib_device_type device_type(const TechnologyDeviceId& device_id) const;
    enum e_tech_lib_model_type device_model_type(const TechnologyDeviceId& device_id) const;
    std::string device_corner(const TechnologyDeviceId& device_id) const;
    std::string device_model_ref(const TechnologyDeviceId& device_id) const;
    std::string device_lib_path(const TechnologyDeviceId& device_id) const;
    float device_vdd(const TechnologyDeviceId& device_id) const;
    float device_pn_ratio(const TechnologyDeviceId& device_id) const;
  public: /* Public Accessors: Basic data query on transistors */
    std::string transistor_model_name(const TechnologyDeviceId& device_id,
                                      const e_tech_lib_trans_type& transistor_type) const;
    float transistor_model_chan_length(const TechnologyDeviceId& device_id,
                                       const e_tech_lib_trans_type& transistor_type) const;
    float transistor_model_min_width(const TechnologyDeviceId& device_id,
                                     const e_tech_lib_trans_type& transistor_type) const;
    TechnologyVariationId transistor_model_variation(const TechnologyDeviceId& device_id,
                                                     const e_tech_lib_trans_type& transistor_type) const;
  public: /* Public Accessors: Basic data query on RRAM devices */
    float rram_rlrs(const TechnologyDeviceId& device_id) const;
    float rram_rhrs(const TechnologyDeviceId& device_id) const;
    TechnologyVariationId rram_variation(const TechnologyDeviceId& device_id) const;
  public: /* Public Accessors: Basic data query on variations */
    std::string variation_name(const TechnologyVariationId& variation_id) const;
    TechnologyVariationId variation(const std::string& name) const;
    float variation_abs_value(const TechnologyVariationId& variation_id) const;
    size_t variation_num_sigma(const TechnologyVariationId& variation_id) const;
  public: /* Public Mutators: device-related */
    TechnologyDeviceId add_device(const std::string& name);
    void set_device_type(const TechnologyDeviceId& device_id, 
                         const e_tech_lib_device_type& type);
    void set_device_model_type(const TechnologyDeviceId& device_id, 
                               const e_tech_lib_model_type& model_type);
    void set_device_corner(const TechnologyDeviceId& device_id, 
                           const std::string& corner);
    void set_device_model_ref(const TechnologyDeviceId& device_id, 
                              const std::string& model_ref);
    void set_device_lib_path(const TechnologyDeviceId& device_id, 
                             const std::string& lib_path);
    void set_device_vdd(const TechnologyDeviceId& device_id, 
                        const float& vdd);
    void set_device_pn_ratio(const TechnologyDeviceId& device_id, 
                             const float& pn_ratio);
  public: /* Public Mutators: transistor-related */
    void set_transistor_model_name(const TechnologyDeviceId& device_id, 
                                   const e_tech_lib_trans_type& transistor_type,
                                   const std::string& model_name);
    void set_transistor_model_chan_length(const TechnologyDeviceId& device_id, 
                                          const e_tech_lib_trans_type& transistor_type,
                                          const float& chan_length);
    void set_transistor_model_min_width(const TechnologyDeviceId& device_id, 
                                        const e_tech_lib_trans_type& transistor_type,
                                        const float& min_width);
    void set_transistor_model_variation_name(const TechnologyDeviceId& device_id, 
                                             const e_tech_lib_trans_type& transistor_type,
                                             const std::string& variation_name);
  public: /* Public Mutators: RRAM-related */
    void set_rram_rlrs(const TechnologyDeviceId& device_id, 
                       const float& rlrs);
    void set_rram_rhrs(const TechnologyDeviceId& device_id, 
                       const float& rhrs);
    void set_rram_variation_name(const TechnologyDeviceId& device_id, 
                                 const std::string& variation_name);
  public: /* Public Mutators: variation-related */
    TechnologyVariationId add_variation(const std::string& name);
    void set_variation_abs_value(const TechnologyVariationId& variation_id, const float& abs_value);
    void set_variation_num_sigma(const TechnologyVariationId& variation_id, const size_t& num_sigma);
  public: /* Public invalidators/validators */
    bool valid_device_id(const TechnologyDeviceId& device_id) const;
    bool valid_variation_id(const TechnologyVariationId& variation_id) const;
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

    /* Transistor models stored in vtr::Point data structure */
    vtr::vector<TechnologyDeviceId, std::array<std::string, 2>> transistor_model_names_;
    vtr::vector<TechnologyDeviceId, std::array<float, 2>> transistor_model_chan_lengths_;
    vtr::vector<TechnologyDeviceId, std::array<float, 2>> transistor_model_min_widths_;
    vtr::vector<TechnologyDeviceId, std::array<std::string, 2>> transistor_model_variation_names_;
    vtr::vector<TechnologyDeviceId, std::array<TechnologyVariationId, 2>> transistor_model_variation_ids_;

    /* ReRAM-related fundamental information: LRS -> x(); HRS -> y() */
    vtr::vector<TechnologyDeviceId, vtr::Point<float>> rram_resistances_;
    vtr::vector<TechnologyDeviceId, std::string> rram_variation_names_;
    vtr::vector<TechnologyDeviceId, TechnologyVariationId> rram_variation_ids_;
    
    /* Variation-related fundamental information */
    vtr::vector<TechnologyVariationId, TechnologyVariationId> variation_ids_;
    vtr::vector<TechnologyVariationId, std::string> variation_names_;
    vtr::vector<TechnologyVariationId, float> variation_abs_values_;
    vtr::vector<TechnologyVariationId, size_t> variation_num_sigmas_;

    /* Fast name-to-id lookup */
    std::map<std::string, TechnologyDeviceId> device_name2ids_;
    std::map<std::string, TechnologyVariationId> variation_name2ids_;
};

#endif
