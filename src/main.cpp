#include <iostream>
#include <iterator>
#include <csignal>
#include <assert.h>

#include "CPU/CPU.h"
#include "MMU/MMU.h"
#include "MMU/Cartridge.h"
#include "PPU/PPU.h"
#include "Timer/Timer.h"


//perhaps this will show up in the GB class too
// void onExit(int sigNum) {
//   std::cout << std::endl
//             << "------------- Core Dump --------------"
//             << std::endl;
//   test_cpu.coreDump();
//   exit(sigNum);
// }

int main(int argc, char** argv){
  //all this gets hidden inside GB() constructor
  Cartridge game_cart = Cartridge(argv[1]);
  MMU memory_manager = MMU(game_cart.getMemoryController());
  CPU test_cpu = CPU(memory_manager, NULL); //&std::cout);
  PPU test_ppu = PPU(memory_manager);
  Timer test_timer = Timer(memory_manager);

  //test specifif stuff
  // signal (SIGINT, onExit); //dump core on exit for debugging
  // signal (SIGABRT, onExit);
  uint8_t start_log = 0;

  //fake stuff for retro_init?

  //fake stuff for retro_load_game()
  //std::cout << game_cart.getCartHeader().getTypeStr() << std::endl;

  // retro_run()
  while(1) { //abstract this into GB::executeSingle()
    unsigned saved_pc = test_cpu.PC;
    uint32_t clocks = test_cpu.executeInstruction();
    test_cpu.processInterrupts();
    test_ppu.update(clocks);
    test_timer.update(clocks);
  }

  std::cout << std::endl;
  return 0;
}
