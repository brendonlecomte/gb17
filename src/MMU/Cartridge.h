#pragma once
#include <vector>
#include "Cartridge_Header.h"
#include <stdint.h>

class CartridgeMemory {
public:
  CartridgeMemory() {};
  ~CartridgeMemory(){};
  virtual uint8_t read(const uint16_t addr) { return 0; };
  virtual void write(const uint16_t addr, const uint8_t data) {  };
};

class RomOnlyCart : public CartridgeMemory {
public:
  RomOnlyCart(std::vector<uint8_t> &loaded_rom) : rom(loaded_rom)  {};
  uint8_t read(const uint16_t addr) { return rom[addr];  };
  void write(const uint16_t addr, const uint8_t data) {  };
private:
  std::vector<uint8_t> &rom;
};

class MBC1 : public CartridgeMemory {
public:
  MBC1(std::vector<uint8_t> &loaded_rom) : rom(loaded_rom),
                                           ram_bank(1),
                                           rom_bank(1),
                                           mem_mode_select(0),
                                           ram_enabled(0) {};
  uint8_t read(const uint16_t addr);
  void write(const uint16_t addr, const uint8_t data);
private:
  std::vector<uint8_t> &rom;
  std::vector<uint8_t> ram;
  uint8_t ram_bank;
  uint8_t rom_bank;
  bool mem_mode_select;
  bool ram_enabled;
};

class Cartridge {
public:
  Cartridge(const char* filename);
  ~Cartridge(){};
  CartridgeMemory* getMemoryController() { return memory_controller; };
  CartridgeHeader& getCartHeader() { return cart_header; };
CartridgeHeader cart_header;
private:

  CartridgeMemory* memory_controller;
  std::vector<uint8_t> rom;
};
