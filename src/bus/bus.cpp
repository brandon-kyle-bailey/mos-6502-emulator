#include "bus.h"

void Bus::attach(uint16_t base, uint16_t size, Device *device) {
  devices.push_back({base, size, device});
};

uint8_t Bus::read(uint16_t address) {
  for (auto &map : devices) {
    if (address >= map.base && address < map.base + map.size) {
      return map.device->read(address - map.base);
    };
  };
  return 0xFF;
};

void Bus::write(uint16_t address, uint8_t value) {
  for (auto &map : devices) {
    if (address >= map.base && address < map.base + map.size) {
      map.device->write(address - map.base, value);
    };
  };
};
