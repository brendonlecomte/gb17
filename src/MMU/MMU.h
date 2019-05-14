#pragma once
#include <stdint.h>

#include "Cartridge.h"

class SerialPort {
public:
  SerialPort(){};
  ~SerialPort(){};
  uint8_t read() { return 0; };
  void write(const uint8_t data) { std::cout << data; }
};

class MMU {
public:
  MMU(CartridgeMemory *cart_memory) : m_cartridge(cart_memory), m_serialPort(), boot_mode(1){};
  ~MMU(){};
  uint8_t read8bit(const uint16_t address);
  uint16_t read16bit(const uint16_t address);

  void write(const uint16_t address, const uint8_t data);
  void write(const uint16_t address, const uint16_t data);

private:
  bool boot_mode;
  CartridgeMemory *m_cartridge;
  SerialPort m_serialPort;
  uint8_t hram[127];
  uint8_t io[127];
  uint8_t vram[0x1FFF];
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
