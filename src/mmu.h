#pragma once
#include <stdint.h>

#include "cartridge.h"
#include "cpu.h"

class SerialPort {
public:
  SerialPort(){};
  ~SerialPort(){};
  uint8_t read(){};
  void write(const uint8_t data) { std::cout << data << std : endl; }
}

class MMU {
public:
  MMU(CartridgeMemory &cart_memory) : m_cart(cart_memory), m_serialPort(){};
  ~MMU(){};
  uint8_t read8bit(const uint16_t address);
  uint16_t read16bit(const uint16_t address);

  void write(const uint16_t address, const uint8_t data);
  void write(const uint16_t address, const uint16_t data);

private:
  Cartridge &m_cartridge;
  SerialPort m_serialPort;
}
