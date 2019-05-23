#ifndef DEVICE_COORDINATOR_H
#define DEVICE_COORDINATOR_H

#include "sides.h"

/* Coordinator System for FPGA Device
 * It is based on a 3-D (x,y,z) coordinator system
 * (x,y) is used for all the routing resources,
 * z is used for only grid, which have multiple logic blocks 
 */
class DeviceCoordinator {
  public: /* Contructors */
    DeviceCoordinator(size_t x, size_t y);
    DeviceCoordinator();
  public: /* Accessors */
    size_t get_x() const;
    size_t get_y() const;
  public: /* Mutators */
    void set(size_t x, size_t y);
    void set_x(size_t x);
    void set_y(size_t y);
    void rotate();
    void clear();
  private: /* Internal Mutators */
  private: /* internal functions */
  private: /* Internal Data */
    size_t x_;
    size_t y_;
    size_t z_;
};

#endif
