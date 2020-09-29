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
 * A fabric may consist of multiple regions
 * Each region contains a number of keys
 * 
 * Note that:
 *   - each key can only be defined in one unique region 
 *
 * Typical usage:
 * --------------
 *   // Create an empty fabric key
 *   FabricKey fabric_key;
 *   // Create a region
 *   FabricRegionId region = fabric_key.create_region();
 *   // Add a key with name and value
 *   FabricKeyId key = fabric_key.create_key(key_name, key_value);
 *   // Affilate a key to a region
 *   fabric_key.add_key_to_region(region, key);
 *
 *******************************************************************/
class FabricKey {
  public: /* Types */
    typedef vtr::vector<FabricKeyId, FabricKeyId>::const_iterator fabric_key_iterator;
    typedef vtr::vector<FabricRegionId, FabricRegionId>::const_iterator fabric_region_iterator;
    /* Create range */
    typedef vtr::Range<fabric_region_iterator> fabric_region_range;
    typedef vtr::Range<fabric_key_iterator> fabric_key_range;
  public:  /* Constructors */
    FabricKey();
  public: /* Accessors: aggregates */
    fabric_key_range keys() const;
    fabric_region_range regions() const;
  public: /* Public Accessors: Basic data query */
    /* Access all the keys of a region */
    std::vector<FabricKeyId> region_keys(const FabricRegionId& region_id) const;

    /* Access the name of a key */
    std::string key_name(const FabricKeyId& key_id) const;

    /* Access the value of a key */
    size_t key_value(const FabricKeyId& key_id) const;

    /* Access the alias of a key */
    std::string key_alias(const FabricKeyId& key_id) const;

    /* Check if there are any keys */
    bool empty() const;

  public: /* Public Mutators: model-related */

    /* Reserve a number of regions to be memory efficent */
    void reserve_regions(const size_t& num_regions);

    /* Create a new region and add it to the library, return an id */
    FabricRegionId create_region();

    /* Reserve the memory space for keys under a region, to be memory efficient */
    void reserve_region_keys(const FabricRegionId& region_id,
                             const size_t& num_keys);

    /* Add a key to a region */
    void add_key_to_region(const FabricRegionId& region_id,
                           const FabricKeyId& key_id);

    /* Reserve a number of keys to be memory efficent */
    void reserve_keys(const size_t& num_keys);

    /* Create a new key and add it to the library, return an id */
    FabricKeyId create_key();

    /* Configure attributes of a key */
    void set_key_name(const FabricKeyId& key_id,
                      const std::string& name);

    void set_key_value(const FabricKeyId& key_id,
                       const size_t& value);

    void set_key_alias(const FabricKeyId& key_id,
                       const std::string& alias);

  public: /* Public invalidators/validators */
    bool valid_region_id(const FabricRegionId& region_id) const;
    bool valid_key_id(const FabricKeyId& key_id) const;
  private: /* Internal data */
    /* Unique ids for each region */
    vtr::vector<FabricRegionId, FabricRegionId> region_ids_;

    /* Key ids for each region */
    vtr::vector<FabricRegionId, std::vector<FabricKeyId>> region_key_ids_;

    /* Unique ids for each key */
    vtr::vector<FabricKeyId, FabricKeyId> key_ids_;

    /* Names for each key */
    vtr::vector<FabricKeyId, std::string> key_names_;

    /* Values for each key */
    vtr::vector<FabricKeyId, size_t> key_values_;

    /* Region for each key */
    vtr::vector<FabricKeyId, FabricRegionId> key_regions_;

    /* Optional alias for each key, with which a key can also be represented */
    vtr::vector<FabricKeyId, std::string> key_alias_;
};

#endif
