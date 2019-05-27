#pragma once
#include <stdint.h>
#include "Cartridge.h"
#include "../CPU/Interrupts.h"

class SerialPort {
public:
  SerialPort(std::ostream *out = NULL): serial(out){};
  ~SerialPort(){};
  void setControl(const uint8_t data) {};
  uint8_t getControl() { return 0; };
  uint8_t read() { return 0; };
  void write(const uint8_t data) { *serial << char(data); }
private:
  std::ostream* serial;
};

class MMU {
public:
  MMU(CartridgeMemory *cart_memory, Interrupts &flags) : m_cartridge(cart_memory), m_flags(flags), m_serialPort(&std::cout){
    *boot = 0;
  };
  ~MMU(){};
  uint8_t read8bit(const uint16_t address);
  uint16_t read16bit(const uint16_t address);

  void write(const uint16_t address, const uint8_t data);
  void write(const uint16_t address, const uint16_t data);

private:
  Interrupts &m_flags;
  CartridgeMemory *m_cartridge;
  SerialPort m_serialPort;
  uint8_t hram[0x7F];
  uint8_t io[0x7F];
  uint8_t vram[0x1FFF];
  uint8_t wram[0x2000];
  uint8_t echo[0x2000];
  uint8_t *boot = &io[0x4D];
  uint8_t ie;
};

class MemRef {
public:
  MemRef(MMU &memory, const uint16_t address) : mem(memory), addr(address) {};
  ~MemRef() {};
  void setPointer(const uint16_t address) {addr = address;};
  operator uint8_t() { return mem.read8bit(addr); };
  operator uint16_t() { return mem.read16bit(addr); };
  MemRef& operator =(const uint8_t& value) { mem.write(addr, value); return *this; };
  MemRef& operator =(const uint16_t& value) { mem.write(addr, value); return *this; };
private:
  uint16_t addr;
  MMU &mem;

};
