#include <algorithm>
#include "device_coordinator.h"

/* Member functions for DeviceCoordinator */
/* Public constructors */
/*
DeviceCoordinator::DeviceCoordinator(DeviceCoordinator& coordinator) {
  set(coordinator.get_x(), coordinator.get_y());
  return;
}
*/

DeviceCoordinator::DeviceCoordinator(size_t x, size_t y) {
  set(x, y);
  return;
}

DeviceCoordinator::DeviceCoordinator() {
  set(0, 0);
  return;
}

/* Public accessors  */

size_t DeviceCoordinator::get_x() const {
  return x_;
}

size_t DeviceCoordinator::get_y() const {
  return y_;
}

/* Public mutators */
void DeviceCoordinator::set(size_t x, size_t y) {
  set_x(x); 
  set_y(y);
  return;
}

void DeviceCoordinator::set_x(size_t x) {
  x_ = x;
  return;
}

void DeviceCoordinator::set_y(size_t y) {
  y_ = y;
  return;
}

void DeviceCoordinator::rotate() {
  std::swap(x_, y_);
  return;
}

void DeviceCoordinator::clear() {
  set(0, 0);
  return;
}
