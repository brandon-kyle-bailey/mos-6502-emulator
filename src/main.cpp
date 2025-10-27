#include <iostream>
#include <stdexcept>

using BYTE = unsigned char;
using TWO_BYTE = unsigned short;

struct MEM {
  // 64kb of memory for 16 bit address space
  static constexpr unsigned int MAX_MEM = 1024 * 64;
  BYTE AD[MAX_MEM];

  void Initialize() {
    for (unsigned int i = 0; i < MAX_MEM; i++) {
      AD[i] = 0;
    }
  }
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

  // program counter
  TWO_BYTE PC = 0;

  // stack pointer
  BYTE SP = 0x01;
  BYTE SP_MIN = 0x00;
  BYTE SP_MAX = 0xFF;

  // accumulator, index registers, processor status
  BYTE AC, IRX, IRY, PS = 0;

  // bit positions for processor status flags
  static constexpr BYTE CARRY_FLAG = 1 << 0;        // Bit 0
  static constexpr BYTE ZERO_FLAG = 1 << 1;         // Bit 1
  static constexpr BYTE INTERRUPT_DISABLE = 1 << 2; // Bit 2
  static constexpr BYTE DECIMAL_MODE = 1 << 3;      // Bit 3
  static constexpr BYTE BREAK_COMMAND = 1 << 4;     // Bit 4
  static constexpr BYTE UNUSED = 1 << 5;            // Bit 5
  static constexpr BYTE OVERFLOW_FLAG = 1 << 6;     // Bit 6
  static constexpr BYTE NEGATIVE_FLAG = 1 << 7;     // Bit 7

  // operation codes
  static const BYTE LDA_IMMEDIATE = 0xA9;
  static const BYTE LDA_ZP = 0xA5;
  static const BYTE JMP_A = 0x4C;
  static const BYTE JMP_I = 0x6C;
  static const BYTE JSR_A = 0x20;
  static const BYTE RTS_I = 0x60;

  void LdaStatus() {
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
  }

  // reads from memory without incrementing program counter
  BYTE ReadData(MEM &mem, TWO_BYTE &address) {
    BYTE data = mem.Read(address);
    std::cout << "Reading data from address: " << std::hex
              << static_cast<int>(address) << " data: " << std::hex
              << static_cast<int>(data) << "\n";
    return data;
  }

  // reads from memory and increments program counter
  BYTE ReadMemory(unsigned int &cycles, MEM &mem, TWO_BYTE &address) {
    BYTE data = ReadData(mem, address);
    PC++;
    cycles--;
    return data;
  }

  TWO_BYTE ReadAddress(unsigned int &cycles, MEM &mem, TWO_BYTE &address) {
    BYTE lowByte = ReadMemory(cycles, mem, address);
    BYTE highByte = ReadMemory(cycles, mem, address);
    TWO_BYTE fullAddress = (static_cast<TWO_BYTE>(highByte) << 8) | lowByte;
    std::cout << "Reading address: " << std::hex
              << static_cast<int>(fullAddress) << "\n";
    return fullAddress;
  }

  void WriteMemory(MEM &mem, TWO_BYTE &address, BYTE &value) {
    mem.Write(address, value);
    std::cout << "Writing to memory address: " << std::hex
              << static_cast<int>(address) << " value: " << std::hex
              << static_cast<int>(value) << "\n";
  }

  void HandlerLdaImmediate(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to load accumulator immediately" << "\n";
    BYTE data = ReadMemory(cycles, mem, PC);
    AC = data;
    LdaStatus();
  }

  void HandlerLdaZp(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to load accumulator zero flag" << "\n";
    TWO_BYTE address = ReadAddress(cycles, mem, PC);
    BYTE data = ReadData(mem, address);
    AC = data;
    LdaStatus();
  }

  void HandlerJumpAbsolute(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to jump absolute" << "\n";
    TWO_BYTE address = ReadAddress(cycles, mem, PC);
    BYTE data = ReadData(mem, address);
    PC = data;
  }

  void HandlerJumpIndirect(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to jump indirect" << "\n";
  }

  void HandlerJumpToSubroutineAbsolute(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to jump to subroutine absolute" << "\n";
  }

  void HandlerReturnFromSubroutineImplied(unsigned int &cycles, MEM &mem) {
    std::cout << "Instruction is to return from subroutine implied" << "\n";
  }

  void setProcessorStatus(BYTE flag) { PS |= flag; }
  void clearProcessorStatus(BYTE flag) { PS &= ~flag; }

  void Initialize(MEM &mem) {
    mem.Initialize();
    PC = 0xFFFC;
  }

  void Execute(MEM &mem, unsigned int cycles) {

    std::cout << "Starting execution for " << cycles << " cycles." << "\n";
    std::cout << "Initial State:" << "\n";
    std::cout << "PC: " << std::hex << static_cast<int>(PC) << "\n";
    std::cout << "SP: " << std::hex << static_cast<int>(SP) << "\n";
    std::cout << "AC: " << std::hex << static_cast<int>(AC) << "\n";
    std::cout << "IRX: " << std::hex << static_cast<int>(IRX) << "\n";
    std::cout << "IRY: " << std::hex << static_cast<int>(IRY) << "\n";
    std::cout << "PS: " << std::hex << static_cast<int>(PS) << "\n";

    while (cycles > 0) {
      BYTE instruction = ReadMemory(cycles, mem, PC);
      switch (instruction) {
      case LDA_IMMEDIATE: {
        HandlerLdaImmediate(cycles, mem);
      } break;
      case LDA_ZP: {
        HandlerLdaZp(cycles, mem);
      } break;
      case JMP_A: {
        HandlerJumpAbsolute(cycles, mem);
      } break;
      case JMP_I: {
        HandlerJumpIndirect(cycles, mem);
      } break;
      case JSR_A: {
        HandlerJumpIndirect(cycles, mem);
      } break;
      case RTS_I: {
        HandlerReturnFromSubroutineImplied(cycles, mem);
      } break;
      default: {
        std::cout << "Unknown instruction: " << std::hex
                  << static_cast<int>(instruction) << "\n";
      }
      }
    }

    std::cout << "Final State:" << "\n";
    std::cout << "PC: " << std::hex << static_cast<int>(PC) << "\n";
    std::cout << "SP: " << std::hex << static_cast<int>(SP) << "\n";
    std::cout << "AC: " << std::hex << static_cast<int>(AC) << "\n";
    std::cout << "IRX: " << std::hex << static_cast<int>(IRX) << "\n";
    std::cout << "IRY: " << std::hex << static_cast<int>(IRY) << "\n";
    std::cout << "PS: " << std::hex << static_cast<int>(PS) << "\n";
  }
};

int main(int argc, char *argv[]) {
  std::cout << "Hello world!" << "\n";
  CPU cpu = CPU{};
  MEM mem = MEM{};

  cpu.Initialize(mem);

  mem.Write(0xFFFC, CPU::LDA_IMMEDIATE);
  mem.Write(0xFFFD, 0x42);
  mem.Write(0xFFFE, CPU::LDA_ZP);
  mem.Write(0xFFFF, 0x69);
  mem.Write(0x0000, 0x68);
  mem.Write(0xd1, 0x67);
  mem.Write(0x0001, CPU::LDA_IMMEDIATE);
  mem.Write(0x0002, 0x43);
  cpu.Execute(mem, 7);

  return 0;
}
