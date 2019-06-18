#pragma once


class Tile {
public:
  Tile(uint8_t* tile_data) : data(tile_data) {};
  ~Tile() {};

  uint8_t getPixel(uint8_t x, uint8_t y) {};
private:
  uint8_t* data; //location of actual data
}

class Sprite {
public:
  Sprite() {};
  ~Sprite() {};

  uint8_t getPixel(uint8_t x, uint8_t y) { return tile.getPixel(x, y); };

private:
  uint8_t x; //minus 8
  uint8_t y; //minus 16
  uint8_t tile_index;
  uint8_t flags;

  Tile tile;
}

class Background {
public:

}
