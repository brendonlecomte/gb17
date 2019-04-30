#pragma once
#include <stdint.h>
#include "ALU.h"
#include "../MMU.h"
#include "register.h"

class CPU {
  public:
    CPU(MMU &mmu);
    ~CPU();

  // private:
    Register A, B, C, D, E, H, L;
    FlagRegister F;
    RegisterPair AF, BC, DE, HL;
    uint16_t SP;
    uint16_t PC;

    ALU alu;
    MMU &memory;

    uint8_t single_instruction(void);
    void stack_push(const uint16_t value);
    uint16_t stack_pop(void);

    void adc(Register &reg, const uint8_t n);
    void add(Register &reg, const uint8_t n);
    void add(RegisterPair &reg, const uint16_t n);
    void addSigned(uint16_t &reg, const int8_t n);
    void andReg(Register &reg, const uint8_t n);
    void bit(Register &r, const uint8_t b);
    void callN(const uint16_t n);
    void callCc(const uint16_t n, const uint8_t predicate);
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
    void jpCc(const uint16_t addr, const uint8_t predicate);
    void jr(const int8_t n);
    void halt(void);
    void setBit(Register &r,uint8_t b);
    void load(Register &reg, uint8_t n);
    void load(RegisterPair &reg, const uint16_t n);
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

    // OPCODE PNEUMONICS
    // clang-format off
    uint8_t NOP(void);           uint8_t LD_BC_d16(void);      uint8_t LD_mBC_A(void);         uint8_t INC_BC(void);
    uint8_t INC_B(void);         uint8_t DEC_B(void);          uint8_t LD_B_d8(void);          uint8_t RLCA(void);
    uint8_t LD_a16_SP(void);     uint8_t ADD_HL_BC(void);      uint8_t LD_A_BC(void);          uint8_t DEC_BC(void);
    uint8_t INC_C(void);         uint8_t DEC_C(void);          uint8_t LD_C_d8(void);          uint8_t RRCA(void);
    uint8_t STOP(void);          int8_t LD_DE_d16(void);       uint8_t LD_DE_A(void);          uint8_t INC_DE(void);
    uint8_t INC_D(void);         uint8_t DEC_D(void);          uint8_t LD_D_d8(void);          uint8_t RLA(void);
    uint8_t JR_r8(void);         uint8_t ADD_HL_DE(void);      uint8_t LD_A_DE(void);          uint8_t DEC_DE(void);
    uint8_t INC_E(void);         uint8_t DEC_E(void);          uint8_t LD_E_d8(void);          uint8_t RRA(void);
    uint8_t JR_NZ_r8(void);      uint8_t LD_HL_d16(void);      uint8_t LD_HLp_A(void);         uint8_t INC_HL(void);
    uint8_t INC_H(void);         uint8_t DEC_H(void);          uint8_t LD_H_d8(void);          uint8_t DAA(void);
    uint8_t JR_Z_r8(void);       uint8_t ADD_HL_HL(void);      uint8_t LD_A_HLp(void);         uint8_t DEC_HL(void);
    uint8_t INC_L(void);         uint8_t DEC_L(void);          uint8_t LD_L_d8(void);          uint8_t CPL(void);
    uint8_t JR_NC_r8(void);      uint8_t LD_SP_d16(void);      uint8_t LD_HLs_A(void);         uint8_t INC_SP(void);
    uint8_t INC_aHL(void);       uint8_t DEC_aHL(void);        uint8_t LD_mHL_d8(void);        uint8_t SCF(void);
    uint8_t JR_C_r8(void);       uint8_t ADD_HL_SP(void);      uint8_t LD_A_HLs(void);         uint8_t DEC_SP(void);
    uint8_t INC_A(void);         uint8_t DEC_A(void);          uint8_t LD_A_d8(void);          uint8_t CCF(void);
    uint8_t LD_B_B(void);        uint8_t LD_B_C(void);         uint8_t LD_B_D(void);           uint8_t LD_B_E(void);
    uint8_t LD_B_H(void);        uint8_t LD_B_L(void);         uint8_t LD_B_HLm(void);         uint8_t LD_B_A(void);
    uint8_t LD_C_B(void);        uint8_t LD_C_C(void);         uint8_t LD_C_D(void);           uint8_t LD_C_E(void);
    uint8_t LD_C_H(void);        uint8_t LD_C_L(void);         uint8_t LD_C_HLm(void);         uint8_t LD_C_A(void);
    uint8_t LD_D_B(void);        uint8_t LD_D_C(void);         uint8_t LD_D_D(void);           uint8_t LD_D_E(void);
    uint8_t LD_D_H(void);        uint8_t LD_D_L(void);         uint8_t LD_D_HLm(void);         uint8_t LD_D_A(void);
    uint8_t LD_E_B(void);        uint8_t LD_E_C(void);         uint8_t LD_E_D(void);           uint8_t LD_E_E(void);
    uint8_t LD_E_H(void);        uint8_t LD_E_L(void);         uint8_t LD_E_HLm(void);         uint8_t LD_E_A(void);
    uint8_t LD_H_B(void);        uint8_t LD_H_C(void);         uint8_t LD_H_D(void);           uint8_t LD_H_E(void);
    uint8_t LD_H_H(void);        uint8_t LD_H_L(void);         uint8_t LD_H_HLm(void);         uint8_t LD_H_A(void);
    uint8_t LD_L_B(void);        uint8_t LD_L_C(void);         uint8_t LD_L_D(void);           uint8_t LD_L_E(void);
    uint8_t LD_L_H(void);        uint8_t LD_L_L(void);         uint8_t LD_L_HLm(void);         uint8_t LD_L_A(void);
    uint8_t LD_HLm_B(void);      uint8_t LD_HLm_C(void);       uint8_t LD_HLm_D(void);         uint8_t LD_HLm_E(void);
    uint8_t LD_HLm_H(void);      uint8_t LD_HLm_L(void);       uint8_t HALT(void);             uint8_t LD_HLm_A(void);
    uint8_t LD_A_B(void);        uint8_t LD_A_C(void);         uint8_t LD_A_D(void);           uint8_t LD_A_E(void);
    uint8_t LD_A_H(void);        uint8_t LD_A_L(void);         uint8_t LD_A_HLm(void);         uint8_t LD_A_A(void);
    uint8_t ADD_A_B(void);       uint8_t ADD_A_C(void);        uint8_t ADD_A_D(void);          uint8_t ADD_A_E(void);
    uint8_t ADD_A_H(void);       uint8_t ADD_A_L(void);        uint8_t ADD_A_HLm(void);        uint8_t ADD_A_A(void);
    uint8_t ADC_A_B(void);       uint8_t ADC_A_C(void);        uint8_t ADC_A_D(void);          uint8_t ADC_A_E(void);
    uint8_t ADC_A_H(void);       uint8_t ADC_A_L(void);        uint8_t ADC_A_HLm(void);        uint8_t ADC_A_A(void);
    uint8_t SUB_B(void);         uint8_t SUB_C(void);          uint8_t SUB_D(void);            uint8_t SUB_E(void);
    uint8_t SUB_H(void);         uint8_t SUB_L(void);          uint8_t SUB_HLm(void);          uint8_t SUB_A(void);
    uint8_t SBC_A_B(void);       uint8_t SBC_A_C(void);        uint8_t SBC_A_D(void);          uint8_t SBC_A_E(void);
    uint8_t SBC_A_H(void);       uint8_t SBC_A_L(void);        uint8_t SBC_A_HLm(void);        uint8_t SBC_A_A(void);
    uint8_t AND_B(void);         uint8_t AND_C(void);          uint8_t AND_D(void);            uint8_t AND_E(void);
    uint8_t AND_H(void);         uint8_t AND_L(void);          uint8_t AND_HLm(void);          uint8_t AND_A(void);
    uint8_t XOR_B(void);         uint8_t XOR_C(void);          uint8_t XOR_D(void);            uint8_t XOR_E(void);
    uint8_t XOR_H(void);         uint8_t XOR_L(void);          uint8_t XOR_HLm(void);          uint8_t XOR_A(void);
    uint8_t OR_B(void);          uint8_t OR_C(void);           uint8_t OR_D(void);             uint8_t OR_E(void);
    uint8_t OR_H(void);          uint8_t OR_L(void);           uint8_t OR_HLm(void);           uint8_t OR_A(void);
    uint8_t CP_B(void);          uint8_t CP_C(void);           uint8_t CP_D(void);             uint8_t CP_E(void);
    uint8_t CP_H(void);          uint8_t CP_L(void);           uint8_t CP_HLm(void);           uint8_t CP_A(void);
    uint8_t RET_NZ(void);        uint8_t POP_BC(void);         uint8_t JP_NZ_a16(void);        uint8_t JP_a16(void);
    uint8_t CALL_NZ_a16(void);   uint8_t PUSH_BC(void);        uint8_t ADD_A_d8(void);
    uint8_t RST_00H(void);       uint8_t RET_Z(void);          uint8_t RET(void);              uint8_t JP_Z_a16(void);
    uint8_t PREFIX_CB(void);     uint8_t CALL_Z_a16(void);     uint8_t CALL_a16(void);
    uint8_t ADC_A_d8(void);      uint8_t RST_08H(void);        uint8_t RET_NC(void);           uint8_t POP_DE(void);
    uint8_t JP_NC_a16(void);     uint8_t CALL_NC_a16(void);    uint8_t PUSH_DE(void);          uint8_t SUB_d8(void);
    uint8_t RST_10H(void);       uint8_t RET_C(void);          uint8_t RETI(void);             uint8_t JP_C_a16(void);
    uint8_t CALL_C_a16(void);    uint8_t SBC_A_d8(void);       uint8_t RST_18H(void);          uint8_t LDH_a8_A(void);
    uint8_t POP_HL(void);        uint8_t LD_Cm_A(void);        uint8_t PUSH_HL(void);          uint8_t AND_d8(void);
    uint8_t RST_20H(void);       uint8_t ADD_SP_r8(void);      uint8_t JP_HLm(void);           uint8_t LD_a16_A(void);
    uint8_t XOR_d8(void);        uint8_t RST_28H(void);        uint8_t LDH_A_a8(void);         uint8_t POP_AF(void);
    uint8_t LD_A_Cm(void);       uint8_t DI(void);             uint8_t PUSH_AF(void);          uint8_t OR_d8(void);
    uint8_t RST_30H(void);       uint8_t LD_HL_SPr8(void);     uint8_t LD_SP_HL(void);
    uint8_t LD_A_a16(void);      uint8_t EI(void);             uint8_t CP_d8(void);            uint8_t RST_38(void);
    // clang-format on
};
