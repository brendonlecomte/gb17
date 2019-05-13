#include "Cartridge.h"
#include "Cartridge_Header.h"
#include <assert.h>

CartridgeMemory* Create(cart_type_e type, std::vector<uint8_t> &loaded_rom) {
  switch (type) {
    case ROM_ONLY:
      return new RomOnlyCart(loaded_rom);
    default:
      // assert(0);
      break;
  }
  return NULL;
}

Cartridge::Cartridge(std::vector<uint8_t> &loaded_rom) : rom(loaded_rom) {
  cart_header.populate(loaded_rom);
  memory_controller = Create(cart_header.getCartType(), loaded_rom);
}
