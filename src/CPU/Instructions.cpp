#include "CPU.h"
#include <assert.h>

void CPU::adc(Register &reg, const uint8_t n) {
  /*
  ADC A,n  - Add n + Carry flag to A.

          n = A,B,C,D,E,H,L,(HL),#

          Flags affected:
          Z - Set if result is zero.
          N - Reset.
          H - Set if carry from bit 3.
          C - Set if carry from bit 7
  */
  uint8_t res = alu.add(reg, n, F.carry());
  reg = res;
  if (reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
  F.clear_subtract();
}

void CPU::add(Register &reg, const uint8_t n) {
  /*
  ADD A,n - Add n to A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set if carry from bit 3.
                  C - Set if carry from bit 7.
  */
  uint8_t res = alu.add(reg, n, 0);
  reg = res;
  if (reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
  F.clear_subtract();
}

void CPU::add(RegisterPair &reg, const uint16_t n) {
  /*
  ADD HL,n  - Add n to HL.
          n = BC,DE,HL

          Flags affected:
                  Z - Not affected
                  N - Reset.
                  H - Set if carry from bit 11.
                  C - Set if carry from bit 15.
  */
  uint16_t res = alu.add(reg, n, 0);
  reg = res;
  F.clear_subtract();
}

void CPU::addSigned(uint16_t &reg, const int8_t n) {
  /*
  ADD SP,n - Add n to Stack Pointer (SP).

          n = one byte signed immediate value

          Flags affected:
                  Z - Reset
                  N - Reset.
                  H - Set or reset according to operation.
                  C - Set or reset according to operation.
  */
  uint16_t temp = reg + n;

  if(((reg ^ n ^ (temp & 0xFFFF)) & 0x10) == 0x10) {
    F.set_half_carry();
  }
  else {
    F.clear_half_carry();
  }
  if(((reg ^ n ^ (temp & 0xFFFF)) & 0x100) == 0x100) {
    F.set_carry();
  }
  else {
    F.clear_carry();
  }

  F.clear_zero();
  F.clear_subtract();
  reg = temp;
}

void CPU::andReg(Register &reg, const uint8_t n) {
  /*
  AND n  - Logically AND n with A, result in A.

          n = A,B,C,D,E,H,L,(HL),#

          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set.
                  C - Reset.
  */
  reg = reg & n;
  if (reg.value() == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
  F.clear_subtract();
  F.set_half_carry();
  F.clear_carry();
}

void CPU::bit(Register &r, const uint8_t b) {
  /*
  BIT b,r  - Test bit b in register r.

          b = 0-7, r = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if bit b of register r is 0.
                  N - Reset.
                  H - Set.
                  C - Not affected.
  */
  uint8_t res = (r.value() >> b) & 0x01;
  if (res == 0) {
    F.set_zero();
  }
  else {
    F.clear_zero();
  }
  F.clear_subtract();
  F.set_half_carry();
}

void CPU::callN(const uint16_t n) {
  /*
  CALL n        - Push address of next instruction onto

                  stack and then jump to address n.

          Flags affected:
                  None
  */
  // push(SP); //TODO: implement stack push
  stackPush(PC);
  PC = n;
}

void CPU::ccf(void) {
  /*
  CCF           - Complement carry flag.
          If C flag is set then reset it.
          If C flag is reset then set it.

          Flags affected:
                  Z - Not affected.
                  N - Reset.
                  H - Reset.
                  C - Complemented.
  */
  F.clear_subtract();
  F.clear_half_carry();
  if (F.carry())
    F.clear_carry();
  else
    F.set_carry();
}

void CPU::daa(void) {
  /*
  DAA           - Decimal adjust register A.

          This instruction adjusts register A so that the
          correct representation of Binary Coded Decimal
          (BCD) is obtained.

          Flags affected:
                  Z - Set if register A is zero.
                  N - Not affected.
                  H - Reset.
                  C - Set of reset according to operation.
  */
  int16_t t = A.value();
  if(!F.subtract()){
      if(F.half_carry() || ((t & 0xF) > 9)){
        t += 0x06;
      }
      if(F.carry() || (t > 0x9F)){
        t += 0x60;
        F.set_carry();
      }
  }
  else {
       if(F.half_carry()) {
          t = (t - 6) & 0xFF;
        }

        if(F.carry()){
          t -= 0x60;
        }
  }
  F.clear_half_carry();
  t &= 0xFF;
  if (t ==0)
    F.set_zero();
  else
    F.clear_zero();
  A = t;
}

void CPU::cp(Register &reg, const uint8_t n) {
  /*
  CP n          - Compare A with n.
          This is basically an A - n subtraction
          instruction but the results are thrown away.

          n = A,B,C,D,E,H,L,(HL),#

          Flags affected:
                  Z - Set if result is zero. (Set if A = n)
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Set for no borrow. (Set if A < n.)
  */
  if(reg.value() == n)
    F.set_zero();
  else
    F.clear_zero();

  F.set_subtract();

  if((n&0x0F) > (reg.value()&0x0F))
    F.set_half_carry();
  else
    F.clear_half_carry();

  if(reg.value() < n)
    F.set_carry();
  else
    F.clear_carry();
}

void CPU::cpl(void) {
  /*
  CPL           - Complement A register. (Flip all bits.)

          Flags affected:
                  Z - Not affected.
                  N - Set.
                  H - Set.
                  C - Not affected.
  */
  A = ~A.value();
  F.set_subtract();
  F.set_half_carry();
}

void CPU::inc(Register &reg) {
  /*
  INC n         - Increment register n.

          n = A,B,C,D,E,H,L,(HL)

    Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set if carry from bit 3.
                  C - Not affected.
  */
  if(((uint8_t)reg & 0xF) == 0xF)
    F.set_half_carry();
  else
    F.clear_half_carry();

  reg = (uint8_t)reg + 1;

  if((uint8_t)reg == 0)
    F.set_zero();
  else
    F.clear_zero();

  F.clear_subtract();
}

void CPU::dec(Register &reg) {
  /*
  DEC n         - Decrement register n.

          n = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Not affected.
  */
  if(((uint8_t)reg &0x0F) == 0)
    F.set_half_carry();
  else
    F.clear_half_carry();

  reg = (uint8_t)reg - 1;

  if((uint8_t)reg == 0)
    F.set_zero();
  else
    F.clear_zero();
  F.set_subtract();
}

void CPU::inc(RegisterPair &reg) {
  /*
  INC nn        - Increment register nn.
          n = BC,DE,HL,SP

          Flags affected:
                  None
  */
  reg = (uint16_t)reg + 1;
}

void CPU::dec(RegisterPair &reg) {
  /*
  DEC nn        - Decrement register nn.

          nn = BC,DE,HL,SP

          Flags affected:
                  None
  */
  reg = (uint16_t)reg - 1;
}

void CPU::jp(const uint16_t addr) {
  /*
  JP n          - Jump to address n.

          n = two byte immediate value. (LSByte first)
          Flags affected:
                  None
  */
  PC = addr;
}


void CPU::jr(const int8_t n) {
  /*
  JR n          - Add n to current address and jump to it.
          n = one byte signed immediate value.

          Flags affected:
                  None
  */
  PC += n;
}

void CPU::halt(void) {
  /*
  HALT          - Power down CPU until an interrupt occurs.
          Flags affected:
                  None
  */
  assert(0);
}

/*
SET b,r       - Set bit b in register r.
        b = 0-7, r = A,B,C,D,E,H,L,(HL)
        Flags affected        None
*/
void CPU::setBit(Register &r, const uint8_t b) {
  r = r | (0x01 << b);
}


void CPU::load(Register &reg, const uint8_t n) {
  /*
  LD n,A        - Put value n into A.

  n = A,B,C,D,E,H,L,(BC,(DE),(HL),(nnnn)

          Flags affected:
                  None
  */
  reg = n;
}

void CPU::load(uint8_t &reg, const uint8_t n) {
  reg = n;
}

void CPU::load(RegisterPair &reg, const uint16_t n) {
  /*
  LD n,nn       - Put value nn into n.
          n = BC,DE,HL,SP
          nn = 16 bit immediate value
          Flags affected:    Non
  */
  reg = n;
}

void CPU::load(uint16_t &reg, const uint16_t n) {
  reg = n;
}

void CPU::load(MemRef &reg, const uint8_t n) {
  reg = n;
}

void CPU::load(MemRef &reg, const uint16_t n) {
  reg = (uint16_t)n;
}

void CPU::nop(void) {
  /*
  NOP           - No operation.
          Flags affected:     None
  */
}

void CPU::scf(void) {
  /*
  SCF           - Set Carry flag.
          Flags affected:
                  Z - Not affected.
                  N - Reset.
                  H - Reset.
                  C - Set.
  */
  F.set_carry();
  F.clear_subtract();
  F.clear_half_carry();
}

void CPU::sla(Register &reg) {
  /*
  SLA n         - Shift n left into Carry. LSBit of n set to 0.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  uint8_t msb = reg.value() >> 7;
  if (msb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  F.clear_subtract();
  F.clear_half_carry();

  reg = reg.value() << 1;
  if ((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }

}

void CPU::sra(Register &reg) {
  /*
  SRA n         - Shift n right into Carry. MSBit doesn't change.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  uint8_t msb = (uint8_t)reg & 0x80;
  uint8_t lsb = (uint8_t)reg & 0x01;
  if (lsb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  F.clear_subtract();
  F.clear_half_carry();

  reg = ((uint8_t)reg >> 1) | msb;
  if ((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}

void CPU::srl(Register &reg) {
  /*
  SRL n         - Shift n right into Carry. MSBit of n set to 0.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  uint8_t lsb = (uint8_t)reg & 0x01;
  if (lsb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  F.clear_subtract();
  F.clear_half_carry();

  reg = ((uint8_t)reg >> 1);
  if ((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}

void CPU::rst(const uint16_t n) {
  /*
  RST n         - Push present address onto stack.
                  Jump to address $0000 + n.
          n = $00,$08,$10,$18,$20,$28,$30,$38
          Flags affected: none
  */
  stackPush(PC);
  PC = n;
}

void CPU::orReg(Register &reg, const uint8_t n) {
  /*
  OR n          - Logical OR n with register A, result in A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Reset.
  */
  reg = reg.value() | n;
  if((uint8_t)reg == 0) {
    F.set_zero();
  }
  else {
    F.clear_zero();
  }
  F.clear_carry();
  F.clear_subtract();
  F.clear_half_carry();
}

void CPU::res(Register &reg, const uint8_t b) {
  /*
  RES b,r       - Reset bit b in register r.
          b = 0-7, r = A,B,C,D,E,H,L,(HL)
          Flags affected:    None
  */
  reg = reg.value() & ~(0x01 << b);
}

void CPU::ret(void) {
  /*
  RET           - Pop two bytes from stack & jump to that address.
          Flags affected:   None
  */
  uint16_t stacked_pc = stackPop();
  jp(stacked_pc);
}

void CPU::rr(Register &reg) {
  /*
  RR n          - Rotate n right through Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  uint8_t c = F.carry() << 7;
  uint8_t lsb = (uint8_t)reg & 0x01;
  if(lsb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  reg = (uint8_t)reg>>1 | c;

  F.clear_subtract();
  F.clear_half_carry();
  if((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}

void CPU::rl(Register &reg) {
  /*
  RL n          - Rotate n left through Carry flag.

          n = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  uint8_t c = F.carry() & 0x01;
  uint8_t msb = (uint8_t)reg & 0x80;
  if(msb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  reg = (uint8_t)(reg<<1) | c;

  F.clear_subtract();
  F.clear_half_carry();

  if((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}

void CPU::rrc(Register &reg) {
  /*
  RRC n         - Rotate n right. Old bit 0 to Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  uint8_t c = (uint8_t)reg << 7;
  uint8_t lsb = (uint8_t)reg & 0x01;
  if(lsb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  reg = (uint8_t)reg>>1 | c;

  F.clear_subtract();
  F.clear_half_carry();
  if((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}

void CPU::rlc(Register &reg) {
  /*
  RLC n         - Rotate n left. Old bit 7 to Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  uint8_t c = (uint8_t)reg&0x01;
  uint8_t msb = (uint8_t)reg >> 7;
  if(msb) {
    F.set_carry();
  } else {
    F.clear_carry();
  }

  reg = (uint8_t)reg<<1 | msb;

  F.clear_subtract();
  F.clear_half_carry();
  if((uint8_t)reg == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
}



void CPU::sbc(Register &reg, const uint8_t n) {
  /*
  SBC A,n       - Subtract n + Carry flag from A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Set if no borrow.
  */
  reg = alu.sub((uint8_t)reg, n, F.carry());
}

void CPU::sub(Register &reg, const uint8_t n) {
  /*
  SUB n         - Subtract n from A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Set if no borrow.
  */
  reg = alu.sub((uint8_t)reg, n, 0);
}

void CPU::stop(void) {
  /*
  STOP          - ??? affected:    ?
  */
  assert(0);
}

void CPU::swap(Register &reg) {
  /*
  SWAP n        - Swap upper & lower bits of n.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Reset.
  */
  uint8_t low, high;
  low = reg.value()&0x0F;
  high = reg.value()&0xF0;
  reg = (low<<4) | (high>>4);

  if(reg.value() == 0)
    F.set_zero();
  else
    F.clear_zero();

  F.clear_subtract();
  F.clear_half_carry();
  F.clear_carry();
}

void CPU::xorReg(Register &reg, const uint8_t n) {
  /*
  XOR n         - Logical exclusive OR n with
                  register A, result in A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Reset.
  */
  reg = reg.value() ^ n;
  if(reg.value() == 0 )
    F.set_zero();
  else
    F.clear_zero();

  F.clear_subtract();
  F.clear_half_carry();
  F.clear_carry();
}
