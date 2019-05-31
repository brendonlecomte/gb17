#pragma once
#include "BankMemory.h"
#include <stdint.h>

class CartridgeMemory {
public:
  CartridgeMemory(uint8_t* cart_rom, uint16_t banks) : rom(cart_rom, 0x4000, banks) {};
  ~CartridgeMemory(){};
  virtual uint8_t read(const uint16_t addr) { return 0; };
  virtual void write(const uint16_t addr, const uint8_t data) {  };
protected:
  BankMemory rom;
};
