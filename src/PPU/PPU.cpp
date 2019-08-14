#include "PPU.h"
#include <string.h>
#include <iostream>
#include <stdio.h>

const uint16_t OAM_CLOCKS = (80 / 4);
const uint16_t TRANSFER_CLOCKS =  (172 / 4);
const uint16_t HBLANK_CLOCKS =  (204 / 4);
const uint16_t LINE_CLOCKS =  (OAM_CLOCKS + TRANSFER_CLOCKS + HBLANK_CLOCKS);
const uint8_t SCREEN_LINE_END = 144;
const uint8_t VBLANK_LINE_END = 250;
const uint8_t bg_tile_map_width = 32;

const uint16_t lut[4] = {0xFFFF, 0x6969, 0xa9a9, 0x00};


tile_t* PPU::getTileData(int8_t tile_index){
  //Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 
  // 1=8000-8FFF) access unsigned 0 - 255?
  uint8_t index;
  index = (uint8_t)(tile_index);
  return &(tile_data[0])[index]; //get the sprite using the index
}

int8_t PPU::getBGTileIndex(uint8_t x, uint8_t y) {
  uint8_t y_tile = y >> 3;
  uint8_t x_tile = x >> 3; //figure out which tile column we are using
  uint16_t t_offset = (y_tile * bg_tile_map_width) + x_tile; //y=0 [0 ... 32], y=1 [1...64]
  return (background_map[LCDC.fields.bg_tile_map_sel])[t_offset]; //get the tile index from the bg map
}

void PPU::drawBGLine(const uint8_t ly) {
  for(uint8_t x = 0; x != bg_buffer_width; x++) { //0 ... 160
    int8_t tile_index = getBGTileIndex(((x+SCX)%256), ((ly+SCY)%256));
    tile_t* tile_sprite = getTileData(tile_index);
    uint8_t pixel = getTilePixel(tile_sprite, ((x+SCX)%256), ((ly+SCY)%256)); //draw the sprite by line
    drawPixelOnScreen(x, ly, lut, pixel);
  }
}

void PPU::drawPixelOnScreen(const uint8_t screen_x, const uint8_t screen_y, const uint16_t* pal, const uint8_t index) {
  uint16_t* buffer = &bg_buffer[(bg_buffer_width * screen_y)];
  buffer[screen_x] = pal[index];
}

uint8_t PPU::getTilePixel(tile_t* tile, const uint8_t tile_x, const uint8_t tile_y) {
  //draw the sprite into the buffer starting at x and y
  uint8_t y_in_tile = (tile_y % 8) * 2; // >> 3 gives 0 .. 31 from 0 .. 255
  uint8_t x_in_tile = (tile_x % 8);
  uint8_t y_even = y_in_tile; //double it because we only use the even indexes
  uint8_t y_odd = y_even + 1;

  uint8_t bit_mask = 0x80 >> x_in_tile;
  uint8_t bit_a = !!(tile->data[y_even] & bit_mask);
  uint8_t bit_b = !!(tile->data[y_odd] & bit_mask);
  uint8_t pixel = (bit_a << 1) | bit_b;

  return pixel;
}

void PPU::drawWindowLine(const uint8_t ly) {
}

void PPU::drawSpriteLine(const uint8_t ly) {
  for(auto sp = found_sprites.begin(); sp != found_sprites.end(); sp++) {
    sprite_t* i = &sprites[*sp];
    int8_t tile_index = i->info.index;
    tile_t* tile_sprite = getTileData(tile_index); // TODO this one accesses 0x8000 -> using unsigned 
    uint8_t tile_y = ly-(i->info.y-16);
    for(uint8_t tile_x = 0; tile_x < 8; tile_x++) {
      uint8_t screen_x = tile_x + (i->info.x-8);
      if(screen_x > 160) break; //bail if part of the sprite is off screen
      //need something for tiles the are part on the screen
      uint8_t index = getTilePixel(tile_sprite, tile_x, tile_y); //draw the sprite by line
      if(index != 0)
      {
        drawPixelOnScreen(screen_x, ly, lut, index);
      }
    }
  } 
}

bool PPU::update(uint32_t clocks) {
  clock_count += clocks;

  switch((PPUMode)(STAT.fields.mode)) {
    case PPUMode::OAM:
      if(clock_count >= OAM_CLOCKS) {
        found_sprites = oamSearch((sprite_t*)oam, LY);
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

std::vector<uint8_t> PPU::oamSearch(const sprite_t* oam, const uint8_t ly) {
  // uint8_t count = 0;
  //find all the sprites for this line
  //FE00-FE9F search space. 4byte values. 40 total
  std::vector<uint8_t> active_sprites;
  for(uint8_t i = 0; i < 40; i++) {
   if(active_sprites.size() >= 10) break;
   if(oam[i].info.x == 0) continue; //X is outside the screen 
   int16_t y_min = oam[i].info.y - 16;
   int16_t y_max = oam[i].info.y - 8;
   if(y_min <= ly && y_max > ly) {
     active_sprites.push_back(i);
   }
  }
  return active_sprites; 
}

void PPU::pixelTransfer() { 
  //Draw background for line
  drawBGLine(LY);

  //Draw the window on the line
  if(LCDC.fields.win_display_enable) {
    drawWindowLine(LY);
  }

  //Draw sprites on the line
  if(LCDC.fields.sprite_enable) {
    drawSpriteLine(LY);
  }
  found_sprites.clear();
}

void PPU::hblank() {
  //maybe do more pixel transfer here?

}

void PPU::vblank() {
  //oam dma transfer
  flags.setInterrupt(Interrupt::VBlank);

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
  oam[addr] = data;
}

uint8_t PPU::readSprite(const uint16_t addr) {
  return oam[addr];
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
