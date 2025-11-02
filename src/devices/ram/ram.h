
#pragma once

#include "../device.h"
#include <array>
struct Ram : Device {

  // 2 KB internal RAM
  std::array<uint8_t, 0x10000> data{};

  uint8_t read(uint16_t address) override;

  void write(uint16_t address, uint8_t value) override;
};
