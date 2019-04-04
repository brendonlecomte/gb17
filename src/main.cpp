#include <iostream>
#include "alu.h"
#include "cartridge.h"
#include "cpu.h"

int main(int argc, char** argv){
  uint8_t a, b, c;
  uint8_t res = 66;
  a = 5;
  b = 5;
  Cartridge game_cart = Cartridge();
  CPU cpu = CPU();
  std::cout << "test AF: " << std::hex << unsigned(cpu.AF) << std::endl;
  cpu.A = 0xF;
  cpu.add_a(0x01);
  std::cout << "test AF: " << std::hex << unsigned(cpu.AF) << std::endl;
  cpu.add_a(2);
  std::cout << "test A: " << std::hex << unsigned(cpu.A) << std::endl;
  std::cout << "test AF: " << std::hex << unsigned(cpu.AF) << std::endl;
  cpu.F.set_carry();
  std::cout << "test F: " << std::hex << unsigned(cpu.F) << std::endl;
  cpu.F.set_zero();
  std::cout << "test F: " << std::hex << unsigned(cpu.F) << std::endl;
  std::cout << "test AF: " << std::hex << unsigned(cpu.AF) << std::endl;
  game_cart.write(0, 0xAA);
  std::cout << "test cart: " << std::hex << unsigned(game_cart.read(0)) << std::endl;
  return 0;
}
