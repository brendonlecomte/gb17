#include <assert.h>

#include "ALU.h"
#include "CPU.h"

CPU::CPU(MMU &mmu)
    : A(), F(), B(), C(), D(), E(), H(), L(),
      AF(A, F), BC(B, C), DE(D, E), HL(H, L),
      SP(0xFFFE), PC(0x0000), alu(F), memory(mmu) {}

CPU::~CPU() {}

uint8_t CPU::single_instruction(void) {
  //OPCode_enum op = memory.read(PC); // read OP code from memory
  //PC++; // increment PC
  //op_callbacks[op](); // execute op code
  //return op_cycle_count[op]; // return used number of cycles
  return 0;
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
