#include <iostream>
#include "alu.h"
#include "cartridge.h"
#include "cpu.h"

int main(int argc, char** argv){
  uint8_t a, b, c;
  uint8_t res = 66;
  a = 5;
  b = 5;
  Cartridge game_cart = Cartridge("../roms/Tetris.gb");
  // game_cart.write(0, 0xAA);
  std::cout << game_cart.cart_header << std::endl;
  return 0;
}
