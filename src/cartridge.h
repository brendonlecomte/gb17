#pragma once
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

// gameboy loads..
// takes file. checks cart type.
// generates Cartrigde object. MMC1...5

enum cart_type_e {
  ROM_ONLY = 0,
  MBC1 = 0x01,
  MBC1_RAM = 0x02,
  MBC1_RAM_BATTERY = 0x03,
  MBC2 = 0x05,
  MBC2_BATTERY = 0x06,
  ROM_RAM = 0x08,
  ROM_RAM_BATTERY = 0x09,
  MMM01 = 0x0B,
  MMM01_RAM = 0x0C,
  MMM01_RAM_BATTERY = 0x0D,
  MBC3_TIMER_BATTERY = 0x0F,
  MBC3_TIMER_RAM_BATTERY = 0x10,
  MBC3 = 0x11,
  MBC3_RAM = 0x12,
  MBC3_RAM_BATTERY = 0x13,
  MBC4 = 0x15,
  MBC4_RAM = 0x16,
  MBC4_RAM_BATTERY = 0x17,
  MBC5 = 0x19,
  MBC5_RAM = 0x1A,
  MBC5_RAM_BATTERY = 0x1B,
  MBC5_RUMBLE = 0x1C,
  MBC5_RUMBLE_RAM = 0x1D,
  MBC5_RUMBLE_RAM_BATTERY = 0x1E,
  CAMERA = 0xFC,
  BANDAI = 0xFD,
  HUC3 = 0xFE,
  HUC1_MBC_RAM_BATTERY = 0xFF
};

enum rom_size_e {
  ROM_32KByte = 0x00,
  ROM_64KByte = 0x01,
  ROM_128KByte = 0x02,
  ROM_256KByte = 0x03,
  ROM_512KByte = 0x04,
  ROM_1MByte = 0x05,
  ROM_2MByte = 0x06,
  ROM_4MByte = 0x07,
  ROM_1_1MByte = 0x52,
  ROM_1_2MByte = 0x53,
  ROM_1_5MByte = 0x54,
};

enum ram_size_e {
  RAM_None,
  RAM_2KBytes,
  RAM_8Kbytes,
  RAM_32KBytes,
};

enum dest_code_e { Japanese, Non_Japanese };

class CartridgeHeader {
public:
  CartridgeHeader(){};
  ~CartridgeHeader(){};
  void print(std::ostream &out) const;

  friend std::ostream &operator<<(std::ostream &out, const CartridgeHeader &c);
  friend std::istream &operator>>(std::istream &in, CartridgeHeader &c);

private:
  std::string getTypeStr() const;
  std::string getRomSizeStr() const;
  std::string getRamSizeStr() const;
  std::string getDestStr() const;

  uint8_t entry[0x04];  // 0x100-103
  uint8_t logo[0x30];   // 0x104 - 0x133
  char title[0x0F];     // 0x134 - 0x143
  uint8_t cgb_flag;     // 0x143
  uint16_t licensee;    // 0x144 - 0x145
  uint8_t sgb_flag;     // 0x146
  uint8_t cart_type;    // 0x147
  uint8_t rom_size;     // 0x148
  uint8_t ram_size;     // 0x149
  uint8_t dest_code;    /// 0x14A
  uint8_t old_licensee; // 0x14B
  uint8_t version;      // 0x14D
  uint8_t header_checksum;
  uint16_t checksum; // 0x14E - 0x14F
};

class CartridgeMemory {
public:
  CartridgeMemory(const char *filename){

  };
  ~CartridgeMemory(){};
  uint8_t read(const uint16_t addr) { return rom.at(addr); };
  virtual void write(const uint16_t addr, const uint8_t data) { rom.at(addr) = data; };

protected:
  std::vector<uint8_t> rom;
  std::vector<uint8_t> ram;
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
