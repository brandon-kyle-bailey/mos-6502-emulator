#include "rom.h"

uint8_t Rom::read(uint16_t address) { return data[address & 0x07FF]; }

void Rom::write(uint16_t address, uint8_t value) {
  data[address & 0x07FF] = value;
}
