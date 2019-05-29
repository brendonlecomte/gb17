#include <iostream>
#include <iterator>
#include <csignal>
#include <assert.h>
#include  <iomanip>
#include <stdio.h>

#include "CPU/CPU.h"
#include "MMU/MMU.h"
#include "MMU/Cartridge.h"
#include "PPU/PPU.h"

typedef struct {
  uint16_t op, pc, sp, af, bc, de, hl;
} cpu_state;

void getState(std::string line, cpu_state *state) {
  auto found =std::sscanf (line.c_str(),
          "OP:%hx PC:%hx SP:%hx AF:%hx BC:%hx DE:%hx HL:%hx",
          &state->op,
          &state->pc,
          &state->sp,
          &state->af,
          &state->bc,
          &state->de,
          &state->hl );
}

bool compareStates(cpu_state &state, CPU &cpu){
  bool fail = false;
  std::string out;
  if(state.op == 0x00f0) return true;

  if(state.op != (uint16_t)cpu.op) {
    fail = true;
    out += "OP incorrect: " + std::to_string(state.op) + " != " + std::to_string((uint16_t)cpu.op) + "\n";
  }
  if(state.pc != cpu.PC) {
    fail = true;
    out += "PC incorrect: " + std::to_string(state.pc) + " != " + std::to_string(cpu.PC) + "\n";
  }
  if(state.sp != cpu.SP) {
    fail = true;
    out += "SP incorrect: " + std::to_string(state.sp) + " != " + std::to_string(cpu.SP) + "\n";
  }
  if(state.af != cpu.AF) {
    fail = true;
    out += "AF incorrect: " + std::to_string(state.af) + " != " + std::to_string(cpu.AF) + "\n";
  }
  if(state.bc != cpu.BC) {
    fail = true;
    out += "BC incorrect: " + std::to_string(state.bc) + " != " + std::to_string(cpu.BC) + "\n";
  }
  if(state.de != cpu.DE) {
    fail = true;
    out += "DE incorrect: " + std::to_string(state.de) + " != " + std::to_string(cpu.DE) + "\n";
  }
  if(state.hl != cpu.HL) {
    fail = true;
    out += "HL incorrect: " + std::to_string(state.hl) + " != " + std::to_string(cpu.HL) + "\n";
  }
  if(fail) std::cout << out << std::endl;
  return !fail;
}

int main(int argc, char** argv){
  //all this gets hidden inside GB() constructor
  Cartridge game_cart = Cartridge(argv[1]);
  Interrupts int_flags = Interrupts();
  Timer timer = Timer(int_flags);
  SerialPort serial = SerialPort();
  MMU memory_manager = MMU(game_cart.getMemoryController(), int_flags, timer, serial);
  CPU test_cpu = CPU(memory_manager, int_flags, &std::cout);
  PPU test_ppu = PPU(memory_manager, int_flags);


  //skip the boot loader
  test_cpu.setState(0x100, 0xFFFE, 0x1B0, 0x0013, 0x00D8, 0x014D);
  test_cpu.mem(0xFF50) = (uint8_t)1;

  std::ifstream file(argv[2], std::ios::binary);

  cpu_state state, post_state;

  int op, pc, sp, af, bc, de, hl;
  std::string pre_line;
  getline( file, pre_line );
  for( std::string line; getline( file, line ); ) {

    getState(pre_line, &state); //get starting state

    //prep cpu into that state
    test_cpu.setState(state.pc, state.sp, state.af, state.bc, state.de, state.hl);
    //run that state in BLC GB
    test_cpu.executeInstruction();

    //get the next state to compare new BLCGB state to
    getState(line, &post_state);

    //check my emulator matches the reference
    if(!compareStates(post_state, test_cpu)) //compare state for fail!
    {
        std::cout << "Pre: " << pre_line.c_str() <<std::endl;
        std::cout << "GB:  ";
        test_cpu.debugState();
        std::cout << "Ref: " << line.c_str() <<std::endl;
        std::string x;
        std::cin >> x;
    }
    pre_line = line;
  }


  return 0;
}
