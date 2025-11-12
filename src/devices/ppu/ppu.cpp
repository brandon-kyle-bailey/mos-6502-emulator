#include "devices/ppu.h"

uint8_t Ppu::read(uint16_t address) { return data[address & 0x07FF]; }

void Ppu::write(uint16_t address, uint8_t value) {
  data[address & 0x07FF] = value;
}
