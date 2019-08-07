#pragma once
#include <stdint.h>
#include <vector>
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
} tile_t;

typedef struct {
  union {
    struct {
        uint8_t y; //This number is +16 for Y scrolling.
        uint8_t x; //this number is +8 its actual value. To allow X scrolling
        uint8_t index;
        union {
            struct {
              uint8_t obj_priority:1; //(0=OBJ Above BG, 1=OBJ Behind BG color 1-3)
              uint8_t y_flip:1;       //(0=Normal, 1=Vertically mirrored)
              uint8_t x_flip :1;      // (0=Normal, 1=Horizontally mirrored)
              uint8_t palette:1;      //Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
              uint8_t vram_bank:1;    // Tile VRAM-Bank  **CGB Mode Only**     (0=Bank 0, 1=Bank 1)
              uint8_t cbg_palette:3;  //-0 Palette number  **CGB Mode Only**     (OBP0-7)
            } fields;
            uint8_t reg;
        } attributes;
    } info;
    uint8_t data[4];
  };
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

        tile_data[0] = (tile_t*)&vram[0];
        tile_data[1] = (tile_t*)&vram[0x800]; //signed indexing
        sprites = (sprite_t*)&oam[0];
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

  uint8_t* dmaTransferDst(void) { return &oam[0]; };
  uint8_t  dmaStart(void) { return DMA; }

  // Take a X,Y on the screen and draw the pixel using a given palette
  void drawPixelOnScreen(const uint8_t screen_x, const uint8_t screen_y, const uint16_t* pal, const uint8_t index);

  // Return the Tile Pixel Index (0 - 3) from a position within a tile 
  uint8_t getTilePixel(tile_t* tile, const uint8_t tile_x, const uint8_t tile_y);

  // Search the oam for a vector of active sprite indexes
  std::vector<uint8_t> oamSearch(const sprite_t* oam, const uint8_t ly);


private:
  void pixelTransfer();
  void hblank();
  void vblank();

  int8_t getBGTileIndex(uint8_t x, uint8_t y);
  tile_t* getTileData(int8_t tile_index);

  void drawBGLine(const uint8_t ly);
  void drawWindowLine(const uint8_t ly);
  void drawSpriteLine(const uint8_t ly);

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

  uint16_t* screen_buffer;
  uint16_t bg_buffer[bg_buffer_width*bg_buffer_height]; //buffer for the full bg map
  uint8_t vram[0x2000]; //8k VRAM starts at 0x8000
  tile_t* tile_data[2];
  uint8_t* background_map[2];
  uint8_t oam[0xA0];
  sprite_t* sprites;
  std::vector<uint8_t> found_sprites;

  uint32_t clock_count;
};
