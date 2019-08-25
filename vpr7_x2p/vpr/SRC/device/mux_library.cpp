/**************************************************
 * This file includes member functions for the 
 * data structures in mux_library.h
 *************************************************/

#include "vtr_assert.h"

#include "mux_library.h"

/**************************************************
 * Member functions for the class MuxLibrary
 *************************************************/

/**************************************************
 * Public accessors: aggregates
 *************************************************/
MuxLibrary::mux_range MuxLibrary::muxes() const {
  return vtr::make_range(mux_ids_.begin(), mux_ids_.end());
}

/**************************************************
 * Public accessors: data query
 *************************************************/
/* Get a MUX graph (read-only) */
MuxId MuxLibrary::mux_graph(const CircuitModelId& circuit_model, 
                            const size_t& mux_size) const {
  /* Make sure we have a valid mux look-up */
  VTR_ASSERT_SAFE(valid_mux_lookup());
  /* Validate circuit model id and mux_size */
  VTR_ASSERT_SAFE(valid_mux_size(circuit_model, mux_size));

  return mux_lookup_[circuit_model][mux_size];
}

const MuxGraph& MuxLibrary::mux_graph(const MuxId& mux_id) const {
  VTR_ASSERT_SAFE(valid_mux_id(mux_id));
  return mux_graphs_[mux_id];
}

/* Get a mux circuit model id */
CircuitModelId MuxLibrary::mux_circuit_model(const MuxId& mux_id) const {
  VTR_ASSERT_SAFE(valid_mux_id(mux_id));
  return mux_circuit_models_[mux_id];
}

/**************************************************
 * Private mutators:
 *************************************************/
/* Add a mux to the library */
void MuxLibrary::add_mux(const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model, const size_t& mux_size) {
  /* First, check if there is already an existing graph */
  if (valid_mux_size(circuit_model, mux_size)) {
    return;
  }

  /* create a new id for the mux */
  MuxId mux = MuxId(mux_ids_.size());
  /* Push to the node list */
  mux_ids_.push_back(mux);
  /* Add a mux graph */
  mux_graphs_.push_back(MuxGraph(circuit_lib, circuit_model, mux_size));
  /* Recorde mux cirucit model id */
  mux_circuit_models_.push_back(circuit_model);

  /* update mux_lookup*/
  mux_lookup_[circuit_model][mux_size] = mux;
} 

/**************************************************
 * Private accessors: validator and invalidators
 *************************************************/
bool MuxLibrary::valid_mux_id(const MuxId& mux) const {
  return size_t(mux) < mux_ids_.size() && mux_ids_[mux] == mux;
}

bool MuxLibrary::valid_mux_lookup() const {
  return mux_lookup_.empty();
}

bool MuxLibrary::valid_mux_circuit_model_id(const CircuitModelId& circuit_model) const {
  MuxLookup::iterator it = mux_lookup_.find(circuit_model);
  return (it != mux_lookup_.end());
}

bool MuxLibrary::valid_mux_size(const CircuitModelId& circuit_model, const size_t& mux_size) const {
  if (false == valid_mux_circuit_model_id(circuit_model)) {
    return false;
  }
  std::map<size_t, MuxId>::iterator it = mux_lookup_[circuit_model].find(mux_size);
  return (it != mux_lookup_[circuit_model].end());
}

/**************************************************
 * Private mutators: validator and invalidators
 *************************************************/

/* Build fast node lookup */
void MuxLibrary::build_mux_lookup() {
  /* Invalidate the mux lookup if necessary */
  invalidate_mux_lookup();
}

/* Invalidate (empty) the mux fast lookup*/
void MuxLibrary::invalidate_mux_lookup() {
  mux_lookup_.clear();
}


/**************************************************
 * End of Member functions for the class MuxLibrary
 *************************************************/



