#include "Cartridge.h"
#include "Cartridge_Header.h"
#include <fstream>
#include <assert.h>

CartridgeMemory *Create(cart_type_e type, const char *filename) {
  switch (type) {
    case ROM_ONLY:
      return new RomOnlyCart(filename);
    default:
      assert(0);
  }
  return NULL;
}

Cartridge::Cartridge(const char *filename) {
  std::ifstream f(filename, std::ifstream::binary);
  f.seekg(0x100); //Cartridge Header starts at 0x100
  f >> cart_header;
  f.close();
  memory_controller = Create(cart_header.getCartType(), filename);
}
