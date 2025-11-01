#include "bus/bus.h"
#include "cpu/cpu.h"
#include "devices/ppu/ppu.h"
#include "devices/ram/ram.h"
#include "devices/rom/rom.h"
#include <cstdint>

int main() {

  Rom rom = Rom{};
  Ram ram = Ram{};
  Ppu ppu = Ppu{};
  Bus bus = Bus{};

  uint16_t rom_address_space = 0xc000;
  bus.attach(rom_address_space, 0x4000, &rom); // 16KB ROM

  uint16_t ram_address_space = 0x0000;
  uint16_t ram_size = 0x0800;
  bus.attach(ram_address_space, ram_size, &ram); // 2KB RAM

  uint16_t ppu_address_space = 0x2000;
  bus.attach(ppu_address_space, 8, &ppu);

  CPU cpu = CPU{&bus};

  uint16_t program_start = 0x0100;
  std::vector<char> message = {'H', 'e', 'l', 'l', 'o', ' ',
                               'w', 'o', 'r', 'l', 'd', '!'};
  int message_length = size(message);

  std::vector<uint8_t> program;
  for (int i = 0; i < message_length; i++) {
    program.push_back(0xA9);
    program.push_back(message[i]);
    program.push_back(0x8D);
    program.push_back((program_start + i) & 0xFF);
    program.push_back((program_start + i) >> 8);
  }
  std::cout << "Program byte code:\n";
  for (int x : program) {
    std::cout << std::hex << x << " ";
  }
  std::cout << "\n";
  cpu.load_program(program, 0x0000);
  cpu.execute(message_length * 6);

  for (int i = 0; i < message_length; i++) {
    std::cout << cpu.read(program_start + i);
  };
  std::cout << "\n";

  for (int i = 0; i < size(ram.data) / 4; i++) {
    std::cout << std::hex << +ram.data[i] << " ";
  }

  return 0;
}
