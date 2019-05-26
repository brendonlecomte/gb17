#include "MMU.h"
#include "boot.h"
#include <assert.h>

uint8_t MMU::read8bit(const uint16_t address) {
  switch (address) {
    case 0x0000 ... 0x7FFF:
      if(*boot == 0 && address < 0x100) {
        return bootDMG[address];
      }
      // 0 - 0x3FFF 16KB ROM Bank 00     (in cartridge, fixed at bank 00)
      // 0x4000 - 0x7fff 16KB ROM Bank 01 ... NN (in cartridge, switchable bank number)
      return m_cartridge->read(address);
    case 0x8000 ... 0x9FFF:
      // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
      return vram[address & (~0x8000)];
      break;
    case 0xA000 ... 0xBFFF:
      // 8KB External RAM     (in cartridge, switchable bank, if any)
      return m_cartridge->read(address);
      break;
    case 0xC000 ... 0xCFFF:
      // 4KB Work RAM Bank 0 (WRAM)
      return wram[address & (~0xC000)];
      break;
    case 0xD000 ... 0xDFFF:
      // 4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
      return wram[address & (~0xD000)];
      break;
    case 0xE000 ... 0xFDFF:
      // Same as C000-DFFF (ECHO)    (typically not used)
      return echo[address & (~0xE000)];
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
      return m_serialPort.read();
      break;
    case 0xFF02: // Serial Control ... .
      return m_serialPort.getControl();
      break;
    case 0xFF03 ... 0xFF7F:
      // I/O Ports
      return io[((uint8_t)address & (~0xFF00))];
      break;
    case 0xFF80 ... 0xFFFF:
      // High RAM (HRAM)
      return hram[((uint8_t)address & (~0xFF80))];
    default:

      break;
  }
  std::cout << "0x" << std::hex <<unsigned(address) << std::endl;
  assert(0);
  return 0;
}

uint16_t MMU::read16bit(const uint16_t address) { return ((read8bit(address +1 ) << 8) | read8bit(address)); }

void MMU::write(const uint16_t address, const uint8_t data) {
  switch (address) {
    case 0x0000 ... 0x7FFF:
      // 0 - 0x3FFF 16KB ROM Bank 00     (in cartridge, fixed at bank 00)
      // 0x4000 - 0x7fff 16KB ROM Bank 01 ... NN (in cartridge, switchable bank number)
      m_cartridge->write(address, data);
      break;
    case 0x8000 ... 0x9FFF:
      // 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
      vram[address & (~0x8000)] = data;
      break;
    case 0xA000 ... 0xBFFF:
      // 8KB External RAM     (in cartridge, switchable bank, if any)
      break;
    case 0xC000 ... 0xCFFF:
      // 4KB Work RAM Bank 0 (WRAM)
      wram[address & (~0xC000)] = data;
      break;
    case 0xD000 ... 0xDFFF:
      // 4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
      wram[address & (~0xD000)] = data;
      break;
    case 0xE000 ... 0xFDFF:
      // Same as C000-DDFF (ECHO)    (typically not used)
      echo[address & (~0xE000)] = data;
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
      m_serialPort.setControl(data);
      break;
    case 0xFF03 ... 0xFF7F:
    {
      // I/O Ports
      uint8_t io_address = ((uint8_t)address & (~0xFF00));
      io[io_address] = data;
      break;
    }
    case 0xFF80 ... 0xFFFF:
    {
      // High RAM (HRAM)
      uint8_t hram_address = ((uint8_t)address & (~0xFF80));
      hram[hram_address] = data;
      break;
    }
    default:
      m_cartridge->write(address, data);
      break;
  }
}

void MMU::write(const uint16_t address, const uint16_t data) {
  write(address + 1, (uint8_t)(data >> 8));
  write(address, (uint8_t)data);
}
