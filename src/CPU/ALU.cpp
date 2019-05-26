#include "ALU.h"

uint8_t ALU::add(const uint8_t a, const uint8_t b, const uint8_t carry) {
  uint16_t temp = a + b + carry;
  uint8_t hc_temp = (a & 0x0F) + (b & 0x0F) + carry;
  uint8_t result = (uint8_t)temp & 0xFF;
  if(hc_temp & 0x10)
  {
    m_flags.set_half_carry();
  } else {
    m_flags.clear_half_carry();
  }

  if(temp & 0x100) {
    m_flags.set_carry();
  } else {
    m_flags.clear_carry();
  }
  return result;
}

uint16_t ALU::add(const uint16_t a, const uint16_t b, const uint8_t carry) {
  uint32_t temp = a + b + carry;
  uint16_t hc_temp = (a & 0x0FFF) + (b & 0x0FFF) + carry;
  uint16_t result = temp&0xFFFF;
  if(hc_temp & 0x1000) {
    m_flags.set_half_carry();
  } else {
    m_flags.clear_half_carry();
  }

  if(temp & 0x10000) {
    m_flags.set_carry();
  } else {
    m_flags.clear_carry();
  }
  return result;
}

uint8_t ALU::sub(const uint8_t a, const uint8_t b) {
  uint16_t temp = a - b;
  uint8_t hc_temp = (a & 0x0F) < (b & 0x0F);
  uint8_t result = (uint8_t)temp & 0xFF;
  if(hc_temp) {
    m_flags.set_half_carry();
  } else {
    m_flags.clear_half_carry();
  }

  if(b > a) {
    m_flags.set_carry();
  } else {
    m_flags.clear_carry();
  }

  m_flags.set_subtract();

  if(result == 0){
    m_flags.set_zero();
  } else {
    m_flags.clear_zero();
  }

  return result;
}

uint16_t ALU::sub(const uint16_t a, const uint16_t b) {
  uint32_t temp = a - b;
  uint16_t hc_temp = (a & 0x0FFF) < (b & 0x0FFF);
  uint16_t result = temp&0xFFFF;
  if(hc_temp) {
    m_flags.set_half_carry();
  } else {
    m_flags.clear_half_carry();
  }

  if(b > a) {
    m_flags.set_carry();
  } else {
    m_flags.clear_carry();
  }

  m_flags.set_subtract();

  if(result == 0){
    m_flags.set_zero();
  } else {
    m_flags.clear_zero();
  }
  return result;
}

uint8_t ALU::inc(const uint8_t reg) {
  uint8_t hc = (reg&0x0F)+1;
  if(reg == 0xFF) {
    return 0;
  }
  if(hc == 0x10) {
    m_flags.set_half_carry();
  }
  return reg + 1;
}

uint16_t ALU::inc(const uint16_t reg) {
  return reg + 1;
}

// void ALU::shiftL
// void ALU::shiftR
//
// void ALU::rotateL // w/ carry?
// void ALU::rotateR // w/ carry?
