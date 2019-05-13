#include "CPU.h"
#include <assert.h>

CPU::CPU(MMU &mmu, std::ostream *out)
    : A(), F(), B(), C(), D(), E(), H(), L(), AF(A, F), BC(B, C), DE(D, E), HL(H, L), SP(0xFFFE), PC(0x0000), alu(F),
      memory(mmu), m_mem(mmu, 0), debug(out) {}

CPU::~CPU() {}

void CPU::coreDump(void) {
  if(!debug) return;
  (*debug) << "PC: 0x" << std::hex << PC << std::endl;
  (*debug) << "SP: 0x" << std::hex << SP << std::endl;
  (*debug) << "AF: 0x" << std::hex << unsigned(AF) << std::endl;
  (*debug) << "BC: 0x" << std::hex << unsigned(BC) << std::endl;
  (*debug) << "DE: 0x" << std::hex << unsigned(DE) << std::endl;
  (*debug) << "HL: 0x" << std::hex << unsigned(HL) << std::endl;
}

void CPU::stack_push(const uint16_t value) {
  SP -= 2;
  memory.write(SP, value);
}

uint16_t CPU::stack_pop(void) {
  uint16_t val = memory.read16bit(SP);
  SP += 2;
  return val;
}

OpCode CPU::readOp(void) {
  uint16_t stored_pc = PC;
  PC++;
  OpCode op = (OpCode)memory.read8bit(stored_pc);
  if(debug) *debug << "0x" <<std::hex << stored_pc << ": " << opToString(op) << " ";
  return op;
}

int8_t CPU::read_r8(void) {
  uint16_t stored_pc = PC;
  PC++;
  int8_t r8 = (int8_t)memory.read8bit(stored_pc);
   if(debug) *debug << "0x" << std::hex << (+r8&0xFF);
  return r8;
}

uint8_t CPU::read_d8(void) {
  uint16_t stored_pc = PC;
  PC++;
  uint8_t d8 = memory.read8bit(stored_pc);
  if(debug) *debug << "0x" <<std::hex << (+d8&0xFF);
  return d8;
}

uint16_t CPU::read_d16(void) {
  uint16_t stored_pc = PC;
  PC += 2;
  uint16_t d16 = memory.read16bit(stored_pc);
  if(debug) *debug << "0x" << std::hex << (+d16&0xFFFF);
  return d16;
}

MemRef& CPU::mem(const uint16_t address) {
  m_mem.setPointer(address);
  if(debug) *debug << " mem(0x" << std::hex << unsigned(address) <<"): "
                   << "0x" << std::hex<< unsigned((uint8_t)m_mem);
  return m_mem;
}

uint8_t CPU::execute_op(OpCode op) {
  switch (op) {
    case OpCode::NOP:
      nop();
      break;
    case OpCode::LD_BC_d16:
      load(BC, read_d16());
      break;
    case OpCode::LD_mBC_A:
      load(mem(BC), A);
      break;
    case OpCode::INC_BC:
      inc(BC);
      break;
    case OpCode::INC_B:
      inc(B);
      break;
    case OpCode::DEC_B:
      dec(B);
      break;
    case OpCode::LD_B_d8:
      load(B, read_d8());
      break;
    case OpCode::RLCA:
      rlc(A);
      break;
    case OpCode::LD_a16_SP:
      load(mem(read_d8()), SP);
      break;
    case OpCode::ADD_HL_BC:
      add(HL, BC);
      break;
    case OpCode::LD_A_mBC:
      load(A, mem(BC));
      break;
    case OpCode::DEC_BC:
      dec(BC);
      break;
    case OpCode::INC_C:
      inc(C);
      break;
    case OpCode::DEC_C:
      dec(C);
      break;
    case OpCode::LD_C_d8:
      load(C,read_d8());
      break;
    case OpCode::RRCA:
      rrc(A);
      break;
    case OpCode::STOP:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_DE_d16:
      load(DE, read_d16());
      break;
    case OpCode::LD_DE_A:
      load(mem(DE), A);
      break;
    case OpCode::INC_DE:
      inc(DE);
      break;
    case OpCode::INC_D:
      inc(D);
      break;
    case OpCode::DEC_D:
      dec(D);
      break;
    case OpCode::LD_D_d8:
      load(D, read_d8());
      break;
    case OpCode::RLA:
      rl(A);
      break;
    case OpCode::JR_r8:
      jr(read_r8());
      break;
    case OpCode::ADD_HL_DE:
      add(HL, DE);
      break;
    case OpCode::LD_A_DE:
      load(A, mem(DE));
      break;
    case OpCode::DEC_DE:
      dec(DE);
      break;
    case OpCode::INC_E:
      inc(E);
      break;
    case OpCode::DEC_E:
      dec(E);
      break;
    case OpCode::LD_E_d8:
      load(E, read_d8());
      break;
    case OpCode::RRA:
      rr(A);
      break;
    case OpCode::JR_NZ_r8:
      if(F.zero()) {
        PC += 1;
        return 2;
      }
      jr(read_r8());
      break;
    case OpCode::LD_HL_d16:
      load(HL, read_d16());
      break;
    case OpCode::LD_HLp_A:
      load(mem(HL), A);
      inc(HL);
      break;
    case OpCode::INC_HL:
      inc(HL);
      break;
    case OpCode::INC_H:
      inc(H);
      break;
    case OpCode::DEC_H:
      dec(H);
      break;
    case OpCode::LD_H_d8:
      load(H, read_d8());
      break;
    case OpCode::DAA:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::JR_Z_r8:
      if(!F.zero()) {
        PC += 1;
        return 2;
      }
      jr(read_d8());
      break;
    case OpCode::ADD_HL_HL:
      add(HL, HL);
      break;
    case OpCode::LD_A_HLp:
      load(A, mem(HL));
      inc(HL);
      break;
    case OpCode::DEC_HL:
      dec(HL);
      break;
    case OpCode::INC_L:
      inc(L);
      break;
    case OpCode::DEC_L:
      dec(L);
      break;
    case OpCode::LD_L_d8:
      load(L, read_d8());
      break;
    case OpCode::CPL:
      cpl();
      break;
    case OpCode::JR_NC_r8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_SP_d16:
      load(SP, read_d16());
      break;
    case OpCode::LD_HLs_A:
      load(mem(HL), A);
      HL = HL - 1;
      break;
    case OpCode::INC_SP:
      SP += 1;
      break;
    case OpCode::INC_aHL:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::DEC_aHL:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_mHL_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SCF:
      scf();
      break;
    case OpCode::JR_C_r8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADD_HL_SP:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_A_HLs:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::DEC_SP:
      SP -= 1;
      break;
    case OpCode::INC_A:
      inc(A);
      break;
    case OpCode::DEC_A:
      dec(A);
      break;
    case OpCode::LD_A_d8:
      load(A, read_d8());
      break;
    case OpCode::CCF:
      ccf();
      break;
    case OpCode::LD_B_B:
      load(B, B);
      break;
    case OpCode::LD_B_C:
      load(B, C);
      break;
    case OpCode::LD_B_D:
      load(B, D);
      break;
    case OpCode::LD_B_E:
      load(B, E);
      break;
    case OpCode::LD_B_H:
      load(B, H);
      break;
    case OpCode::LD_B_L:
      load(B, L);
      break;
    case OpCode::LD_B_HLm:
      load(B, mem(HL));
      break;
    case OpCode::LD_B_A:
      load(B, A);
      break;
    case OpCode::LD_C_B:
      load(C, B);
      break;
    case OpCode::LD_C_C:
      load(C, C);
      break;
    case OpCode::LD_C_D:
      load(C, D);
      break;
    case OpCode::LD_C_E:
      load(C, E);
      break;
    case OpCode::LD_C_H:
      load(C, H);
      break;
    case OpCode::LD_C_L:
      load(C, L);
      break;
    case OpCode::LD_C_HLm:
      load(C, mem(HL));
      break;
    case OpCode::LD_C_A:
      load(C, A);
      break;
    case OpCode::LD_D_B:
      load(D, B);
      break;
    case OpCode::LD_D_C:
      load(D, C);
      break;
    case OpCode::LD_D_D:
      load(D, D);
      break;
    case OpCode::LD_D_E:
      load(D, E);
      break;
    case OpCode::LD_D_H:
      load(D, H);
      break;
    case OpCode::LD_D_L:
      load(D, L);
      break;
    case OpCode::LD_D_HLm:
      load(D, mem(HL));
      break;
    case OpCode::LD_D_A:
      load(D, A);
      break;
    case OpCode::LD_E_B:
      load(E, B);
      break;
    case OpCode::LD_E_C:
      load(E, C);
      break;
    case OpCode::LD_E_D:
      load(E, D);
      break;
    case OpCode::LD_E_E:
      load(E, E);
      break;
    case OpCode::LD_E_H:
      load(E, H);
      break;
    case OpCode::LD_E_L:
      load(E, L);
      break;
    case OpCode::LD_E_HLm:
      load(E, mem(HL));
      break;
    case OpCode::LD_E_A:
      load(E, A);
      break;
    case OpCode::LD_H_B:
      load(H, B);
      break;
    case OpCode::LD_H_C:
      load(H, C);
      break;
    case OpCode::LD_H_D:
      load(H, D);
      break;
    case OpCode::LD_H_E:
      load(H, E);
      break;
    case OpCode::LD_H_H:
      load(H, H);
      break;
    case OpCode::LD_H_L:
      load(H, L);
      break;
    case OpCode::LD_H_HLm:
      load(H, mem(HL));
      break;
    case OpCode::LD_H_A:
      load(H, A);
      break;
    case OpCode::LD_L_B:
      load(L, B);
      break;
    case OpCode::LD_L_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_L_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::HALT:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HLm_A:
      load(mem(HL), A);
      break;
    case OpCode::LD_A_B:
      load(A, B);
      break;
    case OpCode::LD_A_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_A_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_A_E:
      load(A, E);
      break;
    case OpCode::LD_A_H:
      load(A, H);
      break;
    case OpCode::LD_A_L:
      load(A, L);
      break;
    case OpCode::LD_A_HLm:
      load(A, mem(HL));
      break;
    case OpCode::LD_A_A:
      load(A, A);
      break;
    case OpCode::ADD_A_B:
      add(A, B);
      break;
    case OpCode::ADD_A_C:
      add(A, C);
      break;
    case OpCode::ADD_A_D:
      add(A, D);
      break;
    case OpCode::ADD_A_E:
      add(A, E);
      break;
    case OpCode::ADD_A_H:
      add(A, H);
      break;
    case OpCode::ADD_A_L:
      add(A, L);
      break;
    case OpCode::ADD_A_HLm:
      add(A, mem(HL));
      break;
    case OpCode::ADD_A_A:
      add(A, A);
      break;
    case OpCode::ADC_A_B:
      adc(A, B);
      break;
    case OpCode::ADC_A_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADC_A_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_B:
      sub(A, B);
      break;
    case OpCode::SUB_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_A:
      xorReg(A, (uint8_t)A);
      break;
    case OpCode::OR_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::OR_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_B:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_D:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_E:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_L:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_HLm:
      cp(A, mem(HL));
      break;
    case OpCode::CP_A:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RET_NZ:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::POP_BC:
      BC = stack_pop();
      break;
    case OpCode::JP_NZ_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::JP_a16:
      jp(read_d16());
      break;
    case OpCode::CALL_NZ_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::PUSH_BC:
      stack_push(BC);
      break;
    case OpCode::ADD_A_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_00H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RET_Z:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RET:
      ret();
      break;
    case OpCode::JP_Z_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::PREFIX_CB:
      return OpCode_cycles[(uint8_t)op] + execute_op_cb(read_d8());
    case OpCode::CALL_Z_a16:
      if(!F.zero()) {
        PC += 2;
        return 2;
      }
      callN(read_d16());
      break;
    case OpCode::CALL_a16:
      callN(read_d16());
      break;
    case OpCode::ADC_A_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_08H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RET_NC:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::POP_DE:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::JP_NC_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION_1:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CALL_NC_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::PUSH_DE:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SUB_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_10H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RET_C:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RETI:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::JP_C_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION2:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CALL_C_a16:
      if(!F.carry()) {
        PC += 2;
        return 2;
      }
      callN(read_d16());
      break;
    case OpCode::ILLEGAL_INSTRUCTION3:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::SBC_A_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_18H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LDH_a8_A:
      load(mem(0xFF00 + read_d8()), A);
      break;
    case OpCode::POP_HL:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_Cm_A:
      load(mem(0xFF00 + (uint16_t)C), A);
      break;
    case OpCode::ILLEGAL_INSTRUCTION4:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION5:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::PUSH_HL:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::AND_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_20H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ADD_SP_r8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::JP_HLm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_a16_A:
      load(mem(read_d16()), A);
      break;
    case OpCode::ILLEGAL_INSTRUCTION6:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION7:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::XOR_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_28H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LDH_A_a8:
      load(A, mem(0xFF00 + read_d8()));
      break;
    case OpCode::POP_AF:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_A_Cm:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::DI:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION9:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::PUSH_AF:
      stack_push(AF);
      break;
    case OpCode::OR_d8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::RST_30H:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_HL_SPr8:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_SP_HL:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::LD_A_a16:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::EI:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl;
      break;
    case OpCode::ILLEGAL_INSTRUCTION10:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::ILLEGAL_INSTRUCTION11:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    case OpCode::CP_d8:
      cp(A, read_d8());
      break;
    case OpCode::RST_38:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
    default:
      std::cout << "Fault: 0x" << std::hex << unsigned(op) << " " << opToString(op) << std::endl; assert(0);
      break;
  }
  return OpCode_cycles[(uint8_t)op];
}

uint8_t CPU::execute_op_cb(uint8_t prefix_cb) {
  switch(prefix_cb) {
    case 0x00: rlc(B); break;
    case 0x01: rlc(C); break;
    case 0x02: rlc(D); break;
    case 0x03: rlc(E); break;
    case 0x04: rlc(H); break;
    case 0x05: rlc(L); break;
    case 0x06: rlc((Register&)mem(HL)); break;
    case 0x07: rlc(A); break;

    case 0x08: rrc(B); break;
    case 0x09: rrc(C); break;
    case 0x0A: rrc(D); break;
    case 0x0B: rrc(E); break;
    case 0x0C: rrc(H); break;
    case 0x0D: rrc(L); break;
    case 0x0E: rrc((Register&)mem(HL)); break;
    case 0x0F: rrc(A); break;

    case 0x10: rl(B); break;
    case 0x11: rl(C); break;
    case 0x12: rl(D); break;
    case 0x13: rl(E); break;
    case 0x14: rl(H); break;
    case 0x15: rl(L); break;
    case 0x16: rl((Register&)mem(HL)); break;
    case 0x17: rl(A); break;

    case 0x18: rr(B); break;
    case 0x19: rr(C); break;
    case 0x1A: rr(D); break;
    case 0x1B: rr(E); break;
    case 0x1C: rr(H); break;
    case 0x1D: rr(L); break;
    case 0x1E: rr((Register&)mem(HL)); break;
    case 0x1F: rr(A); break;

    case 0x20: sla(B); break;
    case 0x21: sla(C); break;
    case 0x22: sla(D); break;
    case 0x23: sla(E); break;
    case 0x24: sla(H); break;
    case 0x25: sla(L); break;
    case 0x26: sla((Register&)mem(HL)); break;
    case 0x27: sla(A); break;

    case 0x28: sra(B); break;
    case 0x29: sra(C); break;
    case 0x2A: sra(D); break;
    case 0x2B: sra(E); break;
    case 0x2C: sra(H); break;
    case 0x2D: sra(L); break;
    case 0x2E: sra((Register&)mem(HL)); break;
    case 0x2F: sra(A); break;

    case 0x30: swap(B); break;
    case 0x31: swap(C); break;
    case 0x32: swap(D); break;
    case 0x33: swap(E); break;
    case 0x34: swap(H); break;
    case 0x35: swap(L); break;
    case 0x36: swap((Register&)mem(HL)); break;
    case 0x37: swap(A); break;

    case 0x38: srl(B); break;
    case 0x39: srl(C); break;
    case 0x3A: srl(D); break;
    case 0x3B: srl(E); break;
    case 0x3C: srl(H); break;
    case 0x3D: srl(L); break;
    case 0x3E: srl((Register&)mem(HL)); break;
    case 0x3F: srl(A); break;

    case 0x40: bit(B, 0); break;
    case 0x41: bit(C, 0); break;
    case 0x42: bit(D, 0); break;
    case 0x43: bit(E, 0); break;
    case 0x44: bit(H, 0); break;
    case 0x45: bit(L, 0); break;
    case 0x46: bit((Register&)mem(HL), 0); break;
    case 0x47: bit(A, 0); break;

    case 0x48: bit(B, 1); break;
    case 0x49: bit(C, 1); break;
    case 0x4A: bit(D, 1); break;
    case 0x4B: bit(E, 1); break;
    case 0x4C: bit(H, 1); break;
    case 0x4D: bit(L, 1); break;
    case 0x4E: bit((Register&)mem(HL), 1); break;
    case 0x4F: bit(A, 1); break;

    case 0x50: bit(B, 2); break;
    case 0x51: bit(C, 2); break;
    case 0x52: bit(D, 2); break;
    case 0x53: bit(E, 2); break;
    case 0x54: bit(H, 2); break;
    case 0x55: bit(L, 2); break;
    case 0x56: bit((Register&)mem(HL), 2); break;
    case 0x57: bit(A, 2); break;

    case 0x58: bit(B, 3); break;
    case 0x59: bit(C, 3); break;
    case 0x5A: bit(D, 3); break;
    case 0x5B: bit(E, 3); break;
    case 0x5C: bit(H, 3); break;
    case 0x5D: bit(L, 3); break;
    case 0x5E: bit((Register&)mem(HL), 3); break;
    case 0x5F: bit(A, 3); break;

    case 0x60: bit(B, 4); break;
    case 0x61: bit(C, 4); break;
    case 0x62: bit(D, 4); break;
    case 0x63: bit(E, 4); break;
    case 0x64: bit(H, 4); break;
    case 0x65: bit(L, 4); break;
    case 0x66: bit((Register&)mem(HL), 4); break;
    case 0x67: bit(A, 4); break;

    case 0x68: bit(B, 5); break;
    case 0x69: bit(C, 5); break;
    case 0x6A: bit(D, 5); break;
    case 0x6B: bit(E, 5); break;
    case 0x6C: bit(H, 5); break;
    case 0x6D: bit(L, 5); break;
    case 0x6E: bit((Register&)mem(HL), 5); break;
    case 0x6F: bit(A, 5); break;

    case 0x70: bit(B, 6); break;
    case 0x71: bit(C, 6); break;
    case 0x72: bit(D, 6); break;
    case 0x73: bit(E, 6); break;
    case 0x74: bit(H, 6); break;
    case 0x75: bit(L, 6); break;
    case 0x76: bit((Register&)mem(HL), 6); break;
    case 0x77: bit(A, 6); break;

    case 0x78: bit(B, 7); break;
    case 0x79: bit(C, 7); break;
    case 0x7A: bit(D, 7); break;
    case 0x7B: bit(E, 7); break;
    case 0x7C: bit(H, 7); break;
    case 0x7D: bit(L, 7); break;
    case 0x7E: bit((Register&)mem(HL), 7); break;
    case 0x7F: bit(A, 7); break;

    case 0x80: res(B, 0); break;
    case 0x81: res(C, 0); break;
    case 0x82: res(D, 0); break;
    case 0x83: res(E, 0); break;
    case 0x84: res(H, 0); break;
    case 0x85: res(L, 0); break;
    case 0x86: res((Register&)mem(HL), 0); break;
    case 0x87: res(A, 0); break;

    case 0x88: res(B, 1); break;
    case 0x89: res(C, 1); break;
    case 0x8A: res(D, 1); break;
    case 0x8B: res(E, 1); break;
    case 0x8C: res(H, 1); break;
    case 0x8D: res(L, 1); break;
    case 0x8E: res((Register&)mem(HL), 1); break;
    case 0x8F: res(A, 1); break;

    case 0x90: res(B, 2); break;
    case 0x91: res(C, 2); break;
    case 0x92: res(D, 2); break;
    case 0x93: res(E, 2); break;
    case 0x94: res(H, 2); break;
    case 0x95: res(L, 2); break;
    case 0x96: res((Register&)mem(HL), 2); break;
    case 0x97: res(A, 2); break;

    case 0x98: res(B, 3); break;
    case 0x99: res(C, 3); break;
    case 0x9A: res(D, 3); break;
    case 0x9B: res(E, 3); break;
    case 0x9C: res(H, 3); break;
    case 0x9D: res(L, 3); break;
    case 0x9E: res((Register&)mem(HL), 3); break;
    case 0x9F: res(A, 3); break;

    case 0xA0: res(B, 4); break;
    case 0xA1: res(C, 4); break;
    case 0xA2: res(D, 4); break;
    case 0xA3: res(E, 4); break;
    case 0xA4: res(H, 4); break;
    case 0xA5: res(L, 4); break;
    case 0xA6: res((Register&)mem(HL), 4); break;
    case 0xA7: res(A, 4); break;

    case 0xA8: res(B, 5); break;
    case 0xA9: res(C, 5); break;
    case 0xAA: res(D, 5); break;
    case 0xAB: res(E, 5); break;
    case 0xAC: res(H, 5); break;
    case 0xAD: res(L, 5); break;
    case 0xAE: res((Register&)mem(HL), 5); break;
    case 0xAF: res(A, 5); break;

    case 0xB0: res(B, 6); break;
    case 0xB1: res(C, 6); break;
    case 0xB2: res(D, 6); break;
    case 0xB3: res(E, 6); break;
    case 0xB4: res(H, 6); break;
    case 0xB5: res(L, 6); break;
    case 0xB6: res((Register&)mem(HL), 6); break;
    case 0xB7: res(A, 6); break;

    case 0xB8: res(B, 7); break;
    case 0xB9: res(C, 7); break;
    case 0xBA: res(D, 7); break;
    case 0xBB: res(E, 7); break;
    case 0xBC: res(H, 7); break;
    case 0xBD: res(L, 7); break;
    case 0xBE: res((Register&)mem(HL), 7); break;
    case 0xBF: res(A, 7); break;

    case 0xC0: setBit(B, 0); break;
    case 0xC1: setBit(C, 0); break;
    case 0xC2: setBit(D, 0); break;
    case 0xC3: setBit(E, 0); break;
    case 0xC4: setBit(H, 0); break;
    case 0xC5: setBit(L, 0); break;
    case 0xC6: setBit((Register&)mem(HL), 0); break;
    case 0xC7: setBit(A, 0); break;

    case 0xC8: setBit(B, 1); break;
    case 0xC9: setBit(C, 1); break;
    case 0xCA: setBit(D, 1); break;
    case 0xCB: setBit(E, 1); break;
    case 0xCC: setBit(H, 1); break;
    case 0xCD: setBit(L, 1); break;
    case 0xCE: setBit((Register&)mem(HL), 1); break;
    case 0xCF: setBit(A, 1); break;

    case 0xD0: setBit(B, 2); break;
    case 0xD1: setBit(C, 2); break;
    case 0xD2: setBit(D, 2); break;
    case 0xD3: setBit(E, 2); break;
    case 0xD4: setBit(H, 2); break;
    case 0xD5: setBit(L, 2); break;
    case 0xD6: setBit((Register&)mem(HL), 2); break;
    case 0xD7: setBit(A, 2); break;

    case 0xD8: setBit(B, 3); break;
    case 0xD9: setBit(C, 3); break;
    case 0xDA: setBit(D, 3); break;
    case 0xDB: setBit(E, 3); break;
    case 0xDC: setBit(H, 3); break;
    case 0xDD: setBit(L, 3); break;
    case 0xDE: setBit((Register&)mem(HL), 3); break;
    case 0xDF: setBit(A, 3); break;

    case 0xE0: setBit(B, 4); break;
    case 0xE1: setBit(C, 4); break;
    case 0xE2: setBit(D, 4); break;
    case 0xE3: setBit(E, 4); break;
    case 0xE4: setBit(H, 4); break;
    case 0xE5: setBit(L, 4); break;
    case 0xE6: setBit((Register&)mem(HL), 4); break;
    case 0xE7: setBit(A, 4); break;

    case 0xE8: setBit(B, 5); break;
    case 0xE9: setBit(C, 5); break;
    case 0xEA: setBit(D, 5); break;
    case 0xEB: setBit(E, 5); break;
    case 0xEC: setBit(H, 5); break;
    case 0xED: setBit(L, 5); break;
    case 0xEE: setBit((Register&)mem(HL), 5); break;
    case 0xEF: setBit(A, 5); break;

    case 0xF0: setBit(B, 6); break;
    case 0xF1: setBit(C, 6); break;
    case 0xF2: setBit(D, 6); break;
    case 0xF3: setBit(E, 6); break;
    case 0xF4: setBit(H, 6); break;
    case 0xF5: setBit(L, 6); break;
    case 0xF6: setBit((Register&)mem(HL), 6); break;
    case 0xF7: setBit(A, 6); break;

    case 0xF8: setBit(B, 7); break;
    case 0xF9: setBit(C, 7); break;
    case 0xFA: setBit(D, 7); break;
    case 0xFB: setBit(E, 7); break;
    case 0xFC: setBit(H, 7); break;
    case 0xFD: setBit(L, 7); break;
    case 0xFE: setBit((Register&)mem(HL), 7); break;
    case 0xFF: setBit(A, 7); break;

  }
  return OpCode_cycles_cb[prefix_cb];
}
