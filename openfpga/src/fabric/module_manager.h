#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "module_manager_fwd.h"
#include "openfpga_port.h"
#include "physical_types.h"
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * This files includes data structures for module management.
 * It keeps a list of modules that have been generated, the port map of the
 *modules, parents and children of each modules. This will ease instanciation of
 *modules with explicit port map and outputting a hierarchy of modules
 *
 * Module includes the basic information:
 * 1. unique identifier
 * 2. module name: which should be unique
 * 3. port list: basic information of all the ports belonging to the module
 * 4. port types: types of each port, which will matter how we output the ports
 * 5. parent modules: ids of parent modules
 * 6. children modules: ids of child modules
 ******************************************************************************/
class ModuleManager {
 public: /* Private data structures */
  /* A type to indicate the usage of ports
   * Test bench generator use this to identify
   * what signals to drive
   */
  enum e_module_port_type {
    MODULE_GLOBAL_PORT, /* Global inputs */
    MODULE_GPIN_PORT,   /* General-purpose input */
    MODULE_GPOUT_PORT,  /* General-purpose outputs, could be used for spypads */
    MODULE_GPIO_PORT,   /* General-purpose IOs, which are data IOs of the fabric
                         */
    MODULE_INOUT_PORT,  /* Normal (non-global) inout ports */
    MODULE_INPUT_PORT,  /* Normal (non-global) input ports */
    MODULE_OUTPUT_PORT, /* Normal (non-global) output ports */
    MODULE_CLOCK_PORT,  /* Normal (non-global) clock ports*/
    NUM_MODULE_PORT_TYPES
  };

  /* A type to indicate the usage of module
   * This helps FPGA-SPICE to identify which VDD/VSS
   * port should be applied to modules
   */
  enum e_module_usage_type {
    MODULE_TOP,          /* Top-level module */
    MODULE_CONFIG,       /* Configuration modules, i.e., decoders, sram etc. */
    MODULE_CONFIG_GROUP, /* Configuration modules, i.e., decoders, sram etc. */
    MODULE_INTERC,  /* Programmable interconnection, e.g., routing multiplexer
                       etc. */
    MODULE_GRID,    /* Grids (programmable blocks) */
    MODULE_LUT,     /* Look-Up Table (LUT) modules */
    MODULE_HARD_IP, /* Hard IP modules */
    MODULE_SB,      /* Switch block modules */
    MODULE_CB,      /* Connection block modules */
    MODULE_IO,      /* I/O modules */
    MODULE_VDD,     /* Local VDD lines to generate constant voltages */
    MODULE_VSS,     /* Local VSS lines to generate constant voltages */
    NUM_MODULE_USAGE_TYPES
  };

  /* Type of configurable child:
   * - logical: represent a logical configurable block, which may not contain a
   * physical memory inside
   * - physical: represent a physical configurable block, which contains a
   * physical memory inside
   * - unified: a unified block whose physical memory is also the logical memory
   */
  enum class e_config_child_type { LOGICAL, PHYSICAL, UNIFIED, NUM_TYPES };

 public: /* Public Constructors */
 public: /* Type implementations */
  /*
   * This class (forward delcared above) is a template used to represent a
   * lazily calculated iterator of the specified ID type. The key assumption
   * made is that the ID space is contiguous and can be walked by incrementing
   * the underlying ID value. To account for invalid IDs, it keeps a reference
   * to the invalid ID set and returns ID::INVALID() for ID values in the set.
   *
   * It is used to lazily create an iteration range (e.g. as returned by
   * RRGraph::edges() RRGraph::nodes()) just based on the count of allocated
   * elements (i.e. RRGraph::num_nodes_ or RRGraph::num_edges_), and the set of
   * any invalid IDs (i.e. RRGraph::invalid_node_ids_,
   * RRGraph::invalid_edge_ids_).
   */
  template <class ID>
  class lazy_id_iterator
    : public std::iterator<std::bidirectional_iterator_tag, ID> {
   public:
    // Since we pass ID as a template to std::iterator we need to use an
    // explicit 'typename' to bring the value_type and iterator names into scope
    typedef
      typename std::iterator<std::bidirectional_iterator_tag, ID>::value_type
        value_type;
    typedef
      typename std::iterator<std::bidirectional_iterator_tag, ID>::iterator
        iterator;

    lazy_id_iterator(value_type init, const std::unordered_set<ID>& invalid_ids)
      : value_(init), invalid_ids_(invalid_ids) {}

    // Advance to the next ID value
    iterator operator++() {
      value_ = ID(size_t(value_) + 1);
      return *this;
    }

    // Advance to the previous ID value
    iterator operator--() {
      value_ = ID(size_t(value_) - 1);
      return *this;
    }

    // Dereference the iterator
    value_type operator*() const {
      return (invalid_ids_.count(value_)) ? ID::INVALID() : value_;
    }

    friend bool operator==(const lazy_id_iterator<ID> lhs,
                           const lazy_id_iterator<ID> rhs) {
      return lhs.value_ == rhs.value_;
    }
    friend bool operator!=(const lazy_id_iterator<ID> lhs,
                           const lazy_id_iterator<ID> rhs) {
      return !(lhs == rhs);
    }

   private:
    value_type value_;
    const std::unordered_set<ID>& invalid_ids_;
  };

 public: /* Types and ranges */
  // Lazy iterator utility forward declaration
  template <class ID>
  class lazy_id_iterator;

  typedef vtr::vector<ModuleId, ModuleId>::const_iterator module_iterator;
  typedef vtr::vector<ModulePortId, ModulePortId>::const_iterator
    module_port_iterator;
  typedef lazy_id_iterator<ModuleNetId> module_net_iterator;
  typedef vtr::vector<ModuleNetSrcId, ModuleNetSrcId>::const_iterator
    module_net_src_iterator;
  typedef vtr::vector<ModuleNetSinkId, ModuleNetSinkId>::const_iterator
    module_net_sink_iterator;
  typedef vtr::vector<ConfigRegionId, ConfigRegionId>::const_iterator
    region_iterator;

  typedef vtr::Range<module_iterator> module_range;
  typedef vtr::Range<module_port_iterator> module_port_range;
  typedef vtr::Range<module_net_iterator> module_net_range;
  typedef vtr::Range<module_net_src_iterator> module_net_src_range;
  typedef vtr::Range<module_net_sink_iterator> module_net_sink_range;
  typedef vtr::Range<region_iterator> region_range;

 public: /* Public aggregators */
  /* Find all the modules */
  module_range modules() const;
  /** @brief find all the modules with a given usage. Note that this function is
   * not optimized when the number of modules are large. In most cases, the
   * number of modules are fairly small (less than 10k). */
  std::vector<ModuleId> modules_by_usage(
    const ModuleManager::e_module_usage_type& usage) const;
  /* Find all the ports belonging to a module */
  module_port_range module_ports(const ModuleId& module) const;
  /* Find all the nets belonging to a module */
  module_net_range module_nets(const ModuleId& module) const;
  /* Find all the child modules under a parent module */
  std::vector<ModuleId> child_modules(const ModuleId& parent_module) const;
  /* Find all the instances under a parent module */
  std::vector<size_t> child_module_instances(
    const ModuleId& parent_module, const ModuleId& child_module) const;
  /* Find all the configurable child modules under a parent module */
  std::vector<ModuleId> configurable_children(
    const ModuleId& parent_module, const e_config_child_type& type) const;
  /* Find all the instances of configurable child modules under a parent module
   */
  std::vector<size_t> configurable_child_instances(
    const ModuleId& parent_module, const e_config_child_type& type) const;
  /* Find the coordindate of a configurable child module under a parent module
   */
  std::vector<vtr::Point<int>> configurable_child_coordinates(
    const ModuleId& parent_module, const e_config_child_type& type) const;

  /* Find all the configurable child modules under a parent module
   * Note that a physical configurable child module may be at
   * another module; Only the logical child module is under the current parent
   * module
   */
  std::vector<ModuleId> logical2physical_configurable_children(
    const ModuleId& parent_module) const;
  /* Find all the instance names of configurable child modules under a parent
   * module
   */
  std::vector<std::string> logical2physical_configurable_child_instance_names(
    const ModuleId& parent_module) const;

  /* Find all the I/O child modules under a parent module */
  std::vector<ModuleId> io_children(const ModuleId& parent_module) const;
  /* Find all the instances of I/O child modules under a parent module */
  std::vector<size_t> io_child_instances(const ModuleId& parent_module) const;
  /* Find the coordindate of an I/O child module under a parent module */
  std::vector<vtr::Point<int>> io_child_coordinates(
    const ModuleId& parent_module) const;

  /* Find the source ids of modules */
  module_net_src_range module_net_sources(const ModuleId& module,
                                          const ModuleNetId& net) const;
  /* Find the sink ids of modules */
  module_net_sink_range module_net_sinks(const ModuleId& module,
                                         const ModuleNetId& net) const;

  /* Find all the regions */
  region_range regions(const ModuleId& module) const;
  /* Find all the configurable child modules under a region of a parent module
   * Note that we use logical children here
   */
  std::vector<ModuleId> region_configurable_children(
    const ModuleId& parent_module, const ConfigRegionId& region) const;
  /* Find all the instances of configurable child modules under a region of a
   * parent module; Note that we use logical children here */
  std::vector<size_t> region_configurable_child_instances(
    const ModuleId& parent_module, const ConfigRegionId& region) const;

  /* Find all the coordinates of configurable child modules under a region of a
   * parent module; Note that we use logical children here */
  std::vector<vtr::Point<int>> region_configurable_child_coordinates(
    const ModuleId& parent_module, const ConfigRegionId& region) const;

 public: /* Public accessors */
  size_t num_modules() const;
  size_t num_nets(const ModuleId& module) const;
  std::string module_name(const ModuleId& module_id) const;
  e_module_usage_type module_usage(const ModuleId& module_id) const;
  std::string module_port_type_str(
    const enum e_module_port_type& port_type) const;
  std::vector<BasicPort> module_ports_by_type(
    const ModuleId& module_id, const enum e_module_port_type& port_type) const;
  std::vector<ModulePortId> module_port_ids_by_type(
    const ModuleId& module_id, const enum e_module_port_type& port_type) const;
  /* Find a port of a module by a given name */
  ModulePortId find_module_port(const ModuleId& module_id,
                                const std::string& port_name) const;
  /* Find the Port information with a given port id */
  BasicPort module_port(const ModuleId& module_id,
                        const ModulePortId& port_id) const;
  /* Find a module by a given name */
  ModuleId find_module(const std::string& name) const;
  /* Find the number of instances of a child module in the parent module */
  size_t num_instance(const ModuleId& parent_module,
                      const ModuleId& child_module) const;
  /* Find the instance name of a child module */
  std::string instance_name(const ModuleId& parent_module,
                            const ModuleId& child_module,
                            const size_t& instance_id) const;
  /* Find the instance id of a given instance name */
  size_t instance_id(const ModuleId& parent_module,
                     const ModuleId& child_module,
                     const std::string& instance_name) const;
  /** @brief Count the number of logical configurable children */
  size_t num_configurable_children(const ModuleId& parent_module,
                                   const e_config_child_type& type) const;
  /* Find the type of a port */
  ModuleManager::e_module_port_type port_type(const ModuleId& module,
                                              const ModulePortId& port) const;
  /* Get the physical side of a port. Note that not every pin has a valid side.
   * An invalid value NUM_SIDES will be returned when the pin does not has a
   * specific physical location */
  e_side port_side(const ModuleId& module, const ModulePortId& port) const;
  /* Find if a port is a wire connection */
  bool port_is_wire(const ModuleId& module, const ModulePortId& port) const;
  /* Find if a port is mappable to an I/O from users' implementations */
  bool port_is_mappable_io(const ModuleId& module,
                           const ModulePortId& port) const;
  /* Find if a port is register */
  bool port_is_register(const ModuleId& module, const ModulePortId& port) const;
  /* Return the pre-processing flag of a port */
  std::string port_preproc_flag(const ModuleId& module,
                                const ModulePortId& port) const;
  /* Find a net from an instance of a module */
  ModuleNetId module_instance_port_net(const ModuleId& parent_module,
                                       const ModuleId& child_module,
                                       const size_t& child_instance,
                                       const ModulePortId& child_port,
                                       const size_t& child_pin) const;
  /* Find the name of net */
  std::string net_name(const ModuleId& module, const ModuleNetId& net) const;
  /* Find the source modules of a net */
  vtr::vector<ModuleNetSrcId, ModuleId> net_source_modules(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the ids of source instances of a net */
  vtr::vector<ModuleNetSrcId, size_t> net_source_instances(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the source ports of a net */
  vtr::vector<ModuleNetSrcId, ModulePortId> net_source_ports(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the source pin indices of a net */
  vtr::vector<ModuleNetSrcId, size_t> net_source_pins(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Identify if a pin of a port in a module already exists in the net source
   * list*/
  bool net_source_exist(const ModuleId& module, const ModuleNetId& net,
                        const ModuleId& src_module, const size_t& instance_id,
                        const ModulePortId& src_port, const size_t& src_pin);

  /* Find the sink modules of a net */
  vtr::vector<ModuleNetSinkId, ModuleId> net_sink_modules(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the ids of sink instances of a net */
  vtr::vector<ModuleNetSinkId, size_t> net_sink_instances(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the sink ports of a net */
  vtr::vector<ModuleNetSinkId, ModulePortId> net_sink_ports(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Find the sink pin indices of a net */
  vtr::vector<ModuleNetSinkId, size_t> net_sink_pins(
    const ModuleId& module, const ModuleNetId& net) const;
  /* Identify if a pin of a port in a module already exists in the net sink
   * list*/
  bool net_sink_exist(const ModuleId& module, const ModuleNetId& net,
                      const ModuleId& sink_module, const size_t& instance_id,
                      const ModulePortId& sink_port, const size_t& sink_pin);

  /** @brief Check if the configurable children under a given module are unified
   * or not. If unified, it means that the logical configurable children are the
   * same as the physical configurable children */
  bool unified_configurable_children(const ModuleId& curr_module) const;

 private: /* Private accessors */
  size_t find_child_module_index_in_parent_module(
    const ModuleId& parent_module, const ModuleId& child_module) const;

 public: /* Public mutators */
  /* Add a module */
  ModuleId add_module(const std::string& name);
  /* Add a port to a module */
  ModulePortId add_port(const ModuleId& module, const BasicPort& port_info,
                        const enum e_module_port_type& port_type);
  /* Set a name for a module port */
  void set_module_port_name(const ModuleId& module,
                            const ModulePortId& module_port,
                            const std::string& port_name);
  /* Set a name for a module */
  void set_module_name(const ModuleId& module, const std::string& name);
  /* Set a usage for a module */
  void set_module_usage(const ModuleId& module,
                        const e_module_usage_type& usage);
  /* Set a port to be a wire */
  void set_port_is_wire(const ModuleId& module, const std::string& port_name,
                        const bool& is_wire);
  void set_port_is_wire(const ModuleId& module, const ModulePortId& port_id,
                        const bool& is_wire);
  /* Set a port to be mappable to an I/O from users' implemenations */
  void set_port_is_mappable_io(const ModuleId& module,
                               const ModulePortId& port_id,
                               const bool& is_mappable_io);
  /* Set a port to be a register */
  void set_port_is_register(const ModuleId& module,
                            const std::string& port_name,
                            const bool& is_register);
  void set_port_is_register(const ModuleId& module, const ModulePortId& port_id,
                            const bool& is_register);
  /* Set the preprocessing flag for a port */
  void set_port_preproc_flag(const ModuleId& module, const ModulePortId& port,
                             const std::string& preproc_flag);
  /* Set side to a given pin of a module port. Note that the pin id must be a
   * valid one. Otherwise, abort and error out. The valid pin range can be get
   * from module_port().pins() */
  void set_port_side(const ModuleId& module, const ModulePortId& port,
                     const e_side& pin_side);
  /** @brief Add a child module to a parent module.
   *  By default, it considers the child module as an I/O child, and update the
   * children list of I/O modules inside It not needed, just turn it off. Then
   * you need to call add_io_child() API to update child list
   *
   *  .. note:: By default, we assume the I/O indexing to the same as sequence
   * when adding child modules to a parent. However, it may not be true all the
   * time, especially for the top-level module, where customization is needed.
   */
  void add_child_module(const ModuleId& parent_module,
                        const ModuleId& child_module,
                        const bool& is_io_child = true);
  /* Set the instance name of a child module */
  void set_child_instance_name(const ModuleId& parent_module,
                               const ModuleId& child_module,
                               const size_t& instance_id,
                               const std::string& instance_name);
  /* Add a configurable child module to module
   * This function also set the coordinate of a configurable child
   * The coordinate is a relative position in each region, which is used to
   * idenify BL/WL sharing
   * By default, it is an invalid coordinate
   */
  void add_configurable_child(
    const ModuleId& module, const ModuleId& child_module,
    const size_t& child_instance, const e_config_child_type& type,
    const vtr::Point<int> coord = vtr::Point<int>(-1, -1));
  /** @brief Create a pair of mapping from a logical configurable child to a
   * physical configurable child */
  void set_logical2physical_configurable_child(
    const ModuleId& parent_module, const size_t& logical_child_id,
    const ModuleId& physical_child_module);
  /** @brief Create a pair of mapping from a logical configurable child to a
   * physical configurable child */
  void set_logical2physical_configurable_child_instance_name(
    const ModuleId& parent_module, const size_t& logical_child_id,
    const std::string& physical_child_instance_name);
  /* Reserved a number of configurable children for memory efficiency */
  void reserve_configurable_child(const ModuleId& module,
                                  const size_t& num_children,
                                  const e_config_child_type& type);

  /* Create a new configurable region under a module */
  ConfigRegionId add_config_region(const ModuleId& module);
  /* Add a configurable child module to a region
   * Note:
   *   - The child module must be added as a physical configurable child to the
   * parent module before calling this function!
   */
  void add_configurable_child_to_region(const ModuleId& parent_module,
                                        const ConfigRegionId& config_region,
                                        const ModuleId& child_module,
                                        const size_t& child_instance,
                                        const size_t& config_child_id);
  /** @brief Add a I/O child to module
   * This function also set the coordinate of a configurable child
   * The coordinate is used for build I/O location map. So it is consistent with
   * the VPR coordinate system By default, it is an invalid coordinate
   *
   * .. note:: I/O child does not necessary have to be a I/O block. It just
   * provide a sequence for other functions, e.g., connect_gpio_module() to
   * index the I/Os from each child module/instance.
   */
  void add_io_child(const ModuleId& module, const ModuleId& child_module,
                    const size_t& child_instance,
                    const vtr::Point<int> coord = vtr::Point<int>(-1, -1));
  /** @brief Reserved a number of I/O children for memory efficiency */
  void reserve_io_child(const ModuleId& module, const size_t& num_children);

  /* Reserved a number of module nets for a given module for memory efficiency
   */
  void reserve_module_nets(const ModuleId& module, const size_t& num_nets);

  /* Add a net to the connection graph of the module */
  ModuleNetId create_module_net(const ModuleId& module);
  /* Set the name of net */
  void set_net_name(const ModuleId& module, const ModuleNetId& net,
                    const std::string& name);

  /* Reserved a number of sources for a module net for a given module for memory
   * efficiency */
  void reserve_module_net_sources(const ModuleId& module,
                                  const ModuleNetId& net,
                                  const size_t& num_sources);

  /* Add a source to a net in the connection graph */
  ModuleNetSrcId add_module_net_source(const ModuleId& module,
                                       const ModuleNetId& net,
                                       const ModuleId& src_module,
                                       const size_t& instance_id,
                                       const ModulePortId& src_port,
                                       const size_t& src_pin);

  /* Reserved a number of sinks for a module net for a given module for memory
   * efficiency */
  void reserve_module_net_sinks(const ModuleId& module, const ModuleNetId& net,
                                const size_t& num_sinks);

  /* Add a sink to a net in the connection graph */
  ModuleNetSinkId add_module_net_sink(const ModuleId& module,
                                      const ModuleNetId& net,
                                      const ModuleId& sink_module,
                                      const size_t& instance_id,
                                      const ModulePortId& sink_port,
                                      const size_t& sink_pin);

  /** @brief Create a wrapper module on an existing module. The wrapper module
   * will herit all the ports with the same direction, width and names from the
   * selected module. The wrapper module will contain the existing module. For
   * example,
   *
   *     Wrapper module
   *     +------------------------+
   *     |  existing module       |
   *     |  +------------------+  |
   *     |  |                  |  |
   * a ->+->+ a              b-+--+-> b
   *     |  |                  |  |
   *     |  +------------------+  |
   *     +------------------------+
   */
  ModuleId create_wrapper_module(const ModuleId& existing_module,
                                 const std::string& wrapper_module_name,
                                 const std::string& instance_name,
                                 const bool& add_nets);

 public: /* Public deconstructors */
  /* This is a strong function which will remove all the configurable children
   * under a given parent module
   * It is mainly used by loading fabric keys
   * Do NOT use unless you know what you are doing!!!
   */
  void clear_configurable_children(const ModuleId& parent_module);

  /* This is a strong function which will remove all the configurable regions
   * under a given parent module
   * It is mainly used by loading fabric keys
   * Do NOT use unless you know what you are doing!!!
   */
  void clear_config_region(const ModuleId& parent_module);

  /* This is a strong function which will remove all the io children
   * under a given parent module
   * It is mainly used by other functions which want to force an I/O sequence
   * Do NOT use unless you know what you are doing!!!
   */
  void clear_io_children(const ModuleId& parent_module);

  /* Remove all the sinks for a given net under a module */
  void clear_module_net_sinks(const ModuleId& parent_module,
                              const ModuleNetId& net);

 public: /* Public validators/invalidators */
  bool valid_module_id(const ModuleId& module) const;
  bool valid_module_port_id(const ModuleId& module,
                            const ModulePortId& port) const;
  bool valid_module_net_id(const ModuleId& module,
                           const ModuleNetId& net) const;
  bool valid_module_instance_id(const ModuleId& parent_module,
                                const ModuleId& child_module,
                                const size_t& instance_id) const;
  bool valid_region_id(const ModuleId& module,
                       const ConfigRegionId& region) const;

 private: /* Private validators/invalidators */
  void invalidate_name2id_map();
  void invalidate_port_lookup();
  void invalidate_net_lookup();

 private: /* Internal data */
  /* Module-level data */
  vtr::vector<ModuleId, ModuleId> ids_; /* Unique identifier for each Module */
  vtr::vector<ModuleId, std::string>
    names_; /* Unique identifier for each Module */
  vtr::vector<ModuleId, e_module_usage_type> usages_; /* Usage of each module */
  vtr::vector<ModuleId, std::vector<ModuleId>>
    parents_; /* Parent modules that include the module */
  vtr::vector<ModuleId, std::vector<ModuleId>>
    children_; /* Child modules that this module contain */
  vtr::vector<ModuleId, std::vector<size_t>>
    num_child_instances_; /* Number of children instance in each child module */
  vtr::vector<ModuleId, std::vector<std::vector<std::string>>>
    child_instance_names_; /* Number of children instance in each child module
                            */

  /* Configurable child modules are used to record the position of configurable
   * modules in bitstream The sequence of children in the list denotes which one
   * is configured first, etc. Note that the sequence can be totally different
   * from the children_ list This is really dependent how the configuration
   * protocol is organized which should be made by users/designers
   * Note that there could be two types of configurable children under a module
   * - logical: only contains virtual/feedthough memory blocks. A logical
   * configurable child can only contain logical subchild. Logical memory block
   * is required for architecture bitstream generation, because it carries
   * logical information (the location of memory to its programmable resources)
   * - physical: contains physical memory blocks. Logical memory blocks are
   * mapped to the physical memory block. A physical memory block may contain
   * coordinates and configuration regions which are required for fabric
   * bitstream generation.
   */
  vtr::vector<ModuleId, std::vector<ModuleId>>
    logical_configurable_children_; /* Child modules with configurable memory
                               bits that this module contain */
  vtr::vector<ModuleId, std::vector<size_t>>
    logical_configurable_child_instances_; /* Instances of child modules with
                                      configurable memory bits that this module
                                      contain */
  vtr::vector<ModuleId, std::vector<ModuleId>>
    logical2physical_configurable_children_; /* Child modules with configurable
                               memory bits that this module contain */
  vtr::vector<ModuleId, std::vector<std::string>>
    logical2physical_configurable_child_instance_names_; /* Instances of child
                                      modules with configurable memory bits that
                                      this module contain */

  vtr::vector<ModuleId, std::vector<ModuleId>>
    physical_configurable_children_; /* Child modules with configurable memory
                               bits that this module contain */
  vtr::vector<ModuleId, std::vector<size_t>>
    physical_configurable_child_instances_; /* Instances of child modules with
                                      configurable memory bits that this module
                                      contain */
  vtr::vector<ModuleId, std::vector<ConfigRegionId>>
    physical_configurable_child_regions_; /* Instances of child modules with
                                    configurable memory bits that this module
                                    contain */
  vtr::vector<ModuleId, std::vector<vtr::Point<int>>>
    physical_configurable_child_coordinates_; /* Relative coorindates of child
                                        modules with configurable memory bits
                                        that this module contain */

  /* Configurable regions to group the physical configurable children
   * Note:
   *   - Each child can only be added a group
   */
  vtr::vector<ModuleId, vtr::vector<ConfigRegionId, ConfigRegionId>>
    config_region_ids_;
  vtr::vector<ModuleId, vtr::vector<ConfigRegionId, std::vector<size_t>>>
    config_region_children_;

  /* I/O child modules are used to record the position of I/O modules in GPIO
   * indexing The sequence of children in the list denotes which one is indexed
   * in the GPIO first, etc. Note that the sequence can be totally different
   * from the children_ list This is really dependent how the I/O indexing is
   * organized which should be made by users/designers
   */
  vtr::vector<ModuleId, std::vector<ModuleId>> io_children_;
  vtr::vector<ModuleId, std::vector<size_t>> io_child_instances_;
  vtr::vector<ModuleId, std::vector<vtr::Point<int>>> io_child_coordinates_;

  /* Port-level data */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, ModulePortId>>
    port_ids_; /* List of ports for each Module */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, BasicPort>>
    ports_; /* List of ports for each Module */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, enum e_module_port_type>>
    port_types_; /* Type of ports */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, e_side>>
    port_sides_; /* Type of ports */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, bool>>
    port_is_mappable_io_; /* If the port is mappable  to an I/O for user's
                             implementations */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, bool>>
    port_is_wire_; /* If the port is a wire, use for Verilog port definition. If
                      enabled: <port_type> reg <port_name>  */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, bool>>
    port_is_register_; /* If the port is a register, use for Verilog port
                          definition. If enabled: <port_type> reg <port_name>
                        */
  vtr::vector<ModuleId, vtr::vector<ModulePortId, std::string>>
    port_preproc_flags_; /* If a port is available only when a pre-processing
                            flag is enabled. This is to record the
                            pre-processing flags */

  /* Graph-level data:
   * We use nets to model the connection between pins of modules and instances.
   * To avoid large memory footprint, we do NOT create pins,
   * To enable fast look-up on pins, we create a fast look-up
   */
  vtr::vector<ModuleId, size_t> num_nets_; /* List of nets for each Module */
  vtr::vector<ModuleId, std::unordered_set<ModuleNetId>>
    invalid_net_ids_; /* Invalid net ids */
  vtr::vector<ModuleId, vtr::vector<ModuleNetId, std::string>>
    net_names_; /* Name of net */

  vtr::vector<
    ModuleId,
    vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, ModuleNetSrcId>>>
    net_src_ids_; /* Unique id of the source that drive the net */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, size_t>>>
    net_src_terminal_ids_; /* Pin ids that drive the net */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, size_t>>>
    net_src_instance_ids_; /* Pin ids that drive the net */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, size_t>>>
    net_src_pin_ids_; /* Pin ids that drive the net */

  vtr::vector<
    ModuleId,
    vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, ModuleNetSinkId>>>
    net_sink_ids_; /* Unique ids of the sink that the net drives */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, size_t>>>
    net_sink_terminal_ids_; /* Pin ids that the net drives */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, size_t>>>
    net_sink_instance_ids_; /* Pin ids that drive the net */
  vtr::vector<ModuleId,
              vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, size_t>>>
    net_sink_pin_ids_; /* Pin ids that drive the net */

  /* fast look-up for module */
  std::map<std::string, ModuleId> name_id_map_;
  /* fast look-up for ports */
  typedef vtr::vector<ModuleId, std::vector<std::vector<ModulePortId>>>
    PortLookup;
  mutable PortLookup port_lookup_; /* [module_ids][port_types][port_ids] */

  /* fast look-up for nets */
  typedef vtr::vector<
    ModuleId,
    std::map<ModuleId,
             std::vector<std::map<ModulePortId, std::vector<ModuleNetId>>>>>
    NetLookup;
  mutable NetLookup
    net_lookup_; /* [module_ids][module_ids][instance_ids][port_ids][pin_ids] */

  /* Store pairs of a module and a port, which are frequently used in net
   * terminals (either source or sink)
   */
  std::vector<std::pair<ModuleId, ModulePortId>> net_terminal_storage_;
};

} /* end namespace openfpga */

#endif
