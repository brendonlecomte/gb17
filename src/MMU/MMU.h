#pragma once
#include "../CPU/Interrupts.h"
#include "../CPU/register.h"
#include "../Timer/Timer.h"
#include "../PPU/PPU.h"
#include "BankMemory.h"
#include "Cartridge.h"
#include "SerialPort.h"
#include <stdint.h>

class MMU {
public:
  MMU(Cartridge &cart, PPU &ppu, Interrupts &flags, Timer &timer, SerialPort &serial)
      : m_cartridge(cart), m_flags(flags), m_timer(timer), m_serialPort(serial),
        m_ppu(ppu),
        wram(wram_mem, 0x1000, 2),
        joyp(0x1F)
  {
    boot = 0;
  };
  ~MMU(){};
  uint8_t read8bit(const uint16_t address);
  uint16_t read16bit(const uint16_t address);

  void write(const uint16_t address, const uint8_t data);
  void write(const uint16_t address, const uint16_t data);

private:
  Interrupts &m_flags;
  Cartridge &m_cartridge;
  SerialPort &m_serialPort;
  Timer &m_timer;
  PPU &m_ppu;
  uint8_t hram[0x7F];
  uint8_t io[0x7F];
  uint8_t wram_mem[0x2000] = {0};
  BankMemory wram;
  uint8_t boot;
  uint8_t ie;
  uint8_t joyp;
};

class MemRef {
public:
  MemRef(MMU &memory, const uint16_t address) : mem(memory), addr(address){};
  ~MemRef(){};
  void setPointer(const uint16_t address) { addr = address; };
  operator uint8_t() { return mem.read8bit(addr); };
  operator uint16_t() { return mem.read16bit(addr); };
  MemRef &operator=(const uint8_t &value) {
    mem.write(addr, value);
    return *this;
  };
  MemRef &operator=(const uint16_t &value) {
    mem.write(addr, value);
    return *this;
  };


private:
  uint16_t addr;
  MMU &mem;
};
