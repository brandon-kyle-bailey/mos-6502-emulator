#include "bus/bus.h"
#include "cpu/cpu.h"
#include "devices/ppu/ppu.h"
#include "devices/ram/ram.h"
#include "devices/rom/rom.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
  Bus bus{};
  Ram ram{};
  bus.attach(0x0000, 0x0800, &ram);
  Ppu ppu{};
  bus.attach(0x2000, 0x0008, &ppu);
  Rom rom{};
  bus.attach(0xC000, 0x4000, &rom);

  CPU cpu{&bus};

  uint16_t output_addr = 0x0200;
  std::string input = "Hello world this is my first cpu assembled script.";
  // std::vector<char> message = {'H', 'e', 'l', 'l', 'o', ' ',
  //                              'w', 'o', 'r', 'l', 'd', '!', ' ', 'H', };

  std::vector<char> message(input.begin(), input.end());

  // Create a subroutine for each character at different addresses
  uint16_t sub_start = 0x0600;
  std::vector<uint16_t> sub_addrs;
  std::vector<uint8_t> program;

  for (size_t i = 0; i < message.size(); i++) {
    sub_addrs.push_back(sub_start);
    program.push_back(0xA9); // LDA #char
    program.push_back(static_cast<uint8_t>(message[i]));
    program.push_back(0x8D); // STA output_addr + i
    program.push_back(static_cast<uint8_t>((output_addr + i) & 0xFF));
    program.push_back(static_cast<uint8_t>((output_addr + i) >> 8));
    program.push_back(0x60); // RTS
    sub_start += 6;          // each subroutine is 6 bytes
  }

  cpu.load_program(program, 0x0600);

  // Main program: JSR to each subroutine
  uint16_t main_start = 0x0000;
  std::vector<uint8_t> main_program;
  for (auto saddr : sub_addrs) {
    main_program.push_back(0x20); // JSR
    main_program.push_back(static_cast<uint8_t>(saddr & 0xFF));
    main_program.push_back(static_cast<uint8_t>(saddr >> 8));
  }

  cpu.load_program(main_program, main_start);

  for (int i = 0; i < size(ram.data) / 25; i++) {
    std::cout << std::hex << +ram.data[i] << " ";
  };
  std::cout << std::dec << std::endl;
  std::cout << std::hex << +cpu.AC << " " << +cpu.PC << " " << +cpu.SP << " "
            << +cpu.IRX << " " << +cpu.IRY << " " << +cpu.PS << std::dec
            << std::endl;
  cpu.execute(main_program.size() * 6); // rough estimate
  std::cout << std::hex << +cpu.AC << " " << +cpu.PC << " " << +cpu.SP << " "
            << +cpu.IRX << " " << +cpu.IRY << " " << +cpu.PS << std::dec
            << std::endl;
  for (int i = 0; i < size(ram.data) / 25; i++) {
    std::cout << std::hex << +ram.data[i] << " ";
  };
  std::cout << std::dec << std::endl;

  // Read and print output
  for (size_t i = 0; i < message.size(); i++) {
    std::cout << static_cast<char>(cpu.read(0x0200 + i));
  }
  std::cout << "\n";

  return 0;
}
