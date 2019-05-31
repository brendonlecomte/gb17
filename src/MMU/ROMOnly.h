#pragma once
#include <stdint.h>
#include "CartridgeMemory.h"


class RomOnlyCart : public CartridgeMemory {
public:
  RomOnlyCart(uint8_t *rom) : CartridgeMemory(rom, 2) {};

  uint8_t read(const uint16_t addr) {
    //assert read out of range
    return rom[addr];
  };

  void write(const uint16_t addr, const uint8_t data) {};
};
