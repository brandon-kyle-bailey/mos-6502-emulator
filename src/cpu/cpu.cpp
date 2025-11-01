#include "cpu.h"

void CPU::setFlag(uint8_t flag) { CPU::PS |= flag; }
void CPU::clearFlag(uint8_t flag) { CPU::PS &= ~flag; }

uint8_t CPU::read(uint16_t address) { return bus->read(address); }
void CPU::write(uint16_t address, uint8_t value) {
  bus->write(address, value);
};

void CPU::nop(int &cycles) {
  CPU::PC++;
  cycles--;
}

void CPU::lda_immediate(int &cycles) {
  uint8_t value = CPU::read(CPU::PC++);
  CPU::AC = value;
  // set zero flag
  if (CPU::AC == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::AC & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::ldx_immediate(int &cycles) {
  uint8_t value = CPU::read(CPU::PC++);
  CPU::IRX = value;
  // set zero flag
  if (CPU::IRX == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::IRX & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::ldy_immediate(int &cycles) {
  uint8_t value = CPU::read(CPU::PC++);
  CPU::IRY = value;
  // set zero flag
  if (CPU::IRY == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::IRY & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::sta_zp(int &cycles) {
  uint8_t location = CPU::read(CPU::PC++);
  cycles--;
  CPU::write(location, CPU::AC);
  cycles--;
}

void CPU::sta_abs(int &cycles) {
  uint8_t low = CPU::read(CPU::PC++);
  cycles--;
  uint8_t high = CPU::read(CPU::PC++);
  cycles--;
  uint16_t location = (static_cast<uint16_t>(high) << 8) | low;
  CPU::write(location, CPU::AC);
  cycles--;
}

void CPU::stx_zp(int &cycles) {
  uint8_t location = CPU::read(CPU::PC++);
  cycles--;
  CPU::write(location, CPU::IRX);
  cycles--;
}

void CPU::sty_zp(int &cycles) {
  uint8_t location = CPU::read(CPU::PC++);
  cycles--;
  CPU::write(location, CPU::IRY);
  cycles--;
}

void CPU::tax(int &cycles) {
  CPU::IRX = CPU::AC;
  // set zero flag
  if (CPU::IRX == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::IRX & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::tay(int &cycles) {
  CPU::IRY = CPU::AC;
  // set zero flag
  if (CPU::IRY == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::IRY & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::txa(int &cycles) {
  CPU::AC = CPU::IRX;
  // set zero flag
  if (CPU::AC == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::AC & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::tya(int &cycles) {
  CPU::AC = CPU::IRY;
  // set zero flag
  if (CPU::AC == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::AC & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::tsx(int &cycles) {
  CPU::IRX = CPU::SP;
  // set zero flag
  if (CPU::IRX == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::IRX & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
  cycles--;
}

void CPU::txs(int &cycles) {
  CPU::SP = CPU::IRX;
  cycles--;
}

void CPU::pha(int &cycles) {
  uint16_t address = 0x0100 + CPU::SP;
  cycles--;
  write(address, CPU::AC);
  CPU::SP--;
  cycles--;
}

void CPU::php(int &cycles) {
  uint16_t address = 0x0100 + CPU::SP;
  cycles--;
  write(address, CPU::PS);
  CPU::SP--;
  cycles--;
}

void CPU::pla(int &cycles) {
  CPU::read(0x0100 + CPU::SP);
  cycles--;
  CPU::SP++;
  cycles--;
  CPU::AC = CPU::read(0x0100 + CPU::SP);
  cycles--;
  // set zero flag
  if (CPU::AC == 0) {
    CPU::setFlag(CPU::ZERO_FLAG);
  } else {
    CPU::clearFlag(CPU::ZERO_FLAG);
  }
  // set negative flag
  if (CPU::AC & 0x80) {
    CPU::setFlag(CPU::NEGATIVE_FLAG);
  } else {
    CPU::clearFlag(CPU::NEGATIVE_FLAG);
  }
}

void CPU::plp(int &cycles) {
  CPU::read(0x0100 + CPU::SP);
  cycles--;
  CPU::SP++;
  cycles--;
  CPU::PS = CPU::read(0x0100 + CPU::SP);
  cycles--;
}

void CPU::jmp_absolute(int &cycles) {
  uint8_t low = CPU::read(CPU::PC++);
  cycles--;
  uint8_t high = CPU::read(CPU::PC++);
  cycles--;
  // little-endian
  CPU::PC = (static_cast<uint16_t>(high) << 8) | low;
}

void CPU::jmp_indirect(int &cycles) {
  uint8_t low = CPU::read(CPU::PC++);
  cycles--;
  uint8_t high = CPU::read(CPU::PC++);
  cycles--;
  // little-endian
  uint16_t location = (static_cast<uint16_t>(high) << 8) | low;

  uint8_t indirect_low = CPU::read(location);
  cycles--;
  // wrap page boundary bug
  uint8_t indirect_high =
      CPU::read((location & 0xFF00) | ((location + 1) & 0x00FF));
  cycles--;

  // little-endian
  CPU::PC = (static_cast<uint16_t>(indirect_high) << 8) | indirect_low;
}

void CPU::jsr(int &cycles) {
  uint8_t low = CPU::read(CPU::PC++);
  cycles--;
  uint8_t high = CPU::read(CPU::PC++);
  cycles--;
  uint16_t return_address = CPU::PC - 1;
  CPU::write(0x0100 + SP, (return_address >> 8) & 0xFF);
  CPU::SP--;
  cycles--;
  CPU::write(0x0100 + CPU::SP, return_address & 0xFF);
  CPU::SP--;
  cycles--;
  CPU::PC = (static_cast<uint16_t>(high) << 8) | low;
  cycles--;
}

void CPU::rts(int &cycles) {
  CPU::SP++;
  cycles--;

  uint8_t low = CPU::read(0x0100 + CPU::SP);
  CPU::SP++;
  cycles--;
  uint8_t high = CPU::read(0x0100 + CPU::SP);
  cycles--;

  CPU::PC = (static_cast<uint16_t>(high) << 8) | low;
  CPU::PC++;
  cycles--;
}

void CPU::execute(int cycles) {
  std::cout << "Starting execution for " << cycles << " cycles." << "\n";
  while (cycles > 0) {
    uint8_t opcode = CPU::read(CPU::PC++);
    cycles--;
    switch (opcode) {
    case CPU::NOP: {
      CPU::nop(cycles);
    } break;
    case CPU::LDA_IMMEDIATE: {
      CPU::lda_immediate(cycles);
    } break;
    case CPU::LDX_IMMEDIATE: {
      CPU::ldx_immediate(cycles);
    } break;
    case CPU::LDY_IMMEDIATE: {
      CPU::ldy_immediate(cycles);
    } break;
    case CPU::STA_ZP: {
      CPU::sta_zp(cycles);
    } break;
    case CPU::STA_ABS: {
      CPU::sta_abs(cycles);
    } break;
    case CPU::STX_ZP: {
      CPU::stx_zp(cycles);
    } break;
    case CPU::STY_ZP: {
      CPU::sty_zp(cycles);
    } break;
    case CPU::TAX: {
      CPU::tax(cycles);
    } break;
    case CPU::TAY: {
      CPU::tay(cycles);
    } break;
    case CPU::TXA: {
      CPU::txa(cycles);
    } break;
    case CPU::TYA: {
      CPU::tya(cycles);
    } break;
    case CPU::TSX: {
      CPU::tsx(cycles);
    } break;
    case CPU::TXS: {
      CPU::txs(cycles);
    } break;
    case CPU::PHA: {
      CPU::pha(cycles);
    } break;
    case CPU::PHP: {
      CPU::php(cycles);
    } break;
    case CPU::PLA: {
      CPU::pla(cycles);
    } break;
    case CPU::PLP: {
      CPU::plp(cycles);
    } break;
    case CPU::JMP_ABSOLUTE: {
      CPU::jmp_absolute(cycles);
    } break;
    case CPU::JMP_INDIRECT: {
      CPU::jmp_indirect(cycles);
    } break;
    case CPU::JSR: {
      CPU::jsr(cycles);
    } break;
    case CPU::RTS: {
      CPU::rts(cycles);
    } break;
    default:
      std::cout << "Unknown opcode: " << std::hex << +opcode << "\n";
      break;
    }
  }
  std::cout << "Finishing execution for " << cycles << " cycles." << "\n";
}

void CPU::load_program(std::vector<uint8_t> const &program,
                       uint16_t start_address) {
  for (size_t i = 0; i < program.size(); i++) {
    CPU::write(start_address + i, program[i]);
  }
}
