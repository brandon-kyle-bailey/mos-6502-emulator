#include "devices/ram.h"

uint8_t Ram::read(uint16_t address) { return data[address & 0x07FF]; }

void Ram::write(uint16_t address, uint8_t value) {
  data[address & 0x07FF] = value;
}
