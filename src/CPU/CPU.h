#pragma once
#include "../MMU/MMU.h"
#include "Interrupts.h"
#include "ALU.h"
#include "OpCode.h"
#include "register.h"
#include <stdint.h>

class CPU {
public:
  CPU(MMU &mmu, Interrupts &ints, std::ostream *out);

  ~CPU();

  // private:
  Register A, B, C, D, E, H, L;
  FlagRegister F;
  RegisterPair AF, BC, DE, HL;
  uint16_t SP;
  uint16_t PC;
  MemRef m_mem;
  bool halted;
  OpCode op;
  Interrupts &flags;

  ALU alu;
  MMU &memory;

  std::ostream *debug;

  void coreDump(void);
  void debugState(void);
  void setState(uint16_t pc, uint16_t sp, uint16_t af, uint16_t bc, uint16_t de, uint16_t hl);

  uint8_t executeInstruction(void);
  uint8_t processInterrupts(void);

  // private:
  uint8_t executeOp(OpCode op);
  uint8_t executeOpCb(uint8_t prefix_cb);
  uint8_t vectorInterrupt(InterruptAddr address);
  void stackPush(const uint16_t value);
  uint16_t stackPop(void);

  OpCode readOp(void);
  int8_t readR8(void);
  uint8_t readD8(void);
  uint16_t readD16(void);
  MemRef &mem(const uint16_t address);

  void adc(Register &reg, const uint8_t n);
  void add(Register &reg, const uint8_t n);
  void add(RegisterPair &reg, const uint16_t n);
  void addSigned(uint16_t &reg, const int8_t n);
  void andReg(Register &reg, const uint8_t n);
  void bit(Register &r, const uint8_t b);
  void bit(MemRef &r, const uint8_t b);
  void callN(const uint16_t n);
  void ccf(void);
  void daa(void);
  void cp(Register &reg, const uint8_t n);
  void cpl(void);

  void inc(Register &reg);
  void dec(Register &reg);
  void inc(RegisterPair &reg);
  void dec(RegisterPair &reg);
  void di(void);
  void ei(void);

  void jp(const uint16_t addr);
  void jr(const int8_t n);
  void halt(void);
  void setBit(Register &r, uint8_t b);

  void load(Register &reg, uint8_t n);
  void load(MemRef &reg, const uint8_t n);
  void load(uint8_t &reg, const uint8_t n);
  void load(RegisterPair &reg, const uint16_t n);
  void load(uint16_t &reg, const uint16_t n);
  void nop(void);
  void scf(void);
  void sla(Register &reg);
  void sra(Register &reg);
  void srl(Register &reg);
  void rst(const uint16_t n);

  void orReg(Register &reg, const uint8_t n);
  void pop(RegisterPair &reg);
  void push(RegisterPair &reg);
  void res(Register &reg, const uint8_t b);
  void ret(void);
  void rl(Register &reg);
  void rlc(Register &reg);
  void rr(Register &reg);
  void rrc(Register &reg);
  void sbc(Register &reg, const uint8_t n);
  void sub(Register &reg, const uint8_t n);
  void stop(void);
  void swap(Register &reg);
  void xorReg(Register &reg, const uint8_t n);
};
