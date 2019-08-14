#include <iostream>
#include <iterator>
#include <csignal>
#include <assert.h>

#include "CPU/CPU.h"
#include "MMU/MMU.h"
#include "MMU/Cartridge.h"
#include "PPU/PPU.h"
#include "Timer/Timer.h"
#include "Controller.h"
CPU *test_cpu;


//perhaps this will show up in the GB class too
void onExit(int sigNum) {
  std::cout << std::endl
            << "------------- Core Dump --------------"
            << std::endl;
  test_cpu->debugState();
  exit(sigNum);
}

int main(int argc, char** argv){
  //all this gets hidden inside GB() constructor
  Cartridge game_cart = Cartridge();
  Interrupts test_interrupts = Interrupts();
  Timer test_timer = Timer(test_interrupts);
  PPU test_ppu = PPU(test_interrupts);
  SerialPort test_serial = SerialPort();
  Controller test_buttons = Controller(test_interrupts);
  MMU memory_manager = MMU(game_cart, test_ppu, test_interrupts, test_timer, test_serial, test_buttons);
  std::ostream *debug = NULL;
  if(argc == 3)
    debug = &std::cout;

  game_cart.loadCart(argv[1]);
  test_cpu = new CPU(memory_manager, test_interrupts, debug);


  //test specifif stuff
  signal (SIGINT, onExit); //dump core on exit for debugging
  signal (SIGABRT, onExit);
  signal (SIGSEGV, onExit);

  uint8_t start_log = 0;
  uint8_t x;

  //fake stuff for retro_init?

  //fake stuff for retro_load_game()
  //std::cout << game_cart.getCartHeader().getTypeStr() << std::endl;

  // retro_run()
    while(1) { //abstract this into GB::executeSingle()
      unsigned saved_pc = test_cpu->PC;
      uint32_t clocks = test_cpu->executeInstruction();
      clocks += test_cpu->processInterrupts();
      test_ppu.update(clocks);
      test_timer.update(clocks);
      memory_manager.processDMA();
    }

  std::cout << std::endl;
  return 0;
}
