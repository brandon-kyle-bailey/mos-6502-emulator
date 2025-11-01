#include "bus/bus.h"
#include "cpu/cpu.h"
#include "devices/ppu/ppu.h"
#include "devices/ram/ram.h"
#include "devices/rom/rom.h"
#include <array>
#include <iostream>

int main() {

  Rom rom = Rom{};
  Ram ram = Ram{};
  Ppu ppu = Ppu{};
  Bus bus = Bus{};

  bus.attach(0x0000, 0x0800, &ram); // 2KB RAM
  bus.attach(0x2000, 8, &ppu);
  bus.attach(0xC000, 0x4000, &rom); // 16KB ROM

  CPU cpu = CPU{&bus};

  std::cout << "Accumulator: " << std::hex << +cpu.AC << " ";
  std::cout << "Program Counter: " << std::hex << +cpu.PC << " ";
  std::cout << "Stack Pointer: " << std::hex << +cpu.SP << " ";
  std::cout << "Register X: " << std::hex << +cpu.IRX << " ";
  std::cout << "Register Y: " << std::hex << +cpu.IRY << " ";
  std::cout << "Processor Status: " << std::hex << +cpu.PS << "\n";
  for (int i = 0; i < size(ram.data) / 8; i++) {
    std::cout << std::hex << +ram.read(i) << " ";
  }
  std::cout << "\n";

  cpu.write(0x0000, 0xA9);
  cpu.write(0x0001, 0x67);
  cpu.write(0x0002, 0xA2);
  cpu.write(0x0003, 0x68);
  cpu.write(0x0004, 0xA0);
  cpu.write(0x0005, 0x69);
  cpu.execute(6);

  std::cout << "Accumulator: " << std::hex << +cpu.AC << " ";
  std::cout << "Program Counter: " << std::hex << +cpu.PC << " ";
  std::cout << "Stack Pointer: " << std::hex << +cpu.SP << " ";
  std::cout << "Register X: " << std::hex << +cpu.IRX << " ";
  std::cout << "Register Y: " << std::hex << +cpu.IRY << " ";
  std::cout << "Processor Status: " << std::hex << +cpu.PS << "\n";
  for (int i = 0; i < size(ram.data) / 8; i++) {
    std::cout << std::hex << +ram.read(i) << " ";
  }
  std::cout << "\n";

  return 0;
}
