#pragma once
#include <stdint.h>

#include "cartridge.h"
#include "cpu.h"

class MMU {
public:
  MMU(Cartridge &cart) : m_cart(cart){};
  ~MMU(){};
  uint8_t read8bit(const uint16_t address){m_cart.read(address)};
  uint16_t read16bit(const uint16_t address){};

  void write(const uint16_t address, const uint8_t data){};
  void write(const uint16_t address, const uint16_t data){};

private:
  Cartridge &m_cart;
}
