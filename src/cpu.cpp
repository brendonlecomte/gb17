#include <assert.h>

#include "alu.h"
#include "cpu.h"

CPU::CPU()
    : A(), F(), B(), C(), D(), E(), H(), L(), AF(A, F), BC(B, C), DE(D, E), HL(H, L), SP(0xFFFE), PC(0x0000), alu(F) {}

CPU::~CPU() {}

void CPU::adc(const uint8_t n) {
  /*
  ADC A,n  - Add n + Carry flag to A.

          n = A,B,C,D,E,H,L,(HL),#

          Flags affected:
          Z - Set if result is zero.
          N - Reset.
          H - Set if carry from bit 3.
          C - Set if carry from bit 7
  */
  uint8_t res = alu.add(A, n, F.carry());
  A = res;
  if (A == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
  F.clear_subtract();
}

void CPU::add_a(const uint8_t n) {
  /*
  ADD A,n - Add n to A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set if carry from bit 3.
                  C - Set if carry from bit 7.
  */
  uint8_t res = alu.add(A, n, 0);
  A = res;
  if (A == 0) {
    F.set_zero();
  } else {
    F.clear_zero();
  }
  F.clear_subtract();
}

void CPU::add_HL(const uint16_t n) {
  /*
  ADD HL,n  - Add n to HL.
          n = BC,DE,HL

          Flags affected:
                  Z - Not affected
                  N - Reset.
                  H - Set if carry from bit 11.
                  C - Set if carry from bit 15.
  */
  uint16_t res = alu.add(HL, n, 0);
  HL = res;
  F.clear_subtract();
}

void CPU::add_SP(const int8_t n) {
  /*
  ADD SP,n - Add n to Stack Pointer (SP).

          n = one byte signed immediate value

          Flags affected:
                  Z - Reset
                  N - Reset.
                  H - Set or reset according to operation.
                  C - Set or reset according to operation.
  */
  uint16_t res = alu.add(SP, n, 0);
  SP = res;
  F.clear_zero();
  F.clear_subtract();
}

void CPU::and_a(const uint8_t n) {
  /*
  AND n  - Logically AND n with A, result in A.

          n = A,B,C,D,E,H,L,(HL),#

          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set.
                  C - Reset.
  */
  A = A & n;
  if (A.value() == 0) {
    F.set_zero();
  }
  F.clear_subtract();
  F.set_half_carry();
  F.clear_carry();
}

void CPU::bit(const uint8_t b, const uint8_t r) {
  /*
  BIT b,r  - Test bit b in register r.

          b = 0-7, r = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if bit b of register r is 0.
                  N - Reset.
                  H - Set.
                  C - Not affected.
  */
  uint8_t res = r & (0x01 << b);
  if (res == 0) {
    F.set_zero();
  }
  F.clear_subtract();
  F.set_half_carry();
  F.clear_carry();
}

void CPU::call_n(const uint16_t n) {
  /*
  CALL n        - Push address of next instruction onto

                  stack and then jump to address n.

          Flags affected:
                  None
  */
  // push(SP); //TODO: implement stack push
  PC = n;
}

void CPU::call_cc(const uint8_t predicate, const uint16_t n) {
  /*CALL cc,n     - Call address n if following condition is true:
          cc = NZ, Call if Z flag is reset.
          cc = Z,  Call if Z flag is set.
          cc = NC, Call if C flag is reset.
          cc = C,  Call if C flag is set.

          Flags affected:
                  None
  */
  if (predicate) {
    call_n(n);
  }
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
  // int16_t t = *A;
  // if(!CPU_check_flag(SUBTRACT_FLAG)){
  //     if(CPU_check_flag(HALF_CARRY_FLAG) || ((t & 0xF) > 9)){
  //       t += 0x06;
  //     }
  //     if(CPU_check_flag(CARRY_FLAG) || (t > 0x9F)){
  //       t += 0x60;
  //       CPU_set_flag(CARRY_FLAG, 1);
  //     }
  // }
  // else {
  //      if(CPU_check_flag(HALF_CARRY_FLAG)) {
  //         t = (t - 6) & 0xFF;
  //       }
  //
  //       if(CPU_check_flag(CARRY_FLAG)){
  //         t -= 0x60;
  //       }
  // }
  // CPU_set_flag(HALF_CARRY_FLAG, 0);
  // t &= 0xFF;
  // CPU_set_flag(ZERO_FLAG, (t == 0));
  // *A = t;
  assert(0);
}

void CPU::cp(const uint8_t n) {
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
  // CPU_set_flag(ZERO_FLAG, (*A == n));
  // CPU_set_flag(SUBTRACT_FLAG, 1);
  // CPU_set_flag(HALF_CARRY_FLAG, (n&0x0F) > (*A&0x0F));
  // CPU_set_flag(CARRY_FLAG, (*A < n));
  assert(0);
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
  // uint8_t t = ~*A;
  // *A = t;
  // CPU_set_flag(SUBTRACT_FLAG, 1);
  // CPU_set_flag(HALF_CARRY_FLAG, 1);
  assert(0);
}

void CPU::inc_n(uint8_t *reg) {
  /*
  INC n         - Increment register n.

          n = A,B,C,D,E,H,L,(HL)

    Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Set if carry from bit 3.
                  C - Not affected.
  */
  assert(0);
}

void CPU::dec_n(uint8_t *reg) {
  /*
  DEC n         - Decrement register n.

          n = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Not affected.
  */
  assert(0);
}

void CPU::inc_nn(uint16_t *reg) {
  /*
  INC nn        - Increment register nn.
          n = BC,DE,HL,SP

          Flags affected:
                  None
  */
  assert(0);
}

void CPU::dec_nn(uint16_t *nn) {
  /*
  DEC nn        - Decrement register nn.

          nn = BC,DE,HL,SP

          Flags affected:
                  None
  */
  assert(0);
}

void CPU::di(void) {
  /*
  DI            - Disable interrupts.
          Flags affected: None
  */
  assert(0);
}

void CPU::ei(void) {
  /*
  EI            - Enable interrupts.
          This instruction enables the interrupts but not immediately.
          Interrupts are enabled after the instruction after EI is
          executed.

          Flags affected: None
  */
  assert(0);
}

void CPU::jp(const uint16_t addr) {
  /*
  JP n          - Jump to address n.

          n = two byte immediate value. (LSByte first)
          Flags affected:
                  None
  */
  assert(0);
}

void CPU::jp_hl(void) {
  /*
  JP cc,n       - Jump to address n if following condition
                  is true:
          n = two byte immediate value. (LSByte first.)
          cc = NZ, Jump if Z flag is reset.
    cc = Z,  Jump if Z flag is set.
          cc = NC, Jump if C flag is reset.
    cc = C,  Jump if C flag is set.

          Flags affected:
                  None
  */
  // TODO: Define for all these

  /*
  JP [HL]       - Jump to address contained in HL.

          Flags affected:
                  None
  */
  assert(0);
}

void CPU::jr(const uint8_t n) {
  /*
  JR n          - Add n to current address and jump to it.
          n = one byte signed immediate value.

          Flags affected:
                  None
  */
  assert(0);
}

void CPU::halt_cpu(void) {
  /*
  HALT          - Power down CPU until an interrupt occurs.
          Flags affected:
                  None
  */
  assert(0);
}

void CPU::set_b(const uint8_t b, uint8_t *r) {
  /*
  SET b,r       - Set bit b in register r.
          b = 0-7, r = A,B,C,D,E,H,L,(HL)
          Flags affected        None
  */
  assert(0);
}

void CPU::load_a(const uint8_t n) {
  /*
  LD n,A        - Put value n into A.

  n = A,B,C,D,E,H,L,(BC,(DE),(HL),(nnnn)

          Flags affected:
                  None
  */
  assert(0);
}

void CPU::load_16(uint16_t *reg, const uint16_t n) {
  /*
  LD n,nn       - Put value nn into n.
          n = BC,DE,HL,SP
          nn = 16 bit immediate value
          Flags affected:    Non
  */
  assert(0);
}

void CPU::nop(void) {
  /*
  NOP           - No operation.
          Flags affected:     None
  */
  assert(0);
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
  assert(0);
}

void CPU::sla(uint8_t *n) {
  /*
  SLA n         - Shift n left into Carry. LSBit of n set to 0.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  assert(0);
}

void CPU::sra(uint8_t *n) {
  /*
  SRA n         - Shift n right into Carry. MSBit doesn't change.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  assert(0);
}

void CPU::srl(uint8_t *n) {
  /*
  SRL n         - Shift n right into Carry. MSBit of n set to 0.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  assert(0);
}

void CPU::rst(const uint8_t n) {
  /*
  RST n         - Push present address onto stack.
                  Jump to address $0000 + n.
          n = $00,$08,$10,$18,$20,$28,$30,$38
          Flags affected: none
  */
  assert(0);
}

void CPU::or_a(const uint8_t n) {
  /*
  OR n          - Logical OR n with register A, result in A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Reset.
  */
  assert(0);
}

void CPU::pop(uint16_t *nn) {
  /*
  POP nn        - Pop two bytes off stack into register pair nn.
                  Increment Stack Pointer (SP) twice.
          nn = AF,BC,DE,HL
          Flags affected: None
  */
  assert(0);
}

void CPU::push(const uint16_t nn) {
  /*
  PUSH nn       - Push register pair nn onto stack.
                  Decrement Stack Pointer (SP) twice.
          nn = AF,BC,DE,HL
          Flags affected:    None
  */
  assert(0);
}

void CPU::res(uint8_t b, uint8_t *r) {
  /*
  RES b,r       - Reset bit b in register r.
          b = 0-7, r = A,B,C,D,E,H,L,(HL)
          Flags affected:    None
  */
  assert(0);
}

void CPU::ret(void) {
  /*
  RET           - Pop two bytes from stack & jump to that address.
          Flags affected:   None
  */
  assert(0);
}

void CPU::rl(uint8_t *n) {
  /*
  RL n          - Rotate n left through Carry flag.

          n = A,B,C,D,E,H,L,(HL)

          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  assert(0);
}

void CPU::rrc(uint8_t *n) {
  /*
  RRC n         - Rotate n right. Old bit 0 to Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  assert(0);
}

void CPU::rlc(uint8_t *n) {
  /*
  RLC n         - Rotate n left. Old bit 7 to Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 7 data.
  */
  assert(0);
}

void CPU::rr(uint8_t *n) {
  /*
  RR n          - Rotate n right through Carry flag.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Contains old bit 0 data.
  */
  assert(0);
}

void CPU::sbc(const uint8_t n) {
  /*
  SBC A,n       - Subtract n + Carry flag from A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Set if no borrow.
  */
  assert(0);
}

void CPU::sub_n(const uint8_t n) {
  /*
  SUB n         - Subtract n from A.
          n = A,B,C,D,E,H,L,(HL),#
          Flags affected:
                  Z - Set if result is zero.
                  N - Set.
                  H - Set if no borrow from bit 4.
                  C - Set if no borrow.
  */
  assert(0);
}

void CPU::stop(void) {
  /*
  STOP          - ??? affected:    ?
  */
  assert(0);
}

void CPU::swap(uint8_t *n) {
  /*
  SWAP n        - Swap upper & lower bits of n.
          n = A,B,C,D,E,H,L,(HL)
          Flags affected:
                  Z - Set if result is zero.
                  N - Reset.
                  H - Reset.
                  C - Reset.
  */
  assert(0);
}

void CPU::xor_a(const uint8_t n) {
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
  assert(0);
}
