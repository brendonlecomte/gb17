
// Cycles are set for the 1MHz speed
const uint8_t OpCode_cycles[256] = {
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
    1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
    2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
    2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
    3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
    3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
};

const uint8_t OpCode_width[256] = {
    1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 3, 3, 1, 1, 1, 1, 1, 3, 0, 3, 3, 1, 1,
    1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1,
    2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1,
    2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1
};

const uint8_t OpCode_cycles_cb[256] = {
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2
};

enum class OpCode : uint8_t {
  NOP,
  LD_BC_d16,
  LD_mBC_A,
  INC_BC,
  INC_B,
  DEC_B,
  LD_B_d8,
  RLCA,
  LD_a16_SP,
  ADD_HL_BC,
  LD_A_mBC,
  DEC_BC,
  INC_C,
  DEC_C,
  LD_C_d8,
  RRCA,
  STOP,
  LD_DE_d16,
  LD_DE_A,
  INC_DE,
  INC_D,
  DEC_D,
  LD_D_d8,
  RLA,
  JR_r8,
  ADD_HL_DE,
  LD_A_DE,
  DEC_DE,
  INC_E,
  DEC_E,
  LD_E_d8,
  RRA,
  JR_NZ_r8,
  LD_HL_d16,
  LD_HLp_A,
  INC_HL,
  INC_H,
  DEC_H,
  LD_H_d8,
  DAA,
  JR_Z_r8,
  ADD_HL_HL,
  LD_A_HLp,
  DEC_HL,
  INC_L,
  DEC_L,
  LD_L_d8,
  CPL,
  JR_NC_r8,
  LD_SP_d16,
  LD_HLs_A,
  INC_SP,
  INC_aHL,
  DEC_aHL,
  LD_mHL_d8,
  SCF,
  JR_C_r8,
  ADD_HL_SP,
  LD_A_HLs,
  DEC_SP,
  INC_A,
  DEC_A,
  LD_A_d8,
  CCF,
  LD_B_B,
  LD_B_C,
  LD_B_D,
  LD_B_E,
  LD_B_H,
  LD_B_L,
  LD_B_HLm,
  LD_B_A,
  LD_C_B,
  LD_C_C,
  LD_C_D,
  LD_C_E,
  LD_C_H,
  LD_C_L,
  LD_C_HLm,
  LD_C_A,
  LD_D_B,
  LD_D_C,
  LD_D_D,
  LD_D_E,
  LD_D_H,
  LD_D_L,
  LD_D_HLm,
  LD_D_A,
  LD_E_B,
  LD_E_C,
  LD_E_D,
  LD_E_E,
  LD_E_H,
  LD_E_L,
  LD_E_HLm,
  LD_E_A,
  LD_H_B,
  LD_H_C,
  LD_H_D,
  LD_H_E,
  LD_H_H,
  LD_H_L,
  LD_H_HLm,
  LD_H_A,
  LD_L_B,
  LD_L_C,
  LD_L_D,
  LD_L_E,
  LD_L_H,
  LD_L_L,
  LD_L_HLm,
  LD_L_A,
  LD_HLm_B,
  LD_HLm_C,
  LD_HLm_D,
  LD_HLm_E,
  LD_HLm_H,
  LD_HLm_L,
  HALT,
  LD_HLm_A,
  LD_A_B,
  LD_A_C,
  LD_A_D,
  LD_A_E,
  LD_A_H,
  LD_A_L,
  LD_A_HLm,
  LD_A_A,
  ADD_A_B,
  ADD_A_C,
  ADD_A_D,
  ADD_A_E,
  ADD_A_H,
  ADD_A_L,
  ADD_A_HLm,
  ADD_A_A,
  ADC_A_B,
  ADC_A_C,
  ADC_A_D,
  ADC_A_E,
  ADC_A_H,
  ADC_A_L,
  ADC_A_HLm,
  ADC_A_A,
  SUB_B,
  SUB_C,
  SUB_D,
  SUB_E,
  SUB_H,
  SUB_L,
  SUB_HLm,
  SUB_A,
  SBC_A_B,
  SBC_A_C,
  SBC_A_D,
  SBC_A_E,
  SBC_A_H,
  SBC_A_L,
  SBC_A_HLm,
  SBC_A_A,
  AND_B,
  AND_C,
  AND_D,
  AND_E,
  AND_H,
  AND_L,
  AND_HLm,
  AND_A,
  XOR_B,
  XOR_C,
  XOR_D,
  XOR_E,
  XOR_H,
  XOR_L,
  XOR_HLm,
  XOR_A,
  OR_B,
  OR_C,
  OR_D,
  OR_E,
  OR_H,
  OR_L,
  OR_HLm,
  OR_A,
  CP_B,
  CP_C,
  CP_D,
  CP_E,
  CP_H,
  CP_L,
  CP_HLm,
  CP_A,
  RET_NZ,
  POP_BC,
  JP_NZ_a16,
  JP_a16,
  CALL_NZ_a16,
  PUSH_BC,
  ADD_A_d8,
  RST_00H,
  RET_Z,
  RET,
  JP_Z_a16,
  PREFIX_CB,
  CALL_Z_a16,
  CALL_a16,
  ADC_A_d8,
  RST_08H,
  RET_NC,
  POP_DE,
  JP_NC_a16,
  ILLEGAL_INSTRUCTION_1,
  CALL_NC_a16,
  PUSH_DE,
  SUB_d8,
  RST_10H,
  RET_C,
  RETI,
  JP_C_a16,
  ILLEGAL_INSTRUCTION2,
  CALL_C_a16,
  ILLEGAL_INSTRUCTION3,
  SBC_A_d8,
  RST_18H,
  LDH_a8_A,
  POP_HL,
  LD_Cm_A,
  ILLEGAL_INSTRUCTION4,
  ILLEGAL_INSTRUCTION5,
  PUSH_HL,
  AND_d8,
  RST_20H,
  ADD_SP_r8,
  JP_HLm,
  LD_a16_A,
  ILLEGAL_INSTRUCTION6,
  ILLEGAL_INSTRUCTION7,
  ILLEGAL_INSTRUCTION8,
  XOR_d8,
  RST_28H,
  LDH_A_a8,
  POP_AF,
  LD_A_Cm,
  DI,
  ILLEGAL_INSTRUCTION9,
  PUSH_AF,
  OR_d8,
  RST_30H,
  LD_HL_SPr8,
  LD_SP_HL,
  LD_A_a16,
  EI,
  ILLEGAL_INSTRUCTION10,
  ILLEGAL_INSTRUCTION11,
  CP_d8,
  RST_38,
};

static std::string opToString(OpCode op) {
  switch(op) {
      case OpCode::NOP:
        return "NOP";
      case OpCode::LD_BC_d16:
        return "LD_BC_d16";
      case OpCode::LD_mBC_A:
        return "LD_mBC_A";
      case OpCode::INC_BC:
        return "INC_BC";
      case OpCode::INC_B:
        return "INC_B";
      case OpCode::DEC_B:
        return "DEC_B";
      case OpCode::LD_B_d8:
        return "LD_B_d8";
      case OpCode::RLCA:
        return "RLCA";
      case OpCode::LD_a16_SP:
        return "LD_a16_SP";
      case OpCode::ADD_HL_BC:
        return "ADD_HL_BC";
      case OpCode::LD_A_mBC:
        return "LD_A_mBC";
      case OpCode::DEC_BC:
        return "DEC_BC";
      case OpCode::INC_C:
        return "INC_C";
      case OpCode::DEC_C:
        return "DEC_C";
      case OpCode::LD_C_d8:
        return "LD_C_d8";
      case OpCode::RRCA:
        return "RRCA";
      case OpCode::STOP:
        return "STOP";
      case OpCode::LD_DE_d16:
        return "LD_DE_d16";
      case OpCode::LD_DE_A:
        return "LD_DE_A";
      case OpCode::INC_DE:
        return "INC_DE";
      case OpCode::INC_D:
        return "INC_D";
      case OpCode::DEC_D:
        return "DEC_D";
      case OpCode::LD_D_d8:
        return "LD_D_d8";
      case OpCode::RLA:
        return "RLA";
      case OpCode::JR_r8:
        return "JR_r8";
      case OpCode::ADD_HL_DE:
        return "ADD_HL_DE";
      case OpCode::LD_A_DE:
        return "LD_A_DE";
      case OpCode::DEC_DE:
        return "DEC_DE";
      case OpCode::INC_E:
        return "INC_E";
      case OpCode::DEC_E:
        return "DEC_E";
      case OpCode::LD_E_d8:
        return "LD_E_d8";
      case OpCode::RRA:
        return "RRA";
      case OpCode::JR_NZ_r8:
        return "JR_NZ_r8";
      case OpCode::LD_HL_d16:
        return "LD_HL_d16";
      case OpCode::LD_HLp_A:
        return "LD_HLp_A";
      case OpCode::INC_HL:
        return "INC_HL";
      case OpCode::INC_H:
        return "INC_H";
      case OpCode::DEC_H:
        return "DEC_H";
      case OpCode::LD_H_d8:
        return "LD_H_d8";
      case OpCode::DAA:
        return "DAA";
      case OpCode::JR_Z_r8:
        return "JR_Z_r8";
      case OpCode::ADD_HL_HL:
        return "ADD_HL_HL";
      case OpCode::LD_A_HLp:
        return "LD_A_HLp";
      case OpCode::DEC_HL:
        return "DEC_HL";
      case OpCode::INC_L:
        return "INC_L";
      case OpCode::DEC_L:
        return "DEC_L";
      case OpCode::LD_L_d8:
        return "LD_L_d8";
      case OpCode::CPL:
        return "CPL";
      case OpCode::JR_NC_r8:
        return "JR_NC_r8";
      case OpCode::LD_SP_d16:
        return "LD_SP_d16";
      case OpCode::LD_HLs_A:
        return "LD_HLs_A";
      case OpCode::INC_SP:
        return "INC_SP";
      case OpCode::INC_aHL:
        return "INC_aHL";
      case OpCode::DEC_aHL:
        return "DEC_aHL";
      case OpCode::LD_mHL_d8:
        return "LD_mHL_d8";
      case OpCode::SCF:
        return "SCF";
      case OpCode::JR_C_r8:
        return "JR_C_r8";
      case OpCode::ADD_HL_SP:
        return "ADD_HL_SP";
      case OpCode::LD_A_HLs:
        return "LD_A_HLs";
      case OpCode::DEC_SP:
        return "DEC_SP";
      case OpCode::INC_A:
        return "INC_A";
      case OpCode::DEC_A:
        return "DEC_A";
      case OpCode::LD_A_d8:
        return "LD_A_d8";
      case OpCode::CCF:
        return "CCF";
      case OpCode::LD_B_B:
        return "LD_B_B";
      case OpCode::LD_B_C:
        return "LD_B_C";
      case OpCode::LD_B_D:
        return "LD_B_D";
      case OpCode::LD_B_E:
        return "LD_B_E";
      case OpCode::LD_B_H:
        return "LD_B_H";
      case OpCode::LD_B_L:
        return "LD_B_L";
      case OpCode::LD_B_HLm:
        return "LD_B_HLm";
      case OpCode::LD_B_A:
        return "LD_B_A";
      case OpCode::LD_C_B:
        return "LD_C_B";
      case OpCode::LD_C_C:
        return "LD_C_C";
      case OpCode::LD_C_D:
        return "LD_C_D";
      case OpCode::LD_C_E:
        return "LD_C_E";
      case OpCode::LD_C_H:
        return "LD_C_H";
      case OpCode::LD_C_L:
        return "LD_C_L";
      case OpCode::LD_C_HLm:
        return "LD_C_HLm";
      case OpCode::LD_C_A:
        return "LD_C_A";
      case OpCode::LD_D_B:
        return "LD_D_B";
      case OpCode::LD_D_C:
        return "LD_D_C";
      case OpCode::LD_D_D:
        return "LD_D_D";
      case OpCode::LD_D_E:
        return "LD_D_E";
      case OpCode::LD_D_H:
        return "LD_D_H";
      case OpCode::LD_D_L:
        return "LD_D_L";
      case OpCode::LD_D_HLm:
        return "LD_D_HLm";
      case OpCode::LD_D_A:
        return "LD_D_A";
      case OpCode::LD_E_B:
        return "LD_E_B";
      case OpCode::LD_E_C:
        return "LD_E_C";
      case OpCode::LD_E_D:
        return "LD_E_D";
      case OpCode::LD_E_E:
        return "LD_E_E";
      case OpCode::LD_E_H:
        return "LD_E_H";
      case OpCode::LD_E_L:
        return "LD_E_L";
      case OpCode::LD_E_HLm:
        return "LD_E_HLm";
      case OpCode::LD_E_A:
        return "LD_E_A";
      case OpCode::LD_H_B:
        return "LD_H_B";
      case OpCode::LD_H_C:
        return "LD_H_C";
      case OpCode::LD_H_D:
        return "LD_H_D";
      case OpCode::LD_H_E:
        return "LD_H_E";
      case OpCode::LD_H_H:
        return "LD_H_H";
      case OpCode::LD_H_L:
        return "LD_H_L";
      case OpCode::LD_H_HLm:
        return "LD_H_HLm";
      case OpCode::LD_H_A:
        return "LD_H_A";
      case OpCode::LD_L_B:
        return "LD_L_B";
      case OpCode::LD_L_C:
        return "LD_L_C";
      case OpCode::LD_L_D:
        return "LD_L_D";
      case OpCode::LD_L_E:
        return "LD_L_E";
      case OpCode::LD_L_H:
        return "LD_L_H";
      case OpCode::LD_L_L:
        return "LD_L_L";
      case OpCode::LD_L_HLm:
        return "LD_L_HLm";
      case OpCode::LD_L_A:
        return "LD_L_A";
      case OpCode::LD_HLm_B:
        return "LD_HLm_B";
      case OpCode::LD_HLm_C:
        return "LD_HLm_C";
      case OpCode::LD_HLm_D:
        return "LD_HLm_D";
      case OpCode::LD_HLm_E:
        return "LD_HLm_E";
      case OpCode::LD_HLm_H:
        return "LD_HLm_H";
      case OpCode::LD_HLm_L:
        return "LD_HLm_L";
      case OpCode::HALT:
        return "HALT";
      case OpCode::LD_HLm_A:
        return "LD_HLm_A";
      case OpCode::LD_A_B:
        return "LD_A_B";
      case OpCode::LD_A_C:
        return "LD_A_C";
      case OpCode::LD_A_D:
        return "LD_A_D";
      case OpCode::LD_A_E:
        return "LD_A_E";
      case OpCode::LD_A_H:
        return "LD_A_H";
      case OpCode::LD_A_L:
        return "LD_A_L";
      case OpCode::LD_A_HLm:
        return "LD_A_HLm";
      case OpCode::LD_A_A:
        return "LD_A_A";
      case OpCode::ADD_A_B:
        return "ADD_A_B";
      case OpCode::ADD_A_C:
        return "ADD_A_C";
      case OpCode::ADD_A_D:
        return "ADD_A_D";
      case OpCode::ADD_A_E:
        return "ADD_A_E";
      case OpCode::ADD_A_H:
        return "ADD_A_H";
      case OpCode::ADD_A_L:
        return "ADD_A_L";
      case OpCode::ADD_A_HLm:
        return "ADD_A_HLm";
      case OpCode::ADD_A_A:
        return "ADD_A_A";
      case OpCode::ADC_A_B:
        return "ADC_A_B";
      case OpCode::ADC_A_C:
        return "ADC_A_C";
      case OpCode::ADC_A_D:
        return "ADC_A_D";
      case OpCode::ADC_A_E:
        return "ADC_A_E";
      case OpCode::ADC_A_H:
        return "ADC_A_H";
      case OpCode::ADC_A_L:
        return "ADC_A_L";
      case OpCode::ADC_A_HLm:
        return "ADC_A_HLm";
      case OpCode::ADC_A_A:
        return "ADC_A_A";
      case OpCode::SUB_B:
        return "SUB_B";
      case OpCode::SUB_C:
        return "SUB_C";
      case OpCode::SUB_D:
        return "SUB_D";
      case OpCode::SUB_E:
        return "SUB_E";
      case OpCode::SUB_H:
        return "SUB_H";
      case OpCode::SUB_L:
        return "SUB_L";
      case OpCode::SUB_HLm:
        return "SUB_HLm";
      case OpCode::SUB_A:
        return "SUB_A";
      case OpCode::SBC_A_B:
        return "SBC_A_B";
      case OpCode::SBC_A_C:
        return "SBC_A_C";
      case OpCode::SBC_A_D:
        return "SBC_A_D";
      case OpCode::SBC_A_E:
        return "SBC_A_E";
      case OpCode::SBC_A_H:
        return "SBC_A_H";
      case OpCode::SBC_A_L:
        return "SBC_A_L";
      case OpCode::SBC_A_HLm:
        return "SBC_A_HLm";
      case OpCode::SBC_A_A:
        return "SBC_A_A";
      case OpCode::AND_B:
        return "AND_B";
      case OpCode::AND_C:
        return "AND_C";
      case OpCode::AND_D:
        return "AND_D";
      case OpCode::AND_E:
        return "AND_E";
      case OpCode::AND_H:
        return "AND_H";
      case OpCode::AND_L:
        return "AND_L";
      case OpCode::AND_HLm:
        return "AND_HLm";
      case OpCode::AND_A:
        return "AND_A";
      case OpCode::XOR_B:
        return "XOR_B";
      case OpCode::XOR_C:
        return "XOR_C";
      case OpCode::XOR_D:
        return "XOR_D";
      case OpCode::XOR_E:
        return "XOR_E";
      case OpCode::XOR_H:
        return "XOR_H";
      case OpCode::XOR_L:
        return "XOR_L";
      case OpCode::XOR_HLm:
        return "XOR_HLm";
      case OpCode::XOR_A:
        return "XOR_A";
      case OpCode::OR_B:
        return "OR_B";
      case OpCode::OR_C:
        return "OR_C";
      case OpCode::OR_D:
        return "OR_D";
      case OpCode::OR_E:
        return "OR_E";
      case OpCode::OR_H:
        return "OR_H";
      case OpCode::OR_L:
        return "OR_L";
      case OpCode::OR_HLm:
        return "OR_HLm";
      case OpCode::OR_A:
        return "OR_A";
      case OpCode::CP_B:
        return "CP_B";
      case OpCode::CP_C:
        return "CP_C";
      case OpCode::CP_D:
        return "CP_D";
      case OpCode::CP_E:
        return "CP_E";
      case OpCode::CP_H:
        return "CP_H";
      case OpCode::CP_L:
        return "CP_L";
      case OpCode::CP_HLm:
        return "CP_HLm";
      case OpCode::CP_A:
        return "CP_A";
      case OpCode::RET_NZ:
        return "RET_NZ";
      case OpCode::POP_BC:
        return "POP_BC";
      case OpCode::JP_NZ_a16:
        return "JP_NZ_a16";
      case OpCode::JP_a16:
        return "JP_a16";
      case OpCode::CALL_NZ_a16:
        return "CALL_NZ_a16";
      case OpCode::PUSH_BC:
        return "PUSH_BC";
      case OpCode::ADD_A_d8:
        return "ADD_A_d8";
      case OpCode::RST_00H:
        return "RST_00H";
      case OpCode::RET_Z:
        return "RET_Z";
      case OpCode::RET:
        return "RET";
      case OpCode::JP_Z_a16:
        return "JP_Z_a16";
      case OpCode::PREFIX_CB:
        return "PREFIX_CB";
      case OpCode::CALL_Z_a16:
        return "CALL_Z_a16";
      case OpCode::CALL_a16:
        return "CALL_a16";
      case OpCode::ADC_A_d8:
        return "ADC_A_d8";
      case OpCode::RST_08H:
        return "RST_08H";
      case OpCode::RET_NC:
        return "RET_NC";
      case OpCode::POP_DE:
        return "POP_DE";
      case OpCode::JP_NC_a16:
        return "JP_NC_a16";
      case OpCode::ILLEGAL_INSTRUCTION_1:
        return "ILLEGAL_INSTRUCTION_1";
      case OpCode::CALL_NC_a16:
        return "CALL_NC_a16";
      case OpCode::PUSH_DE:
        return "PUSH_DE";
      case OpCode::SUB_d8:
        return "SUB_d8";
      case OpCode::RST_10H:
        return "RST_10H";
      case OpCode::RET_C:
        return "RET_C";
      case OpCode::RETI:
        return "RETI";
      case OpCode::JP_C_a16:
        return "JP_C_a16";
      case OpCode::ILLEGAL_INSTRUCTION2:
        return "ILLEGAL_INSTRUCTION2";
      case OpCode::CALL_C_a16:
        return "CALL_C_a16";
      case OpCode::ILLEGAL_INSTRUCTION3:
        return "ILLEGAL_INSTRUCTION3";
      case OpCode::SBC_A_d8:
        return "SBC_A_d8";
      case OpCode::RST_18H:
        return "RST_18H";
      case OpCode::LDH_a8_A:
        return "LDH_a8_A";
      case OpCode::POP_HL:
        return "POP_HL";
      case OpCode::LD_Cm_A:
        return "LD_Cm_A";
      case OpCode::ILLEGAL_INSTRUCTION4:
        return "ILLEGAL_INSTRUCTION4";
      case OpCode::ILLEGAL_INSTRUCTION5:
        return "ILLEGAL_INSTRUCTION5";
      case OpCode::PUSH_HL:
        return "PUSH_HL";
      case OpCode::AND_d8:
        return "AND_d8";
      case OpCode::RST_20H:
        return "RST_20H";
      case OpCode::ADD_SP_r8:
        return "ADD_SP_r8";
      case OpCode::JP_HLm:
        return "JP_HLm";
      case OpCode::LD_a16_A:
        return "LD_a16_A";
      case OpCode::ILLEGAL_INSTRUCTION6:
        return "ILLEGAL_INSTRUCTION6";
      case OpCode::ILLEGAL_INSTRUCTION7:
        return "ILLEGAL_INSTRUCTION7";
      case OpCode::ILLEGAL_INSTRUCTION8:
        return "ILLEGAL_INSTRUCTION8";
      case OpCode::XOR_d8:
        return "XOR_d8";
      case OpCode::RST_28H:
        return "RST_28H";
      case OpCode::LDH_A_a8:
        return "LDH_A_a8";
      case OpCode::POP_AF:
        return "POP_AF";
      case OpCode::LD_A_Cm:
        return "LD_A_Cm";
      case OpCode::DI:
        return "DI";
      case OpCode::ILLEGAL_INSTRUCTION9:
        return "ILLEGAL_INSTRUCTION9";
      case OpCode::PUSH_AF:
        return "PUSH_AF";
      case OpCode::OR_d8:
        return "OR_d8";
      case OpCode::RST_30H:
        return "RST_30H";
      case OpCode::LD_HL_SPr8:
        return "LD_HL_SPr8";
      case OpCode::LD_SP_HL:
        return "LD_SP_HL";
      case OpCode::LD_A_a16:
        return "LD_A_a16";
      case OpCode::EI:
        return "EI";
      case OpCode::ILLEGAL_INSTRUCTION10:
        return "ILLEGAL_INSTRUCTION10";
      case OpCode::ILLEGAL_INSTRUCTION11:
        return "ILLEGAL_INSTRUCTION11";
      case OpCode::CP_d8:
        return "CP_d8";
      case OpCode::RST_38:
        return "RST_38";
  }
}
