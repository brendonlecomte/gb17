#include <iostream>
#include "alu.h"
#include "Cartridge.h"
#include "cpu.h"
#include "mmu.h"

int main(int argc, char** argv){
  uint8_t a, b, c;
  uint8_t res = 66;
  a = 5;
  b = 5;
  Cartridge game_cart = Cartridge("../roms/Tetris.gb");
  MMU memory_manager = MMU(game_cart.getMemoryController());
  // game_cart.write(0, 0xAA);
  std::cout << game_cart.cart_header << std::endl;

  memory_manager.write(0, (uint8_t)0xAA);
  memory_manager.write(10,(uint16_t) 0x5555);

  std::cout << std::hex << unsigned(memory_manager.read8bit(0)) << std::endl;
  std::cout << std::hex << unsigned(memory_manager.read16bit(10)) << std::endl;
  //
  // memory_manager.write(0xFF01, (uint8_t)'T');

  std::cout << std::endl;
  return 0;
}
