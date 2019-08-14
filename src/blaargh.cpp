#include <iostream>
#include <csignal>
#include <sstream>

#include "CPU/CPU.h"
#include "MMU/MMU.h"
#include "PPU/PPU.h"
#include "Timer/Timer.h"
#include "Controller.h"


CPU* cpu;
std::stringstream ser_out;

//perhaps this will show up in the GB class too
void onExit(int sigNum) {
  std::cout << std::endl
            << "------------- Core Dump --------------"
            << std::endl;
  cpu->debugState();
  std::cout << ser_out.str();
  exit(sigNum);
}

int main(int argc, char** argv){
  //all this gets hidden inside GB() constructor
  Cartridge game_cart = Cartridge();
  Interrupts interrupts = Interrupts();
  Timer timer = Timer(interrupts);
  SerialPort serial = SerialPort(&ser_out);
  Controller controller = Controller(interrupts);
  PPU ppu = PPU(interrupts);
  MMU memory_manager = MMU(game_cart, ppu, interrupts, timer, serial, controller);
  cpu = new CPU(memory_manager, interrupts, NULL);


  signal (SIGINT, onExit); //dump core on exit for debugging
  signal (SIGABRT, onExit);
  signal (SIGSEGV, onExit);

  game_cart.loadCart(argv[1]);
  game_cart.getCartHeader().print(std::cout);

  while(1) { //abstract this into GB::executeSingle()
    unsigned saved_pc = cpu->PC;
    uint32_t clocks = cpu->executeInstruction();
    clocks += cpu->processInterrupts();
    ppu.update(clocks);
    timer.update(clocks);
    memory_manager.processDMA();

    if(saved_pc == cpu->PC && !cpu->halted && cpu->op != OpCode::NOP){
        std::string ss;
        ss = ser_out.str();
        cpu->debugState();
        //likely we are in a while(1)
        std::cout << ser_out.str();
        if(std::string::npos == ss.find("Passed")) {
          exit(1); //Passed wasnt printed on the serial port
        }
        exit(0);
    }
  }

  std::cout << std::endl;
  return 0;
}
