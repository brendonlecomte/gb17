#pragma once
#include <stdint.h>
#include <vector>

class Cartridge {
public:
  uint8_t read(const uint16_t addr) { return rom.at(addr); };
  virtual void write(const uint16_t addr, const uint8_t data) { rom.at(addr) = data; };

protected:
  std::vector<uint8_t> rom = {0, 0, 0};
  std::vector<uint8_t> ram;
};
