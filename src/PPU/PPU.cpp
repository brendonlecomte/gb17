#include "PPU.h"

void PPU::update(uint32_t clocks) {
  clock_count += clocks;
  if (clock_count >= LINE_CLOCKS) {
    current_line += 1;
    clock_count = 0;
  }
  if (current_line >= VBLANK_LINE_END) {
    current_line = 0;
  }
  LY = current_line;
};

void PPU::drawScanLine(uint8_t* line_buffer) {};

void PPU::writeVram(const uint16_t addr, const uint8_t data) {
  vram[addr] = data;
};

uint8_t PPU::readVram(const uint16_t addr) {
  return vram[addr];
};

void PPU::writeRegister(const uint16_t addr, uint8_t data) {
  switch(addr) {
    case 0xFF40: // LCDC
      LCDC = data;
      break;
    case 0xFF41: // STAT
      STAT = data;
      break;
    case 0xFF42: // SCY
      SCY = data;
      break;
    case 0xFF43: // SCX
      SCX = data;
      break;
    case 0xFF44: // LY
      LY = data;
      break;
    case 0xFF45: // LYC
      LYC = data;
      break;
    case 0xFF46:
      DMA = data;
      break;
    case 0xFF4A: // WY
      WY = data;
      break;
    case 0xFF4B: // WX
      WX = data;
      break;
  }
};

uint8_t PPU::readRegister(const uint16_t addr) {
  switch(addr) {
    case 0xFF40: // LCDC
      return LCDC;
    case 0xFF41: // STAT
      return STAT;
    case 0xFF42: // SCY
      return SCY;
    case 0xFF43: // SCX
      return SCX;
    case 0xFF44: // LY
      return LY;
    case 0xFF45: // LYC
      return LYC;
    case 0xFF46:
      return DMA;
    case 0xFF4A: // WY
      return WY;
    case 0xFF4B: // WX
      return WX;
  }
  return 0;
};
