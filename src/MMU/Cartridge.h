#pragma once
#include <vector>
#include "Cartridge_Header.h"
#include "CartridgeMemory.h"
#include <stdint.h>


class Cartridge {
public:
  Cartridge() {};
  ~Cartridge() {};
  void loadCart(const char* filename);
  uint8_t read(const uint16_t addr) { return memory_controller->read(addr); };
  void write(const uint16_t addr, const uint8_t data) { memory_controller->write(addr, data);  };
  CartridgeHeader& getCartHeader(void) { return cart_header; };

private:
  bool loaded;
  void create(void);
  CartridgeMemory* getMemoryController(void) { return memory_controller; };
  void createController(void);

  CartridgeHeader cart_header; // TODO: make this private.
  CartridgeMemory* memory_controller;
  uint8_t* rom;
  uint8_t* ram;
};
