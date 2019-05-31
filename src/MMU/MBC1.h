#pragma once
#include <stdint.h>
#include "CartridgeMemory.h"

class MBC1 : public CartridgeMemory {
public:
  MBC1(uint8_t *rom, uint16_t banks) : CartridgeMemory(rom, banks),
                                       ram_bank(1),
                                       mem_mode_select(0),
                                       ram_enabled(0) {};

  uint8_t read(const uint16_t addr) {
    switch(addr) {
      case 0x0000 ... 0x7FFF: // ROM Bank 00 (Read Only)
      {
        return rom[addr];
      }
      case 0xA000 ... 0xBFFF: // RAM Bank 00-03, if any (Read/Write)
      {
        break;
      }
    }
    assert(0);
    return 0;
  };

  void write(const uint16_t addr, const uint8_t data) {
    switch(addr) {
      case 0x0000 ... 0x1FFF: //- RAM Enable (Write Only)
      {
        if(data == 0x0A) ram_enabled = 1;
        else if(data == 0x00) ram_enabled = 0;
        break;
      }
      case 0x2000 ... 0x3FFF: //- ROM Bank Number (Write Only)
      {
        // assert(0);
        break;
      }
      case 0x4000 ... 0x5FFF: //- RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
      {
        // assert(0);
        break;
      }
      case 0x6000 ... 0x7FFF: //- ROM/RAM Mode Select (Write Only)
      {
        // assert(0);
        break;
      }
      case 0xA000 ... 0xBFFF: //- RAM Bank 00-03, if any (Read/Write)
      {
        // assert(0);
        break;
      }
    }
  };

private:
  std::vector<uint8_t> ram;
  uint8_t ram_bank;
  bool mem_mode_select;
  bool ram_enabled;
};
