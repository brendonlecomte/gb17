#pragma once
#include <stdint.h>

#include "Cartridge.h"
#include "CPU.h"

class SerialPort {
public:
  SerialPort(){};
  ~SerialPort(){};
  uint8_t read() { return 0; };
  void write(const uint8_t data) { std::cout << data; }
};

class MMU {
public:
  MMU(CartridgeMemory *cart_memory) : m_cartridge(cart_memory), m_serialPort(){};
  ~MMU(){};
  uint8_t read8bit(const uint16_t address);
  uint16_t read16bit(const uint16_t address);

  void write(const uint16_t address, const uint8_t data);
  void write(const uint16_t address, const uint16_t data);

private:
  CartridgeMemory *m_cartridge;
  SerialPort m_serialPort;
};
