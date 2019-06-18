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
  Interrupts test_interrupts = Interrupts();
  Timer test_timer = Timer(test_interrupts);
  PPU test_ppu = PPU(test_interrupts);
  SerialPort test_serial = SerialPort();
  MMU memory_manager = MMU(game_cart, test_ppu, test_interrupts, test_timer, test_serial);
  std::ostream *debug = NULL;
  if(argc == 3)
    debug = &std::cout;

  CPU test_cpu = CPU(memory_manager, test_interrupts, debug);


  //test specifif stuff
  // signal (SIGINT, onExit); //dump core on exit for debugging
  // signal (SIGABRT, onExit);
  uint8_t start_log = 0;
  uint8_t x;

  //fake stuff for retro_init?

  //fake stuff for retro_load_game()
  //std::cout << game_cart.getCartHeader().getTypeStr() << std::endl;

  // retro_run()
  while(1) { //abstract this into GB::executeSingle()
    unsigned saved_pc = test_cpu.PC;
    uint32_t clocks = test_cpu.executeInstruction();
    clocks += test_cpu.processInterrupts();
    test_ppu.update(clocks);
    test_timer.update(clocks);
    if(test_cpu.op == OpCode::EI) {
      start_log = 1;
    }
    if(start_log) {
      std::cin >> x;
    }
  }

  std::cout << std::endl;
  return 0;
}
