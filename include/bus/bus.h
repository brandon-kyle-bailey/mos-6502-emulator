#pragma once

#include "devices/device.h"

#include <cstdint>
#include <vector>

struct Bus {
  struct DeviceMap {
    uint16_t base;
    uint16_t size;
    Device *device;
  };
  std::vector<DeviceMap> devices;

  void attach(uint16_t base, uint16_t size, Device *device);

  uint8_t read(uint16_t address);

  void write(uint16_t address, uint8_t value);
};
