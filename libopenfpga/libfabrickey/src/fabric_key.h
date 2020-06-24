#ifndef FABRIC_KEY_H
#define FABRIC_KEY_H

/********************************************************************
 * This file include the declaration of fabric key
 *******************************************************************/
#include <string>
#include <map>
#include <array>

/* Headers from vtrutil library */
#include "vtr_vector.h"

#include "fabric_key_fwd.h"

/********************************************************************
 * A data structure to describe a secure key for fabric organization
 *
 * Typical usage:
 * --------------
 *   // Create an empty fabric key
 *   FabricKey fabric_key;
 *   // Add a key with name and value
 *   FabricKeyId key = fabic_key.create_key(key_name, key_value);
 *
 *******************************************************************/
class FabricKey {
  public: /* Types */
    typedef vtr::vector<FabricKeyId, FabricKeyId>::const_iterator fabric_key_iterator;
    /* Create range */
    typedef vtr::Range<fabric_key_iterator> fabric_key_range;
  public:  /* Constructors */
    FabricKey();
  public: /* Accessors: aggregates */
    fabric_key_range keys() const;
  public: /* Public Accessors: Basic data query */
    std::string key_name(const FabricKeyId& key_id) const;
    size_t key_value(const FabricKeyId& key_id) const;
    bool empty() const;
  public: /* Public Mutators: model-related */
    void reserve_keys(const size_t& num_keys);
    FabricKeyId create_key();
    void set_key_name(const FabricKeyId& key_id,
                      const std::string& name);
    void set_key_value(const FabricKeyId& key_id,
                       const size_t& value);
  public: /* Public invalidators/validators */
    bool valid_key_id(const FabricKeyId& key_id) const;
  private: /* Internal data */
    /* Unique ids for each key */
    vtr::vector<FabricKeyId, FabricKeyId> key_ids_;

    /* Names for each key */
    vtr::vector<FabricKeyId, std::string> key_names_;

    /* Values for each key */
    vtr::vector<FabricKeyId, size_t> key_values_;
};

#endif
