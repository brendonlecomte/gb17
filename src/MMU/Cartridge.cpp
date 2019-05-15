#include "Cartridge.h"
#include "Cartridge_Header.h"
#include <assert.h>
#include <iterator>

CartridgeMemory* Create(CartType type, std::vector<uint8_t> &loaded_rom) {
  switch (type) {
    case CartType::ROM_ONLY:
      return new RomOnlyCart(loaded_rom);
    case CartType::MBC1:
      return new MBC1(loaded_rom);
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

void MBC1::write(const uint16_t addr, const uint8_t data) {
  switch(addr) {
    case 0x0000 ... 0x1FFF: //- RAM Enable (Write Only)
    {
      break;
    }
    case 0x2000 ... 0x3FFF: //- ROM Bank Number (Write Only)
    {
      break;
    }
    case 0x4000 ... 0x5FFF: //- RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
    {
      break;
    }
    case 0x6000 ... 0x7FFF: //- ROM/RAM Mode Select (Write Only)
    {
      break;
    }
    case 0xA000 ... 0xBFFF: //- RAM Bank 00-03, if any (Read/Write)
    {
      break;
    }
  }
}

uint8_t MBC1::read(const uint16_t addr) {
  switch(addr) {
    case 0x0000 ... 0x3FFF: // ROM Bank 00 (Read Only)
    {
      break;
    }
    case 0x4000 ... 0x7FFF: // ROM Bank 01-7F (Read Only)
    {
      break;
    }
    case 0xA000 ... 0xBFFF: // RAM Bank 00-03, if any (Read/Write)
    {
      break;
    }
  }
  return 0;
}
