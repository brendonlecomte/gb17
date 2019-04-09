#include "mmu.h"

uint8_t MMU::read8bit(const uint16_t address) { return m_cartridge->read(address); }
uint16_t MMU::read16bit(const uint16_t address) {
  return ((m_cartridge->read(address) << 8) | m_cartridge->read(address + 1));
}

void MMU::write(const uint16_t address, const uint8_t data) {
  switch(address) {

    case 0xFF01:
      m_serialPort.write(data);
    default:
      m_cartridge->write(address, data);
      break;
  }
}

void MMU::write(const uint16_t address, const uint16_t data) {
  m_cartridge->write(address, data >> 8);
  m_cartridge->write(address + 1, data);
}
