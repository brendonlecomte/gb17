#pragma once
#include <stdint.h>
#include "../CPU/Interrupts.h"

#define OAM_CLOCKS (80 / 4)
#define TRANSFER_CLOCKS (172 / 4)
#define HBLANK_CLOCKS (204 / 4)
#define LINE_CLOCKS (OAM_CLOCKS + TRANSFER_CLOCKS + HBLANK_CLOCKS)

const uint8_t SCREEN_LINE_END = 144;
const uint8_t VBLANK_LINE_END = 250;

class PPU {
public:
  PPU(Interrupts &ints)
      : clock_count(0), current_line(0), state(0),
      LY(0),
      SCY(0),
      SCX(0),
      LYC(0),
      WY(0),
      WX(0),
      DMA(0), flags(ints){};
  ~PPU(){};

  void update(uint32_t clocks);

  void drawScanLine(uint8_t* line_buffer);

  void writeVram(const uint16_t addr, const uint8_t data);

  uint8_t readVram(const uint16_t addr);

  void writeRegister(const uint16_t addr, uint8_t data);
  uint8_t readRegister(const uint16_t addr);

private:
  Interrupts &flags;
  uint8_t LCDC;
  uint8_t STAT;
  uint8_t LY;
  uint8_t SCY; //scroll Y
  uint8_t SCX; //scroll X
  uint8_t LYC;
  uint8_t WY;
  uint8_t WX; // (minus 7) TODO: make this better, minus 7 bit
  uint8_t DMA;

  uint8_t vram[0x1FFF]; //8k VRAM
  uint8_t* tile_data;
  uint8_t* background_map;
  uint8_t* sprites;

  uint8_t current_line;
  uint32_t clock_count;
  uint32_t state;
};
