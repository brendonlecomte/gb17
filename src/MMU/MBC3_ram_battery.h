#pragma once
#include "CartridgeMemory.h"
#include <stdint.h>

class MBC3_ram_batt : public CartridgeMemory {
public:
  MBC3_ram_batt(uint8_t *rom, uint16_t rom_banks, uint8_t *ram, uint16_t ram_size, uint16_t ram_banks)
      : CartridgeMemory(rom, rom_banks), ram(ram, ram_size, ram_banks), mem_mode_select(0), ram_enabled(0){};

  uint8_t read(const uint16_t addr) {
    switch (addr) {
      case 0x0000 ... 0x7FFF: // ROM Bank 00 (Read Only)
      {
        return rom[addr];
      }
      case 0xA000 ... 0xBFFF: // RAM Bank 00-03, if any (Read/Write)
      {
        if(!ram_enabled) break;
        if(ram.getBank() <= 3) {
          return ram[addr - 0xA000];
        }
        else if(ram.getBank() >= 0x08 && ram.getBank() <= 0x0C) {
          return clock[ram.getBank() - 0x8];
        }
      }
    }
    return 0;
  };

  void write(const uint16_t addr, const uint8_t data) {
    switch (addr) {
      case 0x0000 ... 0x1FFF: //- RAM and Timer Enable (Write Only)
      {
        if (data == 0x0A)
          ram_enabled = 1;
        else if (data == 0x00)
          ram_enabled = 0;
        break;
      }
      case 0x2000 ... 0x3FFF: //- ROM Bank Number (Write Only)
      {
        uint8_t bank_sel = data & 0x1F;
        if(bank_sel == 0) bank_sel = 1;
        rom.setBank(bank_sel);
        break;
      }
      case 0x4000 ... 0x5FFF: //- RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
      {
        if(mem_mode_select) { //ram banking
          ram.setBank(data & 0x03);
        } else { //rom banking
          //TODO: understand this?
        }
        break;
      }
      case 0x6000 ... 0x7FFF: //- Latch Clock Data
      {
        mem_mode_select = !!data;
        break;
      }
    }
  };

private:
  BankMemory ram;
  bool mem_mode_select;
  bool ram_enabled;
  uint8_t clock[5];
};
