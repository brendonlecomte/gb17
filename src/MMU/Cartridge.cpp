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

Cartridge::Cartridge(const char* filename) {
  std::ifstream file(filename, std::ios::binary);

// Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  // get its size:
  std::streampos fileSize;

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // reserve capacity
  rom.resize(fileSize);

  // read the data:
  rom.insert(rom.begin(),
             std::istream_iterator<uint8_t>(file),
             std::istream_iterator<uint8_t>());

  cart_header.populate(rom);

  memory_controller = Create(cart_header.getCartType(), rom);
}
