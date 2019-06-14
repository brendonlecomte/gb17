#pragma once

#include <stdint.h>

class Register{
  public:
      Register(uint8_t val = 0) : m_val(val) {}
      // Register(const uint8_t val) : m_val(val) {}
      ~Register() {};

      Register& operator =(const uint8_t& value) { set(value); return *this; };
      operator uint8_t()  { return value(); };
      void set(uint8_t new_value) { m_val = new_value; };
      void reset() { m_val = 0x00; };
      uint8_t value() const { return m_val; };
  protected:
      uint8_t m_val;
};



class FlagRegister : public Register {
  public:
    // TODO: Replace bit manip with bitset
      FlagRegister& operator =(const uint8_t& value) { set(value); return *this; };
      void set(uint8_t new_value) { m_val = new_value & 0xF0; };
      void set_carry() { m_val |= 0x10; };
      void set_half_carry() { m_val |= 0x20; };
      void set_subtract() { m_val |= 0x40; };
      void set_zero() { m_val |= 0x80; };

      void clear_carry() { m_val &= ~0x10; };
      void clear_half_carry() { m_val &= ~0x20; };
      void clear_subtract() { m_val &= ~0x40; };
      void clear_zero() { m_val &= ~0x80; };

      bool carry() { return (m_val & 0x10) ? 1 : 0; };
      bool half_carry() { return (m_val & 0x20) ? 1 : 0; };
      bool subtract() { return (m_val & 0x40) ? 1 : 0; };
      bool zero() { return (m_val & 0x80) ? 1 : 0; };
};

class RegisterPair {
  public:
      RegisterPair(Register& high, Register& low) : low_byte(low), high_byte(high) {};
      ~RegisterPair() {};

      RegisterPair& operator=(const uint16_t& value) { set(value); return *this; };
      operator uint16_t() { return value(); };

      void set(uint16_t word) { low_byte.set(word&0xFF); high_byte.set(word>>8); };
      uint16_t value() { return high_byte.value() << 8 | low_byte.value(); };

      uint8_t low() { return low_byte.value(); };
      uint8_t high(){ return high_byte.value(); };
      void reset() { low_byte.reset(); high_byte.reset(); }

  private:
      Register& low_byte;
      Register& high_byte;
};
