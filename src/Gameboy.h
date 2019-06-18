#pragma once
#include <stdint.h>
#include "CPU/CPU.h"
#include "Controller.h"

class Gameboy {
public:
  Gameboy(void) : interrupts(), controller(interrupts) {};
  ~Gameboy(void){};

  const char *getVersion(void) { return "0.0.1"; };
  const char *getName(void) { return "GB17"; };

  void loadRom(const char *rom){};
  void unloadRom(void){};

  void pressButton(Buttons b, bool val) { controller.setButton(b, val); };

  void reset(void){};

  void runFrame(uint16_t *screen_buffer, uint8_t *audio_buffer){
    memset(screen_buffer, controller.getRegister(), 160*144*2); //test random colour
  };

private:
  // Cartridge cart;
  Interrupts interrupts;
  // Timer timer;
  // SerialPort serial;
  // MMU mmu;
  // CPU cpu;
  // PPU ppu;
  Controller controller;
};
