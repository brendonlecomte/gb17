#include "cartridge.h"
#include <assert.h>
#include <fstream>

// CartridgeMemory *CartridgeMemory::Create(cart_type_e type) {
//   switch (type) {
//     case ROM_ONLY:
//       return new CartridgeMemory();
//     default:
//       assert(0);
//   }
//   return NULL;
// }

std::string CartridgeHeader::getTypeStr() const {
  switch (this->cart_type) {
    case ROM_ONLY:
      return "ROM_ONLY";
    case MBC1:
      return "MBC1";
    case MBC1_RAM:
      return "MBC1_RAM";
    case MBC1_RAM_BATTERY:
      return "MBC1_RAM_BATTERY";
    case MBC2:
      return "MBC2";
    case MBC2_BATTERY:
      return "MBC2_BATTERY";
    case ROM_RAM:
      return "ROM_RAM";
    case ROM_RAM_BATTERY:
      return "ROM_RAM_BATTERY";
    case MMM01:
      return "MMM01";
    case MMM01_RAM:
      return "MMM01_RAM";
    case MMM01_RAM_BATTERY:
      return "MMM01_RAM_BATTERY";
    case MBC3_TIMER_BATTERY:
      return "MBC3_TIMER_BATTERY";
    case MBC3_TIMER_RAM_BATTERY:
      return "MBC3_TIMER_RAM_BATTERY";
    case MBC3:
      return "MBC3";
    case MBC3_RAM:
      return "MBC3_RAM";
    case MBC3_RAM_BATTERY:
      return "MBC3_RAM_BATTERY";
    case MBC4:
      return "MBC4";
    case MBC4_RAM:
      return "MBC4_RAM";
    case MBC4_RAM_BATTERY:
      return "MBC4_RAM_BATTERY";
    case MBC5:
      return "MBC5";
    case MBC5_RAM:
      return "MBC5_RAM";
    case MBC5_RAM_BATTERY:
      return "MBC5_RAM_BATTERY";
    case MBC5_RUMBLE:
      return "MBC5_RUMBLE";
    case MBC5_RUMBLE_RAM:
      return "MBC5_RUMBLE_RAM";
    case MBC5_RUMBLE_RAM_BATTERY:
      return "MBC5_RUMBLE_RAM_BATTERY";
    case CAMERA:
      return "CAMERA";
    case BANDAI:
      return "BANDAI";
    case HUC3:
      return "HUC3";
    case HUC1_MBC_RAM_BATTERY:
      return "HUC1_MBC_RAM_BATTER";
  }
  return "ERROR";
}

std::string CartridgeHeader::getRomSizeStr() const {
  switch (this->rom_size) {
    case ROM_32KByte:
      return "32KByte ";
    case ROM_64KByte:
      return "64KByte ";
    case ROM_128KByte:
      return "128KByte";
    case ROM_256KByte:
      return "256KByte";
    case ROM_512KByte:
      return "512KByte";
    case ROM_1MByte:
      return "1MByte";
    case ROM_2MByte:
      return "2MByte";
    case ROM_4MByte:
      return "4MByte";
    case ROM_1_1MByte:
      return "1.1MByte";
    case ROM_1_2MByte:
      return "1.2MByte";
    case ROM_1_5MByte:
      return "1.55MByte";
  }
  return "ERROR";
}

std::string CartridgeHeader::getRamSizeStr() const {
  switch (this->ram_size) {
    case RAM_None:
      return "None";
    case RAM_2KBytes:
      return "2KBytes";
    case RAM_8Kbytes:
      return "8Kbytes";
    case RAM_32KBytes:
      return "32KBytes";
  }
  return "ERROR";
}

std::string CartridgeHeader::getDestStr() const {
  switch (this->dest_code) {
    case Japanese:
      return "Japanese";
    case Non_Japanese:
      return "Non_Japanese";
  }
  return "ERROR";
}

void CartridgeHeader::print(std::ostream &out) const {
  out << "Title: " << title << std::endl;
  out << "Type: " << CartridgeHeader::getTypeStr() << std::endl;
  out << "Rom Size: " << CartridgeHeader::getRomSizeStr() << std::endl;
  out << "Ram Size: " << CartridgeHeader::getRamSizeStr() << std::endl;
  out << "Dest: " << CartridgeHeader::getDestStr() << std::endl;
}

std::ostream &operator<<(std::ostream &out, const CartridgeHeader &c) {
  c.print(out);
  return out;
}

std::istream &operator>>(std::istream &in, CartridgeHeader &c) {
  in.read((char *)&c.entry[0], sizeof(c.entry));
  in.read((char *)&c.logo[0], sizeof(c.logo));
  in.read(&c.title[0], sizeof(c.title));
  in.read((char *)&c.cgb_flag, sizeof(c.cgb_flag));
  in.read((char *)&c.licensee, sizeof(c.licensee));
  in.read((char *)&c.sgb_flag, sizeof(c.sgb_flag));
  in.read((char *)&c.cart_type, sizeof(c.cart_type));
  in.read((char *)&c.rom_size, sizeof(c.rom_size));
  in.read((char *)&c.ram_size, sizeof(c.ram_size));
  in.read((char *)&c.dest_code, sizeof(c.dest_code));
  in.read((char *)&c.old_licensee, sizeof(c.old_licensee));
  in.read((char *)&c.version, sizeof(c.version));
  in.read((char *)&c.header_checksum, sizeof(c.header_checksum));
  in.read((char *)&c.checksum, sizeof(c.checksum));
  return in;
}

Cartridge::Cartridge(const char *filename) {
  std::ifstream f(filename, std::ifstream::binary);
  std::cout << filename << std::endl;
  f.seekg(0x100);
  f >> cart_header;
  f.close();
}
