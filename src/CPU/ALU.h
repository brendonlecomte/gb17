#pragma once
#include "register.h"
#include <stdint.h>

class ALU {
public:
  ALU(FlagRegister &flag) : m_flags(flag){};
  ~ALU(){};

  uint8_t add(const uint8_t a, const uint8_t b, const uint8_t carry);
  uint16_t add(const uint16_t a, const uint16_t b, const uint8_t carry);

  uint8_t sub(const uint8_t a, const uint8_t b, const uint8_t carry);
  uint16_t sub(const uint16_t a, const uint16_t b);

  uint8_t inc(const uint8_t reg);
  uint16_t inc(const uint16_t reg);

  uint8_t shiftLeft(const uint8_t reg);
  uint8_t shiftRight(const uint8_t reg);

  uint8_t rotateLeft(const uint8_t reg);
  uint8_t rotateRight(const uint8_t reg);

private:
  FlagRegister &m_flags; // cpu flags to use for everything
};
