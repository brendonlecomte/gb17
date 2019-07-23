#include "PPU.h"
#include <string.h>
#include <stdio.h>

const uint16_t OAM_CLOCKS = (80 / 4);
const uint16_t TRANSFER_CLOCKS =  (172 / 4);
const uint16_t HBLANK_CLOCKS =  (204 / 4);
const uint16_t LINE_CLOCKS =  (OAM_CLOCKS + TRANSFER_CLOCKS + HBLANK_CLOCKS);
const uint8_t SCREEN_LINE_END = 144;
const uint8_t VBLANK_LINE_END = 250;

const uint8_t bg_tile_map_width = 32;

sprite_t* PPU::getTileSprite(uint8_t tile_index){
  return &(tile_data[0])[tile_index]; //get the sprite using the index
}

uint8_t PPU::getBGTileIndex(uint8_t x, uint8_t y) {
  uint8_t y_tile = y >> 3;
  uint8_t x_tile = x >> 3; //figure out which tile column we are using
  uint16_t t_offset = (y_tile * bg_tile_map_width) + x_tile; //y=0 [0 ... 32], y=1 [1...64]
  return (background_map[0])[t_offset]; //get the tile index from the bg map
}

uint16_t* PPU::getLineBuffer(uint8_t y) {
  return &bg_buffer[(bg_buffer_width*y)];
}


void PPU::drawBGLine(const uint8_t ly) {
  for(uint8_t x = 0; x != bg_buffer_width; x++) { //0 ... 255
    uint16_t tile_index = getBGTileIndex(((x+SCX)%256), ((ly+SCY)%256));
    sprite_t* tile_sprite = getTileSprite(tile_index);
    drawTilePixel(tile_sprite, getLineBuffer(ly), x, ly); //draw the sprite by line
  }
}

const uint16_t lut[4] = {0xFFFF, 0x6969, 0xa9a9, 0x00};

void PPU::drawTilePixel(sprite_t* sprite, uint16_t* buffer, const uint8_t x, const uint8_t y) {
  //draw the sprite into the buffer starting at x and y

  uint8_t y_in_tile = (y%8)* 2; // >> 3 gives 0 .. 31 from 0 .. 255
  uint8_t x_in_tile = (x%8);
  uint8_t y_even = y_in_tile; //double it because we only use the even indexes
  uint8_t y_odd = y_even + 1;

  uint8_t bit_mask = 0x80 >> x_in_tile;
  uint8_t bit_a = !!(sprite->data[y_even] & bit_mask);
  uint8_t bit_b = !!(sprite->data[y_odd] & bit_mask);
  uint8_t pixel = (bit_a << 1) | bit_b;

  buffer[x] = lut[pixel];
}


bool PPU::update(uint32_t clocks) {
  clock_count += clocks;

  switch((PPUMode)(STAT.fields.mode)) {
    case PPUMode::OAM:
      if(clock_count >= OAM_CLOCKS) {
        oamSearch();
        STAT.fields.mode = (uint8_t)PPUMode::TRANSFER;
        clock_count = 0;
      }
      break;
    case PPUMode::TRANSFER:
      if(clock_count >= TRANSFER_CLOCKS) {
        pixelTransfer();
        STAT.fields.mode = (uint8_t)PPUMode::HBLANK;
        clock_count = 0;
      }
      break;
    case PPUMode::HBLANK:
      if(clock_count >= HBLANK_CLOCKS) {
        hblank();
        LY++;
        if(LY <  SCREEN_LINE_END) STAT.fields.mode = (uint8_t)PPUMode::OAM;
        if(LY >= SCREEN_LINE_END) STAT.fields.mode = (uint8_t)PPUMode::VBLANK;
        clock_count = 0;
      }
      break;
    case PPUMode::VBLANK:
      if(clock_count >= LINE_CLOCKS) {
        LY++;
        clock_count = 0;
      }
      if(LY >= VBLANK_LINE_END) {
        vblank();
        STAT.fields.mode = (uint8_t)PPUMode::OAM;
        clock_count = 0;
        LY = 0;
        return true;
      }
      break;
  }
  return false;
}

void PPU::oamSearch() {
  //find all the sprites for this line
}

void PPU::pixelTransfer() {
  //Draw background for line
  drawBGLine(LY);

  //Draw sprites on the line

  //Draw the window on the line
}

void PPU::hblank() {
  //maybe do more pixel transfer here?
}

void PPU::vblank() {
  //oam dma transfer


  //tfr to screen buffer
  if(screen_buffer != 0) {
    memcpy(screen_buffer, bg_buffer, bg_buffer_width*bg_buffer_height*sizeof(uint16_t));
  }
}


/* PUBLIC API */

void PPU::writeVram(const uint16_t addr, const uint8_t data) {
  // if((PPUMode)STAT.fields.mode == PPUMode::TRANSFER) return; //handle vram protection
  vram[addr] = data;
}

uint8_t PPU::readVram(const uint16_t addr) {
  if((PPUMode)STAT.fields.mode == PPUMode::TRANSFER) return 0xFF; //handle vram protection
  return vram[addr];
}

void PPU::writeSprite(const uint16_t addr, const uint8_t data){
}

uint8_t PPU::readSprite(const uint16_t addr) {
  return 0;
}

void PPU::writeRegister(const uint16_t addr, uint8_t data) {
  switch(addr) {
    case 0xFF40: // LCDC
      LCDC.reg = data;
      break;
    case 0xFF41: // STAT
      STAT.reg = data;
      break;
    case 0xFF42: // SCY
      SCY = data;
      break;
    case 0xFF43: // SCX
      SCX = data;
      break;
    case 0xFF44: // LY
      // read only
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
}

uint8_t PPU::readRegister(const uint16_t addr) {
  switch(addr) {
    case 0xFF40: // LCDC
      return LCDC.reg;
    case 0xFF41: // STAT
      return STAT.reg;
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
}
