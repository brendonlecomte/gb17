#include "Cartridge.h"
#include "Cartridge_Header.h"
#include "MBC1.h"
#include "MBC3_ram_battery.h"
#include "ROMOnly.h"
#include <assert.h>
#include <iterator>

const uint16_t rom_bank = 0x4000;

void Cartridge::createController(void) {
  switch (cart_header.getCartType()) {
    case CartType::ROM_ONLY:
      memory_controller = new RomOnlyCart(rom);
      break;
    case CartType::MBC3_RAM_BATTERY:
      memory_controller = 
        new MBC3_ram_batt(rom, cart_header.getRomBanks(), ram, cart_header.getRamSize(), cart_header.getRamBanks());
      break;
    case CartType::MBC1:
      memory_controller =
          new MBC1(rom, cart_header.getRomBanks(), ram, cart_header.getRamSize(), cart_header.getRamBanks());
    default:
      break;
  }
}

void Cartridge::loadCart(const char *filename) {
  std::ifstream file(filename, std::ifstream::in);
  // ensure file exists
  if (!file)
    assert(0);

  // Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  // Create the CartType
  file.seekg(0, std::ios::beg);
  file.seekg(0x100);
  cart_header.populate(file); // TODO: replace this with using the rom array post read.

  file.seekg(0, std::ios::end);
  uint32_t length = file.tellg();
  rom = new uint8_t[length]; // create rom memory the same size as the cart

  file.seekg(0, std::ios::beg);
  file.read((char *)rom, length); // populate the array

  ram = new uint8_t[cart_header.getRamSize()];
  createController();
  loaded = true;
}
