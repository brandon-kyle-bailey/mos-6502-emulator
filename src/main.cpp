#include <array>
#include <cstdint>
#include <iostream>

struct Device {
  virtual uint8_t read(uint16_t address) = 0;
  virtual void write(uint16_t address, uint8_t data) = 0;
  virtual ~Device() = default;
};

struct Ram : Device {

  // 2 KB internal RAM
  std::array<uint8_t, 0x0800> data{};

  uint8_t read(uint16_t address) override { return data[address & 0x07FF]; }

  void write(uint16_t address, uint8_t value) override {
    data[address & 0x07FF] = value;
  }
};

struct Bus {
  Ram *ram = nullptr;

  uint8_t read(uint16_t address) {
    std::cout << "Bus read from address: " << std::hex << address << "\n";
    if (address >= 0x0000 && address <= 0x1FFF) {
      return ram->read(address);
    };

    // return -1 to signify address invalid
    return -1;
  };

  void write(uint16_t address, uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
      ram->write(address, value);
    };
  };
};

struct CPU {
  Bus *bus;

  // registers
  uint8_t AC = 0x00;
  uint16_t PC = 0x0000;
  uint8_t SP = 0x00;
  uint8_t IRX = 0x00;
  uint8_t IRY = 0x00;
  uint8_t PS = 0x00;
  uint8_t CARRY_FLAG = 1 << 0;
  uint8_t ZERO_FLAG = 1 << 1;
  uint8_t ID_FLAG = 1 << 2;
  uint8_t DM_FLAG = 1 << 3;
  uint8_t BC_FLAG = 1 << 4;
  uint8_t OVERFLOW_FLAG = 1 << 6;
  uint8_t NEGATIVE_FLAG = 1 << 7;

  static constexpr uint8_t
      // load/store operations
      LDA_IMMEDIATE = 0xA9,
      LDX_IMMEDIATE = 0xA2, LDY_IMMEDIATE = 0xA0, STA_ZP = 0x85, STX_ZP = 0x86,
      STY_ZP = 0x84,

      // register transfers
      TAX = 0xAA, TAY = 0xA8, TXA = 0x8A, TYA = 0x98,

      // stack operations
      TSX = 0xBA, TXS = 0x9A, PHA = 0x48, PHP = 0x08, PLA = 0x68, PLP = 0x28,

      // logical
      // arithmetic
      // increment/decrement
      // shifts

      // jumps and calls
      JMP_ABSOLUTE = 0x4C, JMP_INDIRECT = 0x6C, JSR = 0x20, RTS = 0x60,

      // branches

      // status flag changes
      CLC = 0x18, CLD = 0xD8, CLI = 0x58, CLV = 0xB8, SEC = 0x38, SED = 0xF8,
      SEI = 0x78,

      // system functions
      BRK = 0x00, NOP = 0xEA, RTI = 0x40;

  void setFlag(uint8_t flag) { PS |= flag; }
  void clearFlag(uint8_t flag) { PS &= ~flag; }

  uint8_t read(uint16_t address) { return bus->read(address); }
  void write(uint16_t address, uint8_t value) { bus->write(address, value); };

  void nop(int &cycles) {
    PC++;
    cycles--;
  }

  void lda_immediate(int &cycles) {
    uint8_t value = read(PC++);
    AC = value;
    // set zero flag
    if (AC == 0) {
      setFlag(ZERO_FLAG);
    } else {
      clearFlag(ZERO_FLAG);
    }
    // set negative flag
    if (AC & 0x80) {
      setFlag(NEGATIVE_FLAG);
    } else {
      clearFlag(NEGATIVE_FLAG);
    }
    std::cout << "LDA Immediate executed, AC: " << std::hex << AC
              << " Processor Status: " << std::hex << +PS << "\n";
    cycles--;
  }

  void ldx_immediate(int &cycles) {
    uint8_t value = read(PC++);
    IRX = value;
    // set zero flag
    if (IRX == 0) {
      setFlag(ZERO_FLAG);
    } else {
      clearFlag(ZERO_FLAG);
    }
    // set negative flag
    if (IRX & 0x80) {
      setFlag(NEGATIVE_FLAG);
    } else {
      clearFlag(NEGATIVE_FLAG);
    }
    std::cout << "LDX Immediate executed, IRX: " << std::hex << IRX
              << " Processor Status: " << std::hex << +PS << "\n";
    cycles--;
  }

  void ldy_immediate(int &cycles) {
    uint8_t value = read(PC++);
    IRY = value;
    // set zero flag
    if (IRY == 0) {
      setFlag(ZERO_FLAG);
    } else {
      clearFlag(ZERO_FLAG);
    }
    // set negative flag
    if (IRY & 0x80) {
      setFlag(NEGATIVE_FLAG);
    } else {
      clearFlag(NEGATIVE_FLAG);
    }
    std::cout << "LDY Immediate executed, IRY: " << std::hex << IRY
              << " Processor Status: " << std::hex << +PS << "\n";
    cycles--;
  }

  void sta_zp(int &cycles) {
    uint8_t location = read(PC++);
    cycles--;
    write(location, AC);
    cycles--;
    std::cout << "STA Zero page executed, AC: " << std::hex << AC << "\n";
  }

  void stx_zp(int &cycles) {
    uint8_t location = read(PC++);
    cycles--;
    write(location, IRX);
    cycles--;
    std::cout << "STX Zero page executed, IRX: " << std::hex << IRX << "\n";
  }

  void sty_zp(int &cycles) {
    uint8_t location = read(PC++);
    cycles--;
    write(location, IRY);
    cycles--;
    std::cout << "STY Zero page executed, IRY: " << std::hex << IRY << "\n";
  }

  void execute(int cycles) {
    std::cout << "Starting execution for " << cycles << " cycles." << "\n";
    while (cycles > 0) {
      uint8_t opcode = read(PC++);
      std::cout << "opcode: " << std::hex << +opcode << "\n";
      cycles--;
      switch (opcode) {
      case NOP: {
        nop(cycles);
      } break;
      case LDA_IMMEDIATE: {
        lda_immediate(cycles);
      } break;
      case LDX_IMMEDIATE: {
        ldx_immediate(cycles);
      } break;
      case LDY_IMMEDIATE: {
        ldy_immediate(cycles);
      } break;
      case STA_ZP: {
        sta_zp(cycles);
      } break;
      case STX_ZP: {
        stx_zp(cycles);
      } break;
      case STY_ZP: {
        sty_zp(cycles);
      } break;
      default:
        std::cout << "Unknown opcode: " << std::hex << +opcode << "\n";
        break;
      }
    }
    std::cout << "Finishing execution for " << cycles << " cycles." << "\n";
  }
};

int main() {
  Ram ram = Ram{};
  Bus bus = Bus{&ram};
  CPU cpu = CPU{&bus};

  std::cout << cpu.AC << "\n";
  std::cout << cpu.IRX << "\n";
  std::cout << cpu.IRY << "\n";

  cpu.write(0x0000, 0xA9);
  cpu.write(0x0001, 0x67);
  cpu.write(0x0002, 0xA2);
  cpu.write(0x0003, 0x68);
  cpu.write(0x0004, 0xA0);
  cpu.write(0x0005, 0x69);
  cpu.execute(6);

  return 0;
}
