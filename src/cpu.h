#pragma once
#include <stdint.h>
#include "ALU.h"
#include "register.h"

class CPU {
  public:
    CPU();
    ~CPU();

  // private:
    Register A, B, C, D, E, H, L;
    FlagRegister F;
    RegisterPair AF, BC, DE, HL;
    uint16_t SP;
    uint16_t PC;

    ALU alu;

    void adc(const uint8_t n); //A
    void add_a(const uint8_t n); //A
    void add_HL(const uint16_t n);
    void add_SP(const int8_t n);
    void and_a(const uint8_t n); //A
    void bit(const uint8_t b, const uint8_t r);
    void call_n(const uint16_t n);
    void call_cc(const uint8_t predicate, const uint16_t n);
    void ccf(void);
    void daa(void); //A
    void cp(const uint8_t n); //A
    void cpl(void); //A

    void inc_n(uint8_t *reg);
    void dec_n(uint8_t *reg);
    void inc_nn(uint16_t *reg);
    void dec_nn(uint16_t *nn);
    void di(void);
    void ei(void);

    void jp(const uint16_t addr);
    void jp_hl(void);
    void jr(const uint8_t n);
    void halt_cpu(void);
    void set_b(uint8_t b, uint8_t *r);
    void load_a(uint8_t n); //A
    void load_16(uint16_t *reg, const uint16_t n); //A
    void nop(void);
    void scf(void);
    void sla(uint8_t *n);
    void sra(uint8_t *n);
    void srl(uint8_t *n);
    void rst(uint8_t n);

    void or_a(uint8_t n);
    void pop(uint16_t *nn);
    void push(uint16_t nn);
    void res(uint8_t b, uint8_t *r);
    void ret(void);

    void rl_a(uint8_t *n);
    void rrc_a(uint8_t *n);
    void rlc_a(uint8_t *n);
    void rr_a(uint8_t *n);
    void rl(uint8_t *n);
    void rlc(uint8_t *n);
    void rr(uint8_t *n);
    void rrc(uint8_t *n);
    void sbc(uint8_t n); //A
    void sub_n(uint8_t n); //A
    void stop(void);
    void swap(uint8_t *n);
    void xor_a(uint8_t n); //A
};
