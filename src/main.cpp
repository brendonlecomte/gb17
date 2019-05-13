#include <iostream>
#include <iterator>
#include <csignal>
#include <assert.h>

#include "CPU/CPU.h"
#include "MMU/MMU.h"
#include "MMU/Cartridge.h"
#include "PPU/PPU.h"

std::vector<uint8_t> rom;
Cartridge game_cart = Cartridge(rom);
MMU memory_manager = MMU(game_cart.getMemoryController());
CPU test_cpu = CPU(memory_manager, &std::cout);
PPU test_ppu = PPU(memory_manager);

void onExit(int sigNum) {
  std::cout << std::endl
            << "------------- Core Dump --------------"
            << std::endl;
  test_cpu.coreDump();
  exit(sigNum);
}

int main(int argc, char** argv){
  //test specifif stuff
  signal (SIGINT, onExit); //dump core on exit for debugging
  signal (SIGABRT, onExit);


  //fake stuff for retro_init?
  //object creation etc....

  //fake stuff for retro_load_game()
  std::ifstream file("../gb-test-roms/cpu_instrs/individual/06-ld r,r.gb", std::ios::binary);
  file.unsetf(std::ios::skipws);
  std::streampos fileSize;
  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  std::copy(std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>(),
  			  std::back_inserter(rom));
  std::cout << std::hex << unsigned(rom[0]) <<std::endl;

  // retro_run()
  while(1) {
    unsigned saved_pc = test_cpu.PC;
    OpCode op = test_cpu.readOp();
    uint32_t clocks = test_cpu.execute_op(op);
    test_ppu.update(clocks);
    std::cout << std::endl;
    if(test_cpu.PC == 0x100 || test_cpu.PC > 0x1000) {
      assert(0);
    }
  }

  std::cout << std::endl;
  return 0;
}
