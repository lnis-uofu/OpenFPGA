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
TechnologyLibrary::technology_model_range TechnologyLibrary::models() const {
  return vtr::make_range(model_ids_.begin(), model_ids_.end());
}

TechnologyLibrary::technology_variation_range TechnologyLibrary::variations() const {
  return vtr::make_range(variation_ids_.begin(), variation_ids_.end());
}

/* Find technology models in the same type (defined by users) and return a list of ids */
std::vector<TechnologyModelId> TechnologyLibrary::models_by_type(const enum e_tech_lib_model_type& type) const {
  std::vector<TechnologyModelId> type_ids;
  for (auto id : models()) {
    /* Skip unmatched types */
    if (type != model_type(id)) {
      continue;
    }
    /* Matched type, update the vector */
    type_ids.push_back(id);
  }
  return type_ids;
}

/************************************************************************
 * Public Accessors : Basic data query on technology models
 ***********************************************************************/
/* Access the name of a technology model */
std::string TechnologyLibrary::model_name(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_names_[model_id]; 
}

/* Access the id of a technology model by name,
 * If the name is valid, we return a valid id
 * Otherwise, return an invalid id
 */
TechnologyModelId TechnologyLibrary::model(const std::string& name) const {
  std::map<std::string, TechnologyModelId>::const_iterator it = model_name2ids_.find(name);
  if (it == model_name2ids_.end()) {
    return TechnologyModelId::INVALID();
  }

  return model_name2ids_.at(name); 
}

/* Access the type of a technology model */
enum e_tech_lib_model_type TechnologyLibrary::model_type(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_types_[model_id]; 
}

/* Access the model type of a technology model */
enum e_tech_lib_type TechnologyLibrary::model_lib_type(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_lib_types_[model_id]; 
}

/* Access the process corner name of a technology model */
std::string TechnologyLibrary::model_corner(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_corners_[model_id]; 
}

/* Access the model reference name of a technology model */
std::string TechnologyLibrary::model_ref(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_refs_[model_id]; 
}

/* Access the path of library for a technology model */
std::string TechnologyLibrary::model_lib_path(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  return model_lib_paths_[model_id]; 
}

/* Access the VDD of a technology model
 * Note: This is ONLY applicable to transistor model 
 */
float TechnologyLibrary::model_vdd(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return model_vdds_[model_id]; 
}

/* Access the width ratio between PMOS and NMOS for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
float TechnologyLibrary::model_pn_ratio(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return model_pn_ratios_[model_id]; 
}

/************************************************************************
 * Public Accessors : Basic data query on transistors
 ***********************************************************************/
/* Access the model name of a transistor (either PMOS or NMOS) for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
std::string TechnologyLibrary::transistor_model_name(const TechnologyModelId& model_id,
                                                     const e_tech_lib_transistor_type& transistor_type) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return transistor_model_names_[model_id][transistor_type]; 
}

/* Access the channel length of a transistor (either PMOS or NMOS) for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
float TechnologyLibrary::transistor_model_chan_length(const TechnologyModelId& model_id,
                                                      const e_tech_lib_transistor_type& transistor_type) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return transistor_model_chan_lengths_[model_id][transistor_type]; 
}

/* Access the minimum width of a transistor (either PMOS or NMOS) for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
float TechnologyLibrary::transistor_model_min_width(const TechnologyModelId& model_id,
                                                    const e_tech_lib_transistor_type& transistor_type) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return transistor_model_min_widths_[model_id][transistor_type]; 
}

/* Access the maximum width of a transistor (either PMOS or NMOS) for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
float TechnologyLibrary::transistor_model_max_width(const TechnologyModelId& model_id,
                                                    const e_tech_lib_transistor_type& transistor_type) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return transistor_model_max_widths_[model_id][transistor_type]; 
}

/* Access the minimum width of a transistor (either PMOS or NMOS) for a technology model
 * Note: This is ONLY applicable to transistor model 
 */
TechnologyVariationId TechnologyLibrary::transistor_model_variation(const TechnologyModelId& model_id,
                                                                    const e_tech_lib_transistor_type& transistor_type) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  return transistor_model_variation_ids_[model_id][transistor_type]; 
}

/************************************************************************
 * Public Accessors : Basic data query on RRAM models
 ***********************************************************************/
/* Access the Low Resistence of a RRAM for a technology model
 * Note: This is ONLY applicable to RRAM model 
 */
float TechnologyLibrary::rram_rlrs(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  return rram_resistances_[model_id].x(); 
}

/* Access the High Resistence of a RRAM for a technology model
 * Note: This is ONLY applicable to RRAM model 
 */
float TechnologyLibrary::rram_rhrs(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to transistor model */
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  return rram_resistances_[model_id].y(); 
}

/* Access the Variation id of a RRAM for a technology model
 * Note: This is ONLY applicable to RRAM model 
 */
TechnologyVariationId TechnologyLibrary::rram_variation(const TechnologyModelId& model_id) const {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  /* This is only applicable to RRAM model */
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  return rram_variation_ids_[model_id]; 
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

/* Access the id of a technology variation by name
 * If the name is valid, we return a valid id
 * Otherwise, return an invalid id
 */
TechnologyVariationId TechnologyLibrary::variation(const std::string& name) const {
  std::map<std::string, TechnologyVariationId>::const_iterator it = variation_name2ids_.find(name);
  if (it != variation_name2ids_.end()) {
    return TechnologyVariationId::INVALID();
  }

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
 * Public Mutators: model-related
 ***********************************************************************/
/* Add a new model to the library, return an id 
 * This function will check if the name has already been used inside the data structure.
 * If used, it will return an invalid id 
 */
TechnologyModelId TechnologyLibrary::add_model(const std::string& name) {
  std::map<std::string, TechnologyModelId>::iterator it = model_name2ids_.find(name);
  if (it != model_name2ids_.end()) {
    return TechnologyModelId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  TechnologyModelId model = TechnologyModelId(model_ids_.size());
  model_ids_.push_back(model);
  model_names_.push_back(name);
  model_types_.emplace_back(NUM_TECH_LIB_MODEL_TYPES);
  model_lib_types_.emplace_back(NUM_TECH_LIB_TYPES);
  model_corners_.emplace_back();
  model_refs_.emplace_back();
  model_lib_paths_.emplace_back();
  model_vdds_.push_back(-1);
  model_pn_ratios_.push_back(-1);

  transistor_model_names_.emplace_back();
  transistor_model_chan_lengths_.emplace_back();
  transistor_model_min_widths_.emplace_back();
  transistor_model_max_widths_.emplace_back();
  transistor_model_variation_names_.emplace_back();
  transistor_model_variation_ids_.push_back(std::array<TechnologyVariationId, 2>{TechnologyVariationId::INVALID(), TechnologyVariationId::INVALID()});

  rram_resistances_.emplace_back();
  rram_variation_names_.emplace_back();
  rram_variation_ids_.push_back(TechnologyVariationId::INVALID());

  /* Register in the name-to-id map */
  model_name2ids_[name] = model;
  
  return model;
}

/* Set the model type of a model in the library */
void TechnologyLibrary::set_model_type(const TechnologyModelId& model_id, 
                                        const e_tech_lib_model_type& type) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  model_types_[model_id] = type;
  return;
}

/* Set the model type of a device model in the library */
void TechnologyLibrary::set_model_lib_type(const TechnologyModelId& model_id, 
                                           const e_tech_lib_type& model_lib_type) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  model_lib_types_[model_id] = model_lib_type;
  return;
}

/* Set the process corner of a model in the library */
void TechnologyLibrary::set_model_corner(const TechnologyModelId& model_id, 
                                          const std::string& corner) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  model_corners_[model_id] = corner;
  return;
}

/* Set the string used to model reference of a model in the library */
void TechnologyLibrary::set_model_ref(const TechnologyModelId& model_id, 
                                      const std::string& model_ref) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  model_refs_[model_id] = model_ref;
  return;
}

/* Set the library file path of a model in the library */
void TechnologyLibrary::set_model_lib_path(const TechnologyModelId& model_id, 
                                           const std::string& lib_path) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  model_lib_paths_[model_id] = lib_path;
  return;
}

/* Set the operating voltage of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_model_vdd(const TechnologyModelId& model_id, 
                                       const float& vdd) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  model_vdds_[model_id] = vdd;
  return;
}

/* Set the width ratio between PMOS and NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_model_pn_ratio(const TechnologyModelId& model_id, 
                                            const float& pn_ratio) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  model_pn_ratios_[model_id] = pn_ratio;
  return;
}

/************************************************************************
 * Public Mutators: transistor-related
 ***********************************************************************/
/* Set the model name for either PMOS or NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_transistor_model_name(const TechnologyModelId& model_id, 
                                                  const e_tech_lib_transistor_type& transistor_type,
                                                  const std::string& model_name) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  transistor_model_names_[model_id][transistor_type] = model_name;
  return;
}

/* Set the channel length for either PMOS or NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_transistor_model_chan_length(const TechnologyModelId& model_id, 
                                                         const e_tech_lib_transistor_type& transistor_type,
                                                         const float& chan_length) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  transistor_model_chan_lengths_[model_id][transistor_type] = chan_length;
  return;
}

/* Set the minimum width for either PMOS or NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_transistor_model_min_width(const TechnologyModelId& model_id, 
                                                       const e_tech_lib_transistor_type& transistor_type,
                                                       const float& min_width) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  transistor_model_min_widths_[model_id][transistor_type] = min_width;
  return;
}

/* Set the maximum width for either PMOS or NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_transistor_model_max_width(const TechnologyModelId& model_id, 
                                                       const e_tech_lib_transistor_type& transistor_type,
                                                       const float& max_width) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  transistor_model_max_widths_[model_id][transistor_type] = max_width;
  return;
}

/* Set the variation name for either PMOS or NMOS of a model in the library 
 * This is ONLY applicable to transistors
 */
void TechnologyLibrary::set_transistor_model_variation_name(const TechnologyModelId& model_id, 
                                                            const e_tech_lib_transistor_type& transistor_type,
                                                            const std::string& variation_name) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_TRANSISTOR == model_type(model_id));
  transistor_model_variation_names_[model_id][transistor_type] = variation_name;
  return;
}

/************************************************************************
 * Public Mutators: RRAM-related
 ***********************************************************************/
/* Set the Low Resistance State (LRS) resistance for a RRAM model in the library 
 * This is ONLY applicable to RRAM models
 */
void TechnologyLibrary::set_rram_rlrs(const TechnologyModelId& model_id, 
                                      const float& rlrs) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  rram_resistances_[model_id].set_x(rlrs);
  return;
}

/* Set the High Resistance State (HRS) resistance for a RRAM model in the library 
 * This is ONLY applicable to RRAM models
 */
void TechnologyLibrary::set_rram_rhrs(const TechnologyModelId& model_id, 
                                      const float& rhrs) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  rram_resistances_[model_id].set_y(rhrs);
  return;
}

/* Set the variation name for a RRAM model in the library 
 * This is ONLY applicable to RRAM models
 */
void TechnologyLibrary::set_rram_variation_name(const TechnologyModelId& model_id, 
                                                const std::string& variation_name) {
  /* validate the model_id */
  VTR_ASSERT(valid_model_id(model_id));
  VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model_id));
  rram_variation_names_[model_id] = variation_name;
  return;
}

/************************************************************************
 * Public Mutators: variation-related
 ***********************************************************************/
/* Add a new variation to the library, return an id 
 * This function will check if the name has already been used inside the data structure.
 * If used, it will return an invalid id 
 */
TechnologyVariationId TechnologyLibrary::add_variation(const std::string& name) {
  std::map<std::string, TechnologyVariationId>::iterator it = variation_name2ids_.find(name);
  if (it != variation_name2ids_.end()) {
    return TechnologyVariationId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  TechnologyVariationId variation = TechnologyVariationId(variation_ids_.size());
  variation_ids_.push_back(variation);
  variation_names_.push_back(name);
  variation_abs_values_.push_back(0.);
  variation_num_sigmas_.push_back(size_t(-1));

  /* Register in the name-to-id map */
  variation_name2ids_[name] = variation;
  
  return variation;
}

/* Set the absolute value of a variation */
void TechnologyLibrary::set_variation_abs_value(const TechnologyVariationId& variation_id, const float& abs_value) {
  /* validate the variation_id */
  VTR_ASSERT(valid_variation_id(variation_id));
  variation_abs_values_[variation_id] = abs_value;
  return;
}

/* Set the number of sigma of a variation */
void TechnologyLibrary::set_variation_num_sigma(const TechnologyVariationId& variation_id, const size_t& num_sigma) {
  /* validate the variation_id */
  VTR_ASSERT(valid_variation_id(variation_id));
  variation_num_sigmas_[variation_id] = num_sigma;
  return;
}

/************************************************************************
 * Public mutators: linkers
 ***********************************************************************/
/* This function builds the links between models and variations,
 * which have been defined in the technology library
 */
void TechnologyLibrary::link_models_to_variations() {
  for (const TechnologyModelId& model : models()) {
    /* For transistors, find the variation name for each model and build a link */
    if (TECH_LIB_MODEL_TRANSISTOR == model_type(model)) {
      /* PMOS transistor, if a variation name is specified, we try to build a link
       * Otherwise, we assign any invalid id */
      const std::string& pmos_var_name = transistor_model_variation_names_[model][TECH_LIB_TRANSISTOR_PMOS];
      transistor_model_variation_ids_[model][TECH_LIB_TRANSISTOR_PMOS] = variation(pmos_var_name);

      /* NMOS transistor, if a variation name is specified, we try to build a link
       * Otherwise, we assign any invalid id 
       */
      const std::string& nmos_var_name = transistor_model_variation_names_[model][TECH_LIB_TRANSISTOR_NMOS];
      transistor_model_variation_ids_[model][TECH_LIB_TRANSISTOR_NMOS] = variation(nmos_var_name);
      /* Finish for transistors, go to the next */
      continue;
    } 

    /* Reach here it means an RRAM model, we find the variation name and try to build a link */ 
    VTR_ASSERT(TECH_LIB_MODEL_RRAM == model_type(model));
    const std::string& rram_var_name = rram_variation_names_[model];
    rram_variation_ids_[model] = variation(rram_var_name);
    /* Finish for RRAMs, go to the next */
  }
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool TechnologyLibrary::valid_model_id(const TechnologyModelId& model_id) const {
  return ( size_t(model_id) < model_ids_.size() ) && ( model_id == model_ids_[model_id] ); 
}

bool TechnologyLibrary::valid_variation_id(const TechnologyVariationId& variation_id) const {
  return ( size_t(variation_id) < variation_ids_.size() ) && ( variation_id == variation_ids_[variation_id] ); 
}
