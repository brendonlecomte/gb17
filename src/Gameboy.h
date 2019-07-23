#pragma once
#include <stdint.h>
#include "CPU/CPU.h"
#include "Controller.h"
#include "Timer/Timer.h"
#include "PPU/PPU.h"

class Gameboy {
public:
  Gameboy() :
    cart(),
    interrupts(),
    serial(NULL),
    controller(interrupts),
    ppu(interrupts),
    timer(interrupts),
    mmu(cart, ppu, interrupts, timer, serial),
    cpu(mmu, interrupts, NULL) {};
  ~Gameboy(void){};

  const char *getVersion(void) { return "0.0.1"; };
  const char *getName(void) { return "GB17"; };

  void loadRom(const char *filename){
    cart.loadCart(filename);
  };

  void unloadRom(void){};

  void pressButton(Buttons b, bool val) { controller.setButton(b, val); };

  void reset(void){};

  void runFrame(uint16_t *screen_buffer, uint8_t *audio_buffer){
    ppu.setScreenBuffer(screen_buffer);

    while(1) {
      uint32_t clocks = cpu.executeInstruction();
      clocks += cpu.processInterrupts();
      timer.update(clocks);
      if(ppu.update(clocks)){
        break;
      }
    }

  };

private:
  Cartridge cart;
  Interrupts interrupts;
  Timer timer;
  SerialPort serial;
  MMU mmu;
  CPU cpu;
  PPU ppu;
  Controller controller;
};
