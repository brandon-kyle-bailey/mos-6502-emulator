#include <iostream>
#include <stdexcept>

using BYTE = unsigned char;
using TWO_BYTE = unsigned short;

struct MEM {
  // 64kb of memory for 16 bit address space
  static constexpr unsigned int MAX_MEM = 1024 * 64;
  BYTE AD[MAX_MEM];

  void validAddressSpace(unsigned int address) const {
    if (address < 0) {
      throw std::runtime_error("Memory underflow");
    }
    if (address > MAX_MEM) {
      throw std::runtime_error("Memory overflow");
    }
  }

  BYTE Read(unsigned int address) const {
    validAddressSpace(address);
    return AD[address];
  };
  void Write(unsigned int address, BYTE value) {
    validAddressSpace(address);
    AD[address] = value;
  }
};

struct CPU {

  BYTE SP_MIN = 0x00;
  BYTE SP_MAX = 0xFF;
  BYTE SP = 0x01;

  // Bit positions. Applied to PS
  static constexpr BYTE CARRY_FLAG = 1 << 0;        // Bit 0
  static constexpr BYTE ZERO_FLAG = 1 << 1;         // Bit 1
  static constexpr BYTE INTERRUPT_DISABLE = 1 << 2; // Bit 2
  static constexpr BYTE DECIMAL_MODE = 1 << 3;      // Bit 3
  static constexpr BYTE BREAK_COMMAND = 1 << 4;     // Bit 4
  static constexpr BYTE UNUSED = 1 << 5;            // Bit 5, often unused
  static constexpr BYTE OVERFLOW_FLAG = 1 << 6;     // Bit 6
  static constexpr BYTE NEGATIVE_FLAG = 1 << 7;     // Bit 7

  TWO_BYTE PC = 0;
  BYTE AC, IRX, IRY, PS = 0;

  // op codes
  static const BYTE LDA_IMMEDIATE = 0xA9;

  void setProcessorStatus(BYTE flag) { PS |= flag; }
  void clearProcessorStatus(BYTE flag) { PS &= ~flag; }

  BYTE ReadFromMemory(MEM &mem, TWO_BYTE &address) {
    BYTE data = mem.Read(address);
    PC++;
    return data;
  }

  void WriteToMemory(MEM &mem, TWO_BYTE &address, BYTE &value) {
    mem.Write(address, value);
  }

  void Execute(MEM &mem, unsigned int cycles) {

    while (cycles > 0) {
      BYTE instruction = ReadFromMemory(mem, PC);
      std::cout << "Instruction:" << "\n";
      cycles--;
      switch (instruction) {
      case LDA_IMMEDIATE: {
        std::cout << "Instruction is to load accumulator immediately" << "\n";
        BYTE data = ReadFromMemory(mem, PC);
        AC = data;
        if (AC == 0) {
          setProcessorStatus(ZERO_FLAG);
        } else {
          clearProcessorStatus(ZERO_FLAG);
        }
        if (AC & 0x80) {
          setProcessorStatus(NEGATIVE_FLAG);
        } else {
          clearProcessorStatus(NEGATIVE_FLAG);
        }

      } break;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  std::cout << "Hello world!" << "\n";
  CPU cpu = CPU{};
  MEM mem = MEM{};

  cpu.Execute(mem, 10);

  return 0;
}
