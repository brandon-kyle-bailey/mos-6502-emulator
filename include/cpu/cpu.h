#include "bus/bus.h"

#include <cstdint>
#include <iostream>

struct CPU {
  Bus *bus;

  // registers
  uint8_t AC = 0x00;
  uint16_t PC = 0x0000;
  uint8_t SP = 0xFD; // Stack pointer: stack is at 0x0100-0x01FF, grows downward
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
      LDX_IMMEDIATE = 0xA2, LDY_IMMEDIATE = 0xA0, STA_ZP = 0x85, STA_ABS = 0x8D,
      STX_ZP = 0x86, STY_ZP = 0x84,

      // register transfers
      TAX = 0xAA, TAY = 0xA8, TXA = 0x8A, TYA = 0x98,

      // stack operations
      TSX = 0xBA, TXS = 0x9A, PHA = 0x48, PHP = 0x08, PLA = 0x68, PLP = 0x28,

      // logical
      AND_IMMEDIATE = 0x29, ORA_IMMEDIATE = 0x09, EOR_IMMEDIATE = 0x49,

      // arithmetic
      // increment/decrement
      INC_ZP = 0xE6, DEC_ZP = 0xC6, INX = 0xE8, INY = 0xC8, DEX = 0xCA,
      DEY = 0x88,

      // shifts

      // jumps and calls
      JMP_ABSOLUTE = 0x4C, JMP_INDIRECT = 0x6C, JSR = 0x20, RTS = 0x60,

      // branches

      // status flag changes
      CLC = 0x18, CLD = 0xD8, CLI = 0x58, CLV = 0xB8, SEC = 0x38, SED = 0xF8,
      SEI = 0x78,

      // system functions
      BRK = 0x00, NOP = 0xEA, RTI = 0x40;

  void setFlag(uint8_t flag);
  void clearFlag(uint8_t flag);

  uint8_t read(uint16_t address);
  void write(uint16_t address, uint8_t value);

  void nop(int &cycles);

  void brk(int &cycles);

  void rti(int &cycles);

  void lda_immediate(int &cycles);

  void ldx_immediate(int &cycles);

  void ldy_immediate(int &cycles);

  void sta_zp(int &cycles);

  void sta_abs(int &cycles);

  void stx_zp(int &cycles);

  void sty_zp(int &cycles);

  void tax(int &cycles);

  void tay(int &cycles);

  void txa(int &cycles);

  void tya(int &cycles);

  void tsx(int &cycles);

  void txs(int &cycles);

  void pha(int &cycles);

  void php(int &cycles);

  void pla(int &cycles);

  void plp(int &cycles);

  void jmp_absolute(int &cycles);

  void jmp_indirect(int &cycles);

  void jsr(int &cycles);

  void rts(int &cycles);

  void execute(int cycles);

  void load_program(const std::vector<uint8_t> &program,
                    uint16_t start_address);
};
