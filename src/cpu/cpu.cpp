#include "cpu.h"

void CPU::setFlag(uint8_t flag) { PS |= flag; }
void CPU::clearFlag(uint8_t flag) { PS &= ~flag; }

uint8_t CPU::read(uint16_t address) {
  uint8_t value = bus->read(address);
  return value;
}
void CPU::write(uint16_t address, uint8_t value) {
  if (!bus) {
    return;
  }
  bus->write(address, value);
};

void CPU::nop(int &cycles) {
  PC++;
  cycles--;
}

void CPU::brk(int &cycles) {
  PC++; // advance past BRK
  cycles--;

  // Push PC high, then low
  write(0x0100 + SP--, (PC >> 8) & 0xFF);
  cycles--;
  write(0x0100 + SP--, PC & 0xFF);
  cycles--;

  // Push status register with B flag set for stack only
  write(0x0100 + SP--, PS | 0x10);
  cycles--;
  setFlag(ID_FLAG);

  uint8_t low = read(0xFFFE);
  cycles--;
  uint8_t high = read(0xFFFF);
  cycles--;
  PC = (static_cast<uint16_t>(high) << 8) | low;
};

void CPU::rti(int &cycles) {
  // Pop status register
  SP++;
  cycles--;
  PS = read(0x0100 + SP);
  cycles--;
  // Pop PC low byte
  SP++;
  cycles--;
  uint8_t low = read(0x0100 + SP);
  cycles--;
  // Pop PC high byte
  SP++;
  cycles--;
  uint8_t high = read(0x0100 + SP);
  cycles--;
  PC = (static_cast<uint16_t>(high) << 8) | low;
  cycles--;
};

void CPU::lda_immediate(int &cycles) {
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
  cycles--;
}

void CPU::ldx_immediate(int &cycles) {
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
  cycles--;
}

void CPU::ldy_immediate(int &cycles) {
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
  cycles--;
}

void CPU::sta_zp(int &cycles) {
  uint8_t location = read(PC++);
  cycles--;
  write(location, AC);
  cycles--;
}

void CPU::sta_abs(int &cycles) {
  uint8_t low = read(PC++);
  cycles--;
  uint8_t high = read(PC++);
  cycles--;
  uint16_t location = (static_cast<uint16_t>(high) << 8) | low;
  write(location, AC);
  cycles--;
}

void CPU::stx_zp(int &cycles) {
  uint8_t location = read(PC++);
  cycles--;
  write(location, IRX);
  cycles--;
}

void CPU::sty_zp(int &cycles) {
  uint8_t location = read(PC++);
  cycles--;
  write(location, IRY);
  cycles--;
}

void CPU::tax(int &cycles) {
  IRX = AC;
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
  cycles--;
}

void CPU::tay(int &cycles) {
  IRY = AC;
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
  cycles--;
}

void CPU::txa(int &cycles) {
  AC = IRX;
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
  cycles--;
}

void CPU::tya(int &cycles) {
  AC = IRY;
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
  cycles--;
}

void CPU::tsx(int &cycles) {
  IRX = SP;
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
  cycles--;
}

void CPU::txs(int &cycles) {
  SP = IRX;
  cycles--;
}

void CPU::pha(int &cycles) {
  write(0x0100 + SP, AC);
  cycles--;
  SP--;
  cycles--;
}

void CPU::php(int &cycles) {
  write(0x0100 + SP, PS | BC_FLAG); // Always set break flag when pushing
  cycles--;
  SP--;
  cycles--;
}

void CPU::pla(int &cycles) {
  SP++;
  cycles--;
  AC = read(0x0100 + SP);
  cycles--;
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
}

void CPU::plp(int &cycles) {
  SP++;
  cycles--;
  PS = read(0x0100 + SP);
  cycles--;
}

void CPU::jmp_absolute(int &cycles) {
  uint8_t low = read(PC++);
  cycles--;
  uint8_t high = read(PC++);
  cycles--;
  // little-endian
  PC = (static_cast<uint16_t>(high) << 8) | low;
}

void CPU::jmp_indirect(int &cycles) {
  uint8_t low = read(PC++);
  cycles--;
  uint8_t high = read(PC++);
  cycles--;
  // little-endian
  uint16_t location = (static_cast<uint16_t>(high) << 8) | low;

  uint8_t indirect_low = read(location);
  cycles--;
  // wrap page boundary bug
  uint8_t indirect_high = read((location & 0xFF00) | ((location + 1) & 0x00FF));
  cycles--;

  // little-endian
  PC = (static_cast<uint16_t>(indirect_high) << 8) | indirect_low;
}

void CPU::jsr(int &cycles) {
  uint8_t low = read(PC++);
  cycles--;
  uint8_t high = read(PC++);
  cycles--;
  // Push high byte first (write then decrement)
  write(0x0100 + SP--, (PC-- >> 8) & 0xFF);
  cycles--;
  // Push low byte
  write(0x0100 + SP--, PC-- & 0xFF);
  cycles--;
  PC = (static_cast<uint16_t>(high) << 8) | low;
  cycles--;
}

void CPU::rts(int &cycles) {
  // Pop low byte
  SP++;
  cycles--;
  uint8_t low = read(0x0100 + SP);
  cycles--;
  // Pop high byte
  SP++;
  cycles--;
  uint8_t high = read(0x0100 + SP);
  cycles--;
  PC = (static_cast<uint16_t>(high) << 8) | low;
  PC++; // RTS returns to address after JSR
  cycles--;
}

void CPU::load_program(std::vector<uint8_t> const &program,
                       uint16_t start_address) {
  for (size_t i = 0; i < program.size(); i++) {
    write(start_address + i, program[i]);
    if (!bus) {
      return;
    }
  }
}

void CPU::execute(int cycles) {
  std::cout << "Starting execution for " << cycles << " cycles." << "\n";
  while (cycles > 0) {
    uint8_t opcode = read(PC++);
    cycles--;
    switch (opcode) {
    case NOP: {
      nop(cycles);
    } break;
    case BRK: {
      brk(cycles);
    } break;
    case RTI: {
      rti(cycles);
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
    case STA_ABS: {
      sta_abs(cycles);
    } break;
    case STX_ZP: {
      stx_zp(cycles);
    } break;
    case STY_ZP: {
      sty_zp(cycles);
    } break;
    case TAX: {
      tax(cycles);
    } break;
    case TAY: {
      tay(cycles);
    } break;
    case TXA: {
      txa(cycles);
    } break;
    case TYA: {
      tya(cycles);
    } break;
    case TSX: {
      tsx(cycles);
    } break;
    case TXS: {
      txs(cycles);
    } break;
    case PHA: {
      pha(cycles);
    } break;
    case PHP: {
      php(cycles);
    } break;
    case PLA: {
      pla(cycles);
    } break;
    case PLP: {
      plp(cycles);
    } break;
    case JMP_ABSOLUTE: {
      jmp_absolute(cycles);
    } break;
    case JMP_INDIRECT: {
      jmp_indirect(cycles);
    } break;
    case JSR: {
      jsr(cycles);
    } break;
    case RTS: {
      rts(cycles);
    } break;
    default:
      std::cout << "Unknown opcode: " << std::hex << +opcode << std::dec
                << "\n";
      break;
    }
  }
  std::cout << "Finishing execution for " << cycles << " cycles." << "\n";
}
