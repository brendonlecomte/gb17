#include "MMU.h"
#include <assert.h>

uint8_t MMU::read8bit(const uint16_t address) {
  switch (address) {
    case 0x0000 ... 0x3FFF:
      // 16KB ROM Bank 00     (in cartridge, fixed at bank 00)
      return m_cartridge->read(address);
    case 0x4000 ... 0x7FFF:
      // 16KB ROM Bank 01 ... NN (in cartridge, switchable bank number)
      break;
    case 0x8000 ... 0x9FFF:
      // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
      break;
    case 0xA000 ... 0xBFFF:
      // 8KB External RAM     (in cartridge, switchable bank, if any)
      break;
    case 0xC000 ... 0xCFFF:
      // 4KB Work RAM Bank 0 (WRAM)
      break;
    case 0xD000 ... 0xDFFF:
      // 4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
      break;
    case 0xE000 ... 0xFDFF:
      // Same as C000-DDFF (ECHO)    (typically not used)
      break;
    case 0xFE00 ... 0xFE9F:
      // Sprite Attribute Table (OAM)
      break;
    case 0xFEA0 ... 0xFEFF:
      // Not Usable
      break;
    case 0xFF00:
      // JOYP
      break;
    case 0xFF01:
      // Serial Data
      break;
    case 0xFF02: // Serial Control ... .
      break;
    case 0xFF03 ... 0xFF7F:
      // I/O Ports
      break;
    case 0xFF80 ... 0xFFFE:
      // High RAM (HRAM)
      return hram[((uint8_t)address & (~0xFF80))];
    case 0xFFFF:
      // Interrupt Enable Register
      break;
    default:

      break;
  }
  assert(address);
  return 0;
}

uint16_t MMU::read16bit(const uint16_t address) { return ((read8bit(address) << 8) | read8bit(address + 1)); }

void MMU::write(const uint16_t address, const uint8_t data) {
  switch (address) {
    case 0x0000 ... 0x3FFF:
      // 16KB ROM Bank 00     (in cartridge, fixed at bank 00)
      break;
    case 0x4000 ... 0x7FFF:
      // 16KB ROM Bank 01 ... NN (in cartridge, switchable bank number)
      break;
    case 0x8000 ... 0x9FFF:
      // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
      break;
    case 0xA000 ... 0xBFFF:
      // 8KB External RAM     (in cartridge, switchable bank, if any)
      break;
    case 0xC000 ... 0xCFFF:
      // 4KB Work RAM Bank 0 (WRAM)
      break;
    case 0xD000 ... 0xDFFF:
      // 4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
      break;
    case 0xE000 ... 0xFDFF:
      // Same as C000-DDFF (ECHO)    (typically not used)
      break;
    case 0xFE00 ... 0xFE9F:
      // Sprite Attribute Table (OAM)
      break;
    case 0xFEA0 ... 0xFEFF:
      // Not Usable
      break;
    case 0xFF00:
      // JOYP
      break;
    case 0xFF01:
      // Serial Data
      m_serialPort.write(data);
      break;
    case 0xFF02: // Serial Control ...
      break;
    case 0xFF03 ... 0xFF7F:
      // I/O Ports
      break;
    case 0xFF80 ... 0xFFFE:
    {
      // High RAM (HRAM)
      uint8_t hram_address = ((uint8_t)address & (~0xFF80));
      hram[hram_address] = data;
      break;
    }
    case 0xFFFF:
      // Interrupt Enable Register
      break;
    default:
      m_cartridge->write(address, data);
      break;
  }
}

void MMU::write(const uint16_t address, const uint16_t data) {
  write(address, (uint8_t)(data >> 8));
  write(address + 1, (uint8_t)data);
}
