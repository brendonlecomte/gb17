#pragma once
#include <stdint.h>
#include "../CPU/Interrupts.h"

const uint8_t bg_buffer_width = 160;
const uint8_t bg_buffer_height = 144;

enum class PPUMode : uint8_t {
  HBLANK,
  VBLANK,
  OAM,
  TRANSFER,
};


typedef struct {
  uint8_t data[16];
} sprite_t;

typedef struct {
  union {
    struct {
      uint8_t bg_enable : 1;
      uint8_t sprite_enable : 1;
      uint8_t sprite_size : 1;
      uint8_t bg_tile_map_sel : 1;
      uint8_t tile_map_sel : 1;
      uint8_t win_display_enable : 1;
      uint8_t win_tile_map_sel : 1;
      uint8_t enable : 1;
    } fields;
    uint8_t reg;
  };
} lcdc_reg_t;

typedef struct {
  union {
    struct {
      uint8_t mode : 2;
      uint8_t coincidence : 1;
      uint8_t hblank_int : 1;
      uint8_t vblank_int : 1;
      uint8_t oam_int : 1;
      uint8_t lyc_enable : 1;
    } fields;
    uint8_t reg;
  };
} stat_reg_t;

// $9C00-$9FFF	BG Map Data 2
// $9800-$9BFF	BG Map Data 1
// $8000-$97FF	Character RAM

class PPU {

public:
  PPU(Interrupts &ints) :
      clock_count(0),
      LY(0),
      SCY(0),
      SCX(0),
      LYC(0),
      WY(0),
      WX(0),
      DMA(0), flags(ints){
        STAT.fields.mode = (uint8_t)PPUMode::OAM;
        background_map[0] = &vram[0x1800]; //location in VRAM of BG Map
        background_map[1] = &vram[0x1C00];

        tile_data[0] = (sprite_t*)&vram[0];
        tile_data[1] = (sprite_t*)&vram[0x800]; //signed indexing

        screen_buffer = 0;
      };
  ~PPU(){};

  void reset(void) {
    LY = 0;
    clock_count =0;
    LY = 0;
    SCY = 0;
    SCX = 0;
    LYC = 0;
    WY = 0;
    WX = 0;
    STAT.reg = (uint8_t)PPUMode::OAM;
    LCDC.reg = 0;
  }

  void setScreenBuffer(uint16_t* buff) { screen_buffer = buff; };
  bool update(uint32_t clocks);

  void writeVram(const uint16_t addr, const uint8_t data);
  uint8_t readVram(const uint16_t addr);

  void writeSprite(const uint16_t addr, const uint8_t data);
  uint8_t readSprite(const uint16_t addr);

  void writeRegister(const uint16_t addr, uint8_t data);
  uint8_t readRegister(const uint16_t addr);
  void drawTilePixel(sprite_t* sprite, uint16_t* buffer, const uint8_t x, const uint8_t y);

private:
  void oamSearch();
  void pixelTransfer();
  void hblank();
  void vblank();

  uint16_t* getLineBuffer(uint8_t y);
  uint8_t getBGTileIndex(uint8_t x, uint8_t y);
  sprite_t* getTileSprite(uint8_t tile_index);
  void drawBGLine(const uint8_t ly);


  Interrupts &flags;
  lcdc_reg_t LCDC;
  stat_reg_t STAT;
  uint8_t LY;
  uint8_t SCY; //scroll Y
  uint8_t SCX; //scroll X
  uint8_t LYC;
  uint8_t WY;
  uint8_t WX; // (minus 7) TODO: make this better, minus 7 bit
  uint8_t DMA;

  bool vlanked;
  uint16_t* screen_buffer;
  uint16_t bg_buffer[bg_buffer_width*bg_buffer_height]; //buffer for the full bg map
  uint8_t vram[0x2000]; //8k VRAM starts at 0x8000
  sprite_t* tile_data[2];
  uint8_t* background_map[2];
  uint8_t sprites[0xA0];

  uint32_t clock_count;
};
