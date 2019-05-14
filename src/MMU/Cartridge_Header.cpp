#include "Cartridge_Header.h"
#include <assert.h>
#include <algorithm>

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

void CartridgeHeader::populate(std::istream &in) {
  in.read((char *)&entry[0], sizeof(entry));
  in.read((char *)&logo[0], sizeof( logo));
  in.read(&title[0], sizeof(title));
  in.read((char *)&cgb_flag, sizeof( cgb_flag));
  in.read((char *)&licensee, sizeof( licensee));
  in.read((char *)&sgb_flag, sizeof( sgb_flag));
  in.read((char *)&cart_type, sizeof( cart_type));
  in.read((char *)&rom_size, sizeof( rom_size));
  in.read((char *)&ram_size, sizeof( ram_size));
  in.read((char *)&dest_code, sizeof( dest_code));
  in.read((char *)&old_licensee, sizeof( old_licensee));
  in.read((char *)&version, sizeof( version));
  in.read((char *)&header_checksum, sizeof( header_checksum));
  in.read((char *)&checksum, sizeof( checksum));
}

void CartridgeHeader::populate(std::vector<uint8_t> &rom) {
  std::copy(rom.begin() + 0x100, rom.begin() + 0x103, entry);
  std::copy(rom.begin() + 0x104, rom.begin() + 0x133, logo);
  std::copy(rom.begin() + 0x134, rom.begin() + 0x142, title);
  cgb_flag = rom[0x143];
  licensee = (uint16_t)rom[0x144];
  sgb_flag = rom[0x146];
  cart_type = rom[0x147];
  rom_size = rom[0x148];
  ram_size = rom[0x149];
  // std::copy(rom.begin() + 0x134, rom.begin() + 0x142, dest_code);
  // std::copy(rom.begin() + 0x134, rom.begin() + 0x142, old_licensee);
  version = rom[0x14D];
  // std::copy(rom.begin() + 0x14E, rom.begin() + 0x14F, checksum);
}

std::string CartridgeHeader::getTitleStr(void) const{
  std::string s = std::string(title);
  return s;
}

std::ostream &operator<<(std::ostream &out, const CartridgeHeader &c) {
  c.print(out);
  return out;
}

std::istream &operator>>(std::istream &in, CartridgeHeader &c) {
  c.populate(in);
  return in;
}
