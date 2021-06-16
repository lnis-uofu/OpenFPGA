#ifndef IO_MAP_H
#define IO_MAP_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include "vtr_vector.h"
#include "openfpga_port.h"
#include "io_map_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is a data structure storing io mapping information 
 * - the net-to-I/O mapping
 * - each I/O directionality
 *******************************************************************/
class IoMap {
  public: /* Types and ranges */
    enum e_direction {
      IO_MAP_DIR_INPUT,
      IO_MAP_DIR_OUTPUT,
      NUM_IO_MAP_DIR_TYPES
    };
    typedef vtr::vector<IoMapId, IoMapId>::const_iterator io_map_iterator;
    typedef vtr::Range<io_map_iterator> io_map_range;
  public: /* Public aggregators */
    /* Find all io mapping */
    io_map_range io_map() const;

    /* Get the port of the io that is mapped */
    BasicPort io_port(IoMapId io_map_id) const; 

    /* Get the net of the io that is mapped to */
    BasicPort io_net(IoMapId io_map_id) const; 

    /* Query on if an io is configured as an input */
    bool is_io_input(IoMapId io_map_id) const; 

    /* Query on if an io is configured as an output */
    bool is_io_output(IoMapId io_map_id) const; 
  public: /* Public mutators */
    /* Create a new I/O mapping */
    IoMapId create_io_mapping(const BasicPort& port,
                              const BasicPort& net,
                              e_direction dir);
  public: /* Public validators/invalidators */
    bool valid_io_map_id(IoMapId io_map_id) const;
  private: /* Internal Data */
    vtr::vector<IoMapId, IoMapId> io_map_ids_;
    vtr::vector<IoMapId, BasicPort> io_ports_;
    vtr::vector<IoMapId, BasicPort> mapped_nets_;
    vtr::vector<IoMapId, e_direction> io_directionality_;
};

} /* End namespace openfpga*/

#endif
