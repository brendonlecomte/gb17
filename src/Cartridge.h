#pragma once
#include "Cartridge_Header.h"
#include <stdint.h>

class CartridgeMemory {
public:
  CartridgeMemory(){};
  ~CartridgeMemory(){};
  uint8_t read(const uint16_t addr) { return rom[addr]; };
  virtual void write(const uint16_t addr, const uint8_t data) { rom[addr] = data; };

protected:
  uint8_t *rom;
  uint8_t *ram;
};

class RomOnlyCart : public CartridgeMemory {
public:
  RomOnlyCart(const char *filename) {
    rom = new uint8_t[32767];
    ram = NULL;
  }
  uint8_t read(const uint16_t addr) { return rom[addr]; };
  void write(const uint16_t addr, const uint8_t data) { rom[addr] = data; };

};

class Cartridge {
public:
  Cartridge(const char *filename);
  ~Cartridge(){};

  CartridgeMemory *getMemoryController() { return memory_controller; };

  CartridgeHeader cart_header;
private:
  CartridgeMemory *memory_controller;
};
